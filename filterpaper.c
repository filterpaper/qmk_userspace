// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include QMK_KEYBOARD_H
#include "autocorrect.h"
#ifdef COMBO_ENABLE
#   include "combos.h"
#endif

// Convert 5-bit to 8-bit packed modifiers by extracting the
// upper bit and shift by 4 if bit 12 is set, otherwise by 8
#define MOD_TAP_GET_MOD_BITS(k) (((k) & 0x0f00) >> (((k) & 0x1000) ? 4 : 8))
// Basic keycode filter for tap-hold keys
#define GET_TAP_KEYCODE(k) ((k) & 0xFF)

// Tap-hold decision helper macros
#define IS_TAP_HOLD(k) (IS_QK_LAYER_TAP(k) || IS_QK_MOD_TAP(k))
#define IS_SHORTCUT(k) (IS_QK_LAYER_TAP(k) && !QK_LAYER_TAP_GET_LAYER(k))
#define IS_LAYER_TAP(k) (IS_QK_LAYER_TAP(k) && QK_LAYER_TAP_GET_LAYER(k))
#define IS_MOD_TAP_SHIFT(k) (IS_QK_MOD_TAP(k) && (k) & QK_LSFT)
#define IS_MOD_TAP_CAG(k) (IS_QK_MOD_TAP(k) && (k) & (QK_LCTL|QK_LALT|QK_LGUI))

#define HOMEROW_MASK ((1U << 1) | (1U << 5)) // Bitmasks for rows 1 and 5
#define IS_HOMEROW(r) (HOMEROW_MASK & (1U << (r)->event.key.row))
#define IS_HOMEROW_SHIFT(k, r) (IS_HOMEROW(r) && IS_MOD_TAP_SHIFT(k))
#define IS_HOMEROW_CAG(k, r) (IS_HOMEROW(r) && IS_MOD_TAP_CAG(k))

// Same-hand key press detection by comparing two key events using row bitmasks
// Row 1 matches rows 0,1,2 (bitmask 0x07) and row 5 matches rows 4,5,6 (bitmask 0x70)
#define UNILATERAL_MASK(h) ((h) == 1 ? 0x07 : ((h) == 5 ? 0x70 : 0x00))
#define IS_UNILATERAL(r, c) (UNILATERAL_MASK((r)->event.key.row) & (1U << (c).event.key.row))
#define IS_UNILATERAL_AND_NOT_SHIFT(r, c) (IS_UNILATERAL((r), (c).record) && !IS_MOD_TAP_SHIFT((c).keycode))

// Recent letter key input detection
#define IS_TYPING(k) ((GET_TAP_KEYCODE(k) <= KC_Z) && (last_matrix_activity_elapsed() < INPUT_IDLE_MS))


// Struct for tap keycode bit array indexing
typedef struct {
    uint_fast8_t  index;
    uint_fast32_t bitmask;
} tap_bit_t;

#define TAP_BIT_FROM_KEYCODE(k)                    \
    ((tap_bit_t){                                  \
        .index   = GET_TAP_KEYCODE(k) / 32,        \
        .bitmask = 1U << (GET_TAP_KEYCODE(k) % 32) \
    })

// Bit array for tracking pressed state of tap keycodes (0-255)
static uint32_t pressed_keys[(UINT8_MAX + 31) / 32];

// Contextual input cache
static struct {
    uint_fast16_t keycode;
    keyrecord_t   record;
} context;


bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) { // Key press
        if ((IS_HOMEROW_CAG(keycode, record) || IS_SHORTCUT(keycode)) && (IS_TYPING(context.keycode))) {
            // Press the tap keycode when it follows the previous key swiftly
            const tap_bit_t tap = TAP_BIT_FROM_KEYCODE(keycode);
            pressed_keys[tap.index] |= tap.bitmask;
            record->keycode = GET_TAP_KEYCODE(keycode);
        }

        // Store the intermediate input for contextual processing
        context.keycode = keycode;
        context.record  = *record;

    } else if (IS_TAP_HOLD(keycode)) { // Tap-hold key release
        const tap_bit_t tap = TAP_BIT_FROM_KEYCODE(keycode);
        if (pressed_keys[tap.index] & tap.bitmask) {
            // Release the tap keycode if pressed
            pressed_keys[tap.index] &= ~tap.bitmask;
            record->tap.count++;
        }
    }

    return true;
}


bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    // If the tap-hold key overlaps with another non-Shift key on the same
    // hand or if the key is a shortcut overlapping with any other key,
    // track its pressed state and process it as a tap
    if (IS_UNILATERAL_AND_NOT_SHIFT(record, context) || IS_SHORTCUT(keycode)) {
        const tap_bit_t tap = TAP_BIT_FROM_KEYCODE(keycode);
        pressed_keys[tap.index] |= tap.bitmask;
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


// Letter and word-related keys that retain caps lock
#define IS_LETTER_KEY(k) (          \
    ((k) >= KC_A && (k) <= KC_0) || \
    (k) == KC_BSPC ||               \
    (k) == KC_MINS ||               \
    (k) == KC_UNDS ||               \
    (k) == KC_CAPS                  \
)

static inline void process_caps_unlock(uint16_t keycode, keyrecord_t *record) {
    // Get tap keycode from tap-hold keys
    if (IS_TAP_HOLD(keycode)) {
        if (record->tap.count == 0) return;
        keycode = GET_TAP_KEYCODE(keycode);
    }

    // Keep caps lock if there are no non-Shift modifiers
    if (IS_LETTER_KEY(keycode) && ((get_mods() & ~MOD_MASK_SHIFT) == 0)) return;

    // Toggle caps lock at word boundary
    tap_code(KC_CAPS);
}


// Send shortcut keycode for held key
#define PROCESS_TAP_HOLD(k, r) (              \
    (r)->tap.count ? true                     \
                   : (tap_code16((k)), false) \
)

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
