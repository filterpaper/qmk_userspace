// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include "filterpaper.h"

#if defined(TAPPING_TERM_PER_KEY) || defined(PERMISSIVE_HOLD_PER_KEY)
#	define IS_TYPING() (timer_elapsed(tap_timer) < TAPPING_TERM * 1.3)
#	define IS_HOME_ROW() (record->event.key.row == 1 || record->event.key.row == 5)
static uint16_t tap_timer = 0;
#endif


#ifdef TAPPING_TERM_PER_KEY
// Increase tapping term in between short key presses to avoid false trigger.
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
	return IS_HOME_ROW() && IS_TYPING() ? TAPPING_TERM * 1.3 : TAPPING_TERM;
}
#endif


#ifdef PERMISSIVE_HOLD_PER_KEY
// Select Shift mod tap immediately when another key is pressed and released.
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
	return MODTAP_BIT(keycode) & MOD_MASK_SHIFT && !IS_TYPING() ? true : false;
}
#endif


#ifdef HOLD_ON_OTHER_KEY_PRESS_PER_KEY
// Select hold immediately with another key for layer tap 1 and higher.
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
	return IS_LAYER_TAP(keycode) ? true : false;
}
#endif


#ifdef TAPPING_FORCE_HOLD_PER_KEY
// Force hold on tap-hold key when held after tapping,
// enable for (right split) bottom row key.
bool get_tapping_force_hold(uint16_t keycode, keyrecord_t *record) {
	return record->event.key.row == MATRIX_ROWS - 1 ? true : false;
}
#endif


// Send custom hold keycode for mod tap.
static inline bool process_tap_hold(uint16_t hold_keycode, keyrecord_t *record) {
	if (!record->tap.count) {
		tap_code16(hold_keycode);
		return false;
	}
	return true;
}


bool process_record_user(uint16_t const keycode, keyrecord_t *record) {
	if (record->event.pressed) {
#if defined(TAPPING_TERM_PER_KEY) || defined(PERMISSIVE_HOLD_PER_KEY)
		tap_timer = timer_read();
#endif
#if defined(OLED_ENABLE) && !defined(WPM_ENABLE)
		extern uint32_t oled_tap_timer;
		oled_tap_timer = timer_read32();
#endif
#ifdef AUTO_CORRECT
		extern bool process_autocorrect(uint16_t keycode, keyrecord_t* record);
		if (!process_autocorrect(keycode, record)) {
			return false;
		}
#endif
#ifdef CAPS_UNLOCK
		extern bool process_caps_unlock(uint16_t keycode, keyrecord_t *record);
		if (!process_caps_unlock(keycode, record)) {
			return false;
		}
#endif
		// Clipboard shortcuts.
		switch(keycode) {
			case TH_SLSH: return process_tap_hold(Z_UND, record);
			case TH_DOT:  return process_tap_hold(Z_CUT, record);
			case TH_COMM: return process_tap_hold(Z_CPY, record);
			case TH_M:    return process_tap_hold(Z_PST, record);
		}
	}
	return true;
}