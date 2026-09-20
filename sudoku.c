#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Assume: string != NULL */
static void string_tolower(char *string) {
    while (*string) {
        *string = tolower((unsigned char)*string);
        ++string;
    }
}

/* Assume: infile_str != NULL && infile != NULL */
static int validate_infile(const char *infile_str, FILE **infile) {
    *infile = fopen(infile_str, "rb");

    if (!*infile) {
        int errno_copy = errno;

        fprintf(stderr, "Error: can not open <input_file> at '%s' (%s)\n",
                infile_str, strerror(errno_copy));
        return -1;
    }

    return 0;
}

/* Assume: infile != NULL */
static int load_grid(int grid[9][9], FILE *infile) {
    char buffer[32];
    int row = 0;

    while (fgets(buffer, sizeof(buffer), infile)) {
        int i = 0;
        int col = 0;
        char c = buffer[i];

        while (c != '\0' && c != '\n') {
            if (c == ' ' || c == '\t' || c == '\r') {
                ++i;
            } else if (c == '.' || c == '0' || c == 'O' || c == 'o') {
                if (row >= 9 || col >= 9) {
                    fprintf(stderr, "Error: rows and columns must be exactly 9\n");
                    return -1;
                }

                grid[row][col] = 0;
                ++col;
                ++i;
            } else if (c >= '1' && c <= '9') {
                if (row >= 9 || col >= 9) {
                    fprintf(stderr, "Error: rows and columns must be exactly 9\n");
                    return -1;
                }

                grid[row][col] = c - '0';
                ++col;
                ++i;
            } else {
                fprintf(stderr, "Error: character %c found, "
                        "use [1-9] for numbers and [.0Oo] for unknowns\n", c);
                return -1;
            }

            c = buffer[i];
        }

        if (col > 0) {
            ++row;

            if (col != 9) {
                fprintf(stderr, "Error: %d columns found at row %d, must be exactly 9\n", col, row);
                return -1;
            }
        }
    }

    if (row != 9) {
        fprintf(stderr, "Error: %d rows found, must be exactly 9\n", row);
        return -1;
    }

    if (ferror(infile)) {
        fprintf(stderr, "Error: reading error\n");
        return -1;
    }

    return 0;
}


static void print_grid(int grid[9][9]) {
    int i, j;

    printf("┏━━━┳━━━┳━━━┳━━━┳━━━┳━━━┳━━━┳━━━┳━━━┓\n");

    for (i = 0; i < 9; ++i) {
        printf("┃");

        for (j = 0; j < 9; ++j) {
            if (grid[i][j] == 0) {
                printf(" • ┃");
            } else {
                printf(" %d ┃", grid[i][j]);
            }
        }
        printf("\n");

        if (i < 8) {
            printf("┣━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━┫\n");
        }
    }

    printf("┗━━━┻━━━┻━━━┻━━━┻━━━┻━━━┻━━━┻━━━┻━━━┛\n");
}

static int legal(int grid[9][9], int x, int y, int n) {
    int i, j;

    for (j = 0; j < 9; ++j) {
        if (grid[x][j] == n)
            return 0;
    }

    for (i = 0; i < 9; ++i) {
        if (grid[i][y] == n)
            return 0;
    }

    x = (x / 3) * 3;
    y = (y / 3) * 3;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            if (grid[x + i][y + j] == n)
                return 0;
        }
    }

    return 1;
}

static int check_answer(void) {
    char answer[5];
    size_t len;

    if (!fgets(answer, sizeof(answer), stdin)) {
        fprintf(stderr, "\nError: reading failed\n");
        return 1;
    }

    printf("\n");

    len = strcspn(answer, "\n");
    if (answer[len] == '\0') {
        int c;

        while ((c = getchar()) != '\n' && c != EOF); /* empty buffer */

        if (c == EOF)
            return 1;
    } else {
        answer[len] = '\0';
    }

    string_tolower(answer);

    if ((strcmp(answer, "yes") == 0 && len == 3) ||
        strcmp(answer, "y") == 0 ||
        strcmp(answer, "") == 0)
        return 0;

    return 1;
}

/* Assume: num_solutions != NULL */
static int solve(int grid[9][9], size_t *num_solutions) {
    int x, y, n;

    for (x = 0; x < 9; ++x) {
        for (y = 0; y < 9; ++y) {
            if (grid[x][y] == 0) {
                for (n = 1; n < 10; ++n) {
                    if (legal(grid, x, y, n)) {
                        grid[x][y] = n;

                        if (solve(grid, num_solutions))
                            return 1; /* cut solutions */

                        grid[x][y] = 0; /* backtracking */
                    }
                }

                return 0;
            }
        }
    }

    ++(*num_solutions);

    printf("=========== Solution %lu ==============\n", (unsigned long)*num_solutions);
    print_grid(grid);
    printf("More solutions? (Y/n): ");

    return check_answer();
}

int main(int argc, char *argv[]) {
    FILE *infile = NULL;
    int grid[9][9];
    size_t num_solutions = 0;
    int print_msg;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (validate_infile(argv[1], &infile) != 0)
        return EXIT_FAILURE;

    if (load_grid(grid, infile) != 0) {
        fclose(infile);
        return EXIT_FAILURE;
    }

    print_msg = solve(grid, &num_solutions);

    if (!print_msg) {
        if (num_solutions == 0) {
            printf("No solutions found.\n");
        } else {
            printf("No more solutions.\n");
        }
    }

    fclose(infile);

    return EXIT_SUCCESS;
}
