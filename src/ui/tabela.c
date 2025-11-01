#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <math.h>
#include "constants.h"
#include "menu.h"
#include "../controller/server.h"

void draw_tabela_header(WINDOW *win)
{
    mvwprintw(win, 0, 0, "┌────────────────────────────────────────────────────────────────────────────────────────┐");
    mvwprintw(win, 1, 0, "│                                                                                        │");
    mvwprintw(win, 2, 0, "├────────────────────────────────────────────────────────────────────────────────────────┤");

    wattron(win, COLOR_PAIR(3)); // Turn on yellow color pair for the title
    mvwprintw(win, 1, 1, "▃▁▁▃▂▇▇▅▁▇▂▂▂▇▂▅▁▆▅▂▇▅▁▂▇▅▁▇▂▃▁▇▂▂▂▇▂▅▁▇▂▂▂▇▂▅▁▂▇▂▅▅▁▇▂▂▂▇▂▅▅▇▆▇▅▁▇▂▇▂▅▁▂▇▅▁▇▂▅▁▇▂▅▃▆▁▆▁");
    wattroff(win, COLOR_PAIR(3)); // Turn off yellow color pair

    wattron(win, COLOR_PAIR(2)); // Turn on blue color pair for the title
    mvwprintw(win, 1, 32, " Queimadas_TUI - Tabela ");
    wattroff(win, COLOR_PAIR(2)); // Turn off blue color pair

    wrefresh(win);
}

void handle_algoritimos(Server *server, WINDOW *win)
{
    mvwprintw(win, 3, 0, "│ Algorítimo: (1) Bubble, (2) Merge                                             Menu (q) │");

    if (server->sort_algorithm > 0)
    {
        wattron(win, COLOR_PAIR(4));
        switch (server->sort_algorithm)
        {
        case 'b':
            mvwprintw(win, 3, 14, "(1) Bubble");
            break;
        case 'm':
            mvwprintw(win, 3, 26, "(2) Merge");
            break;
        default:
            break;
        }
        wattroff(win, COLOR_PAIR(4));
    }
    wrefresh(win);
}

void handle_ordenar_por(char option, WINDOW *win)
{
    mvwprintw(win, 6, 0, "│ Data (d)            Latitude (a) │");
    mvwprintw(win, 7, 0, "│ Município (m)      Longitude (o) │");

    if (option > 0)
    {
        wattron(win, COLOR_PAIR(4));
        switch (option)
        {
        case 'd':
            mvwprintw(win, 6, 2, "Data (d)");
            break;
        case 'a':
            mvwprintw(win, 6, 22, "Latitude (a)");
            break;
        case 'm':
            mvwprintw(win, 7, 2, "Município (m)");
            break;
        case 'o':
            mvwprintw(win, 7, 21, "Longitude (o)");
            break;
        default:
            break;
        }
        wattroff(win, COLOR_PAIR(4));
    }
    wrefresh(win);
}

void handle_status(Server *server, WINDOW *win)
{
    if (server->deserialization_done)
    {
        mvwprintw(win, 6, 36, " Tempo: %.2f s", server->tempo);
        mvwprintw(win, 7, 36, " Comparações: %d", server->comparacoes);

        wrefresh(win);
    }
}

void render_table(Server *server, WINDOW *win)
{
    if (!server->results_ready)
        return;
    int start_index = server->pagina_atual * PAGE_SIZE;
    for (int i = 0; i < PAGE_SIZE; ++i)
    {
        int data_index = start_index + i;
        if (data_index < server->total_resultados)
        {
            mvwprintw(win, 10 + i, 2, "%s", server->resultados[data_index]);
        }
        else
        {
            mvwprintw(win, 10 + i, 2, "           │       │                │           │            │                       │ ");
        }
    }

    mvwprintw(win, 9, 88, " ");

    server->scroll_pos = 9 + (server->pagina_atual * PAGE_SIZE / (double)server->total_resultados) * (PAGE_SIZE + 1);
    mvwprintw(win, server->scroll_pos, 88, "▇");
    wrefresh(win);
}

void handle_scrolling(int direction, WINDOW *win, Server *server)
{
    server->pagina_atual += direction;
    if (server->pagina_atual < 0)
        server->pagina_atual = 0;
    if (server->pagina_atual >= server->total_resultados / PAGE_SIZE)
        server->pagina_atual = server->total_resultados / PAGE_SIZE;
    render_table(server, win);
}

void open_tabela(Server *server)
{
    // Construir interface
    WINDOW *win = newwin(0, 0, 0, 0);
    draw_tabela_header(win);
    handle_algoritimos(server, win);
    mvwprintw(win, 4, 0, "├──────────────────────────────────┬─────────────────────────────────────────────────────┤");
    mvwprintw(win, 5, 0, "│           Ordenar por:           │                       Status:                       │");
    handle_ordenar_por(server->sort_by, win);
    handle_status(server, win);
    mvwprintw(win, 6, 89, "│");
    mvwprintw(win, 7, 89, "│");
    mvwprintw(win, 8, 0, "├────────────┬───────┬─────────────┴──┬───────────┬────────────┬───────────────────────┬─┤");
    mvwprintw(win, 9, 0, "│ Data:      │ Hora: │ Bioma:         │ Latitude: │ Longitude: │ Município:            │ │");
    mvwprintw(win, 10, 0, "│            │       │                │           │            │                       │ │");
    mvwprintw(win, 11, 0, "│            │       │                │           │            │                       │ │");
    mvwprintw(win, 12, 0, "│            │       │                │           │            │                       │ │");
    mvwprintw(win, 13, 0, "│            │       │                │           │            │                       │ │");
    mvwprintw(win, 14, 0, "│            │       │                │           │            │                       │ │");
    mvwprintw(win, 15, 0, "│            │       │                │           │            │                       │ │");
    mvwprintw(win, 16, 0, "│            │       │                │           │            │                       │ │");
    mvwprintw(win, 17, 0, "│            │       │                │           │            │                       │ │");
    mvwprintw(win, 18, 0, "│            │       │                │           │            │                       │ │");
    mvwprintw(win, 19, 0, "│            │       │                │           │            │                       │ │");
    mvwprintw(win, 20, 0, "│            │       │                │           │            │                       │ │");
    mvwprintw(win, 21, 0, "└────────────┴───────┴────────────────┴───────────┴────────────┴───────────────────────┴─┘");

    render_table(server, win);
    wrefresh(win);

    // Loop da aplicação
    bool exit_tabela = false;
    bool reset_sorting = false;
    int ch = 0;
    while (ch != 'q' && ch != 27 && !exit_tabela)
    {
        // get pressed key
        ch = getch();
        // handle keypress
        switch (ch)
        {
        case '1':
            server->sort_algorithm = 'b';
            reset_sorting = true;
            handle_algoritimos(server, win);
            break;
        case '2':
            server->sort_algorithm = 'm';
            reset_sorting = true;
            handle_algoritimos(server, win);
            break;
        case 'd':
            server->sort_by = 'd';
            reset_sorting = true;
            handle_ordenar_por(server->sort_by, win);
            break;
        case 'm':
            reset_sorting = true;
            server->sort_by = 'm';
            reset_sorting = true;
            handle_ordenar_por(server->sort_by, win);
            wrefresh(win);
            break;
        case 'a':
            server->sort_by = 'a';
            reset_sorting = true;
            handle_ordenar_por(server->sort_by, win);
            break;
        case 'o':
            server->sort_by = 'o';
            reset_sorting = true;
            handle_ordenar_por(server->sort_by, win);
            break;
        case KEY_UP:
            handle_scrolling(-1, win, server);
            break;
        case KEY_DOWN:
            handle_scrolling(1, win, server);
            break;
        default:
            break;
        }

        if (server->sort_algorithm && server->sort_by && reset_sorting)
        {
            reset_server(server);
            sort_queimadas(server, server->sort_by);
            generate_results(server);
            handle_status(server, win);
            render_table(server, win);
            reset_sorting = false;
        }
    }
    delwin(win);
    server->pagina_atual = 0;
    open_menu(server);
}
