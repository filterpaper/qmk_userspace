# Word Processing Features

## Caps Word
This is a simple code function that will automatically disable caps lock at the end of a word, after a preset keycode is detected.

## Autocorrections
Code is adapted from [Pascal Getreuer](https://getreuer.info/posts/keyboards/autocorrection)'s source, with added support for mod tap shift and dictionary storage in AVR program space.

Dictionary is encoded in a trie data structure and the code will send corrections for typos detected in a buffer as they are being processed by QMK. Typos and corrections can be added into `autocorrection_dict.txt` in the format:
```c
typo   -> correction
```
Run `python3 make_autocorrection_data.py` to generate a trie dictionary in the file `autocorrection_data.h`.

## QMK Integration
Add the following line into `rules.mk` to build both files:
```c
SRC += autocorrection.c caps_word.c
```
Add the following code block into `keymaps.c` or userspace source file to process every key press:
```c
bool process_record_user(uint16_t const keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        extern bool process_autocorrection(uint16_t keycode, keyrecord_t* record);
        extern bool process_caps_word(uint16_t keycode, keyrecord_t *record);
        if (!process_autocorrection(keycode, record)) { return false; }
        if (!process_caps_word(keycode, record)) { return false; }
    }
    return true;
}
```
