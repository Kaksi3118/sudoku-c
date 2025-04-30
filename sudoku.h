#ifndef SUDOKU_H
#define SUDOKU_H

#define MAX_N 16

// Generate a full n×n solution and remove (n*n - hints) cells,
void generateSudoku(int puzzle[MAX_N][MAX_N],
                    int solution[MAX_N][MAX_N],
                    int hints,
                    int n);

// Check if a move is valid
int isMoveValid(int puzzle[MAX_N][MAX_N],
                int row,
                int col,
                int val,
                int n);

// Check if a move is correct
int isCorrectMove(int solution[MAX_N][MAX_N], int row, int col, int val);

// Print the n×n grid
void printGrid(int grid[MAX_N][MAX_N], int n);

#endif