#include "sudoku.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// This code checks if number is not used in the 3×3 box
static int unUsedInBox(int grid[MAX_N][MAX_N], int rowStart, int colStart, int num, int sr) {
    for (int i = 0; i < sr; i++)
        for (int j = 0; j < sr; j++)
            if (grid[rowStart+i][colStart+j] == num)
                return 0;
    return 1;
}
// This code fills a single box
static void fillBox(int grid[MAX_N][MAX_N], int row, int col, int sr) {
    int num;
    int max_val = sr * sr;          // 4 for 4×4, 9 for 9×9, 16 for 16×16
    for (int i = 0; i < sr; i++) {
        for (int j = 0; j < sr; j++) {
            do {
                num = (rand() % max_val) + 1;
            } while (!unUsedInBox(grid, row, col, num, sr));
            grid[row + i][col + j] = num;
        }
    }
}

// This code checks if number is not used in the row
static int usedInRow(int grid[MAX_N][MAX_N], int row, int num, int n) {
    for (int c = 0; c < n; c++)
        if (grid[row][c] == num)
            return 1;
    return 0;
}

// This code checks if number is not used in the column
static int usedInCol(int grid[MAX_N][MAX_N], int col, int num, int n) {
    for (int r = 0; r < n; r++)
        if (grid[r][col] == num)
            return 1;
    return 0;
}

// This code checks if it is safe to place number in the cell
static int checkIfSafe(int grid[MAX_N][MAX_N], int row, int col, int num, int n) {
    int sr = (int)sqrt(n);
    return !usedInRow(grid, row, num, n)
        && !usedInCol(grid, col, num, n)
        &&  unUsedInBox(grid, row - row % sr, col - col % sr, num, sr);
}

// This code checks if a move is valid
int isMoveValid(int puzzle[MAX_N][MAX_N],
                int row,
                int col,
                int val,
                int n)
{
    if (puzzle[row][col] != 0)           // must be empty
      return 0;
    return checkIfSafe(puzzle, row, col, val, n);
}

// This code fills the three diagonal 3×3 boxes
static void fillDiagonal(int grid[MAX_N][MAX_N], int n, int sr) {
    for (int i = 0; i < n; i += sr)
        fillBox(grid, i, i, sr);
}

// This code backtracks to fill remaining cells
static int fillRemaining(int grid[MAX_N][MAX_N], int row, int col, int n, int sr) {
    if (row == n) return 1;
    if (col == n) return fillRemaining(grid, row+1, 0, n, sr);
    if (grid[row][col] != 0) return fillRemaining(grid, row, col+1, n, sr);

    for (int num = 1; num <= n; num++) {
        if (checkIfSafe(grid, row, col, num, n)) {
            grid[row][col] = num;
            if (fillRemaining(grid, row, col+1, n, sr)) return 1;
            grid[row][col] = 0;
        }
    }
    return 0;
}

// This code removes "hints" cells at random
static void removeKDigits(int grid[MAX_N][MAX_N], int k, int n) {
    while (k > 0) {
        int cell = rand() % (n*n);
        int i = cell / n, j = cell % n;
        if (grid[i][j] != 0) {
            grid[i][j] = 0;
            k--;
        }
    }
}   

// This code generates a Sudoku puzzle with the given number of hints
void generateSudoku(int puzzle[MAX_N][MAX_N],
                    int solution[MAX_N][MAX_N],
                    int hints,
                    int n)
{
    int sr = (int)sqrt(n);
    srand((unsigned)time(NULL));

    // keep retrying until backtracking succeeds
    while (1) {
        // clear the solution grid
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                solution[i][j] = 0;

        fillDiagonal(solution, n, sr);
        if (fillRemaining(solution, 0, 0, n, sr))
            break;
    }

    // copy full solution into puzzle, then remove cells
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            puzzle[i][j] = solution[i][j];

    removeKDigits(puzzle, n*n - hints, n);
}

// New helper to validate a move
int isCorrectMove(int solution[MAX_N][MAX_N], int row, int col, int val) {
    return solution[row][col] == val;
}

// This code prints the NxN grid
void printGrid(int grid[MAX_N][MAX_N], int n) {
    int sr = (int)sqrt(n);

    // compute digits in 'n' (e.g. 1 for 4/9, 2 for 16) + space padding
    int maxValDigits = 0, tmp = n;
    while (tmp > 0) { maxValDigits++; tmp /= 10; }
    int width = maxValDigits + 1;

    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            // right-align in a field of 'width'
            printf("%*d", width, grid[r][c]);
            if ((c+1) % sr == 0 && c < n-1)
                printf(" |");
        }
        printf("\n");

        if ((r+1) % sr == 0 && r < n-1) {
            for (int c = 0; c < n; c++) {
                // dashes across the cell width
                for (int k = 0; k < width; k++) 
                    putchar('-');
                // extra dash + plus at each box boundary
                if ((c+1) % sr == 0 && c < n-1) {
                    putchar('-');
                    putchar('+');
                }
            }
            putchar('\n');
        }
    }
}