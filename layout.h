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
 * The top 3x12 is shared with 3 keyboards.
 */

#pragma once


// Base layout wrapper macros
#define LAYOUT_wrapper_split_3x6_3(...) LAYOUT_split_3x6_3(__VA_ARGS__)
#define LAYOUT_wrapper_ortho_4x12(...) LAYOUT_ortho_4x12(__VA_ARGS__)
#define LAYOUT_wrapper_planck_mit(...) LAYOUT_planck_mit(__VA_ARGS__)
#define LAYOUT_wrapper_all(...) LAYOUT_all(__VA_ARGS__)


// Shared 3x12
#define QWERTY \
	KC_TAB,  Q_TH,        W_TH,        KC_E,        KC_R,        KC_T, KC_Y, KC_U,        KC_I,        KC_O,        KC_P,           KC_BSPC, \
	KC_GESC, SFT_T(KC_A), ALT_T(KC_S), CTL_T(KC_D), GUI_T(KC_F), KC_G, KC_H, GUI_T(KC_J), CTL_T(KC_K), ALT_T(KC_L), SFT_T(KC_SCLN), KC_QUOT, \
	KC_LSFT, KC_Z,        KC_X,        KC_C,        KC_V,        KC_B, KC_N, M_TH,        COMM_TH,     DOT_TH,      KC_SLSH,        KC_ENT

#define COLEMAK \
	_______, _______, _______,     KC_F,        KC_P,        KC_G, KC_J,    KC_L,        KC_U,        KC_Y,        KC_SCLN,     _______, \
	_______, _______, ALT_T(KC_R), CTL_T(KC_S), GUI_T(KC_T), KC_D, _______, GUI_T(KC_N), CTL_T(KC_E), ALT_T(KC_I), SFT_T(KC_O), _______, \
	_______, _______, _______,     _______,     _______,     KC_B, KC_K,    _______,     _______,     _______,     _______,     _______

#define LOWER \
	_______, _______, _______, KC_LPRN, KC_RPRN, _______, _______, KC_MINS, KC_EQL,  KC_BSLS, _______, _______, \
	_______, _______, _______, KC_LCBR, KC_RCBR, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, _______, \
	KC_CAPS, _______, _______, KC_LBRC, KC_RBRC, _______, _______, _______, _______, _______, _______, _______

#define RAISE \
	KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,        KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______, \
	KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,          KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______, \
	_______, _______, _______, _______, S(G(A(KC_V))), _______, _______, _______, _______, _______, _______, _______

#define ADJUST \
	RESET,   KC_F1,        KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,     TG(1), \
	_______, KC_F11,       KC_F12,  _______, _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  _______,    _______, \
	_______, A(G(KC_PWR)), _______, _______, _______, _______, _______, KC_INS,  KC_DEL,  _______, C(G(KC_Q)), _______

#define MOUSE \
	_______, _______, _______, _______, _______, _______, _______, KC_WH_U, KC_WH_D, _______, _______, _______, \
	_______, _______, _______, _______, _______, _______, KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, _______, _______, \
	_______, _______, _______, _______, _______, _______, _______, KC_BTN2, KC_BTN1, _______, _______, _______


// Last row
#define CORNEQWERTY                                   LT(5,KC_VOLD), LT(2,KC_VOLU), SFT_T(KC_ENT), SFT_T(KC_SPC), LT(3,KC_SPC), KC_RGUI
#define PLANCKQWERTY KC_DEL, ALT_T(KC_VOLD), CTL_T(KC_VOLU), KC_LGUI, LT(2,KC_SPC), SFT_T(KC_SPC), SFT_T(KC_SPC), LT(3,KC_SPC), KC_LEFT, KC_DOWN, KC_UP, KC_RGHT
#define BM40QWERTY   KC_DEL, ALT_T(KC_VOLD), CTL_T(KC_VOLU), KC_LGUI, LT(2,KC_SPC),        SFT_T(KC_SPC),         LT(3,KC_SPC), KC_LEFT, KC_DOWN, KC_UP, KC_RGHT

#define CORNELOWER                             _______, _______, _______, _______, MO(4),   _______
#define PLANCKLOWER _______, _______, _______, _______, _______, _______, _______, MO(4),   KC_HOME, KC_PGDN, KC_PGUP, KC_END
#define BM40LOWER   _______, _______, _______, _______, _______,     _______,      MO(4),   KC_HOME, KC_PGDN, KC_PGUP, KC_END

#define CORNERAISE                             _______, MO(4),   _______, _______, _______, _______
#define PLANCKRAISE _______, _______, _______, _______, MO(4),   _______, _______, _______, _______, _______, _______, _______
#define BM40RAISE   _______, _______, _______, _______, MO(4),       _______,      _______, _______, _______, _______, _______

#define CORNEBLANK                             _______, _______, _______, _______, _______, _______
#define PLANCKBLANK _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
#define BM40BLANK   _______, _______, _______, _______, _______,     _______,      _______, _______, _______, _______, _______

// Boardsource The Mark 65
#define MARK_QWERTY \
KC_GESC, KC_1,        KC_2,        KC_3,        KC_4,        KC_5,       KC_6,    KC_7,        KC_8,        KC_9,        KC_0,           KC_MINS, KC_EQL,  KC_BSLS, KC_DEL,  KC_VOLU, \
KC_TAB,  Q_TH,        W_TH,        KC_E,        KC_R,        T_TH,       KC_Y,    KC_U,        KC_I,        KC_O,        KC_P,           KC_LBRC, KC_RBRC, KC_BSPC,          KC_VOLD, \
KC_CAPS, SFT_T(KC_A), ALT_T(KC_S), CTL_T(KC_D), GUI_T(KC_F), KC_G,       KC_H,    GUI_T(KC_J), CTL_T(KC_K), ALT_T(KC_L), SFT_T(KC_SCLN), KC_QUOT, KC_ENT,                    KC_MUTE, \
KC_LSFT, KC_LSFT,     KC_Z,        KC_X,        KC_C,        KC_V,       KC_B,    N_TH,        M_TH,        COMM_TH,     DOT_TH,         KC_SLSH, KC_RSFT,          KC_UP, \
KC_LALT, KC_LCTL,     KC_LGUI,                  LT(2,KC_SPC),            KC_BSPC,              SFT_T(KC_SPC),            KC_RGUI,        KC_RCTL, KC_RALT, KC_LEFT, KC_DOWN, KC_RGHT
#define MARK_COLEMAK \
_______, _______,     _______,     _______,     _______,     _______,    _______, _______,     _______,     _______,     _______,        _______, _______, _______, _______, _______, \
_______, _______,     _______,     KC_F,        KC_P,        KC_G,       KC_J,    KC_L,        KC_U,        KC_Y,        KC_SCLN,        _______, _______, _______,          _______, \
_______, _______,     ALT_T(KC_R), CTL_T(KC_S), GUI_T(KC_T), KC_D,       _______, GUI_T(KC_N), CTL_T(KC_E), ALT_T(KC_I), SFT_T(KC_O),    _______, _______,                   _______, \
_______, _______,     _______,     _______,     _______,     _______,    KC_B,    KC_K,        _______,     _______,     _______,        _______, _______,          _______, \
_______, _______,     _______,                  _______,                 _______,              _______,                  _______,        _______, _______, _______, _______, _______
#define MARK_LAYER2 \
KC_GRV,  KC_F1,      KC_F2,        KC_F3,      KC_F4,        KC_F5,      KC_F6,   KC_F7,       KC_F8,       KC_F9,       KC_F10,         KC_F11,  KC_F12,  _______, _______, _______, \
_______, _______,    _______,      _______,    _______,      _______,    _______, _______,     _______,     _______,     _______,        _______, _______, _______,          _______, \
_______, _______,    _______,      _______,    _______,      _______,    KC_LEFT, KC_DOWN,     KC_UP,       KC_RGHT,     _______,        _______, _______,                   _______, \
_______, _______,    _______,      _______,    _______,      S(G(A(KC_V))), _______, _______,  _______,     _______,     _______,        _______, _______,          _______, \
_______, _______,    _______,                  _______,                 _______,              MO(3),                     _______,        _______, _______, _______, _______, _______
#define MARK_LAYER3 \
RESET,   _______,    _______,      _______,    _______,      _______,    _______, _______,     _______,     _______,     _______,        _______, _______, _______, TG(1),   _______, \
_______, _______,    _______,      _______,    _______,      _______,    _______, _______,     _______,     _______,     _______,        _______, _______, _______,          _______, \
_______, _______,    _______,      _______,    _______,      _______,    KC_HOME, KC_PGDN,     KC_PGUP,     KC_END,      _______,        _______, _______,                   _______, \
_______, _______,    A(G(KC_EJCT)), _______,   _______,      _______,    _______, _______,     _______,     _______,     _______,        C(G(KC_Q)), _______,       _______, \
_______, _______,     _______,                  _______,                 _______,              _______,                  _______,        _______, _______, _______, _______, _______
