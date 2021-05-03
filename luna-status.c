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

/* Graphical Luna dog animation display, requires WPM_ENABLE.
   Luna is 4x3 on OLED and its frames are oriented for
   vertical rendering (OLED_ROTATION_270).

   Modified from @HellSingCoder's Luna dog
   (https://github.com/HellSingCoder/qmk_firmware/tree/master/keyboards/sofle/keymaps/HellSingCoder)

   Includes white Felix dog frames from @ItsWaffIe
   (https://github.com/ItsWaffIe/waffle_corne/blob/proton-c/firmware/oled.c)
*/

#include "filterpaper.h"

#define LUNA_FRAMES 2
#define LUNA_FRAME_DURATION 200 // Number of ms between frames
#define LUNA_SIZE 96 // 96-byte arrays for the little dog

static uint8_t current_frame = 0;


static void render_logo(void) {
	static char const corne_logo[] PROGMEM = {
		0x80, 0x81, 0x82, 0x83, 0x84,
		0xa0, 0xa1, 0xa2, 0xa3, 0xa4,
		0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0};
	static char const corne[] PROGMEM = {
		0x20, 0xd1, 0xd2, 0xd3, 0x20, 0};

	oled_write_P(corne_logo, false);
	layer_state_is(CMK) ? oled_write_P(corne, false) : oled_write_P(PSTR("corne"), false);
}


static void render_luna_sit(void) {
	static char const sit[LUNA_FRAMES][LUNA_SIZE] PROGMEM = { {
#ifndef FELIX
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0x1c,
		0x02,0x05,0x02,0x24,0x04,0x04,0x02,0xa9,0x1e,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0x10,0x08,0x68,0x10,0x08,0x04,0x03,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x02,0x06,0x82,0x7c,0x03,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x04,0x0c,0x10,0x10,0x20,0x20,0x20,0x28,
		0x3e,0x1c,0x20,0x20,0x3e,0x0f,0x11,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	}, {
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0x1c,
		0x02,0x05,0x02,0x24,0x04,0x04,0x02,0xa9,0x1e,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0xe0,0x90,0x08,0x18,0x60,0x10,0x08,0x04,0x03,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x02,0x0e,0x82,0x7c,0x03,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x04,0x0c,0x10,0x10,0x20,0x20,0x20,0x28,
		0x3e,0x1c,0x20,0x20,0x3e,0x0f,0x11,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
#else
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0xfc,
		0xfe,0xfd,0xfe,0xdc,0xfc,0xfc,0xfe,0x5d,0xfe,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0xf0,0xf8,0x1c,0xf0,0xf8,0xfc,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xfd,0xf9,0xfd,0x7f,0x03,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x0f,0x1f,0x1f,0x3f,0x3f,0x3f,0x37,
		0x21,0x13,0x3f,0x3f,0x3f,0x0f,0x11,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	}, {
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0xfe,
		0xfd,0xfe,0xfc,0xdc,0xfc,0xfe,0xfd,0x5e,0xfc,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0xe0,0xf0,0xf8,0xfc,0x80,0xf0,0xf8,0xfc,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xfd,0xf1,0xfd,0x7f,0x03,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x0f,0x1f,0x1f,0x3f,0x3f,0x3f,0x37,
		0x21,0x13,0x3f,0x3f,0x3f,0x0f,0x11,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
#endif
	} };

//	current_frame = (current_frame + 1) % LUNA_FRAMES;
	current_frame = (current_frame + 1) & 1; // Faster and smaller than % 2
	oled_write_raw_P(sit[current_frame], LUNA_SIZE);
}


static void render_luna_walk(void) {
	static char const walk[LUNA_FRAMES][LUNA_SIZE] PROGMEM = { {
#ifndef FELIX
		0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x20,0x10,0x90,0x90,0x90,0xa0,0xc0,0x80,0x80,
		0x80,0x70,0x08,0x14,0x08,0x90,0x10,0x10,0x08,0xa4,0x78,0x80,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x07,0x08,0xfc,0x01,0x00,0x00,0x00,0x00,0x80,0x00,0x00,
		0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x18,0xea,0x10,0x0f,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x1c,0x20,0x20,0x3c,0x0f,0x11,0x1f,0x03,
		0x06,0x18,0x20,0x20,0x3c,0x0c,0x12,0x1e,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	}, {
		0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x20,0x20,0x20,0x40,0x80,0x00,0x00,0x00,
		0x00,0xe0,0x10,0x28,0x10,0x20,0x20,0x20,0x10,0x48,0xf0,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x1f,0x20,0xf8,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
		0x03,0x00,0x00,0x00,0x00,0x01,0x00,0x10,0x30,0xd5,0x20,0x1f,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x20,0x30,0x0c,0x02,0x05,0x09,0x12,0x1e,
		0x02,0x1c,0x14,0x08,0x10,0x20,0x2c,0x32,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00
#else
		0x00,0x00,0x00,0x00,0x00,0x80,0xc0,0xe0,0xf0,0xf0,0x70,0x30,0x20,0x00,0x80,0xc0,
		0xc0,0xf0,0xf8,0xf4,0xf8,0x70,0xf0,0xf0,0xf8,0x74,0xf8,0x80,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x07,0x0f,0xff,0xfd,0xfe,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf7,0xe7,0xf5,0x1f,0x0f,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x1f,0x3f,0x3f,0x3f,0x0f,0x11,0x1f,0x1f,
		0x03,0x07,0x1f,0x3f,0x3f,0x0f,0x13,0x1f,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	}, {
		0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xc0,0xe0,0xe0,0xe0,0xc0,0x80,0x00,0x00,0x00,
		0x00,0xe0,0xf0,0xe8,0xf0,0xe0,0xe0,0xe0,0xf0,0xe8,0xf0,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x1f,0x3f,0xfb,0xfd,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,
		0xff,0xff,0xff,0xff,0xff,0xfe,0xff,0xef,0xcf,0xea,0x3f,0x1f,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x3f,0x3f,0x0f,0x03,0x07,0x0f,0x1f,0x03,
		0x1f,0x1f,0x17,0x0f,0x1f,0x3f,0x3f,0x33,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00
#endif
	} };

//	current_frame = (current_frame + 1) % LUNA_FRAMES;
	current_frame = (current_frame + 1) & 1; // Faster and smaller than % 2
	oled_write_raw_P(walk[current_frame], LUNA_SIZE);
}


static void render_luna_run(void) {
	static char const run[LUNA_FRAMES][LUNA_SIZE] PROGMEM = { {
#ifndef FELIX
		0x00,0x00,0x00,0x00,0xe0,0x10,0x08,0x08,0xc8,0xb0,0x80,0x80,0x80,0x80,0x80,0x80,
		0x80,0x40,0x40,0x3c,0x14,0x04,0x08,0x90,0x18,0x04,0x08,0xb0,0x40,0x80,0x00,0x00,
		0x00,0x00,0x00,0x00,0x01,0x02,0xc4,0xa4,0xfc,0x00,0x00,0x00,0x00,0x80,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xc8,0x58,0x28,0x2a,0x10,0x0f,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x0e,0x09,0x04,0x04,0x04,0x04,0x02,0x03,0x02,0x01,0x01,
		0x02,0x02,0x04,0x08,0x10,0x26,0x2b,0x32,0x04,0x05,0x06,0x00,0x00,0x00,0x00,0x00
	}, {
		0x00,0x00,0x00,0xe0,0x10,0x10,0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,
		0x80,0x80,0x78,0x28,0x08,0x10,0x20,0x30,0x08,0x10,0x20,0x40,0x80,0x00,0x00,0x00,
		0x00,0x00,0x00,0x03,0x04,0x08,0x10,0x11,0xf9,0x01,0x01,0x01,0x01,0x01,0x01,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x10,0xb0,0x50,0x55,0x20,0x1f,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x0c,0x10,0x20,0x28,0x37,
		0x02,0x1e,0x20,0x20,0x18,0x0c,0x14,0x1e,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00
#else
		0x00,0x00,0x00,0x00,0xe0,0xf0,0xf8,0xf8,0xf8,0xb0,0xa0,0x80,0x80,0x80,0x80,0x80,
		0x80,0xc0,0xc0,0xfc,0xf4,0xe4,0xf8,0x70,0xf8,0xe4,0xf8,0x70,0xc0,0x80,0x00,0x00,
		0x00,0x00,0x00,0x00,0x01,0x63,0xe7,0xe7,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf7,0x67,0x37,0x35,0x1f,0x0f,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x0e,0x0f,0x07,0x07,0x07,0x07,0x03,0x03,0x03,0x01,0x01,
		0x03,0x03,0x07,0x0f,0x1f,0x3f,0x3b,0x33,0x07,0x07,0x06,0x00,0x00,0x00,0x00,0x00
	}, {
		0x00,0x00,0x00,0xe0,0xf0,0xf0,0xf8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,
		0x80,0x80,0xf8,0xe8,0xc8,0xf0,0xe0,0xf0,0xc8,0xf0,0xe0,0xc0,0x80,0x00,0x00,0x00,
		0x00,0x00,0x00,0x03,0x07,0x0f,0x1f,0x1f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0xef,0xcf,0x6f,0x6a,0x3f,0x1f,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x0f,0x1f,0x3f,0x3f,0x37,
		0x03,0x1f,0x3f,0x3f,0x1f,0x0f,0x1f,0x3f,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00
#endif
	} };

//	current_frame = (current_frame + 1) % LUNA_FRAMES;
	current_frame = (current_frame + 1) & 1; // Faster and smaller than % 2
	oled_write_raw_P(run[current_frame], LUNA_SIZE);
}


static void render_luna_bark(void) {
	static char const bark[LUNA_FRAMES][LUNA_SIZE] PROGMEM = { {
#ifndef FELIX
		0x00,0xc0,0x20,0x10,0xd0,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x40,
		0x3c,0x14,0x04,0x08,0x90,0x18,0x04,0x08,0xb0,0x40,0x80,0x00,0x00,0x00,0x00,0x00,
		0x00,0x03,0x04,0x08,0x10,0x11,0xf9,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x80,0xc8,0x48,0x28,0x2a,0x10,0x0f,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x0c,0x10,0x20,0x28,0x37,0x02,0x02,
		0x04,0x08,0x10,0x26,0x2b,0x32,0x04,0x05,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	}, {
		0x00,0xe0,0x10,0x10,0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x40,
		0x40,0x2c,0x14,0x04,0x08,0x90,0x18,0x04,0x08,0xb0,0x40,0x80,0x00,0x00,0x00,0x00,
		0x00,0x03,0x04,0x08,0x10,0x11,0xf9,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x80,0xc0,0x48,0x28,0x2a,0x10,0x0f,0x20,0x4a,0x09,0x10,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x0c,0x10,0x20,0x28,0x37,0x02,0x02,
		0x04,0x08,0x10,0x26,0x2b,0x32,0x04,0x05,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00
#else
		0x00,0xc0,0xe0,0xf0,0xf0,0x30,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0xc0,
		0xfc,0xf4,0xe4,0xf8,0x70,0xf8,0xe4,0xf8,0x70,0xc0,0x80,0x00,0x00,0x00,0x00,0x00,
		0x00,0x03,0x07,0x0f,0x1f,0x1f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xf7,0xf7,0x37,0x35,0x1f,0x0f,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x0f,0x1f,0x3f,0x3f,0x37,0x03,0x03,
		0x07,0x0f,0x1f,0x3f,0x3b,0x33,0x07,0x07,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	}, {
		0x00,0xe0,0xf0,0xf0,0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0xc0,
		0xc0,0xec,0xf4,0xe4,0xf8,0x70,0xf8,0xe4,0xf8,0x70,0xc0,0x80,0x00,0x00,0x00,0x00,
		0x00,0x03,0x07,0x0f,0x1f,0x1f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x77,0x37,0x35,0x1f,0x0f,0x20,0x4a,0x09,0x10,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x0f,0x1f,0x3f,0x3f,0x37,0x03,0x03,
		0x07,0x0f,0x1f,0x3f,0x3b,0x33,0x07,0x07,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00
#endif
	} };

//	current_frame = (current_frame + 1) % LUNA_FRAMES;
	current_frame = (current_frame + 1) & 1; // Faster and smaller than % 2
	oled_write_raw_P(bark[current_frame], LUNA_SIZE);
}


static void render_luna_sneak(void) {
	static char const sneak[LUNA_FRAMES][LUNA_SIZE] PROGMEM = { {
#ifndef FELIX
		0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x40,0x40,0x40,0x80,0x00,0x00,0x00,0x00,
		0x00,0x00,0xc0,0x40,0x40,0x80,0x00,0x80,0x40,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x1e,0x21,0xf0,0x04,0x02,0x02,0x02,0x02,0x03,0x02,0x02,0x04,
		0x04,0x04,0x03,0x01,0x00,0x00,0x09,0x01,0x80,0x80,0xab,0x04,0xf8,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x1c,0x20,0x20,0x3c,0x0f,0x11,0x1f,0x02,0x06,
		0x18,0x20,0x20,0x38,0x08,0x10,0x18,0x04,0x04,0x02,0x02,0x01,0x00,0x00,0x00,0x00
	}, {
		0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x40,0x40,0x80,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0xe0,0xa0,0x20,0x40,0x80,0xc0,0x20,0x40,0x80,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x3e,0x41,0xf0,0x04,0x02,0x02,0x02,0x03,0x02,0x02,0x02,0x04,
		0x04,0x02,0x01,0x00,0x00,0x00,0x04,0x00,0x40,0x40,0x55,0x82,0x7c,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x20,0x30,0x0c,0x02,0x05,0x09,0x12,0x1e,0x04,
		0x18,0x10,0x08,0x10,0x20,0x28,0x34,0x06,0x02,0x01,0x01,0x00,0x00,0x00,0x00,0x00
#else
		0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xc0,0xc0,0xc0,0xc0,0x80,0x00,0x00,0x00,0x00,
		0x00,0x00,0xc0,0x40,0x40,0x80,0x00,0x80,0x40,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x1e,0x3f,0xff,0xfb,0xfd,0xfd,0xfd,0xfd,0xfd,0xfc,0xfc,0xfc,
		0xfc,0xfe,0xff,0xff,0xfe,0xff,0xf7,0xff,0x7e,0x7f,0x57,0xfc,0xf8,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x1f,0x3f,0x3f,0x3f,0x0f,0x1f,0x1f,0x03,0x07,
		0x1f,0x3f,0x3f,0x3f,0x0f,0x1f,0x1f,0x07,0x07,0x03,0x03,0x01,0x00,0x00,0x00,0x00
	}, {
		0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xc0,0xc0,0xc0,0x80,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0xe0,0xa0,0x20,0xc0,0x80,0xc0,0x20,0xc0,0x80,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x3e,0x7f,0xf7,0xfb,0xfd,0xfd,0xfd,0xfd,0xfc,0xfc,0xfc,0xfc,
		0xfc,0xfe,0xff,0xff,0xff,0xff,0xfb,0xff,0xbf,0xbf,0xab,0xfe,0x7c,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x3f,0x3f,0x0f,0x03,0x07,0x0f,0x1f,0x1f,0x07,
		0x1f,0x1f,0x0f,0x1f,0x3f,0x3f,0x37,0x07,0x03,0x01,0x01,0x00,0x00,0x00,0x00,0x00
#endif
	} };

//	current_frame = (current_frame + 1) % LUNA_FRAMES;
	current_frame = (current_frame + 1) & 1; // Faster and smaller than % 2
	oled_write_raw_P(sneak[current_frame], LUNA_SIZE);
}


static void render_luna_status(void) {
	static uint8_t prev_wpm = 0;
	static uint32_t tap_timer = 0; // WPM and mod triggered
	if (get_current_wpm() > prev_wpm || get_mods()) { tap_timer = timer_read32(); }
	prev_wpm = get_current_wpm();

	// Elapsed time between key presses
	uint32_t keystroke = timer_elapsed32(tap_timer);
	static uint16_t anim_timer = 0;

	void animation_phase(void) {
		bool const caps = host_keyboard_led_state().caps_lock;

		render_logo();
		oled_set_cursor(0,8);
		if (get_mods() & MOD_MASK_SHIFT || caps) {
			render_luna_bark();
			set_current_wpm(0);
		}
		else if (get_mods() & MOD_MASK_CAG) {
			render_luna_sneak();
			set_current_wpm(0);
		}
		else if (prev_wpm && keystroke < LUNA_FRAME_DURATION*2) { render_luna_run(); }
		else if (prev_wpm && keystroke < LUNA_FRAME_DURATION*8) { render_luna_walk(); }
		else { render_luna_sit(); }
	}

	if (keystroke > OLED_TIMEOUT) { oled_off(); }
	else if (timer_elapsed(anim_timer) > LUNA_FRAME_DURATION) {
		anim_timer = timer_read();
		animation_phase();
	}
}


// Init and rendering calls
oled_rotation_t oled_init_user(oled_rotation_t const rotation) {
	return OLED_ROTATION_270;
}

void oled_task_user(void) {
	extern void render_bongocat(void);
	is_keyboard_master() ? render_luna_status() : render_bongocat();
}
