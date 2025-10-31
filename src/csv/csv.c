#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../tipos.h"

// Trim whitespace (spaces, tabs, CR, LF) in-place from both ends
static void trim_inplace(char *s)
{
    if (!s || !*s)
        return;
    char *start = s;
    while (*start && isspace((unsigned char)*start))
        start++;
    if (start != s)
        memmove(s, start, strlen(start) + 1);
    char *end = s + strlen(s) - 1;
    while (end >= s && isspace((unsigned char)*end))
    {
        *end = '\0';
        end--;
    }
}

// normalize common byte artifacts: strip UTF-8 BOM and replace NBSP (0xC2 0xA0) with normal space 
static void normalize_utf8_bytes(char *s)
{
    if (!s || !*s)
        return;

    unsigned char *p = (unsigned char *)s;

    // strip BOM if present (0xEF 0xBB 0xBF)
    if (p[0] == 0xEF && p[1] == 0xBB && p[2] == 0xBF)
    {
        memmove(s, s + 3, strlen(s + 3) + 1);
        p = (unsigned char *)s;
    }

    // Replace non-breaking space sequence (0xC2 0xA0) with regular space (0x20)
    // Walk through bytes, copying into a write pointer.
    unsigned char *read = p;
    unsigned char *write = p;
    while (*read)
    {
        if (read[0] == 0xC2 && read[1] == 0xA0)
        {
            *write++ = ' ';
            read += 2;
            continue;
        }
        // otherwise copy current byte
        *write++ = *read++;
    }
    *write = '\0';
}

// Strip surrounding double quotes, handling leading/trailing whitespace and CR/LF
static void strip_quotes(char *str)
{
    if (!str)
        return;
    // normalize before trimming/quoting so stray byte sequences are fixed
    normalize_utf8_bytes(str);
    trim_inplace(str);
    size_t len = strlen(str);
    if (len >= 2 && str[0] == '"' && str[len - 1] == '"')
    {
        // remove trailing quote then shift to remove leading quote
        str[len - 1] = '\0';
        memmove(str, str + 1, len - 1);
    }
    // normalize again in case trimming/extraction revealed sequences
    normalize_utf8_bytes(str);
    trim_inplace(str);
}

Bioma *lerBiomaCSV(const char *filename)
{
    Bioma *head = NULL;
    Bioma *tail = NULL;
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir arquivo de biomas");
        return NULL;
    }

    char line[256];
    // Pular o cabeçalho
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        Bioma *novoBioma = malloc(sizeof(Bioma));
        if (!novoBioma) continue;
        char nomeBuf[256] = {0};
        if (sscanf(line, "%lu,%255[^\n]", &novoBioma->id, nomeBuf) != 2)
        {
            free(novoBioma);
            continue; // Linha mal formatada
        }
        strip_quotes(nomeBuf);
        novoBioma->nome = malloc(strlen(nomeBuf) + 1);
        if (novoBioma->nome) strcpy(novoBioma->nome, nomeBuf);
        novoBioma->next = NULL;
        novoBioma->prev = tail;

        if (!head)
        {
            head = novoBioma;
        }
        else
        {
            tail->next = novoBioma;
        }
        tail = novoBioma;
    }

    fclose(file);
    return head;
}

Estado *lerEstadoCSV(const char *filename)
{
    Estado *head = NULL;
    Estado *tail = NULL;
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir arquivo de estados");
        return NULL;
    }

    char line[256];
    // Pular o cabeçalho
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        Estado *novoEstado = malloc(sizeof(Estado));
        if (!novoEstado) continue;
        char nomeBuf[256] = {0};
        if (sscanf(line, "%lu,%255[^\n]", &novoEstado->id, nomeBuf) != 2) {
            free(novoEstado);
            continue; // Linha mal formatada
        }
        strip_quotes(nomeBuf);
        novoEstado->nome = malloc(strlen(nomeBuf) + 1);
        if (novoEstado->nome) strcpy(novoEstado->nome, nomeBuf);
        novoEstado->next = NULL;
        novoEstado->prev = tail;

        if (!head)
        {
            head = novoEstado;
        }
        else
        {
            tail->next = novoEstado;
        }
        tail = novoEstado;
    }

    fclose(file);
    return head;
}

Municipio *lerMunicipioCSV(const char *filename)
{
    Municipio *head = NULL;
    Municipio *tail = NULL;
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir arquivo de municípios");
        return NULL;
    }

    char line[256];
    // Pular o cabeçalho
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        Municipio *novoMunicipio = malloc(sizeof(Municipio));
        if (!novoMunicipio) continue;
        char nomeBuf[256] = {0};
        if (sscanf(line, "%lu,%255[^\n]", &novoMunicipio->id, nomeBuf) != 2) {
            free(novoMunicipio);
            continue; // Linha mal formatada
        }
        strip_quotes(nomeBuf);
        novoMunicipio->nome = malloc(strlen(nomeBuf) + 1);
        if (novoMunicipio->nome) strcpy(novoMunicipio->nome, nomeBuf);
        novoMunicipio->next = NULL;
        novoMunicipio->prev = tail;

        if (!head)
        {
            head = novoMunicipio;
        }
        else
        {
            tail->next = novoMunicipio;
        }
        tail = novoMunicipio;
    }

    fclose(file);
    return head;
}

Queimada *lerQueimadaCSV(const char *filename)
{
    Queimada *head = NULL;
    Queimada *tail = NULL;
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir arquivo de queimadas ");
        return NULL;
    }

    char line[512];
    // Pular o cabeçalho
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        Queimada *novaQueimada = malloc(sizeof(Queimada));
        if (!novaQueimada)
            continue;
        char dataBuf[16] = {0}; // Buffer for data string
        char horaBuf[8] = {0};  // Buffer for hora string
        if (sscanf(line, "%lu,%f,%f,%15[^,],%7[^,],%d,%d,%d,%d",
                   &novaQueimada->id,
                   &novaQueimada->lat,
                   &novaQueimada->lon,
                   dataBuf,
                   horaBuf,
                   &novaQueimada->timestamp,
                   &novaQueimada->estadoId,
                   &novaQueimada->municipioId,
                   &novaQueimada->biomaId) != 9)
        {
            free(novaQueimada);
            // Linha mal formatada
            perror("Linha mal formatada em queimadas CSV");
            continue;
        }
        strip_quotes(dataBuf);
        strip_quotes(horaBuf);
        // Allocate and copy data
        novaQueimada->data = malloc(strlen(dataBuf) + 1);
        if (novaQueimada->data)
            strcpy(novaQueimada->data, dataBuf);
        // Allocate and copy hora
        novaQueimada->hora = malloc(strlen(horaBuf) + 1);
        if (novaQueimada->hora)
            strcpy(novaQueimada->hora, horaBuf);
        novaQueimada->next = NULL;
        novaQueimada->prev = tail;

        if (!head)
        {
            head = novaQueimada;
        }
        else
        {
            tail->next = novaQueimada;
        }
        tail = novaQueimada;
    }

    fclose(file);
    return head;
}