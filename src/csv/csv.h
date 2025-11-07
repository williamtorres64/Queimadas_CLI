#ifndef CSV_H
#define CSV_H

#include <stdio.h>
#include "../tipos.h"

_IdNome *lerIdNomeCSV(const char *filename);
Queimada *lerQueimadaCSV(const char *filename);

#endif // CSV_H