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
//#	define DEF_MODE	RGB_MATRIX_SOLID_REACTIVE_SIMPLE
//#	define CMK_MODE	RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
#	define DEF_MODE	RGB_MATRIX_CUSTOM_PIXEL_RAIN
#	define CMK_MODE	RGB_MATRIX_CUSTOM_FRACTAL
#endif

// Mod and layer tap bitmasks
#define LT0_MASK 0x4000
#define LT1_MASK 0x4100
#define LT2_MASK 0x4200
#define LT3_MASK 0x4300
#define CTL_T_MASK 0x6100
#define SFT_T_MASK 0x6200
#define ALT_T_MASK 0x6400
#define GUI_T_MASK 0x6800
