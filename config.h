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

// Key and layout wrapper macros
#include "layout.h"

// Common QMK settings
#define TAPPING_TERM 230
#define TAPPING_TERM_PER_KEY
#define PERMISSIVE_HOLD
#define PERMISSIVE_HOLD_PER_KEY
#define TAPPING_FORCE_HOLD
#define TAPPING_FORCE_HOLD_PER_KEY
#define IGNORE_MOD_TAP_INTERRUPT
#define LAYER_STATE_8BIT
#define TAP_CODE_DELAY 5
#define ONESHOT_TIMEOUT 3000
#define BOOTMAGIC_LITE_ROW 0
#define BOOTMAGIC_LITE_COLUMN 0
#define GRAVE_ESC_SHIFT_OVERRIDE
#define USB_SUSPEND_WAKEUP_DELAY 200

#ifdef RGB_MATRIX_ENABLE
#	ifndef SPLIT_KEYBOARD
#		define RGB_DISABLE_TIMEOUT 180000
#	endif
#	undef RGB_MATRIX_MAXIMUM_BRIGHTNESS
#	define RGB_MATRIX_MAXIMUM_BRIGHTNESS 150
#	undef RGB_DISABLE_WHEN_USB_SUSPENDED
#	define RGB_DISABLE_WHEN_USB_SUSPENDED true
#	define RGB_MATRIX_STARTUP_HUE 208
#	define RGB_MATRIX_STARTUP_SAT 135
#	include "rgb_matrix_disable.h" // Legacy disable options
#	define RGB_MATRIX_KEYPRESSES
#	ifdef RGB_MATRIX_CUSTOM_USER
#		define ENABLE_RGB_MATRIX_CANDY_TAP
#		define ENABLE_RGB_MATRIX_CANDY_RAIN
#	else
#		undef DISABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#		define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#		undef DISABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
#		define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
#	endif
#endif

#ifdef RGBLIGHT_ENABLE
#	undef RGBLED_NUM
#	define RGBLED_NUM 54
#	undef RGBLED_SPLIT
#	define RGBLED_SPLIT { 27, 27 }
#	define RGBLIGHT_SPLIT
#	define RGBLIGHT_LIMIT_VAL 150
#	define RGBLIGHT_SLEEP 180000
#endif

#ifdef OLED_ENABLE
#	define OLED_TIMEOUT 10000
#	define OLED_DISABLE_TIMEOUT // Prevent matrix_changed triggering oled_on()
#	define OLED_UPDATE_INTERVAL 50
#	define OLED_BRIGHTNESS 180
#	define OLED_FONT_H "users/filterpaper/oledfont.c"
#endif

#ifdef SPLIT_KEYBOARD
#	define EE_HANDS
#	define BOOTMAGIC_LITE_ROW_RIGHT 4
#	define BOOTMAGIC_LITE_COLUMN_RIGHT 0
#	if defined(OLED_ENABLE) || defined(RGB_MATRIX_ENABLE)
#		define SPLIT_MODS_ENABLE
#		define SPLIT_LED_STATE_ENABLE
#		define SPLIT_LAYER_STATE_ENABLE
#	endif
#endif

#ifdef MOUSEKEY_ENABLE
//#	define MK_KINETIC_SPEED
#	ifdef MK_KINETIC_SPEED
#		define MOUSEKEY_DELAY 0
#		define MOUSEKEY_INTERVAL 16
#		define MOUSEKEY_MOVE_DELTA 4
#		define MOUSEKEY_INITIAL_SPEED 50 // Initial speed of the cursor in pixel per second
#		define MOUSEKEY_BASE_SPEED 1000  // Maximum cursor speed at which acceleration stops
#		define MOUSEKEY_DECELERATED_SPEED 400
#		define MOUSEKEY_ACCELERATED_SPEED 2000
#	else
#		define MOUSEKEY_DELAY 0			// Delay between pressing a key and cursor movement
#		define MOUSEKEY_INTERVAL 16		// Time between cursor movements in milliseconds
#		define MOUSEKEY_MOVE_DELTA 10	// Step size for acceleration
#		define MOUSEKEY_MAX_SPEED 12
#		define MOUSEKEY_TIME_TO_MAX 70
#	endif
#	define MOUSEKEY_WHEEL_DELAY 16
#	define MOUSEKEY_WHEEL_INTERVAL 30
#	define MOUSEKEY_WHEEL_MAX_SPEED 10
#	define MOUSEKEY_WHEEL_TIME_TO_MAX 95
// Kinetic Mode
#	define MOUSEKEY_WHEEL_INITIAL_MOVEMENTS 500
#	define MOUSEKEY_WHEEL_BASE_MOVEMENTS 1000
#	define MOUSEKEY_WHEEL_DECELERATED_MOVEMENTS 400
#	define MOUSEKEY_WHEEL_ACCELERATED_MOVEMENTS 3000
#endif

#ifdef COMBO_ENABLE
#	define COMBO_TERM 20
#	define COMBO_VARIABLE_LEN
#	define COMBO_ONLY_FROM_LAYER 0
#endif

#ifdef AUTO_SHIFT_ENABLE
#	define NO_AUTO_SHIFT_ALPHA
#	define NO_AUTO_SHIFT_NUMERIC
#	define AUTO_SHIFT_TIMEOUT 150
#endif
