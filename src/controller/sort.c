#include "server.h"

void swap_queimadas(Queimada *a, Queimada *b)
{
    if (!a || !b || a == b)
        return;

    // Salvar conexões das queimadas
    Queimada *a_prev = a->prev;
    Queimada *a_next = a->next;
    Queimada *b_prev = b->prev;
    Queimada *b_next = b->next;

    // Trocar os conteúdos
    Queimada tmp = *a;
    *a = *b;
    *b = tmp;

    // Restaurar as conexões
    a->prev = a_prev;
    a->next = a_next;
    b->prev = b_prev;
    b->next = b_next;
}

void bubble_sort(Server *server)
{
    if (!server || !server->queimadas)
        return;

    if (!server->queimadas->next)
        return;

    int comparisons = 0;
    bool swapped;
    do
    {
        swapped = false;
        Queimada *qi = server->queimadas;

        while (qi != NULL && qi->next != NULL)
        {
            bool do_swap = false;
            switch (server->sort_by)
            {
            case 'd':
                if (qi->timestamp > qi->next->timestamp)
                    do_swap = true;
                comparisons++;
                break;
            case 'a':
                if (qi->lat > qi->next->lat)
                    do_swap = true;
                comparisons++;
                break;
            case 'o':
                if (qi->lon > qi->next->lon)
                    do_swap = true;
                comparisons++;
                break;
            case 'm':
                if (qi->municipioId > qi->next->municipioId)
                    do_swap = true;
                comparisons++;
                break;
            default:
                break;
            }

            if (do_swap)
            {
                swap_queimadas(qi, qi->next);
                swapped = true;
            }

            qi = qi->next;
        }
    } while (swapped);
    server->comparacoes = comparisons;
}

// Determina se 'a' deve vir antes de 'b' segundo o critério de ordenação.
static bool should_a_come_first(Queimada *a, Queimada *b, char ordenar_por, Server *server)
{
    switch (ordenar_por)
    {
    case 'd':
        server->comparacoes++;
        return a->timestamp <= b->timestamp;
    case 'a':
        server->comparacoes++;
        return a->lat <= b->lat;
    case 'o':
        server->comparacoes++;
        return a->lon <= b->lon;
    case 'm':
        server->comparacoes++;
        return a->municipioId <= b->municipioId;
    default:
        server->comparacoes++;
        return a->timestamp <= b->timestamp;
    }
}

// Mescla duas listas ordenadas em uma única lista ordenada.
static Queimada *sortedMerge(Queimada *a, Queimada *b, char ordenar_por, Server *server)
{
    if (a == NULL)
    {
        return b;
    }
    if (b == NULL)
    {
        return a;
    }

    Queimada *result = NULL;

    if (should_a_come_first(a, b, ordenar_por, server))
    {
        result = a;
        result->next = sortedMerge(a->next, b, ordenar_por, server);
    }
    else
    {
        result = b;
        result->next = sortedMerge(a, b->next, ordenar_por, server);
    }

    return result;
}

// Retorna o nó do meio da lista (algorítmo fast/slow pointer)
static Queimada *getMiddle(Queimada *head)
{
    if (head == NULL)
    {
        return head;
    }

    Queimada *slow = head;
    Queimada *fast = head->next;

    while (fast != NULL)
    {
        fast = fast->next;
        if (fast != NULL)
        {
            slow = slow->next;
            fast = fast->next;
        }
    }
    return slow;
}

// Função recursiva do merge sort para listas encadeadas.
// Divide a lista, ordena as metades e mescla o resultado.
static Queimada *mergeSortRecursive(Queimada *head, char ordenar_por, Server *server)
{
    if (head == NULL || head->next == NULL)
    {
        return head;
    }

    Queimada *middle = getMiddle(head);

    Queimada *right_half = middle->next;
    middle->next = NULL;
    Queimada *left_half = head;

    Queimada *sorted_left = mergeSortRecursive(left_half, ordenar_por, server);
    Queimada *sorted_right = mergeSortRecursive(right_half, ordenar_por, server);

    return sortedMerge(sorted_left, sorted_right, ordenar_por, server);
}

// Interface para ordenar as queimadas usando merge sort.
void merge_sort(Server *server)
{
    if (!server || !server->queimadas || !server->queimadas->next)
    {
        return;
    }

    server->comparacoes = 0;

    Queimada *new_head = mergeSortRecursive(server->queimadas, server->sort_by, server);

    server->queimadas = new_head;
}