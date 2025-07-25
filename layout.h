// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#pragma once

// macOS shortcuts
#define Z_UND   G(KC_Z)
#define Z_CUT   G(KC_X)
#define Z_CPY   G(KC_C)
#define Z_PST   G(KC_V)
#define Z_LOCK  C(G(KC_Q))
#define Z_SLEEP A(G(KC_PWR))
#define Z_SHUT  C(A(G(KC_PWR)))
#define Z_SSAVE HYPR(KC_S)

// Application shortcuts
#define SA_UP  S(A(KC_UP))
#define SA_DN  S(A(KC_DOWN))
#define SG_SPC LSG(KC_SPC)

// Tap hold macros
#define TH_M    LT(0,KC_M)
#define TH_COMM LT(0,KC_COMM)
#define TH_DOT  LT(0,KC_DOT)
#define TH_SLSH LT(0,KC_SLSH)
#define TH(k1,k2,k3,k4) LT(0,k1),LT(0,k2),LT(0,k3),LT(0,k4)

// Mod tap macros
#define HM_A    LCTL_T(KC_A)
#define HM_S    LALT_T(KC_S)
#define HM_D    LGUI_T(KC_D)
#define HM_F    LSFT_T(KC_F)
#define HM_J    RSFT_T(KC_J)
#define HM_K    RGUI_T(KC_K)
#define HM_L    RALT_T(KC_L)
#define HM_QUOT RCTL_T(KC_QUOT)
#define HRML(k1,k2,k3,k4) LCTL_T(k1),LALT_T(k2),LGUI_T(k3),LSFT_T(k4)
#define HRMR(k1,k2,k3,k4) RSFT_T(k1),RGUI_T(k2),RALT_T(k3),RCTL_T(k4)

// Layers
enum layers { BSE, CMK, SYM, NUM, FNC };

// Thumb keys
#define SYM_TAB LT(SYM,KC_TAB)
#define LCA_ENT LCA_T(KC_ENT)
#define SFT_SPC RSFT_T(KC_SPC)
#define NUM_BSP LT(NUM,KC_BSPC)

// Default 3x5_2 split layout
#define BASE \
    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    \
    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,        KC_H,    KC_J,    KC_K,    KC_L,    KC_QUOT, \
    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,        KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, \
                               SYM_TAB, LCA_ENT,     SFT_SPC, NUM_BSP
 /*╭────────┬────────┬────────┬────────┬────────╮   ╭────────┬────────┬────────┬────────┬────────╮
   │  Q     │  W     │  E     │  R     │  T     │   │  Y     │  U     │  I     │  O     │  P     │
   ├────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┤
   │  A/CTL │  S/ALT │  D/CMD │  F/SFT │  G     │   │  H     │  J/SFT │  K/CMD │  L/ALT │ ' "/CTL│
   ├────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┤
   │  Z     │  X     │  C     │  V     │  B     │   │  N     │  M     │ , <    │ . >    │ / ?    │
   ╰────────┴────────┴────────┼────────┼────────┤   ├────────┼────────┼────────┴────────┴────────╯
                              │TAB/SYM │ENT/LCA │   │SPC/SFT │BSPC/NUM│
                              ╰────────┴────────╯   ╰────────┴────────╯*/

#define COLE \
    KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,        KC_J,    KC_L,    KC_U,    KC_Y,    KC_QUOT, \
    KC_A,    KC_R,    KC_S,    KC_T,    KC_D,        KC_H,    KC_N,    KC_E,    KC_I,    KC_O,    \
    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,        KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, \
                               _______, _______,     _______, _______
 /*╭────────┬────────┬────────┬────────┬────────╮   ╭────────┬────────┬────────┬────────┬────────╮
   │  Q     │  W     │  F     │  P     │  G     │   │  J     │  L     │  U     │  Y     │ ' "    │
   ├────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┤
   │  A/CTL │  R/ALT │  S/CMD │  T/SFT │  D     │   │  H     │  N/SFT │  E/CMD │  I/ALT │  O/CTL │
   ├────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┤
   │  Z     │  X     │  C     │  V     │  B     │   │  K     │  M     │ , <    │ . >    │ / ?    │
   ╰────────┴────────┴────────┼────────┼────────┤   ├────────┼────────┼────────┴────────┴────────╯
                              │        │        │   │        │        │
                              ╰────────┴────────╯   ╰────────┴────────╯*/

#define SYMB \
    _______, KC_LBRC, KC_LCBR, KC_RCBR, _______,     KC_CIRC, KC_LPRN, KC_RPRN, KC_RBRC, KC_TILD, \
    KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,     KC_ASTR, KC_MINS, KC_EQL,  KC_BSLS, KC_GRV,  \
    _______, _______, _______, _______, _______,     KC_AMPR, KC_UNDS, KC_PLUS, KC_PIPE, _______, \
                               _______, _______,     _______, _______
 /*╭────────┬────────┬────────┬────────┬────────╮   ╭────────┬────────┬────────┬────────┬────────╮
   │        │  [     │  {     │  }     │        │   │  ^     │  (     │  )     │  ]     │  ~     │
   ├────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┤
   │  !     │  @     │  #     │  $     │  %     │   │  *     │  -     │  =     │  \     │  `     │
   ├────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┤
   │        │        │        │        │        │   │  &     │  _     │  +     │  │     │        │
   ╰────────┴────────┴────────┼────────┼────────┤   ├────────┼────────┼────────┴────────┴────────╯
                              │        │        │   │        │        │
                              ╰────────┴────────╯   ╰────────┴────────╯*/

#define NUMB \
    KC_INS,  KC_1,    KC_2,    KC_3,    KC_VOLU,     KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_COLN, \
    KC_CAPS, KC_4,    KC_5,    KC_6,    KC_VOLD,     KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_SCLN, \
    KC_DEL,  KC_7,    KC_8,    KC_9,    KC_0,        _______, SA_DN,   SA_UP,   _______, _______, \
                               MO(FNC), KC_ESC,      _______, _______
 /*╭────────┬────────┬────────┬────────┬────────╮   ╭────────┬────────┬────────┬────────┬────────╮
   │ INSERT │  1     │  2     │  3     │ VOL UP │   │ HOME   │ PG DN  │ PG UP  │ END    │  :     │
   ├────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┤
   │ CAPS   │  4     │  5     │  6     │ VOL DN │   │ LEFT   │ DOWN   │ UP     │ RIGHT  │  ;     │
   ├────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┤
   │ DELETE │  7     │  8     │  9     │  0     │   │        │ SA DN  │ SA UP  │        │        │
   ╰────────┴────────┴────────┼────────┼────────┤   ├────────┼────────┼────────┴────────┴────────╯
                              │ FNC    │ ESC    │   │        │        │
                              ╰────────┴────────╯   ╰────────┴────────╯*/

#define FUNC \
    QK_BOOT, KC_F1,   KC_F2,   KC_F3,   KC_F10,      _______, MS_WHLU, MS_WHLD, _______, TG(CMK), \
    _______, KC_F4,   KC_F5,   KC_F6,   KC_F11,      MS_LEFT, MS_DOWN, MS_UP,   MS_RGHT, _______, \
    Z_SLEEP, KC_F7,   KC_F8,   KC_F9,   KC_F12,      _______, MS_BTN2, MS_BTN1, _______, Z_SSAVE, \
                               _______, _______,     _______, _______
 /*╭────────┬────────┬────────┬────────┬────────╮   ╭────────┬────────┬────────┬────────┬────────╮
   │ BOOT   │  F1    │  F2    │  F3    │ F10    │   │        │ WH UP  │ WH DN  │        │COLEMAK │
   ├────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┤
   │        │  F4    │  F5    │  F6    │ F11    │   │ MS LFT │ MS DN  │ MS UP  │ MS RHT │        │
   ├────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┤
   │ SLEEP  │  F7    │  F8    │  F9    │ F12    │   │        │ BTN 2  │ BTN 1  │        │ SSAVE  │
   ╰────────┴────────┴────────┼────────┼────────┤   ├────────┼────────┼────────┴────────┴────────╯
                              │        │        │   │        │        │
                              ╰────────┴────────╯   ╰────────┴────────╯*/


// Mod-tap wrapper
#define HRM(k) HR_MODTAP(k)
#define HR_MODTAP( \
      l01, l02, l03, l04, l05,   r01, r02, r03, r04, r05, \
      l06, l07, l08, l09, l10,   r06, r07, r08, r09, r10, \
      l11, l12, l13, l14, l15,   r11, r12, r13, r14, r15, \
                     l16, l17,   r16, r17                 \
) \
      l01, l02, l03, l04, l05,   r01, r02, r03, r04, r05,       \
HRML(l06, l07, l08, l09), l10,   r06, HRMR(r07, r08, r09, r10), \
      l11, l12, l13, l14, l15,   r11,   TH(r12, r13, r14, r15), \
                     l16, l17,   r16, r17


// Layout aliases for json keymap
#define LAYOUT_34key_w(...) LAYOUT_split_3x5_2(__VA_ARGS__)
#define LAYOUT_crkbd_w(...) LAYOUT_split_3x6_3(__VA_ARGS__)


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
                 KC_NO, l16, l17,   r16, r17, KC_NO
