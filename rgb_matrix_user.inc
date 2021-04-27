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

RGB_MATRIX_EFFECT(PIXEL_RAIN)
RGB_MATRIX_EFFECT(FRACTAL)


#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static bool PIXEL_RAIN(effect_params_t* params) {
	//extern uint8_t jsf8(void);
	uint8_t jsf8(void) {
		static uint8_t a = 161, b = 62, c = 21, d = 97, t;
		t = a - ((b << 1)|(b >> 7));
		a = b ^ ((c << 4)|(c >> 4));
		b = c + d; c = d + t;
		return d = t + a;
	}

	void set_rgb(uint8_t i, effect_params_t* params, bool on) {
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
	if (mod8(tick, 5) == 0) { set_rgb(mod8(jsf8(), DRIVER_LED_TOTAL), params, jsf8() & 1); }
	return false;
}

uint32_t f_timer = 0;

uint8_t halfArray[6][4];
uint8_t fullArray[12][4];
uint8_t halfbottom[3];
uint8_t fullbottom[6];
uint8_t displArray[54];

static bool FRACTAL(effect_params_t* params) {
	uint16_t f_speed = (400 / rgb_matrix_config.speed) * 127;

	if (f_timer > g_rgb_timer) return false;

	HSV hsv = { (g_rgb_timer >> 6) & 255, rgb_matrix_config.hsv.s, rgb_matrix_config.hsv.v };
	RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
	RGB_MATRIX_USE_LIMITS(led_min, led_max);
	for (int j = led_min; j < led_max; j++) {
		int i = j > 41 ? j + 1 : j;
		if (displArray[i]) rgb_matrix_set_color(j, rgb.r, rgb.g, rgb.b);
		else rgb_matrix_set_color(j, 0x00, 0x00, 0x00);
	}

	if (led_max == DRIVER_LED_TOTAL) {
		f_timer = g_rgb_timer + f_speed;

		// move columns
		for (uint8_t col = 5; col > 0; col--) {
			for (uint8_t row = 0; row < 4; row++) {
				halfArray[col][row] = halfArray[col-1][row];
			}
		}
		//bottom
		for (uint8_t col = 2; col > 0; col--) {
			halfbottom[col] = halfbottom[col-1];
		}

		// random fill first column
		for (uint8_t row = 0; row < 4; row++) {
			halfArray[0][row] = rand() % 2;
		}
		// bottom
		halfbottom[0] = rand() % 2;

		// transfer to big array
		for (uint8_t col = 0; col < 6; col++) {
			for (uint8_t row = 0; row < 4; row++){
				fullArray[5 - col][row] = halfArray[col][row];
				fullArray[6 + col][row] = halfArray[col][row];
			}
		}
		//bottom
		for (uint8_t col = 0; col < 3; col++){
			fullbottom[2 - col] = halfbottom[col];
			fullbottom[3 + col] = halfbottom[col];
		}

		// transfer to matrix array
		uint8_t k = 0;
		for (uint8_t row = 0; row < 4; row++) {
			for (uint8_t col = 0; col < 12; col++) {
				displArray[k] = fullArray[col][row];
				k++;
			}
		}
		//bottom
		uint8_t l = 48;
		for (uint8_t col = 0; col < 6; col++) {
			displArray[l] = fullbottom[col];
			l++;
		}
	}

	return led_max < DRIVER_LED_TOTAL;
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
