#include <ncurses.h>
#include <menu.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

// Function prototypes
void init_curses();
void draw_main_menu();
void draw_tabela_menu();
void draw_mapa_menu();
void handle_main_input();
void handle_tabela_input();
void handle_mapa_input();

// Global variables for menus (using ncurses menu library for options)
MENU* main_menu;
MENU* tabela_menu;
MENU* mapa_menu;
ITEM** main_items;
ITEM** tabela_items;
ITEM** mapa_items;

// Sample choices for each menu (based on ui.txt)
char* main_choices[] = {"Tabela (t)", "Mapa (m)", "Quit (q)", (char*)NULL};
char* tabela_choices[] = {"Bubble (1)", "Quick (2)", "Merge (3)", "Data (d)", "Município (m)", "Menu (q)", (char*)NULL};
char* mapa_choices[] = {"2023 (1)", "2024 (2)", "todos (3)", "Menu (q)", (char*)NULL};

int main() {
    init_curses();

    // Create menu items
    int n_main = 0, n_tabela = 0, n_mapa = 0;
    while (main_choices[n_main]) n_main++;
    main_items = (ITEM**)calloc(n_main + 1, sizeof(ITEM*));
    for (int i = 0; i < n_main; ++i) main_items[i] = new_item(main_choices[i], "");

    while (tabela_choices[n_tabela]) n_tabela++;
    tabela_items = (ITEM**)calloc(n_tabela + 1, sizeof(ITEM*));
    for (int i = 0; i < n_tabela; ++i) tabela_items[i] = new_item(tabela_choices[i], "");

    while (mapa_choices[n_mapa]) n_mapa++;
    mapa_items = (ITEM**)calloc(n_mapa + 1, sizeof(ITEM*));
    for (int i = 0; i < n_mapa; ++i) mapa_items[i] = new_item(mapa_choices[i], "");

    // Create menus
    main_menu = new_menu(main_items);
    tabela_menu = new_menu(tabela_items);
    mapa_menu = new_menu(mapa_items);

    // Start with main menu
    draw_main_menu();
    handle_main_input();

    // Cleanup
    unpost_menu(main_menu);
    free_menu(main_menu);
    for (int i = 0; i < n_main; ++i) free_item(main_items[i]);
    free(main_items);

    unpost_menu(tabela_menu);
    free_menu(tabela_menu);
    for (int i = 0; i < n_tabela; ++i) free_item(tabela_items[i]);
    free(tabela_items);

    unpost_menu(mapa_menu);
    free_menu(mapa_menu);
    for (int i = 0; i < n_mapa; ++i) free_item(mapa_items[i]);
    free(mapa_items);

    endwin();
    return 0;
}

void init_curses() {
    setlocale(LC_ALL, "");
    initscr();              // Start curses mode
    raw();                  // Disable line buffering
    keypad(stdscr, TRUE);   // Enable function keys
    noecho();               // Don't echo user input
    curs_set(0);            // Hide cursor
    start_color();          // Enable colors if supported
    init_pair(1, COLOR_WHITE, COLOR_BLACK); // Basic color pair
    attron(COLOR_PAIR(1));
}

void draw_main_menu() {
    clear(); // Clear screen
    // Draw exact title
    mvprintw(0, 0, "┌─────────────────────────────────────────────────────────────────────┐");
    mvprintw(1, 0, "│▃▁▁▃▂▇▇▆▇▅▁▇▂▂▆▅▇▅▁▇▂▃▁▂▅▂ Queimadas_CLI - Menu ▆▁▅▇▆▇▅▁▇▂▇▂▅▁▇▂▅▃▆▁▆│");
    mvprintw(2, 0, "├──────────────────────────────────┬──────────────────────────────────┤");
    mvprintw(3, 0, "│           Tabela (t)             │             Mapa (m)             │");
    mvprintw(4, 0, "├──────────────────────────────────┼──────────────────────────────────┤");
    mvprintw(5, 0, "│  A B C D E F G H I J K L M N O P │ # #                              │");
    mvprintw(6, 0, "│1 □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ │ ######## # #                     │");
    mvprintw(7, 0, "│2 □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ │   #################              │");
    mvprintw(8, 0, "│3 □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ │    #####################         │");
    mvprintw(9, 0, "│4 □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ │      ######### ############## #  │");
    mvprintw(10, 0, "│5 □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ │         # # #  ###############   │");
    mvprintw(11, 0, "│6 □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ │                ########### #     │");
    mvprintw(12, 0, "│7 □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ │                 #      #         │");
    mvprintw(13, 0, "└──────────────────────────────────┴──────────────────────────────────┘");

    // Post menu options (using ncurses menu for navigation, positioned below)
    set_menu_win(main_menu, stdscr);
    set_menu_sub(main_menu, derwin(stdscr, 3, 20, LINES - 4, (COLS - 20) / 2));
    post_menu(main_menu);

    refresh();
}

void draw_tabela_menu() {
    clear();
    // Draw exact title and content
    mvprintw(0, 0, "┌────────────────────────────────────────────────────────────────────────────────┐");
    mvprintw(1, 0, "│▃▁▁▃▂▇▇▅▁▇▂▂▂▇▂▅▁▆▅▇▅▁▇▂▃▁▂▅▂ Queimadas_CLI - Tabela ▆▁▅▇▆▇▅▁▇▂▇▂▅▁▂▇▂▅▁▇▂▅▃▆▁▆▁│");
    mvprintw(2, 0, "├────────────────────────────────────────────────────────────────────────────────┤");
    mvprintw(3, 0, "│ Algorítimo: (1) Bubble, (2) Quick, (3) Merge                          Menu (q) │");
    mvprintw(4, 0, "├──────────────────────────────────┬─────────────────────────────────────────────┤");
    mvprintw(5, 0, "│          Ordenar por:            │              Status:                        │");
    mvprintw(6, 0, "│ Data (d)                         │ Tempo: 0.0 ms                               │");
    mvprintw(7, 0, "│ Município (m)                    │ Comparações: 0                              │");
    mvprintw(8, 0, "├──────┬─────┬─────┬───────┬───────┴────────┬──────────────────────────────────┬─┤");
    mvprintw(9, 0, "│ Ano: │ Mês:│ Dia:│ Hora: │ Bioma:         │ Município:                       │▊│");
    mvprintw(10, 0, "│ 2023 │ 10  │ 14  │ 20:24 │ Mata Atlântica │ Rio Branco                       │ │");
    mvprintw(11, 0, "│ 2023 │ 12  │ 15  │ 05:35 │ Cerrado        │ Porto Acre                       │ │");
    mvprintw(12, 0, "│ 2024 │ 5   │ 16  │ 17:48 │ Caatinga       │ Tarauacá                         │ │");
    mvprintw(13, 0, "│ 2024 │ 5   │ 4   │ 18:25 │ Pampa          │ Xapuri                           │ │");
    mvprintw(14, 0, "│ 2024 │ 7   │ 6   │ 14:38 │ Pantanal       │ Rio Branco                       │ │");
    mvprintw(15, 0, "│ 2023 │ 10  │ 14  │ 20:24 │ Mata Atlântica │ Rio Branco                       │ │");
    mvprintw(16, 0, "│ 2023 │ 12  │ 15  │ 05:35 │ Cerrado        │ Porto Acre                       │ │");
    mvprintw(17, 0, "│ 2024 │ 5   │ 16  │ 17:48 │ Caatinga       │ Tarauacá                         │ │");
    mvprintw(18, 0, "│ 2024 │ 5   │ 4   │ 18:25 │ Pampa          │ Xapuri                           │ │");
    mvprintw(19, 0, "└──────┴─────┴─────┴───────┴────────────────┴──────────────────────────────────┴─┘");

    // Post menu
    set_menu_win(tabela_menu, stdscr);
    set_menu_sub(tabela_menu, derwin(stdscr, 6, 30, 3, COLS - 40)); // Position near the options
    post_menu(tabela_menu);

    refresh();
}

void draw_mapa_menu() {
    clear();
    // Draw exact title and content
    mvprintw(0, 0, "┌────────────────────────────────────────────────────────────────────────────────┐");
    mvprintw(1, 0, "│▃▁▁▃▂▇▇▅▁▇▂▂▂▇▂▅▁▆▅▇▅▁▇▂▃▁▂▅▂ Queimadas_CLI - Mapa ▆▁▅▇▆▇▇▁▅▁▇▂▇▂▅▁▂▇▂▅▁▇▂▅▃▆▁▆▁│");
    mvprintw(2, 0, "├────────────────────────────────────────────────────────────────────────────────┤");
    mvprintw(3, 0, "│ Ano: (1) 2023, (2) 2024, (3) todos                                    Menu (q) │");
    mvprintw(4, 0, "├────────────────────────────────────────────────────────────────────────────────┤");
    mvprintw(5, 0, "│      ### #                                                                     │");
    mvprintw(6, 0, "│    ########### #                                                               │");
    mvprintw(7, 0, "│     ################# #  #                                                     │");
    mvprintw(8, 0, "│      ############################ #                                            │");
    mvprintw(9, 0, "│        ##################################                                      │");
    mvprintw(10, 0, "│        #######################################                                 │");
    mvprintw(11, 0, "│           ########################################                             │");
    mvprintw(12, 0, "│           #############################################                        │");
    mvprintw(13, 0, "│              ##############################################                    │");
    mvprintw(14, 0, "│              ##################################################                │");
    mvprintw(15, 0, "│             ## ###################  ############################### #          │");
    mvprintw(16, 0, "│                     #############   ####################################       │");
    mvprintw(17, 0, "│                      ##########    ###################################         │");
    mvprintw(18, 0, "│                       #     #      ################################ #          │");
    mvprintw(19, 0, "│                                     #############################              │");
    mvprintw(20, 0, "│                                     ########################  #                │");
    mvprintw(21, 0, "│                                     ######################                     │");
    mvprintw(22, 0, "│                                       ###    #    #  ####                      │");
    mvprintw(23, 0, "└────────────────────────────────────────────────────────────────────────────────┘");

    // Post menu
    set_menu_win(mapa_menu, stdscr);
    set_menu_sub(mapa_menu, derwin(stdscr, 4, 30, 3, COLS - 40)); // Position near the options
    post_menu(mapa_menu);

    refresh();
}

void handle_main_input() {
    int ch;
    while ((ch = getch()) != 'q' && ch != 'Q') {
        switch (ch) {
            case KEY_DOWN: menu_driver(main_menu, REQ_DOWN_ITEM); break;
            case KEY_UP: menu_driver(main_menu, REQ_UP_ITEM); break;
            case 10: { // Enter
                ITEM* cur = current_item(main_menu);
                if (strcmp(item_name(cur), "Tabela (t)") == 0) {
                    unpost_menu(main_menu);
                    draw_tabela_menu();
                    handle_tabela_input();
                    draw_main_menu();
                } else if (strcmp(item_name(cur), "Mapa (m)") == 0) {
                    unpost_menu(main_menu);
                    draw_mapa_menu();
                    handle_mapa_input();
                    draw_main_menu();
                }
                break;
            }
            case 't': case 'T':
                unpost_menu(main_menu);
                draw_tabela_menu();
                handle_tabela_input();
                draw_main_menu();
                break;
            case 'm': case 'M':
                unpost_menu(main_menu);
                draw_mapa_menu();
                handle_mapa_input();
                draw_main_menu();
                break;
        }
        refresh();
    }
}

void handle_tabela_input() {
    int ch;
    while ((ch = getch()) != 'q' && ch != 'Q') {
        switch (ch) {
            case KEY_DOWN: menu_driver(tabela_menu, REQ_DOWN_ITEM); break;
            case KEY_UP: menu_driver(tabela_menu, REQ_UP_ITEM); break;
            case 10: { // Enter - handle selection (skeleton: just print action, implement sorting later)
                ITEM* cur = current_item(tabela_menu);
                mvprintw(LINES - 1, 0, "Selected: %s (implement action here)", item_name(cur));
                clrtoeol();
                refresh();
                break;
            }
            case '1': case '2': case '3': case 'd': case 'D': case 'm': case 'M':
                // Handle direct keys (skeleton)
                mvprintw(LINES - 1, 0, "Key %c pressed (implement action)", ch);
                clrtoeol();
                refresh();
                break;
        }
    }
    unpost_menu(tabela_menu);
}

void handle_mapa_input() {
    int ch;
    while ((ch = getch()) != 'q' && ch != 'Q') {
        switch (ch) {
            case KEY_DOWN: menu_driver(mapa_menu, REQ_DOWN_ITEM); break;
            case KEY_UP: menu_driver(mapa_menu, REQ_UP_ITEM); break;
            case 10: { // Enter - handle selection (skeleton: just print action, implement update later)
                ITEM* cur = current_item(mapa_menu);
                mvprintw(LINES - 1, 0, "Selected: %s (implement action here)", item_name(cur));
                clrtoeol();
                refresh();
                break;
            }
            case '1': case '2': case '3':
                // Handle direct keys (skeleton)
                mvprintw(LINES - 1, 0, "Key %c pressed (implement action)", ch);
                clrtoeol();
                refresh();
                break;
        }
    }
    unpost_menu(mapa_menu);
}