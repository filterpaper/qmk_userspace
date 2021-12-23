// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+
// Lean caps word code using caps lock

#include QMK_KEYBOARD_H

bool process_caps_word(uint16_t keycode, keyrecord_t *record) {
	// Return if caps lock is inactive
	if (!host_keyboard_led_state().caps_lock) {
		return true;
	}

#ifndef NO_ACTION_TAPPING
	if ((QK_MOD_TAP <= keycode && keycode <= QK_MOD_TAP_MAX)
#	ifndef NO_ACTION_LAYER
	|| (QK_LAYER_TAP <= keycode && keycode <= QK_LAYER_TAP_MAX)
#	endif
	) {
		if (!record->tap.count) {
			return true;
		}
		// Get base keycode from tap hold key
		keycode &= 0xff;
	}
#endif

	// Deactivate caps lock for unmatched keycodes
	switch (keycode) {
		case KC_A ... KC_0:
		case KC_BSPC:
		case KC_MINS:
		case KC_UNDS:
			break;
		default:
			tap_code(KC_CAPS);
	}
	return true;
}
