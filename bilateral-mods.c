// Copyright 2022 @filterpaper
// SPDX-License-Identifier: Apache-2.0
/* Simple bilateral home row mod implementation that replaces activated
   modifier with the base keycode if rolled on the same half.

   Usage:
   Include this source file in 'rules.mk' with:
   SRC += bilateral-mods.c

   Call 'process_bilateral_mod' in 'process_record_user':
   if (record->event.pressed) {
     extern void process_bilateral_mod(uint16_t keycode, keyrecord_t *record);
     process_bilateral_mod(keycode, record);
   }

 */

#include QMK_KEYBOARD_H

void process_bilateral_mod(uint16_t keycode, keyrecord_t *record) {

	void process_unilateral(uint16_t kc) {
		// Check for R mod-tap bit and shift 4-bits left for mod_bits
		uint8_t mod_tap_bit = (kc & 0x1000) ? ((kc>>8)&0xf)<<4 : (kc>>8)&0xf;
		if (keycode != kc && get_mods() & mod_tap_bit) {
			unregister_mods(mod_tap_bit);
			tap_code(kc);
		}
	}

	if ((QK_MOD_TAP <= keycode && keycode <= QK_MOD_TAP_MAX && record->tap.count) || IS_ANY(keycode)) {
#ifdef SPLIT_KEYBOARD
		if (record->event.key.row < MATRIX_ROWS/2) {
#else
		if (record->event.key.col < MATRIX_COLS/2) {
#endif
			// List of left mod taps to exclude unilateral modifier
			process_unilateral(HM_A);
			process_unilateral(HM_S);
			process_unilateral(HM_D);
			process_unilateral(HM_F);
		}
#ifdef SPLIT_KEYBOARD
		if (record->event.key.row >= MATRIX_ROWS/2) {
#else
		if (record->event.key.col >= MATRIX_COLS/2) {
#endif
			// List of right mod taps to exclude unilateral modifier
			process_unilateral(HM_J);
			process_unilateral(HM_K);
			process_unilateral(HM_L);
			process_unilateral(HM_QUOT);
		}

	}
}
