// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#pragma once

#define U_NP KC_NO  // key is not present
#define U_NA KC_NO  // present but not available for use
#define U_NU KC_NO  // available but not used

// cut / copy / paste / undo / redo / select all
#define U_CUT C(KC_X)
#define U_CPY C(KC_C)
#define U_PST C(KC_V)
#define U_UND C(KC_Z)
#define U_RDO C(KC_Y)
#define U_SLA C(KC_A)

// close
#define U_CLOSE A(KC_F4)

// run window
#define U_RUN RGUI(KC_R)

// Home row mod-tap macros
#define HM_A	LGUI_T(KC_A)
#define HM_S	LALT_T(KC_S)
#define HM_D	LCTL_T(KC_D)
#define HM_F	LSFT_T(KC_F)
#define HM_J	RSFT_T(KC_J)
#define HM_K	LCTL_T(KC_K)
#define HM_L	LALT_T(KC_L)
#define HM_QUOT	RGUI_T(KC_QUOT)
#define HRML(k1,k2,k3,k4) LGUI_T(k1),LALT_T(k2),LCTL_T(k3),LSFT_T(k4)
#define HRMR(k1,k2,k3,k4) RSFT_T(k1),LCTL_T(k2),LALT_T(k3),LGUI_T(k4)

// Layers
#define BASE 0
#define MEDR 1
#define NAVR 2
#define MOUR 3
#define NSSL 4
#define NSL 5
#define FUNL 6

// Default 3x6_3 split layout
#define _BASE \
        KC_EQL, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_MINS, \
        KC_LCTL, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_QUOT, KC_BSLS, \
        KC_CAPS, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_SCLN, \
            LT(MEDR, KC_ESC), LT(NAVR, KC_SPC), LT(MOUR, KC_TAB), LT(NSSL, KC_ENT), LT(NSL, KC_BSPC), LT(FUNL, KC_DEL) 
/* ,-----------------------------------------.                ,-----------------------------------------.
   |  =   |  Q   |  W   |  E   |  R   |  T   |                |  Y   |  U   |  I   |  O   |  P   |  -   |
   |------+------+------+------+------+------|                |------+------+------+------+------+------|
   | CTRL |  A   |  S   |  D   |  F   |  G   |                |  H   |  J   |  K   |  L   | ' "  | \ |  |
   |------+------+------+------+------+------|                |------+------+------+------+------+------|
   | CAPS |  Z   |  X   |  C   |  V   |  B   |                |  N   |  M   | , <  | . >  | / ?  | ; :  |
   `------+------+------+------+------+------+------.  ,------+------+------+------+------+------+------'
                               |_MEDR |_NAVR |_MOUR |  |_NSSL | _NSL |_FUNL |
                               | ESC  | SPC  | TAB  |  | ENT  | BSPC | DEL  |
                               `--------------------'  `--------------------'
 */
#define _MEDR \
        KC_EQL, QK_RBT, U_NA, U_NA, U_NA, U_NA, RGB_TOG, RGB_MOD, RGB_HUI, RGB_SAI, RGB_VAI, KC_MINS, \
        KC_LCTL, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, U_NA, U_NU, KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT, KC_BSLS, \
        KC_CAPS, U_NA, KC_ALGR, U_NA, U_NA, U_NA, U_NU, U_NU, U_NU, U_NU, U_NU, KC_SCLN, \
                                    U_NA, U_NA, U_NA, KC_MSTP, KC_MPLY, KC_MUTE
/* ,-----------------------------------------.                ,-----------------------------------------.
   |  =   | RBT  |      |      |      |      |                | RTOG | RMOD | RHUI | RSAT | RVAI |  -   |
   |------+------+------+------+------+------|                |------+------+------+------+------+------|
   | CTRL | LGUI |  ALT | CTL  | LSFT |      |                |      | MPRV | VOLD | VOLU | MNXT | \ |  |
   |------+------+------+------+------+------|                |------+------+------+------+------+------|
   | CAPS |      | ALGR |      |      |      |                |      |      |      |      |      | ; :  |
   `------+------+------+------+------+------+------.  ,------+------+------+------+------+------+------'
                               |      |      |      |  | MED  | MED  | MED  |
                               |      |      |      |  | STOP | PLAY | MUTE |
                               `--------------------'  `--------------------'
 */
#define _NAVR \
        KC_EQL,  QK_RBT,   U_NA,    U_NA,   U_RUN,  U_CLOSE,  U_RDO,   U_PST,   U_CPY,   U_CUT,   U_UND,   KC_MINS, \
        KC_LCTL, U_SLA, KC_LALT, KC_LCTL, KC_LSFT, U_NA,  KC_CAPS, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_BSLS, \
        KC_CAPS, U_UND,   U_CUT,   U_CPY,   U_PST,   U_RDO, KC_INS,  KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_SCLN,\
                                    U_NA,    U_NA,    U_NA,  KC_ENT,  KC_BSPC, KC_DEL
/* ,-----------------------------------------.                ,-----------------------------------------.
   |  =   | RBT  |      |      | RUN  |ALT+F4|                | RDO  | PST  | CPY  | CUT  | UND  |  -   |
   |------+------+------+------+------+------|                |------+------+------+------+------+------|
   | CTRL |SelAll|  ALT | CTL  | LSFT |      |                | CAPS | LEFT | DOWN | UP   | RGHT | \ |  |
   |------+------+------+------+------+------|                |------+------+------+------+------+------|
   | CAPS |  UND | CUT  | CPY  | PST  | RDO  |                | INS  | HOME | PGDN | PGUP | END  | ; :  |
   `------+------+------+------+------+------+------.  ,------+------+------+------+------+------+------'
                               |      |      |      |  |_NSSL | _NSL |_FUNL |
                               |      |      |      |  | ENT  | BSPC | DEL  |
                               `--------------------'  `--------------------'
 */
#define _MOUR \
        KC_EQL, QK_RBT, U_NA, U_NA, U_NA, U_NA, U_NU, U_NU, U_NU, U_NU, U_NU, KC_MINS, \
        KC_LCTL, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, U_NA, U_NU, KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, KC_BSLS, \
        KC_CAPS, U_NA,    KC_ALGR, U_NA,    U_NA,    U_NA, U_NU, KC_WH_L, KC_WH_D, KC_WH_U, KC_WH_R, KC_SCLN, \
                                    U_NA, U_NA, U_NA, KC_BTN1, KC_BTN3, KC_BTN2
/* ,-----------------------------------------.                ,-----------------------------------------.
   |  =   | RBT  |      |      |      |      |                |      |      |      |      |      |  -   |
   |------+------+------+------+------+------|                |------+------+------+------+------+------|
   | CTRL | LGUI |  ALT | CTL  | LSFT |      |                |      | MSLF | MSDN | MSUP | MSRT | \ |  |
   |------+------+------+------+------+------|                |------+------+------+------+------+------|
   | CAPS |      | ALGR |      |      |      |                |      | WHLF | WHDN | WHUP | WHRT | ; :  |
   `------+------+------+------+------+------+------.  ,------+------+------+------+------+------+------'
                               |      |      |      |  | MS   | MS   | MS   |
                               |      |      |      |  | BTN1 | BTN3 | BTN2 |
                               `--------------------'  `--------------------'
 */
#define _NSSL \
        KC_EQL, KC_LCBR, KC_AMPR, KC_ASTR, KC_LPRN, KC_RCBR, U_NA, U_NA, U_NA, U_NA, QK_RBT, KC_MINS, \
        KC_LCTL, KC_COLN, KC_DLR, KC_PERC, KC_CIRC, KC_PLUS, U_NA, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, KC_BSLS, \
        KC_CAPS, KC_TILD, KC_EXLM, KC_AT, KC_HASH, KC_PIPE, U_NA, U_NA, KC_COMM, KC_DOT, KC_SLSH, KC_SCLN, \
                                KC_LPRN, KC_RPRN, KC_UNDS, U_NA, U_NA, U_NA
/* ,-----------------------------------------.                ,-----------------------------------------.
   |  =   |  {   |  &   |  *   |  (   |  }   |                |      |      |      |      | RBT  |  -   |
   |------+------+------+------+------+------|                |------+------+------+------+------+------|
   | CTRL |  :   |  $   |  %   |  ^   |  +   |                |      | RSFT | LCTL | LALT | LGUI | \ |  |
   |------+------+------+------+------+------|                |------+------+------+------+------+------|
   | CAPS |  ~   |  !   |  @   |  #   |  |   |                |      |      |  ,   |  .   |  /   | ; :  |
   `------+------+------+------+------+------+------.  ,------+------+------+------+------+------+------'
                               |      |      |      |  |      |      |      |
                               |  APP | SPC  | TAB  |  |      |      |      |
                               `--------------------'  `--------------------'
 */
#define _NSL \
        KC_EQL, KC_LBRC, KC_7, KC_8, KC_9, KC_RBRC, U_NA, U_NA, U_NA, U_NA, QK_RBT, KC_MINS, \
        KC_LCTL, KC_SCLN, KC_4, KC_5, KC_6, KC_EQL, U_NA, KC_RSFT, KC_LCTL, KC_LALT, KC_LGUI, KC_BSLS, \
        KC_CAPS, KC_GRV, KC_1, KC_2, KC_3, KC_BSLS, U_NA, U_NA, KC_COMM, KC_DOT, KC_SLSH, KC_SCLN, \
                                KC_DOT, KC_0, KC_MINS, U_NA, U_NA, U_NA
/* ,-----------------------------------------.                ,-----------------------------------------.
   |  =   | [ {  |  7   |  8   |  9   | ] }  |                |      |      |      |      | RBT  |  -   |
   |------+------+------+------+------+------|                |------+------+------+------+------+------|
   | CTRL | ; :  |  4   |  5   |  6   | = +  |                |      | RSFT | LCTL | LALT | LGUI | \ |  |
   |------+------+------+------+------+------|                |------+------+------+------+------+------|
   | CAPS | ` ~  |  1   |  2   |  3   | \ |  |                |      |      |  ,   |  .   |  /   | ; :  |
   `------+------+------+------+------+------+------.  ,------+------+------+------+------+------+------'
                               |      |      |      |  |      |      |      |
                               |  APP | SPC  | TAB  |  |      |      |      |
                               `--------------------'  `--------------------'
 */
#define _FUNL \
        KC_EQL, KC_F12, KC_F7, KC_F8, KC_F9, KC_PSCR, U_NA, U_NA, U_NA, U_NA, QK_RBT, KC_MINS, \
        KC_LCTL, KC_F11, KC_F4, KC_F5, KC_F6, KC_SCRL, U_NA, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, KC_BSLS, \
        KC_CAPS, KC_F10, KC_F1, KC_F2, KC_F3, KC_PAUS, U_NA, U_NA, KC_COMM, KC_DOT, KC_SLSH, KC_SCLN, \
                                KC_APP, KC_SPC, KC_TAB, U_NA, U_NA, U_NA

/* ,-----------------------------------------.                ,-----------------------------------------.
   |  =   | F12  |  F7  |  F8  |  F9  | PSCR |                |      |      |      |      | RBT  |  -   |
   |------+------+------+------+------+------|                |------+------+------+------+------+------|
   | CTRL | F11  |  F4  |  F5  |  F6  | SCRL |                |      | RSFT | LCTL | LALT | LGUI | \ |  |
   |------+------+------+------+------+------|                |------+------+------+------+------+------|
   | CAPS | F10  |  F1  |  F2  |  F3  | BRK  |                |      |      |  ,   |  .   |  /   | ; :  |
   `------+------+------+------+------+------+------.  ,------+------+------+------+------+------+------'
                               |      |      |      |  |      |      |      |
                               |  APP | SPC  | TAB  |  |      |      |      |
                               `--------------------'  `--------------------'
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
	k25,      k26, k27, k28, k29,  k30,    k31,      k32, k33, k34, k35, k36, \
	                    k37, k38,  k39,    k40, k41, k42

// Layout aliases
#define LAYOUT_corne_w(...) LAYOUT_split_3x6_3(__VA_ARGS__)
#define LAYOUT_skeletyl_w(...) LAYOUT_split_3x5_3(__VA_ARGS__)
#define LAYOUT_ortho_w(...) LAYOUT_ortho_4x12(__VA_ARGS__)
#define LAYOUT_36key_w(...) LAYOUT(__VA_ARGS__)
#define LAYOUT_cornelius_w(...) LAYOUT(__VA_ARGS__)
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
KC_MUTE, KC_VOLD, KC_VOLU, k37, k38, k39, k40, k41, k42, KC_MSTP, KC_MPLY, NK_TOGG
