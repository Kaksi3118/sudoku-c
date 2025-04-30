#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdio.h>
#include <stdlib.h>
// Create NxN board
int **create_grid(int N);
// Free the grid
void free_grid(int **grid, int N);

void generate_sudoku(int **grid, int N, int B, int k);

// Print the Sudoku board
void print_grid(int **grid, int N);

int save_game(const char *filename, int **grid, int N, int k_remaining);

int load_game(const char *filename, int ***grid_out, int *N_out, int *k_remaining_out);

#endif