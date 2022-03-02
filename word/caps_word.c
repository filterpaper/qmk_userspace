// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+
// Lean caps word code using caps lock

#include QMK_KEYBOARD_H

bool process_caps_word(uint16_t keycode, keyrecord_t *record) {
	// Return and ignore further processing if caps lock was
	// not enabled or key press was a one-shot-mod
	if (!host_keyboard_led_state().caps_lock
#ifndef NO_ACTION_ONESHOT
		|| (QK_ONE_SHOT_MOD <= keycode && keycode <= QK_ONE_SHOT_MOD_MAX)
#endif
	) {
		return true;
	}

	// Ignore hold from a tap-hold key and mask base keycode from a tap
#ifndef NO_ACTION_TAPPING
	if ((QK_MOD_TAP <= keycode && keycode <= QK_MOD_TAP_MAX)
#	ifndef NO_ACTION_LAYER
		|| (QK_LAYER_TAP <= keycode && keycode <= QK_LAYER_MOD_MAX)
#	endif
	) {
		if (!record->tap.count) {
			return true;
		}
		keycode &= 0xff;
	}
#endif

	// Deactivate caps lock if keycodes do not match the
	// following or mods other than shift is used
	if (
		(!(KC_A <= keycode && keycode <= KC_0)
		&& keycode != KC_BSPC
		&& keycode != KC_MINS
		&& keycode != KC_UNDS)
		|| (get_mods() & ~MOD_MASK_SHIFT)
	) {
		tap_code(KC_CAPS);
	}
	return true;
}
