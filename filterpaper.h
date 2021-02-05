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

// Layer number reference
enum layers {
	_QWERTY = 0,
	_COLEMAK,
	_LOWER,
	_RAISE,
	_ADJUST,
};

// Layer colors and effects
#ifdef RGB_MATRIX_ENABLE
#	define RGB_BSTEEL	70, 130, 180
#	define RGB_DEFAULT	RGB_BSTEEL
#	define RGB_MODS		RGB_BSTEEL
#	define RGB_CAPS		RGB_PURPLE
#	define RGB_LAYER	RGB_YELLOW
#	define HSV_DEFAULT	HSV_AZURE
#	define MATRIX_NORMAL	RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#	define MATRIX_SPECIAL	RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
#endif
