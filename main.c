#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"
#include "io.h"

// forward‐declare so the compiler knows about it
void playGame(int puzzle[MAX_N][MAX_N],
              int solution[MAX_N][MAX_N],
              int n);
void loadGameMenu(void);

void newGame() {
    int sizeChoice, diffChoice, hints;
    printf("\nSelect board size:\n 1) 4 x 4  2) 9 x 9  3) 16 x 16\nChoice: ");
    if (scanf("%d", &sizeChoice)!=1) return;
    printf("Select difficulty:\n 1) Easy  2) Medium  3) Hard\nChoice: ");
    if (scanf("%d", &diffChoice)!=1) return;

    switch(diffChoice) {
      case 1: hints= (sizeChoice==1?6:  sizeChoice==2?40:  100); break;
      case 2: hints= (sizeChoice==1?5:  sizeChoice==2?30:  80); break;
      case 3: hints= (sizeChoice==1?4:  sizeChoice==2?20:  60); break;
      default: hints= (sizeChoice==1?5:  sizeChoice==2?30:  80);
    }

    int n = (sizeChoice==1?4 : sizeChoice==2?9 : 16);
    int puzzle[MAX_N][MAX_N], solution[MAX_N][MAX_N];
    generateSudoku(puzzle, solution, hints, n);
    playGame(puzzle, solution, n);

    // After play loop, offer to save
    {
      char ans;
      printf("Save this game? (y/n): ");
      if (scanf(" %c", &ans)==1 && (ans=='y'||ans=='Y')) {
        char fn[256];
        printf("Filename to save: ");
        if (scanf("%255s", fn)==1) {
          if (saveGame(fn, puzzle, solution, n))
            printf("Saved to '%s'.\n", fn);
          else
            printf("Failed to save.\n");
        }
      }
    }
}

void playGame(int puzzle[MAX_N][MAX_N], int solution[MAX_N][MAX_N], int n) {
    int fixed[MAX_N][MAX_N];
    // Build mask: 1 if this was a hint at start
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            fixed[i][j] = (puzzle[i][j] != 0);

    int row, col, val;
    while (1) {
        printGrid(puzzle, n);
        printf("Enter row col value (0 0 0 to quit): ");
        if (scanf("%d %d %d", &row, &col, &val) != 3) break;
        if (row == 0 && col == 0 && val == 0) break;
        if (row<1 || row>n || col<1 || col>n || val<0 || val>n) {
            printf("Invalid input.\n"); continue;
        }
        int r = row - 1, c = col - 1;
        // BLOCK any edit on a fixed (hint) cell:
        if (fixed[r][c]) {
            printf("Cell [%d,%d] is a given hint and cannot be changed.\n", row, col);
            continue;
        }
        if (val == 0) {
            // erase move
            puzzle[r][c] = 0;
            continue;
        }
        if (isMoveValid(puzzle, r, c, val, n)) {
        puzzle[r][c] = val;
        // only after the board is full do we do the final solution‐match:
        int done = 1;
        for (int i = 0; i < n && done; i++)
            for (int j = 0; j < n; j++)
                if (puzzle[i][j] != solution[i][j]) {
                    done = 0; break;
                }
        if (done) {
            printGrid(puzzle, n);
            printf("Congratulations, you solved it!\n");
            break;
        }
        } else {
            printf("Wrong move.\n");
        }
    }
}

// Called when user selects "Load Game"
void loadGameMenu(void) {
    char fn[256];
    printf("Enter save filename: ");
    if (scanf("%255s", fn)!=1) return;
    int puzzle[MAX_N][MAX_N], solution[MAX_N][MAX_N], n;
    if (!loadGame(fn, puzzle, solution, &n)) {
        printf("Failed to load '%s'.\n", fn);
        return;
    }
    playGame(puzzle, solution, n);
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
                loadGameMenu();
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