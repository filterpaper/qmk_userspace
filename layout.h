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

// Navigation shortcuts
#define SA_UP S(A(KC_UP))
#define SA_DN S(A(KC_DOWN))

// Layers
#define BSE 0
#define CMK 1
#define NUM 2
#define SYM 3
#define FNC 4

// Default 3x5_2 split layout
#define _BASE \
/* ╭────────┬────────┬────────┬────────┬────────╮   ╭────────┬────────┬────────┬────────┬────────╮ */\
/* │  Q     │  W     │  E     │  R     │  T     │   │  Y     │  U     │  I     │  O     │  P     │ */\
	KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,       \
/* ├────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┤ */\
/* │  A     │  S     │  D     │  F     │  G     │   │  H     │  J     │  K     │  L     │ ' "    │ */\
	KC_A,    KC_S,    KC_D,    KC_F,    KC_G,        KC_H,    KC_J,    KC_K,    KC_L,    KC_QUOT,    \
/* ├────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┤ */\
/* │  Z     │  X     │  C     │  V     │  B     │   │  N     │  M     │ , <    │ . >    │ / ?    │ */\
	KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,        KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,    \
/* ╰────────┴────────┴────────┼────────┼────────┤   ├────────┼────────┼────────┴────────┴────────╯ */\
	              LT(SYM,KC_TAB), LCA_T(KC_ENT),     RSFT_T(KC_SPC), LT(NUM,KC_BSPC)
/*                            ╰────────┴────────╯   ╰────────┴────────╯ */
#define _COLE \
/* ╭────────┬────────┬────────┬────────┬────────╮   ╭────────┬────────┬────────┬────────┬────────╮ */\
/* │  Q     │  W     │  F     │  P     │  G     │   │  J     │  L     │  U     │  Y     │ ' "    │ */\
	KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,        KC_J,    KC_L,    KC_U,    KC_Y,    KC_QUOT,    \
/* ├────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┤ */\
/* │  A     │  R     │  S     │  T     │  D     │   │  H     │  N     │  E     │  I     │  O     │ */\
	KC_A,    KC_R,    KC_S,    KC_T,    KC_D,        KC_H,    KC_N,    KC_E,    KC_I,    KC_O,       \
/* ├────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┤ */\
/* │  Z     │  X     │  C     │  V     │  B     │   │  K     │  M     │ , <    │ . >    │ / ?    │ */\
	KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,        KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,    \
/* ╰────────┴────────┴────────┼────────┼────────┤   ├────────┼────────┼────────┴────────┴────────╯ */\
	                           _______, _______,     _______, _______
/*                            ╰────────┴────────╯   ╰────────┴────────╯ */
#define _NUMB \
/* ╭────────┬────────┬────────┬────────┬────────╮   ╭────────┬────────┬────────┬────────┬────────╮ */\
/* │ INSERT │  1     │  2     │  3     │ VOL UP │   │ HOME   │ PG DN  │ PG UP  │ END    │  :     │ */\
	KC_INS,  KC_1,    KC_2,    KC_3,    KC_VOLU,     KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_COLN,    \
/* ├────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┤ */\
/* │ DELETE │  4     │  5     │  6     │ VOL DN │   │ LEFT   │ DOWN   │ UP     │ RIGHT  │  ;     │ */\
	KC_DEL,  KC_4,    KC_5,    KC_6,    KC_VOLD,     KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_SCLN,    \
/* ├────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┤ */\
/* │ CAPS   │  7     │  8     │  9     │  0     │   │        │SHFTA DN│SHFTA UP│        │        │ */\
	KC_CAPS, KC_7,    KC_8,    KC_9,    KC_0,        _______,  SA_DN,   SA_UP,  _______, _______,    \
/* ╰────────┴────────┴────────┼────────┼────────┤   ├────────┼────────┼────────┴────────┴────────╯ */\
	                           MO(FNC), QK_GESC,     _______, _______
/*                            ╰────────┴────────╯   ╰────────┴────────╯ */
#define _SYMB \
/* ╭────────┬────────┬────────┬────────┬────────╮   ╭────────┬────────┬────────┬────────┬────────╮ */\
/* │        │  [     │  {     │  }     │        │   │  ^     │  (     │  )     │  ]     │  ~     │ */\
	_______, KC_LBRC, KC_LCBR, KC_RCBR, _______,     KC_CIRC, KC_LPRN, KC_RPRN, KC_RBRC, KC_TILD,    \
/* ├────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┤ */\
/* │  !     │  @     │  #     │  $     │  %     │   │  *     │  -     │  =     │  \     │  `     │ */\
	KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,     KC_ASTR, KC_MINS, KC_EQL,  KC_BSLS, KC_GRV,     \
/* ├────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┤ */\
/* │        │        │        │        │        │   │  &     │  _     │  +     │  │     │        │ */\
	_______, _______, _______, _______, _______,     KC_AMPR, KC_UNDS, KC_PLUS, KC_PIPE, _______,    \
/* ╰────────┴────────┴────────┼────────┼────────┤   ├────────┼────────┼────────┴────────┴────────╯ */\
	                           _______, _______,     _______, MO(FNC)
/*                            ╰────────┴────────╯   ╰────────┴────────╯ */
#define _FUNC \
/* ╭────────┬────────┬────────┬────────┬────────╮   ╭────────┬────────┬────────┬────────┬────────╮ */\
/* │ BOOT   │  F1    │  F2    │  F3    │ F10    │   │        │ WH UP  │ WH DN  │        │COLEMAK │ */\
	QK_BOOT, KC_F1,   KC_F2,   KC_F3,   KC_F10,      _______, KC_WH_U, KC_WH_D, _______, TG(CMK),    \
/* ├────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┤ */\
/* │        │  F4    │  F5    │  F6    │ F11    │   │ MS LFT │ MS DN  │ MS UP  │ MS RHT │        │ */\
	_______, KC_F4,   KC_F5,   KC_F6,   KC_F11,      KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, _______,    \
/* ├────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┤ */\
/* │ SLEEP  │  F7    │  F8    │  F9    │ F12    │   │        │ BTN 2  │ BTN 1  │        │ SSAVE  │ */\
	Z_SLEEP, KC_F7,   KC_F8,   KC_F9,   KC_F12,      _______, KC_BTN2, KC_BTN1, _______, Z_SSAVE,    \
/* ╰────────┴────────┴────────┼────────┼────────┤   ├────────┼────────┼────────┴────────┴────────╯ */\
	                           _______, QK_RBT,      _______, _______
/*                            ╰────────┴────────╯   ╰────────┴────────╯ */

// Mod-tap wrapper
#define HRM(k) HRM_TAPHOLD(k)
#define HRM_TAPHOLD( \
	  l01, l02, l03, l04, l05,   r01, r02, r03, r04, r05, \
	  l06, l07, l08, l09, l10,   r06, r07, r08, r09, r10, \
	  l11, l12, l13, l14, l15,   r11, r12, r13, r14, r15, \
	                 l16, l17,   r16, r17                 \
) \
	  l01, l02, l03, l04, l05,   r01, r02, r03, r04, r05,       \
HRML(l06, l07, l08, l09), l10,   r06, HRMR(r07, r08, r09, r10), \
	  l11, l12, l13, l14, l15,   r11,   TH(r12, r13, r14, r15), \
	                 l16, l17,   r16, r17


// Layout aliases for
#define LAYOUT_34key_w(...) LAYOUT_split_3x5_2(__VA_ARGS__)
#define LAYOUT_crkbd_w(...) LAYOUT_split_3x6_3(__VA_ARGS__)
#define LAYOUT_ortho_w(...) LAYOUT_ortho_4x12(__VA_ARGS__)


// Convert 3x5_2 to 42-key
#define C_42(k) CONV_42(k)
#define CONV_42( \
	     l01, l02, l03, l04, l05,   r01, r02, r03, r04, r05, \
	     l06, l07, l08, l09, l10,   r06, r07, r08, r09, r10, \
	     l11, l12, l13, l14, l15,   r11, r12, r13, r14, r15, \
	                    l16, l17,   r16, r17                 \
) \
KC_TAB,  l01, l02, l03, l04, l05,   r01, r02, r03, r04, r05, KC_BSPC, \
QK_GESC, l06, l07, l08, l09, l10,   r06, r07, r08, r09, r10, KC_SCLN, \
KC_LSFT, l11, l12, l13, l14, l15,   r11, r12, r13, r14, r15, KC_ENT,  \
	     RSA_T(KC_ESC), l16, l17,   r16, r17, RAG_T(KC_DEL)

// Convert 3x5_2 to 4x12
#define C_O(k) CONV_4x12(k)
#define CONV_4x12( \
	                      l01, l02, l03, l04, l05, r01, r02, r03, r04, r05, \
	                      l06, l07, l08, l09, l10, r06, r07, r08, r09, r10, \
	                      l11, l12, l13, l14, l15, r11, r12, r13, r14, r15, \
	                                     l16, l17, r16, r17                 \
) \
                 KC_TAB,  l01, l02, l03, l04, l05, r01, r02, r03, r04, r05, KC_BSPC, \
                 QK_GESC, l06, l07, l08, l09, l10, r06, r07, r08, r09, r10, KC_SCLN, \
                 KC_LSFT, l11, l12, l13, l14, l15, r11, r12, r13, r14, r15, KC_ENT,  \
KC_DEL, KC_VOLD, KC_VOLU, RSA_T(KC_ESC), l16, l17, r16, r17, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT
