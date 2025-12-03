#include "metrics.h"
#include "layout_helpers.h"

bool fire_for_all(smart_key_t *key, keypos_t pos) {
    return true;
}

void tap_multi_matching_brace(smart_key_t *key) {
    uprintf("DEBUG: tap matching braces, tap count: %d\n", key->state.tap_count);
    uint16_t mask = key->tap.mask;
    uint16_t kc = key->tap.keycode;
    switch (key->state.tap_count) {
        case 1:
            uprintf("DEBUG: send only opening bracket\n");
            register_with_mods(&kc, mask, NULL);
            key->state.release.keycode = kc;
            break;
        case 2:
            uprintf("DEBUG: send opening bracket\n");
            register_with_mods(&kc, mask, NULL);
            unregister_code(kc);
            uprintf("DEBUG: send closing bracket\n");
            kc++;
            register_with_mods(&kc, mask, NULL);
            key->state.release.keycode = kc;
            break;
    }
}

bool deactivate_on_other_layer(smart_layer_t *layer, int index) {
    return false;
}

void activate_tmux(smart_key_t *key) {
    register_with_mods(PTR_TO(KC_B), MOD_BIT(KC_LEFT_CTRL), NULL);
    unregister_code(KC_B);
    activate_layer(LAYER_TMUX);
    layer_activations[LAYER_TMUX] = 1;
}

bool always_on_other_press(smart_key_t *key, keypos_t pos) {
    return true;
}

void clear(smart_key_t *key) {
    clear_oneshot_mods();
    smart_case_off();
    set_mods(0);
    for (int i=1; i<SMART_LAYER_COUNT; i++) {
        deactivate_layer(i);
    }
}

void tap_num(smart_key_t *key) {
    set_oneshot_mods(get_oneshot_mods() | MOD_BIT_LSHIFT);
}

void hold_num(smart_key_t *key) {
    switch (key->state.tap_count) {
    case 1:
        activate_layer(LAYER_NUM);
        key->state.release.layer = LAYER_NUM;
        return;
    case 2:
        activate_layer(LAYER_FUNCTION_KEYS);
        key->state.release.layer = LAYER_FUNCTION_KEYS;
        return;
    }
}

void tap_backspace(smart_key_t *key) {
    if (get_mods() & MOD_MASK_SHIFT) {
        register_with_mods(PTR_TO(KC_DELETE), 0, 0);
        key->state.release.keycode = KC_DELETE;
        return;
    }
    register_with_mods(PTR_TO(KC_BACKSPACE), 0, 0);
    key->state.release.keycode = KC_BACKSPACE;
}

int os_index = 0;
void toggle_os_index(smart_key_t *key) {
    deactivate_layer(LAYER_DUMB);
    int count = key->state.tap_count;
    switch (count) {
    case 1:
    case 2:
        os_index = count - 1;
        break;
    case 3:
        os_index = 0;
        activate_layer(LAYER_DUMB);
        break;
    }
}

void print_magic_stats(smart_key_t *key) {
    metrics_print_histogram(&magic_timing_histogram, "magic_action2");
}

void jump_layer(smart_key_t *key) {
    switch (os_index) {
    case 0:
        activate_layer(LAYER_JUMP_WIN);
        key->state.release.layer = LAYER_JUMP_WIN;
        return;
    case 1:
        activate_layer(LAYER_JUMP_MAC);
        key->state.release.layer = LAYER_JUMP_MAC;
        return;
    }
    return;
}

void tap_ctrl_a(smart_key_t *key) {
    switch (os_index) {
    case 0:
        register_with_mods(PTR_TO(KC_A), MOD_BIT(KC_LEFT_CTRL), NULL);
        key->state.release.keycode = KC_A;
        return;
    case 1:
        register_with_mods(PTR_TO(KC_A), MOD_BIT(KC_LCMD), NULL);
        key->state.release.keycode = KC_A;
        return;
    }
}

void tap_ctrl_c(smart_key_t *key) {
    switch (os_index) {
    case 0:
        register_with_mods(PTR_TO(KC_C), MOD_BIT(KC_LEFT_CTRL), NULL);
        key->state.release.keycode = KC_C;
        return;
    case 1:
        register_with_mods(PTR_TO(KC_C), MOD_BIT(KC_LCMD), NULL);
        key->state.release.keycode = KC_C;
        return;
    }
}

void tap_ctrl_v(smart_key_t *key) {
    switch (os_index) {
    case 0:
        register_with_mods(PTR_TO(KC_V), MOD_BIT(KC_LEFT_CTRL), NULL);
        key->state.release.keycode = KC_V;
        return;
    case 1:
        register_with_mods(PTR_TO(KC_V), MOD_BIT(KC_LCMD), NULL);
        key->state.release.keycode = KC_V;
        return;
    }
}

void tap_ctrl_x(smart_key_t *key) {
    switch (os_index) {
    case 0:
        register_with_mods(PTR_TO(KC_X), MOD_BIT(KC_LEFT_CTRL), NULL);
        key->state.release.keycode = KC_X;
        return;
    case 1:
        register_with_mods(PTR_TO(KC_X), MOD_BIT(KC_LCMD), NULL);
        key->state.release.keycode = KC_X;
        return;
    }
}

void tap_ctrl_z(smart_key_t *key) {
    switch (os_index) {
    case 0:
        register_with_mods(PTR_TO(KC_Z), MOD_BIT(KC_LEFT_CTRL), NULL);
        key->state.release.keycode = KC_Z;
        return;
    case 1:
        register_with_mods(PTR_TO(KC_Z), MOD_BIT(KC_LCMD), NULL);
        key->state.release.keycode = KC_Z;
        return;
    }
}

void tap_ctrl_r(smart_key_t *key) {
    switch (os_index) {
    case 0:
        register_with_mods(PTR_TO(KC_R), MOD_BIT(KC_LEFT_CTRL), NULL);
        key->state.release.keycode = KC_R;
        return;
    case 1:
        register_with_mods(PTR_TO(KC_R), MOD_BIT(KC_LCMD), NULL);
        key->state.release.keycode = KC_R;
        return;
    }
}

#ifdef MOUSEKEY_ENABLE
void hold_mouse(smart_key_t *key) {
    switch (key->state.tap_count) {
    case 1:
        register_with_mods(PTR_TO(KC_RIGHT_SHIFT), 0, NULL);
        key->state.release.keycode = KC_RIGHT_SHIFT;
        return;
    case 2:
        activate_layer(LAYER_MOUSE);
        key->state.release.layer = LAYER_MOUSE;
        return;
    }
    return;
}
#endif

void vim_blackhole_register(smart_key_t *key) {
    SEND_STRING("\"_");
}

void smart_case(smart_key_t *key) {
    smart_case_on();
}

void repeat_last_key(smart_key_t *key) {
    history_entry_t last = history_top();
    if (!last.c) {
        return;
    }

    bool shifted = false;
    uint16_t kc = char_to_keycode(last.c, &shifted);
    if (kc == KC_NO) {
        return;
    }

    uint8_t mods = last.mods;
    register_with_mods(&kc, mods, &key->state.release.mask);
    key->state.release.keycode = kc;
}

void hold_alpha(smart_key_t *key) {
    switch (key->state.tap_count) {
    case 1:
        activate_layer(LAYER_ALPHA_2);
        key->state.release.layer = LAYER_ALPHA_2;
        return;
    case 2:
        register_with_mods(PTR_TO(KC_LEFT_SHIFT), 0, NULL);
        activate_layer(LAYER_ALPHA_2);
        key->state.release.keycode = KC_LEFT_SHIFT;
        key->state.release.layer = LAYER_ALPHA_2;
        return;
    }
    return;
}

smart_layer_t * smart_layers[SMART_LAYER_COUNT] = {
                [LAYER_ALPHA_1] =
                    &(smart_layer_t){
                        .map = {
                                L_HOME_RING = &(smart_key_t){ .max_tap = 2, .defer_release = true, .tap.keycode = KC_TRANSPARENT, .hold.action = &hold_alpha, .hold_on_key_press = &always_on_other_press, },
                                L_HOME_MID  = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.keycode = KC_LEFT_SHIFT, },
                                L_HOME_IDX  = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer  = LAYER_SYMBOLS, },

                                R_HOME_IDX  = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer  = LAYER_VIM_TEXT, },
                                R_HOME_MID  = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer  = LAYER_VIM_NAV, },
                                R_HOME_RING = &(smart_key_t){ .defer_release = true, .tap.action = &magickey_action, .hold.layer = LAYER_ALPHA_2, .hold_on_key_press = &always_on_other_press, },

                                L_BOT_RING  = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.keycode = KC_LEFT_SHIFT, },
                                L_BOT_IDX   = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer  = LAYER_SYMBOLS_2, },

                                R_BOT_MID   = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer  = LAYER_H_HOLD, },
                                R_BOT_RING  = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer  = LAYER_L_HOLD, },
                                R_BOT_PINK  = &(smart_key_t){ .defer_release = true, .tap.keycode = KC_TRANSPARENT, .hold.keycode = KC_RIGHT_SHIFT, .hold_on_key_press = &always_on_other_press, },

                                L_THUMB_IN  = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer = LAYER_SYS, },
                                L_THUMB_OUT = &(smart_key_t){ .max_tap = 2, .tap.action = &repeat_last_key, .hold.action = &hold_num, .hold_on_key_press = &always_on_other_press, },

                                R_THUMB_IN  = &(smart_key_t){ .tap.action = &magickey_complete, },
                                R_THUMB_OUT = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer = LAYER_NUM, },
                            },
                    },

                [LAYER_ALPHA_2] =
                    &(smart_layer_t){
                        .map = {
                            L_TOP_RING  = &(smart_key_t){ .tap.keycode = KC_2, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
                            L_TOP_MID   = &(smart_key_t){ .tap.keycode = KC_X, },
                            L_TOP_IDX   = &(smart_key_t){ .tap.keycode = KC_SLASH, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },

                            R_TOP_IDX   = &(smart_key_t){ .tap.keycode = KC_P, },
                            R_TOP_MID   = &(smart_key_t){ .tap.keycode = KC_GRV, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
                            R_TOP_RING  = &(smart_key_t){ .tap.keycode = KC_Q, },

                            L_HOME_MID  = &(smart_key_t){ .tap.keycode = KC_COMMA, },
                            L_HOME_IDX  = &(smart_key_t){ .tap.keycode = KC_DOT, },

                            R_HOME_IDX  = &(smart_key_t){ .tap.keycode = KC_Y, },
                            R_HOME_MID  = &(smart_key_t){ .tap.keycode = KC_K, },
                            R_HOME_PINK = &(smart_key_t){ .tap.keycode = KC_J, },
                            R_HOME_RING = &(smart_key_t){ .tap.action = &magickey_action2, },

                            L_BOT_IDX   = &(smart_key_t){ .tap.keycode = KC_V, },

                            R_BOT_MID   = &(smart_key_t){ .tap.keycode = KC_Z, },
                            R_BOT_RING  = &(smart_key_t){ .tap.keycode = KC_SEMICOLON, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
                            R_BOT_PINK  = &(smart_key_t){ .tap.keycode = KC_SEMICOLON, },

                            L_THUMB_IN  = &(smart_key_t){ .tap.action = &smart_case, },
                        },
                        .on_layer_activate = &deactivate_on_other_layer,
                    },

                [LAYER_SYMBOLS_2] =
                    &(smart_layer_t){
                        .map = {
                            R_TOP_IDX   = &(smart_key_t){ .tap.keycode = KC_QUOTE, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
                            R_TOP_MID   = &(smart_key_t){ .tap.keycode = KC_QUOTE, },
                            R_TOP_RING  = &(smart_key_t){ .tap.keycode = KC_GRV, },

                            R_HOME_IDX  = &(smart_key_t){ .tap.keycode = KC_9, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), .max_tap = 2, .tap.action = &tap_multi_matching_brace, },
                            R_HOME_MID  = &(smart_key_t){ .tap.keycode = KC_LEFT_BRACKET, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
                            R_HOME_RING = &(smart_key_t){ .tap.keycode = KC_1, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
                            R_HOME_PINK = &(smart_key_t){ .tap.keycode = KC_3, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },

                            R_BOT_MID   = &(smart_key_t){ .tap.keycode = KC_COMMA, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
                            R_BOT_RING  = &(smart_key_t){ .tap.keycode = KC_LEFT_BRACKET, },
                            R_BOT_PINK  = &(smart_key_t){ .tap.keycode = KC_4, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
                        },
                    },

                [LAYER_SYMBOLS] =
                    &(smart_layer_t){
                        .map = {
                            R_TOP_IDX   = &(smart_key_t){ .tap.keycode = KC_1, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
                            R_TOP_MID   = &(smart_key_t){ .tap.keycode = KC_SLASH, },
                            R_TOP_RING  = &(smart_key_t){ .tap.keycode = KC_7, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },

                            R_HOME_IDX  = &(smart_key_t){ .tap.keycode = KC_SEMICOLON, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
                            R_HOME_MID  = &(smart_key_t){ .tap.keycode = KC_MINUS, },
                            R_HOME_RING = &(smart_key_t){ .tap.keycode = KC_EQUAL, },
                            R_HOME_PINK = &(smart_key_t){ .tap.keycode = KC_BACKSLASH, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },

                            R_BOT_MID   = &(smart_key_t){ .tap.keycode = KC_MINUS, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
                            R_BOT_RING  = &(smart_key_t){ .tap.keycode = KC_8, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
                            R_BOT_PINK  = &(smart_key_t){ .tap.keycode = KC_BACKSLASH, },
                        },
                    },

                // Closing brackets
                [LAYER_H_HOLD] =
                    &(smart_layer_t){
                        .map = {
                                L_BOT_IDX = &(smart_key_t){ .tap.keycode = KC_DOT, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
                            },
                    },

                [LAYER_VIM_TEXT] =
                    &(smart_layer_t){
                        .map = {
                                L_TOP_RING  = &(smart_key_t){ .tap.keycode = KC_P, .tap.mask = MOD_BIT(KC_LEFT_SHIFT), },
                                L_TOP_MID   = &(smart_key_t){ .tap.keycode = KC_P, },
                                L_TOP_IDX   = &(smart_key_t){ .tap.keycode = KC_X, },

                                L_HOME_PINK = &(smart_key_t){ .tap.action = &vim_blackhole_register, },
                                L_HOME_RING = &(smart_key_t){ .tap.keycode = KC_J, .tap.mask = MOD_BIT(KC_LEFT_SHIFT), },
                                L_HOME_MID  = &(smart_key_t){ .tap.keycode = KC_V, },
                                L_HOME_IDX  = &(smart_key_t){ .tap.keycode = KC_Y, },

                                L_BOT_RING  = &(smart_key_t){ .tap.keycode = KC_V, .tap.mask = MOD_BIT(KC_LEFT_CTRL), },
                                L_BOT_MID   = &(smart_key_t){ .tap.keycode = KC_V, .tap.mask = MOD_BIT(KC_LEFT_SHIFT), },
                                L_BOT_IDX   = &(smart_key_t){ .tap.keycode = KC_0, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
                            },
                    },

                [LAYER_VIM_NAV] =
                    &(smart_layer_t){
                        .map = {
                                L_TOP_RING  = &(smart_key_t){ .tap.keycode = KC_B, .tap.mask = MOD_BIT(KC_LEFT_SHIFT), },
                                L_TOP_MID   = &(smart_key_t){ .tap.keycode = KC_H, .tap.mask = MOD_BIT(KC_LEFT_SHIFT), },
                                L_TOP_IDX   = &(smart_key_t){ .tap.keycode = KC_W, .tap.mask = MOD_BIT(KC_LEFT_SHIFT), },

                                L_HOME_PINK = &(smart_key_t){ .tap.keycode = KC_6, .tap.mask = MOD_BIT(KC_LEFT_SHIFT), },
                                L_HOME_RING = &(smart_key_t){ .tap.keycode = KC_U, .tap.mask = MOD_BIT(KC_LEFT_CTRL), },
                                L_HOME_MID  = &(smart_key_t){ .tap.keycode = KC_M, .tap.mask = MOD_BIT(KC_LEFT_SHIFT), },
                                L_HOME_IDX  = &(smart_key_t){ .tap.keycode = KC_4, .tap.mask = MOD_BIT(KC_LEFT_SHIFT), },

                                L_BOT_RING  = &(smart_key_t){ .tap.keycode = KC_D, .tap.mask = MOD_BIT(KC_LEFT_CTRL), },
                                L_BOT_MID   = &(smart_key_t){ .tap.keycode = KC_L, .tap.mask = MOD_BIT(KC_LEFT_SHIFT), },
                                L_BOT_IDX   = &(smart_key_t){ .tap.keycode = KC_RIGHT_BRACKET, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
                            },
                    },

                [LAYER_L_HOLD] =
                    &(smart_layer_t){
                        .map = {
                                L_BOT_IDX = &(smart_key_t){ .tap.keycode = KC_RIGHT_BRACKET, },
                            },
                    },

                [LAYER_SYS] =
                    &(smart_layer_t){
                        .map = {
                            L_TOP_RING  = &(smart_key_t){ .tap.keycode = KC_TAB, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
                            L_TOP_MID   = &(smart_key_t){ .tap.keycode = KC_INSERT, },
                            L_TOP_IDX   = &(smart_key_t){ .tap.keycode = KC_GRV, },

                            R_TOP_IDX   = &(smart_key_t){ .tap.keycode = KC_BACKSPACE, },
                            R_TOP_MID   = &(smart_key_t){ .tap.keycode = KC_UP, },
                            R_TOP_RING  = &(smart_key_t){ .tap.keycode = KC_DELETE, },

                            L_HOME_PINK = &(smart_key_t){ .tap.action = &tap_ctrl_r, },
                            L_HOME_RING = &(smart_key_t){ .tap.keycode = KC_ESC, },
                            L_HOME_MID  = &(smart_key_t){ .tap.keycode = KC_ENT, },
                            L_HOME_IDX  = &(smart_key_t){ .tap.keycode = KC_TAB, },

                            R_HOME_IDX  = &(smart_key_t){ .tap.keycode = KC_LEFT, },
                            R_HOME_MID  = &(smart_key_t){ .tap.keycode = KC_DOWN, },
                            R_HOME_RING = &(smart_key_t){ .tap.keycode = KC_RIGHT, },
                            R_HOME_PINK = &(smart_key_t){ .tap.action = &tap_ctrl_z, },

                            L_BOT_RING  = &(smart_key_t){ .hold.action = &jump_layer, },
                            L_BOT_MID   = &(smart_key_t){ .tap.mask_oneshot = MOD_BIT_LCTRL | MOD_BIT_LALT | MOD_BIT_LGUI, },
                            L_BOT_IDX   = &(smart_key_t){ .tap.action = &activate_tmux, },

                            R_BOT_MID   = &(smart_key_t){ .tap.action = &tap_ctrl_c, },
                            R_BOT_RING  = &(smart_key_t){ .tap.action = &tap_ctrl_x, },
                            R_BOT_PINK  = &(smart_key_t){ .tap.action = &tap_ctrl_v, },

                            R_THUMB_IN  = &(smart_key_t){ .tap.action = &clear, },
                            R_THUMB_OUT = &(smart_key_t){ .tap.action = &tap_ctrl_a, },
                            },
                    },

                [LAYER_NUM] = &(smart_layer_t){
                        .map = {
                                L_TOP_RING  = &(smart_key_t){ .tap.keycode = KC_6, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
                                L_TOP_MID   = &(smart_key_t){ .tap.keycode = KC_5, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
                                L_TOP_IDX   = &(smart_key_t){ .tap.keycode = KC_SEMICOLON, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },

                                R_TOP_IDX   = &(smart_key_t){ .tap.keycode = KC_BACKSPACE, },
                                R_TOP_MID   = &(smart_key_t){ .tap.keycode = KC_MINUS, },
                                R_TOP_RING  = &(smart_key_t){ .tap.keycode = KC_EQUAL, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },

                                L_HOME_PINK = &(smart_key_t){ .tap.keycode = KC_8, },
                                L_HOME_RING = &(smart_key_t){ .tap.keycode = KC_7, },
                                L_HOME_MID  = &(smart_key_t){ .tap.keycode = KC_6, },
                                L_HOME_IDX  = &(smart_key_t){ .tap.keycode = KC_5, },

                                R_HOME_IDX  = &(smart_key_t){ .tap.keycode = KC_1, },
                                R_HOME_MID  = &(smart_key_t){ .tap.keycode = KC_2, },
                                R_HOME_RING = &(smart_key_t){ .tap.keycode = KC_3, },
                                R_HOME_PINK = &(smart_key_t){ .tap.keycode = KC_4, },

                                L_BOT_RING  = &(smart_key_t){ .tap.keycode = KC_COMMA, },
                                L_BOT_MID   = &(smart_key_t){ .tap.keycode = KC_DOT, },
                                L_BOT_IDX   = &(smart_key_t){ .tap.keycode = KC_9, },

                                R_BOT_MID   = &(smart_key_t){ .tap.keycode = KC_0, },
                                R_BOT_RING  = &(smart_key_t){ .tap.keycode = KC_8, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
                                R_BOT_PINK  = &(smart_key_t){ .tap.keycode = KC_SLASH, },

                                L_THUMB_OUT = &(smart_key_t){ .max_tap = 3, .tap.action = &toggle_os_index, .hold.action = &print_magic_stats, },
                            },
                    },

                [LAYER_JUMP_MAC] = &(smart_layer_t){
                        .map = {
                                R_TOP_IDX   = &(smart_key_t){ .tap.keycode = KC_BACKSPACE, .tap.mask = MOD_BIT(KC_ROPT), },
                                R_TOP_MID   = &(smart_key_t){ .tap.keycode = KC_PAGE_UP, },

                                R_HOME_IDX  = &(smart_key_t){ .tap.keycode = KC_LEFT, .tap.mask = MOD_BIT(KC_ROPT), .hold.keycode = KC_LEFT, .hold.mask = MOD_BIT(KC_LEFT_GUI), },
                                R_HOME_MID  = &(smart_key_t){ .tap.keycode = KC_PAGE_DOWN, },
                                R_HOME_RING = &(smart_key_t){ .tap.keycode = KC_RIGHT, .tap.mask = MOD_BIT(KC_ROPT), .hold.keycode = KC_RIGHT, .hold.mask = MOD_BIT(KC_LEFT_GUI), },
                            },
                    },

                [LAYER_JUMP_WIN] = &(smart_layer_t){
                        .map = {
                                R_TOP_IDX   = &(smart_key_t){ .tap.keycode = KC_BACKSPACE, .tap.mask = MOD_BIT(KC_RIGHT_CTRL), },
                                R_TOP_MID   = &(smart_key_t){ .tap.keycode = KC_PAGE_UP, },
                                R_TOP_RING  = &(smart_key_t){ .tap.keycode = KC_DELETE, .tap.mask = MOD_BIT(KC_RIGHT_CTRL), },

                                R_HOME_IDX  = &(smart_key_t){ .tap.keycode = KC_LEFT, .tap.mask = MOD_BIT(KC_RIGHT_CTRL), .hold.keycode = KC_HOME, },
                                R_HOME_MID  = &(smart_key_t){ .tap.keycode = KC_PAGE_DOWN, },
                                R_HOME_RING = &(smart_key_t){ .tap.keycode = KC_RIGHT, .tap.mask = MOD_BIT(KC_RIGHT_CTRL), .hold.keycode = KC_END, },
                            },
                    },

                [LAYER_FUNCTION_KEYS] = &(smart_layer_t){
                        .map = {
                                L_TOP_RING  = &(smart_key_t){ .tap.keycode = KC_F12, },
                                L_TOP_MID   = &(smart_key_t){ .tap.keycode = KC_F11, },
                                L_TOP_IDX   = &(smart_key_t){ .tap.keycode = KC_F10, },

                                R_TOP_IDX   = &(smart_key_t){ .tap.keycode = KC_F7, },
                                R_TOP_MID   = &(smart_key_t){ .tap.keycode = KC_F8, },
                                R_TOP_RING  = &(smart_key_t){ .tap.keycode = KC_F9, },

                                L_HOME_RING = &(smart_key_t){ .tap.keycode = KC_F6, },
                                L_HOME_MID  = &(smart_key_t){ .tap.keycode = KC_F5, },
                                L_HOME_IDX  = &(smart_key_t){ .tap.keycode = KC_F4, },

                                R_HOME_IDX  = &(smart_key_t){ .tap.keycode = KC_F1, },
                                R_HOME_MID  = &(smart_key_t){ .tap.keycode = KC_F2, },
                                R_HOME_RING = &(smart_key_t){ .tap.keycode = KC_F3, },
                            },
                    },

                [LAYER_DUMB] = &(smart_layer_t){
                        .map = {
                                L_TOP_RING  = &(smart_key_t){ .tap.keycode = KC_B, },
                                L_TOP_MID   = &(smart_key_t){ .tap.keycode = KC_L, },
                                L_TOP_IDX   = &(smart_key_t){ .tap.keycode = KC_W, },

                                L_HOME_PINK = &(smart_key_t){ .tap.keycode = KC_LEFT_SHIFT, },
                                L_HOME_RING = &(smart_key_t){ .tap.keycode = KC_A, },
                                L_HOME_MID  = &(smart_key_t){ .tap.keycode = KC_S, },
                                L_HOME_IDX  = &(smart_key_t){ .tap.keycode = KC_T, },

                                L_BOT_PINK  = &(smart_key_t){ .tap.keycode = KC_LEFT_CTRL, },
                                L_BOT_RING  = &(smart_key_t){ .tap.keycode = KC_M, },
                                L_BOT_MID   = &(smart_key_t){ .tap.keycode = KC_G, },
                                L_BOT_IDX   = &(smart_key_t){ .tap.keycode = KC_D, },

                                L_THUMB_IN  = &(smart_key_t){ .tap.keycode = KC_SPC, },
                                L_THUMB_OUT = &(smart_key_t){ .hold.layer = LAYER_DUMB_2, },
                            },
                    },

                [LAYER_DUMB_2] = &(smart_layer_t){
                        .map = {
                                L_TOP_RING  = &(smart_key_t){ .tap.keycode = KC_3, },
                                L_TOP_MID   = &(smart_key_t){ .tap.keycode = KC_2, },
                                L_TOP_IDX   = &(smart_key_t){ .tap.keycode = KC_1, },

                                L_HOME_RING = &(smart_key_t){ .tap.keycode = KC_ESC, },
                                L_HOME_MID  = &(smart_key_t){ .tap.keycode = KC_ENTER, },
                                L_HOME_IDX  = &(smart_key_t){ .tap.keycode = KC_TAB, },

                                L_BOT_RING  = &(smart_key_t){ .tap.keycode = KC_7, },
                                L_BOT_MID   = &(smart_key_t){ .tap.keycode = KC_8, },
                                L_BOT_IDX   = &(smart_key_t){ .tap.keycode = KC_9, },

                                L_THUMB_IN  = &(smart_key_t){ .max_tap = 3, .tap.action = &toggle_os_index, .hold.action = &print_magic_stats, },
                            },
                    },

                [LAYER_TMUX] = &(smart_layer_t){
                        .map = {
                                L_TOP_RING  = &(smart_key_t){ .tap.keycode = KC_P, },
                                L_TOP_MID   = &(smart_key_t){ .tap.keycode = KC_5, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
                                L_TOP_IDX   = &(smart_key_t){ .tap.keycode = KC_RIGHT_BRACKET, },

                                L_HOME_PINK = &(smart_key_t){ .tap.keycode = KC_Z, },
                                L_HOME_RING = &(smart_key_t){ .tap.layer_toggle = LAYER_TMUX, },
                                L_HOME_MID  = &(smart_key_t){ .tap.keycode = KC_QUOTE, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
                                L_HOME_IDX  = &(smart_key_t){ .tap.keycode = KC_N, },

                                L_BOT_RING  = &(smart_key_t){ .tap.keycode = KC_LEFT_BRACKET, },
                                L_BOT_MID   = &(smart_key_t){ .tap.keycode = KC_C, },
                                L_BOT_IDX   = &(smart_key_t){ .tap.action = &activate_tmux, },
                            },
                    },

#ifdef MOUSEKEY_ENABLE
                [LAYER_MOUSE] = &(smart_layer_t){
                        .map = {
                                L_TOP_MID   = &(smart_key_t){ .tap.keycode = KC_MS_UP, },
                                R_TOP_MID   = &(smart_key_t){ .tap.keycode = KC_MS_WH_UP, },

                                L_HOME_RING = &(smart_key_t){ .tap.keycode = KC_MS_LEFT, },
                                L_HOME_MID  = &(smart_key_t){ .tap.keycode = KC_MS_DOWN, },
                                L_HOME_IDX  = &(smart_key_t){ .tap.keycode = KC_MS_RIGHT, },

                                R_HOME_IDX  = &(smart_key_t){ .tap.keycode = KC_MS_BTN1, },
                                R_HOME_MID  = &(smart_key_t){ .tap.keycode = KC_MS_WH_DOWN, },
                                R_HOME_RING = &(smart_key_t){ .tap.keycode = KC_MS_BTN2, },
                            },
                    },
#endif
};

smart_key_t smart_keys[2][SMART_KEY_COUNT] = {
    [0] = {
        [SMART_KEY_SMART_SHIFT] = {
                .keycode      = CKC_SMART_SHIFT,
                .speculative_hold = true,
                .tap.mask_oneshot = MOD_BIT_LSHIFT,
                .hold.keycode = KC_LEFT_SHIFT,
            },
        [SMART_KEY_SMART_CTRL] = {
                .keycode      = CKC_SMART_CTRL,
                .speculative_hold = true,
                .tap.mask_oneshot = MOD_BIT_LCTRL,
                .hold.keycode = KC_LEFT_CTRL,
            },
        [SMART_KEY_SMART_ALT] = {
                .keycode      = CKC_SMART_ALT,
                .speculative_hold = true,
                .tap.mask_oneshot = MOD_BIT_LALT,
                .hold.keycode = KC_LEFT_ALT,
            },
        [SMART_KEY_SMART_GUI] = {
                .keycode      = CKC_SMART_GUI,
                .speculative_hold = true,
                .tap.mask_oneshot = MOD_BIT_LGUI,
                .hold.keycode = KC_LEFT_GUI,
            },
        [SMART_KEY_SMART_CTRL_SHIFT] = {
            .keycode = CKC_SMART_CTRL_SHIFT,
            .speculative_hold = true,
            .tap.mask_oneshot = MOD_BIT_LCTRL | MOD_BIT_LSHIFT,
            .hold.keycode = KC_LEFT_CTRL,
            .hold.mask = MOD_BIT(KC_LEFT_SHIFT)
        },
    },
    [1] = {
        [SMART_KEY_SMART_SHIFT] = {
                .keycode      = CKC_SMART_SHIFT,
                .speculative_hold = true,
                .tap.mask_oneshot = MOD_BIT_LSHIFT,
                .hold.keycode = KC_LEFT_SHIFT,
            },
        [SMART_KEY_SMART_CTRL] = {
                .keycode      = CKC_SMART_CTRL,
                .speculative_hold = true,
                .tap.mask_oneshot = MOD_BIT_LGUI,
                .hold.keycode = KC_LCMD,
            },
        [SMART_KEY_SMART_ALT] = {
                .keycode      = CKC_SMART_ALT,
                .speculative_hold = true,
                .tap.mask_oneshot = MOD_BIT_LCTRL,
                .hold.keycode = KC_LEFT_CTRL,
            },
        [SMART_KEY_SMART_GUI] = {
                .keycode      = CKC_SMART_GUI,
                .speculative_hold = true,
                .tap.mask_oneshot = MOD_BIT_LALT,
                .hold.keycode = KC_LOPT,
            },
        [SMART_KEY_SMART_CTRL_SHIFT] = {
            .keycode = CKC_SMART_CTRL_SHIFT,
            .speculative_hold = true,
            .tap.mask_oneshot = MOD_BIT_LGUI | MOD_BIT_LSHIFT,
            .hold.keycode = KC_LCMD,
            .hold.mask = MOD_BIT(KC_LEFT_SHIFT)
        },
    }
};

smart_key_t *lookup_key(uint16_t keycode, keypos_t pos) {
    if (!pos.row && !pos.col) {
        for (int i = 0; i < SMART_KEY_COUNT; ++i) {
            if (keycode == smart_keys[os_index][i].keycode) {
                uprintf("DEBUG: found smart key by keycode %s\n", keycode_to_string(keycode));
                return &smart_keys[os_index][i];
            }
        }
        uprintf("DEBUG: found smart key by falling back to ALPHA1 %s\n", keycode_to_string(keycode));
        return smart_layers[LAYER_ALPHA_1]->map[pos.row][pos.col];
    }

    for (int i = 0; i < PENDING_QUEUE_MAX; i++) {
        smart_key_t *key = pending_keys[i].key;
        if (!key) {
            break;
        }
        if (is_same_pos(pos, key->pos)) {
            uprintf("DEBUG: found smart key in pressed keys %s\n", keycode_to_string(keycode));
            return key;
        }
    }

    for (int i = 0; i < SMART_LAYER_COUNT && active_layers[i]; ++i) {
        smart_key_t *key = smart_layers[active_layers[i]]->map[pos.row][pos.col];
        if (key) {
            uprintf("DEBUG: found smart in layer %s\n", layer_to_string(active_layers[i]));
            return key;
        }
    }
    return smart_layers[LAYER_ALPHA_1]->map[pos.row][pos.col];
}

bool oneshot_on_key_press_default(smart_layer_t *l, uint16_t keycode, uint16_t mask) {
    switch (keycode) {
        case KC_LEFT_CTRL ... KC_RIGHT_GUI:
            return false;
        default:
            return true;
    }
}

void matrix_init_user(void) {
    // initialize keycodes
    for (int i = 0; i < MATRIX_ROWS; ++i) {
        for (int j = 0; j < MATRIX_COLS; ++j) {
            for (int l = 0; l < SMART_LAYER_COUNT; ++l) {
                smart_key_t *key = smart_layers[l]->map[i][j];
                if (key) {
                    // copy position
                    key->pos = (keypos_t){
                        .row = i,
                        .col = j,
                    };
                    // copy keycode from base layer
                    if (!key->keycode) {
                        key->keycode = keymaps[WIN_BASE][i][j];
                    }
                    // copy keycode from base layer
                    if (key->tap.keycode == KC_TRANSPARENT) {
                        key->tap.keycode = keymaps[WIN_BASE][i][j];
                    }
                }
            }
        }
    }

    // default layer behavior
    for (int i = 1; i < SMART_LAYER_COUNT; i++) {
        if (!smart_layers[i]->oneshot_on_key_press) {
            smart_layers[i]->oneshot_on_key_press = &oneshot_on_key_press_default;
        }
    }
}
