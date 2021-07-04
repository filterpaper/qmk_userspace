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

/* Simplified combo helper file from Jane Bernhardt (http://gboards.ca/)
   that generates combo source codes at preprocessor stage with
   "combos.def" data file. See: http://combos.gboards.ca/docs/combos/

   Merge early_combo patch to work with mod tap:
   git pull https://github.com/sevanteri/qmk_firmware.git early_combo
*/

// Combo code macros
#define C_ENUM(name, val, ...) name,
#define C_DATA(name, val, ...) const uint16_t PROGMEM cmb_##name[] = {__VA_ARGS__, COMBO_END};
#define C_COMB(name, val, ...) [name] = COMBO(cmb_##name, val),
#define A_COMB(name, val, ...) [name] = COMBO_ACTION(cmb_##name),
#define A_SUBS(name, val, ...) case name: if (pressed) SEND_STRING(val); break;
#define BLANK(...)

// Enumerate name list
#undef COMB
#undef SUBS
#define COMB C_ENUM
#define SUBS C_ENUM
enum combos {
	#include "combos.def"
	COMBO_LENGTH
};
uint16_t COMBO_LEN = COMBO_LENGTH;

// Store combos in PROGMEM
#undef COMB
#undef SUBS
#define COMB C_DATA
#define SUBS C_DATA
#include "combos.def"

// Fill key array
#undef COMB
#undef SUBS
#define COMB C_COMB
#define SUBS A_COMB
combo_t key_combos[] = {
	#include "combos.def"
};

// Fill action function
#undef COMB
#undef SUBS
#define COMB BLANK
#define SUBS A_SUBS
void process_combo_event(uint16_t combo_index, bool pressed) {
	switch (combo_index) {
		#include "combos.def"
	}
}
