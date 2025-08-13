int layer_activations[SMART_LAYER_COUNT];
smart_layer_t *smart_layers[SMART_LAYER_COUNT];
int active_layers[SMART_LAYER_COUNT];
bool deactivate_layer(int);

void add_key_to_history(uint16_t keycode, bool shifted);

void register_with_mods(uint16_t *keycode, uint16_t mask, uint16_t *release_mask) {
    uint16_t current_mods = get_mods();

    if (handle_smart_case(keycode, &mask, release_mask)) {
        return;
    }

    // apply oneshot mods
    if (get_oneshot_mods()) {
        mask |= get_oneshot_mods();
        uprintf("DEBUG: apply oneshot mask: %d\n", get_oneshot_mods());
        clear_oneshot_mods();
    }

    uprintf("DEBUG: register %s with mask %d\n", keycode_to_string(*keycode), mask);

    set_mods(current_mods | mask);
    register_code(*keycode);

    if (IS_MODIFIER_KEYCODE(*keycode)) {
        *release_mask = mask & ~current_mods;
    } else {
        if (!(get_mods() & ~MOD_MASK_SHIFT)) {
            add_key_to_history(*keycode, (get_mods() & MOD_MASK_SHIFT));
        }
        set_mods(current_mods);
    }

    // use up oneshot layer activations
    for (int i=0; i < SMART_LAYER_COUNT && active_layers[i]; ++i) {
        smart_layer_t *l = smart_layers[active_layers[i]];
        if (l->oneshot_on_key_press(l, *keycode, mask)) {
            if (layer_activations[active_layers[i]] > 0) {
                layer_activations[active_layers[i]]--;
            }
        }
    }
}
