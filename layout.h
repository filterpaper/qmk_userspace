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

// Tap hold macros
#define Q_TH	LT(0,KC_Q)
#define W_TH	LT(0,KC_W)
#define SLSH_TH	LT(0,KC_SLSH)
#define DOT_TH	LT(0,KC_DOT)
#define COMM_TH	LT(0,KC_COMM)
#define M_TH	LT(0,KC_M)
#define V_TH	LT(0,KC_V)

// OS X shortcuts
#define Z_CUT G(KC_X)
#define Z_CPY G(KC_C)
#define Z_PST G(KC_V)
#define Z_UND G(KC_Z)
#define Z_PASTE	S(G(KC_V))
#define Z_SLEEP	A(G(KC_PWR))
#define Z_SHUT	C(A(G(KC_PWR)))
#define Z_LOCK	C(G(KC_Q))

// One shot mods
#define OSM_SFT	OSM(MOD_LSFT)
#define OSM_CTL	OSM(MOD_LCTL)
#define OSM_ALT	OSM(MOD_LALT)
#define OSM_GUI	OSM(MOD_LGUI)
#define OSS_LT2	LT(2,KC_NO) // OSM shift layer tap
#define OSS_LT() if (record->tap.count) { if (record->event.pressed) add_oneshot_mods(MOD_LSFT); return false }

#define BSE 0
#define CMK 1
#define NUM 2
#define NAV 3
#define FNC 4



// Corne 3x6_3
#define CORNE_wrapper(...)  LAYOUT_split_3x6_3(__VA_ARGS__)
// Home row mod wrapper
#define HRM(k) HRM_SACG(k)
#define HRM_SACG( \
	k01, k02, k03, k04, k05, k06, k07, k08, k09, k10, k11, k12, \
	k13, k14, k15, k16, k17, k18, k19, k20, k21, k22, k23, k24, \
	k25, k26, k27, k28, k29, k30, k31, k32, k33, k34, k35, k36, \
	k37, k38, k39, k40, k41, k42 \
) \
	k01, k02, k03, k04, k05, k06, k07, k08, k09, k10, k11, k12, \
	k13,  SFT_T(k14), ALT_T(k15), CTL_T(k16), GUI_T(k17),  k18, \
	k19,  GUI_T(k20), CTL_T(k21), ALT_T(k22), SFT_T(k23),  k24, \
	k25, k26, k27, k28, k29, k30, k31, k32, k33, k34, k35, k36, \
	k37, k38, k39, k40, k41, k42

#define _BASE \
	KC_TAB,  Q_TH,    W_TH,    KC_E,    KC_R,    KC_T,        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC, \
	KC_GESC, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,        KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
	OSM_SFT, KC_Z,    KC_X,    KC_C,    V_TH,    KC_B,        KC_N,    M_TH,    COMM_TH, DOT_TH,  SLSH_TH, KC_ENT,  \
	        RSA_T(KC_ESC), LT(NAV,KC_TAB), RSG_T(KC_ENT),     RSFT_T(KC_SPC), LT(NUM,KC_BSPC), RAG_T(KC_DEL)
#define _COLE \
	_______, _______, _______, KC_F,    KC_P,    KC_G,        KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, _______, \
	_______, KC_A,    KC_R,    KC_S,    KC_T,    KC_D,        _______, KC_N,    KC_E,    KC_I,    KC_O,    _______, \
	_______, _______, _______, _______, _______, _______,     KC_K,    _______, _______, _______, _______, _______, \
	                           _______, _______, _______,     _______, _______, _______
#define _NUMB \
	_______, _______, KC_1,    KC_2,    KC_3,    _______,     KC_VOLU, KC_LPRN, KC_RPRN, _______, KC_DQUO, _______, \
	_______, _______, KC_4,    KC_5,    KC_6,    _______,     KC_VOLD, KC_LCBR, KC_RCBR, _______, KC_QUOT, _______, \
	_______, _______, KC_7,    KC_8,    KC_9,    KC_0,        KC_MUTE, KC_LBRC, KC_RBRC, _______, _______, _______, \
	                           _______, MO(FNC), _______,     _______, _______, _______
#define _NAVI \
	_______, _______, KC_CIRC, KC_AMPR, KC_ASTR, _______,     KC_PGUP, KC_MINS, KC_EQL,  KC_BSLS, KC_TILD, _______, \
	_______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,     KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_GRV,  _______, \
	_______, KC_CAPS, _______, _______, _______, _______,     KC_PGDN, KC_UNDS, KC_PLUS, KC_PIPE, _______, _______, \
	                           _______, _______, _______,     _______, MO(FNC), _______
#define _FUNC \
	_______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,       KC_HOME, KC_WH_U, KC_WH_D, KC_INS,  _______, _______, \
	_______, KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,      KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, _______, _______, \
	_______, Z_SLEEP, _______, _______, KC_F11,  KC_F12,      KC_END,  KC_BTN2, KC_BTN1, KC_DEL,  Z_SHUT,  _______, \
	                             RESET, _______, _______,     _______, _______, TG(CMK)



// 3w6 3x5_3
#define FIVECOL_wrapper(...) LAYOUT(__VA_ARGS__)
// Corne to 3w6 layout conversion
#define C_3W6(k) SPLIT_3x6_3_TO_3x5_3(k)
#define SPLIT_3x6_3_TO_3x5_3( \
	k01, k02, k03, k04, k05, k06, k07, k08, k09, k10, k11, k12, \
	k13, k14, k15, k16, k17, k18, k19, k20, k21, k22, k23, k24, \
	k25, k26, k27, k28, k29, k30, k31, k32, k33, k34, k35, k36, \
	               k37, k38, k39, k40, k41, k42 \
) \
	     k02, k03, k04, k05, k06, k07, k08, k09, k10, k11, \
	     k14, k15, k16, k17, k18, k19, k20, k21, k22, k23, \
	     k26, k27, k28, k29, k30, k31, k32, k33, k34, k35, \
	               k37, k38, k39, k40, k41, k42



// Ortho 4x12
#define ORTHO_wrapper(...) LAYOUT_ortho_4x12(__VA_ARGS__)
// Corne to Ortho 3x12 layout conversion
#define C_O(k) SPLIT_3x6_3_TO_3x12(k)
#define SPLIT_3x6_3_TO_3x12( \
	k01, k02, k03, k04, k05, k06, k07, k08, k09, k10, k11, k12, \
	k13, k14, k15, k16, k17, k18, k19, k20, k21, k22, k23, k24, \
	k25, k26, k27, k28, k29, k30, k31, k32, k33, k34, k35, k36, \
	...) \
	k01, k02, k03, k04, k05, k06, k07, k08, k09, k10, k11, k12, \
	k13, k14, k15, k16, k17, k18, k19, k20, k21, k22, k23, k24, \
	k25, k26, k27, k28, k29, k30, k31, k32, k33, k34, k35, k36

// Bottom row keys
#define O_BASE 	KC_DEL, KC_VOLD, KC_VOLU, RSA_T(KC_TAB), LT(NAV,KC_BSPC), RSG_T(KC_ENT), \
				RSFT_T(KC_SPC), LT(NUM,KC_SPC), RAG_T(KC_LEFT), KC_DOWN, KC_UP, KC_RGHT
#define O_BLNK _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
#define O_NUMB _______, _______, _______, _______, MO(FNC), _______, _______, _______, _______, _______, _______, _______
#define O_NAVI _______, _______, _______, _______, _______, _______, _______, MO(FNC), _______, _______, _______, _______
#define O_FUNC _______, _______, _______, RESET,   _______, _______, _______, _______, TG(CMK), _______, _______, _______



// Boardsource The Mark 65
#define MARK65_wrapper(...) LAYOUT_all(__VA_ARGS__)
// Home row mod wrapper
#define HRM65(k) HRM65_SACG(k)
#define HRM65_SACG( \
	k000, k001, k002, k003, k004, k005, k006, k007, k008, k009, k010, k011, k012, k013, k014, k015, \
	k100,       k102, k103, k104, k105, k106, k107, k108, k109, k110, k111, k112, k113, k114, k115, \
	k200,       k202, k203, k204, k205, k206, k207, k208, k209, k210, k211, k212, k213,       k215, \
	k300, k301, k302, k303, k304, k305, k306, k307, k308, k309, k310, k311,       k313, k314,       \
	k400, k401,       k403, k404,       k406,       k408,       k410, k411, k412, k413, k414, k415  \
) \
	k000, k001, k002, k003, k004, k005, k006, k007, k008, k009, k010, k011, k012, k013, k014, k015, \
	k100,       k102, k103, k104, k105, k106, k107, k108, k109, k110, k111, k112, k113, k114, k115, \
	k200,  SFT_T(k202), ALT_T(k203), CTL_T(k204), GUI_T(k205),  k206,                               \
	k207,  GUI_T(k208), CTL_T(k209), ALT_T(k210), SFT_T(k211),  k212, k213, k215,                   \
	k300, k301, k302, k303, k304, k305, k306, k307, k308, k309, k310, k311,       k313, k314,       \
	k400, k401,       k403, k404,       k406,       k408,       k410, k411, k412, k413, k414, k415

#define MARK_DEFAULT \
	KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSLS, KC_GRV,  KC_VOLU, \
	KC_TAB,           Q_TH,    W_TH,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSPC, KC_VOLD, \
	KC_CAPS,          KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,           KC_MUTE, \
	OSM_SFT, _______, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    M_TH,    COMM_TH, DOT_TH,  SLSH_TH, KC_RSFT,          KC_UP,            \
	OSM_ALT, OSM_CTL,          OSM_GUI, LT(2,KC_BSPC),    KC_ENT,           RSFT_T(KC_SPC),   KC_RCTL, KC_RALT, KC_RALT, KC_LEFT, KC_DOWN, KC_RGHT
#define MARK_COLEMAK \
	_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
	_______,          _______, _______, KC_F,    KC_P,    KC_G,    KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, _______, _______, _______, _______, \
	_______,          KC_A,    KC_R,    KC_S,    KC_T,    KC_D,    _______, KC_N,    KC_E,    KC_I,    KC_O,    _______, _______,          _______, \
	_______, _______, _______, _______, _______, _______, _______, KC_K,    _______, _______, _______, _______, _______,          _______,          \
	_______, _______,          _______, _______,          _______,          _______,          _______, _______, _______, _______, _______, _______
#define MARK_LAYER2 \
	_______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, KC_HOME, KC_PGUP, \
	_______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_DEL,  KC_PGDN, \
	_______,          _______, _______, KC_BTN2, KC_BTN1, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, _______, _______,          KC_END,  \
	_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          KC_MS_U,          \
	_______, _______,          _______, _______,          _______,          MO(3),            _______, _______, _______, KC_MS_L, KC_MS_D, KC_MS_R
#define MARK_LAYER3 \
	RESET,   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, TG(1),   _______, \
	_______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
	_______,          _______, _______, _______, _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  _______, _______, _______,          _______, \
	_______, _______, Z_SLEEP, _______, _______, Z_PASTE, _______, _______, _______, _______, _______, Z_LOCK,  _______,          _______,          \
	_______, _______,          _______, _______,          _______,          _______,          _______, _______, _______, _______, _______, _______
