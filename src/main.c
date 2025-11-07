#define PDC_WIDE 1
#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
#include "PDCurses/curses.h"
#define LOCALE_STRING ".UTF-8"
#else
#include <ncurses.h>
#define LOCALE_STRING ""
#endif

#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include "ui/constants.h"
#include "ui/menu.h"
#include "controller/server.h"

void init_curses()
{
    setlocale(LC_ALL, LOCALE_STRING);
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_YELLOW);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_GREEN, COLOR_BLACK);
    init_pair(6, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    refresh();
}

int main()
{
    // Inicializar o servidor e carregar os dados
    Server *server = criarServer();
    read_data(server, "dados/csv_tratado/bioma.csv", "dados/csv_tratado/estado.csv", "dados/csv_tratado/municipio_acre.csv", "dados/csv_tratado/estado_2324/ACRE.csv");

    // Iniciar a interface ncurses
    init_curses();
    open_menu(server);

    // Finalizar ncurses
    free(server);
    endwin();

    return 0;
}

