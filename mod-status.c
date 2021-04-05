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

/* Graphical active layer and modifier status display. Module can be
   rendered on primary OLED or without layer state on secondary.

   Modified from @soundmonster's graphical status code
   (keyboards/crkbd/keymaps/soundmonster)
*/

#include "filterpaper.h"


static void render_logo(void) {
	static char const corne_logo[] PROGMEM = {
		0x80, 0x81, 0x82, 0x83, 0x84,
		0xa0, 0xa1, 0xa2, 0xa3, 0xa4,
		0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0};
	static char const katakana[] PROGMEM = {
		0x20, 0xd1, 0xd2, 0xd3, 0x20, 0};

	oled_write_P(corne_logo, false);
#ifndef SPLIT_MODS_ENABLE
	if (layer_state_is(CMK)) { oled_write_P(katakana, false); }
	else { oled_write_P(PSTR("corne"), false); }
#else
	oled_write_P(katakana, false);
#endif
}


#ifndef SPLIT_MODS_ENABLE
// Graphical layer display
static void render_layer_state(void) {
	static char const default_layer[] PROGMEM = {
		0x20, 0x94, 0x95, 0x96, 0x20,
		0x20, 0xb4, 0xb5, 0xb6, 0x20,
		0x20, 0xd4, 0xd5, 0xd6, 0x20, 0};
	static char const lower_layer[] PROGMEM = {
		0x20, 0x9a, 0x9b, 0x9c, 0x20,
		0x20, 0xba, 0xbb, 0xbc, 0x20,
		0x20, 0xda, 0xdb, 0xdc, 0x20, 0};
	static char const raise_layer[] PROGMEM = {
		0x20, 0x97, 0x98, 0x99, 0x20,
		0x20, 0xb7, 0xb8, 0xb9, 0x20,
		0x20, 0xd7, 0xd8, 0xd9, 0x20, 0};
	static char const adjust_layer[] PROGMEM = {
		0x20, 0x9d, 0x9e, 0x9f, 0x20,
		0x20, 0xbd, 0xbe, 0xbf, 0x20,
		0x20, 0xdd, 0xde, 0xdf, 0x20, 0};

/*	static char const *layer[] = {
		default_layer,
		default_layer,
		lower_layer,
		raise_layer,
		adjust_layer
	};
	oled_write_P(layer[get_highest_layer(layer_state)], false); */

	if (layer_state_is(ADJ)) { oled_write_P(adjust_layer, false); }
	else if (layer_state_is(RSE)) { oled_write_P(raise_layer, false); }
	else if (layer_state_is(LWR)) { oled_write_P(lower_layer, false); }
	else { oled_write_P(default_layer, false); }
}
#endif


static void render_mod_status_gui_alt(uint_fast8_t const mods) {
	static char const gui_off_1[] PROGMEM = {0x85, 0x86, 0};
	static char const gui_off_2[] PROGMEM = {0xa5, 0xa6, 0};
	static char const gui_on_1[] PROGMEM = {0x8d, 0x8e, 0};
	static char const gui_on_2[] PROGMEM = {0xad, 0xae, 0};

	static char const alt_off_1[] PROGMEM = {0x87, 0x88, 0};
	static char const alt_off_2[] PROGMEM = {0xa7, 0xa8, 0};
	static char const alt_on_1[] PROGMEM = {0x8f, 0x90, 0};
	static char const alt_on_2[] PROGMEM = {0xaf, 0xb0, 0};

	// fillers between the modifier icons bleed into the icon frames
	static char const off_off_1[] PROGMEM = {0xc5, 0};
	static char const off_off_2[] PROGMEM = {0xc6, 0};
	static char const on_off_1[] PROGMEM = {0xc7, 0};
	static char const on_off_2[] PROGMEM = {0xc8, 0};
	static char const off_on_1[] PROGMEM = {0xc9, 0};
	static char const off_on_2[] PROGMEM = {0xca, 0};
	static char const on_on_1[] PROGMEM = {0xcb, 0};
	static char const on_on_2[] PROGMEM = {0xcc, 0};

	if (mods & MOD_MASK_GUI) { oled_write_P(gui_on_1, false); }
	else { oled_write_P(gui_off_1, false); }

	if (mods & MOD_MASK_GUI && mods & MOD_MASK_ALT) { oled_write_P(on_on_1, false); }
	else if (mods & MOD_MASK_GUI) { oled_write_P(on_off_1, false); }
	else if (mods & MOD_MASK_ALT) { oled_write_P(off_on_1, false); }
	else { oled_write_P(off_off_1, false); }

	if (mods & MOD_MASK_ALT) { oled_write_P(alt_on_1, false); }
	else { oled_write_P(alt_off_1, false); }

	if (mods & MOD_MASK_GUI) { oled_write_P(gui_on_2, false); }
	else { oled_write_P(gui_off_2, false); }

	if (mods & MOD_MASK_GUI & MOD_MASK_ALT) { oled_write_P(on_on_2, false); }
	else if (mods & MOD_MASK_GUI) { oled_write_P(on_off_2, false); }
	else if (mods & MOD_MASK_ALT) { oled_write_P(off_on_2, false); }
	else { oled_write_P(off_off_2, false); }

	if (mods & MOD_MASK_ALT) { oled_write_P(alt_on_2, false); }
	else { oled_write_P(alt_off_2, false); }
}


static void render_mod_status_ctrl_shift(uint_fast8_t const mods, bool const caps) {
	static char const ctrl_off_1[] PROGMEM = {0x89, 0x8a, 0};
	static char const ctrl_off_2[] PROGMEM = {0xa9, 0xaa, 0};
	static char const ctrl_on_1[] PROGMEM = {0x91, 0x92, 0};
	static char const ctrl_on_2[] PROGMEM = {0xb1, 0xb2, 0};

	static char const shift_off_1[] PROGMEM = {0x8b, 0x8c, 0};
	static char const shift_off_2[] PROGMEM = {0xab, 0xac, 0};
	static char const shift_on_1[] PROGMEM = {0xcd, 0xce, 0};
	static char const shift_on_2[] PROGMEM = {0xcf, 0xd0, 0};

	// fillers between the modifier icons bleed into the icon frames
	static char const off_off_1[] PROGMEM = {0xc5, 0};
	static char const off_off_2[] PROGMEM = {0xc6, 0};
	static char const on_off_1[] PROGMEM = {0xc7, 0};
	static char const on_off_2[] PROGMEM = {0xc8, 0};
	static char const off_on_1[] PROGMEM = {0xc9, 0};
	static char const off_on_2[] PROGMEM = {0xca, 0};
	static char const on_on_1[] PROGMEM = {0xcb, 0};
	static char const on_on_2[] PROGMEM = {0xcc, 0};

	if (mods & MOD_MASK_CTRL) { oled_write_P(ctrl_on_1, false); }
	else { oled_write_P(ctrl_off_1, false); }

	if (mods & MOD_MASK_CTRL && (mods & MOD_MASK_SHIFT || caps)) { oled_write_P(on_on_1, false); }
	else if (mods & MOD_MASK_CTRL) { oled_write_P(on_off_1, false); }
	else if (mods & MOD_MASK_SHIFT || caps) { oled_write_P(off_on_1, false); }
	else { oled_write_P(off_off_1, false); }

	if (mods & MOD_MASK_SHIFT || caps) { oled_write_P(shift_on_1, false); }
	else { oled_write_P(shift_off_1, false); }

	if (mods & MOD_MASK_CTRL) { oled_write_P(ctrl_on_2, false); }
	else { oled_write_P(ctrl_off_2, false); }

	if (mods & MOD_MASK_CTRL && (mods & MOD_MASK_SHIFT || caps)) { oled_write_P(on_on_2, false); }
	else if (mods & MOD_MASK_CTRL) { oled_write_P(on_off_2, false); }
	else if (mods & MOD_MASK_SHIFT || caps) { oled_write_P(off_on_2, false); }
	else { oled_write_P(off_off_2, false); }

	if (mods & MOD_MASK_SHIFT || caps) { oled_write_P(shift_on_2, false); }
	else { oled_write_P(shift_off_2, false); }
}

// Primary modifier status display function
static void render_mod_status(void) {
	render_logo();
	oled_set_cursor(0,6);
#ifndef SPLIT_MODS_ENABLE
	render_layer_state();
	oled_set_cursor(0,11);
#endif
	render_mod_status_gui_alt(get_mods()|get_oneshot_mods());
	render_mod_status_ctrl_shift(get_mods()|get_oneshot_mods(),host_keyboard_led_state().caps_lock);
}


// Init and rendering calls
oled_rotation_t oled_init_user(oled_rotation_t const rotation) {
#ifndef SPLIT_MODS_ENABLE
	if (is_keyboard_master())    { return OLED_ROTATION_270; }
#else
	if (!is_keyboard_master())   { return OLED_ROTATION_270; }
#endif
	else if (is_keyboard_left()) { return OLED_ROTATION_0; }
	else                         { return OLED_ROTATION_180; }
}

void oled_task_user(void) {
#ifndef SPLIT_MODS_ENABLE
	if (is_keyboard_master()) { render_mod_status(); }
	else                      { render_bongocat(); }
#else
	if (is_keyboard_master()) { render_bongocat();  }
	else                      { render_mod_status(); }
#endif
}
