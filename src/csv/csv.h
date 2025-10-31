#ifndef CSV_H
#define CSV_H

#include <stdio.h>
#include "../tipos.h"

Bioma *lerBiomaCSV(const char *filename);
Estado *lerEstadoCSV(const char *filename);
Municipio *lerMunicipioCSV(const char *filename);
Queimada *lerQueimadaCSV(const char *filename);

#endif // CSV_H