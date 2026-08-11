// Copyright 2025 squ94wk
// SPDX-License-Identifier: GPL-2.0-or-later

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

bool defer_release_except_space(smart_key_t *key, uint16_t keycode) {
    return keycode != KC_SPACE;
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
        register_with_mods(PTR_TO(KC_LEFT_SHIFT), 0, NULL);
        key->state.release.keycode = KC_LEFT_SHIFT;
        // activate_layer(LAYER_NUM);
        // key->state.release.layer = LAYER_NUM;
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

void hold_mouse(smart_key_t *key) {
    switch (key->state.tap_count) {
    case 1:
        magickey_complete(key);
        return;
    case 2:
#ifdef MOUSEKEY_ENABLE
        activate_layer(LAYER_MOUSE);
        key->state.release.layer = LAYER_MOUSE;
#endif
        return;
    }
}

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

// ============================================================================
// LAYER DEFINITIONS
// ============================================================================
// RP2040 Limitation: Compound literals containing function pointers cause issues.
//
// Workaround Pattern:
// 1. Define layers below with only the keys relevant to that layer:
//    L_HOME_RING = &(smart_key_t){}
//
// 2. Assign fields and most importanty function pointers at runtime in lazy_init_layers():
//    smart_layers[LAYER_ALPHA_1]->map[1][1]->tap.action = &func;
// ============================================================================
smart_layer_t * smart_layers[SMART_LAYER_COUNT] = {
                [LAYER_ALPHA_1] =
                    &(smart_layer_t){
                        .map = {
                                L_HOME_RING = &(smart_key_t){},
                                L_HOME_MID  = &(smart_key_t){},
                                L_HOME_IDX  = &(smart_key_t){},
                                R_HOME_IDX  = &(smart_key_t){},
                                R_HOME_MID  = &(smart_key_t){},
                                R_HOME_RING = &(smart_key_t){},
                                L_BOT_RING  = &(smart_key_t){},
                                L_BOT_IDX   = &(smart_key_t){},
                                R_BOT_IDX   = &(smart_key_t){},
                                R_BOT_MID  = &(smart_key_t){},
                                R_BOT_RING  = &(smart_key_t){},
                                L_THUMB_IN  = &(smart_key_t){},
                                L_THUMB_OUT = &(smart_key_t){},
                                R_THUMB_OUT  = &(smart_key_t){},
                                R_THUMB_IN = &(smart_key_t){},
                            },
                    },

                [LAYER_ALPHA_2] =
                    &(smart_layer_t){
                        .map = {
                                L_TOP_RING  = &(smart_key_t){},
                                L_TOP_MID   = &(smart_key_t){},
                                L_TOP_IDX   = &(smart_key_t){},
                                R_TOP_IDX   = &(smart_key_t){},
                                R_TOP_MID   = &(smart_key_t){},
                                R_TOP_RING  = &(smart_key_t){},
                                L_HOME_RING  = &(smart_key_t){},
                                L_HOME_MID  = &(smart_key_t){},
                                L_HOME_IDX  = &(smart_key_t){},
                                R_HOME_IDX  = &(smart_key_t){},
                                R_HOME_MID  = &(smart_key_t){},
                                R_HOME_PINK = &(smart_key_t){},
                                R_HOME_RING = &(smart_key_t){},
                                L_BOT_RING  = &(smart_key_t){},
                                L_BOT_MID   = &(smart_key_t){},
                                L_BOT_IDX   = &(smart_key_t){},
                                R_BOT_IDX   = &(smart_key_t){},
                                R_BOT_MID  = &(smart_key_t){},
                                R_BOT_RING  = &(smart_key_t){},
                                L_THUMB_IN  = &(smart_key_t){},
                            },
                    },

                [LAYER_SYMBOLS_2] =
                    &(smart_layer_t){
                        .map = {
                                R_TOP_IDX   = &(smart_key_t){},
                                R_TOP_MID   = &(smart_key_t){},
                                R_TOP_RING  = &(smart_key_t){},
                                R_HOME_IDX  = &(smart_key_t){},
                                R_HOME_MID  = &(smart_key_t){},
                                R_HOME_RING = &(smart_key_t){},
                                R_HOME_PINK = &(smart_key_t){},
                                R_BOT_IDX   = &(smart_key_t){},
                                R_BOT_MID  = &(smart_key_t){},
                                R_BOT_RING  = &(smart_key_t){},
                            },
                    },

                [LAYER_SYMBOLS] =
                    &(smart_layer_t){
                        .map = {
                                R_TOP_IDX   = &(smart_key_t){},
                                R_TOP_MID   = &(smart_key_t){},
                                R_TOP_RING  = &(smart_key_t){},
                                R_HOME_IDX  = &(smart_key_t){},
                                R_HOME_MID  = &(smart_key_t){},
                                R_HOME_RING = &(smart_key_t){},
                                R_HOME_PINK = &(smart_key_t){},
                                R_BOT_IDX   = &(smart_key_t){},
                                R_BOT_MID  = &(smart_key_t){},
                                R_BOT_RING  = &(smart_key_t){},
                            },
                    },

                // Closing brackets
                [LAYER_H_HOLD] =
                    &(smart_layer_t){
                        .map = {
                                L_BOT_IDX = &(smart_key_t){},
                            },
                    },

                [LAYER_VIM_TEXT] =
                    &(smart_layer_t){
                        .map = {
                                L_TOP_RING  = &(smart_key_t){},
                                L_TOP_MID   = &(smart_key_t){},
                                L_TOP_IDX   = &(smart_key_t){},
                                L_HOME_PINK = &(smart_key_t){},
                                L_HOME_RING = &(smart_key_t){},
                                L_HOME_MID  = &(smart_key_t){},
                                L_HOME_IDX  = &(smart_key_t){},
                                L_BOT_RING  = &(smart_key_t){},
                                L_BOT_MID   = &(smart_key_t){},
                                L_BOT_IDX   = &(smart_key_t){},
                            },
                    },

                [LAYER_VIM_NAV] =
                    &(smart_layer_t){
                        .map = {
                                L_TOP_RING  = &(smart_key_t){},
                                L_TOP_MID   = &(smart_key_t){},
                                L_TOP_IDX   = &(smart_key_t){},
                                L_HOME_PINK = &(smart_key_t){},
                                L_HOME_RING = &(smart_key_t){},
                                L_HOME_MID  = &(smart_key_t){},
                                L_HOME_IDX  = &(smart_key_t){},
                                L_BOT_RING  = &(smart_key_t){},
                                L_BOT_MID   = &(smart_key_t){},
                                L_BOT_IDX   = &(smart_key_t){},
                                L_BOT_PINK  = &(smart_key_t){},
                            },
                    },

                [LAYER_L_HOLD] =
                    &(smart_layer_t){
                        .map = {
                                L_BOT_IDX = &(smart_key_t){},
                            },
                    },

                [LAYER_SYS] =
                    &(smart_layer_t){
                        .map = {
                                L_TOP_RING  = &(smart_key_t){},
                                L_TOP_MID   = &(smart_key_t){},
                                L_TOP_IDX   = &(smart_key_t){},
                                R_TOP_IDX   = &(smart_key_t){},
                                R_TOP_MID   = &(smart_key_t){},
                                R_TOP_RING  = &(smart_key_t){},
                                L_HOME_PINK = &(smart_key_t){},
                                L_HOME_RING = &(smart_key_t){},
                                L_HOME_MID  = &(smart_key_t){},
                                L_HOME_IDX  = &(smart_key_t){},
                                R_HOME_IDX  = &(smart_key_t){},
                                R_HOME_MID  = &(smart_key_t){},
                                R_HOME_RING = &(smart_key_t){},
                                R_HOME_PINK = &(smart_key_t){},
                                L_BOT_RING  = &(smart_key_t){},
                                L_BOT_MID   = &(smart_key_t){},
                                L_BOT_IDX   = &(smart_key_t){},
                                R_BOT_IDX   = &(smart_key_t){},
                                R_BOT_MID  = &(smart_key_t){},
                                R_BOT_RING  = &(smart_key_t){},
                                R_THUMB_OUT  = &(smart_key_t){},
                                R_THUMB_IN = &(smart_key_t){},
                            },
                    },

                [LAYER_NUM] = &(smart_layer_t){
                        .map = {
                                L_TOP_RING  = &(smart_key_t){},
                                L_TOP_MID   = &(smart_key_t){},
                                L_TOP_IDX   = &(smart_key_t){},
                                R_TOP_IDX   = &(smart_key_t){},
                                R_TOP_MID   = &(smart_key_t){},
                                R_TOP_RING  = &(smart_key_t){},
                                L_HOME_PINK = &(smart_key_t){},
                                L_HOME_RING = &(smart_key_t){},
                                L_HOME_MID  = &(smart_key_t){},
                                L_HOME_IDX  = &(smart_key_t){},
                                R_HOME_IDX  = &(smart_key_t){},
                                R_HOME_MID  = &(smart_key_t){},
                                R_HOME_RING = &(smart_key_t){},
                                R_HOME_PINK = &(smart_key_t){},
                                L_BOT_RING  = &(smart_key_t){},
                                L_BOT_MID   = &(smart_key_t){},
                                L_BOT_IDX   = &(smart_key_t){},
                                R_BOT_IDX   = &(smart_key_t){},
                                R_BOT_MID  = &(smart_key_t){},
                                R_BOT_RING  = &(smart_key_t){},
                                L_THUMB_OUT = &(smart_key_t){},
                            },
                    },

                [LAYER_JUMP_MAC] = &(smart_layer_t){
                        .map = {
                                R_TOP_IDX   = &(smart_key_t){},
                                R_TOP_MID   = &(smart_key_t){},
                                R_HOME_IDX  = &(smart_key_t){},
                                R_HOME_MID  = &(smart_key_t){},
                                R_HOME_RING = &(smart_key_t){},
                            },
                    },

                [LAYER_JUMP_WIN] = &(smart_layer_t){
                        .map = {
                                R_TOP_IDX   = &(smart_key_t){},
                                R_TOP_MID   = &(smart_key_t){},
                                R_TOP_RING  = &(smart_key_t){},
                                R_HOME_IDX  = &(smart_key_t){},
                                R_HOME_MID  = &(smart_key_t){},
                                R_HOME_RING = &(smart_key_t){},
                            },
                    },

                [LAYER_FUNCTION_KEYS] = &(smart_layer_t){
                        .map = {
                                L_TOP_RING  = &(smart_key_t){},
                                L_TOP_MID   = &(smart_key_t){},
                                L_TOP_IDX   = &(smart_key_t){},
                                R_TOP_IDX   = &(smart_key_t){},
                                R_TOP_MID   = &(smart_key_t){},
                                R_TOP_RING  = &(smart_key_t){},
                                L_HOME_RING = &(smart_key_t){},
                                L_HOME_MID  = &(smart_key_t){},
                                L_HOME_IDX  = &(smart_key_t){},
                                R_HOME_IDX  = &(smart_key_t){},
                                R_HOME_MID  = &(smart_key_t){},
                                R_HOME_RING = &(smart_key_t){},
                            },
                    },

                [LAYER_MEDIA] = &(smart_layer_t){
                        .map = {
                                R_TOP_IDX   = &(smart_key_t){},
                                R_TOP_MID   = &(smart_key_t){},
                                R_TOP_RING  = &(smart_key_t){},
                            },
                    },

                [LAYER_DUMB] = &(smart_layer_t){
                        .map = {
                                L_TOP_RING  = &(smart_key_t){},
                                L_TOP_MID   = &(smart_key_t){},
                                L_TOP_IDX   = &(smart_key_t){},
                                L_HOME_PINK = &(smart_key_t){},
                                L_HOME_RING = &(smart_key_t){},
                                L_HOME_MID  = &(smart_key_t){},
                                L_HOME_IDX  = &(smart_key_t){},
                                L_BOT_RING  = &(smart_key_t){},
                                L_BOT_MID   = &(smart_key_t){},
                                L_BOT_IDX   = &(smart_key_t){},
                                L_THUMB_IN  = &(smart_key_t){},
                                L_THUMB_OUT = &(smart_key_t){},
                            },
                    },

                [LAYER_DUMB_2] = &(smart_layer_t){
                        .map = {
                                L_TOP_RING  = &(smart_key_t){},
                                L_TOP_MID   = &(smart_key_t){},
                                L_TOP_IDX   = &(smart_key_t){},
                                L_HOME_RING = &(smart_key_t){},
                                L_HOME_MID  = &(smart_key_t){},
                                L_HOME_IDX  = &(smart_key_t){},
                                L_BOT_RING  = &(smart_key_t){},
                                L_BOT_MID   = &(smart_key_t){},
                                L_BOT_IDX   = &(smart_key_t){},
                                L_THUMB_IN  = &(smart_key_t){},
                            },
                    },

                [LAYER_TMUX] = &(smart_layer_t){
                        .map = {
                                L_TOP_RING  = &(smart_key_t){},
                                L_TOP_MID   = &(smart_key_t){},
                                L_TOP_IDX   = &(smart_key_t){},
                                L_HOME_PINK = &(smart_key_t){},
                                L_HOME_RING = &(smart_key_t){},
                                L_HOME_MID  = &(smart_key_t){},
                                L_HOME_IDX  = &(smart_key_t){},
                                L_BOT_PINK  = &(smart_key_t){},
                                L_BOT_RING  = &(smart_key_t){},
                                L_BOT_MID   = &(smart_key_t){},
                                L_BOT_IDX   = &(smart_key_t){},
                            },
                    },

#ifdef MOUSEKEY_ENABLE
                [LAYER_MOUSE] = &(smart_layer_t){
                        .map = {
                                L_TOP_MID   = &(smart_key_t){},
                                R_TOP_IDX   = &(smart_key_t){},
                                R_TOP_MID   = &(smart_key_t){},
                                R_TOP_RING  = &(smart_key_t){},
                                L_HOME_RING = &(smart_key_t){},
                                L_HOME_MID  = &(smart_key_t){},
                                L_HOME_IDX  = &(smart_key_t){},
                                R_HOME_IDX  = &(smart_key_t){},
                                R_HOME_MID  = &(smart_key_t){},
                                R_HOME_RING = &(smart_key_t){},
                                R_BOT_IDX   = &(smart_key_t){},
                                R_BOT_MID  = &(smart_key_t){},
                                R_BOT_RING  = &(smart_key_t){},
                            },
                    },
#endif
};

static void lazy_init_layers(void);

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
    lazy_init_layers();

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

// Lazy initialization - called on first key press
// This is necessary because RP2040 has issues with compound literals containing function pointers.
// The workaround is to define compound literals WITHOUT function pointers, then assign them at runtime.
static bool initialized = false;

static void lazy_init_layers(void) {
    if (initialized) return;
    initialized = true;

    // Step 3: Define key behaviors
    // RP2040 Issue: Compound literals with function pointers don't work reliably on this MCU.
    // Solution: Define empty compound literals above, assign ALL behaviors here.

    // ========================================================================
    // LAYER_ALPHA_1 - Main typing layer with smart modifiers
    // ========================================================================

    // L_HOME_RING - Tap: ESC (from base), Hold: Activate ALPHA_2 layer
    smart_layers[LAYER_ALPHA_1]->map L_HOME_RING->max_tap = 2;
    smart_layers[LAYER_ALPHA_1]->map L_HOME_RING->defer_release = true;
    smart_layers[LAYER_ALPHA_1]->map L_HOME_RING->tap.keycode = KC_TRANSPARENT;
    smart_layers[LAYER_ALPHA_1]->map L_HOME_RING->hold.action = &hold_alpha;
    smart_layers[LAYER_ALPHA_1]->map L_HOME_RING->hold_on_key_press = &always_on_other_press;

    // L_HOME_MID - Tap: S (from base), Hold: Left Shift
    smart_layers[LAYER_ALPHA_1]->map L_HOME_MID->tap.keycode = KC_TRANSPARENT;
    smart_layers[LAYER_ALPHA_1]->map L_HOME_MID->hold.keycode = KC_LEFT_SHIFT;

    // L_HOME_IDX - Tap: T (from base), Hold: SYMBOLS layer
    smart_layers[LAYER_ALPHA_1]->map L_HOME_IDX->tap.keycode = KC_TRANSPARENT;
    smart_layers[LAYER_ALPHA_1]->map L_HOME_IDX->hold.layer = LAYER_SYMBOLS;

    // R_HOME_IDX - Tap: N (from base), Hold: VIM_TEXT layer
    smart_layers[LAYER_ALPHA_1]->map R_HOME_IDX->tap.keycode = KC_TRANSPARENT;
    smart_layers[LAYER_ALPHA_1]->map R_HOME_IDX->hold.layer = LAYER_VIM_TEXT;

    // R_HOME_MID - Tap: E (from base), Hold: VIM_NAV layer
    smart_layers[LAYER_ALPHA_1]->map R_HOME_MID->tap.keycode = KC_TRANSPARENT;
    smart_layers[LAYER_ALPHA_1]->map R_HOME_MID->hold.layer = LAYER_VIM_NAV;

    // R_HOME_RING - Tap: Magic key completion, Hold: ALPHA_2 layer
    smart_layers[LAYER_ALPHA_1]->map R_HOME_RING->defer_release = true;
    smart_layers[LAYER_ALPHA_1]->map R_HOME_RING->tap.action = &magickey_action;
    smart_layers[LAYER_ALPHA_1]->map R_HOME_RING->hold.layer = LAYER_ALPHA_2;
    smart_layers[LAYER_ALPHA_1]->map R_HOME_RING->hold_on_key_press = &always_on_other_press;
    smart_layers[LAYER_ALPHA_1]->map R_HOME_RING->defer_release_on_key = &defer_release_except_space;

    // L_BOT_RING - Tap: R (from base)
    smart_layers[LAYER_ALPHA_1]->map L_BOT_RING->tap.keycode = KC_TRANSPARENT;

    // L_BOT_IDX - Tap: D (from base), Hold: SYMBOLS_2 layer
    smart_layers[LAYER_ALPHA_1]->map L_BOT_IDX->tap.keycode = KC_TRANSPARENT;
    smart_layers[LAYER_ALPHA_1]->map L_BOT_IDX->hold.layer = LAYER_SYMBOLS_2;

    // R_BOT_IDX - Tap: H (from base), Hold: H_HOLD layer (closing brackets)
    smart_layers[LAYER_ALPHA_1]->map R_BOT_IDX->tap.keycode = KC_TRANSPARENT;
    smart_layers[LAYER_ALPHA_1]->map R_BOT_IDX->hold.layer = LAYER_H_HOLD;

    // R_BOT_MID - Tap: . (from base), Hold: L_HOLD layer
    smart_layers[LAYER_ALPHA_1]->map R_BOT_MID->tap.keycode = KC_TRANSPARENT;
    smart_layers[LAYER_ALPHA_1]->map R_BOT_MID->hold.layer = LAYER_L_HOLD;

    // R_BOT_RING - Tap: / (from base)
    smart_layers[LAYER_ALPHA_1]->map R_BOT_RING->tap.keycode = KC_TRANSPARENT;

    // L_THUMB_IN - Tap: SPC (from base), Hold: SYS layer
    smart_layers[LAYER_ALPHA_1]->map L_THUMB_IN->tap.keycode = KC_TRANSPARENT;
    smart_layers[LAYER_ALPHA_1]->map L_THUMB_IN->hold.layer = LAYER_SYS;

    // L_THUMB_OUT - Tap: Repeat last key, Hold: NUM layer (with tap count)
    smart_layers[LAYER_ALPHA_1]->map L_THUMB_OUT->max_tap = 2;
    smart_layers[LAYER_ALPHA_1]->map L_THUMB_OUT->tap.action = &repeat_last_key;
    smart_layers[LAYER_ALPHA_1]->map L_THUMB_OUT->hold.action = &hold_num;
    smart_layers[LAYER_ALPHA_1]->map L_THUMB_OUT->hold_on_key_press = &always_on_other_press;

    // R_THUMB_OUT - Tap/Hold(1): Magic key complete, Hold(2): Mouse layer
    smart_layers[LAYER_ALPHA_1]->map R_THUMB_OUT->max_tap = 2;
    smart_layers[LAYER_ALPHA_1]->map R_THUMB_OUT->defer_release = true;
    smart_layers[LAYER_ALPHA_1]->map R_THUMB_OUT->hold.action = &hold_mouse;
    smart_layers[LAYER_ALPHA_1]->map R_THUMB_OUT->hold_on_key_press = &always_on_other_press;
    smart_layers[LAYER_ALPHA_1]->map R_THUMB_OUT->tap.action = &magickey_complete;

    // R_THUMB_IN - Tap: A (from base), Hold: NUM layer
    smart_layers[LAYER_ALPHA_1]->map R_THUMB_IN->tap.keycode = KC_TRANSPARENT;
    smart_layers[LAYER_ALPHA_1]->map R_THUMB_IN->hold.layer = LAYER_NUM;

    // ========================================================================
    // LAYER_ALPHA_2 - Secondary alpha layer with additional symbols
    // ========================================================================

    smart_layers[LAYER_ALPHA_2]->map L_TOP_RING->tap.keycode = KC_2;
    smart_layers[LAYER_ALPHA_2]->map L_TOP_RING->tap.mask = MOD_BIT(KC_RIGHT_SHIFT);
    smart_layers[LAYER_ALPHA_2]->map L_TOP_MID->tap.keycode = KC_X;
    smart_layers[LAYER_ALPHA_2]->map L_TOP_IDX->tap.keycode = KC_SLASH;
    smart_layers[LAYER_ALPHA_2]->map L_TOP_IDX->tap.mask = MOD_BIT(KC_RIGHT_SHIFT);
    smart_layers[LAYER_ALPHA_2]->map R_TOP_IDX->tap.keycode = KC_P;
    smart_layers[LAYER_ALPHA_2]->map R_TOP_MID->tap.keycode = KC_GRV;
    smart_layers[LAYER_ALPHA_2]->map R_TOP_MID->tap.mask = MOD_BIT(KC_RIGHT_SHIFT);
    smart_layers[LAYER_ALPHA_2]->map R_TOP_RING->tap.keycode = KC_Q;
    smart_layers[LAYER_ALPHA_2]->map L_HOME_MID->tap.keycode = KC_MINUS;
    smart_layers[LAYER_ALPHA_2]->map L_HOME_IDX->tap.keycode = KC_DOT;
    smart_layers[LAYER_ALPHA_2]->map R_HOME_IDX->tap.keycode = KC_Y;
    smart_layers[LAYER_ALPHA_2]->map R_HOME_MID->tap.keycode = KC_K;
    smart_layers[LAYER_ALPHA_2]->map R_HOME_PINK->tap.keycode = KC_J;
    smart_layers[LAYER_ALPHA_2]->map L_BOT_RING->tap.keycode = KC_SLASH;
    smart_layers[LAYER_ALPHA_2]->map L_BOT_MID->tap.keycode = KC_COMMA;
    smart_layers[LAYER_ALPHA_2]->map L_BOT_IDX->tap.keycode = KC_V;
    smart_layers[LAYER_ALPHA_2]->map R_BOT_IDX->tap.keycode = KC_Z;
    smart_layers[LAYER_ALPHA_2]->map R_BOT_MID->tap.keycode = KC_SEMICOLON;
    smart_layers[LAYER_ALPHA_2]->map R_BOT_MID->tap.mask = MOD_BIT(KC_RIGHT_SHIFT);
    smart_layers[LAYER_ALPHA_2]->map R_BOT_RING->tap.keycode = KC_SEMICOLON;

    // R_HOME_RING - Tap: Magic key action 2
    smart_layers[LAYER_ALPHA_2]->map L_HOME_RING->tap.action = &magickey_action2;
    smart_layers[LAYER_ALPHA_2]->map R_HOME_RING->tap.action = &magickey_action2;

    // L_THUMB_IN - Tap: Enable smart case
    smart_layers[LAYER_ALPHA_2]->map L_THUMB_IN->tap.action = &smart_case;

    // Layer behavior: Deactivate when other layer activates
    smart_layers[LAYER_ALPHA_2]->on_layer_activate = &deactivate_on_other_layer;

    // ========================================================================
    // LAYER_SYMBOLS_2 - Secondary symbols layer with brackets
    // ========================================================================

    smart_layers[LAYER_SYMBOLS_2]->map R_TOP_IDX->tap.keycode = KC_QUOTE;
    smart_layers[LAYER_SYMBOLS_2]->map R_TOP_IDX->tap.mask = MOD_BIT(KC_RIGHT_SHIFT);
    smart_layers[LAYER_SYMBOLS_2]->map R_TOP_MID->tap.keycode = KC_QUOTE;
    smart_layers[LAYER_SYMBOLS_2]->map R_TOP_RING->tap.keycode = KC_GRV;
    smart_layers[LAYER_SYMBOLS_2]->map R_HOME_IDX->tap.keycode = KC_9;
    smart_layers[LAYER_SYMBOLS_2]->map R_HOME_IDX->tap.mask = MOD_BIT(KC_RIGHT_SHIFT);
    smart_layers[LAYER_SYMBOLS_2]->map R_HOME_IDX->max_tap = 2;
    smart_layers[LAYER_SYMBOLS_2]->map R_HOME_MID->tap.keycode = KC_LEFT_BRACKET;
    smart_layers[LAYER_SYMBOLS_2]->map R_HOME_MID->tap.mask = MOD_BIT(KC_RIGHT_SHIFT);
    smart_layers[LAYER_SYMBOLS_2]->map R_HOME_RING->tap.keycode = KC_1;
    smart_layers[LAYER_SYMBOLS_2]->map R_HOME_RING->tap.mask = MOD_BIT(KC_RIGHT_SHIFT);
    smart_layers[LAYER_SYMBOLS_2]->map R_HOME_PINK->tap.keycode = KC_3;
    smart_layers[LAYER_SYMBOLS_2]->map R_HOME_PINK->tap.mask = MOD_BIT(KC_RIGHT_SHIFT);
    smart_layers[LAYER_SYMBOLS_2]->map R_BOT_IDX->tap.keycode = KC_COMMA;
    smart_layers[LAYER_SYMBOLS_2]->map R_BOT_IDX->tap.mask = MOD_BIT(KC_RIGHT_SHIFT);
    smart_layers[LAYER_SYMBOLS_2]->map R_BOT_MID->tap.keycode = KC_LEFT_BRACKET;
    smart_layers[LAYER_SYMBOLS_2]->map R_BOT_RING->tap.keycode = KC_4;
    smart_layers[LAYER_SYMBOLS_2]->map R_BOT_RING->tap.mask = MOD_BIT(KC_RIGHT_SHIFT);

    // R_HOME_IDX - Tap: Matching braces (tap once for opening, twice for both)
    smart_layers[LAYER_SYMBOLS_2]->map R_HOME_IDX->tap.action = &tap_multi_matching_brace;

    // ========================================================================
    // LAYER_SYMBOLS - Main symbols layer
    // ========================================================================

    smart_layers[LAYER_SYMBOLS]->map R_TOP_IDX->tap.keycode = KC_1;
    smart_layers[LAYER_SYMBOLS]->map R_TOP_IDX->tap.mask = MOD_BIT(KC_RIGHT_SHIFT);
    smart_layers[LAYER_SYMBOLS]->map R_TOP_MID->tap.keycode = KC_SLASH;
    smart_layers[LAYER_SYMBOLS]->map R_TOP_RING->tap.keycode = KC_7;
    smart_layers[LAYER_SYMBOLS]->map R_TOP_RING->tap.mask = MOD_BIT(KC_RIGHT_SHIFT);
    smart_layers[LAYER_SYMBOLS]->map R_HOME_IDX->tap.keycode = KC_SEMICOLON;
    smart_layers[LAYER_SYMBOLS]->map R_HOME_IDX->tap.mask = MOD_BIT(KC_RIGHT_SHIFT);
    smart_layers[LAYER_SYMBOLS]->map R_HOME_MID->tap.keycode = KC_MINUS;
    smart_layers[LAYER_SYMBOLS]->map R_HOME_RING->tap.keycode = KC_EQUAL;
    smart_layers[LAYER_SYMBOLS]->map R_HOME_PINK->tap.keycode = KC_BACKSLASH;
    smart_layers[LAYER_SYMBOLS]->map R_HOME_PINK->tap.mask = MOD_BIT(KC_RIGHT_SHIFT);
    smart_layers[LAYER_SYMBOLS]->map R_BOT_IDX->tap.keycode = KC_MINUS;
    smart_layers[LAYER_SYMBOLS]->map R_BOT_IDX->tap.mask = MOD_BIT(KC_RIGHT_SHIFT);
    smart_layers[LAYER_SYMBOLS]->map R_BOT_MID->tap.keycode = KC_8;
    smart_layers[LAYER_SYMBOLS]->map R_BOT_MID->tap.mask = MOD_BIT(KC_RIGHT_SHIFT);
    smart_layers[LAYER_SYMBOLS]->map R_BOT_RING->tap.keycode = KC_BACKSLASH;

    // ========================================================================
    // LAYER_H_HOLD - Closing brackets
    // ========================================================================

    smart_layers[LAYER_H_HOLD]->map L_BOT_IDX->tap.keycode = KC_DOT;
    smart_layers[LAYER_H_HOLD]->map L_BOT_IDX->tap.mask = MOD_BIT(KC_RIGHT_SHIFT);

    // ========================================================================
    // LAYER_VIM_TEXT - Vim text manipulation layer
    // ========================================================================

    smart_layers[LAYER_VIM_TEXT]->map L_TOP_RING->tap.keycode = KC_P;
    smart_layers[LAYER_VIM_TEXT]->map L_TOP_RING->tap.mask = MOD_BIT(KC_LEFT_SHIFT);
    smart_layers[LAYER_VIM_TEXT]->map L_TOP_MID->tap.keycode = KC_P;
    smart_layers[LAYER_VIM_TEXT]->map L_TOP_IDX->tap.keycode = KC_X;
    smart_layers[LAYER_VIM_TEXT]->map L_HOME_RING->tap.keycode = KC_J;
    smart_layers[LAYER_VIM_TEXT]->map L_HOME_RING->tap.mask = MOD_BIT(KC_LEFT_SHIFT);
    smart_layers[LAYER_VIM_TEXT]->map L_HOME_MID->tap.keycode = KC_V;
    smart_layers[LAYER_VIM_TEXT]->map L_HOME_IDX->tap.keycode = KC_Y;
    smart_layers[LAYER_VIM_TEXT]->map L_BOT_RING->tap.keycode = KC_V;
    smart_layers[LAYER_VIM_TEXT]->map L_BOT_RING->tap.mask = MOD_BIT(KC_LEFT_CTRL);
    smart_layers[LAYER_VIM_TEXT]->map L_BOT_MID->tap.keycode = KC_V;
    smart_layers[LAYER_VIM_TEXT]->map L_BOT_MID->tap.mask = MOD_BIT(KC_LEFT_SHIFT);
    smart_layers[LAYER_VIM_TEXT]->map L_BOT_IDX->tap.keycode = KC_0;
    smart_layers[LAYER_VIM_TEXT]->map L_BOT_IDX->tap.mask = MOD_BIT(KC_RIGHT_SHIFT);

    // L_HOME_PINK - Tap: Vim blackhole register
    smart_layers[LAYER_VIM_TEXT]->map L_HOME_PINK->tap.action = &vim_blackhole_register;

    // ========================================================================
    // LAYER_VIM_NAV - Vim navigation layer
    // ========================================================================

    smart_layers[LAYER_VIM_NAV]->map L_TOP_RING->tap.keycode = KC_B;
    smart_layers[LAYER_VIM_NAV]->map L_TOP_RING->tap.mask = MOD_BIT(KC_LEFT_SHIFT);
    smart_layers[LAYER_VIM_NAV]->map L_TOP_MID->tap.keycode = KC_H;
    smart_layers[LAYER_VIM_NAV]->map L_TOP_MID->tap.mask = MOD_BIT(KC_LEFT_SHIFT);
    smart_layers[LAYER_VIM_NAV]->map L_TOP_IDX->tap.keycode = KC_E;
    smart_layers[LAYER_VIM_NAV]->map L_TOP_IDX->tap.mask = MOD_BIT(KC_LEFT_SHIFT);
    smart_layers[LAYER_VIM_NAV]->map L_HOME_PINK->tap.keycode = KC_6;
    smart_layers[LAYER_VIM_NAV]->map L_HOME_PINK->tap.mask = MOD_BIT(KC_LEFT_SHIFT);
    smart_layers[LAYER_VIM_NAV]->map L_HOME_RING->tap.keycode = KC_U;
    smart_layers[LAYER_VIM_NAV]->map L_HOME_RING->tap.mask = MOD_BIT(KC_LEFT_CTRL);
    smart_layers[LAYER_VIM_NAV]->map L_HOME_MID->tap.keycode = KC_M;
    smart_layers[LAYER_VIM_NAV]->map L_HOME_MID->tap.mask = MOD_BIT(KC_LEFT_SHIFT);
    smart_layers[LAYER_VIM_NAV]->map L_HOME_IDX->tap.keycode = KC_4;
    smart_layers[LAYER_VIM_NAV]->map L_HOME_IDX->tap.mask = MOD_BIT(KC_LEFT_SHIFT);
    smart_layers[LAYER_VIM_NAV]->map L_BOT_RING->tap.keycode = KC_D;
    smart_layers[LAYER_VIM_NAV]->map L_BOT_RING->tap.mask = MOD_BIT(KC_LEFT_CTRL);
    smart_layers[LAYER_VIM_NAV]->map L_BOT_MID->tap.keycode = KC_L;
    smart_layers[LAYER_VIM_NAV]->map L_BOT_MID->tap.mask = MOD_BIT(KC_LEFT_SHIFT);
    smart_layers[LAYER_VIM_NAV]->map L_BOT_IDX->tap.keycode = KC_RIGHT_BRACKET;
    smart_layers[LAYER_VIM_NAV]->map L_BOT_IDX->tap.mask = MOD_BIT(KC_RIGHT_SHIFT);
    smart_layers[LAYER_VIM_NAV]->map L_BOT_PINK->tap.keycode = KC_4;
    smart_layers[LAYER_VIM_NAV]->map L_BOT_PINK->tap.mask = MOD_BIT(KC_LEFT_SHIFT);

    // ========================================================================
    // LAYER_L_HOLD
    // ========================================================================

    smart_layers[LAYER_L_HOLD]->map L_BOT_IDX->tap.keycode = KC_RIGHT_BRACKET;

    // ========================================================================
    // LAYER_SYS - System layer with navigation and control commands
    // ========================================================================

    smart_layers[LAYER_SYS]->map L_TOP_RING->tap.keycode = KC_TAB;
    smart_layers[LAYER_SYS]->map L_TOP_RING->tap.mask = MOD_BIT(KC_RIGHT_SHIFT);
    smart_layers[LAYER_SYS]->map L_TOP_MID->tap.keycode = KC_INSERT;
    smart_layers[LAYER_SYS]->map L_TOP_IDX->tap.keycode = KC_GRV;
    smart_layers[LAYER_SYS]->map R_TOP_IDX->tap.keycode = KC_BACKSPACE;
    smart_layers[LAYER_SYS]->map R_TOP_MID->tap.keycode = KC_UP;
    smart_layers[LAYER_SYS]->map R_TOP_RING->tap.keycode = KC_DELETE;
    smart_layers[LAYER_SYS]->map L_HOME_RING->tap.keycode = KC_ESC;
    smart_layers[LAYER_SYS]->map L_HOME_MID->tap.keycode = KC_ENT;
    smart_layers[LAYER_SYS]->map L_HOME_IDX->tap.keycode = KC_TAB;
    smart_layers[LAYER_SYS]->map R_HOME_IDX->tap.keycode = KC_LEFT;
    smart_layers[LAYER_SYS]->map R_HOME_MID->tap.keycode = KC_DOWN;
    smart_layers[LAYER_SYS]->map R_HOME_RING->tap.keycode = KC_RIGHT;
    smart_layers[LAYER_SYS]->map L_BOT_MID->tap.mask_oneshot = MOD_BIT_LCTRL | MOD_BIT_LALT | MOD_BIT_LGUI;

    // L_HOME_PINK - Tap: Ctrl+R (or Cmd+R on Mac)
    smart_layers[LAYER_SYS]->map L_HOME_PINK->tap.action = &tap_ctrl_r;

    // R_HOME_PINK - Tap: Ctrl+Z (or Cmd+Z on Mac) - Undo
    smart_layers[LAYER_SYS]->map R_HOME_PINK->tap.action = &tap_ctrl_z;

    // R_BOT_RING - Tap: Shift+Enter
    smart_layers[LAYER_SYS]->map R_BOT_RING->tap.keycode = KC_ENTER;
    smart_layers[LAYER_SYS]->map R_BOT_RING->tap.mask = MOD_BIT(KC_LEFT_SHIFT);

    // L_BOT_RING - Hold: Jump layer (word navigation, OS-aware)
    smart_layers[LAYER_SYS]->map L_BOT_RING->hold.action = &jump_layer;

    // L_BOT_IDX - Tap: Activate tmux mode
    smart_layers[LAYER_SYS]->map L_BOT_IDX->tap.action = &activate_tmux;

    // R_BOT_IDX - Tap: Ctrl+C (or Cmd+C on Mac) - Copy
    smart_layers[LAYER_SYS]->map R_BOT_IDX->tap.action = &tap_ctrl_c;

    // R_BOT_MID - Tap: Ctrl+X (or Cmd+X on Mac) - Cut
    smart_layers[LAYER_SYS]->map R_BOT_MID->tap.action = &tap_ctrl_x;

    // R_BOT_RING - Tap: Ctrl+V (or Cmd+V on Mac) - Paste
    smart_layers[LAYER_SYS]->map R_BOT_RING->tap.action = &tap_ctrl_v;

    // R_THUMB_OUT - Tap: Clear all modifiers and layers
    smart_layers[LAYER_SYS]->map R_THUMB_OUT->tap.action = &clear;

    // R_THUMB_IN - Tap: Ctrl+A (or Cmd+A on Mac) - Select all
    smart_layers[LAYER_SYS]->map R_THUMB_IN->tap.action = &tap_ctrl_a;

    // ========================================================================
    // LAYER_NUM - Number layer with numpad layout
    // ========================================================================

    smart_layers[LAYER_NUM]->map L_TOP_RING->tap.keycode = KC_6;
    smart_layers[LAYER_NUM]->map L_TOP_RING->tap.mask = MOD_BIT(KC_RIGHT_SHIFT);
    smart_layers[LAYER_NUM]->map L_TOP_MID->tap.keycode = KC_5;
    smart_layers[LAYER_NUM]->map L_TOP_MID->tap.mask = MOD_BIT(KC_RIGHT_SHIFT);
    smart_layers[LAYER_NUM]->map L_TOP_IDX->tap.keycode = KC_SEMICOLON;
    smart_layers[LAYER_NUM]->map L_TOP_IDX->tap.mask = MOD_BIT(KC_RIGHT_SHIFT);
    smart_layers[LAYER_NUM]->map R_TOP_IDX->tap.keycode = KC_BACKSPACE;
    smart_layers[LAYER_NUM]->map R_TOP_MID->tap.keycode = KC_MINUS;
    smart_layers[LAYER_NUM]->map R_TOP_RING->tap.keycode = KC_EQUAL;
    smart_layers[LAYER_NUM]->map R_TOP_RING->tap.mask = MOD_BIT(KC_RIGHT_SHIFT);
    smart_layers[LAYER_NUM]->map L_HOME_PINK->tap.keycode = KC_8;
    smart_layers[LAYER_NUM]->map L_HOME_RING->tap.keycode = KC_7;
    smart_layers[LAYER_NUM]->map L_HOME_MID->tap.keycode = KC_6;
    smart_layers[LAYER_NUM]->map L_HOME_IDX->tap.keycode = KC_5;
    smart_layers[LAYER_NUM]->map R_HOME_IDX->tap.keycode = KC_1;
    smart_layers[LAYER_NUM]->map R_HOME_MID->tap.keycode = KC_2;
    smart_layers[LAYER_NUM]->map R_HOME_RING->tap.keycode = KC_3;
    smart_layers[LAYER_NUM]->map R_HOME_PINK->tap.keycode = KC_4;
    smart_layers[LAYER_NUM]->map L_BOT_RING->tap.keycode = KC_COMMA;
    smart_layers[LAYER_NUM]->map L_BOT_MID->tap.keycode = KC_DOT;
    smart_layers[LAYER_NUM]->map L_BOT_IDX->tap.keycode = KC_9;
    smart_layers[LAYER_NUM]->map R_BOT_IDX->tap.keycode = KC_0;
    smart_layers[LAYER_NUM]->map R_BOT_MID->tap.keycode = KC_8;
    smart_layers[LAYER_NUM]->map R_BOT_MID->tap.mask = MOD_BIT(KC_RIGHT_SHIFT);
    smart_layers[LAYER_NUM]->map R_BOT_RING->tap.keycode = KC_SLASH;
    smart_layers[LAYER_NUM]->map L_THUMB_OUT->max_tap = 3;

    // L_THUMB_OUT - Tap: Toggle OS index, Hold: Print magic stats
    smart_layers[LAYER_NUM]->map L_THUMB_OUT->tap.action = &toggle_os_index;
    smart_layers[LAYER_NUM]->map L_THUMB_OUT->hold.action = &print_magic_stats;

    // ========================================================================
    // LAYER_JUMP_MAC
    // ========================================================================

    smart_layers[LAYER_JUMP_MAC]->map R_TOP_IDX->tap.keycode = KC_BACKSPACE;
    smart_layers[LAYER_JUMP_MAC]->map R_TOP_IDX->tap.mask = MOD_BIT(KC_ROPT);
    smart_layers[LAYER_JUMP_MAC]->map R_TOP_MID->tap.keycode = KC_PAGE_UP;
    smart_layers[LAYER_JUMP_MAC]->map R_HOME_IDX->tap.keycode = KC_LEFT;
    smart_layers[LAYER_JUMP_MAC]->map R_HOME_IDX->tap.mask = MOD_BIT(KC_ROPT);
    smart_layers[LAYER_JUMP_MAC]->map R_HOME_IDX->hold.keycode = KC_LEFT;
    smart_layers[LAYER_JUMP_MAC]->map R_HOME_IDX->hold.mask = MOD_BIT(KC_LEFT_GUI);
    smart_layers[LAYER_JUMP_MAC]->map R_HOME_MID->tap.keycode = KC_PAGE_DOWN;
    smart_layers[LAYER_JUMP_MAC]->map R_HOME_RING->tap.keycode = KC_RIGHT;
    smart_layers[LAYER_JUMP_MAC]->map R_HOME_RING->tap.mask = MOD_BIT(KC_ROPT);
    smart_layers[LAYER_JUMP_MAC]->map R_HOME_RING->hold.keycode = KC_RIGHT;
    smart_layers[LAYER_JUMP_MAC]->map R_HOME_RING->hold.mask = MOD_BIT(KC_LEFT_GUI);

    // ========================================================================
    // LAYER_JUMP_WIN
    // ========================================================================

    smart_layers[LAYER_JUMP_WIN]->map R_TOP_IDX->tap.keycode = KC_BACKSPACE;
    smart_layers[LAYER_JUMP_WIN]->map R_TOP_IDX->tap.mask = MOD_BIT(KC_RIGHT_CTRL);
    smart_layers[LAYER_JUMP_WIN]->map R_TOP_MID->tap.keycode = KC_PAGE_UP;
    smart_layers[LAYER_JUMP_WIN]->map R_TOP_RING->tap.keycode = KC_DELETE;
    smart_layers[LAYER_JUMP_WIN]->map R_TOP_RING->tap.mask = MOD_BIT(KC_RIGHT_CTRL);
    smart_layers[LAYER_JUMP_WIN]->map R_HOME_IDX->tap.keycode = KC_LEFT;
    smart_layers[LAYER_JUMP_WIN]->map R_HOME_IDX->tap.mask = MOD_BIT(KC_RIGHT_CTRL);
    smart_layers[LAYER_JUMP_WIN]->map R_HOME_IDX->hold.keycode = KC_HOME;
    smart_layers[LAYER_JUMP_WIN]->map R_HOME_MID->tap.keycode = KC_PAGE_DOWN;
    smart_layers[LAYER_JUMP_WIN]->map R_HOME_RING->tap.keycode = KC_RIGHT;
    smart_layers[LAYER_JUMP_WIN]->map R_HOME_RING->tap.mask = MOD_BIT(KC_RIGHT_CTRL);
    smart_layers[LAYER_JUMP_WIN]->map R_HOME_RING->hold.keycode = KC_END;

    // ========================================================================
    // LAYER_FUNCTION_KEYS
    // ========================================================================

    smart_layers[LAYER_FUNCTION_KEYS]->map L_TOP_RING->tap.keycode = KC_F12;
    smart_layers[LAYER_FUNCTION_KEYS]->map L_TOP_MID->tap.keycode = KC_F11;
    smart_layers[LAYER_FUNCTION_KEYS]->map L_TOP_IDX->tap.keycode = KC_F10;
    smart_layers[LAYER_FUNCTION_KEYS]->map R_TOP_IDX->tap.keycode = KC_F7;
    smart_layers[LAYER_FUNCTION_KEYS]->map R_TOP_MID->tap.keycode = KC_F8;
    smart_layers[LAYER_FUNCTION_KEYS]->map R_TOP_RING->tap.keycode = KC_F9;
    smart_layers[LAYER_FUNCTION_KEYS]->map L_HOME_RING->tap.keycode = KC_F6;
    smart_layers[LAYER_FUNCTION_KEYS]->map L_HOME_MID->tap.keycode = KC_F5;
    smart_layers[LAYER_FUNCTION_KEYS]->map L_HOME_IDX->tap.keycode = KC_F4;
    smart_layers[LAYER_FUNCTION_KEYS]->map R_HOME_IDX->tap.keycode = KC_F1;
    smart_layers[LAYER_FUNCTION_KEYS]->map R_HOME_MID->tap.keycode = KC_F2;
    smart_layers[LAYER_FUNCTION_KEYS]->map R_HOME_RING->tap.keycode = KC_F3;

    // ========================================================================
    // LAYER_MEDIA
    // ========================================================================

    smart_layers[LAYER_MEDIA]->map R_TOP_IDX->tap.keycode = KC_AUDIO_VOL_DOWN;
    smart_layers[LAYER_MEDIA]->map R_TOP_MID->tap.keycode = KC_AUDIO_MUTE;
    smart_layers[LAYER_MEDIA]->map R_TOP_RING->tap.keycode = KC_AUDIO_VOL_UP;

    // ========================================================================
    // LAYER_DUMB
    // ========================================================================

    smart_layers[LAYER_DUMB]->map L_TOP_RING->tap.keycode = KC_B;
    smart_layers[LAYER_DUMB]->map L_TOP_MID->tap.keycode = KC_L;
    smart_layers[LAYER_DUMB]->map L_TOP_IDX->tap.keycode = KC_W;
    smart_layers[LAYER_DUMB]->map L_HOME_PINK->tap.keycode = KC_LEFT_SHIFT;
    smart_layers[LAYER_DUMB]->map L_HOME_RING->tap.keycode = KC_A;
    smart_layers[LAYER_DUMB]->map L_HOME_MID->tap.keycode = KC_S;
    smart_layers[LAYER_DUMB]->map L_HOME_IDX->tap.keycode = KC_T;
    smart_layers[LAYER_DUMB]->map L_BOT_RING->tap.keycode = KC_LEFT_CTRL;
    smart_layers[LAYER_DUMB]->map L_BOT_MID->tap.keycode = KC_M;
    smart_layers[LAYER_DUMB]->map L_BOT_IDX->tap.keycode = KC_D;
    smart_layers[LAYER_DUMB]->map L_THUMB_IN->tap.keycode = KC_SPC;
    smart_layers[LAYER_DUMB]->map L_THUMB_OUT->hold.layer = LAYER_DUMB_2;

    // ========================================================================
    // LAYER_DUMB_2 - Secondary dumb layer (fallback mode)
    // ========================================================================

    smart_layers[LAYER_DUMB_2]->map L_TOP_RING->tap.keycode = KC_3;
    smart_layers[LAYER_DUMB_2]->map L_TOP_MID->tap.keycode = KC_2;
    smart_layers[LAYER_DUMB_2]->map L_TOP_IDX->tap.keycode = KC_1;
    smart_layers[LAYER_DUMB_2]->map L_HOME_RING->tap.keycode = KC_ESC;
    smart_layers[LAYER_DUMB_2]->map L_HOME_MID->tap.keycode = KC_ENTER;
    smart_layers[LAYER_DUMB_2]->map L_HOME_IDX->tap.keycode = KC_TAB;
    smart_layers[LAYER_DUMB_2]->map L_BOT_RING->tap.keycode = KC_7;
    smart_layers[LAYER_DUMB_2]->map L_BOT_MID->tap.keycode = KC_8;
    smart_layers[LAYER_DUMB_2]->map L_BOT_IDX->tap.keycode = KC_9;
    smart_layers[LAYER_DUMB_2]->map L_THUMB_IN->max_tap = 3;

    // L_THUMB_IN - Tap: Toggle OS index, Hold: Print magic stats
    smart_layers[LAYER_DUMB_2]->map L_THUMB_IN->tap.action = &toggle_os_index;
    smart_layers[LAYER_DUMB_2]->map L_THUMB_IN->hold.action = &print_magic_stats;

    // ========================================================================
    // LAYER_TMUX - Tmux control layer
    // ========================================================================

    smart_layers[LAYER_TMUX]->map L_TOP_RING->tap.keycode = KC_P;
    smart_layers[LAYER_TMUX]->map L_TOP_MID->tap.keycode = KC_5;
    smart_layers[LAYER_TMUX]->map L_TOP_MID->tap.mask = MOD_BIT(KC_RIGHT_SHIFT);
    smart_layers[LAYER_TMUX]->map L_TOP_IDX->tap.keycode = KC_RIGHT_BRACKET;
    smart_layers[LAYER_TMUX]->map L_HOME_PINK->tap.keycode = KC_Z;
    smart_layers[LAYER_TMUX]->map L_HOME_RING->tap.layer_toggle = LAYER_TMUX;
    smart_layers[LAYER_TMUX]->map L_HOME_MID->tap.keycode = KC_QUOTE;
    smart_layers[LAYER_TMUX]->map L_HOME_MID->tap.mask = MOD_BIT(KC_RIGHT_SHIFT);
    smart_layers[LAYER_TMUX]->map L_HOME_IDX->tap.keycode = KC_N;
    smart_layers[LAYER_TMUX]->map L_BOT_PINK->tap.keycode = KC_W;
    smart_layers[LAYER_TMUX]->map L_BOT_RING->tap.keycode = KC_LEFT_BRACKET;
    smart_layers[LAYER_TMUX]->map L_BOT_MID->tap.keycode = KC_C;

    // L_BOT_IDX - Tap: Activate tmux mode
    smart_layers[LAYER_TMUX]->map L_BOT_IDX->tap.action = &activate_tmux;

#ifdef MOUSEKEY_ENABLE
    // ========================================================================
    // LAYER_MOUSE
    // ========================================================================

    smart_layers[LAYER_MOUSE]->map L_TOP_MID->tap.keycode = KC_MS_UP;
    smart_layers[LAYER_MOUSE]->map R_TOP_IDX->tap.keycode = KC_MS_WH_LEFT;
    smart_layers[LAYER_MOUSE]->map R_TOP_MID->tap.keycode = KC_MS_WH_UP;
    smart_layers[LAYER_MOUSE]->map R_TOP_RING->tap.keycode = KC_MS_WH_RIGHT;
    smart_layers[LAYER_MOUSE]->map L_HOME_RING->tap.keycode = KC_MS_LEFT;
    smart_layers[LAYER_MOUSE]->map L_HOME_MID->tap.keycode = KC_MS_DOWN;
    smart_layers[LAYER_MOUSE]->map L_HOME_IDX->tap.keycode = KC_MS_RIGHT;
    smart_layers[LAYER_MOUSE]->map R_HOME_IDX->tap.keycode = KC_MS_BTN1;
    smart_layers[LAYER_MOUSE]->map R_HOME_MID->tap.keycode = KC_MS_WH_DOWN;
    smart_layers[LAYER_MOUSE]->map R_HOME_RING->tap.keycode = KC_MS_BTN2;
    smart_layers[LAYER_MOUSE]->map R_BOT_IDX->tap.keycode = KC_AUDIO_VOL_DOWN;
    smart_layers[LAYER_MOUSE]->map R_BOT_MID->tap.keycode = KC_AUDIO_MUTE;
    smart_layers[LAYER_MOUSE]->map R_BOT_RING->tap.keycode = KC_AUDIO_VOL_UP;
#endif

    // Step 1: Initialize keycodes from base layer and copy positions
    for (int i = 0; i < MATRIX_ROWS; ++i) {
        for (int j = 0; j < MATRIX_COLS; ++j) {
            for (int l = 0; l < SMART_LAYER_COUNT; ++l) {
                if (!smart_layers[l]) continue;
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

    // Step 2: Set default layer behaviors
    for (int i = 1; i < SMART_LAYER_COUNT; i++) {
        if (!smart_layers[i]) continue;
        if (!smart_layers[i]->oneshot_on_key_press) {
            smart_layers[i]->oneshot_on_key_press = &oneshot_on_key_press_default;
        }
    }
}
