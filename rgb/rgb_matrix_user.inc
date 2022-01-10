// Copyright 2021 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

// Timer notes
//#define TIMER_TICK (timer_read32() >> 4) // every 10 is half second
//#define RGB_TICK ((g_rgb_timer * (1 + (rgb_matrix_config.speed >> 5))) >> 8)
//uint16_t tick = scale16by8(g_rgb_timer, add8(rgb_matrix_config.speed, 1) >> 5);

/*
uint8_t jsf8(void) { // Bob Jenkin's fast prng
	static uint8_t a = 161, b = 62, c = 21, d = 97, t;
	t = a - ((b << 1)|(b >> 7));
	a = b ^ ((c << 4)|(c >> 4));
	b = c + d; c = d + t;
	return d = t + a;
}*/

#ifdef RGB_MATRIX_KEYREACTIVE_ENABLED

#ifdef ENABLE_RGB_MATRIX_CANDY_TAP
RGB_MATRIX_EFFECT(CANDY_TAP)
#	ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

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
#	endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // ENABLE_RGB_MATRIX_CANDY_TAP


#ifdef ENABLE_RGB_MATRIX_CANDY_SPLASH
RGB_MATRIX_EFFECT(CANDY_SPLASH)
#	ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
static bool CANDY_SPLASH(effect_params_t* params) {
	HSV CANDY_WIDE_math(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick) {
		uint16_t effect = tick + dist * 5;
		if (effect > 255) effect = 255;
		hsv.v = qadd8(hsv.v, 255 - effect);
		return hsv_by_time(hsv);
	}
	return effect_runner_reactive_splash(0, params, &CANDY_WIDE_math);
}
#	endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // ENABLE_RGB_MATRIX_CANDY_SPLASH

#endif // RGB_MATRIX_KEYREACTIVE_ENABLED



#ifdef ENABLE_RGB_MATRIX_CANDY_RAIN
RGB_MATRIX_EFFECT(CANDY_RAIN) // 278 bytes
#	ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
RGB random_hsv_to_rgb(void) {
	return hsv_to_rgb((HSV){random8(), qadd8(random8() >> 1, 127), rgb_matrix_config.hsv.v});
}

static bool CANDY_RAIN(effect_params_t* params) {
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

	return rain_pixel(mod8(random8(), DRIVER_LED_TOTAL), params, random8() & 2);
}
#	endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // ENABLE_RGB_MATRIX_CANDY_RAIN



#ifdef ENABLE_RGB_MATRIX_CANDY_FRACTAL
RGB_MATRIX_EFFECT(CANDY_FRACTAL)
#	ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
static bool CANDY_FRACTAL(effect_params_t* params) {
	#define MID_COL MATRIX_COLS / 2
	static bool led[MATRIX_ROWS][MATRIX_COLS];

	static uint32_t wait_timer = 0;
	if (wait_timer > g_rgb_timer) { return false; }

	inline uint32_t interval(void) { return 3000 / scale16by8(qadd8(rgb_matrix_config.speed, 16), 16); }

	RGB rgb = rgb_matrix_hsv_to_rgb(rgb_matrix_config.hsv);
	for (uint8_t h = 0; h < MATRIX_ROWS; ++h) {
		for (uint8_t l = 0; l < MID_COL-1; ++l) {
			if (led[h][l]) { rgb_matrix_set_color(g_led_config.matrix_co[h][l], rgb.r, rgb.g, rgb.b); }
			else { rgb_matrix_set_color(g_led_config.matrix_co[h][l], 0, 0, 0); }
			led[h][l] = led[h][l+1];
		}
		for (uint8_t r = MATRIX_COLS-1; r > MID_COL; --r) {
			if (led[h][r]) { rgb_matrix_set_color(g_led_config.matrix_co[h][r], rgb.r, rgb.g, rgb.b); }
			else { rgb_matrix_set_color(g_led_config.matrix_co[h][r], 0, 0, 0); }
			led[h][r] = led[h][r-1];
		}
		if (led[h][MID_COL]) { rgb_matrix_set_color(g_led_config.matrix_co[h][MID_COL], rgb.r, rgb.g, rgb.b); }
		else { rgb_matrix_set_color(g_led_config.matrix_co[h][MID_COL], 0, 0, 0); }
		if (led[h][MID_COL-1]) { rgb_matrix_set_color(g_led_config.matrix_co[h][MID_COL-1], rgb.r, rgb.g, rgb.b); }
		else { rgb_matrix_set_color(g_led_config.matrix_co[h][MID_COL-1], 0, 0, 0); }
		led[h][MID_COL] = led[h][MID_COL-1] = (random8() & 3) ? false : true;
	}

	wait_timer = g_rgb_timer + interval();
	return false;
}
#	endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // ENABLE_RGB_MATRIX_RACTAL



#ifdef ENABLE_RGB_MATRIX_CANDY_FLOW
RGB_MATRIX_EFFECT(CANDY_FLOW) // 390 bytes

#	ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
static bool CANDY_FLOW(effect_params_t* params) {
	// LED state array
	static RGB led[DRIVER_LED_TOTAL];

	static uint32_t wait_timer = 0;
	if (wait_timer > g_rgb_timer) {
		return false;
	}

	inline uint32_t interval(void) {
		return 3000 / scale16by8(qadd8(rgb_matrix_config.speed, 16), 16);
	}

	if (params->init) {
		rgb_matrix_set_color_all(0, 0, 0);
	}

	RGB_MATRIX_USE_LIMITS(led_min, led_max);
	// Light LEDs based on state array
	for (uint8_t i = led_min; i < led_max; ++i) {
		RGB_MATRIX_TEST_LED_FLAGS();
		rgb_matrix_set_color(i, led[i].r, led[i].g, led[i].b);
	}

	if (led_max == DRIVER_LED_TOTAL) {
		for (uint8_t j = 0; j < led_max-1; ++j) {
			led[j] = led[j+1];  // Shift LED state forward
		}
		// Fill last LED
		led[led_max-1] = (random8() & 3) ? (RGB){0,0,0} : hsv_to_rgb((HSV){random8(), qadd8(random8() >> 1, 127), rgb_matrix_config.hsv.v});
		// Set pulse timer
		wait_timer = g_rgb_timer + interval();
	}

	return led_max < DRIVER_LED_TOTAL;
}
#	endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // ENABLE_RGB_MATRIX_CANDY_FLOW
