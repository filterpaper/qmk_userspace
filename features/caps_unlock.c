// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+
// Disables caps lock automatically on a "non-word" keycode.

#include QMK_KEYBOARD_H

bool process_caps_unlock(uint16_t keycode, keyrecord_t *record) {
	// Skip processing if caps lock is off or for one-shot key.
	if (host_keyboard_led_state().caps_lock == false
#ifndef NO_ACTION_ONESHOT
	|| (QK_ONE_SHOT_MOD <= keycode && keycode <= QK_ONE_SHOT_MOD_MAX)
#endif
	) {
		return true;
	}

	uint8_t mods = get_mods();
#ifndef NO_ACTION_ONESHOT
	mods |= get_oneshot_mods();
#endif

	// Ignore hold key and mask base keycode from a tap.
	if ((QK_MOD_TAP <= keycode && keycode <= QK_MOD_TAP_MAX)
	|| (QK_LAYER_TAP <= keycode && keycode <= QK_LAYER_TAP_MAX)) {
		if (!record->tap.count) {
			return true;
		}
		keycode &= 0xff;
	}

	switch(keycode) {
		case KC_A ... KC_0:
		case KC_BSPC:
		case KC_MINS:
		case KC_UNDS:
			// Retain caps lock for these keycodes if
			// there are no active non-Shift modifiers.
			if ((mods & ~MOD_MASK_SHIFT) == false) {
				break;
			}
		// Fall-through everything else to unlock caps.
		default:
			tap_code(KC_CAPS);
	}

	return true;
}