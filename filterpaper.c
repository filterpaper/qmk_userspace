// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include "filterpaper.h"


#ifdef TAPPING_TERM_PER_KEY
static uint16_t tap_timer = 0;
// Increase tapping term in between short key presses to avoid false
// trigger while typing
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
	return timer_elapsed(tap_timer) < TAPPING_TERM * 2 ? TAPPING_TERM * 2 : TAPPING_TERM;
}
#endif


#ifdef PERMISSIVE_HOLD_PER_KEY
// Select hold immediately when another key is pressed and released
// Enable for Shift mod tap
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
	return MODTAP_BIT(keycode) & MOD_MASK_SHIFT ? true : false;
}
#endif


#ifdef HOLD_ON_OTHER_KEY_PRESS_PER_KEY
// Select hold immediately when another key is pressed
// Enable for layer tap 1 and higher
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
	return QK_LAYER_TAP_1 <= keycode && keycode <= QK_LAYER_TAP_MAX ? true : false;
}
#endif


#ifdef TAPPING_FORCE_HOLD_PER_KEY
// Force hold on tap-hold key when held after tapping
// Enable for (right split) bottom row keys
bool get_tapping_force_hold(uint16_t keycode, keyrecord_t *record) {
	return record->event.key.row == MATRIX_ROWS - 1 ? true : false;
}
#endif

#ifdef QUICK_TAP_TERM_PER_KEY
// Quick Tap Term PR 17007
uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
	return record->event.key.row == MATRIX_ROWS - 1 ? QUICK_TAP_TERM - 50 : QUICK_TAP_TERM;
}
#endif


// Send custom hold keycode for mod tap
static bool process_tap_hold(uint16_t hold_keycode, keyrecord_t *record) {
	if (!record->tap.count) {
		tap_code16(hold_keycode);
		return false;
	}
	return true;
}


bool process_record_user(uint16_t const keycode, keyrecord_t *record) {
	if (record->event.pressed) {
#ifdef TAPPING_TERM_PER_KEY
		tap_timer = timer_read();
#endif
#if defined(OLED_ENABLE) && !defined(WPM_ENABLE)
		extern uint32_t oled_tap_timer;
		oled_tap_timer = timer_read32(); // Reset OLED animation timer
#endif
#ifdef HRM_AUDIT
		extern bool process_hrm_audit(keyrecord_t *record);
		if (!process_hrm_audit(record)) {
			return false;
		}
#endif
#ifdef AUTO_CORRECT
		extern bool process_autocorrection(uint16_t keycode, keyrecord_t* record);
		if (!process_autocorrection(keycode, record)) {
			return false;
		}
#endif
#ifdef CAPS_UNLOCK
		extern bool process_caps_unlock(uint16_t keycode, keyrecord_t *record);
		if (!process_caps_unlock(keycode, record)) {
			return false;
		}
#endif
		// LT(0,kc) clipboard shortcuts
		switch(keycode) {
		case SLSH_TH: return process_tap_hold(Z_UND, record);
		case DOT_TH:  return process_tap_hold(Z_CUT, record);
		case COMM_TH: return process_tap_hold(Z_CPY, record);
		case M_TH:    return process_tap_hold(Z_PST, record);
		}
	}

	return true; // Continue with unmatched keycodes
}
