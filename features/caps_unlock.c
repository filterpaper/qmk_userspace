// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+
// Toggle caps lock automatically after a word

#include QMK_KEYBOARD_H

bool process_caps_unlock(uint16_t keycode, keyrecord_t *record) {
	bool caps_lock = host_keyboard_led_state().caps_lock;

	if (caps_lock == false || (uint8_t)keycode == KC_CAPS) {
		return true;
	}

	if (IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode)) {
		// Ignore hold key.
		if (record->tap.count == 0) {
			return true;
		}
		// Mask base keycode.
		keycode &= 0xff;
	}

	uint8_t mods = get_mods();
#ifndef NO_ACTION_ONESHOT
	mods |= get_oneshot_mods();
#endif

	switch(keycode) {
		case KC_BSPC:
		case KC_MINS:
		case KC_UNDS:
		case KC_A ... KC_0:
			// Retain caps lock for these keycodes if
			// there are no active non-Shift modifiers.
			if ((mods & ~MOD_MASK_SHIFT) == false) {
				break;
			}
		// Fall-through everything else.
		default:
			tap_code(KC_CAPS);
	}

	return true;
}