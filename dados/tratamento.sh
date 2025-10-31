#!/bin/bash

echo "Criando diretórios necessários..."
mkdir -p sql csv_tratado/ano csv_tratado/estado csv_tratado/estado_2324

if [ ! -f csv_bruto/merged_focos.csv ]; then
    echo "Erro: O arquivo CSV bruto não foi encontrado em csv_bruto/merged_focos.csv"
    exit 1
fi

echo "Processando o banco de dados e normalizando os dados..."
sqlite3 sql/original.db << 'EOF'
.mode csv
.import csv_bruto/merged_focos.csv focos
.mode table
ATTACH DATABASE 'sql/normalizado.db' AS normalizado;
-- ensure clean state when running multiple times
DROP TABLE IF EXISTS normalizado.bioma;
DROP TABLE IF EXISTS normalizado.municipio;
DROP TABLE IF EXISTS normalizado.estado;
DROP TABLE IF EXISTS normalizado.focos_bruto;
DROP TABLE IF EXISTS normalizado.focos;

-- build dimension tables from imported main.focos
CREATE TABLE normalizado.bioma AS
    SELECT ROW_NUMBER() OVER(ORDER BY bioma) AS id, bioma AS nome
    FROM main.focos
    WHERE bioma IS NOT NULL AND bioma != ''
    GROUP BY bioma;
CREATE TABLE normalizado.municipio AS
    SELECT ROW_NUMBER() OVER(ORDER BY municipio) AS id, municipio AS nome
    FROM main.focos
    WHERE municipio IS NOT NULL AND municipio != ''
    GROUP BY municipio;
CREATE TABLE normalizado.estado AS
    SELECT ROW_NUMBER() OVER(ORDER BY estado) AS id, estado AS nome
    FROM main.focos
    WHERE estado IS NOT NULL AND estado != ''
    GROUP BY estado;

-- create a raw focos_bruto storing the original data_pas (timestamp) and foreign keys
CREATE TABLE normalizado.focos_bruto AS
    SELECT
        id_bdq AS id,
        lat,
        lon,
        data_pas AS data_pas,
        ne.id AS estadoId,
        nm.id AS municipioId,
        nb.id AS biomaId
    FROM main.focos f
    LEFT JOIN normalizado.estado ne ON ne.nome = f.estado
    LEFT JOIN normalizado.municipio nm ON nm.nome = f.municipio
    LEFT JOIN normalizado.bioma nb ON nb.nome = f.bioma;

-- final focos table with formatted date/time and unix timestamp
CREATE TABLE normalizado.focos (
    id INTEGER,
    lat REAL,
    lon REAL,
    data TEXT,
    hora TEXT,
    timestamp INTEGER,
    estadoId INTEGER,
    municipioId INTEGER,
    biomaId INTEGER
);
INSERT INTO normalizado.focos (
    id,
    lat,
    lon,
    data,
    hora,
    timestamp,
    estadoId,
    municipioId,
    biomaId
)
SELECT
    ABS(RANDOM()),
    CAST(lat AS REAL),
    CAST(lon AS REAL),
    -- format date as DD/MM/YYYY
    COALESCE(strftime('%d/%m/%Y', data_pas), data_pas),
    -- extract hour:minute from data_pas
    COALESCE(strftime('%H:%M', data_pas), ''),
    CAST(strftime('%s', data_pas) AS INTEGER),
    estadoId,
    municipioId,
    biomaId
FROM normalizado.focos_bruto;
DROP TABLE IF EXISTS normalizado.focos_bruto;
EOF

DB_PATH="sql/normalizado.db"
OUTPUT_DIR_ANO="csv_tratado/ano"
OUTPUT_DIR_ESTADO="csv_tratado/estado"
OUTPUT_DIR_ESTADO_2324="csv_tratado/estado_2324"
OUTPUT_DIR_PRINCIPAL="csv_tratado"

echo "Exportando tabelas de dimensões (estado, municipio, bioma)..."
sqlite3 "$DB_PATH" ".mode csv" ".headers on" ".output csv_tratado/estado.csv" "SELECT * FROM estado;"
sqlite3 "$DB_PATH" ".mode csv" ".headers on" ".output csv_tratado/municipio.csv" "SELECT * FROM municipio;"
sqlite3 "$DB_PATH" ".mode csv" ".headers on" ".output csv_tratado/bioma.csv" "SELECT * FROM bioma;"

echo "Iniciando exportação do arquivo completo para o diretório: $OUTPUT_DIR_PRINCIPAL/"
sqlite3 "$DB_PATH" ".mode csv" ".headers on" ".output $OUTPUT_DIR_PRINCIPAL/completo.csv" "SELECT * FROM focos;"
echo "Exportação do arquivo completo concluída."

echo "Iniciando exportação por ano para o diretório: $OUTPUT_DIR_ANO/"
for year in {2003..2024}; do
    echo "Processando ano: $year"
    sqlite3 "$DB_PATH" ".mode csv" ".headers on" ".output $OUTPUT_DIR_ANO/focos_$year.csv" "SELECT * FROM focos WHERE substr(data, 7, 4) = '$year';"
done
echo "Exportação por ano concluída."

echo "Iniciando exportação por estado (todos os anos) para o diretório: $OUTPUT_DIR_ESTADO/"
sqlite3 -separator '|' "$DB_PATH" "SELECT id, nome FROM estado;" | while IFS='|' read -r estadoId estadoNome; do
    if [ -n "$estadoId" ] && [ -n "$estadoNome" ]; then
        mkdir -p "$OUTPUT_DIR_ESTADO"
        FILENAME=$(echo "$estadoNome" | tr ' ' '_').csv
        # safety: skip if filename is empty or looks invalid
        if [ -z "$FILENAME" ] || [ "$FILENAME" = ".csv" ]; then
            echo "Aviso: nome de arquivo inválido para estado '$estadoNome' (id=$estadoId). Pulando."
            continue
        fi
        echo "Processando estado: $estadoNome -> $FILENAME"
        sqlite3 "$DB_PATH" ".mode csv" ".headers on" ".output $OUTPUT_DIR_ESTADO/$FILENAME" "SELECT * FROM focos WHERE estadoId = $estadoId;"
    fi
done
echo "Exportação por estado (todos os anos) concluída."

echo "Iniciando exportação por estado (2023-2024) para o diretório: $OUTPUT_DIR_ESTADO_2324/"
mkdir -p "$OUTPUT_DIR_ESTADO_2324"
sqlite3 -separator '|' "$DB_PATH" "SELECT id, nome FROM estado;" | while IFS='|' read -r estadoId estadoNome; do
    if [ -n "$estadoId" ] && [ -n "$estadoNome" ]; then
        mkdir -p "$OUTPUT_DIR_ESTADO_2324"
        FILENAME=$(echo "$estadoNome" | tr ' ' '_').csv
        # safety: skip if filename is empty or looks invalid
        if [ -z "$FILENAME" ] || [ "$FILENAME" = ".csv" ]; then
            echo "Aviso: nome de arquivo inválido para estado '$estadoNome' (id=$estadoId). Pulando."
            continue
        fi
        echo "Processando estado (2023-2024): $estadoNome -> $FILENAME"
        sqlite3 "$DB_PATH" ".mode csv" ".headers on" ".output $OUTPUT_DIR_ESTADO_2324/$FILENAME" \
            "SELECT * FROM focos WHERE estadoId = $estadoId AND substr(data, 7, 4) IN ('2023', '2024');"
    fi
done
echo "Exportação por estado (2023-2024) concluída."

echo "Exportando municipios do estado do Acre..."
sqlite3 "$DB_PATH" ".mode csv" ".headers on" ".output csv_tratado/municipio_acre.csv" \
    "SELECT DISTINCT m.id, m.nome FROM municipio m JOIN focos f ON m.id = f.municipioId JOIN estado e ON e.id = f.estadoId WHERE e.nome = 'ACRE';"
echo "Exportação de municipios do Acre concluída."

echo "----------------------------------------------------"
echo "Processo finalizado. Verifique os diretórios '$OUTPUT_DIR_ANO', '$OUTPUT_DIR_ESTADO', '$OUTPUT_DIR_ESTADO_2324' e o arquivo 'csv_tratado/completo.csv'."
echo "----------------------------------------------------"

