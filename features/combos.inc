// 3x5_2 layout key reference
// KC_Q    KC_W    KC_E    KC_R    KC_T      KC_Y    KC_U    KC_I    KC_O    KC_P
// HM_A    HM_S    HM_D    HM_F    KC_G      KC_H    HM_J    HM_K    HM_L    HM_QUOT
// KC_Z    KC_X    KC_C    KC_V    KC_B      KC_N    TH_M    TH_COMM TH_DOT  TH_SLSH
//                      SYM_TAB LCA_ENT      SFT_SPC NUM_BSP

#ifdef SWAP_HANDS_ENABLE
ACTN(swap_l, swap_hands_toggle(), KC_X, KC_C, KC_V)
ACTN(swap_r, swap_hands_toggle(), TH_M, TH_COMM, TH_DOT)
#endif
ACTN(tog_ac, autocorrect_toggle(), KC_U, KC_I, KC_O)

// Macros
SSTR(vi_quit,  ":q!",    KC_Q,    KC_W)
SSTR(vi_save,  ":x",     KC_Z,    KC_X)
SSTR(dir_up,   "../",    TH_DOT,  TH_SLSH)

COMB(vol_up,   KC_VOLU,  KC_P,    HM_QUOT)
COMB(vol_dn,   KC_VOLD,  HM_QUOT, TH_SLSH)

// Navigation
COMB(arr_up,   KC_UP,    KC_U,    KC_I)
COMB(arr_dn,   KC_DOWN,  HM_J,    HM_K)
COMB(arr_lt,   KC_LEFT,  KC_H,    HM_J)
COMB(arr_rt,   KC_RGHT,  HM_K,    HM_L)
COMB(nav_up,   SA_UP,    TH_COMM, TH_DOT)
COMB(nav_dn,   SA_DN,    TH_M,    TH_COMM)

// Noop thumbs
COMB(thmb_l,   KC_NO,    SYM_TAB, LCA_ENT)
COMB(thmb_r,   KC_NO,    SFT_SPC, NUM_BSP)

// Layer toggles
COMB(tog_num,  TG(NUM),  NUM_BSP, KC_N, TH_M)
COMB(tog_fnc,  TG(FNC),  SYM_TAB, KC_B, KC_V)