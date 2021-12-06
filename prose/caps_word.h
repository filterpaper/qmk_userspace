// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+
// Toggle caps lock following a word

#pragma once
#include QMK_KEYBOARD_H

bool process_caps_word(uint16_t keycode, keyrecord_t *record);
