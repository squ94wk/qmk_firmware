int layer_activations[SMART_LAYER_COUNT];
smart_layer_t *smart_layers[SMART_LAYER_COUNT];
int active_layers[SMART_LAYER_COUNT];

void register_with_mods(uint16_t keycode, uint16_t mask, uint16_t *release_mask);
void add_key_to_history(uint16_t keycode, bool shifted);

void register_with_mods(uint16_t keycode, uint16_t mask, uint16_t *release_mask) {
    uint16_t current_mods = get_mods();

    // apply caps word
    switch (keycode) {
    case KC_A ... KC_Z:
    case KC_MINUS:
    case KC_KP_MINUS:
        if (is_caps_word_on()) {
            caps_word_off(); // hack: qmk doesn't handle our keys so otherwise times out after 5s idle
            caps_word_on();
            if (get_mods() & MOD_MASK_SHIFT) {
                mask ^= MOD_MASK_SHIFT; // reverse caps behavior
            } else {
                mask |= MOD_MASK_SHIFT;
            }
        }
        break;
    case KC_BSPC:
    case KC_1 ... KC_0:
    case KC_KP_1 ... KC_KP_0:
    case KC_RIGHT ... KC_LEFT:
        break;
    default:
        caps_word_off();
    }

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
        if (!(get_mods() & ~MOD_MASK_SHIFT)) {
            add_key_to_history(keycode, (get_mods() & MOD_MASK_SHIFT));
        }
        set_mods(current_mods);
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
