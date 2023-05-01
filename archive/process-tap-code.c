// Copyright @filterpaper
// SPDX-License-Identifier: Apache-2.0

static uint8_t get_modtap_bit(uint16_t kc) {
	// Check for right mod-tap flag and shift 4 bits left for right mod_bit.
	return (kc & 0x1000) ? ((kc>>8)&0xf)<<4 : (kc>>8)&0xf;
}


static inline void process_mod_roll(keyrecord_t *record) {
	// Replace activated mod with tap keycode
	inline void replace_mod(uint8_t mod_bit, uint8_t tap_keycode) {
		keyrecord_t replacement;
		replacement.event.pressed = true;
		replacement.keycode = tap_keycode;

		unregister_mods(mod_bit);
		process_record(&replacement);
#if TAP_CODE_DELAY > 0
		wait_ms(TAP_CODE_DELAY);
#endif
		replacement.event.pressed = false;
		process_record(&replacement);
	}

	// Disable mod rolls with its upper row
	if (record->event.key.row == 0) {
		if (get_mods() == MOD_BIT_LALT) {
			replace_mod(MOD_BIT_LALT, (uint8_t)HM_S);
		} else if (get_mods() == MOD_BIT_LSHIFT) {
			replace_mod(MOD_BIT_LSHIFT, (uint8_t)HM_A);
		}
	} else if (record->event.key.row == 4) {
		if (get_mods() == MOD_BIT_RALT) {
			replace_mod(MOD_BIT_RALT, (uint8_t)HM_L);
		}
	}
}


// Handle keyrecord before quantum processing
bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {

	// Implement instant-tap of mod-tap keys
	if (IS_HOME_ROW(record) && IS_QK_MOD_TAP(keycode)) {
		keyrecord_t instant_tap_record;
		instant_tap_record.keycode = keycode & 0xff;

		// When a matched mod-tap key is pressed within QUICK_TAP_TERM of a previous key,
		// send its masked base keycode through processing
		if (record->event.pressed && (timer_elapsed_fast(tap_timer) < QUICK_TAP_TERM)) {
			instant_tap_record.event.pressed = true;
			action_tapping_process(instant_tap_record);
#if TAP_CODE_DELAY > 0
			wait_ms(TAP_CODE_DELAY);
#endif
			return false; // Skip processing
		} else {
			// Handle record key up event
			instant_tap_record.event.pressed = false;
			process_record(&instant_tap_record);
		}
	}

	return true; // Continue normal processing
}