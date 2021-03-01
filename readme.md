# Summary
This is my personal *userspace* for [QMK Firmware](https://github.com/qmk/qmk_firmware). It is setup as a self-contained folder that avoids placing `keymap.c` source files deep inside QMK's sub-directories. All customisation required to build firmwares are configured within this space in the following manner:

* Design keyboard layouts using [QMK Configurator](https://config.qmk.fm/#/) and export the JSON files with keymap named after this space.
* Create `rules.mk`, `config.h` and shared source codes in this folder, with `#ifdef` preprocessors for unique keyboard or feature specific functions.
* Run `qmk flash` on the exported JSON file to build a custom firmware for each board.
* See my [standalone userspace](https://filterpaper.github.io/qmk/userspace) guide for more details.

## Setup
Clone the QMK firmware, followed by this repository into `users/filterpaper`:
```sh
git clone https://github.com/qmk/qmk_firmware qmk_firmware
git clone https://github.com/filterpaper/qmk_userspace qmk_firmware/users/filterpaper
```

# Supported Keyboards
![corneplanck](corneplanck.png)

* [BM40 HS RGB](../../keyboards/bm40hsrgb) — Per-key RGB matrix indicators.
* [Planck rev6](../../keyboards/planck) — RGB underglow layer and modifier indicators.
* [Corne Keyboard (CRKBD)](../../keyboards/crkbd) — Typing animation and graphical indicators on OLEDs.
* [The Mark: 65](../../keyboards/boardsource/the_mark) — RGB underglow layer and modifier indicators.

# File Listing
File | Description
---- | -----------
rules.mk | QMK compile rules and hardware feature selection
config.h | QMK configuration variables and options, see [configuring QMK](../../docs/config_options.md)
filterpaper.h | User specific variables and options
filterpaper.c | User source with custom functions, see [RGB matrix lighting](../../docs/feature_rgb_matrix.md) and [custom quantum functions](../../docs/custom_quantum_functions.md)
bongocat.c | Bongocat typing animation based on differential pixel of next frame (adds ~3768 bytes)
mod-status.c | Graphical layer and modifier status module for primary OLED
luna.c | Tiny Luna and Felix the dog typing animation source code for primary status module
glcdfont.c | Corne logo, コルネ katakana name, fonts and icon images—required by mod-status.c
rgb_matrix_user.inc | Custom RGB matrix effects collected from Reddit, see [Custom RGB Matrix](../../docs/feature_rgb_matrix.md#custom-rgb-matrix-effects-idcustom-rgb-matrix-effects)
bongocat-fat.c | Original full frame Bongocat typing animation source code that is space consuming (adds ~7684 bytes)
json | Folder of supported keyboard layouts
animation_frames | Folder of Bongocat animation images

# Code Snippets
## Light configured layers keys
```c
if (get_highest_layer(layer_state); >_COLEMAK) {
    uint8_t layer = get_highest_layer(layer_state);
    for (uint8_t row = 0; row <MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col <MATRIX_COLS; col++) {
            if (g_led_config.matrix_co[row][col] !=NO_LED &&
                keymap_key_to_keycode(layer, (keypos_t){col, row}) !=KC_TRNS) {
                rgb_matrix_set_color(g_led_config.matrix_co[row][col], RGB_LAYER);
            }
        }
    }
}
```
Code loops through every row and column on a per-key RGB board, scanning for configured keys (not `KC_TRANS`) and lighting that index location. It is configured to activate on non-default layers. This can be further customised with specific layer colors in a `switch` condition inside the last `if` statement.

## Tap hold macros
```c
#define W_TH LT(0, KC_W)

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case W_TH:
        // Send W on single tap
        if (record->tap.count) {
            if (record->event.pressed) { register_code(KC_W); }
            else { unregister_code(KC_W); }
        // Send macro string on hold
        } else {
            if (record->event.pressed) { SEND_STRING(":wq"); }
        }
        return false;
    }
    return true; // continue with unmatched keycodes
}
```
These features can be found QMK's [tap dance feature](../../docs/feature_tap_dance.md) but replicated using `process_record_user()` with layer tap (`LT()`) key and tapping term delay. It uses less firmware space than `TAP_DANCE_ENABLE` (~35 bytes per macro). Macro `W_TH` replaces `KC_W` on the key map (`keymap[]`).

# Build Commands
QMK will read "keyboard" and "keymap" values from the JSON file to build the firmware:
```sh
qmk flash ~/qmk_firmware/users/filterpaper/json/bm40.json
qmk flash ~/qmk_firmware/users/filterpaper/json/planck.json
qmk flash ~/qmk_firmware/users/filterpaper/json/corne.json
qmk flash ~/qmk_firmware/users/filterpaper/json/mark65.json
```

# Building Corne Keyboard (CRKBD)
## Corne split setup
Corne is configured with EE_HANDS for controllers to read left or right values off EEPROM, allowing USB-C cable to be used on either side. These are one-time flash commands to write left and right handedness into Elite-C MCUs:
```sh
qmk flash -kb crkbd/rev1/common -km default -bl dfu-split-left
qmk flash -kb crkbd/rev1/common -km default -bl dfu-split-right
```
Subsequently, the same firmware binary can be flashed normally to both sides. See [split keyboard features](../../docs/feature_split_keyboard.md) for details.

## Compiling the cat
The `bongocat.c` is an updated source with typing animation using *differential* pixels. The code renders a base frame, followed by *changed* pixels of subsequent animation frames. It uses less space compared to full 512-byte frame renderings. Left and right aligned cat are built by default. To reduce firmware size (about ~1622 bytes), compile with `LEFTCAT` and `RIGHTCAT` preprocessor separately to flash on each side: `qmk flash -e LEFTCAT=yes corne.json`

`bongocat-fat.c` is the original source with typing animation frames aligned correctly for both left and right secondary OLED display. They are quite space consuming because each frame requires 512 bytes to fill the 128x32px OLED display. Default preprocessor `SLIMCAT` in `filterpaper.h` will reduce size by 1060 bytes. To further halve build size, compile with `LEFTCAT` and `RIGHTCAT` separately to flash on each side. All three preprocessors can be enabled with compile-time environment variables: `qmk flash -e LEFTCAT=yes corne.json`

## Compiling the dog
The `luna.c` source has a tiny Luna dog that reacts to typing speed, and modifier activation. Its 5 actions can be customised as reaction to any state. Luna replaces the layer icon in `mod-status.c` if included. It can be compiled with variable `qmk flash -e LUNA=yes corne.json`. An alternative white Felix the dog will be built with `qmk flash -e FELIX=yes corne.json`

## Corne logo file
Images in `glcdfont.c` can be viewed and edited with:
* [Helix Font Editor](https://helixfonteditor.netlify.app/)
* [QMK Logo Editor](https://joric.github.io/qle/)
