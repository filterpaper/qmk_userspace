// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include "filterpaper.h"

static keypos_t next_key;
static fast_timer_t tap_timer = 0;


// Turn off caps lock at word boundary
static inline bool process_caps_unlock(uint16_t keycode, keyrecord_t *record) {
	bool const caps_lock = host_keyboard_led_state().caps_lock;
	uint8_t mods = get_mods();
#ifndef NO_ACTION_ONESHOT
	mods |= get_oneshot_mods();
#endif
	// Ignore inactive state, caps lock or Shifted keys
	if (caps_lock == false || (uint8_t)keycode == KC_CAPS ||
		mods == MOD_BIT_LSHIFT || mods == MOD_BIT_RSHIFT) {
		return true;
	}
	if (IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode)) {
		if (record->tap.count == 0) {
			return true; // Ignore hold key
		}
		keycode &= 0xff; // Mask tap keycode
	}
	switch(keycode) {
		case KC_BSPC:
		case KC_MINS:
		case KC_UNDS:
		case KC_A ... KC_0:
			// Retain caps lock with no active mods
			if (mods == false) {
				break;
			}
		// Fall-through everything as word boundary
		default:
			tap_code(KC_CAPS);
	}
	return true;
}


// Send custom hold keycode for tap-hold key
static inline bool process_tap_hold(uint16_t hold_keycode, keyrecord_t *record) {
	if (record->tap.count == 0) {
		tap_code16(hold_keycode);
		return false;
	}
	return true;
}


// Copy matrix position of subsequent key before quantum processing
bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
	next_key = record->event.key;
	return true;
}


// Increase tapping term for unilateral tap and while typing
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
	return IS_UNILATERAL_TAP() || (IS_HOMEROW() && IS_TYPING()) ? TAPPING_TERM * 2 : TAPPING_TERM;
}


// Select Shift hold immediately with nested bilateral tap
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
	return IS_BILATERAL_TAP() && IS_MT_SHIFT(keycode);
}


// Select layer hold immediately with another key tap
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
	return IS_QK_LAYER_TAP(keycode) && QK_LAYER_TAP_GET_LAYER(keycode) > 1;
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	if (record->event.pressed) {
		tap_timer = timer_read_fast();
		if (!process_autocorrect(keycode, record)) {
			return false;
		}
		if (!process_caps_unlock(keycode, record)) {
			return false;
		}
		switch(keycode) { // Clipboard shortcuts
			case TH_M:    return process_tap_hold(Z_PST, record);
			case TH_COMM: return process_tap_hold(Z_CPY, record);
			case TH_DOT:  return process_tap_hold(Z_CUT, record);
			case TH_SLSH: return process_tap_hold(Z_UND, record);
		}
	}
	return true;
}


void housekeeping_task_user(void) {
	// Restore state after 3 minutes
	if (last_input_activity_elapsed() > TAPPING_TERM * 1000U) {
		if (host_keyboard_led_state().caps_lock) {
			tap_code(KC_CAPS);
		}
		if (get_highest_layer(layer_state|default_layer_state) > 0) {
			layer_off(get_highest_layer(layer_state|default_layer_state));
		}
#ifdef SWAP_HANDS_ENABLE
		if (is_swap_hands_on()) {
			swap_hands_off();
		}
#endif
	}
}