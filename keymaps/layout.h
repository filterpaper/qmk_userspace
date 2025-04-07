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
#define Z_SSAVE	HYPR(KC_S)

// Tap hold macros
#define TH_M	LT(0,KC_M)
#define TH_COMM	LT(0,KC_COMM)
#define TH_DOT	LT(0,KC_DOT)
#define TH_SLSH	LT(0,KC_SLSH)
#define TH(k1,k2,k3,k4) LT(0,k1),LT(0,k2),LT(0,k3),LT(0,k4)

// Home row mod-tap macros
#define HM_A	LSFT_T(KC_A)
#define HM_S	LALT_T(KC_S)
#define HM_D	LCTL_T(KC_D)
#define HM_F	LGUI_T(KC_F)
#define HM_J	LGUI_T(KC_J)
#define HM_K	LCTL_T(KC_K)
#define HM_L	LALT_T(KC_L)
#define HM_QUOT	LSFT_T(KC_QUOT)
#define HRML(k1,k2,k3,k4) LSFT_T(k1),LALT_T(k2),LCTL_T(k3),LGUI_T(k4)
#define HRMR(k1,k2,k3,k4) LGUI_T(k1),LCTL_T(k2),LALT_T(k3),LSFT_T(k4)

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
#define NUM 2
#define SYM 3
#define FNC 4


// Default 3x6_3 split layout
#define _BASE \
	KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC, \
	QK_GESC, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,        KC_H,    KC_J,    KC_K,    KC_L,    KC_QUOT, KC_SCLN, \
	OSM_SFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,        KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT,  \
	        RSA_T(KC_ESC), LT(SYM,KC_TAB), LCA_T(KC_ENT),     RSFT_T(KC_SPC), LT(NUM,KC_BSPC), RAG_T(KC_DEL)
/* ,-----------------------------------------.                ,-----------------------------------------.
   | TAB  |  Q   |  W   |  E   |  R   |  T   |                |  Y   |  U   |  I   |  O   |  P   | BSPC |
   |------+------+------+------+------+------|                |------+------+------+------+------+------|
   | GESC |  A   |  S   |  D   |  F   |  G   |                |  H   |  J   |  K   |  L   | ' "  | ; :  |
   |------+------+------+------+------+------|                |------+------+------+------+------+------|
   |OS_SFT|  Z   |  X   |  C   |  V   |  B   |                |  N   |  M   | , <  | . >  | / ?  | ENT  |
   `------+------+------+------+------+------+------.  ,------+------+------+------+------+------+------'
                               |RSA_T |_SYMB |LCA_T |  |RSFT_T|_NUMB |RAG_T |
                               | ESC  | TAB  | ENT  |  | SPC  | BSPC | DEL  |
                               `--------------------'  `--------------------'
 */
#define _COLE \
	_______, KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,        KC_J,    KC_L,    KC_U,    KC_Y,    KC_QUOT, _______, \
	_______, KC_A,    KC_R,    KC_S,    KC_T,    KC_D,        KC_H,    KC_N,    KC_E,    KC_I,    KC_O,    _______, \
	_______, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,        KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, _______, \
	                           _______, _______, _______,     _______, _______, _______
/* ,-----------------------------------------.                ,-----------------------------------------.
   |      |  Q   |  W   |  F   |  P   |  G   |                |  J   |  L   |  U   |  Y   | ' "  |      |
   |------+------+------+------+------+------|                |------+------+------+------+------+------|
   |      |  A   |  R   |  S   |  T   |  D   |                |  H   |  N   |  E   |  I   |  O   |      |
   |------+------+------+------+------+------|                |------+------+------+------+------+------|
   |      |  Z   |  X   |  C   |  V   |  B   |                |  K   |  M   | , <  | . >  | / ?  |      |
   `------+------+------+------+------+------+------.  ,------+------+------+------+------+------+------'
                               |      |      |      |  |      |      |      |
                               `--------------------'  `--------------------'
 */
#define _NUMB \
	_______, KC_INS,  KC_1,    KC_2,    KC_3,    KC_VOLU,     KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_COLN, _______, \
	_______, KC_DEL,  KC_4,    KC_5,    KC_6,    KC_VOLD,     KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_SCLN, _______, \
	_______, KC_CAPS, KC_7,    KC_8,    KC_9,    KC_0,        _______, RSA_DN,  RSA_UP,  _______, _______, _______, \
	                           _______, MO(FNC), QK_GESC,     _______, _______, _______
/* ,-----------------------------------------.                ,-----------------------------------------.
   |      | INS  |  1   |  2   |  3   | VOLU |                | HOME | PGDN | PGUP | END  |  :   |      |
   |------+------+------+------+------+------|                |------+------+------+------+------+------|
   |      | DEL  |  4   |  5   |  6   | VOLD |                | LEFT | DOWN |  UP  | RGHT |  ;   |      |
   |------+------+------+------+------+------|                |------+------+------+------+------+------|
   |      | CAPS |  7   |  8   |  9   |  0   |                |      |RSA_DN|RSA_UP|      |      |      |
   `------+------+------+------+------+------+------.  ,------+------+------+------+------+------+------'
                               |      |_FUNC | GESC |  |      |      |      |
                               `--------------------'  `--------------------'
 */
#define _SYMB \
	_______, _______, KC_LBRC, KC_LCBR, KC_RCBR, _______,     KC_CIRC, KC_LPRN, KC_RPRN, KC_RBRC, KC_TILD, _______, \
	_______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,     KC_ASTR, KC_MINS, KC_EQL,  KC_BSLS, KC_GRV,  _______, \
	_______, _______, _______, _______, _______, _______,     KC_AMPR, KC_UNDS, KC_PLUS, KC_PIPE, _______, _______, \
	                           _______, _______, _______,     _______, MO(FNC), _______
/* ,-----------------------------------------.                ,-----------------------------------------.
   |      |      |  [   |  {   |  }   |      |                |  ^   |  (   |  )   |  ]   |  ~   |      |
   |------+------+------+------+------+------|                |------+------+------+------+------+------|
   |      |  !   |  @   |  #   |  $   |  %   |                |  *   |  -   |  =   |  \   |  `   |      |
   |------+------+------+------+------+------|                |------+------+------+------+------+------|
   |      |      |      |      |      |      |                |  &   |  _   |  +   |  |   |      |      |
   `------+------+------+------+------+------+------.  ,------+------+------+------+------+------+------'
                               |      |      |      |  |      |_FUNC |      |
                               `--------------------'  `--------------------'
 */
#define _FUNC \
	_______, QK_BOOT, KC_F1,   KC_F2,   KC_F3,   KC_F10,      _______, KC_WH_U, KC_WH_D, _______, TG(CMK), _______, \
	_______, _______, KC_F4,   KC_F5,   KC_F6,   KC_F11,      KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, _______, _______, \
	_______, Z_SLEEP, KC_F7,   KC_F8,   KC_F9,   KC_F12,      _______, KC_BTN2, KC_BTN1, _______, Z_SSAVE, _______, \
	                           _______, _______, QK_RBT,      _______,	 _______, _______
/* ,-----------------------------------------.                ,-----------------------------------------.
   |      |BOOT  |  F1  |  F2  |  F3  | F10  |                |      | WH_U | WH_D |      |_COLE |      |
   |------+------+------+------+------+------|                |------+------+------+------+------+------|
   |      |      |  F4  |  F5  |  F6  | F11  |                | MS_L | MS_D | MS_U | MS_R |      |      |
   |------+------+------+------+------+------|                |------+------+------+------+------+------|
   |      |SLEEP |  F7  |  F8  |  F9  | F12  |                |      | BTN2 | BTN1 |      |SSAVE |      |
   `------+------+------+------+------+------+------.  ,------+------+------+------+------+------+------'
                               |      |      |REBOOT|  |      |      |      |
                               `--------------------'  `--------------------'
 */

// 3x6_3 mod-tap wrapper
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



// Layout aliases for json use
#define LAYOUT_corne_w(...) LAYOUT_split_3x6_3(__VA_ARGS__)
#define LAYOUT_ortho_w(...) LAYOUT_ortho_4x12(__VA_ARGS__)
#define LAYOUT_36key_w(...) LAYOUT(__VA_ARGS__)
#define LAYOUT_34key_w(...) LAYOUT_split_3x5_2(__VA_ARGS__)



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
