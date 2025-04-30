![wiedźminszyn](https://github.com/user-attachments/assets/1eb5e892-0de0-4485-9b21-a25ea017f03c)

## Sudoku in C!

A simple console game of sudoku in C language :)

sudoku.exe tested only in 64-bit Windows

## Screenshots from console

Menu

![menu](https://github.com/user-attachments/assets/2dc84403-1d68-43c5-8331-d553e911475a)

Selecting grid size and difficulty

![selecting](https://github.com/user-attachments/assets/ca3e96f0-b71e-4728-910a-d15c17d5155c)

Instruction

![instructions](https://github.com/user-attachments/assets/be5a2a78-aa07-48aa-aba6-5d2a6bae7f81)

Sample game with 1 move and game save

![example](https://github.com/user-attachments/assets/9ba11a8b-2323-4170-b2cc-aba68af4a06f)

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
 
## Known Issues

Currently unknown
