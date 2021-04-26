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
#ifdef RGB_MATRIX_KEYREACTIVE_ENABLED
RGB_MATRIX_EFFECT(CANDY_TYPE)
#endif

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static bool PIXEL_RAIN(effect_params_t* params) {
	extern uint8_t jsf8(void);

	void set_rgb(int i, effect_params_t* params, bool on) {
		if (!HAS_ANY_FLAGS(g_led_config.flags[i], params->flags)) { return; }
		if (on) {
			HSV hsv = {jsf8(), add8(jsf8() >> 1, 127), rgb_matrix_config.hsv.v};
			RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
			rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
		} else {
			rgb_matrix_set_color(i, RGB_OFF);
		}
	}

	uint16_t tick = scale16by8(g_rgb_timer, add8(rgb_matrix_config.speed >> 5, 1));
	if (mod8(tick, 10) == 0) { set_rgb(mod8(jsf8(), DRIVER_LED_TOTAL), params, true); }
	if (mod8(tick,  5) == 0) { set_rgb(mod8(jsf8(), DRIVER_LED_TOTAL), params, false); }
	return false;
}

#ifdef RGB_MATRIX_KEYREACTIVE_ENABLED
static bool CANDY_TYPE(effect_params_t* params) {
	RGB_MATRIX_USE_LIMITS(led_min, led_max);
	HSV CANDY_TYPE_math(HSV hsv, uint16_t offset) {
		hsv.v = scale8(255 - offset, hsv.v);
		hsv.h = mod8(scale16by8(g_rgb_timer, add8(rgb_matrix_config.speed >> 5, 1)), 255);
		return hsv;
	}
	return effect_runner_reactive(params, &CANDY_TYPE_math);
}
#endif  // RGB_MATRIX_KEYREACTIVE_ENABLED

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
