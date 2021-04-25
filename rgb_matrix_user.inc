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



RGB_MATRIX_EFFECT(PIXELATION)
//RGB_MATRIX_EFFECT(REACTIVE_PIXELATION)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#ifndef RAND16_SEED
uint16_t rand16seed = 0x759f;
#endif

static bool PIXELATION(effect_params_t* params) {

	void set_rgb(int i, effect_params_t* params, bool on) {
		if (!HAS_ANY_FLAGS(g_led_config.flags[i], params->flags)) { return; }
		on ? rgb_matrix_set_color(i, random8(), random8(), random8()) : rgb_matrix_set_color(i, RGB_OFF);
	}

	uint16_t tick = scale16by8(g_rgb_timer, qadd8((rgb_matrix_config.speed >> 5), 1));
	if (tick % 10 == 0) { set_rgb(mod8(random8(), DRIVER_LED_TOTAL), params, 1); }
	if (tick % 5 == 0)  { set_rgb(mod8(random8(), DRIVER_LED_TOTAL), params, 0); }
	return false;
}
/*
static bool REACTIVE_PIXELATION(effect_params_t* params) {

	HSV REACTIVE_math(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick) {
		uint16_t effect = tick + dist * 5;
		if (effect > 255) effect = 255;
		hsv.v = qadd8(hsv.v, 255 - effect);
		return hsv;
	}
	HSV SPLASH_math(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick) {
		uint16_t effect = tick - dist;
		if (effect > 255) effect = 255;
		//hsv.h += effect;
		hsv.h += prng();
		hsv.v = qadd8(hsv.v, 255 - effect);
		return hsv;
	}

	bool effect_runner_reactive_splash(uint8_t start, effect_params_t* params, reactive_splash_f effect_func) {
		RGB_MATRIX_USE_LIMITS(led_min, led_max);
		uint8_t count = g_last_hit_tracker.count;
		for (uint8_t i = led_min; i < led_max; i++) {
			RGB_MATRIX_TEST_LED_FLAGS();
			HSV hsv = rgb_matrix_config.hsv;
			hsv.v   = 0;
			for (uint8_t j = start; j < count; j++) {
				int16_t  dx   = g_led_config.point[i].x - g_last_hit_tracker.x[j];
				int16_t  dy   = g_led_config.point[i].y - g_last_hit_tracker.y[j];
				uint8_t  dist = sqrt16(dx * dx + dy * dy);
				uint16_t tick = scale16by8(g_last_hit_tracker.tick[j], rgb_matrix_config.speed);
				hsv           = effect_func(hsv, dx, dy, dist, tick);
			}
			hsv.v   = scale8(hsv.v, rgb_matrix_config.hsv.v);
			RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
			rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
		}
		return led_max < DRIVER_LED_TOTAL;
	}

	//return effect_runner_reactive_splash(qsub8(g_last_hit_tracker.count, 1), params, &REACTIVE_math);
	return effect_runner_reactive_splash(qsub8(g_last_hit_tracker.count, 1), params, &SPLASH_math);
}
*/
#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
