// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include QMK_KEYBOARD_H
#include "autocorrect.h"
#ifdef COMBO_ENABLE
#   include "combos.h"
#endif

// Convert 5-bit packed mod-tap modifiers to 8-bit packed MOD_MASK modifiers
#define MOD_TAP_GET_MOD_BITS(k) (((k) & 0x0f00) >> (((k) & 0x1000) ? 4 : 8))
// Basic keycode filter for tap-hold keys
#define GET_TAP_KEYCODE(k) ((k) & 0xff)

// Tap-hold decision helper macros
#define IS_LAYER_TAP(k) (IS_QK_LAYER_TAP(k) && QK_LAYER_TAP_GET_LAYER(k))
#define IS_SHORTCUT(k) (IS_QK_LAYER_TAP(k) && !QK_LAYER_TAP_GET_LAYER(k))
#define IS_MOD_TAP_SHIFT(k) (IS_QK_MOD_TAP(k) && (k) & QK_LSFT)
#define IS_MOD_TAP_CAG(k) (IS_QK_MOD_TAP(k) && (k) & (QK_LCTL|QK_LALT|QK_LGUI))

#define IS_HOMEROW(r) (r->event.key.row == 1 || r->event.key.row == 5)
#define IS_HOMEROW_SHIFT(k, r) (IS_HOMEROW(r) && IS_MOD_TAP_SHIFT(k))
#define IS_HOMEROW_CAG(k, r) (IS_HOMEROW(r) && IS_MOD_TAP_CAG(k))

#define IS_TYPING(k) (!IS_LAYER_TAP(k) && last_input_activity_elapsed() < INPUT_IDLE_MS)

#define IS_UNILATERAL(r, n) ( \
    (r->event.key.row == 1 && 0 <= n.event.key.row && n.event.key.row <= 2) || \
    (r->event.key.row == 5 && 4 <= n.event.key.row && n.event.key.row <= 6) )

#define IS_BILATERAL(r, n) ( \
    (r->event.key.row == 1 && 4 <= n.event.key.row && n.event.key.row <= 6) || \
    (r->event.key.row == 5 && 0 <= n.event.key.row && n.event.key.row <= 2) )


static uint16_t    inter_keycode;
static keyrecord_t inter_record;

bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
    static bool     is_pressed[UINT8_MAX];
    const  uint16_t tap_keycode = GET_TAP_KEYCODE(keycode);

    if (record->event.pressed) {
        // Press the tap keycode if the tap-hold key follows a previous key swiftly
        if ((IS_HOMEROW_CAG(keycode, record) || IS_SHORTCUT(keycode)) && IS_TYPING(inter_keycode)) {
            is_pressed[tap_keycode] = true;
            record->keycode         = tap_keycode;
        }
        // Cache incoming input for in-progress and subsequent tap-hold decisions
        inter_keycode = keycode;
        inter_record  = *record;
    }

    // Release the pressed tap keycode
    else if (is_pressed[tap_keycode]) {
        is_pressed[tap_keycode] = false;
        record->keycode         = tap_keycode;
    }

    return true;
}


bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    // If the tap-hold key overlaps with another non-Shift key on the same
    // hand or if the key is a shortcut overlapping with any other key,
    // clear its interrupted state and process the tap-hold key as a tap
    if ((IS_UNILATERAL(record, inter_record) && !IS_MOD_TAP_SHIFT(inter_keycode)) || IS_SHORTCUT(keycode)) {
        record->tap.interrupted = false;
        record->tap.count++;
        process_record(record);
        return false;
    }

    // Activate layer hold with another key press
    else return IS_LAYER_TAP(keycode);
}


bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    // Enable Shift with a nested key press on the opposite hand
    return IS_BILATERAL(record, inter_record) && IS_MOD_TAP_SHIFT(keycode);
}


uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    // Shorten interval for Shift
    return IS_HOMEROW_SHIFT(keycode, record) ? TAPPING_TERM - 80 : TAPPING_TERM;
}


// Turn off caps lock at word boundry
static inline bool process_caps_unlock(uint16_t keycode, keyrecord_t *record) {
    // Skip if caps lock is off
    if (!host_keyboard_led_state().caps_lock) return true;

    // Get tap keycode from tap-hold keys
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
            if ((get_mods() & ~MOD_MASK_SHIFT) == false) break;
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
#if defined (COMBO_ENABLE) && defined (COMBO_SHOULD_TRIGGER)
        if (record->event.type != COMBO_EVENT) input_timer = timer_read_fast();
#endif
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
#ifndef MAGIC_ENABLE
uint8_t mod_config(uint8_t mod) { return mod; }
uint16_t keycode_config(uint16_t keycode) { return keycode; }
#endif


// Reduce matrix scanning delay
#ifndef DIRECT_PINS
void matrix_io_delay(void) { __asm__ volatile("nop\nnop\nnop\n"); }
#endif