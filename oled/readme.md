# OLED modules
These modules are coded for Corne (CRKBD)'s 128x32 and can be integrated as standalone files without extra OLED code in `keymap.c`. They are setup to animate Bongocat or Luna on the primary display with status icons rendered on the secondary.

# QMK Integration
1. Save all files into a folder named `oled` within the user keymap (or userspace) directory.
2. Add the following lines into `rules.mk`:
   ```c
   OLED_ENABLE = yes
   SRC += oled/oled-bongocat.c oled/oled-icons.c
   ```
   (Substitute `oled-bongocat.c` with `oled-luna.c` for Luna the dog)
3. Animation can be driven by WPM feature or input keystrokes:
   * To animate with WPM, add `WPM_ENABLE = yes` into `rules.mk`.
   * Otherwise input detection will be based on `last_input_activity_time()`.
4. Add the following directives into `config.h` to transmit state information to the secondary half for the status icons:
   ```c
   #define SPLIT_MODS_ENABLE
   #define SPLIT_LED_STATE_ENABLE
   #define SPLIT_LAYER_STATE_ENABLE
   ```
5. Add the following font file reference containing status icons into `config.h`:
   ```c
   #define OLED_FONT_H "oled/oledfont.c"
   ```
6. OLED code uses the following preprocessors as layer indicators. Add them into `config.h` and edit accordingly:
   ```c
   #define CMK 1
   #define NUM 2
   #define SYM 3
   #define FNC 4
   ```
