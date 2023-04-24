// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+
// Disable caps lock at word boundary

#include QMK_KEYBOARD_H

bool process_caps_unlock(uint16_t keycode, keyrecord_t *record) {
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