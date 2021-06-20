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
#include QMK_KEYBOARD_H

// Layer references
enum layers { DEF, CMK, LWR, RSE, ADJ };

// Dimmer RGB colors
#define RGB_BSTEEL	55, 80, 115
#define RGB_DPURPLE	60, 0, 115
#define RGB_DPINK	115, 20, 45
#define RGB_DTEAL	5, 35, 35
#define RGB_CAPS	RGB_DPINK
#define RGB_MODS	RGB_BSTEEL
#define RGB_LAYER	RGB_DPURPLE

// Custom effects
#ifdef RGB_MATRIX_CUSTOM_USER
#	define DEF_MODE	RGB_MATRIX_CUSTOM_CANDY_TAP
#	define CMK_MODE	RGB_MATRIX_CUSTOM_PIXEL_RAIN
#else
#	define DEF_MODE	RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#	define CMK_MODE	RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
#endif

// Custom indicators
#if defined(KEYBOARD_crkbd_rev1) && defined(OLED_DRIVER_ENABLE)
#	define MOD_FLAG LED_FLAG_UNDERGLOW
#	define CAP_FLAG LED_FLAG_UNDERGLOW
#else
#	define MOD_FLAG LED_FLAG_MODIFIER
#	define CAP_FLAG LED_FLAG_KEYLIGHT
#endif

// Layer and mod tap mask bits
// 0xFF00 bitmask
#define LT0_BITS 0x4000
#define LT1_BITS 0x4100
#define LT2_BITS 0x4200
#define LT3_BITS 0x4300
#define CTL_T_BITS 0x6100
#define SFT_T_BITS 0x6200
#define ALT_T_BITS 0x6400
#define GUI_T_BITS 0x6800
// 0xF000 bitmask
#define LT_BITS LT0_BITS
#define LMT_BITS 0x6000
#define RMT_BITS 0x7000

/* (kc >> 8) & 0x1F for mod bits
 *
 * Mod bits:    43210
 *   bit 0      ||||+- Control
 *   bit 1      |||+-- Shift
 *   bit 2      ||+--- Alt
 *   bit 3      |+---- Gui
 *   bit 4      +----- LR flag(Left:0, Right:1)
 */
/*
enum mods_bit {
	MOD_LCTL = 0x01,
	MOD_LSFT = 0x02,
	MOD_LALT = 0x04,
	MOD_LGUI = 0x08,
	MOD_RCTL = 0x11,
	MOD_RSFT = 0x12,
	MOD_RALT = 0x14,
	MOD_RGUI = 0x18,
}; */
