#define PDC_WIDE 1
#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
#include <PDCurses/curses.h>
#else
#include <ncurses.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include "ui/constants.h"
#include "ui/menu.h"           // Add include for menu functions
#include "controller/server.h" // Include server header

void init_curses();
void end_curses();

int main()
{
    Server *server = criarServer();
    read_data(server, "dados/csv_tratado/bioma.csv", "dados/csv_tratado/estado.csv", "dados/csv_tratado/municipio_acre.csv", "dados/csv_tratado/estado_2324/ACRE.csv");

    init_curses();

    open_menu(server);

    free(server);

    end_curses();

    return 0;
}

void init_curses()
{
    // initialize ncurses
    setlocale(LC_ALL, ""); // Set the locale to the user's default for proper character handling

    initscr();                              // Initialize the ncurses library and set up the terminal screen
    raw();                                  // Disable line buffering and special key processing for direct input
    keypad(stdscr, TRUE);                   // Enable special keys like arrow keys on the standard screen
    noecho();                               // Prevent typed characters from being echoed to the screen
    curs_set(0);                            // Hide the cursor for a cleaner interface
    start_color();                          // Initialize color support in ncurses
    init_pair(1, COLOR_WHITE, COLOR_BLACK); // Define a color pair: white text on black background
    init_pair(2, COLOR_RED, COLOR_BLACK);   // Define a color pair: red text on black background
    init_pair(3, COLOR_RED, COLOR_YELLOW);  // Define a color pair: yellow text on black background
    init_pair(4, COLOR_BLUE, COLOR_BLACK);  // Define a color pair: blue text on black background
    attron(COLOR_PAIR(1));                  // Apply the color pair to subsequent output
    refresh();
}

void end_curses()
{
    endwin(); // End ncurses mode and restore the terminal to its original state
}