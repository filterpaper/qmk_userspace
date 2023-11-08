// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include "filterpaper.h"

static uint16_t    next_keycode;
static keyrecord_t next_record;

bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        // Cache the next input for mod-tap decisions
        next_keycode = keycode;
        next_record  = *record;
    }
    return true;
}


bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    // Hold Control and Shift with a nested key tap on the opposite hand
    return IS_BILATERAL_TAP(record, next_record) && IS_MOD_TAP_CS(keycode);
}


bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    // Activate layer with another key press
    if (IS_QK_LAYER_TAP(keycode) && QK_LAYER_TAP_GET_LAYER(keycode)) {
        return true;
    }
    // When a mod-tap key overlaps with another non-Shift key on the same
    // hand without any other modifiers active, send its base keycode
    if (IS_UNILATERAL_TAP(record, next_record) && !IS_MOD_TAP_SHIFT(next_keycode) && !get_mods()) {
        record->keycode = keycode & 0xff;
        process_record(record);
        record->event.pressed = false;
        process_record(record);
    }
    return false;
}


uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    // Decrease tapping term for the home row Shift
    return IS_HOMEROW(record) && IS_MOD_TAP_SHIFT(keycode) ? TAPPING_TERM - 50 : TAPPING_TERM;
}


// Turn off caps lock at the end of a word
static inline bool process_caps_unlock(uint16_t keycode, keyrecord_t *record) {
    bool    const caps = host_keyboard_led_state().caps_lock;
    uint8_t const mods = get_mods();

    // Ignore inactive caps lock status and shifted keys
    if (!caps || mods == MOD_BIT_LSHIFT || mods == MOD_BIT_RSHIFT) return true;

    // Filter mod-tap and layer-tap keys
    if (IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode)) {
        if (record->tap.count == 0) return true;
        keycode &= 0xff;
    }

    // Identify caps lock retention key codes
    switch (keycode) {
        case KC_A ... KC_0:
        case KC_BSPC:
        case KC_MINS:
        case KC_UNDS:
        case KC_CAPS: if (!mods) return true;
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
        if (!process_autocorrect(keycode, record) || !process_caps_unlock(keycode, record)) return false;

        // Clipboard shortcuts
        if      (keycode == TH_M)    return process_tap_hold(Z_PST, record);
        else if (keycode == TH_COMM) return process_tap_hold(Z_CPY, record);
        else if (keycode == TH_DOT)  return process_tap_hold(Z_CUT, record);
        else if (keycode == TH_SLSH) return process_tap_hold(Z_UND, record);
    }

    return true;
}


void housekeeping_task_user(void) {
    // Restore state after 3 minutes
    if (last_input_activity_elapsed() > TAPPING_TERM * 1000U) {
        if (host_keyboard_led_state().caps_lock) tap_code(KC_CAPS);
#ifdef SWAP_HANDS_ENABLE
        if (is_swap_hands_on()) swap_hands_off();
#endif
    }
}


// Simplify unused magic config functions
uint8_t mod_config(uint8_t mod) { return mod; }
uint16_t keycode_config(uint16_t keycode) { return keycode; }


// Reduce matrix scanning delay
#ifndef DIRECT_PINS
void matrix_io_delay(void) { __asm__ volatile("nop\nnop\nnop\n"); }
#endif