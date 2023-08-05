// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include "filterpaper.h"


#if defined (PERMISSIVE_HOLD_PER_KEY) || defined (HOLD_ON_OTHER_KEY_PRESS_PER_KEY)
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
#endif


#ifdef TAPPING_TERM_PER_KEY
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    // Decrease tapping term for home row Shift
    return IS_HOMEROW(record) && IS_MOD_TAP_SHIFT(keycode) ? TAPPING_TERM - 30 : TAPPING_TERM;
}
#endif


#ifdef PERMISSIVE_HOLD_PER_KEY
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    // Hold Control and Shift with a nested key tap on the opposite hand
    return IS_BILATERAL_TAP(record, next_record) && IS_MOD_TAP_CS(keycode);
}
#endif


#ifdef HOLD_ON_OTHER_KEY_PRESS_PER_KEY
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    // Replace the mod-tap key with its base keycode when
    // tapped with another non-Shift key on the same hand
    if (IS_UNILATERAL_TAP(record, next_record) && !IS_MOD_TAP_SHIFT(next_keycode)) {
        record->keycode = keycode & 0xff;
        process_record(record);
        record->event.pressed = false;
        process_record(record);
        return true;
    }
    // Hold layer with another key press
    else if (IS_QK_LAYER_TAP(keycode) && QK_LAYER_TAP_GET_LAYER(keycode)) {
        return true;
    }
    return false;
}
#endif


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

    // Identify word breaks
    if (mods & ~MOD_MASK_SHIFT ||
        !(
            keycode == KC_CAPS ||
            keycode == KC_BSPC ||
            keycode == KC_MINS ||
            keycode == KC_UNDS ||
            (KC_A <= keycode && keycode <= KC_0)
        )
    ) tap_code(KC_CAPS);

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