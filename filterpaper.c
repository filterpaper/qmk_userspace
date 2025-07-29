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
#define IS_SHORTCUT(k) (((k) & 0xFF00) == QK_LAYER_TAP) // Layer 0
#define IS_TAP_HOLD(k) (IS_QK_MOD_TAP((k)) || IS_QK_LAYER_TAP((k)))
#define IS_MOD_TAP_CAG(k) (IS_QK_MOD_TAP((k)) && ((k) & (QK_LCTL|QK_LALT|QK_LGUI)))
#define IS_MOD_TAP_SHIFT(k) (IS_QK_MOD_TAP((k)) && ((k) & QK_LSFT))

// Home row modifier key detection
#define IS_HOMEROW(r) (((1U << 1) | (1U << 5)) & (1U << ((r)->event.key.row)))
#define IS_HOMEROW_CAG(k, r) (IS_HOMEROW((r)) && IS_MOD_TAP_CAG((k)))
#define IS_HOMEROW_SHIFT(k, r) (IS_HOMEROW((r)) && IS_MOD_TAP_SHIFT((k)))

// Checks for a non-Shift home row modifier or shortcut key pressed
// in rapid succession after a letter key (A–Z) within INPUT_IDLE_MS
#define IS_QUICK_SUCCESSION_INPUT(k, r, ctx) (              \
    (IS_HOMEROW_CAG((k), (r)) || IS_SHORTCUT((k)))          \
    && (GET_TAP_KEYCODE((ctx).keycode) <= KC_Z)             \
    && (last_matrix_activity_elapsed() <= INPUT_IDLE_MS))

// Same-hand key press detection by comparing two key events using row bitmasks
// Row 1 matches rows 0,1,2 (bitmask 0x07); row 5 matches rows 4,5,6 (bitmask 0x70)
#define UNILATERAL_MASK(r) ((r) == 1 ? 0x07 : ((r) == 5 ? 0x70 : 0x00))
#define IS_UNILATERAL(r, c) (UNILATERAL_MASK((r)->event.key.row) & (1U << (c).event.key.row))

// Checks for a tap-hold key overlapping another non-Shift key on the same hand
// or a shortcut key overlapping with any key
#define IS_CHORDAL_TAP_INPUT(k, r, ctx) (                                    \
    (IS_UNILATERAL((r), (ctx).record) && !IS_MOD_TAP_SHIFT((ctx).keycode))   \
    || IS_SHORTCUT((k)))


// Tap keycode pressed state tracker
static bool tapped[UINT8_MAX];

// Contextual input cache
static struct {
    uint16_t    keycode;
    keyrecord_t record;
} context;


bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        // Flag key as tapped and update the key record
        if (IS_QUICK_SUCCESSION_INPUT(keycode, record, context)) {
            tapped[GET_TAP_KEYCODE(keycode)] = true;
            record->keycode                  = GET_TAP_KEYCODE(keycode);
        }

        // Store the intermediate input for contextual processing
        context.keycode = keycode;
        context.record  = *record;

    } else {
        // Clear tapped flag and update the key record to register a tap release
        if (tapped[GET_TAP_KEYCODE(keycode)]) {
            tapped[GET_TAP_KEYCODE(keycode)] = false;
            record->tap.count                = 1;
        }
    }

    return true;
}


bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    if (IS_CHORDAL_TAP_INPUT(keycode, record, context)) {
        // Flag key as tapped if the subsequent key is also a tap-hold
        if (IS_TAP_HOLD(context.keycode)) tapped[GET_TAP_KEYCODE(keycode)] = true;
        record->tap.interrupted = false;
        record->tap.count       = 1;
        return true;
    }

    // Activate layer hold with another key press
    return IS_QK_LAYER_TAP(keycode);
}


bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    // Activate Shift with a nested key press
    return IS_MOD_TAP_SHIFT(keycode);
}


uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    // Shorten timeout for home row Shift
    return IS_HOMEROW_SHIFT(keycode, record) ? SHIFT_TAP_TERM : TAPPING_TERM;
}


static inline void process_caps_unlock(uint16_t keycode, keyrecord_t *record) {
    // Get tap keycode from tap-hold keys
    if (IS_TAP_HOLD(keycode)) {
        if (record->tap.count == 0) return;
        keycode = GET_TAP_KEYCODE(keycode);
    }

    // Letter and word-related keys that retain caps lock
    bool is_word_key = keycode <= KC_0
        || keycode == KC_BSPC
        || keycode == KC_MINS
        || keycode == KC_UNDS
        || keycode == KC_CAPS;

    // Toggle caps lock at a word boundary if the key is not a word key
    // or if any non-Shift modifiers are active
    if (!is_word_key || (get_mods() & ~MOD_MASK_SHIFT)) tap_code(KC_CAPS);
}


// Send shortcut keycode for held key
#define PROCESS_SHORTCUT(k, r) ((r)->tap.count ? true : (tap_code16((k)), false))

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        if (!process_autocorrect(keycode, record)) return false;
        if (host_keyboard_led_state().caps_lock) process_caps_unlock(keycode, record);

        // Clipboard shortcuts
        if      (keycode == TH_M)    return PROCESS_SHORTCUT(Z_PST, record);
        else if (keycode == TH_COMM) return PROCESS_SHORTCUT(Z_CPY, record);
        else if (keycode == TH_DOT)  return PROCESS_SHORTCUT(Z_CUT, record);
        else if (keycode == TH_SLSH) return PROCESS_SHORTCUT(Z_UND, record);
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
