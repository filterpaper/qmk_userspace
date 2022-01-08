# OLED modules
These modules are coded for Corne (CRKBD)'s 128x32 and can be integrated as standalone files without extra OLED code in `keymap.c`. They are setup to animate Bongocat or Luna on the primary display with status icons rendered on the secondary.

# Integration Guide
1. Save this folder in your keymap (or user space) directory.
2. Add the following lines into `rules.mk` ():
   ```c
   OLED_ENABLE = yes
   SRC += oled-bongocat.c oled-icons.c
   ```
   (Use `oled-luna.c` for Luna the dog)
3. Left and right aligned Bongocat will be compiled by default. To save space, add *either* one of the following directives into `rules.mk`:
   ```c
   OPT_DEFS += -DLEFTCAT
   ```
   ```c
   OPT_DEFS += -DRIGHTCAT
   ```
   (Skip this step for Luna)
4. Animation can be driven by WPM or normal keystrokes:
   * To animate with WPM, add 'WPM_ENABLE = yes' into rules.mk.
   * To animate with keystrokes, add the following 'process_record_user()' code block to your `keymap.c` file to trigger animation tap timer with key presses:
   ```c
   bool process_record_user(uint16_t keycode, keyrecord_t *record) {
       if (record->event.pressed) {
           extern uint32_t tap_timer;
           tap_timer = timer_read32();
       }
   }
   ```
5. Add the following directives into `config.h` to transmit state information for the secondary half to report status with the icons:
   ```c
   #define SPLIT_MODS_ENABLE
   #define SPLIT_LED_STATE_ENABLE
   #define SPLIT_LAYER_STATE_ENABLE
   ```
6. Add the following font file reference containing status icons into `config.h`:
   ```c
   #define OLED_FONT_H "oled/oledfont.c"
   ```
