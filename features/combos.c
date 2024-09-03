// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

// An adaptation of Jane Bernhardt's Combos on Steroids (http://combos.gboards.ca/)
// Uses preprocessors to build combo source code with the following macros:
// COMB(name, keycode_shortcut, combo_sequence...)
// SSTR(name, "string to send", combo_sequence...)
// ACTN(name, function_call(),  combo_sequence...)

#include QMK_KEYBOARD_H
#include "autocorrect.h"

#ifndef COMBOS_DEF
#   define COMBOS_DEF "combos.inc"
#endif

#ifndef COMBO_IDLE_MS
#   define COMBO_IDLE_MS TAPPING_TERM
#endif

// Internal helper macros
#define _C_ENUM(name, val, ...) name,
#define _C_DATA(name, val, ...) static uint16_t const name##_combo[] PROGMEM = {__VA_ARGS__, COMBO_END};
#define _C_TYPE(name, val, ...) [name] = COMBO(name##_combo, val),
#define _A_TYPE(name, val, ...) [name] = COMBO_ACTION(name##_combo),
#define _P_SSTR(name, val, ...) case name: if (pressed) { SEND_STRING(val); } break;
#define _P_ACTN(name, val, ...) case name: if (pressed) { val; } break;
#define _UNUSED(...)

// Generate enum values
#define COMB _C_ENUM
#define SSTR _C_ENUM
#define ACTN _C_ENUM
enum combos {
#include COMBOS_DEF
};
#undef COMB
#undef SSTR
#undef ACTN

// Generate combo key sequences
#define COMB _C_DATA
#define SSTR _C_DATA
#define ACTN _C_DATA
#include COMBOS_DEF
#undef COMB
#undef SSTR
#undef ACTN

// Generate combo definitions
#define COMB _C_TYPE
#define SSTR _A_TYPE
#define ACTN _A_TYPE
combo_t key_combos[] = {
#include COMBOS_DEF
};
#undef COMB
#undef SSTR
#undef ACTN

// Generate combo event handler
#define COMB _UNUSED
#define SSTR _P_SSTR
#define ACTN _P_ACTN
void process_combo_event(uint16_t combo_index, bool pressed) {
    switch (combo_index) {
#include COMBOS_DEF
        default: break; // No action for unrecognized combos
    }
}
#undef COMB
#undef SSTR
#undef ACTN


#ifdef COMBO_SHOULD_TRIGGER
static fast_timer_t input_timer;

bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    // Only trigger if enough idle time has passed
    if (timer_elapsed_fast(input_timer) < COMBO_IDLE_MS) {
        return false;
    }
    // Restrict combos to default layer except layer toggle combos
    return (!layer_state) || (combo_index >= tog_num);
}

void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Update the input timer on key press events
    if (record->event.pressed && IS_KEYEVENT(record->event)) {
        input_timer = timer_read_fast();
    }
}
#endif
