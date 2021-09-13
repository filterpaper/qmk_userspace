
#if defined(KEYBOARD_3w6_rev2) || defined(KEYBOARD_ferris_0_2_compact)
// Vertical combos for choc spaced
COMB(KC_VOLU,	KC_VOLU, KC_P, HM_SCLN)
COMB(KC_VOLD,	KC_VOLD, HM_SCLN, SLSH_TH)

SUBS(vi_quit,	":q!", KC_Q, HM_A)
SUBS(vi_save,	":wq!", HM_A, KC_Z)

#else
// Horizontal combos for MX spaced
COMB(KC_VOLU,	KC_VOLU, KC_Y, KC_U)
COMB(KC_VOLD,	KC_VOLD, KC_H, HM_J)
COMB(KC_MUTE,	KC_MUTE, KC_N, M_TH)

SUBS(vi_quit,	":q!", KC_Q, KC_W)
SUBS(vi_save,	":wq!", KC_Z, KC_X)

#endif
