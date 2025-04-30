#include "io.h"
#include <stdio.h>

int saveGame(const char* filename,
             int puzzle[MAX_N][MAX_N],
             int solution[MAX_N][MAX_N],
             int n)
{
    FILE* f = fopen(filename, "wb");
    if (!f) return 0;
    if (fwrite(&n, sizeof(int), 1, f) != 1) { fclose(f); return 0; }
    for (int i = 0; i < n; i++)
        if (fwrite(puzzle[i], sizeof(int), n, f) != (size_t)n) { fclose(f); return 0; }
    for (int i = 0; i < n; i++)
        if (fwrite(solution[i], sizeof(int), n, f) != (size_t)n) { fclose(f); return 0; }
    fclose(f);
    return 1;
}

int loadGame(const char* filename,
             int puzzle[MAX_N][MAX_N],
             int solution[MAX_N][MAX_N],
             int* n_ptr)
{
    FILE* f = fopen(filename, "rb");
    if (!f) return 0;
    int n;
    if (fread(&n, sizeof(int), 1, f) != 1) { fclose(f); return 0; }
    if (n < 2 || n > MAX_N)          { fclose(f); return 0; }
    *n_ptr = n;
    for (int i = 0; i < n; i++)
        if (fread(puzzle[i], sizeof(int), n, f) != (size_t)n) { fclose(f); return 0; }
    for (int i = 0; i < n; i++)
        if (fread(solution[i], sizeof(int), n, f) != (size_t)n) { fclose(f); return 0; }
    fclose(f);
    return 1;
}