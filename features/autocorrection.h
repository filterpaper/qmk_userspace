// Copyright 2021 Google LLC
// @filterpaper
// SPDX-License-Identifier: Apache-2.0
// https://getreuer.info/posts/keyboards/autocorrection

#pragma once
#include QMK_KEYBOARD_H

bool process_autocorrection(uint16_t keycode, keyrecord_t* record);
