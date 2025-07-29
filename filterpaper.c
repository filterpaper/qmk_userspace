// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include QMK_KEYBOARD_H
#include "autocorrect.h"

// Convert 5-bit to 8-bit packed modifiers by extracting the
// upper bit and shift by 4 if bit 12 is set, otherwise by 8
#define MOD_TAP_GET_MOD_BITS(k) (((k) & 0x0F00) >> (((k) & 0x1000) ? 4 : 8))
#define GET_TAP_KEYCODE(k) ((k) & 0xFF)

#define IS_TAP_HOLD(k) (IS_QK_MOD_TAP((k)) || IS_QK_LAYER_TAP((k)))
#define IS_SHIFT_TAP(k) (((k) & QK_LSFT) && IS_QK_MOD_TAP((k)))
// Alpha row key detection using bitmask 0x77 for rows 0-2 and 4-6
#define IS_ALPHA_ROW(r) ((1U << (r)->event.key.row) & 0x77)

// Checks for a non-Shift tap-hold key pressed in rapid succession
// after a letter key (A–Z) within a short interval
#define IS_QUICK_SUCCESSION_INPUT(k, r, ctx) (                    \
    IS_TAP_HOLD((k)) && !IS_SHIFT_TAP((k)) && IS_ALPHA_ROW((r))   \
    && GET_TAP_KEYCODE((ctx).keycode) <= KC_Z                     \
    && last_matrix_activity_elapsed() <= QUICK_TAP_TERM           \
)

// Same-hand key press detection by comparing two key events using bitmasks
// where row 1 matches rows 0-2 (0x07) and row 5 matches rows 4-6 (0x70)
#define LATERAL_MASK(n) ((n) == 1 ? 0x07 : ((n) == 5 ? 0x70 : 0x00))
#define IS_UNILATERAL(r, c) (LATERAL_MASK((r)->event.key.row) & (1U << (c).event.key.row))

// Checks for a home row key overlapping another non-Shift key on the same
// side of the keyboard, or a shortcut tap-hold key overlapping with any key
#define IS_CHORDAL_TAP_INPUT(k, r, ctx) (                                \
    (IS_UNILATERAL((r), (ctx).record) && !IS_SHIFT_TAP((ctx).keycode))   \
    || (IS_QK_LAYER_TAP((k)) && QK_LAYER_TAP_GET_LAYER((k)) == 0)        \
)


// Tap keycode pressed state tracker
static bool tapped[UINT8_MAX + 1];

// Contextual input cache
static struct {
    uint16_t    keycode;
    keyrecord_t record;
} context;


bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        // Flag key as tapped and update the key record with its tap keycode
        if (IS_QUICK_SUCCESSION_INPUT(keycode, record, context)) {
            uint8_t tap_keycode = GET_TAP_KEYCODE(keycode);
            tapped[tap_keycode] = true;
            record->keycode     = tap_keycode;
        }
        // Store the intermediate input for contextual processing
        context.keycode = keycode;
        context.record  = *record;
    } else {
        // Clear tapped flag and set tap count for release
        uint8_t tap_keycode = GET_TAP_KEYCODE(keycode);
        if (tapped[tap_keycode]) {
            tapped[tap_keycode] = false;
            record->tap.count++;
        }
    }
    return true;
}


bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    if (IS_CHORDAL_TAP_INPUT(keycode, record, context)) {
        // Flag key as tapped if next keycode is a tap-hold key
        if (IS_TAP_HOLD(context.keycode)) {
            tapped[GET_TAP_KEYCODE(keycode)] = true;
        }
        record->tap.interrupted = false;
        record->tap.count++;
        return true;
    }
    // Activate layer hold with another key press
    return IS_QK_LAYER_TAP(keycode) && QK_LAYER_TAP_GET_LAYER(keycode);
}


uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    return IS_SHIFT_TAP(keycode) && IS_ALPHA_ROW(record) ? SHIFT_TAP_TERM : TAPPING_TERM;
}


static inline bool process_caps_word(uint16_t keycode, keyrecord_t *record) {
    if (!host_keyboard_led_state().caps_lock) return true;
    // Get tap keycode from tap-hold keys
    if (IS_TAP_HOLD(keycode)) {
        if (record->tap.count == 0) return true;
        keycode = GET_TAP_KEYCODE(keycode);
    }
    // Letter and word-related keys that retain caps lock
    bool is_word_key = keycode <= KC_0
        || keycode == KC_BSPC
        || keycode == KC_MINS
        || keycode == KC_UNDS
        || keycode == KC_CAPS;
    // Toggle caps lock at a word boundary if the key is not
    // a word key or if any non-Shift modifiers are active
    if (!is_word_key || (get_mods() & ~MOD_MASK_SHIFT)) {
        tap_code(KC_CAPS);
    }
    return true;
}


// Send shortcut keycode for held key
#define PROCESS_SHORTCUT(k, r) ((r)->tap.count ? true : (tap_code16((k)), false))

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        if (!process_autocorrect(keycode, record) || !process_caps_word(keycode, record)) {
            return false;
        }
        // Handle clipboard tap-hold shortcuts
        if      (keycode == TH_M)    return PROCESS_SHORTCUT(Z_PST, record);
        else if (keycode == TH_COMM) return PROCESS_SHORTCUT(Z_CPY, record);
        else if (keycode == TH_DOT)  return PROCESS_SHORTCUT(Z_CUT, record);
        else if (keycode == TH_SLSH) return PROCESS_SHORTCUT(Z_UND, record);
    }
    return true;
}
