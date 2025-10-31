#ifndef TIPOS_H
#define TIPOS_H

#include <stdbool.h>

typedef struct _IdNome {
    unsigned long id;
    char *nome;
    struct _IdNome *prev;
    struct _IdNome *next;
} _IdNome;

typedef _IdNome Bioma;
typedef _IdNome Estado;
typedef _IdNome Municipio;

typedef struct _Queimada {
    unsigned long id;
    float lat;
    float lon;
    char* data;
    char* hora;
    int timestamp;
    int estadoId;
    int municipioId;
    int biomaId;
    struct _Queimada *prev;
    struct _Queimada *next;
} Queimada;

typedef struct _Server {
    Bioma *biomas;
    Estado *estados;
    Municipio *municipios;
    Queimada *queimadas;

    char **resultados;
    int total_resultados;
    int pagina_atual;
    int total_paginas;
 
    int scroll_pos;

    bool deserialization_done;
    bool sorting_done;
    bool results_ready;

    char sort_by;
    char sort_algorithm;

    double tempo;
    int comparacoes;
} Server;

#endif // TIPOS_H