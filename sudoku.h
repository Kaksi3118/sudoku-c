#ifndef SUDOKU_H
#define SUDOKU_H

// Generate a full 9×9 solution and remove (81 - hints) cells,
void generateSudoku(int grid[9][9], int hints);

// Print the 9×9 grid
void printGrid(int grid[9][9]);

#endif