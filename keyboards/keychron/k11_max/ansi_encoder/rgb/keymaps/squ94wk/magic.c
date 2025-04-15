#define KEY_HISTORY_MAX 32
uint16_t history_keycode[KEY_HISTORY_MAX] = {};
uint16_t history_modmask[KEY_HISTORY_MAX] = {};
int history_ptr;

void add_key_to_history(uint16_t keycode, uint16_t mask) {
    uprintf("DEBUG: add keycode %s to history with mask %d \n", keycode_to_string(keycode), mask);
    history_ptr = (history_ptr + 1) % KEY_HISTORY_MAX;
    history_keycode[history_ptr] = keycode;
    history_modmask[history_ptr] = mask;
}

void drop_key_from_history(void) {
    history_keycode[history_ptr] = KC_NO;
    history_modmask[history_ptr] = 0;
    history_ptr = (history_ptr - 1) % KEY_HISTORY_MAX;
}

void magic_action(smart_key_t *key) {
    for (int i=0; i < KEY_HISTORY_MAX; i++) {
        int index = (history_ptr-i) % KEY_HISTORY_MAX;
        uint16_t keycode = history_keycode[index];
        uint16_t mask = history_modmask[index];
        if (keycode == KC_NO) {
            break;
        }
        if (keycode == KC_QUOTE) {
            register_with_mods(keycode, mask);
            unregister_code(keycode);

            history_keycode[index] = KC_TRANSPARENT;
            drop_key_from_history();
            return;
        }
        if (keycode == KC_LEFT_BRACKET || ((keycode == KC_9 || keycode == KC_COMMA) && mask & MOD_MASK_SHIFT)) {
            register_with_mods(keycode+1, mask);
            unregister_code(keycode+1);

            history_keycode[index] = KC_TRANSPARENT;
            drop_key_from_history();
            return;
        }
    }
}
