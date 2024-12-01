#pragma once

#include <stddef.h>

typedef enum puzzle_status {
    PUZZLE_STATUS__OK,
    PUZZLE_STATUS__FILE_MISSING,
    PUZZLE_STATUS__OUT_OF_MEMORY,
} puzzle_status_t;

typedef struct puzzle_result {
    puzzle_status_t status;
    int result;
} puzzle_result_t;

typedef struct puzzle_def {
    int day;
    int index;
    int example_result;
    puzzle_result_t (*run)(size_t const num_lines, char const* const lines[num_lines]);
} puzzle_def_t;

#define PUZZLE(day, index) PUZZLE_ ## day ## _ ## index
#define EXTERN_PUZZLE(day, index) extern puzzle_def_t const PUZZLE(day, index)
#define PUZZLE_FUNC(index) static puzzle_result_t puzzle_ ## index ## _run(size_t const num_lines, char const* const lines[num_lines])
#define DEFINE_PUZZLE_STRUCTURE(puzzle_day, puzzle_index, puzzle_example_result) \
    PUZZLE_FUNC(puzzle_index); \
    puzzle_def_t const PUZZLE(puzzle_day, puzzle_index) = { \
        .day = puzzle_day, \
        .index = puzzle_index, \
        .example_result = puzzle_example_result, \
        .run = puzzle_ ## puzzle_index ## _run \
    }

