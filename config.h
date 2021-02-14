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

#pragma once

// Common QMK settings
#define TAPPING_TERM 280
#define TAPPING_TERM_PER_KEY
#define PERMISSIVE_HOLD
#define IGNORE_MOD_TAP_INTERRUPT
#define TAP_CODE_DELAY 50
#define LAYER_STATE_8BIT // 8-layer limit, saves ~462 bytes

#ifdef MOUSEKEY_ENABLE
#	define MOUSEKEY_DELAY 0
#	define MOUSEKEY_INTERVAL 16
#	define MOUSEKEY_MAX_SPEED 7
#	define MOUSEKEY_TIME_TO_MAX 40
#	define MOUSEKEY_WHEEL_DELAY 0
#	define MOUSEKEY_WHEEL_INTERVAL 50
#	define MOUSEKEY_WHEEL_TIME_TO_MAX 100
#endif

#ifdef SPLIT_KEYBOARD
#	define EE_HANDS
#endif

#ifdef OLED_DRIVER_ENABLE
#	define OLED_TIMEOUT 8000
#	define OLED_FONT_H "users/filterpaper/glcdfont.c"
#endif

#ifdef RGB_MATRIX_ENABLE
#	define RGB_MATRIX_KEYPRESSES
#	define RGB_DISABLE_WHEN_USB_SUSPENDED true
#	define RGB_MATRIX_MAXIMUM_BRIGHTNESS 100
// Disable unused effects
#	define DISABLE_RGB_MATRIX_ALPHAS_MODS
#	define DISABLE_RGB_MATRIX_GRADIENT_UP_DOWN
#	define DISABLE_RGB_MATRIX_BREATHING
#	define DISABLE_RGB_MATRIX_BAND_SAT
#	define DISABLE_RGB_MATRIX_BAND_VAL
#	define DISABLE_RGB_MATRIX_BAND_PINWHEEL_SAT
#	define DISABLE_RGB_MATRIX_BAND_PINWHEEL_VAL
#	define DISABLE_RGB_MATRIX_BAND_SPIRAL_SAT
#	define DISABLE_RGB_MATRIX_BAND_SPIRAL_VAL
#	define DISABLE_RGB_MATRIX_CYCLE_ALL
#	define DISABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
#	define DISABLE_RGB_MATRIX_CYCLE_UP_DOWN
#	define DISABLE_RGB_MATRIX_CYCLE_OUT_IN
#	define DISABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
#	define DISABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
#	define DISABLE_RGB_MATRIX_DUAL_BEACON
#	define DISABLE_RGB_MATRIX_CYCLE_PINWHEEL
#	define DISABLE_RGB_MATRIX_CYCLE_SPIRAL
#	define DISABLE_RGB_MATRIX_RAINBOW_BEACON
#	define DISABLE_RGB_MATRIX_RAINBOW_PINWHEELS
#	define DISABLE_RGB_MATRIX_RAINDROPS
#	define DISABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
#endif

// Layer tap hold macro references for keymap[]
#define TH_Q LT(0,KC_Q)
#define TH_W LT(0,KC_W)
#define TH_T LT(0,KC_T)
#define TH_N LT(0,KC_N)
#define TH_X LT(0,KC_X)
#define TH_C LT(0,KC_C)
#define TH_V LT(0,KC_V)
