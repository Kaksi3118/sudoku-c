#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"
void playGame(int puzzle[9][9], int solution[9][9]) {
    int row, col, val;
    while (1) {
        printGrid(puzzle);
        printf("Enter row col value (0 0 0 to quit): ");
        if (scanf("%d %d %d", &row, &col, &val) != 3) break;
        if (row == 0 && col == 0 && val == 0) break;
        if (row<1 || row>9 || col<1 || col>9 || val<0 || val>9) {
            printf("Invalid input.\n"); continue;
        }
        if (val == 0) {
            // erase move
            puzzle[row-1][col-1] = 0;
            continue;
        }
        if (isCorrectMove(solution, row-1, col-1, val)) {
            puzzle[row-1][col-1] = val;
            // check if solved
            int done = 1;
            for (int i = 0; i < 9 && done; i++)
                for (int j = 0; j < 9; j++)
                    if (puzzle[i][j] != solution[i][j]) {
                        done = 0; break;
                    }
            if (done) {
                printGrid(puzzle);
                printf("Congratulations, you solved it!\n");
                break;
            }
        } else {
            printf("Wrong move.\n");
        }
    }
}
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
        int puzzle[9][9], solution[9][9];
        generateSudoku(puzzle, solution, hints);
        playGame(puzzle, solution);
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