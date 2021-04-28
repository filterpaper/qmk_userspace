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

#define TIMER_TICK (timer_read32() >> 4) // every 10 is half second
#define RGB_TICK ((g_rgb_timer * (1 + (rgb_matrix_config.speed >> 5))) >> 8)
#define RGB_EFFECT_TICK (scale16by8(g_rgb_timer, add8(rgb_matrix_config.speed >> 5, 1)))
//uint16_t time = scale16by8(g_rgb_timer, rgb_matrix_config.speed / 4);
//uint16_t tick = scale16by8(g_rgb_timer, add8(rgb_matrix_config.speed >> 5, 1));


RGB_MATRIX_EFFECT(PIXEL_RAIN)
RGB_MATRIX_EFFECT(PIXEL_SHIFT)


#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
uint8_t jsf8(void) {
	static uint8_t a = 161, b = 62, c = 21, d = 97, t;
	t = a - ((b << 1)|(b >> 7));
	a = b ^ ((c << 4)|(c >> 4));
	b = c + d; c = d + t;
	return d = t + a;
}

uint16_t rand16seed = 0x5eed;

static bool PIXEL_RAIN(effect_params_t* params) {
	static uint32_t timer = 0;
	uint16_t speed = 500 / scale16by8(qadd8(rgb_matrix_config.speed, 16), 16);
	if (timer > g_rgb_timer) { return false; }

	bool set_rgb(uint8_t i, effect_params_t* params, bool off) {
		if (!HAS_ANY_FLAGS(g_led_config.flags[i], params->flags)) { return false; }
		if (off) {
			rgb_matrix_set_color(i, RGB_OFF);
		} else {
			HSV hsv = {random8(), qadd8(random8() >> 1, 127), rgb_matrix_config.hsv.v};
			RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
			rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
		}
		timer = g_rgb_timer + speed;
		return false;
	}

	if (params->init) { random16_set_seed((uint16_t)g_rgb_timer); }
	return set_rgb(mod8(random8(), DRIVER_LED_TOTAL), params, random8() & 3);
}

static bool PIXEL_SHIFT(effect_params_t* params) {
	static RGB led[DRIVER_LED_TOTAL];
	static uint32_t timer = 0;
	uint16_t speed = 1500 / scale16by8(qadd8(rgb_matrix_config.speed, 16), 16);
	if (timer > g_rgb_timer) { return false; }

	void set_rgb(uint8_t i) {
		led[i] = (random8() & 3) ? (RGB){0,0,0} : rgb_matrix_hsv_to_rgb((HSV){random8(), qadd8(random8() >> 1, 127), rgb_matrix_config.hsv.v});
	}
	if (params->init) {
		random16_set_seed((uint16_t)g_rgb_timer);
		for (uint8_t k=0; k < DRIVER_LED_TOTAL; ++k) { set_rgb(k); }
	}

	RGB_MATRIX_USE_LIMITS(led_min, led_max);
	if (led_max == DRIVER_LED_TOTAL) {
		for (uint8_t j=0; j < DRIVER_LED_TOTAL-1; ++j) { led[j] = led[j+1]; }
		set_rgb(DRIVER_LED_TOTAL-1);
		timer = g_rgb_timer + speed;
	}
	for (uint8_t i=led_min; i < led_max; ++i) {
		RGB_MATRIX_TEST_LED_FLAGS();
		rgb_matrix_set_color(i, led[i].r, led[i].g, led[i].b);
	}
	return led_max < DRIVER_LED_TOTAL;
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
