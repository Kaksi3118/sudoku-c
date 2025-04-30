#ifndef SUDOKU_H
#define SUDOKU_H

// Generate a full 9×9 solution and remove (81 - hints) cells,
void generateSudoku(int puzzle[9][9], int solution[9][9], int hints);

// Check if a move is valid
int isMoveValid(int puzzle[9][9], int row, int col, int val);

// Check if a move is correct
int isCorrectMove(int solution[9][9], int row, int col, int val);

// Print the 9×9 grid
void printGrid(int grid[9][9]);

#endif