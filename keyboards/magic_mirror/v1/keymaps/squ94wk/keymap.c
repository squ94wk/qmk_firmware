// Copyright 2025 squ94wk
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "squ94wk.h"
#include "layout_helpers.h"
#include "os_detection.h"

// Include layers and combos
#include "layers.c"
#include "combos.c"

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [WIN_BASE] = LAYOUT(
                 KC_B,    KC_L,    KC_W,
        KC_C,    KC_ESC,  KC_S,    KC_T,
        KC_M,    KC_R,    KC_G,    KC_D,
        KC_SPC,  KC_TRNS,

                 KC_F,    KC_O,    KC_U,
                 KC_N,    KC_E,    CKC_MAGIC,    KC_I,
                 KC_H,    KC_DOT,  KC_SLASH,
                 KC_TRNS, KC_A
    )
};
// clang-format on

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    last_input = timer_read32();

    uprintf("DEBUG: Event\n");
    uprintf("DEBUG: [%d] %s %s [%d, %d]\n",
        record->event.time,
        record->event.pressed ? "pressed" : "released",
        keycode_to_string(keycode),
        record->event.key.row,
        record->event.key.col);

    if (process_smart_key(keycode, record)) {
        return false;
    }

    return true;
}

bool process_detected_host_os_user(os_variant_t detected_os) {
    switch (detected_os) {
        case OS_MACOS:
        case OS_IOS:
            os_index = 1;
            uprintf("OS Detection: macOS/iOS detected, os_index set to 1\n");
            break;
        case OS_WINDOWS:
        case OS_LINUX:
            os_index = 0;
            uprintf("OS Detection: Windows/Linux detected, os_index set to 0\n");
            break;
        case OS_UNSURE:
            uprintf("OS Detection: OS unsure, keeping current os_index = %d\n", os_index);
            break;
    }
    return true;
}
