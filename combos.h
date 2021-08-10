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

/* Adapted from Jane Bernhardt's Combos on Steroids (http://combos.gboards.ca/)

   This file will build QMK's combo source with preprocessor substitution
   using "combos.def" macros in the following format:

   COMB(name, keycode_shortcut, combo_sequence...)
   SUBS(name, "string to send", combo_sequence...)
   ACTN(name, actionfunction(), combo_sequence...)

   COMB are simple keycode shortcuts. This example will increase volume
   with Y+U: COMB(KC_VOLU, KC_VOLU, KC_Y, KC_U)
   Keycodes can be used as names, they will be prefixed to avoid
   conflict.

   SUBS are string substitution combos. This example will send the string
   with W+H: SUBS(which, "which ", KC_W, KC_H)

   ACTN are function triggers. This example will call the layer function
   with Z+X: ACTN(layer2, layer_on(2), KC_Z, KC_X)
   Multiple functions are supported with ; delimiter.

   Usage: add '#include "combos.h"' to keymap.c or user source.
 */

// Combo code building macros
#define C_ENUM(name, val, ...) combo_##name,
#define C_DATA(name, val, ...) uint16_t const cmb_##name[] PROGMEM = {__VA_ARGS__, COMBO_END};
#define C_TYPE(name, val, ...) [combo_##name] = COMBO(cmb_##name, val),
#define A_TYPE(name, val, ...) [combo_##name] = COMBO_ACTION(cmb_##name),
#define P_SSTR(name, val, ...) case combo_##name: if (pressed) SEND_STRING(val); break;
#define P_ACTN(name, val, ...) case combo_##name: if (pressed) { val; } break;
#define UNUSED(...)

// Enumerate combo list with prefixed names
#undef COMB
#undef SUBS
#undef ACTN
#define COMB C_ENUM
#define SUBS C_ENUM
#define ACTN C_ENUM
enum combos {
	#include "combos.def"
	COMBO_LENGTH
};
uint16_t COMBO_LEN = COMBO_LENGTH;

// Create combo name array in PROGMEM with combo key sequences
#undef COMB
#undef SUBS
#undef ACTN
#define COMB C_DATA
#define SUBS C_DATA
#define ACTN C_DATA
#include "combos.def"

// Fill array with combo type names and shortcuts
#undef COMB
#undef SUBS
#undef ACTN
#define COMB C_TYPE
#define SUBS A_TYPE
#define ACTN A_TYPE
combo_t key_combos[] = {
	#include "combos.def"
};

// Fill combo event function with send string or action calls
#undef COMB
#undef SUBS
#undef ACTN
#define COMB UNUSED
#define SUBS P_SSTR
#define ACTN P_ACTN
void process_combo_event(uint16_t combo_index, bool pressed) {
	switch (combo_index) {
		#include "combos.def"
	}
}
