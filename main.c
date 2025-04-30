#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"

// This code starts a new game
void newGame() {
    int sizeChoice, diffChoice, hints;
    printf("\nSelect board size:\n 1) 4×4  2) 9×9  3) 16×16\nChoice: ");
    if (scanf("%d", &sizeChoice) != 1) return;

    printf("Select difficulty:\n 1) Easy  2) Medium  3) Hard\nChoice: ");
    if (scanf("%d", &diffChoice) != 1) return;

    // map difficulty → # visible cells
    switch (diffChoice) {
      case 1: hints = 40; break;  // easy
      case 2: hints = 30; break;  // medium
      case 3: hints = 20; break;  // hard
      default: hints = 30;
    }

// For now only 9x9 grid is implemented
    if (sizeChoice == 2) {
        int grid[9][9];
        generateSudoku(grid, hints);
        printGrid(grid);
    } else {
        printf("Board size not implemented yet.\n");
    }
}

// This code is the main function
int main() {
    int choice;
    printf("Sudoku Game\n");
    while (1) {
        printf("\n1. New Game\n2. Load Game\n3. Instructions\n4. Exit\nChoose: ");
        if (scanf("%d", &choice) != 1) break;
        switch (choice) {
            case 1:
                newGame();
                break;
            case 2:
                printf("Load Game not implemented yet.\n");
                break;
            case 3:
                printf("Instructions not implemented yet.\n");
                break;
            case 4:
                printf("Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice.\n");
        }
    }
    return 0;
}