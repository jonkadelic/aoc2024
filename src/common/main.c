#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "src/common/puzzle.h"
#include "src/common/ansi.h"

[[gnu::nonnull(1)]]
static void run_puzzle(puzzle_def_t const* const puzzle);

[[gnu::nonnull(1, 2)]]
static puzzle_result_t exec_puzzle(puzzle_def_t const* const puzzle, char const* const file_name);

static void cleanup_lines(char*** lines);
static void cleanup_file(FILE* const* fp);

[[gnu::nonnull(1)]]
static void print_puzzle_result(puzzle_result_t const* const result);

EXTERN_PUZZLE(1, 1);
EXTERN_PUZZLE(1, 2);

int main(int argc, char* argv[static argc]) {
    puzzle_def_t const* const puzzles[] = {
        &PUZZLE(1, 1),
        &PUZZLE(1, 2),
    };
    size_t const num_puzzles = sizeof(puzzles) / sizeof(puzzles[0]);

    for (size_t i = 0; i < num_puzzles; i++) {
        run_puzzle(puzzles[i]);
        if (i != num_puzzles - 1) {
            printf("\n");
        }
    }
}

[[gnu::nonnull(1)]]
static void run_puzzle(puzzle_def_t const* const puzzle) {
    char example_path[256];
    char input_path[256];

    snprintf(example_path, sizeof(example_path), "src/day%d/example.txt", puzzle->day);
    snprintf(input_path, sizeof(input_path), "src/day%d/input.txt", puzzle->day);

    printf(BWHT "Running " WHTHB BLK "Day %d Puzzle %d" CRESET BWHT "..." CRESET "\n", puzzle->day, puzzle->index);

    printf(BBLK "Example" CRESET "\n");
    puzzle_result_t const example_result = exec_puzzle(puzzle, example_path);
    printf("    ");
    print_puzzle_result(&example_result);
    if (example_result.status == PUZZLE_STATUS__OK) {
        printf("    ");
        if (example_result.result == puzzle->example_result) {
            printf(GRN "PASSED" CRESET "\n");
        } else {
            printf(RED "FAILED" CRESET ": expected %d != actual %d\n", puzzle->example_result, example_result.result);
        }
    }

    printf(BBLK "Input" CRESET "\n");
    puzzle_result_t const input_result = exec_puzzle(puzzle, input_path);
    printf("    ");
    print_puzzle_result(&input_result);
}

[[gnu::nonnull(1, 2)]]
static puzzle_result_t exec_puzzle(puzzle_def_t const* const puzzle, char const* const file_name) {
    FILE* const fp [[gnu::cleanup(cleanup_file)]] = fopen(file_name, "r");
    if (fp == nullptr) {
        return (puzzle_result_t) { .status = PUZZLE_STATUS__FILE_MISSING };
    }

    size_t num_lines = 1;
    char** lines [[gnu::cleanup(cleanup_lines)]] = malloc(sizeof(char*) * num_lines);
    lines[0] = nullptr;
    char* line = nullptr;
    size_t len = 0;

    while (getline(&line, &len, fp) != -1) {
        num_lines++;
        char** new_lines = realloc(lines, sizeof(char*) * num_lines);
        if (new_lines == nullptr) {
            return (puzzle_result_t) { .status = PUZZLE_STATUS__OUT_OF_MEMORY };
        }
        lines = new_lines;

        lines[num_lines - 2] = line;
        lines[num_lines - 1] = nullptr;
        line = nullptr;
    }

    return puzzle->run(num_lines - 1, (char const* const*) lines);
}

static void cleanup_lines(char*** lines) {
    if (*lines == nullptr) {
        return;
    }

    int i = 0;
    while ((*lines)[i] != nullptr) {
        free((*lines)[i]);
        i++;
    }

    free((*lines));
}

static void cleanup_file(FILE* const* fp) {
    if (*fp != nullptr) {
        fclose(*fp);
    }
}

[[gnu::nonnull(1)]]
static void print_puzzle_result(puzzle_result_t const* const result) {
    printf("Result: [");

    if (result->status == PUZZLE_STATUS__OK) {
        printf(BGRN "SUCCESS" CRESET "] %d\n", result->result);
    } else {
        char const* message = nullptr;
        switch (result->status) {
            case PUZZLE_STATUS__FILE_MISSING:
                message = "Input file was missing!";
                break;
            case PUZZLE_STATUS__OUT_OF_MEMORY:
                message = "Could not allocate memory!";
                break;
            default:
                message = "Unknown error!";
                break;
        }
        printf(BRED "FAILURE" CRESET "] %s\n", message);
    }
}