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

// Checks for a tap-hold key that should be forced to tap in quick succession:
// a non-Shift tap-hold after a letter key (A–Z), or a Shift-tap after another
// Shift-tap, both within a short interval
#define IS_QUICK_SUCCESSION_INPUT(k, r, ctx) (             \
    IS_TAP_HOLD((k)) && IS_ALPHA_ROW((r))                  \
    && (!IS_SHIFT_TAP((k)) || IS_SHIFT_TAP((ctx).keycode)) \
    && GET_TAP_KEYCODE((ctx).keycode) <= KC_Z              \
    && last_matrix_activity_elapsed() <= QUICK_TAP_TERM    \
)

// Same-hand key press detection by comparing two rows using bitmasks
// where row 1 matches rows 0-2 (0x07) and row 5 matches rows 4-6 (0x70)
#define LATERAL_MASK(n) ((n) == 1 ? 0x07 : ((n) == 5 ? 0x70 : 0x00))
#define IS_UNILATERAL(r1, r2) (LATERAL_MASK(r1) & (1U << (r2)))

// Checks for an alpha row key overlapping another non-Shift key on the same
// side of the keyboard, or a shortcut tap-hold key overlapping with any key
#define IS_CHORDAL_TAP_INPUT(k, r, ctx) (                                          \
    (IS_UNILATERAL((r)->event.key.row, (ctx).row) && !IS_SHIFT_TAP((ctx).keycode)) \
    || (IS_QK_LAYER_TAP((k)) && QK_LAYER_TAP_GET_LAYER((k)) == 0)                  \
)

// Checks for Shift-tap exemption when the contextual
// tap-hold state matches an active Shift modifier state
#define IS_SHIFT_EXEMPT(k, ctx) (                                          \
    IS_SHIFT_TAP((k))                                                      \
    && (IS_TAP_HOLD((ctx).keycode) == (bool)(get_mods() & MOD_MASK_SHIFT)) \
)

// Bitmask of letters excluded from Shift chord detection,
// encoded at their respective KC_ value bit positions
#define LETTER_MASK (1U<<KC_A | 1U<<KC_E | 1U<<KC_H | 1U<<KC_I | \
                     1U<<KC_L | 1U<<KC_O | 1U<<KC_U | 1U<<KC_Y)

// Checks for Shift activation when a Shift-tap key is pressed with
// a letter key that is not part of a word composition with F and J
#define IS_SHIFT_CHORD(k, r, ctx) ({              \
    uint8_t _kc = GET_TAP_KEYCODE((ctx).keycode); \
    IS_SHIFT_TAP((k)) && IS_ALPHA_ROW((r))        \
    && (_kc <= KC_Z || _kc == KC_QUOT)            \
    && !((LETTER_MASK >> _kc) & 1);               \
})

// Checks for hold activation when: the current key is a layer tap,
// a Shift chord is formed with a qualifying letter, or a non-zero
// layer tap follows a pending alpha-row key
#define IS_CHORDAL_HOLD_INPUT(k, r, ctx) (                          \
    IS_QK_LAYER_TAP((k))                                            \
    || IS_SHIFT_CHORD((k), (r), (ctx))                              \
    || (IS_ALPHA_ROW((r)) && IS_QK_LAYER_TAP((ctx).keycode)         \
                          && QK_LAYER_TAP_GET_LAYER((ctx).keycode)) \
)

// Tap keycode pressed state tracker
static bool tapped[UINT8_MAX + 1];

// Contextual input cache
static struct {
    uint16_t keycode;
    uint8_t  row;
} context;


bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
    const uint8_t tap_keycode = GET_TAP_KEYCODE(keycode);
    if (record->event.pressed) {
        // Force the current key to tap if it arrives in quick succession
        // within QUICK_TAP_TERM under any of the following conditions:
        // * The current key is a non-Shift tap-hold on the alpha row and
        //   the previous key was a letter (A–Z)
        // * The current key is a Shift-tap on the alpha row and the
        //   previous key was also a Shift-tap on the alpha row
        // These conditions prevent accidental hold activation during fast
        // typing with Shift-taps exempt unless preceded by another Shift-tap
        if (IS_QUICK_SUCCESSION_INPUT(keycode, record, context)) {
            tapped[tap_keycode] = true;
            record->keycode     = tap_keycode;
        }
        // Store the intermediate input for contextual processing
        context.keycode = keycode;
        context.row     = record->event.key.row;
    } else {
        // Clear tapped flag and set tap count for release handling
        if (tapped[tap_keycode]) {
            tapped[tap_keycode] = false;
            record->tap.count   = 1;
        }
    }
    return true;
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    // Force the pending tap-hold to tap under any of the following conditions:
    // * The pending key and the next key are on the same hand, and the
    //   next key is not a Shift-tap
    // * The pending key is a layer-0 shortcut tap-hold
    // These conditions prevent hold activation on same-hand rolls and shortcuts
    if (IS_CHORDAL_TAP_INPUT(keycode, record, context)) {
        if (!IS_SHIFT_EXEMPT(keycode, context)) {
            tapped[GET_TAP_KEYCODE(keycode)] = true;
        }
        record->tap.interrupted = false;
        record->tap.count       = 1;
        return true;
    }
    // Force the pending tap-hold to hold under any of the following conditions:
    // * The pending key is a non-zero layer-tap
    // * The pending key is an alpha row Shift-tap and the next key is a qualifying
    //   letter (A–Z) that forms a Shift chord
    // * The pending key is an alpha row tap-hold and the next key is a non-zero layer-tap
    // These conditions allow layer-hold and Shift-chord to activate immediately
    return IS_CHORDAL_HOLD_INPUT(keycode, record, context);
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
