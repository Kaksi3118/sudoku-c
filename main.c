#include "sudoku.h"

int main() {
    int choice;
    printf("Sudoku Game\n");
    while (1) {
        printf("1. New Game\n2. Load Game\n3. Instructions\n4. Exit\nChoose: ");
        if (scanf("%d", &choice) != 1) break;
        switch (choice) {
            case 1:
                // TODO: New Game
                break;
            case 2:
                // TODO: Load Game
                break;
            case 3:
                // TODO: Instructions
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