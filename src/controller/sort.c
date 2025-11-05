#include "server.h"

void swap_queimadas(Queimada *a, Queimada *b)
{
    Queimada temp = *b;
    b->id = a->id;
    b->lat = a->lat;
    b->lon = a->lon;
    b->data = a->data;
    b->hora = a->hora;
    b->timestamp = a->timestamp;
    b->estadoId = a->estadoId;
    b->municipioId = a->municipioId;
    b->biomaId = a->biomaId;

    a->id = temp.id;
    a->lat = temp.lat;
    a->lon = temp.lon;
    a->data = temp.data;
    a->hora = temp.hora;
    a->timestamp = temp.timestamp;
    a->estadoId = temp.estadoId;
    a->municipioId = temp.municipioId;
    a->biomaId = temp.biomaId;
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