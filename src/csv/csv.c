#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../tipos.h"

BiomaLista lerBiomaCSV(const char *filename) {
    BiomaLista lista = {0};
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir arquivo de biomas");
        return lista;
    }

    char line[256];
    // Pular o cabeçalho
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        Bioma *novoBioma = malloc(sizeof(Bioma));
        if (!novoBioma) continue;
        char nomeBuf[256] = {0};
    if (sscanf(line, "%lu,%255[^\n]", &novoBioma->id, nomeBuf) != 2) {
            free(novoBioma);
            continue; // Linha mal formatada
        }
        novoBioma->nome = malloc(strlen(nomeBuf) + 1);
        if (novoBioma->nome) strcpy(novoBioma->nome, nomeBuf);
        novoBioma->next = NULL;
        novoBioma->prev = NULL;

        // Inserir na lista encadeada
        if (!lista.idNome.id) {
            lista.idNome = *novoBioma;
            lista.prev = NULL;
            lista.next = NULL;
            free(novoBioma);
        } else {
            Bioma *atual = &lista.idNome;
            while (atual->next) {
                atual = atual->next;
            }
            Bioma *node = malloc(sizeof(Bioma));
            if (!node) { free(novoBioma->nome); free(novoBioma); continue; }
            node->id = novoBioma->id;
            node->nome = novoBioma->nome;
            node->prev = atual;
            node->next = NULL;
            atual->next = node;
            free(novoBioma);
        }
    }

    fclose(file);
    return lista;
}

EstadoLista lerEstadoCSV(const char *filename) {
    EstadoLista lista = {0};
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir arquivo de estados");
        return lista;
    }

    char line[256];
    // Pular o cabeçalho
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        Estado *novoEstado = malloc(sizeof(Estado));
        if (!novoEstado) continue;
        char nomeBuf[256] = {0};
        if (sscanf(line, "%lu,%255[^\n]", &novoEstado->id, nomeBuf) != 2) {
            free(novoEstado);
            continue; // Linha mal formatada
        }
        novoEstado->nome = malloc(strlen(nomeBuf) + 1);
        if (novoEstado->nome) strcpy(novoEstado->nome, nomeBuf);
        novoEstado->next = NULL;
        novoEstado->prev = NULL;

        if (!lista.idNome.id) {
            lista.idNome.id = novoEstado->id;
            lista.idNome.nome = novoEstado->nome;
            lista.prev = NULL;
            lista.next = NULL;
            free(novoEstado);
        } else {
            Estado *atual = &lista.idNome;
            while (atual->next) {
                atual = atual->next;
            }
            Estado *node = malloc(sizeof(Estado));
            if (!node) { free(novoEstado->nome); free(novoEstado); continue; }
            node->id = novoEstado->id;
            node->nome = novoEstado->nome;
            node->prev = atual;
            node->next = NULL;
            atual->next = node;
            free(novoEstado);
        }
    }

    fclose(file);
    return lista;
}

MunicipioLista lerMunicipioCSV(const char *filename) {
    MunicipioLista lista = {0};
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir arquivo de municípios");
        return lista;
    }

    char line[256];
    // Pular o cabeçalho
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        Municipio *novoMunicipio = malloc(sizeof(Municipio));
        if (!novoMunicipio) continue;
        char nomeBuf[256] = {0};
        if (sscanf(line, "%lu,%255[^\n]", &novoMunicipio->id, nomeBuf) != 2) {
            free(novoMunicipio);
            continue; // Linha mal formatada
        }
        novoMunicipio->nome = malloc(strlen(nomeBuf) + 1);
        if (novoMunicipio->nome) strcpy(novoMunicipio->nome, nomeBuf);
        novoMunicipio->next = NULL;
        novoMunicipio->prev = NULL;

        if (!lista.idNome.id) {
            lista.idNome.id = novoMunicipio->id;
            lista.idNome.nome = novoMunicipio->nome;
            lista.prev = NULL;
            lista.next = NULL;
            free(novoMunicipio);
        } else {
            Municipio *atual = &lista.idNome;
            while (atual->next) {
                atual = atual->next;
            }
            Municipio *node = malloc(sizeof(Municipio));
            if (!node) { free(novoMunicipio->nome); free(novoMunicipio); continue; }
            node->id = novoMunicipio->id;
            node->nome = novoMunicipio->nome;
            node->prev = atual;
            node->next = NULL;
            atual->next = node;
            free(novoMunicipio);
        }
    }

    fclose(file);
    return lista;
}

QueimadaLista lerQueimadaCSV(const char *filename) {
    QueimadaLista lista = {0};
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir arquivo de queimadas");
        return lista;
    }

    char line[512];
    // Pular o cabeçalho
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        Queimada *novaQueimada = malloc(sizeof(Queimada));
        if (sscanf(line, "%lu,%f,%f,%d,%d,%d,%d,%d,%d,%d,%d",
                   &novaQueimada->id,
                   &novaQueimada->lat,
                   &novaQueimada->lon,
                   &novaQueimada->ano,
                   &novaQueimada->mes,
                   &novaQueimada->dia,
                   &novaQueimada->hora,
                   &novaQueimada->minuto,
                   &novaQueimada->estadoId,
                   &novaQueimada->municipioId,
                   &novaQueimada->biomaId) != 11) {
            free(novaQueimada);
            continue; // Linha mal formatada
        }

        // Inserir na lista encadeada
        if (!lista.queimada.id) {
            lista.queimada = *novaQueimada;
            lista.prev = NULL;
            lista.next = NULL;
        } else {
            Queimada *atual = &lista.queimada;
            while (atual->next) {
                atual = atual->next;
            }
            atual->next = novaQueimada;
            novaQueimada->prev = atual;
            novaQueimada->next = NULL;
        }
    }

    fclose(file);
    return lista;
}