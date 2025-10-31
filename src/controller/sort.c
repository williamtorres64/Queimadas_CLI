#include "server.h"

// Merge-sort instrumentation counters
static unsigned long long ms_comparisons = 0;     // comparisons between elements
static unsigned long long ms_link_ops = 0;        // number of times we link nodes (assign result->next or pick result)
static unsigned long long ms_recursive_calls = 0; // number of recursive mergeSortRecursive calls
static unsigned long long ms_getmiddle_iters = 0; // iterations in getMiddle loop

static void ms_reset_counters(void)
{
    ms_comparisons = 0;
    ms_link_ops = 0;
    ms_recursive_calls = 0;
    ms_getmiddle_iters = 0;
}

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

void bubble_sort(Server *server, char ordenar_por)
{
    // Implementação do Bubble Sort
    if (!server || !server->queimadas)
        return;

    // If there's only one element, nothing to do
    if (!server->queimadas->next)
        return;

    bool swapped;
    do
    {
        swapped = false;
        Queimada *qi = server->queimadas; // reset to head for each pass

        // Walk the list comparing adjacent nodes
        while (qi != NULL && qi->next != NULL)
        {
            bool do_swap = false;
            switch (ordenar_por)
            {
            case 'd':
                if (qi->timestamp > qi->next->timestamp)
                    do_swap = true;
                break;
            case 'a':
                if (qi->lat > qi->next->lat)
                    do_swap = true;
                break;
            case 'o':
                if (qi->lon > qi->next->lon)
                    do_swap = true;
                break;
            case 'm':
                if (qi->municipioId > qi->next->municipioId)
                    do_swap = true;
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
}

static bool should_a_come_first(Queimada *a, Queimada *b, char ordenar_por)
{
    // Count each comparison decision made for merge sort
    ms_comparisons++;
    switch (ordenar_por)
    {
    case 'd': // Data/Timestamp
        // Assumes 'timestamp' is a field that can be compared
        return a->timestamp <= b->timestamp;
    case 'a': // Latitude
        return a->lat <= b->lat;
    case 'o': // Longitude
        return a->lon <= b->lon;
    case 'm': // Municipio ID
        return a->municipioId <= b->municipioId;
    default:
        // Default to sorting by timestamp if 'ordenar_por' is invalid
        return a->timestamp <= b->timestamp;
    }
}

/**
 * @brief Merges two sorted linked lists (a and b) into one sorted list.
 * @return The head of the new merged list.
 */
static Queimada *sortedMerge(Queimada *a, Queimada *b, char ordenar_por)
{
    // Base cases: if one list is empty, return the other
    if (a == NULL)
    {
        return b;
    }
    if (b == NULL)
    {
        return a;
    }

    Queimada *result = NULL;

    // Pick either 'a' or 'b' based on the comparison,
    // and recursively merge the rest of the lists.
    // choosing a or b is considered a link op
    ms_link_ops++;
    if (should_a_come_first(a, b, ordenar_por))
    {
        result = a;
        // assigning result->next is a link operation
        ms_link_ops++;
        result->next = sortedMerge(a->next, b, ordenar_por);
    }
    else
    {
        result = b;
        ms_link_ops++;
        result->next = sortedMerge(a, b->next, ordenar_por);
    }

    return result;
}

/**
 * @brief Finds the middle node of a linked list using the
 * fast/slow pointer technique.
 * @return The middle node (or the node just before the midpoint for
 * even-length lists).
 */
static Queimada *getMiddle(Queimada *head)
{
    if (head == NULL)
    {
        return head;
    }

    Queimada *slow = head;
    Queimada *fast = head->next; // Start fast one step ahead

    // Move fast by two steps and slow by one step
    while (fast != NULL)
    {
        // each loop iteration counts as a step toward finding middle
        ms_getmiddle_iters++;
        fast = fast->next;
        if (fast != NULL)
        {
            slow = slow->next;
            fast = fast->next;
        }
    }
    // 'slow' is now pointing to the node just before the midpoint,
    // which is what we need to split the list.
    return slow;
}

/**
 * @brief The main recursive helper function for Merge Sort.
 * @param head The head of the list to sort.
 * @param ordenar_por The sorting criteria.
 * @return The head of the *new*, sorted list.
 */
static Queimada *mergeSortRecursive(Queimada *head, char ordenar_por)
{
    // count recursive invocations
    ms_recursive_calls++;
    // Base case: list has 0 or 1 element, it's already sorted
    if (head == NULL || head->next == NULL)
    {
        return head;
    }

    // --- 1. DIVIDE ---
    // Find the middle of the list
    Queimada *middle = getMiddle(head);

    // Split the list into two halves
    Queimada *right_half = middle->next;
    middle->next = NULL; // IMPORTANT: sever the list into two
    Queimada *left_half = head;

    // --- 2. CONQUER ---
    // Recursively sort both halves
    Queimada *sorted_left = mergeSortRecursive(left_half, ordenar_por);
    Queimada *sorted_right = mergeSortRecursive(right_half, ordenar_por);

    // --- 3. COMBINE ---
    // Merge the two sorted halves
    return sortedMerge(sorted_left, sorted_right, ordenar_por);
}

/*
 * ===================================================================
 * PUBLIC MERGE SORT FUNCTION
 * ===================================================================
 */

/**
 * @brief Sorts the linked list of Queimada events in the Server struct
 * using the Merge Sort algorithm.
 * @param server The server object containing the list to sort.
 * @param ordenar_por The sorting criteria:
 * 'd' (data/timestamp), 'a' (latitude),
 * 'o' (longitude), 'm' (municipioId).
 */
void merge_sort(Server *server, char ordenar_por)
{
    // Check for edge cases: server is null, list is empty,
    // or list has only one element (already sorted).
    if (!server || !server->queimadas || !server->queimadas->next)
    {
        return;
    }

    // Reset instrumentation counters
    ms_reset_counters();

    // Call the recursive sort function on the head of the list
    Queimada *new_head = mergeSortRecursive(server->queimadas, ordenar_por);

    // Update the server's head pointer to point to the new, sorted list
    server->queimadas = new_head;
}