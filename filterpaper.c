// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include "filterpaper.h"


#if (defined PERMISSIVE_HOLD_PER_KEY || defined HOLD_ON_OTHER_KEY_PRESS_PER_KEY)
static uint16_t next_keycode;
static keyrecord_t next_record;

bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
	if (record->event.pressed) {
		// Copy the next key record for mod-tap decisions
		next_keycode = keycode;
		next_record = *record;
	}
	return true;
}
#endif


#ifdef TAPPING_TERM_PER_KEY
static fast_timer_t tap_timer = 0;

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
	// Increase tapping term for the home row mod-tap while typing
	return IS_HOMEROW(record) && IS_TYPING()
#	ifndef PERMISSIVE_HOLD_PER_KEY
		&& !IS_MOD_TAP_SHIFT(keycode)
#	endif
		? TAPPING_TERM * 2 : TAPPING_TERM;
}
#endif


#ifdef PERMISSIVE_HOLD_PER_KEY
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
	// Hold Shift with a nested bilateral tap
	return IS_BILATERAL_TAP(record, next_record) && IS_MOD_TAP_SHIFT(keycode);
}
#endif


#ifdef HOLD_ON_OTHER_KEY_PRESS_PER_KEY
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
	// Replace the mod-tap key with its base keycode when
	// tapped with another non-modifier key on the same hand
	if (IS_UNILATERAL_TAP(record, next_record) && !IS_QK_MOD_TAP(next_keycode)) {
		record->keycode = keycode & 0xff;
		return true;
	// Hold layer with another key tap
	} else if (IS_QK_LAYER_TAP(keycode) && QK_LAYER_TAP_GET_LAYER(keycode)) {
		return true;
	}
	return false;
}
#endif


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	if (record->event.pressed) {
#ifdef TAPPING_TERM_PER_KEY
		tap_timer = timer_read_fast();
#endif
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


// Process custom hold keycode
static inline bool process_tap_hold(uint16_t keycode, keyrecord_t *record) {
	if (record->tap.count == 0) {
		tap_code16(keycode);
		return false;
	}
	return true;
}


// Turn off caps lock on a word break
static inline bool process_caps_unlock(uint16_t keycode, keyrecord_t *record) {
	bool const caps_lock = host_keyboard_led_state().caps_lock;
	uint8_t mods = get_mods();
#ifndef NO_ACTION_ONESHOT
	mods |= get_oneshot_mods();
#endif
	// Ignore non-lock state, pass-through caps lock and shifted keys
	if (caps_lock == false || keycode == KC_CAPS || mods == MOD_BIT_LSHIFT || mods == MOD_BIT_RSHIFT) {
		return true;
	}
	// Filter mod-tap and layer-tap keys
	if (IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode)) {
		if (record->tap.count == 0) {
			return true; // Ignore hold key
		}
		keycode &= 0xff; // Mask tap keycode
	}
	// Identify word breaks
	switch(keycode) {
		case KC_BSPC:
		case KC_MINS:
		case KC_UNDS:
		case KC_A ... KC_0:
		// Not a word break if there are no active modifiers
			if (mods == false) {
				break;
			}
		default:
			tap_code(KC_CAPS);
	}
	return true;
}