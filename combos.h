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

/* Modification of Jane Bernhardt's Combos on Steroids (http://combos.gboards.ca/)

   This helper file will build all four parts of QMK's combo source codes
   from user's "combos.def" file with shortcuts in the following format:

   COMB(name, keycode_shortcut, combo_sequence...)
   SUBS(name, "string to send", combo_sequence...)
   ACTN(name, actionfunction(), combo_sequence...)

   Merge early_combo patch to work with mod tap:
   git pull https://github.com/sevanteri/qmk_firmware.git early_combo
 */

// Combo code macros
#define C_ENUM(name, val, ...) combo_ ## name,
#define C_DATA(name, val, ...) uint16_t const cmb_ ## name[] PROGMEM = {__VA_ARGS__, COMBO_END};
#define C_COMB(name, val, ...) [combo_ ## name] = COMBO(cmb_ ## name, val),
#define A_COMB(name, val, ...) [combo_ ## name] = COMBO_ACTION(cmb_ ## name),
#define P_SUBS(name, val, ...) case combo_ ## name: if (pressed) SEND_STRING(val); break;
#define P_ACTN(name, val, ...) case combo_ ## name: if (pressed) val; break;
#define UNUSED(...)

// Enumerate name list
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

// Store combos in PROGMEM
#undef COMB
#undef SUBS
#undef ACTN
#define COMB C_DATA
#define SUBS C_DATA
#define ACTN C_DATA
#include "combos.def"

// Fill key array
#undef COMB
#undef SUBS
#undef ACTN
#define COMB C_COMB
#define SUBS A_COMB
#define ACTN A_COMB
combo_t key_combos[] = {
	#include "combos.def"
};

// Fill event function
#undef COMB
#undef SUBS
#undef ACTN
#define COMB UNUSED
#define SUBS P_SUBS
#define ACTN P_ACTN
void process_combo_event(uint16_t combo_index, bool pressed) {
	switch (combo_index) {
		#include "combos.def"
	}
}
