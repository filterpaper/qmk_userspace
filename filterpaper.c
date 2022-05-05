// Copyright 2021 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include "p4p3r.h"


#ifdef TAPPING_FORCE_HOLD_PER_KEY
// Force hold on tap-hold key when held after tapping
// Enable for (right split) bottom row keys
bool get_tapping_force_hold(uint16_t keycode, keyrecord_t *record) {
	return record->event.key.row == MATRIX_ROWS - 1 ? true : false;
}
#endif

#ifdef PERMISSIVE_HOLD_PER_KEY
// Select hold function immediately when another key is pressed and released
// Disable for non-Shift home row mod tap
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
	return MODTAP_BIT(keycode) == MOD_MASK_SHIFT ? true : false;
}
#endif

#ifdef HOLD_ON_OTHER_KEY_PRESS_PER_KEY
// Select hold function immediately when another key is pressed
// Enable for layer tap other than layer 0
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
	return QK_LAYER_TAP_1 <= keycode && keycode <= QK_LAYER_TAP_MAX ? true : false;
}
#endif

#ifdef QUICK_TAP_TERM_PER_KEY
uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
	return record->event.key.row == MATRIX_ROWS - 1 ? QUICK_TAP_TERM-30 : QUICK_TAP_TERM;
}
#endif


// Replace modifier with base key code on unintended activation
// with keys on the same half
#ifdef HRM_AUDIT
static bool process_hrm_audit(keyrecord_t *record) {
	// Create new keyrecord with basic keycode
	// and send it as a process record tap event
	void resend_key(uint8_t base_keycode) {
		keyrecord_t resend_key_record;
		resend_key_record.keycode = base_keycode;

		resend_key_record.event.pressed = true;
		process_record(&resend_key_record);
#	if TAP_CODE_DELAY > 0
		wait_ms(TAP_CODE_DELAY);
#	endif
		resend_key_record.event.pressed = false;
		process_record(&resend_key_record);
	}

	// Disable Alt roll with top row
	switch(record->event.key.row) {
	case 0:
		if (get_mods() == MOD_BIT(KC_LALT)) {
			unregister_mods(MOD_BIT(KC_LALT));
			resend_key((uint8_t)HM_S);
		}
		break;
	case 4:
		if (get_mods() == MOD_BIT(KC_RALT)) {
			unregister_mods(MOD_BIT(KC_RALT));
			resend_key((uint8_t)HM_L);
		}
		break;
	}

	return true;
}
#endif


// Send custom hold keycode for mod tap
static bool process_tap_hold(uint16_t hold_keycode, keyrecord_t *record) {
	if (!record->tap.count) {
		tap_code16(hold_keycode);
		return false;
	}
	return true;
}


bool process_record_user(uint16_t const keycode, keyrecord_t *record) {
	if (record->event.pressed) {
#if defined(OLED_ENABLE) && !defined(WPM_ENABLE)
		extern uint32_t tap_timer;
		tap_timer = timer_read32(); // Reset OLED animation timer
#endif
#ifdef HRM_AUDIT
		if (!process_hrm_audit(record)) {
			return false;
		}
#endif
#ifdef AUTO_CORRECT
		extern bool process_autocorrection(uint16_t keycode, keyrecord_t* record);
		if (!process_autocorrection(keycode, record)) {
			return false;
		}
#endif
#ifdef CAPS_WORD
		extern bool process_caps_word(uint16_t keycode, keyrecord_t *record);
		if (!process_caps_word(keycode, record)) {
			return false;
		}
#endif
		// LT(0,kc) clipboard shortcuts
		switch(keycode) {
		case SLSH_TH: return process_tap_hold(Z_UND, record);
		case DOT_TH:  return process_tap_hold(Z_CUT, record);
		case COMM_TH: return process_tap_hold(Z_CPY, record);
		case M_TH:    return process_tap_hold(Z_PST, record);
		}
	}

	return true; // Continue with unmatched keycodes
}


#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
	if (get_highest_layer(layer_state|default_layer_state)) {
		if (index == 0) {
			clockwise ? tap_code(KC_VOLU) : tap_code(KC_VOLD);
		} else if (index == 1) {
			clockwise ? tap_code(KC_PGDN) : tap_code(KC_PGUP);
		}
	} else { // Base layer
		if (index == 0) {
			clockwise ? tap_code(KC_VOLU) : tap_code(KC_VOLD);
		} else if (index == 1) {
			clockwise ? tap_code(KC_MNXT) : tap_code(KC_MPRV);
		}
	}
	return false;
}
#endif
