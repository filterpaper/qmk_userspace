// Copyright 2021 Google LLC
// Copyright 2022 @filterpaper
// SPDX-License-Identifier: Apache-2.0
// Adapted from Pascal Getreuer's Autocorrection
// https://getreuer.info/posts/keyboards/autocorrection

#pragma once

void autocorrect_toggle(void);
bool process_autocorrect(uint16_t keycode, keyrecord_t *record);