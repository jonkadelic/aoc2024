#include <limits.h>
#include <stddef.h>
#include <stdio.h>

#include "src/common/puzzle.h"

DEFINE_PUZZLE_STRUCTURE(1, 1, 11);
DEFINE_PUZZLE_STRUCTURE(1, 2, 31);

static void sort(size_t const length, int in[length], int out[length]);

PUZZLE_FUNC(1) {
    int list_left[num_lines];
    int list_right[num_lines];

    for (size_t i = 0; i < num_lines; i++) {
        sscanf(lines[i], "%d   %d", &(list_left[i]), &(list_right[i]));
    }

    int list_left_sorted[num_lines];
    int list_right_sorted[num_lines];

    sort(num_lines, list_left, list_left_sorted);
    sort(num_lines, list_right, list_right_sorted);

    int acc = 0;
    for (size_t i = 0; i < num_lines; i++) {
        int distance = list_left_sorted[i] - list_right_sorted[i];
        if (distance < 0) distance = -distance;
        acc += distance;
    }

    return (puzzle_result_t) { .status = PUZZLE_STATUS__OK, .result = acc };
}

PUZZLE_FUNC(2) {
    int list_left[num_lines];
    int list_right[num_lines];

    for (size_t i = 0; i < num_lines; i++) {
        sscanf(lines[i], "%d   %d", &(list_left[i]), &(list_right[i]));
    }

    int acc = 0;
    for (size_t i = 0; i < num_lines; i++) {
        int const target = list_left[i];
        size_t count = 0;

        for (size_t j = 0; j < num_lines; j++) {
            if (list_right[j] == target) {
                count++;
            }
        }
        acc += target * count;
    }

    return (puzzle_result_t) { .status = PUZZLE_STATUS__OK, .result = acc };
}

static void sort(size_t const length, int in[length], int out[length]) {
    for (size_t i = 0; i < length; i++) {
        int current_smallest = INT_MAX;
        size_t current_smallest_index = 0;

        for (size_t j = 0; j < length; j++) {
            if (in[j] < current_smallest) {
                current_smallest = in[j];
                current_smallest_index = j;
            }
        }
        in[current_smallest_index] = INT_MAX;
        out[i] = current_smallest;
    }
}
