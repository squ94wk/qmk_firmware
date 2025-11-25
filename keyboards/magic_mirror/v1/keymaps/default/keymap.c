// Copyright 2025 squ94wk
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
                 KC_B,    KC_L,    KC_W,
        KC_C,    KC_ESC,  KC_S,    KC_T,
        KC_M,    KC_R,    KC_G,    KC_D,
        KC_SPC,  KC_TRANSPARENT,

                 KC_F,    KC_O,    KC_U,
                 KC_N,    KC_E,    KC_TRANSPARENT,    KC_I,
                 KC_H,    KC_DOT,  KC_SLASH,
                 KC_TRANSPARENT,  KC_A
    )
};
