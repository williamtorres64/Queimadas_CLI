#ifndef CSV_H
#define CSV_H

#include <stdio.h>
#include "../tipos.h"

BiomaLista lerBiomaCSV(const char *filename);
EstadoLista lerEstadoCSV(const char *filename);
MunicipioLista lerMunicipioCSV(const char *filename);
QueimadaLista lerQueimadaCSV(const char *filename);

#endif // CSV_H