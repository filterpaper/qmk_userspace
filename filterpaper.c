// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include QMK_KEYBOARD_H
#include "autocorrect.h"

// Convert 5-bit to 8-bit packed modifiers by extracting the
// upper bit and shift by 4 if bit 12 is set, otherwise by 8
#define MOD_TAP_GET_MOD_BITS(k) (((k) & 0x0f00) >> (((k) & 0x1000) ? 4 : 8))
// Basic keycode filter for tap-hold keys
#define GET_TAP_KEYCODE(k) ((k) & 0xff)

// Tap-hold decision helper macros
#define IS_LAYER_TAP(k) (IS_QK_LAYER_TAP(k) && QK_LAYER_TAP_GET_LAYER(k))
#define IS_SHORTCUT(k) (IS_QK_LAYER_TAP(k) && !QK_LAYER_TAP_GET_LAYER(k))
#define IS_MOD_TAP_SHIFT(k) (IS_QK_MOD_TAP(k) && (k) & QK_LSFT)
#define IS_MOD_TAP_CAG(k) (IS_QK_MOD_TAP(k) && (k) & (QK_LCTL|QK_LALT|QK_LGUI))

#define HOMEROW_MASK ((1U << 1) | (1U << 5)) // Bitmasks for rows 1 and 5
#define IS_HOMEROW(r) (HOMEROW_MASK & (1U << (r)->event.key.row))
#define IS_HOMEROW_SHIFT(k, r) (IS_HOMEROW(r) && IS_MOD_TAP_SHIFT(k))
#define IS_HOMEROW_CAG(k, r) (IS_HOMEROW(r) && IS_MOD_TAP_CAG(k))

#define IS_TYPING(k) ( \
    (GET_TAP_KEYCODE(k) <= KC_Z || IS_SHORTCUT(k)) && \
    (last_input_activity_elapsed() < INPUT_IDLE_MS))

// Same-hand key press detection by comparing two key events using row bitmasks
// Row 1 matches rows 0,1,2 (bitmask 0x07) and row 5 matches rows 4,5,6 (bitmask 0x70)
#define UNILATERAL_MASK(h) ((h) == 1 ? 0x07 : ((h) == 5 ? 0x70 : 0x00))
#define IS_UNILATERAL(r, c) (UNILATERAL_MASK((r)->event.key.row) & (1U << (c).event.key.row))
#define IS_UNILATERAL_AND_NOT_SHIFT(r, c) (IS_UNILATERAL((r), (c).record) && !IS_MOD_TAP_SHIFT((c).keycode))

// Contextual input storage
static struct {
    uint_fast16_t keycode;
    keyrecord_t   record;
} context;


bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Bit array for tracking pressed state of tap keycodes (0-255)
    static uint32_t is_pressed[(UINT8_MAX + 31) / 32];

    if (IS_HOMEROW_CAG(keycode, record) || IS_SHORTCUT(keycode)) {
        // Variables to manage the pressed state array
        const uint_fast8_t  tap_keycode = GET_TAP_KEYCODE(keycode);
        const uint_fast8_t  tap_index   = tap_keycode / 32;
        const uint_fast32_t tap_bitmask = 1U << (tap_keycode % 32);

        if (record->event.pressed) {
            // Press the tap keycode when it follows the previous key swiftly
            if (IS_TYPING(context.keycode)) {
                is_pressed[tap_index] |= tap_bitmask;
                record->keycode = tap_keycode;
            }
        } else {
            // Release the tap keycode if pressed
            if (is_pressed[tap_index] & tap_bitmask) {
                is_pressed[tap_index] &= ~tap_bitmask;
                record->keycode = tap_keycode;
            }
        }
    }

    // Store the intermediate keycode and its associated
    // key event record for contextual processing
    if (record->event.pressed) {
        context.keycode = keycode;
        context.record  = *record;
    }

    return true;
}


bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    // If the tap-hold key overlaps with another non-Shift key on the same
    // hand or if the key is a shortcut overlapping with any other key,
    // clear its interrupted state and process the tap-hold key as a tap
    if (IS_UNILATERAL_AND_NOT_SHIFT(record, context) || IS_SHORTCUT(keycode)) {
        record->tap.interrupted = false;
        record->tap.count++;
        process_record(record);
        return false;
    }
    // Activate layer hold with another key press
    return IS_LAYER_TAP(keycode);
}


bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    // Register Shift with a nested key press
    return IS_HOMEROW_SHIFT(keycode, record);
}


uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    // Shorten interval for Shift
    return IS_HOMEROW_SHIFT(keycode, record) ? SHIFT_TAP_TERM : TAPPING_TERM;
}


// Turn off caps lock at word boundary
static inline void process_caps_unlock(uint16_t keycode, keyrecord_t *record) {
    // Get tap keycode from tap-hold keys
    if (IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode)) {
        if (record->tap.count == 0) return;
        keycode = GET_TAP_KEYCODE(keycode);
    }

    switch (keycode) {
        case KC_A ... KC_0:
        case KC_BSPC:
        case KC_MINS:
        case KC_UNDS:
        case KC_CAPS:
            // Retain caps lock if there are no active non-Shift modifiers
            if ((get_mods() & ~MOD_MASK_SHIFT) == 0) return;
        default:
            // Everything else is a word boundary
            tap_code(KC_CAPS);
    }
}


// Send custom hold keycode
static inline bool process_tap_hold(uint16_t keycode, keyrecord_t *record) {
    if (record->tap.count) return true;
    tap_code16(keycode);
    return false;
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        if (!process_autocorrect(keycode, record)) return false;
        if (host_keyboard_led_state().caps_lock) process_caps_unlock(keycode, record);

        // Clipboard shortcuts
        if      (keycode == TH_M)    return process_tap_hold(Z_PST, record);
        else if (keycode == TH_COMM) return process_tap_hold(Z_CPY, record);
        else if (keycode == TH_DOT)  return process_tap_hold(Z_CUT, record);
        else if (keycode == TH_SLSH) return process_tap_hold(Z_UND, record);
    }

    return true;
}


// Simplify unused magic config functions
#ifndef MAGIC_ENABLE
uint8_t mod_config(uint8_t mod) { return mod; }
uint16_t keycode_config(uint16_t keycode) { return keycode; }
#endif


// Reduce matrix scanning delay
#ifndef DIRECT_PINS
void matrix_io_delay(void) { __asm__ volatile("nop\nnop\nnop\n"); }
#endif
