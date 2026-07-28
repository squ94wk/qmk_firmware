#include "smart_case.h"
#include "squ94wk.h"
#include "history.h"

bool smart_case_enabled;
char smart_case_char;

bool is_smart_case_on(void) {
    return smart_case_enabled;
}

void smart_case_on(void) {
    smart_case_enabled = true;
}

void smart_case_off(void) {
    smart_case_enabled = false;
    smart_case_char = 0;
}

bool handle_smart_case(uint16_t *keycode, uint16_t *mask, uint16_t *release_mask) {
    if (is_layer_active(LAYER_NUM)) {
        switch (*keycode) {
        case KC_SPACE:
        case KC_ENTER:
        case KC_TAB:
        case KC_ESC:
            deactivate_layer(LAYER_NUM);
            return false;
        }
    }

    if (!is_smart_case_on()) {
        return false;
    }

    if (*mask & ~MOD_MASK_SHIFT) {
        smart_case_off();
        return false;
    }

    // The lock only makes sense starting on a letter; anything else aborts it.
    if (!smart_case_char) {
        switch (*keycode) {
        case KC_A ... KC_Z:
            smart_case_char = 'a';
            break;
        default:
            smart_case_off();
            return false;
        }
    }

    switch (*keycode) {
    case KC_BSPC:
    case KC_DELETE:
    case KC_RIGHT ... KC_LEFT:
        return false;

    case KC_ENTER:
    case KC_TAB:
    case KC_ESC:
        smart_case_off();
        return false;

    case KC_A ... KC_Z:
        deactivate_layer(LAYER_NUM);
        *mask |= MOD_BIT(KC_LEFT_SHIFT);
        return false;

    case KC_9:
    case KC_0:
        if (!(*mask & MOD_MASK_SHIFT)) {
            break;
        }
        // fallthrough: shifted 9/0 are ( ) — closing the word ends the lock
    case KC_LEFT_BRACKET:
    case KC_RIGHT_BRACKET:
        if (history_top().c == '_') {
            SEND_STRING("\b ");
            drop_key_from_history(0);
        }
        smart_case_off();
        return false;

    case KC_MINUS:
        if (!(*mask & MOD_MASK_SHIFT)) {
            break;
        }
        // shifted minus is '_', a valid separator inside the locked word
        return false;

    case KC_SPACE:
        if (history_top().c == '_') {
            SEND_STRING("\b ");
            drop_key_from_history(0);
            smart_case_off();
            return true;
        }
        *keycode = KC_MINUS;
        *mask |= MOD_BIT(KC_LEFT_SHIFT);
        return false;

    default:
    }
    smart_case_off();
    return false;
}

