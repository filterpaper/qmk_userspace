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

/* Layout wrapper macros for ortholinear and split Corne boards.
 * The top 3x12 is shared across all 3 layouts.
 */

#pragma once

// Wrapper macro names for the real layout
#define LAYOUT_wrapper_split_3x6_3(...) LAYOUT_split_3x6_3(__VA_ARGS__)
#define LAYOUT_wrapper_ortho_4x12(...) LAYOUT_ortho_4x12(__VA_ARGS__)
#define LAYOUT_wrapper_planck_mit(...) LAYOUT_planck_mit(__VA_ARGS__)

// Base QWERTY
#define BASE1 KC_TAB,  Q_TH,         W_TH,         KC_E,         KC_R,         T_TH, KC_Y, KC_U,         KC_I,         KC_O,         KC_P,            KC_BSPC
#define BASE2 KC_GESC, LSFT_T(KC_A), LALT_T(KC_S), LCTL_T(KC_D), LGUI_T(KC_F), KC_G, KC_H, RGUI_T(KC_J), RCTL_T(KC_K), RALT_T(KC_L), RSFT_T(KC_SCLN), KC_QUOT
#define BASE3 KC_LSFT, KC_Z,         KC_X,         KC_C,         KC_V,         KC_B, N_TH, M_TH,         COMM_TH,      DOT_TH,       KC_SLSH,         KC_ENT
// Last row
#define CORNEBASE                                     LT(5,KC_VOLD), LT(2,KC_VOLU), RSFT_T(KC_ENT), RSFT_T(KC_SPC), LT(3,KC_SPC), KC_RGUI
#define PLANCKBASE KC_DEL, LALT_T(KC_VOLD), LCTL_T(KC_VOLU), KC_LGUI, LT(2,KC_SPC), RSFT_T(KC_SPC), RSFT_T(KC_SPC), LT(3,KC_SPC), KC_LEFT, KC_DOWN, KC_UP, KC_RGHT
#define BM40BASE   KC_DEL, LALT_T(KC_VOLD), LCTL_T(KC_VOLU), KC_LGUI, LT(2,KC_SPC), RSFT_T(KC_SPC), LT(3,KC_SPC), KC_LEFT, KC_DOWN, KC_UP, KC_RGHT

// COLEMAK
#define COLEMAK1 _______, _______, _______,      KC_F,         KC_P,         KC_G, KC_J,    KC_L,         KC_U,         KC_Y,         KC_SCLN,      _______
#define COLEMAK2 _______, _______, LALT_T(KC_R), LCTL_T(KC_S), LGUI_T(KC_T), KC_D, _______, RGUI_T(KC_N), RCTL_T(KC_E), RALT_T(KC_I), RSFT_T(KC_O), _______
#define COLEMAK3 _______, _______, _______,      _______,      _______,      KC_B, KC_K,    _______,      _______,      _______,      _______,      _______
// Last row
#define CORNECOLEMAK                             _______, _______, _______, _______, _______, _______
#define PLANCKCOLEMAK _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
#define BM40COLEMAK   _______, _______, _______, _______, _______,     _______,      _______, _______, _______, _______, _______

// Lower
#define LOWER1 _______, _______, _______, KC_LPRN, KC_RPRN, _______, _______, KC_MINS, KC_EQL,  KC_BSLS, _______, _______
#define LOWER2 _______, _______, _______, KC_LCBR, KC_RCBR, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, _______
#define LOWER3 KC_CAPS, _______, _______, KC_LBRC, KC_RBRC, _______, _______, _______, _______, _______, _______, _______
// Last row
#define CORNELOWER                             _______, _______, _______, _______, MO(4), _______
#define PLANCKLOWER _______, _______, _______, _______, _______, _______, _______, MO(4), KC_HOME, KC_PGDN, KC_PGUP, KC_END
#define BM40LOWER   _______, _______, _______, _______, _______,     _______,      MO(4), KC_HOME, KC_PGDN, KC_PGUP, KC_END

// Raise
#define RAISE1 KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,        KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______
#define RAISE2 KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,          KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______
#define RAISE3 _______, _______, _______, _______, S(G(A(KC_V))), _______, _______, _______, _______, _______, _______, _______
// Last row
#define CORNERAISE                             _______, MO(4), _______, _______, _______, _______
#define PLANCKRAISE _______, _______, _______, _______, MO(4), _______, _______, _______, _______, _______, _______, _______
#define BM40RAISE   _______, _______, _______, _______, MO(4),     _______,      _______, _______, _______, _______, _______

// Adjust
#define ADJUST1 RESET,   KC_F1,        KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,     TG(1)
#define ADJUST2 _______, KC_F11,       KC_F12,  _______, _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  _______,    _______
#define ADJUST3 _______, A(G(KC_PWR)), _______, _______, _______, _______, _______, KC_INS,  KC_DEL,  _______, C(G(KC_Q)), _______
// Last row
#define CORNEADJUST                             _______, _______, _______, _______, _______, _______
#define PLANCKADJUST _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
#define BM40ADJUST   _______, _______, _______, _______, _______,     _______,      _______, _______, _______, _______, _______

// Mouse
#define MOUSE1 _______, _______, _______, _______, _______, _______, _______, KC_WH_U, KC_WH_D, _______, _______, _______
#define MOUSE2 _______, _______, _______, _______, _______, _______, KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, _______, _______
#define MOUSE3 _______, _______, _______, _______, _______, _______, _______, KC_BTN2, KC_BTN1, _______, _______, _______
#define CORNEMOUSE                        _______, _______, _______, _______, _______, _______
