#ifndef TIPOS_H
#define TIPOS_H

typedef struct _IdNome {
    unsigned long id;
    char *nome;
    struct _IdNome *prev;
    struct _IdNome *next;
} _IdNome;

typedef struct {
    _IdNome idNome;
    _IdNome* prev;
    _IdNome* next;
} _IdNomeLista;

typedef _IdNome Bioma;
typedef _IdNome Estado;
typedef _IdNome Municipio;

typedef _IdNomeLista MunicipioLista;
typedef _IdNomeLista BiomaLista;
typedef _IdNomeLista EstadoLista;


typedef struct _Queimada {
    unsigned long id;
    float lat;
    float lon;
    int ano;
    int mes;
    int dia;
    int hora;
    int minuto;
    int estadoId;
    int municipioId;
    int biomaId;
    struct _Queimada *prev;
    struct _Queimada *next;
} Queimada;

typedef struct {
    Queimada queimada;
    Queimada* next;
    Queimada* prev;
} QueimadaLista;

#endif // TIPOS_H