// Copyright 2021 Google LLC
// Copyright 2022 @filterpaper
// SPDX-License-Identifier: Apache-2.0
// Adapted from Pascal Getreuer's Autocorrection
// https://getreuer.info/posts/keyboards/autocorrection

#include QMK_KEYBOARD_H

#include "autocorrect.h"
#include <string.h>
#ifdef __AVR__
#   include "autocorrect_data_avr.h"
#else
#   include "autocorrect_data.h"
#endif

static bool autocorrect_on = true;

void autocorrect_toggle(void) {
    autocorrect_on = !autocorrect_on;
}

bool process_autocorrect(uint16_t keycode, keyrecord_t* record) {
    static uint8_t typo_buffer[DICTIONARY_MAX_LENGTH] = {0};
    static uint8_t buffer_size = 0;

    // Reset with non-Shift modifiers or when feature is off.
    if (get_mods() & ~MOD_MASK_SHIFT || autocorrect_on == false) {
        buffer_size = 0;
        return true;
    }

    // Handle modifiers and quantum keycodes.
    switch(keycode) {
        case KC_LSFT:
        case KC_RSFT:
        case KC_CAPS:
            return true;
        case QK_MOD_TAP ... QK_MOD_TAP_MAX:
        case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
            // Exclude hold keycode
            if (record->tap.count == 0) {
                return true;
            }
    }

    // Handle non-alpha keycodes.
    if (!(KC_A <= (uint8_t)keycode && (uint8_t)keycode <= KC_Z)) {
        if (KC_TAB <= (uint8_t)keycode && (uint8_t)keycode <= KC_SLASH) {
            // Replace punctuation with Space as word boundary.
            keycode = KC_SPC;
        } else if ((uint8_t)keycode == KC_ENT) {
            // Reset buffer and replace Enter with Space as word boundary.
            buffer_size = 0;
            keycode = KC_SPC;
        } else if ((uint8_t)keycode == KC_BSPC && buffer_size > 0) {
            // Subtract buffer for Backspace.
            --buffer_size;
            return true;
        } else {
            // Reset for all other keycodes.
            buffer_size = 0;
            return true;
        }
    }

    // Rotate oldest character if buffer is full.
    if (buffer_size >= DICTIONARY_MAX_LENGTH) {
        memmove(typo_buffer, typo_buffer + 1, DICTIONARY_MAX_LENGTH - 1);
        buffer_size = DICTIONARY_MAX_LENGTH - 1;
    }

    // Append keycode to buffer.
    typo_buffer[buffer_size++] = (uint8_t)keycode;
    // Return if buffer is smaller than the shortest word.
    if (buffer_size < DICTIONARY_MIN_LENGTH) {
        return true;
    }

    // Check for typo in buffer using a trie stored in dictionary.
    uint16_t state = 0;
    uint8_t code = pgm_read_byte(dictionary + state);
    for (int8_t i = buffer_size - 1; i >= 0; --i) {
        if (code & 64) {  // Check for match in node with multiple children.
            code &= 63;
            for (; code != typo_buffer[i]; code = pgm_read_byte(dictionary + (state += 3))) {
                if (!code) {
                    return true;
                }
            }
            // Follow link to child node.
            state = (pgm_read_byte(dictionary + state + 1) | pgm_read_byte(dictionary + state + 2) << 8);
        // Otherwise check for match in node with a single child.
        } else if (code != typo_buffer[i]) {
            return true;
        } else if (!(code = pgm_read_byte(dictionary + (++state)))) {
            ++state;
        }

        // Read first byte of the next node.
        code = pgm_read_byte(dictionary + state);

        if (code & 128) {  // A typo was found! Apply correction.
            uint8_t const backspaces = code & 63;
            for (uint8_t i = 0; i < backspaces; ++i) {
                tap_code(KC_BSPC);
            }
            send_string_P((char const *)(dictionary + state + 1));

            if (keycode == KC_SPC) {
                typo_buffer[0] = KC_SPC;
                buffer_size = 1;
                return true;
            } else {
                buffer_size = 0;
                return false;
            }
        }
    }
    return true;
}