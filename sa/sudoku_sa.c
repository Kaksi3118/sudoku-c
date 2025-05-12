#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define N 9
#define MAX_N 9
#define BLOCK_SIZE 3

// Sudoku generator code (from sudoku.c)
static int unUsedInBox(int grid[MAX_N][MAX_N], int rowStart, int colStart, int num, int sr) {
    for (int i = 0; i < sr; i++)
        for (int j = 0; j < sr; j++)
            if (grid[rowStart+i][colStart+j] == num)
                return 0;
    return 1;
}

static void fillBox(int grid[MAX_N][MAX_N], int row, int col, int sr) {
    int num;
    int max_val = sr * sr;
    for (int i = 0; i < sr; i++) {
        for (int j = 0; j < sr; j++) {
            do {
                num = (rand() % max_val) + 1;
            } while (!unUsedInBox(grid, row, col, num, sr));
            grid[row + i][col + j] = num;
        }
    }
}

static int usedInRow(int grid[MAX_N][MAX_N], int row, int num, int n) {
    for (int c = 0; c < n; c++)
        if (grid[row][c] == num)
            return 1;
    return 0;
}

static int usedInCol(int grid[MAX_N][MAX_N], int col, int num, int n) {
    for (int r = 0; r < n; r++)
        if (grid[r][col] == num)
            return 1;
    return 0;
}

static int checkIfSafe(int grid[MAX_N][MAX_N], int row, int col, int num, int n) {
    int sr = (int)sqrt(n);
    return !usedInRow(grid, row, num, n)
        && !usedInCol(grid, col, num, n)
        &&  unUsedInBox(grid, row - row % sr, col - col % sr, num, sr);
}

static void fillDiagonal(int grid[MAX_N][MAX_N], int n, int sr) {
    for (int i = 0; i < n; i += sr)
        fillBox(grid, i, i, sr);
}

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

void generateSudoku(int puzzle[MAX_N][MAX_N], int solution[MAX_N][MAX_N], int hints, int n) {
    int sr = (int)sqrt(n);

    while (1) {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                solution[i][j] = 0;

        fillDiagonal(solution, n, sr);
        if (fillRemaining(solution, 0, 0, n, sr))
            break;
    }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            puzzle[i][j] = solution[i][j];

    removeKDigits(puzzle, n*n - hints, n);
}

// --- SA Solver code ---
typedef struct {
    int grid[MAX_N][MAX_N];
    int fixed[MAX_N][MAX_N];
    double energy;
} SudokuState;

double calculate_energy(SudokuState *state) {
    double energy = 0;
    for (int i = 0; i < N; i++) {
        int count[N+1] = {0};
        for (int j = 0; j < N; j++) {
            int val = state->grid[i][j];
            if (val >= 1 && val <= 9)
                count[val]++;
        }
        for (int k = 1; k <= N; k++) {
            if (count[k] > 1)
                energy += count[k] - 1;
        }
    }
    for (int j = 0; j < N; j++) {
        int count[N+1] = {0};
        for (int i = 0; i < N; i++) {
            int val = state->grid[i][j];
            if (val >= 1 && val <= 9)
                count[val]++;
        }
        for (int k = 1; k <= N; k++) {
            if (count[k] > 1)
                energy += count[k] - 1;
        }
    }
    return energy;
}

void initialize_state(SudokuState *state, int puzzle[MAX_N][MAX_N]) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            state->grid[i][j] = puzzle[i][j];
            state->fixed[i][j] = (puzzle[i][j] != 0);
        }
    for (int block = 0; block < N; block++) {
        int start_row = (block / 3) * 3;
        int start_col = (block % 3) * 3;
        int present[10] = {0};
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++) {
                int val = state->grid[start_row + i][start_col + j];
                if (val != 0)
                    present[val] = 1;
            }
        int missing[9];
        int missing_count = 0;
        for (int num = 1; num <= 9; num++)
            if (!present[num])
                missing[missing_count++] = num;
        // Shuffle missing digits
        for (int i = missing_count - 1; i > 0; i--) {
            int r = rand() % (i + 1);
            int tmp = missing[i];
            missing[i] = missing[r];
            missing[r] = tmp;
        }
        int idx = 0;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (!state->fixed[start_row + i][start_col + j])
                    state->grid[start_row + i][start_col + j] = missing[idx++];
    }
    state->energy = calculate_energy(state);
}

void generate_neighbor(SudokuState *neighbor, const SudokuState *current) {
    memcpy(neighbor->grid, current->grid, sizeof(current->grid));
    memcpy(neighbor->fixed, current->fixed, sizeof(current->fixed));
    int block_row = rand() % 3;
    int block_col = rand() % 3;
    int cell1_row, cell1_col, cell2_row, cell2_col;
    do {
        cell1_row = block_row * 3 + rand() % 3;
        cell1_col = block_col * 3 + rand() % 3;
    } while (neighbor->fixed[cell1_row][cell1_col]);
    do {
        cell2_row = block_row * 3 + rand() % 3;
        cell2_col = block_col * 3 + rand() % 3;
    } while (neighbor->fixed[cell2_row][cell2_col] || (cell2_row == cell1_row && cell2_col == cell1_col));
    int tmp = neighbor->grid[cell1_row][cell1_col];
    neighbor->grid[cell1_row][cell1_col] = neighbor->grid[cell2_row][cell2_col];
    neighbor->grid[cell2_row][cell2_col] = tmp;
    neighbor->energy = calculate_energy(neighbor);
}

void print_grid(const SudokuState *state) {
    printf("\n");
    for (int i = 0; i < N; i++) {
        if (i % 3 == 0 && i != 0)
            printf("---------------------\n");
        for (int j = 0; j < N; j++) {
            if (j % 3 == 0 && j != 0)
                printf("| ");
            printf("%d ", state->grid[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void print_puzzle(int puzzle[MAX_N][MAX_N]) {
    printf("\nGenerated Sudoku puzzle (0 = empty cell):\n\n");
    for (int i = 0; i < N; i++) {
        if (i % 3 == 0 && i != 0)
            printf("---------------------\n");
        for (int j = 0; j < N; j++) {
            if (j % 3 == 0 && j != 0)
                printf("| ");
            printf("%d ", puzzle[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void solve_sudoku_sa(double T_start, double T_end, double alpha, int max_iterations) {
    srand(time(NULL));
    int puzzle[MAX_N][MAX_N], solution[MAX_N][MAX_N];
    generateSudoku(puzzle, solution, 30, N);

    print_puzzle(puzzle);

    SudokuState current_state, neighbor_state, best_state;
    initialize_state(&current_state, puzzle);
    memcpy(&best_state, &current_state, sizeof(SudokuState));

    printf("Initial random block-valid state (may have row/col conflicts):\n");
    print_grid(&current_state);

    double T = T_start;
    int iteration = 0;

    printf("Initial energy: %.2f\n", current_state.energy);

    // Store last 3 iteration info
    int last_iters[3] = {-1, -1, -1};
    double last_current_energy[3] = {-1, -1, -1};
    double last_best_energy[3] = {-1, -1, -1};

    while (iteration < max_iterations && best_state.energy > 0 && T > T_end) {
        generate_neighbor(&neighbor_state, &current_state);
        double delta_E = neighbor_state.energy - current_state.energy;
        if (delta_E < 0 || ((double)rand() / RAND_MAX) < exp(-delta_E / T)) {
            memcpy(&current_state, &neighbor_state, sizeof(SudokuState));
            if (current_state.energy < best_state.energy) {
                memcpy(&best_state, &current_state, sizeof(SudokuState));
            }
        }

        // Every 100 iterations print progress
        if (iteration % 100 == 0) {
            printf("Iter: %d, Current energy: %.2f, Best energy: %.2f\n", iteration, current_state.energy, best_state.energy);
        }

        // Update last 3 iteration info (shift left and add current)
        last_iters[0] = last_iters[1];
        last_current_energy[0] = last_current_energy[1];
        last_best_energy[0] = last_best_energy[1];

        last_iters[1] = last_iters[2];
        last_current_energy[1] = last_current_energy[2];
        last_best_energy[1] = last_best_energy[2];

        last_iters[2] = iteration;
        last_current_energy[2] = current_state.energy;
        last_best_energy[2] = best_state.energy;

        T *= alpha;
        iteration++;

        if (best_state.energy == 0) {
            printf("\n--- Last 3 iterations before solution ---\n");
            for (int i = 0; i < 3; i++) {
                if (last_iters[i] >= 0) {
                    printf("Iter: %d, Current energy: %.2f, Best energy: %.2f\n",
                        last_iters[i], last_current_energy[i], last_best_energy[i]);
                }
            }
            break;
        }
    }

    printf("\nSolved Sudoku (energy: %.2f, iterations: %d):\n", best_state.energy, iteration);
    print_grid(&best_state);
}


int main() {
    double T_start = 100.0;
    double T_end = 0.01;
    double alpha = 0.99995;
    int max_iterations = 1000000;

    solve_sudoku_sa(T_start, T_end, alpha, max_iterations);

    printf("Press Enter to exit...");
    fflush(stdout);
    getchar();
    return 0;
}
