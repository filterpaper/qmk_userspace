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

// HRM mod taps
#define HM_A	SFT_T(KC_A)
#define HM_S	ALT_T(KC_S)
#define HM_D	CTL_T(KC_D)
#define HM_F	GUI_T(KC_F)
#define HM_J	GUI_T(KC_J)
#define HM_K	CTL_T(KC_K)
#define HM_L	ALT_T(KC_L)
// Thumb mod taps
#define LT_TAB RAG_T(KC_TAB)
#define LT_ESC RSA_T(KC_ESC)
#define LT_BSPC LT(NAVI,KC_BSPC)

enum combo_events {
	AN_AND,
	BC_BECAUSE,
	HV_HAVE,
	IO_TION,
	TH_THE,
	WA_WHAT,
	WC_WHICH,
	WI_WITH,
	YR_YOUR,
	YU_YOU,
	COMBO_LENGTH
};
uint16_t COMBO_LEN = COMBO_LENGTH;
#undef COMBO_COUNT
#define COMBO_COUNT COMBO_LENGTH

const uint16_t PROGMEM AN_COMBO[]   = {HM_A, KC_N, COMBO_END};
const uint16_t PROGMEM BC_COMBO[]   = {KC_B, KC_C, COMBO_END};
const uint16_t PROGMEM HV_COMBO[]   = {KC_H, KC_V, COMBO_END};
const uint16_t PROGMEM IO_COMBO[]   = {KC_I, KC_O, COMBO_END};
const uint16_t PROGMEM TH_COMBO[]   = {KC_T, KC_H, COMBO_END};
const uint16_t PROGMEM WA_COMBO[]   = {W_TH, HM_A, COMBO_END};
const uint16_t PROGMEM WC_COMBO[]   = {W_TH, KC_C, COMBO_END};
const uint16_t PROGMEM WI_COMBO[]   = {W_TH, KC_I, COMBO_END};
const uint16_t PROGMEM YR_COMBO[]   = {Y_TH, KC_R, COMBO_END};
const uint16_t PROGMEM YU_COMBO[]   = {Y_TH, KC_U, COMBO_END};

combo_t key_combos[COMBO_COUNT] = {
	[AN_AND] = 		COMBO_ACTION(AN_COMBO),
	[BC_BECAUSE] = 	COMBO_ACTION(BC_COMBO),
	[HV_HAVE] = 	COMBO_ACTION(HV_COMBO),
	[IO_TION] = 	COMBO_ACTION(IO_COMBO),
	[TH_THE] = 		COMBO_ACTION(TH_COMBO),
	[WA_WHAT] = 	COMBO_ACTION(WA_COMBO),
	[WC_WHICH] = 	COMBO_ACTION(WC_COMBO),
	[WI_WITH] = 	COMBO_ACTION(WI_COMBO),
	[YR_YOUR] = 	COMBO_ACTION(YR_COMBO),
	[YU_YOU] = 		COMBO_ACTION(YU_COMBO),
};

void process_combo_event(uint16_t combo_index, bool pressed) {
	switch(combo_index) {
		case AN_AND: 	STENO("and ");
		case BC_BECAUSE: STENO("because ");
		case HV_HAVE: 	STENO("have ");
		case IO_TION: 	STENO("tion ");
		case TH_THE: 	STENO("the ");
		case WA_WHAT: 	STENO("what ");
		case WC_WHICH: 	STENO("which ");
		case WI_WITH: 	STENO("with ");
		case YR_YOUR: 	STENO("your ");
		case YU_YOU: 	STENO("you ");
	}
}
