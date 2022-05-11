// Copyright 2021 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

// macOS shortcuts
#define Z_UND	G(KC_Z)
#define Z_CUT	G(KC_X)
#define Z_CPY	G(KC_C)
#define Z_PST	G(KC_V)
#define Z_LOCK	C(G(KC_Q))
#define Z_SLEEP	A(G(KC_PWR))
#define Z_SHUT	C(A(G(KC_PWR)))
#define Z_FND	G(KC_F)
#define Z_RLD	G(KC_R)

// Shortcuts that will be picked up by a tool like BetterTouchTool
// to set Ploopy Nano Trackball leds and toggle scroll mode or reset
#define PLP_SCR HYPR(KC_S) // Scroll
#define PLP_RST HYPR(KC_R) // Reset

// Tap hold macros
#define M_TH	LT(0,KC_M)
#define COMM_TH	LT(0,KC_COMM)
#define DOT_TH	LT(0,KC_DOT)
#define SLSH_TH	LT(0,KC_SLSH)
#define QUOT_GRAVE_TH	LT(0,KC_QUOT)
#define DOT_UNDS_TH	    LT(0,KC_DOT)
#define COMM_MINUS_TH	LT(0,KC_COMM)
#define SLSH_EXLM_TH	LT(0,KC_SLSH)
#define SCLN_COLN_TH	LT(0,KC_SCLN)

#define TH(k1,k2,k3,k4) LT(0,k1),LT(0,k2),LT(0,k3),LT(0,k4)

// Home row mod-tap macros
#define HM_A	LSFT_T(KC_A)
#define HM_S	LALT_T(KC_S)
#define HM_D	LCTL_T(KC_D)
#define HM_F	LGUI_T(KC_F)
#define HM_J	RGUI_T(KC_J)
#define HM_K	RCTL_T(KC_K)
#define HM_L	RALT_T(KC_L)
#define HM_QUOT	RSFT_T(KC_QUOT)
#define HRML(k1,k2,k3,k4) LCTL_T(k1),LALT_T(k2),LGUI_T(k3),LSFT_T(k4)
#define HRMR(k1,k2,k3,k4) RSFT_T(k1),RGUI_T(k2),RALT_T(k3),RCTL_T(k4)


// One shot mods
#ifdef NO_ACTION_ONESHOT
#	define OSM_SFT KC_LSFT
#	define OSM_CTL KC_LCTL
#	define OSM_ALT KC_LALT
#	define OSM_GUI KC_LGUI
#else
#	define OSM_SFT OSM(MOD_LSFT)
#	define OSM_CTL OSM(MOD_LCTL)
#	define OSM_ALT OSM(MOD_LALT)
#	define OSM_GUI OSM(MOD_LGUI)
#endif

// Navigation shortcuts
#define RSA_UP RSA(KC_UP)
#define RSA_DN RSA(KC_DOWN)

// Layers
#define BSE 0
#define CMK 1
#define SYM 2
#define NUM 3
#define FNC 4


// Default 3x6_3 split layout
#define _BASE \
	PLP_SCR,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,        KC_Y,    KC_U,    KC_I,          KC_O,         KC_P,            Z_CUT, \
	KC_BTN2,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,        KC_H,    KC_J,    KC_K,          KC_L,         KC_QUOT,         Z_CPY, \
	KC_BTN1,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,        KC_N,    KC_M,    COMM_MINUS_TH, DOT_UNDS_TH,  SLSH_EXLM_TH,    Z_PST, \
	               RCS_T(KC_ESC), MEH_T(KC_SPC), OSM_SFT,        OSM_GUI, TO(SYM), RSG_T(KC_BSPC)
/* ,-----------------------------------------.                ,-----------------------------------------.
 * | PSCR |  Q   |  W   |  E   |  R   |  T   |                |  Y   |  U   |  I   |  O   |  P   | CUT  |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * | BTN2 |  A   |  S   |  D   |  F   |  G   |                |  H   |  J   |  K   |  L   | ' "  | CPY  |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * | BTN1 |  Z   |  X   |  C   |  V   |  B   |                |  N   |  M   | , <  | . >  | / ?  | PST  |
 * |      |      |      |      |      |      |                |      |      |  -   |  _   |  !   |      |
 * `------+------+------+------+------+------+------.  ,------+------+------+------+------+------+------'
 *                             |RCS_T |MEH_T | OSM  |  | OSM  |      |RSG_T |
 *                             | ESC  | SPC  | SFT  |  | GUI  | SYM  | DEL  |
 *                             `--------------------'  `--------------------'
 */
#define _COLE_DH \
	_______,    KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,        KC_J,    KC_L,    KC_U,          KC_Y,         QUOT_GRAVE_TH,    _______, \
	_______,    KC_A,    KC_R,    KC_S,    KC_T,    KC_G,        KC_M,    KC_N,    KC_E,          KC_I,         KC_O,             _______, \
	_______,    KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,        KC_K,    KC_H,    COMM_MINUS_TH, DOT_UNDS_TH,  SLSH_EXLM_TH,     _______, \
	               RCS_T(KC_ESC), MEH_T(KC_SPC), OSM_SFT,        OSM_GUI, TO(SYM), RSG_T(KC_BSPC)
/* ,-----------------------------------------.                ,-----------------------------------------.
 * |      |  Q   |  W   |  F   |  P   |  B   |                |  J   |  L   |  U   |  Y   | ' "  |      |
 * |      |      |      |      |      |      |                |      |      |      |      |  `   |      |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * |      |  A   |  R   |  S   |  T   |  G   |                |  M   |  N   |  E   |  I   |  O   |      |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * |      |  Z   |  X   |  C   |  D   |  V   |                |  K   |  H   | , <  | . >  | / ?  |      |
 * `------+------+------+------+------+------+------.  ,------+------+------+------+------+------+------'
 *                             |      |      |      |  |      |      |      |
 *                             `--------------------'  `--------------------'
 */
#define _SYMB \
	_______,  KC_ESC,   KC_AT,  KC_HASH,   KC_DLR,  KC_PERC,        KC_CIRC,  KC_AMPR,  KC_ASTR,  SCLN_COLN_TH,  KC_BSPC,  _______, \
	_______,  KC_TAB,  KC_EQL,  KC_PLUS,  KC_MINS, KC_GRAVE,        KC_BSLS,  KC_LCBR,  KC_RCBR,  KC_PIPE,        KC_ENT,  _______, \
	_______, KC_BTN1, KC_BTN2,  PLP_SCR,  KC_WH_U,  KC_WH_D,        KC_LBRC,  KC_LPRN,  KC_RPRN,  KC_RBRC,       TO(FNC),  _______, \
	                            _______,  TO(BSE),  OSM_SFT,        OSM_GUI,  TO(NUM),  _______
/* ,-----------------------------------------.                ,-----------------------------------------.
 * |      |  ESC |  @   |  #   |  $   |  %   |                |  ^   |  &   |  *   |  ; : |  DEL |      |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * |      |  TAB |  =   |  +   |  -   |  `   |                |  \   |  {   |  }   |  |   |  ENT |      |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * |      | BTN1 | BTN2 | PSCR | WH_U | WH_D |                |  [   |  (   |  )   |  ]   | FUNC |      |
 * `------+------+------+------+------+------+------.  ,------+------+------+------+------+------+------'
 *                             |      | BSE  |      |  |      | NUM  |      |
 *                             `--------------------'  `--------------------'
 */
#define _NUMB \
	_______,  QK_GESC,  XXXXXXX,  KC_VOLD,  KC_VOLU,  KC_PLUS,        KC_ASTR,    KC_7,    KC_8,    KC_9,    KC_BSPC,    Z_UND, \
	_______,   KC_TAB,  XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_MINS,        KC_EQL,     KC_4,    KC_5,    KC_6,    KC_ENT,     Z_RLD, \
	_______,  XXXXXXX,  XXXXXXX,  RGB_HUI,  RGB_SAI,   KC_DOT,        KC_0,       KC_1,    KC_2,    KC_3,    TO(FNC),    Z_FND, \
	                              _______,  TO(BSE),  OSM_SFT,        OSM_GUI,    TO(SYM), _______
/* ,-----------------------------------------.                ,-----------------------------------------.
 * |      |  ESC |      | VOLU | VOLD |  +   |                |  *   |  7   |  8   |  9   |  DEL |      |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * |      |  TAB |      |      |      |  -   |                |  =   |  4   |  5   |  6   |  ENT |      |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * |      |      |      | HUI  | SAI  |  .   |                |  0   |  1   |  2   |  3   | FUNC |      |
 * `------+------+------+------+------+------+------.  ,------+------+------+------+------+------+------'
 *                             |      | BSE  |      |  |      | SYM  |      |
 *                             `--------------------'  `--------------------'
 */
#define _FUNC \
	_______,  QK_GESC,  KC_WH_L,  KC_MS_U,  KC_WH_R,  DF(BSE),             DF(CMK),  KC_HOME,  KC_END,  XXXXXXX,  KC_BSPC,  _______, \
	_______,  PLP_RST,  KC_MS_L,  KC_MS_D,  KC_MS_R,  LGUI_T(KC_DEL),      KC_LEFT,  KC_DOWN,  KC_UP,   KC_RGHT,  KC_ENT,   _______, \
	_______,    RESET,  KC_BTN1,  KC_WH_U,  KC_WH_D,  PLP_SCR,             KC_BTN2,  RSA_DN,   RSA_UP,  XXXXXXX,  XXXXXXX,  _______, \
	                              _______,  TO(BSE),  OSM_CTL,             OSM_GUI,  TO(SYM),  _______
/* ,-----------------------------------------.                ,-----------------------------------------.
 * |      | ESC  | WH_L | MS_U | MS_R | BSE  |                | CMK  | HOME |  END |      |  DEL |      |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * |      | PRST | MS_L | MS_D | MS_R | G_DEL|                | LEFT | DOWN |  UP  | RGHT |  ENT |      |
 * |------+------+------+------+------+------|                |------+------+------+------+------+------|
 * |      | RST  | BTN1 | WH_U | WH_D | PSCR |                | BTN2 | RSA_D| RSA_U|      |      |      |
 * `------+------+------+------+------+------+------.  ,------+------+------+------+------+------+------'
 *                             |      | BSE  |      |  |      | SYM  |      |
 *                             `--------------------'  `--------------------'
 */

// 3x6_3 home row mod and tap hold wrapper
#define HRM(k) HRM_TAPHOLD(k)
#define HRM_TAPHOLD( \
	      k01, k02, k03, k04, k05, k06,    k07, k08, k09, k10, k11, k12, \
	      k13, k14, k15, k16, k17, k18,    k19, k20, k21, k22, k23, k24, \
	      k25, k26, k27, k28, k29, k30,    k31, k32, k33, k34, k35, k36, \
	                     k37, k38, k39,    k40, k41, k42 \
) \
	k01,      k02, k03, k04, k05,  k06,    k07,      k08, k09, k10, k11,  k12, \
	k13, HRML(k14, k15, k16, k17), k18,    k19, HRMR(k20, k21, k22, k23), k24, \
	k25,      k26, k27, k28, k29,  k30,    k31,   TH(k32, k33, k34, k35), k36, \
	                    k37, k38,  k39,    k40, k41, k42



// Layout aliases
#define LAYOUT_corne_w(...) LAYOUT_split_3x6_3(__VA_ARGS__)
#define LAYOUT_ortho_w(...) LAYOUT_ortho_4x12(__VA_ARGS__)
#define LAYOUT_36key_w(...) LAYOUT(__VA_ARGS__)
#define LAYOUT_34key_w(...) LAYOUT_split_3x5_2(__VA_ARGS__)
#define LAYOUT_kyria_w(...) LAYOUT(__VA_ARGS__)



// 3x6_3 to 36-key conversion
#define C_36(k) SPLIT_42_TO_36(k)
#define SPLIT_42_TO_36( \
	k01, k02, k03, k04, k05, k06,    k07, k08, k09, k10, k11, k12, \
	k13, k14, k15, k16, k17, k18,    k19, k20, k21, k22, k23, k24, \
	k25, k26, k27, k28, k29, k30,    k31, k32, k33, k34, k35, k36, \
	               k37, k38, k39,    k40, k41, k42 \
) \
	     k02, k03, k04, k05, k06,    k07, k08, k09, k10, k11, \
	     k14, k15, k16, k17, k18,    k19, k20, k21, k22, k23, \
	     k26, k27, k28, k29, k30,    k31, k32, k33, k34, k35, \
	               k37, k38, k39,    k40, k41, k42

// 3x6_3 to 34-key conversion
#define C_34(k) SPLIT_42_TO_34(k)
#define SPLIT_42_TO_34( \
	k01, k02, k03, k04, k05, k06,    k07, k08, k09, k10, k11, k12, \
	k13, k14, k15, k16, k17, k18,    k19, k20, k21, k22, k23, k24, \
	k25, k26, k27, k28, k29, k30,    k31, k32, k33, k34, k35, k36, \
	               k37, k38, k39,    k40, k41, k42 \
) \
	     k02, k03, k04, k05, k06,    k07, k08, k09, k10, k11, \
	     k14, k15, k16, k17, k18,    k19, k20, k21, k22, k23, \
	     k26, k27, k28, k29, k30,    k31, k32, k33, k34, k35, \
	                    k38, k39,    k40, k41

// 3x6_3 to Ortho 4x12 conversion
#define C_O(k) SPLIT_42_TO_4x12(k)
#define SPLIT_42_TO_4x12( \
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
#define MARK_DEFAULT \
	KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSLS, KC_GRV,  KC_VOLU, \
	KC_TAB,           KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSPC, KC_VOLD, \
	KC_CAPS,          KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,           KC_MUTE, \
	OSM_SFT, _______, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          OSM_SFT, KC_UP,            \
	OSM_ALT, OSM_CTL,          OSM_GUI, LT(2,KC_SPC),     KC_ENT,           RSFT_T(KC_SPC),   KC_RCTL, KC_RALT, KC_RALT, KC_LEFT, KC_DOWN, KC_RGHT
#define MARK_COLEMAK \
	_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
	_______,          KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,    KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, _______, _______, _______, _______, \
	_______,          KC_A,    KC_R,    KC_S,    KC_T,    KC_D,    KC_H,    KC_N,    KC_E,    KC_I,    KC_O,    _______, _______,          _______, \
	_______, _______, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          _______, _______,          \
	_______, _______,          _______, _______,          _______,          _______,          _______, _______, _______, _______, _______, _______
#define MARK_LAYER2 \
	QK_BOOT, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  TG(CMK), KC_HOME, KC_PGUP, \
	_______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_DEL,  KC_PGDN, \
	_______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          KC_END,  \
	_______, _______, Z_SLEEP, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,          \
	_______, _______,          _______, _______,          _______,          _______,          _______, _______, _______, _______, _______, _______

// Mark 65 home row mod and tap hold wrapper
#define HRM65(k) HRM65_TAPHOLD(k)
#define HRM65_TAPHOLD( \
	k000, k001, k002, k003, k004, k005, k006, k007, k008, k009, k010, k011, k012, k013, k014, k015, \
	k100,       k102, k103, k104, k105, k106, k107, k108, k109, k110, k111, k112, k113, k114, k115, \
	k200,       k202, k203, k204, k205, k206, k207, k208, k209, k210, k211, k212, k213,       k215, \
	k300, k301, k302, k303, k304, k305, k306, k307, k308, k309, k310, k311,       k313, k314,       \
	k400, k401,       k403, k404,       k406,       k408,       k410, k411, k412, k413, k414, k415  \
) \
	k000, k001, k002, k003, k004, k005,  k006, k007,      k008, k009, k010, k011,  k012, k013, k014, k015,\
	k100,       k102, k103, k104, k105,  k106, k107,      k108, k109, k110, k111,  k112, k113, k114, k115,\
	k200,  HRML(k202, k203, k204, k205), k206, k207, HRMR(k208, k209, k210, k211), k212, k213,       k215,\
	k300, k301, k302, k303, k304, k305,  k306, k307,   TH(k308, k309, k310, k311),       k313, k314,      \
	k400, k401,       k403, k404,        k406,            k408,       k410, k411,  k412, k413, k414, k415

// Mark 65 layout alias
#define LAYOUT_mrk65_w(...) LAYOUT_all(__VA_ARGS__)



// Macro3
#define MACRO3_0 \
	KC_MUTE, KC_MSTP, KC_MRWD, KC_MFFD, \
	G(KC_V), G(KC_C), G(KC_X), LT(1,KC_MPLY)
#define MACRO3_1 \
	QK_BOOT, _______, KC_SLEP, KC_PWR,  \
	TG(1),   _______, _______, _______
#define LAYOUT_macro3_w(...) LAYOUT(__VA_ARGS__)



// 3x6_3 to Kyria conversion
#define C_K(k) SPLIT_3x6_3_TO_KYRIA(k)
#define SPLIT_3x6_3_TO_KYRIA( \
           k01, k02, k03, k04, k05, k06, k07, k08, k09, k10, k11, k12, \
           k13, k14, k15, k16, k17, k18, k19, k20, k21, k22, k23, k24, \
           k25, k26, k27, k28, k29, k30, k31, k32, k33, k34, k35, k36, \
                          k37, k38, k39, k40, k41, k42 \
) \
           k01, k02, k03, k04, k05, k06,                             k07, k08, k09, k10, k11, k12, \
           k13, k14, k15, k16, k17, k18,                             k19, k20, k21, k22, k23, k24, \
           k25, k26, k27, k28, k29, k30, Z_CPY, Z_PST, Z_FND, Z_RLD, k31, k32, k33, k34, k35, k36, \
                       KC_LBRC, KC_LPRN, k37,   k38,   k39,   k40,   k41, k42, KC_RPRN, KC_RBRC
