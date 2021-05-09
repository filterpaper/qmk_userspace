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

/* Tap hold shortcuts and layout wrapper macros.
 * Ortholinears and split Corne board share the top 3x12 layout
 */

#pragma once


// Tap hold macro keys for keymap[]
#define Q_TH LT(0,KC_Q)
#define W_TH LT(0,KC_W)
#define DOT_TH LT(0,KC_DOT)
#define COMM_TH LT(0,KC_COMM)
#define M_TH LT(0,KC_M)

// Home row modifier wrappers
#define HRM(a) HRM_SACG(a)
#define HRM_SACG( \
	k01, k02, k03, k04, k05, k06, k07, k08, k09, k10, k11, k12, \
	k13, k14, k15, k16, k17, k18, k19, k20, k21, k22, k23, k24, \
	k25, k26, k27, k28, k29, k30, k31, k32, k33, k34, k35, k36  \
) \
	k01, k02, k03, k04, k05, k06, k07, k08, k09, k10, k11, k12, \
	k13, SFT_T(k14), ALT_T(k15), CTL_T(k16), GUI_T(k17), k18, \
	k19, GUI_T(k20), CTL_T(k21), ALT_T(k22), SFT_T(k23), k24, \
	k25, k26, k27, k28, k29, k30, k31, k32, k33, k34, k35, k36

#define HRM65(a) HRM65_SACG(a)
#define HRM65_SACG( \
	k000, k001, k002, k003, k004, k005, k006, k007, k008, k009, k010, k011, k012, k013, k014, k015, \
	k100,       k102, k103, k104, k105, k106, k107, k108, k109, k110, k111, k112, k113, k114, k115, \
	k200,       k202, k203, k204, k205, k206, k207, k208, k209, k210, k211, k212, k213,       k215, \
	k300, k301, k302, k303, k304, k305, k306, k307, k308, k309, k310, k311,       k313, k314,       \
	k400, k401,       k403, k404,       k406,       k408,       k410, k411, k412, k413, k414, k415  \
) \
	k000, k001, k002, k003, k004, k005, k006, k007, k008, k009, k010, k011, k012, k013, k014, k015, \
	k100,       k102, k103, k104, k105, k106, k107, k108, k109, k110, k111, k112, k113, k114, k115, \
	k200, SFT_T(k202), ALT_T(k203), CTL_T(k204), GUI_T(k205), k206,                                 \
	k207, GUI_T(k208), CTL_T(k209), ALT_T(k210), SFT_T(k211), k212, k213, k215,                     \
	k300, k301, k302, k303, k304, k305, k306, k307, k308, k309, k310, k311,       k313, k314,       \
	k400, k401,       k403, k404,       k406,       k408,       k410, k411, k412, k413, k414, k415


// Base layout wrapper macros
#define CORNE_wrapper(...)  LAYOUT_split_3x6_3(__VA_ARGS__)
#define PLANCK_wrapper(...) LAYOUT_ortho_4x12(__VA_ARGS__)
#define BM40_wrapper(...)   LAYOUT_planck_mit(__VA_ARGS__)
#define MARK65_wrapper(...) LAYOUT_all(__VA_ARGS__)


// Shared 3x12
#define QWERTY \
	KC_TAB,  Q_TH,    W_TH,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC, \
	KC_GESC, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
	KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    M_TH,    COMM_TH, DOT_TH,  KC_SLSH, KC_ENT


#define COLEMAK \
	_______, _______, _______, KC_F,    KC_P,    KC_G,    KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, _______, \
	_______, _______, KC_R,    KC_S,    KC_T,    KC_D,    _______, KC_N,    KC_E,    KC_I ,   KC_O,    _______, \
	_______, _______, _______, _______, _______, KC_B,    KC_K,    _______, _______, _______, _______, _______

#define LOWER \
	_______, _______, _______, KC_LPRN, KC_RPRN, _______, _______, KC_MINS, KC_EQL,  KC_BSLS, _______, _______, \
	_______, _______, _______, KC_LCBR, KC_RCBR, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, _______, \
	KC_CAPS, _______, _______, KC_LBRC, KC_RBRC, _______, _______, _______, _______, _______, _______, _______

#define RAISE \
	KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______, \
	KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______, \
	_______, _______, _______, _______, S(G(A(KC_V))), _______, _______, _______, _______, _______, _______, _______

#define ADJUST \
	RESET,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  TG(1),   \
	_______, KC_F11,  KC_F12,  _______, _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  _______, _______, \
	_______, A(G(KC_PWR)), _______, _______, _______, _______, _______, KC_INS, KC_DEL, _______, C(G(KC_Q)), _______

#define MOUSE \
	_______, _______,       _______,       _______,       _______,       _______, _______, KC_WH_U, KC_WH_D, _______, _______, _______, \
	_______, OSM(MOD_LSFT), OSM(MOD_LALT), OSM(MOD_LCTL), OSM(MOD_LGUI), _______, KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, _______, _______, \
	_______, _______,       _______,       _______,       _______,       _______, _______, KC_BTN2, KC_BTN1, _______, _______, _______


// Last row
#define CORNEQWERTY                                   LT(5,KC_VOLD), LT(2,KC_VOLU), RSFT_T(KC_ENT), RSFT_T(KC_SPC), LT(3,KC_SPC), OSM(MOD_LGUI)
#define PLANCKQWERTY KC_DEL, ALT_T(KC_VOLD), CTL_T(KC_VOLU), KC_LGUI, LT(2,KC_SPC), RSFT_T(KC_SPC), RSFT_T(KC_SPC), LT(3,KC_SPC), KC_LEFT, KC_DOWN, KC_UP, KC_RGHT
#define BM40QWERTY   KC_DEL, ALT_T(KC_VOLD), CTL_T(KC_VOLU), KC_LGUI, LT(2,KC_SPC),         RSFT_T(KC_SPC),         LT(3,KC_SPC), KC_LEFT, KC_DOWN, KC_UP, KC_RGHT

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
KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSLS, KC_GRV,  KC_VOLU, \
KC_TAB,           Q_TH,    W_TH,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSPC, KC_VOLD, \
KC_CAPS,          KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,           KC_MUTE, \
KC_LSFT, KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    M_TH,    COMM_TH, DOT_TH,  KC_SLSH, KC_RSFT,          KC_UP,            \
KC_LALT, KC_LCTL,          KC_LGUI, LT(2,KC_SPC),     KC_SPC,           RSFT_T(KC_SPC),   KC_RGUI, KC_RCTL, KC_RALT, KC_LEFT, KC_DOWN, KC_RGHT
#define MARK_COLEMAK \
_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
_______,          _______, _______, KC_F,    KC_P,    KC_G,    KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, _______, _______, _______, _______, \
_______,          _______, KC_R,    KC_S,    KC_T,    KC_D,    _______, KC_N,    KC_E,    KC_I,    KC_O,    _______, _______,          _______, \
_______, _______, _______, _______, _______, _______, KC_B,    KC_K,    _______, _______, _______, _______, _______,          _______,          \
_______, _______,          _______, _______,          _______,          _______,          _______, _______, _______, _______, _______, _______
#define MARK_LAYER2 \
_______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, KC_HOME, KC_PGUP, \
_______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_DEL,  KC_PGDN, \
_______,          _______, _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, _______, _______,          KC_END,  \
_______, _______, _______, _______, _______, S(G(A(KC_V))), _______, _______, _______, _______, _______, _______, _______,    _______,          \
_______, _______,          _______, _______,          _______,          MO(3),            _______, _______, _______, _______, _______, _______
#define MARK_LAYER3 \
RESET,   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, TG(1),   _______, \
_______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
_______,          _______, _______, _______, _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  _______, _______, _______,          _______, \
_______, _______, A(G(KC_EJCT)), _______, _______, _______, _______, _______, _______, _______, _______, C(G(KC_Q)), _______, _______,          \
_______, _______,          _______, _______,          _______,          _______,          _______, _______, _______, _______, _______, _______
