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

// Tap hold macro function for process_record_user()
// using layer tap LT() tapping term delay as a
// hold shortcut, by @sigprof
#define TAP_HOLD(_tap_, _hold_) \
	if (record->tap.count) { \
		if (record->event.pressed) { register_code(_tap_); } \
		else { unregister_code(_tap_); } \
	} else if (record->event.pressed) { _hold_; } \
	return false;


#ifdef SPLIT_MODS_ENABLE
uint_fast32_t tap_timer = 0; // Timer for OLED animation
#endif


#ifdef CAPSWORD_ENABLE
// Deactivate caps lock following a word
static void process_caps_word(uint_fast16_t keycode, keyrecord_t const *record) {
	// Get base key code of mod or layer tap with bitmask
	if (((QK_MOD_TAP <= keycode && keycode <= QK_MOD_TAP_MAX) ||
		(QK_LAYER_TAP <= keycode && keycode <= QK_LAYER_TAP_MAX)) &&
		(record->tap.count)) { keycode = keycode & 0xFF; }
	// Toggle caps lock with the following key codes
	switch (keycode) {
		case KC_TAB:
		case KC_ESC:
		case KC_SPC:
		case KC_ENT:
		case KC_DOT:
		case KC_COMM:
		case KC_GESC:
			if (record->event.pressed) { tap_code(KC_CAPS); }
	}
}
#endif


bool process_record_user(uint16_t const keycode, keyrecord_t *record) {
#ifdef SPLIT_MODS_ENABLE // Reset typing timer for OLED animation
	if (record->event.pressed) { tap_timer = timer_read32(); }
#endif
#ifdef CAPSWORD_ENABLE // Monitor key codes to toggle caps lock
	if (host_keyboard_led_state().caps_lock) { process_caps_word(keycode, record); }
#endif
	switch (keycode) {
		// VIM commands
		case Q_TH: TAP_HOLD(KC_Q, SEND_STRING(":q!"));
		case W_TH: TAP_HOLD(KC_W, SEND_STRING(":wq"));
		// New tab and window
		case T_TH: TAP_HOLD(KC_T, tap_code16(G(KC_T)));
		case N_TH: TAP_HOLD(KC_N, tap_code16(G(KC_N)));
		// Right hand cut copy paste
		case DOT_TH: TAP_HOLD(KC_DOT, tap_code16(G(KC_X)));
		case COMM_TH: TAP_HOLD(KC_COMM, tap_code16(G(KC_C)));
		case M_TH: TAP_HOLD(KC_M, tap_code16(G(KC_V)));
	}
	return true; // Continue with unmatched keycodes
}


#ifdef TAPPING_TERM_PER_KEY
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
	return ((keycode & 0xFF00) == ALT_T_MASK) ? TAPPING_TERM + 100 : TAPPING_TERM;
}
#endif


#ifdef PERMISSIVE_HOLD_PER_KEY
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
	return ((keycode & 0xFF00) == SFT_T_MASK) ? true : false;
}
#endif
