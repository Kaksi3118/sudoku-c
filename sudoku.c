#include "sudoku.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// This code checks if number is not used in the 3×3 box
static int unUsedInBox(int grid[9][9], int rowStart, int colStart, int num) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[rowStart+i][colStart+j] == num)
                return 0;
    return 1;
}
// This code fills a single 3×3 box at (row,col)
static void fillBox(int grid[9][9], int row, int col) {
    int num;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            do {
                num = (rand() % 9) + 1;
            } while (!unUsedInBox(grid, row, col, num));
            grid[row+i][col+j] = num;
        }
}

// This code checks if number is not used in the row
static int usedInRow(int grid[9][9], int row, int num) {
    for (int i = 0; i < 9; i++)
        if (grid[row][i] == num)
            return 1;
    return 0;
}

// This code checks if number is not used in the column
static int usedInCol(int grid[9][9], int col, int num) {
    for (int i = 0; i < 9; i++)
        if (grid[i][col] == num)
            return 1;
    return 0;
}

// This code checks if it is safe to place number in the cell
static int checkSafe(int grid[9][9], int row, int col, int num) {
    return !usedInRow(grid, row, num) && !usedInCol(grid, col, num) && !unUsedInBox(grid, row - row % 3, col - col % 3, num);
}

// This code fills the three diagonal 3×3 boxes
static void fillDiagonal(int grid[9][9]) {
    for (int i = 0; i < 9; i += 3)
        fillBox(grid, i, i);
}

// This code backtracks to fill remaining cells
static int fillRemaining(int grid[9][9], int row, int col) {
    if (row == 9) return 1;
    if (col == 9) return fillRemaining(grid, row+1, 0);
    if (grid[row][col] != 0) return fillRemaining(grid, row, col+1);

    for (int num = 1; num <= 9; num++) {
        if (checkIfSafe(grid,row,col,num)) {
            grid[row][col] = num;
            if (fillRemaining(grid,row,col+1)) return 1;
            grid[row][col] = 0;
        }
    }
    return 0;
}

// This code removes "hints" cells at random
static void removeKDigits(int grid[9][9], int k) {
    while (k > 0) {
        int cell = rand() % 81;
        int i = cell / 9, j = cell % 9;
        if (grid[i][j] != 0) {
            grid[i][j] = 0;
            k--;
        }
    }
}   

// This code generates a Sudoku puzzle with the given number of hints
void generateSudoku(int grid[9][9], int hints) {
    // seed once
    srand((unsigned)time(NULL));
    // zero‐out
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            grid[i][j] = 0;

    fillDiagonal(grid);
    fillRemaining(grid, 0, 0);
    // leave exactly 'hints' cells visible:
    removeKDigits(grid, 81 - hints);
}

// This code prints the 9×9 grid
void printGrid(int grid[9][9]) {
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            printf("%2d", grid[r][c]);
            if ((c+1)%3 == 0 && c < 8) printf(" |");
        }
        printf("\n");
        if ((r+1)%3 == 0 && r < 8)
            printf("------+-------+------\n");
    }
}