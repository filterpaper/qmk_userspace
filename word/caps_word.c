// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+
// Lean caps word code using caps lock

#include QMK_KEYBOARD_H

bool process_caps_word(uint16_t keycode, keyrecord_t *record) {
	bool mods = get_mods();
#ifndef NO_ACTION_ONESHOT
	mods |= get_oneshot_mods();
#endif

	// Skip processing if caps lock is off and for one-shot keys.
	if (!host_keyboard_led_state().caps_lock
#ifndef NO_ACTION_ONESHOT
		|| (QK_ONE_SHOT_MOD <= keycode && keycode <= QK_ONE_SHOT_MOD_MAX)
#endif
	) {
		return true;
	}

	// Ignore hold from a tap-hold key and mask base keycode from a tap.
#ifndef NO_ACTION_TAPPING
	if ((QK_MOD_TAP <= keycode && keycode <= QK_MOD_TAP_MAX)
#	ifndef NO_ACTION_LAYER
		|| (QK_LAYER_TAP <= keycode && keycode <= QK_LAYER_TAP_MAX)
#	endif
	) {
		if (!record->tap.count) {
			return true;
		}
		keycode &= 0xff;
	}
#endif

	// Deactivate caps lock for non-word keycodes
	// or if a modifier other than Shift is active.
	switch(keycode) {
		case KC_A...KC_0:
		case KC_BSPC:
		case KC_MINS:
		case KC_UNDS:
			if ((mods & ~MOD_MASK_SHIFT) == false) {
				break;
			}
		default:
			tap_code(KC_CAPS);
	}

	return true;
}
