// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

/* Adapted from Jane Bernhardt's Combos on Steroids (http://combos.gboards.ca/)
   This file will build QMK's combo source with preprocessor substitution
   using COMBOS_DEF file macros in the following format:
   COMB(name, keycode_shortcut, combo_sequence...)
   SUBS(name, "string to send", combo_sequence...)
   ACTN(name, function_calls(), combo_sequence...)

   Use COMB for simple keycode shortcuts with two or more combo keys to activate
   a keycode. E.g. volume up with Y+U: COMB(vol_up, KC_VOLU, KC_Y, KC_U).

   SUBS is a string substitution macro that can be used to send strings.
   E.g. SUBS(which, "which ", KC_W, KC_H).

   Use ACTN for internal callback functions that can be semicolon separated.
   E.g. ACTN(rgb_tog, rgb_matrix_toggle(), KC_Z, KC_X)

   Usage: Save macros in 'combos.inc' and add the following to rules.mk:
   COMBO_ENABLE = yes
   SRC += combos.c
 */

#include QMK_KEYBOARD_H

#define COMBOS_DEF "combos.inc"

// Combo code building macros
#define C_ENUM(name, val, ...) name,
#define C_DATA(name, val, ...) uint16_t const name##_combo[] PROGMEM = {__VA_ARGS__, COMBO_END};
#define C_TYPE(name, val, ...) [name] = COMBO(name##_combo, val),
#define A_TYPE(name, val, ...) [name] = COMBO_ACTION(name##_combo),
#define P_SSTR(name, val, ...) case name: if (pressed) { SEND_STRING(val); } break;
#define P_ACTN(name, val, ...) case name: if (pressed) { val; } break;
#define UNUSED(...)

// Enumerate combo list with names
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

// Fill combo event with send string or function calls
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