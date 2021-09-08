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

// OS X shortcuts
#define Z_CUT	G(KC_X)
#define Z_CPY	G(KC_C)
#define Z_PST	G(KC_V)
#define Z_UND	G(KC_Z)
#define Z_PASTE	S(G(KC_V))
#define Z_LOCK	C(G(KC_Q))
#define Z_SLEEP	A(G(KC_PWR))
#define Z_SHUT	C(A(G(KC_PWR)))

// Tap hold macros
#define TH(k)	LT(0,k)
#define Q_TH	TH(KC_Q)
#define W_TH	TH(KC_W)
#define V_TH	TH(KC_V)
#define M_TH	TH(KC_M)
#define COMM_TH	TH(KC_COMM)
#define DOT_TH	TH(KC_DOT)
#define SLSH_TH	TH(KC_SLSH)

// Home row mod tap macros
#define HM_A	SFT_T(KC_A)
#define HM_S	ALT_T(KC_S)
#define HM_D	CTL_T(KC_D)
#define HM_F	GUI_T(KC_F)
#define HM_J	GUI_T(KC_J)
#define HM_K	CTL_T(KC_K)
#define HM_L	ALT_T(KC_L)
#define HM_SCLN	SFT_T(KC_SCLN)

// One shot mods
#define OSM_SFT OSM(MOD_LSFT)
#define OSM_CTL OSM(MOD_LCTL)
#define OSM_ALT OSM(MOD_LALT)
#define OSM_GUI OSM(MOD_LGUI)

// Layers
#define BSE 0
#define CMK 1
#define NUM 2
#define SYM 3
#define FNC 4



// Corne 3x6_3
#define CORNE_wrapper(...)  LAYOUT_split_3x6_3(__VA_ARGS__)
// Home row mod and tap hold wrapper
#define HRM(k) HRM_TAPHOLD(k)
#define HRM_TAPHOLD( \
	k01, k02, k03, k04, k05, k06, k07, k08, k09, k10, k11, k12, \
	k13, k14, k15, k16, k17, k18, k19, k20, k21, k22, k23, k24, \
	k25, k26, k27, k28, k29, k30, k31, k32, k33, k34, k35, k36, \
	k37, k38, k39, k40, k41, k42 \
) \
	k01, k02,        k03,        k04,        k05,        k06, k07, k08,        k09,        k10,        k11,        k12, \
	k13, SFT_T(k14), ALT_T(k15), CTL_T(k16), GUI_T(k17), k18, k19, GUI_T(k20), CTL_T(k21), ALT_T(k22), SFT_T(k23), k24, \
	k25, k26,        k27,        k28,        TH(k29),    k30, k31, TH(k32),    TH(k33),    TH(k34),    TH(k35),    k36, \
	k37, k38, k39, k40, k41, k42


#define _BASE \
	KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC, \
	KC_GESC, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,        KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
	OSM_SFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,        KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT,  \
	        RSA_T(KC_ESC), LT(SYM,KC_TAB), RSG_T(KC_ENT),     RSFT_T(KC_SPC), LT(NUM,KC_BSPC), RAG_T(KC_DEL)
/* ,-----------------------------------------.                ,-----------------------------------------.
 * | TAB  |  Q   |  W   |  E   |  R   |  T   |                |  Y   |  U   |  I   |  O   |  P   | BSPC |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * | GESC |  A   |  S   |  D   |  F   |  G   |                |  H   |  J   |  K   |  L   | ; :  | ' "  |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * |OS_SFT|  Z   |  X   |  C   |  V   |  B   |                |  N   |  M   | , <  | . >  | / ?  | ENT  |
 * `------+------+------+------+------+------+------.  ,------+------+------+------+------+------+------|
 *                             |RSA_T |_SYMB |RSG_T |  |RSFT_T|_NUMB |RAG_T |
 *                             | ESC  | TAB  | ENT  |  | SPC  | BSPC | DEL  |
 *                             `--------------------'  `--------------------'
 */
#define _COLE \
	_______, KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,        KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, _______, \
	_______, KC_A,    KC_R,    KC_S,    KC_T,    KC_D,        KC_H,    KC_N,    KC_E,    KC_I,    KC_O,    _______, \
	_______, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,        KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, _______, \
	                           _______, _______, _______,     _______, _______, _______
/* ,-----------------------------------------.                ,-----------------------------------------.
 * |      |  Q   |  W   |  F   |  P   |  G   |                |  J   |  L   |  U   |  Y   | ; :  |      |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * |      |  A   |  R   |  S   |  T   |  D   |                |  H   |  N   |  E   |  I   |  O   |      |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * |      |  Z   |  X   |  C   |  V   |  B   |                |  K   |  M   | , <  | . >  | / ?  |      |
 * `------+------+------+------+------+------+------.  ,------+------+------+------+------+------+------|
 *                             |      |      |      |  |      |      |      |
 *                             `--------------------'  `--------------------'
 */
#define _NUMB \
	_______, _______, KC_1,    KC_2,    KC_3,    KC_VOLU,     KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_DQUO, _______, \
	_______, _______, KC_4,    KC_5,    KC_6,    KC_VOLD,     KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_QUOT, _______, \
	_______, KC_CAPS, KC_7,    KC_8,    KC_9,    KC_0,        _______, _______, _______, _______, _______, _______, \
	                           _______, MO(FNC), KC_ENT,      KC_SPC,  _______, _______
/* ,-----------------------------------------.                ,-----------------------------------------.
 * |      |      |  1   |  2   |  3   | VOLU |                | HOME | PGDN | PGUP | END  |  "   |      |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * |      |      |  4   |  5   |  6   | VOLD |                | LEFT | DOWN |  UP  | RGHT |  '   |      |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * |      | CAPS |  7   |  8   |  9   |  0   |                |      |      |      |      |      |      |
 * `------+------+------+------+------+------+------.  ,------+------+------+------+------+------+------|
 *                             |      |_FUNC | ENT  |  | SPC  |      |      |
 *                             `--------------------'  `--------------------'
 */
#define _SYMB \
	_______, _______, KC_LBRC, KC_LCBR, KC_RCBR, _______,     KC_CIRC, KC_LPRN, KC_RPRN, KC_RBRC, KC_TILD, _______, \
	_______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,     KC_AMPR, KC_MINS, KC_EQL,  KC_BSLS, KC_GRV,  _______, \
	_______, _______, _______, _______, _______, _______,     KC_ASTR, KC_UNDS, KC_PLUS, KC_PIPE, _______, _______, \
	                           _______, _______, KC_ENT,      KC_SPC,  MO(FNC), _______
/* ,-----------------------------------------.                ,-----------------------------------------.
 * |      |      |  [   |  {   |  }   |      |                |  ^   |  (   |  )   |  ]   |  ~   |      |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * |      |  !   |  @   |  #   |  $   |  %   |                |  &   |  -   |  =   |  \   |  `   |      |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * |      |      |      |      |      |      |                |  *   |  _   |  +   |  |   |      |      |
 * `------+------+------+------+------+------+------.  ,------+------+------+------+------+------+------|
 *                             |      |      | ENT  |  | SPC  |_FUNC |      |
 *                             `--------------------'  `--------------------'
 */
#ifdef MOUSEKEY_ENABLE
#define _FUNC \
	_______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,       _______, KC_WH_U, KC_WH_D, _______, TG(CMK), _______, \
	_______, KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,      KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, KC_INS,  _______, \
	_______, Z_SLEEP, _______, _______, KC_F11,  KC_F12,      _______, KC_BTN2, KC_BTN1, _______, Z_SHUT,  _______, \
	                           RESET,   _______, _______,     _______, _______, _______
/* ,-----------------------------------------.                ,-----------------------------------------.
 * |      |  F1  |  F2  |  F3  |  F4  |  F5  |                |      | WH_U | WH_D |      |_COLE |      |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * |      |  F6  |  F7  |  F8  |  F9  | F10  |                | MS_L | MS_D | MS_U | MS_R | INS  |      |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * |      |SLEEP |      |      | F11  | F12  |                |      | BTN2 | BTN1 |      | SHUT |      |
 * `------+------+------+------+------+------+------.  ,------+------+------+------+------+------+------|
 *                             |RESET |      |      |  |      |      |      |
 *                             `--------------------'  `--------------------'
 */
#else
#define _FUNC \
	_______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,       _______, _______, _______, _______, TG(CMK), _______, \
	_______, KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,      _______, _______, _______, _______, KC_INS,  _______, \
	_______, Z_SLEEP, _______, _______, KC_F11,  KC_F12,      _______, _______, _______, _______, Z_SHUT,  _______, \
	                           RESET,   _______, _______,     _______, _______, _______
#endif



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
// Corne to Ortho 4x12 layout conversion
#define C_O(k) SPLIT_3x6_3_TO_4x12(k)
#define SPLIT_3x6_3_TO_4x12( \
	           k01, k02, k03, k04, k05, k06, k07, k08, k09, k10, k11, k12, \
	           k13, k14, k15, k16, k17, k18, k19, k20, k21, k22, k23, k24, \
	           k25, k26, k27, k28, k29, k30, k31, k32, k33, k34, k35, k36, \
	                          k37, k38, k39, k40, k41, k42 \
) \
	           k01, k02, k03, k04, k05, k06, k07, k08, k09, k10, k11, k12, \
	           k13, k14, k15, k16, k17, k18, k19, k20, k21, k22, k23, k24, \
	           k25, k26, k27, k28, k29, k30, k31, k32, k33, k34, k35, k36, \
	KC_DEL, KC_VOLD, KC_VOLU, k37, k38, k39, k40, k41, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT



// Boardsource The Mark 65
#define MARK65_wrapper(...) LAYOUT_all(__VA_ARGS__)
// Home row mod and tap hold wrapper
#define HRM65(k) HRM65_SACG(k)
#define HRM65_SACG( \
	k000, k001, k002, k003, k004, k005, k006, k007, k008, k009, k010, k011, k012, k013, k014, k015, \
	k100,       k102, k103, k104, k105, k106, k107, k108, k109, k110, k111, k112, k113, k114, k115, \
	k200,       k202, k203, k204, k205, k206, k207, k208, k209, k210, k211, k212, k213,       k215, \
	k300, k301, k302, k303, k304, k305, k306, k307, k308, k309, k310, k311,       k313, k314,       \
	k400, k401,       k403, k404,       k406,       k408,       k410, k411, k412, k413, k414, k415  \
) \
	k000, k001, k002,        k003,        k004,        k005,        k006, k007, k008,        k009,        k010,        k011,        k012, k013, k014, k015,\
	k100,       k102,        k103,        k104,        k105,        k106, k107, k108,        k109,        k110,        k111,        k112, k113, k114, k115,\
	k200,       SFT_T(k202), ALT_T(k203), CTL_T(k204), GUI_T(k205), k206, k207, GUI_T(k208), CTL_T(k209), ALT_T(k210), SFT_T(k211), k212, k213,       k215,\
	k300, k301, k302,        k303,        k304,        TH(k305),    k306, k307, TH(k308),    TH(k309),    TH(k310),    TH(k311),          k313, k314,      \
	k400, k401,              k403,        k404,                     k406,       k408,                     k410,        k411,        k412, k413, k414, k415

#define MARK_DEFAULT \
	KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSLS, KC_GRV,  KC_VOLU, \
	KC_TAB,           KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSPC, KC_VOLD, \
	KC_CAPS,          KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,           KC_MUTE, \
	OSM_SFT, _______, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, OSM_SFT,          KC_UP,            \
	OSM_ALT, OSM_CTL,          OSM_GUI, LT(2,KC_SPC),     KC_ENT,           RSFT_T(KC_SPC),   KC_RCTL, KC_RALT, KC_RALT, KC_LEFT, KC_DOWN, KC_RGHT
#define MARK_COLEMAK \
	_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
	_______,          KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,    KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, _______, _______, _______, _______, \
	_______,          KC_A,    KC_R,    KC_S,    KC_T,    KC_D,    KC_H,    KC_N,    KC_E,    KC_I,    KC_O,    _______, _______,          _______, \
	_______, _______, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, _______,          _______,          \
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
