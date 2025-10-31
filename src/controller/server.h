#ifndef SERVER_H
#define SERVER_H

#include "../tipos.h"
#include <stdlib.h>
#include <stdio.h>

// Function declarations
Server *criarServer();
void read_data(Server *s, const char *biomasFile, const char *estadosFile, const char *municipiosFile, const char *queimadasFile);
void server_fill_test_data(Server *server);
void sort_queimadas(Server *s, char ordenar_por);
void generate_results(Server *server);
void reset_server(Server *server);

#endif // SERVER_H
