#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include "constants.h"
#include "tabela.h"
#include "../controller/server.h"

void open_menu(Server *server)
{
    WINDOW *win = newwin(0, 0, 0, 0);

    mvwprintw(win, 0, 0, "┌─────────────────────────────────────────────────────────────────────┐");
    mvwprintw(win, 1, 0, "│                                                                     │");
    mvwprintw(win, 2, 0, "├──────────────────────────────────┬──────────────────────────────────┤");
    mvwprintw(win, 3, 0, "│           Tabela (t)             │             Mapa (m)             │");
    mvwprintw(win, 4, 0, "├──────────────────────────────────┼──────────────────────────────────┤");
    mvwprintw(win, 5, 0, "│  A B C D E F G H I J K L M N O P │ # #                              │");
    mvwprintw(win, 6, 0, "│1 □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ │ ######## # #                     │");
    mvwprintw(win, 7, 0, "│2 □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ │   #################              │");
    mvwprintw(win, 8, 0, "│3 □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ │    #####################         │");
    mvwprintw(win, 9, 0, "│4 □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ │      ######### ############## #  │");
    mvwprintw(win, 10, 0, "│5 □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ │         # # #  ###############   │");
    mvwprintw(win, 11, 0, "│6 □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ │                ########### #     │");
    mvwprintw(win, 12, 0, "│7 □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ │                 #      #         │");
    mvwprintw(win, 13, 0, "└──────────────────────────────────┴──────────────────────────────────┘");

    wattron(win, COLOR_PAIR(3));
    mvwprintw(win, 1, 1, "▃▁▁▃▂▇▇▆▇▅▁▇▂▂▆▅▇▇▂▅▁▅▇▂▅▁▇▇▂▅▁▇▇▂▅▁▇▇▂▅▁▇▂▅▁▇▇▂▅▁▇▂▂▇▅▁▇▂▇▂▅▁▇▂▅▃▆▁▆");
    wattroff(win, COLOR_PAIR(3));

    wattron(win, COLOR_PAIR(2));
    mvwprintw(win, 1, 25, " Queimadas_TUI - Menu ");
    wattroff(win, COLOR_PAIR(2));

    wrefresh(win);

    bool exit_menu = false;
    int ch = 0;
    // Loop until 'q' or ESC (ASCII 27) is pressed
    while (ch != 'q' && ch != 27 && !exit_menu)
    {
        ch = getch();
        switch (ch)
        {
        case 't':
            // Open tabela
            exit_menu = true;
            delwin(win);
            open_tabela(server);
            break;
        case 'm':
            // Open mapa
            // open_mapa();
            break;
        default:
            break;
        }
    }
}
