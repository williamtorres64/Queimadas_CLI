#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
#include "PDCurses/curses.h"
#else
#include <ncurses.h>
#endif
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include "constants.h"
#include "menu.h"
#include "../controller/server.h"

static void limpar_mapa(WINDOW *win)
{
    mvwprintw(win, 7, 0, "│      ••• •                                                                     │");
    mvwprintw(win, 8, 0, "│    ••••••••••• •                                                               │");
    mvwprintw(win, 9, 0, "│     ••••••••••••••••• •  •                                                     │");
    mvwprintw(win, 10, 0, "│      •••••••••••••••••••••••••••• •                                            │");
    mvwprintw(win, 11, 0, "│        ••••••••••••••••••••••••••••••••••                                      │");
    mvwprintw(win, 12, 0, "│        •••••••••••••••••••••••••••••••••••••••                                 │");
    mvwprintw(win, 13, 0, "│           ••••••••••••••••••••••••••••••••••••••••                             │");
    mvwprintw(win, 14, 0, "│           •••••••••••••••••••••••••••••••••••••••••••••                        │");
    mvwprintw(win, 15, 0, "│              ••••••••••••••••••••••••••••••••••••••••••••••                    │");
    mvwprintw(win, 16, 0, "│              ••••••••••••••••••••••••••••••••••••••••••••••••••                │");
    mvwprintw(win, 17, 0, "│             •• •••••••••••••••••••  ••••••••••••••••••••••••••••••• •          │");
    mvwprintw(win, 18, 0, "│                     •••••••••••••   ••••••••••••••••••••••••••••••••••••       │");
    mvwprintw(win, 19, 0, "│                      ••••••••••    •••••••••••••••••••••••••••••••••••         │");
    mvwprintw(win, 20, 0, "│                       •     •      •••••••••••••••••••••••••••••••• •          │");
    mvwprintw(win, 21, 0, "│                                     •••••••••••••••••••••••••••••              │");
    mvwprintw(win, 22, 0, "│                                     ••••••••••••••••••••••••  •                │");
    mvwprintw(win, 23, 0, "│                                     ••••••••••••••••••••••                     │");
    mvwprintw(win, 24, 0, "│                                       •••    •    •  ••••                      │");
    wrefresh(win);
}

static void draw_mapa_template(WINDOW *win)
{
    mvwprintw(win, 0, 0, "┌────────────────────────────────────────────────────────────────────────────────┐");
    mvwprintw(win, 1, 0, "│");
    wattron(win, COLOR_PAIR(3));
    mvwprintw(win, 1, 1, "▃▁▁▃▂▇▇▆▇▅▁▇▂▂▆▅▇▇▂▅▁▅▇▂▅▁▇▇▂▂▆▆▁▅▇▆▇▇▁▅▁▇▂▇▂▅▁▂▇▂▅▁▇▂▅▃▆▁▆▅▅▁▆▁▅▇▆▇▇▁▅▁▇▂▇▂▅▁▁▆");
    wattroff(win, COLOR_PAIR(3));
    wattron(win, COLOR_PAIR(2));
    mvwprintw(win, 1, 31, " Queimadas_TUI - Mapa ");
    wattroff(win, COLOR_PAIR(2));
    wattron(win, COLOR_PAIR(3));
    mvwprintw(win, 1, 55, "");
    wattroff(win, COLOR_PAIR(3));
    mvwprintw(win, 1, 81, "│");
    mvwprintw(win, 2, 0, "├──────────────────┬────────────────────┬────────────────────────────────────────┤");
    mvwprintw(win, 3, 0, "│ Data: 00/00/0000 │ ↕ Velocidade: 0.0x │ Reiniciar (r) Debug (d)       Menu (q) │");
    mvwprintw(win, 4, 0, "├──────────────────┴────────────────────┴────────────────────────────────────────┤");
    mvwprintw(win, 5, 0, "│                                                                                │");
    mvwprintw(win, 6, 0, "├────────────────────────────────────────────────────────────────────────────────┤");
    limpar_mapa(win);
    mvwprintw(win, 25, 0, "└────────────────────────────────────────────────────────────────────────────────┘");
    mvwprintw(win, 7, 82, "Concentração normal:");
    mvwprintw(win, 8, 82, "● ≤ 1%% da máxima");
    mvwprintw(win, 9, 82, "● ≤ 5%% da máxima");
    mvwprintw(win, 10, 82, "● ≤ 10%% da máxima");
    mvwprintw(win, 11, 82, "● > 10%% da máxima");
    wattron(win, COLOR_PAIR(4));
    mvwprintw(win, 8, 82, "●");
    wattroff(win, COLOR_PAIR(4));
    wattron(win, COLOR_PAIR(5));
    mvwprintw(win, 9, 82, "●");
    wattroff(win, COLOR_PAIR(5));
    wattron(win, COLOR_PAIR(6));
    mvwprintw(win, 10, 82, "●");
    wattroff(win, COLOR_PAIR(6));
    wattron(win, COLOR_PAIR(2));
    mvwprintw(win, 11, 82, "●");
    wattroff(win, COLOR_PAIR(2));
    wrefresh(win);
}

static void internal_sort(Server *server)
{
    char previous_sort_by = server->sort_by;
    char previous_sort_algorithm = server->sort_algorithm;
    server->sort_by = 'd';
    server->sort_algorithm = 'm';
    reset_server(server);
    sort_queimadas(server);
    server->queimada_mapa = server->queimadas;
    get_map_data(server);
    server->sort_algorithm = previous_sort_algorithm;
    server->sort_by = previous_sort_by;
}

static void handle_reset(WINDOW *win, Server *server)
{
    // Retornar ponteiro para o início da lista de queimadas
    internal_sort(server);
    server->queimada_mapa = server->queimadas;

    // Redefinir tempo inicial do mapa
    server->map_start_time = ((double)clock()) / CLOCKS_PER_SEC * 10;

    // Atualizar interface
    mvwprintw(win, 3, 35, "%.1f", server->velocidade_mapa);
    limpar_mapa(win);
    wrefresh(win);
}

static void handle_scroll_up(WINDOW *win, Server *server)
{
    if (server->velocidade_mapa < 9.9)
        server->velocidade_mapa += 0.1;
    mvwprintw(win, 3, 35, "%.1f", server->velocidade_mapa);
}

static void handle_scroll_down(WINDOW *win, Server *server)
{
    if (server->velocidade_mapa > 0.01)
        server->velocidade_mapa -= 0.1;
    mvwprintw(win, 3, 35, "%.1f", server->velocidade_mapa);
}

static void handle_progress_bar(WINDOW *win, float progress)
{
    int filled_length = (int)(LARGURA_MAPA * progress);

    for (int i = 1; i <= LARGURA_MAPA; i++)
    {
        if (i <= filled_length)
            mvwaddch(win, 5, i, ACS_CKBOARD);
        else
            mvwaddch(win, 5, i, ' ');
    }
    wrefresh(win);
}

static float normalize_value(float value, float min, float max)
{
    if (max - min == 0)
        return 0.0f;
    return (value - min) / (max - min);
}

static double normalize_value_int(int value, int min, int max)
{
    if (max - min == 0)
        return 0.0f;
    return (double)(value - min) / (double)(max - min);
}

static double draw_mapa_colorido(Server *server, WINDOW *win)
{
    // Converter lat/lon da queimada atual para coordenadas x,y no mapa
    int largura_contorno = LARGURA_MAPA - 11;
    float normalized_lat = normalize_value(server->queimada_mapa->lat, server->lat_low, server->lat_high);
    float normalized_lon = normalize_value(server->queimada_mapa->lon, server->lon_low, server->lon_high);
    int x = (int)(normalized_lon * (largura_contorno - 1));
    int y = (int)((1.0f - normalized_lat) * (ALTURA_MAPA - 1)); // Inverter eixo y

    // Atualizar contador de queimadas no mapa
    server->queimadas_mapa_counter[y * largura_contorno + x]++;
    int highest_count = 0;
    for (int i = 0; i < largura_contorno * ALTURA_MAPA; i++)
    {
        if (server->queimadas_mapa_counter[i] > highest_count)
            highest_count = server->queimadas_mapa_counter[i];
    }

    // Recalcular contador normalizado
    for (int i = 0; i < largura_contorno * ALTURA_MAPA; i++)
    {
        float normalized_count = normalize_value((float)server->queimadas_mapa_counter[i], 0.0f, (float)highest_count);
        server->queimadas_mapa_normalized[i] = normalized_count;
    }

    // Desenhar cada ponto colorido no mapa
    for (int yy = 0; yy < ALTURA_MAPA; yy++)
    {
        for (int xx = 0; xx < largura_contorno; xx++)
        {
            int idx = yy * largura_contorno + xx;
            float normalized_count = server->queimadas_mapa_normalized[idx];
            if (normalized_count == 0.0f)
                continue;

            // Choose color based on normalized count
            int color_pair = 0;
            if (normalized_count <= 0.01f)
                color_pair = 4; // Blue
            else if (normalized_count <= 0.05f)
                color_pair = 5; // Green
            else if (normalized_count <= 0.10f)
                color_pair = 6; // Yellow
            else
                color_pair = 2; // Red

            wattron(win, COLOR_PAIR(color_pair));
            mvwprintw(win, 7 + yy, 5 + xx, "●");
            wattroff(win, COLOR_PAIR(color_pair));
        }
    }

}

static void advance_map(Server *server, WINDOW *win)
{
    // Não contar a passagem do tempo enquanto a velocidade do mapa for zero
    if (server->velocidade_mapa == 0.0f)
    {
        server->map_start_time = ((double)clock()) / CLOCKS_PER_SEC * 10;
        return;
    }

    double current_time = ((double)clock()) / CLOCKS_PER_SEC * 10;
    double elapsed = current_time - server->map_start_time;
    double expected_progress = elapsed / MAP_DURATION * server->velocidade_mapa;

    // Caso a queimada do mapa ainda não tenha sido definida
    if (server->queimada_mapa == NULL)
    {
        server->data_mapa = "00/00/0000";
        return;
    }

    double actual_progress = normalize_value_int(server->queimada_mapa->timestamp, server->timestamp_low, server->timestamp_high);

    handle_progress_bar(win, actual_progress);

    // Atualizar mapa enquanto a progressão não chegar ao fim
    if (expected_progress <= 1.0 || actual_progress < 1.0)
    {
        draw_mapa_colorido(server, win);
    }

    if (server->debug_mapa)
    {
        mvwprintw(win, 15, 2, "Tempo: %f s             ", elapsed);
        mvwprintw(win, 16, 2, "Progresso Esperado: %f          ", expected_progress);
        mvwprintw(win, 17, 2, "Progresso Atual: %f    ", actual_progress);
        mvwprintw(win, 18, 2, "Velocidade Atual: %.1f            ", server->velocidade_mapa);
        mvwprintw(win, 19, 2, "Tempo de Início do Mapa: %f   ", server->map_start_time);
        mvwprintw(win, 20, 2, "Duração do Mapa: %.1f          ", MAP_DURATION);
        mvwprintw(win, 21, 2, "ID da Queimada: %lu     ", server->queimada_mapa->id);
        mvwprintw(win, 22, 2, "Timestamp da Queimada: %d    ", server->queimada_mapa->timestamp);
        mvwprintw(win, 23, 2, "Queimada Lat: %f          ", server->queimada_mapa->lat);
        mvwprintw(win, 24, 2, "Queimada Lon: %f    ", server->queimada_mapa->lon);
    }

    // Avançar para a próxima queimada se necessário
    if (actual_progress < expected_progress)
    {
        if (server->queimada_mapa->next != NULL)
        {
            server->queimada_mapa = server->queimada_mapa->next;
            server->data_mapa = server->queimada_mapa->data;
        }
    }
    else
    {
        server->data_mapa = server->queimada_mapa->data;
    }
    mvwprintw(win, 3, 8, "%s", server->data_mapa);
    wrefresh(win);
}

void open_mapa(Server *server)
{
    WINDOW *win = newwin(0, 0, 0, 0);

    draw_mapa_template(win);
    internal_sort(server);
    server->velocidade_mapa = 0.0f;
    server->map_start_time = ((double)clock()) / CLOCKS_PER_SEC * 10;
    bool exit_mapa = false;
    int ch = -1;

    // Não parar esperando a entrada do usuário
    keypad(win, TRUE);
    wtimeout(win, 1); // 1 ms timeout

    while (!exit_mapa && ch != 'q' && ch != 27)
    {
        ch = wgetch(win); // retorna ERR se não houver entrada dentro do tempo limite
        if (ch != ERR)
        {
            switch (ch)
            {
            case 'q':
                // Menu
                exit_mapa = true;
                break;
            case 'r':
                // Reiniciar
                handle_reset(win, server);
                break;
            case 'd':
                // Debug
                server->debug_mapa = !server->debug_mapa;
                if (server->debug_mapa)
                {
                    wattron(win, COLOR_PAIR(4));
                    mvwprintw(win, 3, 56, "Debug (d)");
                    wattroff(win, COLOR_PAIR(4));
                }
                else
                {
                    mvwprintw(win, 3, 56, "Debug (d)");
                    limpar_mapa(win);
                    draw_mapa_colorido(server, win);
                }
                break;
            case KEY_UP:
                // Aumentar velocidade
                handle_scroll_up(win, server);
                break;
            case KEY_DOWN:
                // Diminuir velocidade
                handle_scroll_down(win, server);
                break;
            default:
                break;
            }
        }

        advance_map(server, win);
    }

    // 'q' ou ESC pressionado
    delwin(win);
    open_menu(server);
}