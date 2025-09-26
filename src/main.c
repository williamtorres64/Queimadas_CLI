#include <stdio.h>
#include "csv/csv.h"
#include "tipos.h"
#include "bioma.h"

int main() {
    BiomaLista biomas = lerBiomaCSV("dados/csv_tratado/bioma.csv");
    imprimirBiomas(biomas);
    return 0;
}