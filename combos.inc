// Layout key reference
// KC_Q    KC_W    KC_E    KC_R    KC_T        KC_Y    KC_U    KC_I    KC_O    KC_P
// HM_A    HM_S    HM_D    HM_F    KC_G        KC_H    HM_J    HM_K    HM_L    HM_QUOT
// KC_Z    KC_X    KC_C    KC_V    KC_B        KC_N    TH_M    TH_COMM TH_DOT  TH_SLSH
//         LT(SYM,KC_TAB)  RSG_T(KC_ENT)       RSFT_T(KC_SPC)  LT(NUM,KC_BSPC)

#ifdef SWAP_HANDS_ENABLE
	COMB(swap_l,  SH_TG,  KC_X, KC_C, KC_V)
	COMB(swap_r,  SH_TG,  TH_M, TH_COMM, TH_DOT)
#endif
// layer toggles
//COMB(func,    TG(FNC), LT(SYM,KC_TAB),  KC_B)
//COMB(numb,    TG(NUM), LT(NUM,KC_BSPC), KC_N)
// volume control
COMB(vol_up,  KC_VOLU, KC_P,    HM_QUOT)
COMB(vol_dn,  KC_VOLD, HM_QUOT, TH_SLSH)
// macros
SUBS(vi_quit, ":q!",   KC_Q,     KC_W)
SUBS(vi_save, ":wq",   KC_Z,     KC_X)
SUBS(dir_up,  "../",   TH_DOT,   TH_SLSH)
// navigation
COMB(nav_dn,  RSA_DN,  TH_M,    TH_COMM)
COMB(nav_up,  RSA_UP,  TH_COMM, TH_DOT)
// caps lock
COMB(caps_l,  KC_CAPS, HM_A, HM_QUOT)