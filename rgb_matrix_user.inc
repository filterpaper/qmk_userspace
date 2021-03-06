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

// Timer notes
//#define TIMER_TICK (timer_read32() >> 4) // every 10 is half second
//#define RGB_TICK ((g_rgb_timer * (1 + (rgb_matrix_config.speed >> 5))) >> 8)
//uint16_t tick = scale16by8(g_rgb_timer, add8(rgb_matrix_config.speed, 1) >> 5);


#ifdef RGB_MATRIX_KEYREACTIVE_ENABLED

#ifndef DISABLE_RGB_MATRIX_CANDY_TAP
RGB_MATRIX_EFFECT(CANDY_TAP)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
uint8_t jsf8(void) { // Bob Jenkin's fast prng
	static uint8_t a = 161, b = 62, c = 21, d = 97, t;
	t = a - ((b << 1)|(b >> 7));
	a = b ^ ((c << 4)|(c >> 4));
	b = c + d; c = d + t;
	return d = t + a;
}

HSV hsv_by_time(HSV hsv) {
	return (HSV){
		scale16by8(g_rgb_timer, add8(rgb_matrix_config.speed, 1) >> 6) & 255,
		qadd8(scale16by8(g_rgb_timer, add8(rgb_matrix_config.speed, 1) >> 4) & 127, 127),
		hsv.v };
}

static bool CANDY_TAP(effect_params_t* params) {
	HSV CANDY_TAP_math(HSV hsv, uint16_t offset) {
		hsv.v = scale8(255 - offset, hsv.v);
		return hsv_by_time(hsv);
	}
	return effect_runner_reactive(params, &CANDY_TAP_math);
}
#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // DISABLE_RGB_MATRIX_CANDY_TAP


#ifndef DISABLE_RGB_MATRIX_CANDY_SPLASH
RGB_MATRIX_EFFECT(CANDY_SPLASH)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
static bool CANDY_SPLASH(effect_params_t* params) {
	HSV CANDY_WIDE_math(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick) {
		uint16_t effect = tick + dist * 5;
		if (effect > 255) effect = 255;
		hsv.v = qadd8(hsv.v, 255 - effect);
		return hsv_by_time(hsv);
	}
	return effect_runner_reactive_splash(0, params, &CANDY_WIDE_math);
}
#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // DISABLE_RGB_MATRIX_CANDY_SPLASH

#endif // RGB_MATRIX_KEYREACTIVE_ENABLED


#ifndef DISABLE_RGB_MATRIX_PIXEL_RAIN
RGB_MATRIX_EFFECT(PIXEL_RAIN) // 278 bytes

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
RGB random_hsv_to_rgb(void) {
	return hsv_to_rgb((HSV){jsf8(), qadd8(jsf8() >> 1, 127), rgb_matrix_config.hsv.v});
}

static bool PIXEL_RAIN(effect_params_t* params) {
	static uint32_t wait_timer = 0;
	if (wait_timer > g_rgb_timer) { return false; }

	inline uint32_t interval(void) { return 500 / scale16by8(qadd8(rgb_matrix_config.speed, 16), 16); }

	bool rain_pixel(uint8_t i, effect_params_t* params, bool off) {
		if (!HAS_ANY_FLAGS(g_led_config.flags[i], params->flags)) { return true; }
		if (off) {
			rgb_matrix_set_color(i, RGB_OFF);
		} else {
			RGB rgb = random_hsv_to_rgb();
			rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
		}
		wait_timer = g_rgb_timer + interval();
		return false;
	}

//	if (params->init) { random16_set_seed((uint16_t)g_rgb_timer); }
	return rain_pixel(mod8(jsf8(), DRIVER_LED_TOTAL), params, jsf8() & 2);
}
#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // DISABLE_RGB_MATRIX_PIXEL_RAIN


#ifndef DISABLE_RGB_MATRIX_PIXEL_FLOW
RGB_MATRIX_EFFECT(PIXEL_FLOW) // 390 bytes

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
static bool PIXEL_FLOW(effect_params_t* params) {
	static RGB led[DRIVER_LED_TOTAL];
	static uint32_t wait_timer = 0;
	if (wait_timer > g_rgb_timer) { return false; }

	inline uint32_t interval(void) { return 1500 / scale16by8(qadd8(rgb_matrix_config.speed, 16), 16); }

	#define FLOW_PIXEL() (jsf8() & 3) ? (RGB){0,0,0} : random_hsv_to_rgb()
//	if (params->init) { //random16_set_seed((uint16_t)g_rgb_timer); }

	RGB_MATRIX_USE_LIMITS(led_min, led_max);
	for (uint8_t i = led_min; i < led_max; ++i) {
		RGB_MATRIX_TEST_LED_FLAGS();
		rgb_matrix_set_color(i, led[i].r, led[i].g, led[i].b);
	}
	if (led_max == DRIVER_LED_TOTAL) {
		for (uint8_t j = 0; j < led_max-1; ++j) { led[j] = led[j+1]; }
		led[led_max-1] = FLOW_PIXEL();
		wait_timer = g_rgb_timer + interval();
	}
	return led_max < DRIVER_LED_TOTAL;
}
#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // DISABLE_RGB_MATRIX_PIXEL_FLOW


#ifndef DISABLE_RGB_MATRIX_PIXEL_FRACTAL
RGB_MATRIX_EFFECT(PIXEL_FRACTAL) // 522 bytes

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
static bool PIXEL_FRACTAL(effect_params_t* params) {
	#define FRACTAL_WIDTH 15
	#define FRACTAL_HEIGHT 5
	#define FRACTAL_MIDDLE 7
	#define FRACTAL_RATIO 16

	typedef struct PACKED { RGB rgb; bool tag; } fractal_led_t;
	typedef struct PACKED { uint8_t x; uint8_t y; } fractal_point_t;
	static fractal_led_t led[FRACTAL_WIDTH][FRACTAL_HEIGHT];
	static uint32_t wait_timer = 0;
	if (wait_timer > g_rgb_timer) { return false; }

	inline uint32_t interval(void) { return 3000 / scale16by8(qadd8(rgb_matrix_config.speed, 16), 16); }

	#define FRACTAL_PIXEL() (jsf8() & 3) ? (RGB){0,0,0} : random_hsv_to_rgb()
	#define GET_FRACTAL_POINT(k) (fractal_point_t){scale8(g_led_config.point[k].x, FRACTAL_RATIO), scale8(g_led_config.point[k].y, FRACTAL_RATIO)}

	RGB_MATRIX_USE_LIMITS(led_min, led_max);
	for (uint8_t i = led_min; i < led_max; ++i) {
		RGB_MATRIX_TEST_LED_FLAGS();
		fractal_point_t map = GET_FRACTAL_POINT(i);
		led[map.x][map.y].tag = true;
		rgb_matrix_set_color(i, led[map.x][map.y].rgb.r, led[map.x][map.y].rgb.g, led[map.x][map.y].rgb.b);
	}
	if (led_max == DRIVER_LED_TOTAL) {
		for (uint8_t h = 0; h < FRACTAL_HEIGHT; ++h) {
			for (uint8_t l = 0; l < FRACTAL_MIDDLE; ++l) {
				led[l][h].rgb = (led[l+1][h].tag) ? led[l+1][h].rgb : led[l+2][h].rgb;
			}
			for (uint8_t r = FRACTAL_WIDTH-1; r > FRACTAL_MIDDLE; --r) {
				led[r][h].rgb = (led[r-1][h].tag) ? led[r-1][h].rgb : led[r-2][h].rgb;
			}
			led[FRACTAL_MIDDLE][h].tag = true;
			led[FRACTAL_MIDDLE][h].rgb = FRACTAL_PIXEL();
		}
		wait_timer = g_rgb_timer + interval();
	}
	return led_max < DRIVER_LED_TOTAL;
}
#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // DISABLE_RGB_MATRIX_PIXEL_FRACTAL
