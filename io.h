#ifndef IO_H
#define IO_H

#include "sudoku.h"

// Save a game: writes board size, then puzzle[n×n], then solution[n×n]
// Returns 1 on success, 0 on failure.
int saveGame(const char* filename,
             int puzzle[MAX_N][MAX_N],
             int solution[MAX_N][MAX_N],
             int n);

// Load a game: reads board size into *n_ptr, then puzzle[n×n], then solution[n×n]
// Returns 1 on success, 0 on failure.
int loadGame(const char* filename,
             int puzzle[MAX_N][MAX_N],
             int solution[MAX_N][MAX_N],
             int* n_ptr);

#endif