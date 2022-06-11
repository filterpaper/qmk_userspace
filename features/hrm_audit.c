// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+
// Replace modifier with base key code on unintended activation
// with keys on the same half

#include QMK_KEYBOARD_H

bool process_hrm_audit(keyrecord_t *record) {
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
