// Copyright 2022 @filterpaper
// SPDX-License-Identifier: Apache-2.0

/* Simple unilateral home row mod handler that replaces activated
   modifier with the base keycode if rolled on the same half. It will
   prevent the activation of mod-tap modifiers on the same hand.

   Code assumes left and right halves are horizontally with MATRIX_COLS/2
   while split keyboard matrix is divided with MATRIX_ROWS/2.

   Usage:
   Include this source file in 'rules.mk' with:
   SRC += unilateral-mods.c

   Call 'process_unilateral_taps' in 'process_record_user':
   if (record->event.pressed) {
     extern void process_unilateral_taps(uint16_t keycode, keyrecord_t *record);
     process_unilateral_taps(keycode, record);
   }

 */

#include QMK_KEYBOARD_H

// List of left and right home row mod keys to consider
static uint16_t const l_hrm[] = {
	HM_A,
	HM_S,
	HM_D,
	HM_F
};

static uint16_t const r_hrm[] = {
	HM_J,
	HM_K,
	HM_L,
	HM_QUOT
};


#ifdef SPLIT_KEYBOARD
// Split keyboard matrix is arranged vertically
#	define KEY_POSITION record->event.key.row
#	define KB_MIDPOINT  MATRIX_ROWS/2
#else
#	define KEY_POSITION record->event.key.col
#	define KB_MIDPOINT  MATRIX_COLS/2
#endif


static uint8_t get_modtap_bit(uint16_t kc) {
	// Check for right mod-tap flag and shift 4 bits left for right mod_bit.
	return (kc & 0x1000) ? ((kc>>8)&0xf)<<4 : (kc>>8)&0xf;
}


// Mod-tap modifier that is replaced with a tap is recreated as a
// keyrecord_t and send through process_record() as a tap record.
static void resend_key(uint16_t modtap_kc) {
	// Remove activated modifier.
	unregister_mods(get_modtap_bit(modtap_kc));
	// Create keyrecord with mod tap basic keycode
	// and send it as a process record tap event.
	keyrecord_t resend_key_record;
	resend_key_record.keycode = (uint8_t)modtap_kc;

	resend_key_record.event.pressed = true;
	process_record(&resend_key_record);
#if TAP_CODE_DELAY > 0
	wait_ms(TAP_CODE_DELAY);
#endif
	resend_key_record.event.pressed = false;
	process_record(&resend_key_record);
}


void process_unilateral_taps(uint16_t keycode, keyrecord_t *record) {
	if ((QK_MOD_TAP <= keycode && keycode <= QK_MOD_TAP_MAX && record->tap.count) || IS_ANY(keycode)) {
		if (KEY_POSITION < KB_MIDPOINT) {
			// Process key press on the left half. If left mod is active, assume it
			// was rolled from the previous left mod tap. Replace the activated
			// modifier with its base keycode.
			for (uint8_t l = 0; l < sizeof(l_hrm)/sizeof(uint16_t); ++l) {
				if (get_mods() == get_modtap_bit(l_hrm[l])) {
					resend_key(l_hrm[l]);
				}
			}
		}
		if (KEY_POSITION >= KB_MIDPOINT) {
			// Process key press on the right half. If right mod is active, assume it
			// was rolled from the previous right mod tap. Replace the activated
			// modifier with its base keycode.
			for (uint8_t r = 0; r < sizeof(r_hrm)/sizeof(uint16_t); ++r) {
				if (get_mods() == get_modtap_bit(r_hrm[r])) {
					resend_key(r_hrm[r]);
				}
			}
		}
	}
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




// Faux quick_tap
static bool process_fast_tap(uint16_t keycode, keyrecord_t *record) {
	// Record non-mod tap keycodes or tapped mod taps
	if (!(QK_MOD_TAP <= keycode && keycode <= QK_MOD_TAP_MAX) ||
	(record->event.pressed && record->tap.count && timer_elapsed(tap_timer) >= TAPPING_TERM)) {	
		tap_timer = timer_read();
		return true;
	}
	// Tap base keycode of mod tap that follows another key within fast tap term
	if (record->event.pressed && timer_elapsed(tap_timer) < FAST_TAP_TERM) {
		tap_code(keycode &= 0xff);
		tap_timer = timer_read();
		return false;
	}
	return true;
}
