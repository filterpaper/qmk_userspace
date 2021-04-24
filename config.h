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
#define PERMISSIVE_HOLD
//#define PERMISSIVE_HOLD_PER_KEY
#define IGNORE_MOD_TAP_INTERRUPT
#define TAP_CODE_DELAY 50
#define GRAVE_ESC_SHIFT_OVERRIDE
#define LAYER_STATE_8BIT // 8-layer limit, saves ~462 bytes

#ifdef RGB_MATRIX_ENABLE
#	define RGB_MATRIX_KEYPRESSES
#	define RGB_MATRIX_MAXIMUM_BRIGHTNESS 200
#	define RGB_DISABLE_WHEN_USB_SUSPENDED true
#	define RGB_MATRIX_STARTUP_HUE 132
#	define RGB_MATRIX_STARTUP_SAT 102
#	define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#	define DISABLE_RGB_MATRIX_ALPHAS_MODS
#	define DISABLE_RGB_MATRIX_GRADIENT_UP_DOWN
#	define DISABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
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
#	define DISABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
#	define DISABLE_RGB_MATRIX_CYCLE_OUT_IN
#	define DISABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
#	define DISABLE_RGB_MATRIX_CYCLE_PINWHEEL
#	define DISABLE_RGB_MATRIX_CYCLE_SPIRAL
#	define DISABLE_RGB_MATRIX_DUAL_BEACON
#	define DISABLE_RGB_MATRIX_RAINBOW_BEACON
#	define DISABLE_RGB_MATRIX_RAINBOW_PINWHEELS
#	define DISABLE_RGB_MATRIX_RAINDROPS
#	define DISABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
#	define DISABLE_RGB_MATRIX_HUE_BREATHING
#	define DISABLE_RGB_MATRIX_HUE_PENDULUM
#	define DISABLE_RGB_MATRIX_HUE_WAVE
#endif

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
#	define SELECT_SOFT_SERIAL_SPEED 0
// Top right corner of right-side Corne
#	define BOOTMAGIC_LITE_ROW_RIGHT 4
#	define BOOTMAGIC_LITE_COLUMN_RIGHT 0
#endif

#ifdef OLED_DRIVER_ENABLE
#	define OLED_TIMEOUT 10000
#	define OLED_DISABLE_TIMEOUT // Prevent matrix_changed triggering oled_on()
#	define OLED_UPDATE_INTERVAL 50
#	define OLED_FONT_H "users/filterpaper/oledfont.c"
#	ifndef WPM_ENABLE
#	define SPLIT_MODS_ENABLE
#	endif
#endif

// Key map layout wrapper macros
#include "layout.h"

// Tap hold macro keys for keymap[]
#define Q_TH LT(0,KC_Q)
#define W_TH LT(0,KC_W)
#define T_TH LT(0,KC_T)
#define N_TH LT(0,KC_N)
#define DOT_TH LT(0,KC_DOT)
#define COMM_TH LT(0,KC_COMM)
#define M_TH LT(0,KC_M)
