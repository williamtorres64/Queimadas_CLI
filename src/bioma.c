#include <stdio.h>
#include "tipos.h"

void imprimirBiomas(BiomaLista lista) {
    Bioma *atual = &lista.idNome;
    while (atual) {
    printf("ID: %lu, Nome: %s\n", atual->id, atual->nome ? atual->nome : "(null)");
        atual = atual->next;
    }
}