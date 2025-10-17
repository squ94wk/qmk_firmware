#include QMK_KEYBOARD_H
#include "keychron_common.h"
#include "squ94wk.h"

#include "combos.c"
#include "layers.c"

#ifdef CONSOLE_ENABLE
void keyboard_post_init_user(void) {
    // Customise these values to desired behaviour
    debug_enable   = true;
    debug_matrix   = true;
    debug_keyboard = true;
}
#endif

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [WIN_BASE] = LAYOUT_69_ansi(
        QK_BOOT,  _______,	   _______,	 _______,	  _______,    _______,	_______,	  _______,    _______,	_______,	 _______,	   _______,  _______,   _______,          KC_MUTE,
        _______,  _______,	   KC_B,	 KC_L,	  KC_W,    _______,	_______,	  KC_F,    KC_O,	KC_U,	 _______,	   _______,  _______,  _______,          KC_DEL,
        _______, KC_C,	   KC_ESC,	 KC_S,	  KC_T,    _______,              _______,    KC_N,	KC_E,	 CKC_MAGIC,  KC_I,	   _______,  _______,           _______,
        _______,           KC_M,	 KC_R,	  KC_G,    KC_D,	_______,	  _______,    _______,	 KC_H,	 KC_DOT,  KC_SLASH,	_______,  _______, _______,
        _______, _______,  _______,           KC_SPC,           _______, _______,            KC_A,       _______,            _______, _______, _______),
};


#if defined(ENCODER_MAP_ENABLE)
	const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
		[WIN_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
	};
#endif // ENCODER_MAP_ENABLE

bool lookup_keypos_in_base_keymap(uint16_t keycode, keypos_t *key) {
    for (int i=0; i < MATRIX_ROWS; ++i) {
        for (int j=0; j < MATRIX_COLS; ++j) {
            key->row = i;
            key->col = j;
            return true;
        }
    }

    return false;
}

// clang-format on
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    last_input = timer_read32();
    if (!rgb_matrix_is_enabled()) {
        rgb_matrix_enable();
    }

    if (!process_record_keychron_common(keycode, record)) {
        uprintf("DEBUG: QMK handled event\n");
        return false;
    }

    if (record->event.key.row == 0 && record->event.key.col == 0) {
        if (!lookup_keypos_in_base_keymap(keycode, &record->event.key)) {
            return false; // COMBO
        }
    }

    uprintf("DEBUG: Event\n");
    uprintf("DEBUG: [%d] %s %s [%d, %d]\n", record->event.time, record->event.pressed ? "pressed" : "released", keycode_to_string(keycode), record->event.key.row, record->event.key.col);

    if (process_smart_key(keycode, record)) {
        return false;
    }

    return true;
}
