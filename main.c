#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sudoku.h"
#include "io.h"
#include <time.h>

// forward‐declare so the compiler knows about it
void playGame(int puzzle[MAX_N][MAX_N],
              int solution[MAX_N][MAX_N],
              int n);
void loadGameMenu(void);
void printInstructions(void);

void newGame() {
    int sizeChoice, diffChoice, hints;
    printf("\nSelect board size:\n 1) 4 x 4  2) 9 x 9  3) 16 x 16\nChoice: ");
    if (scanf("%d", &sizeChoice)!=1) return;
    printf("Select difficulty:\n 1) Easy  2) Medium  3) Hard\nChoice: ");
    if (scanf("%d", &diffChoice)!=1) return;

    // Added the line to stop the "doubling the tables" the first time you load the board
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
    
    // This line is used to set the number of hints based on the difficulty and board size
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

    // Skip save prompt if puzzle is solved
    int solved_flag = 1;
    for (int i = 0; i < n && solved_flag; ++i)
        for (int j = 0; j < n; ++j)
            if (puzzle[i][j] != solution[i][j])
                solved_flag = 0;

    if (!solved_flag) {
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
}

// This function is used to play the game
void playGame(int puzzle[MAX_N][MAX_N], int solution[MAX_N][MAX_N], int n) {
    time_t start = time(NULL);
    int moves = 0;

    // This is the mask that is used to check if the cell is a hint
    int fixed[MAX_N][MAX_N];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            fixed[i][j] = (puzzle[i][j] != 0);

    char line[100];
    int row, col, val;
    // This is the main loop that runs the game
    while (1) {
        printGrid(puzzle, n);
        printf("Enter row col val (0 0 0 to quit), or 'C' to check entries: ");
        if (!fgets(line, sizeof line, stdin)) break;
        // skip blank lines
        if (line[0] == '\n') continue;

        // Check‐entries command
        if (line[0]=='C' || line[0]=='c') {
            int wrong = 0;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (!fixed[i][j] && puzzle[i][j] != 0
                        && puzzle[i][j] != solution[i][j]) {
                        printf("Wrong at [%d,%d]: you have %d, should be %d\n",
                               i+1, j+1,
                               puzzle[i][j], solution[i][j]);
                        wrong++;
                    }
                }
            }
            if (!wrong)
                printf("All current entries are correct!\n");
            continue;
        }

        // Otherwise parse a move
        if (sscanf(line, "%d %d %d", &row, &col, &val) != 3) {
            printf("Invalid input.\n");
            continue;
        }
        if (row==0 && col==0 && val==0) break;
        if (row<1||row>n||col<1||col>n||val<0||val>n) {
            printf("Invalid input.\n");
            continue;
        }
        int r = row-1, c = col-1;
        if (fixed[r][c]) {
            printf("Cell [%d,%d] is a given hint and cannot be changed.\n",
                   row, col);
            continue;
        }
        if (val == 0) {
            puzzle[r][c] = 0;
            moves++;
            continue;
        }
        if (isMoveValid(puzzle, r, c, val, n)) {
            puzzle[r][c] = val;
            moves++;
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

    // print stats
    time_t end = time(NULL);
    int secs = (int)difftime(end, start);
    printf("\nGame over. Moves: %d, Time: %02d:%02d\n\n",
           moves, secs/60, secs%60);
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

// This function is used to print the instructions
void printInstructions(void) {
    printf("\nInstructions:\n");
    printf("  • Fill each row, column and box with all values 1..n once.\n");
    printf("  • Enter moves as: row col value  (e.g. 3 4 5)\n");
    printf("  • Erase with value=0 (e.g. 3 4 0)\n");
    printf("  • Quit game: 0 0 0\n");
    printf("  • At any point, type 'C' to check your current entries.\n\n");
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
                printInstructions();
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