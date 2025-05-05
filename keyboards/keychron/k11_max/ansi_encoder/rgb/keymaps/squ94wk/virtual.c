int layer_activations[SMART_LAYER_COUNT];
smart_layer_t *smart_layers[SMART_LAYER_COUNT];
int active_layers[SMART_LAYER_COUNT];

struct virtual_press_t {
    smart_key_t *trigger;
    uint16_t keycode;
    uint16_t mask;
};

#define VIRTUAL_PRESS_MAX_COUNT 16
struct virtual_press_t virtual_pressed[VIRTUAL_PRESS_MAX_COUNT] = {};

void register_with_mods(uint16_t keycode, uint16_t mask);
void add_key_to_history(uint16_t keycode, uint16_t mask);
void     virtual_send(smart_key_t *key, uint16_t keycode, uint16_t mask);
void     virtual_release(smart_key_t *key);

void register_with_mods(uint16_t keycode, uint16_t mask) {
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
        break;
    default: {
        // check if we clear caps word
        // apply & reset oneshot mods
        uint16_t current_mods = get_mods();
        uint16_t current_oneshot_mods = get_oneshot_mods();
        uprintf("DEBUG: current masks: mods: %d oneshot: %d\n", current_mods, current_oneshot_mods);
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

void virtual_send(smart_key_t *key, uint16_t keycode, uint16_t mask) {
    uprintf("DEBUG: register virtual key press %s with mask %d for trigger %s [%d, %d]\n", keycode_to_string(keycode), mask, keycode_to_string(key->keycode), key->pos.row, key->pos.col);
    memmove(&virtual_pressed[1], &virtual_pressed[0], sizeof(struct virtual_press_t) * (VIRTUAL_PRESS_MAX_COUNT-1));
    register_with_mods(keycode, get_mods() | mask);
    virtual_pressed[0] = (struct virtual_press_t){
        .trigger = key,
        .keycode = keycode,
        .mask = mask,
    };
}

// the code of the key that was pressed not always matches the registered one.
// we remember the keycode that was registered and unregister it here.
void virtual_release(smart_key_t *key) {
    int index = -1;
    for (int i=0; i < VIRTUAL_PRESS_MAX_COUNT; i++) {
        if (virtual_pressed[i].trigger == key) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        return;
    }
    struct virtual_press_t slot = virtual_pressed[index];

    if (slot.keycode) {
        uprintf("DEBUG: unregister virtual key %s for trigger %s [%d, %d]\n", keycode_to_string(slot.keycode), keycode_to_string(slot.trigger->keycode), slot.trigger->pos.row, slot.trigger->pos.col);
        unregister_code(slot.keycode);
    }
    if (slot.mask) {
        uprintf("DEBUG: clear mods %x for trigger %s [%d, %d]\n", slot.mask, keycode_to_string(slot.trigger->keycode), slot.trigger->pos.row, slot.trigger->pos.col);
        set_mods(get_mods() & ~slot.mask);
    }
    memmove(&virtual_pressed[index], &virtual_pressed[index+1], sizeof(struct virtual_press_t) * (VIRTUAL_PRESS_MAX_COUNT-index));
    if (index != VIRTUAL_PRESS_MAX_COUNT) {
        virtual_pressed[VIRTUAL_PRESS_MAX_COUNT-1].trigger = NULL;
    }
}
