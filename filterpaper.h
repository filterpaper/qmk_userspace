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

// Custom dimmer indicator colors and effects
#ifdef RGB_MATRIX_ENABLE
#	define RGB_BSTEEL	60, 90, 128
#	define RGB_DGRAY	50, 70, 90
#	define RGB_MODS		RGB_BSTEEL
#	define RGB_CAPS		RGB_PURPLE
#	define RGB_LAYER	RGB_DGRAY
#if defined(KEYBOARD_bm40hsrgb) && defined(RGB_MATRIX_CUSTOM_USER)
#	define DEF_MODE	RGB_MATRIX_CUSTOM_CANDY_WIDE
#	define CMK_MODE	RGB_MATRIX_CUSTOM_PIXEL_RAIN
#elif defined(KEYBOARD_boardsource_the_mark) && defined(RGB_MATRIX_CUSTOM_USER)
#	define DEF_MODE	RGB_MATRIX_CUSTOM_CANDY_WIDE
#	define CMK_MODE	RGB_MATRIX_CUSTOM_PIXEL_RAIN
#else
#	define DEF_MODE	RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#	define CMK_MODE	RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
#endif
#endif

// Mod and layer tap bitmasks
#define LT_MASK 0x4000
#define CTL_T_MASK 0x6100
#define SFT_T_MASK 0x6200
#define ALT_T_MASK 0x6400
#define GUI_T_MASK 0x6800

/** \brief Key Actions
 * (kc >> 8) & 0x1F) for mod bits
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
