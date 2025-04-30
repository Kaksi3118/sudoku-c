## Sudoku
A simple game of sudoku in C language :)

## Build

Requires a C compiler (GCC/MinGW or MSVC):

```bash
# GCC (Linux/macOS/Windows)
gcc -Wall -Wextra main.c sudoku.c io.c -o sudoku

# MSVC (Developer Command Prompt)
cl /W4 main.c sudoku.c io.c /Fe:sudoku.exe
```

## Run

```bash
./sudoku        # or sudoku.exe on Windows
```

1. Choose **New Game**  
2. Select board size (4×4, 9×9, 16×16)  
3. Select difficulty (Easy, Medium, Hard)  
4. Play:  
   - Enter moves as `row col value` (1-based indices)  
   - Erase with value `0`  
   - Type `C` to check current entries  
   - Type `0 0 0` to quit back to menu  
5. After quitting a game, you can save your state  
6. Back at the main menu, choose **Load Game** to resume

