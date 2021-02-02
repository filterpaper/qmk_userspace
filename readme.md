# Summary

My personal user space is a self-contained folder that avoids placing keymap files inside
any keyboard directories. All customisation required to build QMK firmware is configured
within this space in the following manner:

* Personalise keyboard layout using [QMK Configurator](https://config.qmk.fm/#/) and export
the JSON file with keymap named after this space.
* Create rules.mk, config.h and shared source codes in this folder, with `#ifdef` blocks
for unique keyboard or feature specific functions.
* Run QMK compile on the exported JSON file to build a custom firmware for each board.
* See the [userspace guide](../../docs/feature_userspace.md) for more details.

# Supported Keyboards

![corneplanck](corneplanck.png)

* [BM40 HS RGB](../../keyboards/bm40hsrgb) — Features layer key press effects, per key RGB modifier lights.
* [Planck rev6](../../keyboards/planck) — RGB underglow lights as layer and modifier indicators.
* [Corne Keyboard (CRKBD)](../../keyboards/crkbd) — Bongocat typing animation and graphical indicators on OLEDs.
* [The Mark: 65](../../keyboards/boardsource/the_mark) — RGB underglow effects as layer and modifier indicators.

# File Listing

File | Description
---- | -----------
rules.mk | QMK compile rules and options
config.h | QMK configuration variables and options, see [configuring QMK](../../docs/config_options.md)
filterpaper.h | User specific variables and options
filterpaper.c | User source with custom functions, see [RGB matrix lighting](../../docs/feature_rgb_matrix.md) and [custom quantum functions](../../docs/custom_quantum_functions.md)
bongo-cat-full.c | Bongocat aligned for both left and right 128x32px OLED (huge: 8774 bytes)
bongo-cat-slim.c | Bongocat aligned for both left and right 128x32px OLED without prep paws (large: 7716 bytes)
bongo-cat-right.c | Bongocat aligned only for right 128x32px OLED (small: 4614 bytes)
mod-status.c | Graphical layer and modifier status module for primary OLED
glcdfont.c | QMK logo, コルネ katakana name, fonts and icon images—required by mod-status.c
rgb_matrix_user.inc | Custom RGB matrix effects collected from Reddit, see [Custom RGB Matrix](../../docs/feature_rgb_matrix.md#custom-rgb-matrix-effects-idcustom-rgb-matrix-effects)
json | Folder of supported keyboard layouts

# Build Commands
QMK will read "keyboard" and "keymap" values from the JSON file to build the firmware:
```
qmk compile ~/qmk_firmware/users/filterpaper/json/bm40.json
qmk compile ~/qmk_firmware/users/filterpaper/json/planck.json
qmk compile ~/qmk_firmware/users/filterpaper/json/corne.json
qmk compile ~/qmk_firmware/users/filterpaper/json/mark65.json
```

# Corne Split Setup
Corne is configured with EE_HANDS for controllers to read left or right values off EEPROM,
allowing USB-C cable to be used on either side. These are one-time flash commands to
write left and right side values into both Elite-C MCUs:
```
make crkbd/rev1/common:default:dfu-split-left
make crkbd/rev1/common:default:dfu-split-right
```
Subsequently, the same firmware binary can be flashed normally to both sides.
See [split keyboard features](../../docs/feature_split_keyboard.md) for details.

# QMK logo file
Images in `glcdfont.c` can be viewed and edited with:
* [Helix Font Editor](https://helixfonteditor.netlify.app/)
* [QMK Logo Editor](https://joric.github.io/qle/)
