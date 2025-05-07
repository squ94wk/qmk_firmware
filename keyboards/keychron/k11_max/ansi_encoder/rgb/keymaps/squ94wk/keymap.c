#include QMK_KEYBOARD_H
#include "keychron_common.h"

#include "helper.c"
#include "types.c"
#include "extern.c"
#include "tostring.c"

#include "combos.c"
#include "smart.c"
#include "magic.c"
#include "layers.c"

#include "print.h"

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
        _______,  _______,	   KC_B,	 KC_F,	  KC_W,    _______,	_______,	  KC_L,    KC_I,	KC_G,	 _______,	   _______,  _______,  _______,          KC_DEL,
        _______, KC_A,	   KC_ESC,	 KC_S,	  KC_T,    _______,              _______,    KC_N,	KC_E,	 KC_O,	   CKC_MAGIC,  _______,  _______,           _______,
        _______,           _______,	 KC_R,	  KC_C,    KC_D,	_______,	  _______,    _______,	 KC_H,	 KC_U,  KC_M,	_______,  _______, _______,
        _______, _______,  _______,           KC_SPC,           KC_ESC, KC_BSPC,       KC_ENT,            _______,            _______, _______, _______),
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
    uprintf(
        "DEBUG: [%d] %s %s [%d, %d]\n",
        record->event.time,
        record->event.pressed ? "pressed" : "released",
        keycode_to_string(keycode),
        record->event.key.row,
        record->event.key.col
    );

    if (process_smart_key(keycode, record)) {
        return false;
    }

    return true;
}
