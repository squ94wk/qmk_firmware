enum combos {
    COMBO_LEFT_SHIFT,
    COMBO_RIGHT_SHIFT,
    COMBO_LEFT_CTRL,
    COMBO_RIGHT_CTRL,
    COMBO_LEFT_ALT,
    COMBO_RIGHT_ALT,
    COMBO_RIGHT_CTRL_SHIFT,
    COMBO_LEFT_CTRL_SHIFT,
};

// const uint16_t PROGMEM combo_space_space_enter[] = {KC_SPC, KC_BSPC, COMBO_END};
const uint16_t PROGMEM combo_left_shift[]       = {KC_S, KC_T, COMBO_END};
const uint16_t PROGMEM combo_right_shift[]       = {KC_N, KC_E, COMBO_END};
const uint16_t PROGMEM combo_left_ctrl[]        = {KC_ESC, KC_S, COMBO_END};
const uint16_t PROGMEM combo_right_ctrl[]        = {KC_E, KC_O, COMBO_END};
const uint16_t PROGMEM combo_left_alt[]         = {KC_ESC, KC_T, COMBO_END};
const uint16_t PROGMEM combo_right_alt[] = {KC_N, KC_O, COMBO_END};
const uint16_t PROGMEM combo_right_ctrl_shift[]        = {KC_N, KC_E, KC_O, COMBO_END};
const uint16_t PROGMEM combo_left_ctrl_shift[]         = {KC_ESC, KC_S, KC_T, COMBO_END};

combo_t key_combos[] = {
    [COMBO_LEFT_SHIFT] = COMBO(combo_left_shift, CKC_SMART_SHIFT),
    [COMBO_RIGHT_SHIFT] = COMBO(combo_right_shift, CKC_SMART_SHIFT),
    [COMBO_LEFT_CTRL] = COMBO(combo_left_ctrl, CKC_SMART_CTRL),
    [COMBO_RIGHT_CTRL] = COMBO(combo_right_ctrl, CKC_SMART_CTRL),
    [COMBO_LEFT_ALT] = COMBO(combo_left_alt, CKC_SMART_ALT),
    [COMBO_RIGHT_ALT] = COMBO(combo_right_alt, CKC_SMART_ALT),
    [COMBO_RIGHT_CTRL_SHIFT] = COMBO(combo_right_ctrl_shift, CKC_SMART_CTRL_SHIFT),
    [COMBO_LEFT_CTRL_SHIFT] = COMBO(combo_left_ctrl_shift, CKC_SMART_CTRL_SHIFT),
};
