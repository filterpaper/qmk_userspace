// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include "filterpaper.h"

static uint16_t        next_keycode;
static keyrecord_t     next_record;
static keyevent_type_t prev_event;


bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint16_t prev_keycode;
    static bool     is_pressed[UINT8_MAX];

    // Cache previous and next input for tap-hold decisions
    if (record->event.pressed) {
        prev_keycode = next_keycode;
        next_keycode = keycode;
        next_record  = *record;
    }

    // Override non-Shift tap-hold keys based on previous input
    if (IS_HOMEROW(record) && IS_MOD_TAP_CAG(keycode)) {
        uint8_t const tap_keycode = GET_TAP_KEYCODE(keycode);
        // Press the tap keycode while typing and only if preceded by text keycodes
        if (record->event.pressed && IS_TYPING(prev_keycode) && prev_event != COMBO_EVENT) {
            record->keycode = tap_keycode;
            is_pressed[tap_keycode] = true;
        }
        // Release the tap keycode if pressed
        else if (is_pressed[tap_keycode]) {
            record->keycode = tap_keycode;
            is_pressed[tap_keycode] = false;
        }
    }

    return true;
}


bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    // Activate layer with another key press
    if (IS_LAYER_TAP(keycode)) return true;

    // Sent its tap keycode when non-Shift overlaps with another key on the same hand
    if (IS_UNILATERAL(record, next_record) && !IS_MOD_TAP_SHIFT(next_keycode) && !get_mods()) {
        record->keycode = GET_TAP_KEYCODE(keycode);
        process_record(record);
        record->event.pressed = false;
        process_record(record);
    }

    return false;
}


bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    // Send Control or Shift with a nested key press on the opposite hand
    return IS_BILATERAL(record, next_record) && IS_MOD_TAP_CS(keycode);
}


uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    // Decrease tapping term for Shift
    return IS_MOD_TAP_SHIFT(keycode) ? TAPPING_TERM - 50 : TAPPING_TERM;
}


// Turn off caps lock at a word boundry
static inline bool process_caps_unlock(uint16_t keycode, keyrecord_t *record) {
    // Skip if caps lock is off
    if (!host_keyboard_led_state().caps_lock) return true;

    // Get tap keycode from tap hold keys
    if (IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode)) {
        if (record->tap.count == 0) return true;
        keycode = GET_TAP_KEYCODE(keycode);
    }

    switch (keycode) {
        // Retain caps lock with the following keycodes
        // if there are no active non-Shift modifiers
        case KC_A ... KC_0:
        case KC_BSPC:
        case KC_MINS:
        case KC_UNDS:
        case KC_CAPS:
            if (!(get_mods() & ~MOD_MASK_SHIFT)) break;
        // Everything else is a word boundary
        default: tap_code(KC_CAPS);
    }
    return true;
}


// Send custom hold keycode
static inline bool process_tap_hold(uint16_t keycode, keyrecord_t *record) {
    if (record->tap.count) return true;
    tap_code16(keycode);
    return false;
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        // Store processed event for combo detection in preprocess record
        prev_event = record->event.type;

        if (!process_autocorrect(keycode, record) || !process_caps_unlock(keycode, record)) return false;

        // Clipboard shortcuts
        if      (keycode == TH_M)    return process_tap_hold(Z_PST, record);
        else if (keycode == TH_COMM) return process_tap_hold(Z_CPY, record);
        else if (keycode == TH_DOT)  return process_tap_hold(Z_CUT, record);
        else if (keycode == TH_SLSH) return process_tap_hold(Z_UND, record);
    }

    return true;
}


// Simplify unused magic config functions
uint8_t mod_config(uint8_t mod) { return mod; }
uint16_t keycode_config(uint16_t keycode) { return keycode; }


// Reduce marix scanning delay
#ifndef DIRECT_PINS
void matrix_io_delay(void) { __asm__ volatile("nop\nnop\nnop\n"); }
#endif