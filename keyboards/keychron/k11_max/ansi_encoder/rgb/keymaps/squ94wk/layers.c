bool fire_for_all(smart_key_t *key, keypos_t pos) {
    return true;
}

void tap_multi_matching_brace(smart_key_t *key) {
    uint8_t mask = key->tap.mask;
    switch (key->state.tap_count) {
    case 1:
        virtual_send(key->pos, key->keycode, key->tap.keycode, mask);
        break;
    case 2:
        virtual_send(key->pos, key->keycode, key->tap.keycode, mask);
        virtual_send(key->pos, key->keycode, key->tap.keycode + 1, mask);
        break;
    }
}

smart_key_t *smart_layers[SMART_LAYER_COUNT][MATRIX_ROWS][MATRIX_COLS] = {
    [LAYER_ALPHA_1] = {
        [2][4] = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer = LAYER_SYMBOLS, .fire_on_key_press = &fire_for_all, },
        [2][7] = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer = LAYER_J_HOLD, .fire_on_key_press = &fire_for_all, },
        [2][8] = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer = LAYER_K_HOLD, .fire_on_key_press = &fire_for_all, },
        [2][9] = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer = LAYER_L_HOLD, .fire_on_key_press = &fire_for_all, },
        [2][10] = &(smart_key_t){ .tap.action = &magic_action, },
        [2][3] = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer = LAYER_STRINGS, .fire_on_key_press = &fire_for_all, },
        [2][2] = &(smart_key_t){ .tap.layer_oneshot = LAYER_ALPHA_2, .hold.layer = LAYER_ALPHA_2, .fire_on_key_press = &fire_for_all, },
        [3][5] = &(smart_key_t){ .tap.keycode = KC_D, .hold.layer = LAYER_BRACKETS, .fire_on_key_press = &fire_for_all, },
        [1][3] = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer = LAYER_SEARCH, .fire_on_key_press = &fire_for_all, },
        [1][2] = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer = LAYER_SEARCH_2, .fire_on_key_press = &fire_for_all, },
        [1][8] = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer = LAYER_I_HOLD, .fire_on_key_press = &fire_for_all, },
        [1][9] = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer = LAYER_O_HOLD, .fire_on_key_press = &fire_for_all, },
        [3][9] = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer = LAYER_H_HOLD, .fire_on_key_press = &fire_for_all, },
        [4][6] = &(smart_key_t){ .tap.layer_oneshot = LAYER_SYS, .hold.layer = LAYER_SYS, .fire_on_key_press = &fire_for_all, },
        [4][7] = &(smart_key_t){ .tap.layer_toggle = LAYER_NUM, .hold.layer = LAYER_NUM, .fire_on_key_press = &fire_for_all, },
        [2][2] = &(smart_key_t){ .tap.layer_oneshot = LAYER_ALPHA_2, .hold.layer = LAYER_ALPHA_2, .fire_on_key_press = &fire_for_all, },
        [4][5] = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.keycode = KC_LEFT_SHIFT, .fire_on_key_press = &fire_for_all, },
    },

    [LAYER_ALPHA_2] = {
        [1][2] = &(smart_key_t){ .tap.keycode = KC_2, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
        [1][3] = &(smart_key_t){ .tap.keycode = KC_1, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
        [1][4] = &(smart_key_t){ .tap.keycode = KC_SLASH, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
        [2][3] = &(smart_key_t){ .tap.keycode = KC_COMMA, },

        [2][4] = &(smart_key_t){ .tap.keycode = KC_DOT, },
        [3][4] = &(smart_key_t){ .tap.keycode = KC_X, },
        [3][5] = &(smart_key_t){ .tap.keycode = KC_V, },

        [1][7] = &(smart_key_t){ .tap.keycode = KC_Z, },
        [1][8] = &(smart_key_t){ .tap.keycode = KC_K, },
        [1][9] = &(smart_key_t){ .tap.keycode = KC_P, },
        [2][7] = &(smart_key_t){ .tap.keycode = KC_Y, },
        [2][8] = &(smart_key_t){ .tap.keycode = KC_J, },
        [2][9] = &(smart_key_t){ .tap.keycode = KC_SEMICOLON, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
        [3][10] = &(smart_key_t){ .tap.keycode = KC_Q, },
        [3][11] = &(smart_key_t){ .tap.keycode = KC_SEMICOLON, },
    },

    [LAYER_STRINGS] = {
        [2][7] = &(smart_key_t){ .tap.keycode = KC_QUOTE, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
        [2][8] = &(smart_key_t){ .tap.keycode = KC_QUOTE, },
        [2][10] = &(smart_key_t){ .tap.keycode = KC_BACKSLASH, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
        [2][9] = &(smart_key_t){ .tap.keycode = KC_GRV, },
    },
//    [SMART_EXCLAIM] = { .tap.keycode = KC_1, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
//    [SMART_QUESTION] = { .tap.keycode = KC_SLASH, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
//    [SMART_COMMA] = { .tap.keycode = KC_COMMA, },
//    [SMART_DOT] = { .tap.keycode = KC_DOT, },

    [LAYER_BRACKETS] = {
        [3][9] = &(smart_key_t){ .tap.keycode = KC_COMMA, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
        [2][7] = &(smart_key_t){ .tap.keycode = KC_9, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), .max_tap = 2, },
        [2][8] = &(smart_key_t){ .tap.keycode = KC_LEFT_BRACKET, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), .max_tap = 2, },
        [2][9] = &(smart_key_t){ .tap.keycode = KC_LEFT_BRACKET, .max_tap = 2, },
//        [KC_U] = &smart_keys[SMART_EXCLAIM],
//        [KC_I] = &smart_keys[SMART_QUESTION],
//        [KC_N] = &smart_keys[SMART_COMMA],
//        [KC_M] = &smart_keys[SMART_DOT],
    },

    [LAYER_SYMBOLS] = {
        [1][7] = &(smart_key_t){ .tap.keycode = KC_7, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
        [1][8] = &(smart_key_t){ .tap.keycode = KC_SLASH, },
        [1][9] = &(smart_key_t){ .tap.keycode = KC_BACKSLASH, },
        [2][7] = &(smart_key_t){ .tap.keycode = KC_EQUAL, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
        [2][8] = &(smart_key_t){ .tap.keycode = KC_MINUS, },
        [2][9] = &(smart_key_t){ .tap.keycode = KC_EQUAL, },
        [3][8] = &(smart_key_t){ .tap.keycode = KC_1, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
        [3][9] = &(smart_key_t){ .tap.keycode = KC_MINUS, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
        [3][10] = &(smart_key_t){ .tap.keycode = KC_3, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
        [3][11] = &(smart_key_t){ .tap.keycode = KC_8, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
    },

    // Closing brackets
    [LAYER_H_HOLD] = {
        [3][5] = &(smart_key_t){ .tap.keycode = KC_DOT, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
    },
    [LAYER_J_HOLD] = {
        [3][5] = &(smart_key_t){ .tap.keycode = KC_0, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), .max_tap = 2, },
    },
    [LAYER_K_HOLD] = {
        [3][5] = &(smart_key_t){ .tap.keycode = KC_RIGHT_BRACKET, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), .max_tap = 2, },
    },
    [LAYER_L_HOLD] = {
        [3][5] = &(smart_key_t){ .tap.keycode = KC_RIGHT_BRACKET, .max_tap = 2, },
    },

    [LAYER_SEARCH] = {
        [1][6] = &(smart_key_t){ .tap.keycode = KC_5, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
        [1][8] = &(smart_key_t){ .tap.keycode = KC_SLASH, },
        [1][9] = &(smart_key_t){ .tap.keycode = KC_8, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
    },

    [LAYER_I_HOLD] = {
        [1][3] = &(smart_key_t){ .tap.keycode = KC_SLASH, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
    },

    [LAYER_SEARCH_2] = {
        [1][9] = &(smart_key_t){ .tap.keycode = KC_4, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
    },

    [LAYER_O_HOLD] = {
        [1][3] = &(smart_key_t){ .tap.keycode = KC_3, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
        [1][2] = &(smart_key_t){ .tap.keycode = KC_6, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
    },

    [LAYER_SYS] = {
        [2][2] = &(smart_key_t){ .tap.keycode = KC_ESC, },
        [2][3] = &(smart_key_t){ .tap.keycode = KC_ENT, },
        [2][4] = &(smart_key_t){ .tap.keycode = KC_TAB, },

        [1][9] = &(smart_key_t){ .tap.keycode = KC_BSPC, },
        [1][8] = &(smart_key_t){ .tap.keycode = KC_UP, },

        [2][7] = &(smart_key_t){ .tap.keycode = KC_LEFT, },
        [2][8] = &(smart_key_t){ .tap.keycode = KC_DOWN, },
        [2][9] = &(smart_key_t){ .tap.keycode = KC_RIGHT, },
    },

    [LAYER_NUM] = {
        [3][9] = &(smart_key_t){ .tap.keycode = KC_0, },
        [2][7] = &(smart_key_t){ .tap.keycode = KC_1, },
        [2][8] = &(smart_key_t){ .tap.keycode = KC_2, },
        [2][9] = &(smart_key_t){ .tap.keycode = KC_3, },
        [2][10] = &(smart_key_t){ .tap.keycode = KC_4, },
        [2][3] = &(smart_key_t){ .tap.keycode = KC_COMMA, },
        [2][4] = &(smart_key_t){ .tap.keycode = KC_5, },
        [2][3] = &(smart_key_t){ .tap.keycode = KC_6, },
        [2][2] = &(smart_key_t){ .tap.keycode = KC_7, },
        [2][1] = &(smart_key_t){ .tap.keycode = KC_8, },
        [3][5] = &(smart_key_t){ .tap.keycode = KC_9, },
        [3][8] = &(smart_key_t){ .tap.keycode = KC_DOT, },
    },
};

enum smart_keys {
    SMART_KEY_SMART_SHIFT,
    SMART_KEY_SMART_CTRL,
    SMART_KEY_SMART_ALT,
    SMART_KEY_SMART_CTRL_SHIFT,
    // end
    SMART_KEY_COUNT,
};

void smart_mod_n_tap(smart_key_t *key) {
    switch (key->state.tap_count) {
    case 1:
        uprintf("DEBUG: oneshot\n");
        set_oneshot_mods(get_oneshot_mods() | key->tap.mask_oneshot | MOD_BIT(key->hold.keycode));
        break;
    case 2:
        // FIXME: CAPS word doesn't work with smart keys yet
        uprintf("DEBUG: caps word\n");
        caps_word_on();
        break;
    }
    return;
}

smart_key_t smart_keys[] = {
    [SMART_KEY_SMART_SHIFT] = { .keycode = CKC_SMART_SHIFT, .max_tap = 2, .tap.action = &smart_mod_n_tap, .hold.keycode = KC_LEFT_SHIFT, },
    [SMART_KEY_SMART_CTRL] = { .keycode = CKC_SMART_CTRL, .max_tap = 1, .tap.action = &smart_mod_n_tap, .hold.keycode = KC_LEFT_CTRL, },
    [SMART_KEY_SMART_ALT] = { .keycode = CKC_SMART_ALT, .max_tap = 1, .tap.action = &smart_mod_n_tap, .hold.keycode = KC_LEFT_ALT, },
    [SMART_KEY_SMART_CTRL_SHIFT] = { .keycode = CKC_SMART_CTRL_SHIFT, .max_tap = 1, .tap.action = &smart_mod_n_tap, .hold.keycode = KC_LEFT_CTRL, .hold.mask = MOD_BIT(KC_LEFT_SHIFT)},
};

smart_key_t *lookup_key(uint16_t keycode, keypos_t pos) {
    if (!pos.row && !pos.col) {
        for (int i=0; i < SMART_KEY_COUNT; i++) {
            if (keycode == smart_keys[i].keycode) {
                return &smart_keys[i];
            }
        }
        return smart_layers[LAYER_ALPHA_1][pos.row][pos.col];
    }
    for (int i=0; i < SMART_LAYER_COUNT && active_layers[i]; ++i) {
        smart_key_t *key = smart_layers[active_layers[i]][pos.row][pos.col];
        if (key) {
            return key;
        }
    }
    return smart_layers[LAYER_ALPHA_1][pos.row][pos.col];
}

int active_layers[SMART_LAYER_COUNT] = {};
bool oneshot_layers[SMART_LAYER_COUNT] = {};

void matrix_init_user(void) {
    // initialize keycodes
    for (int i=0; i < MATRIX_ROWS; ++i) {
        for (int j=0; j < MATRIX_COLS; ++j) {
            for (int l=0; l < SMART_LAYER_COUNT; ++l) {
                smart_key_t *key = smart_layers[l][i][j];
                if (key) {
                    // copy position
                    key->pos = (keypos_t){
                        .row = i,
                        .col = j,
                    };
                    // copy keycode from base layer
                    if (key->tap.keycode == KC_TRANSPARENT) {
                        key->tap.keycode = keymaps[WIN_BASE][i][j];
                    }
                }
            }
        }
    }
}
