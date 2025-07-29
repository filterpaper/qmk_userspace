// Improved asymmetric debounce with static array and refactored code

#include "debounce.h"
#include "timer.h"
#include "util.h"

#define DEBOUNCE 5
#define DEBOUNCE_ELAPSED 0

static struct {
    bool    pressed : 1;
    uint8_t time : 7;
} debounce_counters[MATRIX_ROWS_PER_HAND * MATRIX_COLS] = {0};

static bool counters_need_update;
static bool matrix_need_update;
static bool cooked_changed;

static inline void process_debounce_counters(matrix_row_t raw[], matrix_row_t cooked[], uint8_t elapsed_time) {
    counters_need_update = false;
    matrix_need_update   = false;

    for (uint8_t row = 0; row < MATRIX_ROWS_PER_HAND; row++) {
        uint8_t row_offset = row * MATRIX_COLS;

        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            uint8_t index = row_offset + col;

            if (debounce_counters[index].time != DEBOUNCE_ELAPSED) {
                if (debounce_counters[index].time <= elapsed_time) {
                    debounce_counters[index].time = DEBOUNCE_ELAPSED;

                    if (debounce_counters[index].pressed) {
                        // key-down: eager
                        matrix_need_update = true;
                    } else {
                        // key-up: defer
                        matrix_row_t col_mask    = (MATRIX_ROW_SHIFTER << col);
                        matrix_row_t cooked_next = (cooked[row] & ~col_mask) | (raw[row] & col_mask);
                        cooked_changed |= (cooked[row] != cooked_next);
                        cooked[row] = cooked_next;
                    }
                } else {
                    debounce_counters[index].time -= elapsed_time;
                    counters_need_update = true;
                }
            }
        }
    }
}

static inline void apply_debounced_changes(matrix_row_t raw[], matrix_row_t cooked[]) {
    matrix_need_update = false;

    for (uint8_t row = 0; row < MATRIX_ROWS_PER_HAND; row++) {
        uint8_t      row_offset = row * MATRIX_COLS;
        matrix_row_t row_delta  = raw[row] ^ cooked[row];

        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            uint8_t      index    = row_offset + col;
            matrix_row_t col_mask = (MATRIX_ROW_SHIFTER << col);

            if (row_delta & col_mask) {
                if (debounce_counters[index].time == DEBOUNCE_ELAPSED) {
                    debounce_counters[index].pressed = (raw[row] & col_mask);
                    debounce_counters[index].time    = DEBOUNCE;
                    counters_need_update             = true;

                    if (debounce_counters[index].pressed) {
                        // key-down: eager
                        cooked[row] ^= col_mask;
                        cooked_changed = true;
                    }
                }
            } else if (debounce_counters[index].time != DEBOUNCE_ELAPSED) {
                if (!debounce_counters[index].pressed) {
                    // key-up: defer
                    debounce_counters[index].time = DEBOUNCE_ELAPSED;
                }
            }
        }
    }
}

void debounce_init(uint8_t num_rows) {}

bool debounce(matrix_row_t raw[], matrix_row_t cooked[], uint8_t num_rows, bool changed) {
    static fast_timer_t last_time;
    bool                last_updated = false;
    cooked_changed                   = false;

    if (counters_need_update) {
        fast_timer_t now          = timer_read_fast();
        fast_timer_t elapsed_time = TIMER_DIFF_FAST(now, last_time);
        last_time                 = now;
        last_updated              = true;
        if (elapsed_time > 0) process_debounce_counters(raw, cooked, MIN(elapsed_time, UINT8_MAX));
    }

    if (changed || matrix_need_update) {
        if (!last_updated) last_time = timer_read_fast();
        apply_debounced_changes(raw, cooked);
    }

    return cooked_changed;
}
