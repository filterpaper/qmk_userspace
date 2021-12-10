// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+
// Toggle caps lock following a word

#include QMK_KEYBOARD_H

bool process_caps_word(uint16_t keycode, keyrecord_t *record) {
	// Ignore if caps lock is not enabled
	if (!host_keyboard_led_state().caps_lock) { return true; }

	// Get base key code from mod tap
	if (record->tap.count &&
		((QK_MOD_TAP <= keycode && keycode <= QK_MOD_TAP_MAX) ||
		(QK_LAYER_TAP <= keycode && keycode <= QK_LAYER_TAP_MAX))
	) {
		keycode &= 0xff;
	}

	// Deactivate caps lock with listed keycodes
	switch (keycode) {
		case KC_TAB:
		case KC_ESC:
		case KC_SPC:
		case KC_ENT:
		case KC_DOT:
		case KC_EQL:
		case KC_GESC:
			tap_code(KC_CAPS);
	}
	return true;
}
