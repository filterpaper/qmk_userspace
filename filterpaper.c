// Copyright 2021 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include "filterpaper.h"


// Tap hold customization
#ifdef TAPPING_FORCE_HOLD_PER_KEY // Enable for right thumb keys
bool get_tapping_force_hold(uint16_t keycode, keyrecord_t *record) {
	return keycode == RSFT_T(KC_SPC) || keycode == LT(NUM,KC_BSPC) ? true : false;
}
#endif

#ifdef PERMISSIVE_HOLD_PER_KEY // Disable for alphanumeric tap hold
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
	return (keycode & 0xF000) == LMT_BITS ? false : true;
}
#endif


// Toggle caps lock following a word
static void process_caps_word(uint16_t keycode, keyrecord_t *record) {
	// Get base key code from mod tap
	if (record->tap.count &&
		((QK_MOD_TAP < keycode && keycode < QK_MOD_TAP_MAX) ||
		(QK_LAYER_TAP < keycode && keycode < QK_LAYER_TAP_MAX))
	) { keycode &= 0x00FF; }

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
}


// Send custom keycode on hold for mod tap
static bool process_tap_hold(uint16_t hold_keycode, keyrecord_t *record) {
	if (!record->tap.count && record->event.pressed) {
	//	tap_code16(keymap_config.swap_lctl_lgui ? C(hold_keycode) : hold_keycode);
		tap_code16(hold_keycode);
		return false;
	}
	return true;
}


bool process_record_user(uint16_t const keycode, keyrecord_t *record) {
	if (record->event.pressed) {
#if defined(OLED_ENABLE) && !defined(WPM_ENABLE)
		extern uint32_t tap_timer;
		tap_timer = timer_read32();  // Reset OLED animation tap timer
#endif
		if (host_keyboard_led_state().caps_lock) { process_caps_word(keycode, record); }
	}

	switch (keycode) {
		// Undo cut copy paste using LT(0,kc)
		case SLSH_TH: return process_tap_hold(Z_UND, record);
		case DOT_TH:  return process_tap_hold(Z_CUT, record);
		case COMM_TH: return process_tap_hold(Z_CPY, record);
		case M_TH:    return process_tap_hold(Z_PST, record);
#ifdef RGB_MATRIX_ENABLE
		case RESET:
			if (record->event.pressed) {
				rgb_matrix_set_color_all(RGB_RED);
				rgb_matrix_driver.flush();
			}
			break;
#endif
	}

	return true; // Continue with unmatched keycodes
}


#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
	if (index == 0) {
		clockwise ? tap_code(KC_VOLU) : tap_code(KC_VOLD);
	} else if (index == 1) {
		clockwise ? tap_code(KC_MNXT) : tap_code(KC_MPRV);
	}
	return false;
}
#endif
