// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include QMK_KEYBOARD_H
#include "autocorrect.h"
#ifdef COMBO_ENABLE
#   include "combos.h"
#endif

// Convert 5-bit to 8-bit packed modifiers by extracting the
// upper bit and shift by 4 if bit 12 is set, otherwise by 8
#define MOD_TAP_GET_MOD_BITS(k) (((k) & 0x0F00) >> (((k) & 0x1000) ? 4 : 8))
// Basic keycode filter for tap-hold keys
#define GET_TAP_KEYCODE(k) ((k) & 0xFF)

// Tap-hold key detection
#define IS_TAP_HOLD(k) (IS_QK_MOD_TAP((k)) || IS_QK_LAYER_TAP((k)))
#define IS_SHORTCUT(k) (IS_QK_LAYER_TAP((k)) && !QK_LAYER_TAP_GET_LAYER((k)))
#define IS_LAYER_TAP(k) (IS_QK_LAYER_TAP((k)) && QK_LAYER_TAP_GET_LAYER((k)))
#define IS_MOD_TAP_CAG(k) (IS_QK_MOD_TAP((k)) && ((k) & (QK_LCTL|QK_LALT|QK_LGUI)))
#define IS_MOD_TAP_SHIFT(k) (IS_QK_MOD_TAP((k)) && ((k) & QK_LSFT))


// Homerow modifier key detection
#define HOMEROW_MASK ((1U << 1) | (1U << 5)) // Bitmasks for rows 1 and 5
#define IS_HOMEROW(r) (HOMEROW_MASK & (1U << ((r)->event.key.row)))
#define IS_HOMEROW_CAG(k, r) (IS_HOMEROW((r)) && IS_MOD_TAP_CAG((k)))
#define IS_HOMEROW_SHIFT(k, r) (IS_HOMEROW((r)) && IS_MOD_TAP_SHIFT((k)))

// Detects when a non-Shift homerow modifier key or shortcut key is pressed
// immediately after a recent letter key input, capturing rapid succession
// where the previous key was a letter (A–Z) and the time since the last
// keypress is within INPUT_IDLE_MS
#define IS_QUICK_SUCCESSION_INPUT(k, r, ctx) (           \
    (IS_HOMEROW_CAG((k), (r)) || IS_SHORTCUT((k)))       \
    && (GET_TAP_KEYCODE((ctx).keycode) <= KC_Z)          \
    && (last_matrix_activity_elapsed() <= INPUT_IDLE_MS))


// Same-hand key press detection by comparing two key events using row bitmasks
// Row 1 matches rows 0,1,2 (bitmask 0x07) and row 5 matches rows 4,5,6 (bitmask 0x70)
#define UNILATERAL_MASK(r) ((r) == 1 ? 0x07 : ((r) == 5 ? 0x70 : 0x00))
#define IS_UNILATERAL(r, c) (UNILATERAL_MASK((r)->event.key.row) & (1U << (c).event.key.row))

// Detects when a tap-hold key overlaps with another non-Shift key on the same
// hand, or when the key is a shortcut key. This macro captures unilateral
// (same-hand) input patterns and shortcut activations, excluding cases where
// the following key is Shift
#define IS_CHORDAL_TAP_INPUT(k, r, ctx) (                                    \
    (IS_UNILATERAL((r), (ctx).record) && !IS_MOD_TAP_SHIFT((ctx).keycode))   \
    || IS_SHORTCUT((k)))


// Tap keycode pressed state tracker
static bool is_pressed[UINT8_MAX];

// Contextual input cache
static struct {
    uint_fast16_t keycode;
    keyrecord_t   record;
} context;


bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        // Set pressed state and update the key record
        if (IS_QUICK_SUCCESSION_INPUT(keycode, record, context)) {
            is_pressed[GET_TAP_KEYCODE(keycode)] = true;
            record->keycode = GET_TAP_KEYCODE(keycode);
        }

        // Store the intermediate input for contextual processing
        context.keycode = keycode;
        context.record  = *record;

    } else if (IS_TAP_HOLD(keycode)) {
        // Clear pressed state and increment tap count to register a tap release
        if (is_pressed[GET_TAP_KEYCODE(keycode)]) {
            is_pressed[GET_TAP_KEYCODE(keycode)] = false;
            record->tap.count++;
        }
    }

    return true;
}


bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    if (IS_CHORDAL_TAP_INPUT(keycode, record, context)) {
        // Set pressed state and update the key record
        is_pressed[GET_TAP_KEYCODE(keycode)] = true;
        record->tap.interrupted = false;
        record->tap.count++;
        return true;
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


static inline void process_caps_unlock(uint16_t keycode, keyrecord_t *record) {
    // Get tap keycode from tap-hold keys
    if (IS_TAP_HOLD(keycode)) {
        if (record->tap.count == 0) return;
        keycode = GET_TAP_KEYCODE(keycode);
    }

    // Letter and word-related keys that retain caps lock
    bool is_word_key = ((keycode) >= KC_A && (keycode) <= KC_0)
        || (keycode) == KC_BSPC
        || (keycode) == KC_MINS
        || (keycode) == KC_UNDS
        || (keycode) == KC_CAPS;

    // Keep caps lock if there are no non-Shift modifiers
    if (is_word_key && !(get_mods() & ~MOD_MASK_SHIFT)) return;
    // Toggle caps lock at word boundary
    tap_code(KC_CAPS);
}


// Send shortcut keycode for held key
#define PROCESS_TAP_HOLD(k, r) ((r)->tap.count ? true : (tap_code16((k)), false))

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        if (!process_autocorrect(keycode, record)) return false;
        if (host_keyboard_led_state().caps_lock) process_caps_unlock(keycode, record);

        // Clipboard shortcuts
        if      (keycode == TH_M)    return PROCESS_TAP_HOLD(Z_PST, record);
        else if (keycode == TH_COMM) return PROCESS_TAP_HOLD(Z_CPY, record);
        else if (keycode == TH_DOT)  return PROCESS_TAP_HOLD(Z_CUT, record);
        else if (keycode == TH_SLSH) return PROCESS_TAP_HOLD(Z_UND, record);
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
