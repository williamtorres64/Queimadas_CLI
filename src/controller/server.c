#include "../csv/csv.h"
#include "../tipos.h"
#include "sort.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

/*
 * Conta corretamente o número de caracteres Unicode em uma string UTF-8.
 * Considerando bytes de continuação (com os bits superiores 10) como parte do mesmo caractere.
 */
static size_t utf8_charlen(const char *s)
{
    if (!s)
        return 0;
    size_t len = 0;
    const unsigned char *p = (const unsigned char *)s;
    while (*p)
    {
        // Bytes de continuação UTF-8 têm os dois bits superiores iguais a 10 (0x80..0xBF).
        // Somente bytes que não são continuação incrementam o contador de caracteres.
        if ((*p & 0xC0) != 0x80)
            ++len;
        ++p;
    }
    return len;
}

/*
 * Aloca e inicializa uma nova estrutura Server.
 * Inicializa ponteiros, flags, contadores e limites geográficos/padrão do mapa.
 */
Server *criarServer()
{
    Server *server = malloc(sizeof(Server));
    if (!server)
    {
        perror("Erro ao alocar memória para o Server");
        return NULL;
    }
    server->biomas = NULL;
    server->estados = NULL;
    server->municipios = NULL;
    server->queimadas = NULL;
    server->resultados = NULL;
    server->total_resultados = 0;
    server->pagina_atual = 0;
    server->total_paginas = 0;
    server->deserialization_done = false;
    server->sorting_done = false;
    server->results_ready = false;
    server->scroll_pos = 0;
    server->sort_by = 0;
    server->sort_algorithm = 0;
    server->tempo = 0.0;
    server->comparacoes = 0;
    server->data_mapa = "00/00/0000";
    server->velocidade_mapa = 0;
    server->lat_high = -90.0;
    server->lat_low = 90.0;
    server->lon_high = -180.0;
    server->lon_low = 180.0;
    server->timestamp_high = 0;
    server->timestamp_low = 2000000000;
    server->debug_mapa = false;

    for (int i = 0; i < LARGURA_MAPA * ALTURA_MAPA; i++)
    {
        server->queimadas_mapa_counter[i] = 0;
        server->queimadas_mapa_normalized[i] = 0.0f;
    }

    return server;
}

void reset_server(Server *server)
{
    server->sorting_done = false;
    server->results_ready = false;
    server->tempo = 0.0;
    server->comparacoes = 0;

    server->data_mapa = "00/00/0000";
    server->lat_high = -90.0;
    server->lat_low = 90.0;
    server->lon_high = -180.0;
    server->lon_low = 180.0;
    server->timestamp_high = 0;
    server->timestamp_low = 2000000000;
    server->queimada_mapa = server->queimadas;

    for (int i = 0; i < LARGURA_MAPA * ALTURA_MAPA; i++)
    {
        server->queimadas_mapa_counter[i] = 0;
        server->queimadas_mapa_normalized[i] = 0.0f;
    }
}

/*
 * Carrega arquivos CSV para suas respectivas listas em Server.
 * Define total_resultados com base na lista de queimadas lida.
 */
void read_data(Server *s, const char *biomasFile, const char *estadosFile, const char *municipiosFile, const char *queimadasFile)
{
    s->biomas = lerIdNomeCSV(biomasFile);
    s->estados = lerIdNomeCSV(estadosFile);
    s->municipios = lerIdNomeCSV(municipiosFile);
    s->queimadas = lerQueimadaCSV(queimadasFile);

    // Contar elementos na lista de queimadas
    Queimada *q = s->queimadas;
    int count = 0;
    while (q != NULL)
    {
        count++;
        q = q->next;
    }

    s->total_resultados = count;
    s->deserialization_done = true;
}

// Executa o algoritmo de ordenação selecionado em server->sort_algorithm.
void sort_queimadas(Server *s)
{
    if (s->sort_algorithm == 'b')
    {
        bubble_sort(s);
    }
    else if (s->sort_algorithm == 'm')
    {
        merge_sort(s);
    }
    s->sorting_done = true;
}

// Busca o nome associado a um id em uma lista do tipo _IdNome (Bioma/Estado/Município).
char *id_lookup(_IdNome *head, int id)
{
    _IdNome *current = head;
    while (current != NULL)
    {
        if (current->id == id)
        {
            return current->nome;
        }
        current = current->next;
    }
    return NULL;
}

// Gera strings formatadas para exibição na tabela a partir da lista de queimadas.
void generate_results(Server *server)
{
    if (!server->sorting_done)
        return;

    server->resultados = NULL;

    Queimada *q = server->queimadas;
    for (int i = 0; i < server->total_resultados && q != NULL; i++)
    {
        server->resultados = realloc(server->resultados, server->total_resultados * sizeof(char *));
        if (!server->resultados)
        {
            perror("Erro ao alocar memória para resultados");
            return;
        }

        // Obter nome do bioma e ajustar preenchimento visual respeitando caracteres UTF-8
        char *raw_bioma = id_lookup(server->biomas, q->biomaId);
        if (!raw_bioma)
            raw_bioma = "";
        size_t bioma_chars = utf8_charlen(raw_bioma);

        char bioma_padded[256];
        const size_t BIOMA_MIN = 14;
        // Copiar bytes originalmente (preservando UTF-8)
        strncpy(bioma_padded, raw_bioma, sizeof(bioma_padded) - 1);
        bioma_padded[sizeof(bioma_padded) - 1] = '\0';

        // Se o número de caracteres for menor que o mínimo, preencher com espaços
        if (bioma_chars < BIOMA_MIN)
        {
            size_t pad = BIOMA_MIN - bioma_chars;
            size_t copy_len = strlen(bioma_padded);
            if (copy_len + pad >= sizeof(bioma_padded))
                pad = (sizeof(bioma_padded) - 1) - copy_len;
            memset(bioma_padded + copy_len, ' ', pad);
            bioma_padded[copy_len + pad] = '\0';
        }

        char lat_str[64];
        char lon_str[64];
        snprintf(lat_str, sizeof(lat_str), "%.5f", q->lat);
        snprintf(lon_str, sizeof(lon_str), "%.5f", q->lon);

        size_t lat_len = strlen(lat_str);
        if (lat_len < 9)
        {
            size_t pad = 9 - lat_len;
            if (lat_len + pad >= sizeof(lat_str))
                pad = (sizeof(lat_str) - 1) - lat_len;
            memset(lat_str + lat_len, ' ', pad);
            lat_str[lat_len + pad] = '\0';
        }

        size_t lon_len = strlen(lon_str);
        if (lon_len < 10)
        {
            size_t pad = 10 - lon_len;
            if (lon_len + pad >= sizeof(lon_str))
                pad = (sizeof(lon_str) - 1) - lon_len;
            memset(lon_str + lon_len, ' ', pad);
            lon_str[lon_len + pad] = '\0';
        }

        // Obter e padronizar nome do município (tratamento UTF-8 similar ao bioma)
        char *raw_municipio = id_lookup(server->municipios, q->municipioId);
        if (!raw_municipio)
            raw_municipio = "";
        size_t muni_chars = utf8_charlen(raw_municipio);

        char municipio_padded[256];
        const size_t MUNICIPIO_MIN = 21;

        strncpy(municipio_padded, raw_municipio, sizeof(municipio_padded) - 1);
        municipio_padded[sizeof(municipio_padded) - 1] = '\0';

        if (muni_chars < MUNICIPIO_MIN)
        {
            size_t pad = MUNICIPIO_MIN - muni_chars;
            size_t copy_len = strlen(municipio_padded);
            if (copy_len + pad >= sizeof(municipio_padded))
                pad = (sizeof(municipio_padded) - 1) - copy_len;
            memset(municipio_padded + copy_len, ' ', pad);
            municipio_padded[copy_len + pad] = '\0';
        }

        // Calcular tamanho necessário e alocar string do resultado
        size_t needed = snprintf(NULL, 0,
                                 "%s │ %s │ %s │ %s │ %s │ %s │ ",
                                 q->data, q->hora, bioma_padded, lat_str, lon_str, municipio_padded) +
                        1;

        server->resultados[i] = malloc(needed);
        if (!server->resultados[i])
        {
            perror("Erro ao alocar memória para um resultado");
            for (int j = 0; j < i; ++j)
                free(server->resultados[j]);
            free(server->resultados);
            server->resultados = NULL;
            return;
        }
        snprintf(server->resultados[i], needed,
                 "%s │ %s │ %s │ %s │ %s │ %s │ ",
                 q->data, q->hora, bioma_padded, lat_str, lon_str, municipio_padded);

        q = q->next;
    }
    server->results_ready = true;
}

// Percorre a lista de queimadas para atualizar os limites geográficos (lat/lon) e timestamps
void get_map_data(Server *server)
{
    Queimada *q = server->queimadas;
    while (q != NULL)
    {
        if (q->lat > server->lat_high)
        {
            server->lat_high = q->lat;
        }
        if (q->lat < server->lat_low)
        {
            server->lat_low = q->lat;
        }
        if (q->lon > server->lon_high)
        {
            server->lon_high = q->lon;
        }
        if (q->lon < server->lon_low)
        {
            server->lon_low = q->lon;
        }
        if (q->timestamp > server->timestamp_high)
        {
            server->timestamp_high = q->timestamp;
        }
        if (q->timestamp < server->timestamp_low)
        {
            server->timestamp_low = q->timestamp;
        }
        q = q->next;
    }
}