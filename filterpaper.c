/* Copyright (C) 2021 @filterpaper
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "filterpaper.h"

#ifdef COMBO_ENABLE
#include "combos.h"
#endif

// Macro hack using LT(0,kc) mod tap function, returns kc
// on tap, sends custom tap code after TAPPING_TERM
#define TAP_HOLD(_hold_) \
	if (record->tap.count) return true; \
	else if (record->event.pressed) tap_code16(_hold_); \
	return false


#ifdef OLED_DRIVER_ENABLE
uint32_t tap_timer = 0; // Timer for OLED animation
#endif


#ifdef CAPSWORD_ENABLE // Deactivate caps lock after selective key codes
static void process_caps_word(uint16_t keycode, keyrecord_t const *record) {
	// Get base key code from mod and layer taps
	if (((QK_MOD_TAP <= keycode && keycode <= QK_MOD_TAP_MAX) ||
	(QK_LAYER_TAP <= keycode && keycode <= QK_LAYER_TAP_MAX)) &&
	(record->tap.count)) {
		keycode &= 0xFF;
	}
	// Toggle caps lock with the following key codes
	switch (keycode) {
		case KC_TAB:
		case KC_ESC:
		case KC_SPC:
		case KC_ENT:
		case KC_DOT:
		case KC_EQL:
		case KC_COMM:
		case KC_GESC:
			if (record->event.pressed) { tap_code(KC_CAPS); }
	}
}
#endif


bool process_record_user(uint16_t const keycode, keyrecord_t *record) {
#ifdef OLED_DRIVER_ENABLE // Reset typing timer for OLED animation
	if (record->event.pressed) { tap_timer = timer_read32(); }
#endif
#ifdef CAPSWORD_ENABLE // Monitor key codes to toggle caps lock
	if (host_keyboard_led_state().caps_lock) { process_caps_word(keycode, record); }
#endif

	switch (keycode) {
		// Right side undo cut copy paste
		case SLSH_TH: TAP_HOLD(Z_UND);
		case DOT_TH:  TAP_HOLD(Z_CUT);
		case COMM_TH: TAP_HOLD(Z_CPY);
		case M_TH:    TAP_HOLD(Z_PST);
		// Unformatted paste
		case V_TH: TAP_HOLD(Z_PASTE);

#ifdef LT_ONESHOT_ENABLE
		case LT(SYM,KC_NO):
		case LT(NUM,KC_NO):
			if (record->tap.count && record->event.pressed) {
				add_oneshot_mods(MOD_LSFT);
				return false;
			}
#endif

	}

	return true; // Continue with unmatched keycodes
}


#ifdef TAPPING_TERM_PER_KEY // Reduce for thumb keys
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
	return ((keycode & 0xFF00) == RMT_BITS) ? TAPPING_TERM - 100 : TAPPING_TERM;
}
#endif


#ifdef PERMISSIVE_HOLD_PER_KEY // Disable for tap hold macros and home row mods
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
	return ((keycode & 0xFF00) == LT0_BITS || (keycode & 0xF000) == LMT_BITS) ? false : true;
}
#endif


#ifdef TAPPING_FORCE_HOLD_PER_KEY
bool get_tapping_force_hold(uint16_t keycode, keyrecord_t *record) {
	return (keycode == RSFT_T(KC_SPC)) ? true : false;
}
#endif
