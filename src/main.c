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
#include <stdio.h> // added for fprintf
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

int main(int argc, char *argv[])
{
    // Determinar os caminhos dos arquivos CSV (ou dos argumentos ou dos padrões)
    const char *bioma_path = "csv/bioma.csv";
    const char *estado_path = "csv/estado.csv";
    const char *municipio_path = "csv/municipio_acre.csv";
    const char *queimadas_path = "csv/queimadas.csv";

    if (argc == 5)
    {
        bioma_path = argv[1];
        estado_path = argv[2];
        municipio_path = argv[3];
        queimadas_path = argv[4];
    }
    else if (argc != 1)
    {
        fprintf(stderr, "Uso correto: %s <bioma.csv> <estado.csv> <municipio.csv> <queimadas.csv>\n", argv[0]);
        return 1;
    }

    // Inicializar o servidor e carregar os dados
    Server *server = criarServer();
    read_data(server, bioma_path, estado_path, municipio_path, queimadas_path);

    // Iniciar a interface ncurses
    init_curses();
    open_menu(server);

    // Finalizar ncurses
    free(server);
    endwin();

    return 0;
}

