#ifndef TABELA_H
#define TABELA_H

#include <ncurses.h>
#include "../controller/server.h"

void open_tabela(Server *server);
void render_table(Server *server, WINDOW *win);

#endif // TABELA_H
