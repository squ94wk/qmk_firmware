int layer_activations[SMART_LAYER_COUNT];
smart_layer_t *smart_layers[SMART_LAYER_COUNT];
int active_layers[SMART_LAYER_COUNT];

void register_with_mods(uint16_t keycode, uint16_t mask);
void add_key_to_history(uint16_t keycode, uint16_t mask);

void register_with_mods(uint16_t keycode, uint16_t mask) {
    uprintf("DEBUG: register %s with mask %d\n", keycode_to_string(keycode), mask);

    switch (keycode) {
    case KC_A ... KC_Z:
    case KC_MINUS:
        break;
    default:
        caps_word_off();
    }

    switch (keycode) {
    case KC_LEFT_CTRL ... KC_RIGHT_GUI:
        register_code(keycode);
        set_mods(get_mods() | mask);
        break;
    default: {
        // check if we clear caps word
        // apply & reset oneshot mods
        uint16_t current_mods = get_mods();
        uint16_t current_oneshot_mods = get_oneshot_mods();
//        uprintf("DEBUG: current masks: mods: %d oneshot: %d\n", current_mods, current_oneshot_mods);
        if (current_mods != mask) {
            set_mods(mask);
            if (is_caps_word_on()) {
                uprintf("DEBUG: add weak shift\n");
                add_weak_mods(MOD_BIT(KC_LSFT));
            }
            register_code(keycode);
            set_mods(current_mods);
        } else {
            register_code(keycode);
        }
        if (current_oneshot_mods) {
            uprintf("DEBUG: clear oneshot mods\n");
            clear_oneshot_mods();
        }
        add_key_to_history(keycode, mask);

        break;
    }
    }

    // use up oneshot layer activations
    for (int i=0; i < SMART_LAYER_COUNT && active_layers[i]; ++i) {
        smart_layer_t *l = smart_layers[active_layers[i]];
        if (l->oneshot_on_key_press(l, keycode, mask)) {
            if (layer_activations[active_layers[i]] > 0) {
                layer_activations[active_layers[i]]--;
            }
        }
    }
}
