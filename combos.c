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

#define STENO(_k_) if (pressed) SEND_STRING(_k_); break;
#define LT_BSPC LT(NAVI,KC_BSPC)

enum combo_events {
	BSPCU_YOU,
	BSPCU_THE,
	COMBO_LENGTH
};
uint16_t COMBO_LEN = COMBO_LENGTH;

const uint16_t PROGMEM BSPC_U_COMBO[]   = {LT_BSPC, KC_U, COMBO_END};
const uint16_t PROGMEM BSPC_T_COMBO[]   = {LT_BSPC, KC_T, COMBO_END};

combo_t key_combos[] = {
	[BSPCU_YOU] = COMBO_ACTION(BSPC_U_COMBO),
	[BSPCU_THE] = COMBO_ACTION(BSPC_T_COMBO),
};

void process_combo_event(uint16_t combo_index, bool pressed) {
	switch(combo_index) {
		case BSPCU_YOU: STENO("you ");
		case BSPCU_THE: STENO("the ");
	}
}
