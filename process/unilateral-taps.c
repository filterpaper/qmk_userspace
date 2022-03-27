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

#ifdef SPLIT_KEYBOARD
// Split keyboard matrix is arranged vertically
#	define KEY_POSITION record->event.key.row
#	define KB_MIDPOINT  MATRIX_ROWS/2
#else
// On non-split, left and right are split across column
#	define KEY_POSITION record->event.key.col
#	define KB_MIDPOINT  MATRIX_COLS/2
#endif

void process_unilateral_taps(uint16_t keycode, keyrecord_t *record) {
	uint8_t get_modtap_bit(uint16_t kc) {
		// Check for right mod-tap flag and shift 4 bits left for right mod_bit.
		return (kc & 0x1000) ? ((kc>>8)&0xf)<<4 : (kc>>8)&0xf;
	}

	// Mod-tap modifier that is replaced with a tap is recreated as a
	// keyrecord_t and send through process_record() as a tap record.
	void re_process(uint16_t modtap_kc) {
		// Remove modifier status
		unregister_mods(get_modtap_bit(modtap_kc));
		// Create keyrecord with mod-tap basic keycode
		// and send it as a process record tap event
		keyrecord_t re_process_record;
		re_process_record.keycode = (uint8_t)modtap_kc;
		re_process_record.event.pressed = true;
		process_record(&re_process_record);
#if TAP_CODE_DELAY > 0
		wait_ms(TAP_CODE_DELAY);
#endif
		re_process_record.event.pressed = false;
		process_record(&re_process_record);
	}

	if ((QK_MOD_TAP <= keycode && keycode <= QK_MOD_TAP_MAX && record->tap.count) || IS_ANY(keycode)) {
		if (KEY_POSITION < KB_MIDPOINT) {
			// Process key press on the left half. If left mod is active, assume it
			// was rolled from the previous left mod taps listed below and replace
			// the modifier with base keycode.
			if (get_mods() == get_modtap_bit(LSFT_T(KC_A))) {
				re_process(LSFT_T(KC_A);
			} else if (get_mods() == get_modtap_bit(LALT_T(KC_S))) {
				re_process(LALT_T(KC_S));
			} else if (get_mods() == get_modtap_bit(LCTL_T(KC_D))) {
				re_process(LCTL_T(KC_D));
			} else if (get_mods() == get_modtap_bit(LGUI_T(KC_F))) {
				re_process(LGUI_T(KC_F));
			}
		}
		if (KEY_POSITION >= KB_MIDPOINT) {
			// Process key press on the right half. If right mod is active, assume it
			// was rolled from the previous right mod tap listed below and replace
			// the modifier with base keycode.
			if (get_mods() == get_modtap_bit(RSFT_T(KC_QUOT))) {
				re_process(RSFT_T(KC_QUOT));
			} else if (get_mods() == get_modtap_bit(RALT_T(KC_L))) {
				re_process(RALT_T(KC_L));
			} else if (get_mods() == get_modtap_bit(RCTL_T(KC_K))) {
				re_process(RCTL_T(KC_K));
			} else if (get_mods() == get_modtap_bit(RGUI_T(KC_J))) {
				re_process(RGUI_T(KC_J));
			}
		}
	}
}
