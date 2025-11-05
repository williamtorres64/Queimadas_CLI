#ifndef TABELA_H
#define TABELA_H

#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
#include "PDCurses/curses.h"
#else
#include <ncurses.h>
#endif

#include "../controller/server.h"

void open_tabela(Server *server);
void render_table(Server *server, WINDOW *win);

#endif // TABELA_H
