// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+
// Lean caps word code using caps lock

#include QMK_KEYBOARD_H

bool process_caps_word(uint16_t keycode, keyrecord_t *record) {
	bool mods = get_mods();
#ifndef NO_ACTION_ONESHOT
	mods |= get_oneshot_mods();
#endif

	// Skip processing if caps lock is off and for one-shot key.
	if (host_keyboard_led_state().caps_lock == false
#ifndef NO_ACTION_ONESHOT
	|| (QK_ONE_SHOT_MOD <= keycode && keycode <= QK_ONE_SHOT_MOD_MAX)
#endif
	) {
		return true;
	}

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
			// Retain caps lock for word keys if there
			// are no active modifiers other than Shift.
			if ((mods & ~MOD_MASK_SHIFT) == false) {
				break;
			}
		// Fall-through active non-Shift modifiers.
		default:
			// Deactivate caps on word boundary.
			tap_code(KC_CAPS);
	}

	return true;
}
