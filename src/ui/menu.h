#ifndef MENU_H
#define MENU_H

#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
#include "PDCurses/curses.h"
#else
#include <ncurses.h>
#endif

#include "../controller/server.h"

void open_menu(Server *server);

#endif // MENU_H
