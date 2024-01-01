// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include "filterpaper.h"

static uint16_t    next_keycode, processed_keycode;
static keyrecord_t next_record;

// Check for combo generated keycode
static inline bool is_combo(uint16_t keycode) {
    for (uint16_t i = 0; i < sizeof(key_combos) / sizeof(combo_t); ++i) {
        if (keycode == key_combos[i].keycode) return true;
    }
    return false;
}

bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint16_t prev_keycode;
    static bool     is_pressed[UINT8_MAX];

    if (record->event.pressed) {
        // Copy previous keycode for instant tap decision
        prev_keycode = next_keycode;
        // Cache current input for on-going mod-tap decisions
        next_keycode = keycode;
        next_record  = *record;
    }

    // Match non-shift home row mod-tap keys if not preceded by layer or combo keys
    if (IS_HOMEROW(record) && IS_MOD_TAP_CAG(keycode) && !IS_LAYER_TAP(prev_keycode) && !is_combo(processed_keycode)) {
        uint8_t const tap_keycode = keycode & 0xff;
        // Press the tap keycode when it follows a short interval
        if (record->event.pressed && last_input_activity_elapsed() < QUICK_TAP_TERM) {
            record->keycode = tap_keycode;
            is_pressed[tap_keycode] = true;
        }
        // Release the tap keycode if pressed
        else if (!record->event.pressed && is_pressed[tap_keycode]) {
            record->keycode = tap_keycode;
            is_pressed[tap_keycode] = false;
        }
    }
    return true;
}


bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    // Send Control or Shift with a nested key tap on the opposite hand
    return IS_BILATERAL_TAP(record, next_record) && IS_MOD_TAP_CS(keycode);
}


bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    // Activate layer with another key press
    if (IS_LAYER_TAP(keycode)) return true;
    // Sent the tap keycode when the mod-tap key overlaps with
    // another key on the same hand with no active modifiers
    if (IS_UNILATERAL_TAP(record, next_record) && !IS_MOD_TAP_SHIFT(next_keycode) && !get_mods()) {
        record->keycode = keycode & 0xff;
        process_record(record);
        record->event.pressed = false;
        process_record(record);
    }
    return false;
}


uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    // Decrease tapping term for Shift mod tap and clipboard shortcuts
    if (IS_MOD_TAP_SHIFT(keycode) || (IS_QK_LAYER_TAP(keycode) && !QK_LAYER_TAP_GET_LAYER(keycode))) {
        return TAPPING_TERM - 50;
    }
    return TAPPING_TERM;
}


// Turn off caps lock at a word boundry
static inline bool process_caps_unlock(uint16_t keycode, keyrecord_t *record) {
    bool    const caps = host_keyboard_led_state().caps_lock;
    uint8_t const mods = get_mods();
    // Ignore inactive caps lock status and shifted keys
    if (!caps || mods == MOD_BIT_LSHIFT || mods == MOD_BIT_RSHIFT) return true;
    // Get base keycode from mod-tap and layer-tap keys
    if (IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode)) {
        if (record->tap.count == 0) return true;
        keycode &= 0xff;
    }
    // Match caps lock retention keycodes
    switch (keycode) {
        case KC_A ... KC_0:
        case KC_BSPC:
        case KC_MINS:
        case KC_UNDS:
        case KC_CAPS: if (!mods) return true;
        // Any unmatched keycode is a word boundary
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
        processed_keycode = keycode; // Store processed keycode for combo detection with instant tap
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


// Reduce matrix scanning delay
#ifndef DIRECT_PINS
void matrix_io_delay(void) { __asm__ volatile("nop\nnop\nnop\n"); }
#endif