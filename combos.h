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
   using COMBOS_DEF file macros in the following format:

   COMB(name, keycode_shortcut, combo_sequence...)
   SUBS(name, "string to send", combo_sequence...)
   ACTN(name, actionfunction(), combo_sequence...)

   COMB are simple keycode shortcuts. This combination will increase volume
   with Y+U: COMB(KC_VOLU, KC_VOLU, KC_Y, KC_U)
   Keycodes can be used as names, they are prefixed to avoid conflict.

   SUBS are string substitution combos. It can be used to send strings like
   this example with W+H: SUBS(which, "which ", KC_W, KC_H)

   ACTN are function triggers that can be used to for QMK internal functions
   like layer trigger with Z+X: ACTN(layer2, layer_on(2), KC_Z, KC_X)
   Multiple functions are supported with ; delimiter.

   Usage: add '#include "combos.h"' to keymap.c / user source, or
   'SRC += combos.c' to rules.mk
 */

#define COMBOS_DEF "combos.def"

// Combo code building macros
#define C_ENUM(name, val, ...) cmb_##name,
#define C_DATA(name, val, ...) uint16_t const name##_combo[] PROGMEM = {__VA_ARGS__, COMBO_END};
#define C_TYPE(name, val, ...) [cmb_##name] = COMBO(name##_combo, val),
#define A_TYPE(name, val, ...) [cmb_##name] = COMBO_ACTION(name##_combo),
#define P_SSTR(name, val, ...) case cmb_##name: if (pressed) SEND_STRING(val); break;
#define P_ACTN(name, val, ...) case cmb_##name: if (pressed) { val; } break;
#define UNUSED(...)

// Enumerate combo list with prefixed names
#undef COMB
#undef SUBS
#undef ACTN
#define COMB C_ENUM
#define SUBS C_ENUM
#define ACTN C_ENUM
enum combos {
	#include COMBOS_DEF
	COMBO_LENGTH
};
uint16_t COMBO_LEN = COMBO_LENGTH;

// Create combo name array in PROGMEM with key sequences
#undef COMB
#undef SUBS
#undef ACTN
#define COMB C_DATA
#define SUBS C_DATA
#define ACTN C_DATA
#include COMBOS_DEF

// Fill array with combo type and shortcuts
#undef COMB
#undef SUBS
#undef ACTN
#define COMB C_TYPE
#define SUBS A_TYPE
#define ACTN A_TYPE
combo_t key_combos[] = {
	#include COMBOS_DEF
};

// Fill combo event function with send string or function calls
#undef COMB
#undef SUBS
#undef ACTN
#define COMB UNUSED
#define SUBS P_SSTR
#define ACTN P_ACTN
void process_combo_event(uint16_t combo_index, bool pressed) {
	switch (combo_index) {
		#include COMBOS_DEF
	}
}
