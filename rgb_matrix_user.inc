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



RGB_MATRIX_EFFECT(PIXEL_RAIN)
//RGB_MATRIX_EFFECT(PIXEL_TYPE)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static bool PIXEL_RAIN(effect_params_t* params) {
	extern uint8_t jsf8(void);

	void set_rgb(int i, effect_params_t* params, bool on) {
		if (!HAS_ANY_FLAGS(g_led_config.flags[i], params->flags)) { return; }
		on ? rgb_matrix_set_color(i, jsf8(), jsf8(), jsf8()) : rgb_matrix_set_color(i, RGB_OFF);
	// This retain user's max HSV brightness setting
	//	if (on) {
	//		RGB rgb = rgb_matrix_hsv_to_rgb((HSV){jsf8(), add8(mod8(jsf8(), 125), 130), rgb_matrix_config.hsv.v});
	//		RGB rgb = rgb_matrix_hsv_to_rgb((HSV){scale16by8(g_rgb_timer, (rgb_matrix_config.speed >> 3)), rgb_matrix_config.hsv.s, rgb_matrix_config.hsv.v});
	//		rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
	//	} else { rgb_matrix_set_color(i, RGB_OFF); }
	}

	uint16_t tick = scale16by8(g_rgb_timer, add8((rgb_matrix_config.speed >> 5), 1));
	if (mod8(tick, 10) == 0) { set_rgb(mod8(jsf8(), DRIVER_LED_TOTAL), params, 1); }
	if (mod8(tick,  5) == 0) { set_rgb(mod8(jsf8(), DRIVER_LED_TOTAL), params, 0); }
	return false;
}
/*
static bool PIXEL_TYPE(effect_params_t* params) {

	HSV PIXEL_TYPE_math(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick) {
		uint16_t effect = tick - dist;
		if (effect > 255) effect = 255;
		if (dist > 72) effect = 255;
		if ((dx > 8 || dx < -8) && (dy > 8 || dy < -8)) effect = 255;
		hsv.v = qadd8(hsv.v, 255 - effect);
		hsv.h = rgb_matrix_config.hsv.h + dy / 4;
		//hsv.h = scale16by8(g_rgb_timer, add8((rgb_matrix_config.speed ), 1));
		return hsv;
	}

	// RGB_MATRIX_SOLID_REACTIVE_NEXUS
	return effect_runner_reactive_splash(qsub8(g_last_hit_tracker.count, 1), params, &PIXEL_TYPE_math);
	// MATRIX_SOLID_REACTIVE_MULTINEXUS
	//return effect_runner_reactive_splash(0, params, &PIXEL_TYPE_math);
}*/

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
