#if defined(KEYBOARD_3w6_rev2) || defined(KEYBOARD_a_dux) || defined(KEYBOARD_cradio)
	// Vertical for choc spacing
	COMB(KC_VOLU,  KC_VOLU,  KC_Y, KC_H)
	COMB(KC_VOLD,  KC_VOLD,  KC_H, KC_N)
#else
	// Horizontal for MX spacing
	COMB(KC_VOLU,  KC_VOLU,  KC_Y, KC_U)
	COMB(KC_VOLD,  KC_VOLD,  KC_H, HM_J)
	COMB(KC_MUTE,  KC_MUTE,  KC_N, M_TH)
#endif

#ifdef SWAP_HANDS_ENABLE
	COMB(swap_left,  SH_TT,  KC_C, KC_V)
	COMB(swap_rght,  SH_TT,  M_TH, COMM_TH)
#endif

SUBS(vi_quit,  ":q!",   KC_Q, KC_W)
SUBS(vi_save,  ":wq!",  KC_Z, KC_X)
ACTN(dir_up, tap_code(KC_DOT); tap_code(KC_DOT); tap_code(KC_SLSH), DOT_TH, SLSH_TH)

// layer toggles
COMB(func,  TG(FNC),  LT(SYM,KC_TAB), KC_B)
COMB(numb,  TG(NUM),  LT(NUM,KC_BSPC), KC_N)

// vimac
COMB(vimacf,  C(A(S(G(KC_F)))),  HM_D, HM_F)
COMB(vimacj,  C(A(S(G(KC_J)))),  HM_J, HM_K)
