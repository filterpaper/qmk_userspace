
#if defined(KEYBOARD_3w6_rev2) || defined(KEYBOARD_ferris_sweep)
// Vertical combos for choc spaced
COMB(KC_VOLU,	KC_VOLU, KC_Y, KC_H)
COMB(KC_VOLD,	KC_VOLD, KC_H, KC_N)
#else
// Horizontal combos for MX spaced
COMB(KC_VOLU,	KC_VOLU, KC_Y, KC_U)
COMB(KC_VOLD,	KC_VOLD, KC_H, HM_J)
COMB(KC_MUTE,	KC_MUTE, KC_N, M_TH)
#endif

SUBS(vi_quit,	":q!", KC_Q, KC_W)
SUBS(vi_save,	":wq!", KC_Z, KC_X)
