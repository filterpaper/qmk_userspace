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

	if ((QK_MOD_TAP <= keycode && keycode <= QK_MOD_TAP_MAX && record->tap.count) || IS_ANY(keycode)) {
		if (KEY_POSITION < KB_MIDPOINT) {
			// Process key press on the left half. If left mod is active, assume it
			// was rolled from the previous left mod taps listed below and replace
			// the modifier with base keycode.
			if (get_mods() & get_modtap_bit(HM_A)) {
				unregister_mods(get_modtap_bit(HM_A));
				tap_code((uint8_t)HM_A);
			} else if (get_mods() & get_modtap_bit(HM_S)) {
				unregister_mods(get_modtap_bit(HM_S));
				tap_code((uint8_t)HM_S);
			} else if (get_mods() & get_modtap_bit(HM_D)) {
				unregister_mods(get_modtap_bit(HM_D));
				tap_code((uint8_t)HM_D);
			} else if (get_mods() & get_modtap_bit(HM_F)) {
				unregister_mods(get_modtap_bit(HM_F));
				tap_code((uint8_t)HM_F);
			}
		}
		if (KEY_POSITION >= KB_MIDPOINT) {
			// Process key press on the right half. If right mod is active, assume it
			// was rolled from the previous right mod tap listed below and replace
			// the modifier with base keycode.
			if (get_mods() & get_modtap_bit(HM_QUOT)) {
				unregister_mods(get_modtap_bit(HM_QUOT));
				tap_code((uint8_t)HM_QUOT);
			} else if (get_mods() & get_modtap_bit(HM_L)) {
				unregister_mods(get_modtap_bit(HM_L));
				tap_code((uint8_t)HM_L);
			} else if (get_mods() & get_modtap_bit(HM_K)) {
				unregister_mods(get_modtap_bit(HM_K));
				tap_code((uint8_t)HM_K);
			} else if (get_mods() & get_modtap_bit(HM_J)) {
				unregister_mods(get_modtap_bit(HM_J));
				tap_code((uint8_t)HM_J);
			}
		}
	}
}
