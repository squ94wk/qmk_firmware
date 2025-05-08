int layer_activations[SMART_LAYER_COUNT];
smart_layer_t *smart_layers[SMART_LAYER_COUNT];
int active_layers[SMART_LAYER_COUNT];

void register_with_mods(uint16_t keycode, uint16_t mask, uint16_t *release_mask);
void add_key_to_history(uint16_t keycode, uint16_t mask);

void register_with_mods(uint16_t keycode, uint16_t mask, uint16_t *release_mask) {
    uint16_t current_mods = get_mods();

    // apply caps word
    // switch (keycode) {
    // case KC_A ... KC_Z:
    // case KC_MINUS:
    //     if (caps_word_on()) {
    //         *mask |= MOD_BIT_RSHIFT;
    //     }
    //     break;
    // default:
    //     caps_word_off();
    // }

    // apply oneshot mods
    if (get_oneshot_mods()) {
        mask |= get_oneshot_mods();
        uprintf("DEBUG: apply oneshot mask: %d\n", get_oneshot_mods());
        clear_oneshot_mods();
    }

    uprintf("DEBUG: register %s with mask %d\n", keycode_to_string(keycode), mask);

    set_mods(current_mods | mask);
    register_code(keycode);
    if (IS_MODIFIER_KEYCODE(keycode)) {
        *release_mask = mask & ~current_mods;
    } else {
        set_mods(current_mods);
    }

    if (!( current_mods | (mask & ~MOD_MASK_SHIFT) )) {
        add_key_to_history(keycode, mask);
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
