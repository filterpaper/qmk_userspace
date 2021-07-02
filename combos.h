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

// Modified combo helper file from Germ (http://combos.gboards.ca/)

// HRM mod taps
#define HM_A	SFT_T(KC_A)
#define HM_S	ALT_T(KC_S)
#define HM_D	CTL_T(KC_D)
#define HM_F	GUI_T(KC_F)
#define HM_J	GUI_T(KC_J)
#define HM_K	CTL_T(KC_K)
#define HM_L	ALT_T(KC_L)

// Combo helper macros
#define K_ENUM(name, key, ...) name,
#define K_DATA(name, key, ...) const uint16_t PROGMEM cmb_##name[] = {__VA_ARGS__, COMBO_END};
#define K_COMB(name, key, ...) [name] = COMBO(cmb_##name, key),

#define A_ENUM(name, string, ...) name,
#define A_DATA(name, string, ...) const uint16_t PROGMEM cmb_##name[] = {__VA_ARGS__, COMBO_END};
#define A_COMB(name, string, ...) [name] = COMBO_ACTION(cmb_##name),
#define A_ACTI(name, string, ...)         \
	case name:                            \
		if (pressed) SEND_STRING(string); \
		break;
#define A_TOGG(name, layer, ...)          \
	case name:                            \
		if (pressed) layer_invert(layer); \
		break;

#define BLANK(...)

// Create enumeration list
#undef COMB
#undef SUBS
#undef TOGG
#define COMB K_ENUM
#define SUBS A_ENUM
#define TOGG A_ENUM
enum combos {
	#include "combos.def"
	COMBO_LENGTH
};
uint16_t COMBO_LEN = COMBO_LENGTH;

// Add combos into PROGMEM
#undef COMB
#undef SUBS
#undef TOGG
#define COMB K_DATA
#define SUBS A_DATA
#define TOGG A_DATA
#include "combos.def"

// Fill key combo array
#undef COMB
#undef SUBS
#undef TOGG
#define COMB K_COMB
#define SUBS A_COMB
#define TOGG A_COMB
combo_t key_combos[] = {
	#include "combos.def"
};

// Fill QMK combo function
#undef COMB
#undef SUBS
#undef TOGG
#define COMB BLANK
#define SUBS A_ACTI
#define TOGG A_TOGG
void process_combo_event(uint16_t combo_index, bool pressed) {
	switch (combo_index) {
		#include "combos.def"
	}
	// Allow user overrides per keymap
	#if __has_include("inject.h")
		#include "inject.h"
	#endif
}
