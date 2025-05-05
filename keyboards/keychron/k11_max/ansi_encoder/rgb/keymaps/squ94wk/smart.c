#include "queue.c"
#include "virtual.c"

static keyevent_t deferred_event;
static uint16_t deferred_keycode;

int layer_activations[SMART_LAYER_COUNT];

smart_layer_t *smart_layers[SMART_LAYER_COUNT];
smart_key_t smart_keys[];
int active_layers[SMART_LAYER_COUNT];
smart_key_t *lookup_key(uint16_t keycode, keypos_t key);

static bool handle_deferred_event_continuation(smart_key_t *key, keyevent_t cont);
static bool process_event(uint16_t keycode, keyevent_t event);
static bool process_event_with_key(smart_key_t *key, uint16_t keycode, keyevent_t event);

void tap_action(smart_key_t *key);
void hold_action(smart_key_t *key);
void release_action(smart_key_t *key);

static void reset_smart_key(smart_key_t *smart_key);
static enum smart_key_type get_key_type(smart_key_t *key);
static enum event_type get_event_type(smart_key_t *key, keyevent_t event);

bool run_housekeeping = false;

bool is_layer_active(int layer) {
    for (int i=0; i < SMART_LAYER_COUNT && active_layers[i]; ++i) {
        if (active_layers[i] == layer) {
            return true;
        }
    }
    return false;
}

bool deactivate_layer(int layer) {
    if (!is_layer_active(layer)) {
        return false;
    }
    for (int i=0; i < SMART_LAYER_COUNT && active_layers[i]; ++i) {
        if (active_layers[i] != layer) {
            continue;
        }
        active_layers[i] = 0;
        memmove(&active_layers[i], &active_layers[i+1], SMART_LAYER_COUNT-i-1);
        uprintf("DEBUG: layer %d deactivated\n", layer);
        return true;
    }
    return false;
}

bool activate_layer(int layer) {
    if (is_layer_active(layer)) {
        return false;
    }
    memmove(&active_layers[1], &active_layers[0], SMART_LAYER_COUNT-1);
    active_layers[0] = layer;
    uprintf("DEBUG: layer %d activated\n", layer);
    for (int i=1; i < SMART_LAYER_COUNT && active_layers[i]; i++) {
        smart_layer_t *l = smart_layers[active_layers[i]];
        if (l->on_layer_activate && !l->on_layer_activate(l, layer)) {
            if (deactivate_layer(active_layers[i])) {
                i--;
            }
        }
    }
    return true;
}

void toggle_layer(int layer) {
    uprintf("DEBUG: toggle layer %d\n", layer);
    if (!is_layer_active(layer)) {
        activate_layer(layer);
    } else {
        deactivate_layer(layer);
    }
}

void press_key(smart_key_t *key, keyevent_t event) {
    key->state.pressed_time = event.time;
    if (add_pending_key(key)) {
        uprintf("DEBUG: added pending key [%d, %d]\n", key->pos.row, key->pos.col);
    }
}

void tap_action(smart_key_t *key) {
    key->state.tap_fired = true;
    if (key->tap.action) {
        key->tap.action(key);
        return;
    }
    if (key->tap.layer_toggle) {
        toggle_layer(key->tap.layer_toggle);
        layer_activations[key->tap.layer_oneshot] = -1;
        return;
    }
    if (key->tap.layer_oneshot) {
        activate_layer(key->tap.layer_oneshot);
        layer_activations[key->tap.layer_oneshot] = 1;
        return;
    }
    if (key->tap.mask_oneshot) {
        set_oneshot_mods(key->tap.mask_oneshot);
        return;
    }
    virtual_send(key->pos, key->keycode, key->tap.keycode, key->tap.mask);
}

void hold_action(smart_key_t *key) {
    key->state.hold_fired = true;
    if (key->hold.action) {
        key->hold.action(key);
        return;
    }
    if (key->hold.layer) {
        activate_layer(key->hold.layer);
        layer_activations[key->hold.layer] = -1;
        return;
    }
    virtual_send(key->pos, key->keycode, key->hold.keycode, key->hold.mask);
    if (key->hold.tap_keycode) {
        virtual_release(key->pos, key->keycode);
    }
}

void release_action(smart_key_t *key) {
    if (key->release_action) {
        key->release_action(key);
        return;
    }
    if (key->state.tap_fired) {
        if (key->tap.keycode) {
            virtual_release(key->pos, key->keycode);
        }
        return;
    }
    if (key->state.hold_fired) {
        if (key->hold.layer) {
            deactivate_layer(key->hold.layer);
            return;
        }
        if (key->hold.keycode) {
            virtual_release(key->pos, key->keycode);
        }
        return;
    }
}

static bool process_smart_key(uint16_t keycode, keyrecord_t *record) {
    run_housekeeping = true;

    // handle deferred event
    if (deferred_keycode) {
        for (int i = 0; i < PENDING_QUEUE_MAX && pending_keys[i]; ++i) {
            smart_key_t *key = pending_keys[i];
            if (handle_deferred_event_continuation(key, record->event)) {
                break;
            }
        }

        process_event(deferred_keycode, deferred_event);
        deferred_keycode = KC_NO;
    }

    return process_event(keycode, record->event);
}

static bool handle_deferred_event_continuation(smart_key_t *key, keyevent_t cont) {
    enum smart_key_type key_type = get_key_type(key);
    uprintf("DEBUG: handle deferred event for %s key\n", key_type_to_string(key_type));

    switch (key_type) {
    case TAP_HOLD:
        if (!deferred_event.pressed) {
            break; // this key type only defers presses
        }

        // release self = roll
        if (!cont.pressed && is_same_pos(key->pos, cont.key)) {
            if (!key->state.tap_fired && !key->state.hold_fired) {
                key->state.tap_fired = true;
                tap_action(key);
            }
            return true;
        }

        // pressed yet another key
        // or
        // tap (down, up) of single key
        if (cont.pressed || is_same_pos(deferred_event.key, cont.key)) {
            key->state.hold_fired = true;
            hold_action(key);
            return true;
        }

        return false;
    default:
        return false;
    }
    return false;
}

bool process_event(uint16_t keycode, keyevent_t event) {
    // give pending keys a chance to see and process the event first
    for (int i = 0; i < PENDING_QUEUE_MAX && pending_keys[i]; ++i) {
        smart_key_t *key = pending_keys[i];
        if (process_event_with_key(key, keycode, event)) {
            return true;
        }
    }

    // then on key of the event
    smart_key_t *key = lookup_key(keycode, event.key);
    return process_event_with_key(key, keycode, event);
}

static bool process_event_with_key(smart_key_t *key, uint16_t keycode, keyevent_t event) {
    enum event_type event_type = get_event_type(key, event);
    enum smart_key_type key_type = get_key_type(key);
    uprintf("DEBUG: handle %s event for %s key\n", event_to_string(event_type), key_type_to_string(key_type));

    switch (key_type) {
    case DUMB: {
        switch (event_type) {
        case PRESS_SAME:
            virtual_send(key->pos, keycode, keycode, 0);
            return true;
        case RELEASE_SAME:
            virtual_release(key->pos, keycode);
            return true;
        default:
            return false;
        }
    }

    case TAP_ONLY:
        switch (event_type) {
        case PRESS_SAME:
            press_key(key, event);
            tap_action(key);
            return true;
        case RELEASE_SAME:
            release_action(key);
            reset_smart_key(key);
            return true;
        default:
            break;
        }

    case HOLD_ONLY:
        switch (event_type) {
        case PRESS_SAME:
            press_key(key, event);
            hold_action(key);
            return true;
        case RELEASE_SAME:
            release_action(key);
            reset_smart_key(key);
            return true;
        default:
            break;
        }

    case TAP_HOLD:
        switch (event_type) {
        case PRESS_SAME:
            press_key(key, event);
            return true;
        case PRESS_OTHER:
            if (key->state.tap_fired || key->state.hold_fired) {
                return false; // ignore
            }
            if (!key->fire_on_key_press || key->fire_on_key_press(key, event.key)) {
                // maybe it's a roll, maybe a fast combination
                deferred_event = event;
                deferred_keycode = keycode;
                return true;
            }
            return false;
        case RELEASE_OTHER:
            // ignore
            return false;
        case RELEASE_SAME:
            if (!key->state.tap_fired && !key->state.hold_fired) {
                tap_action(key);
            }
            release_action(key);
            reset_smart_key(key);
            return true;
        default:
            break;
        }

    case N_TAP:
        switch (event_type) {
        case PRESS_SAME:
            press_key(key, event);
            key->state.tap_count++;
            key->state.tap_timeout = event.time + TAPPING_TERM;
            if (key->state.tap_count == key->max_tap) {
                tap_action(key);
            }
            return true;
        case RELEASE_SAME:
            if (key->state.tap_fired || key->state.hold_fired) {
                release_action(key);
                reset_smart_key(key);
                return true;
            }
            key->state.pressed_time = 0;
            return true;
        default:
            break;
        }

    case N_HOLD:
        switch (event_type) {
        case PRESS_SAME:
            press_key(key, event);
            key->state.tap_count++;
            key->state.tap_timeout = event.time + TAPPING_TERM;
            if (key->state.tap_count == key->max_tap) {
                hold_action(key);
            }
            return true;
        case RELEASE_SAME:
            if (key->state.tap_fired || key->state.hold_fired) {
                release_action(key);
                reset_smart_key(key);
                return true;
            }
            key->state.pressed_time = 0;
            return true;
        case PRESS_OTHER:
            if (!key->fire_on_key_press || key->fire_on_key_press(key, event.key)) {
                key->state.hold_fired = true;
                hold_action(key);
            }
            return false; // process event as usual
        default:
            break;
        }

    case N_TAP_HOLD:
        switch (event_type) {
        case PRESS_SAME:
            press_key(key, event);
            key->state.tap_count++;
            uprintf("DEBUG: increase tap count to %d\n", key->state.tap_count);
            // todo: unnecessary
            key->state.tap_timeout = event.time + TAPPING_TERM;
            return true;
        case RELEASE_SAME:
            if (key->state.tap_fired || key->state.hold_fired) {
                release_action(key);
                reset_smart_key(key);
                return true;
            }
            if (key->state.tap_count == key->max_tap) {
                tap_action(key);
                release_action(key);
                reset_smart_key(key);
            }
            key->state.pressed_time = 0;
            return true;
        case PRESS_OTHER:
            if (key->state.tap_fired || key->state.hold_fired) {
                return false;
            }
            if (key->state.pressed_time) { // pressed
                if (!key->fire_on_key_press || key->fire_on_key_press(key, event.key)) {
                    key->state.hold_fired = true;
                    hold_action(key);
                }
            } else { // released
                key->state.tap_fired = true;
                tap_action(key);
            }
            return false; // process event as usual
        case RELEASE_OTHER:
            return false;
        default:
            break;
        }

    default:
        break;
    }

    uprintf("DEBUG: warning: unexpected event type\n");
    return true;
}

void housekeeping_task_user(void) {
    if (!run_housekeeping) {
        return;
    }
    run_housekeeping = false;

    // handle pending keys
    for (int i=0; i < PENDING_QUEUE_MAX; ++i) {
        smart_key_t *key = pending_keys[i];
        if (!key->state.pressed_time && !key->state.tap_timeout) {
            if (remove_pending_key(key)) {
                uprintf("DEBUG: removed pending key [%d, %d]\n", key->pos.row, key->pos.col);
            }
        }
    }

    // handle one shot layers
    for (int i=0; i < SMART_LAYER_COUNT; ++i) {
        if (!layer_activations[i]) {
            deactivate_layer(i);
        }
    }
}

void matrix_scan_user() {
    for (int i = 0; i < PENDING_QUEUE_MAX && pending_keys[i]; ++i) {
        smart_key_t *key = pending_keys[i];

        // check if deferred key has been held for TAPPING_TERM
        if (!key->state.hold_fired && !key->state.tap_fired && key->state.pressed_time && timer_elapsed(key->state.pressed_time) > TAPPING_TERM) {
            run_housekeeping = true;
            key->state.hold_fired = true;
            uprintf("DEBUG: time: %d, process key HOLD expiry\n", timer_read());
            switch (get_key_type(key)) {
            case N_TAP:
                tap_action(key);
            case TAP_HOLD:
            case N_HOLD:
            case N_TAP_HOLD:
                hold_action(key);
                break;
            default:
                break;
            }
            continue;
        }

        // check if deferred key is multi tap and hasn't been tapped again recently
        if (!key->state.tap_fired && !key->state.hold_fired && key->state.tap_timeout && timer_read() > key->state.tap_timeout) {
            uprintf("DEBUG: time: %d, process key multi tap expiry\n", timer_read());
            run_housekeeping = true;
            key->state.tap_fired = true;
            switch (get_key_type(key)) {
            case N_TAP:
                tap_action(key);
                // we don't get another release event, so we need to handle that here
                release_action(key);
                reset_smart_key(key);
                break;
            case N_HOLD:
                // N_HOLD is only good if the key is still being held
                break;
            case N_TAP_HOLD:
                tap_action(key);
                release_action(key);
                reset_smart_key(key);
                break;
            default:
                break;
            }
            return;
        }
    }
}

static enum smart_key_type get_key_type(smart_key_t *key) {
    if (!key) {
        return DUMB;
    }

    bool is_tap = key->tap.action || key->tap.keycode || key->tap.layer_toggle || key->tap.layer_oneshot || key->tap.mask_oneshot;
    bool is_hold = key->hold.action || key->hold.keycode || key->hold.layer;

    if (key->max_tap) {
        if (!is_tap) {
            return N_HOLD;
        }
        if (!is_hold) {
            return N_TAP;
        }
        return N_TAP_HOLD;
    } else {
        if (!is_tap) {
            return HOLD_ONLY;
        }
        if (!is_hold) {
            return TAP_ONLY;
        }
        return TAP_HOLD;
    }
    //unreachable
    return 0;
}

static enum event_type get_event_type(smart_key_t *key, keyevent_t event) {
    bool same_key = key ? is_same_pos(key->pos, event.key) : true;

    if (event.pressed) {
        if (same_key) {
            return PRESS_SAME;
        } else {
            return PRESS_OTHER;
        }
    } else {
        if (same_key) {
            return RELEASE_SAME;
        } else {
            return RELEASE_OTHER;
        }
    }
}

static void reset_smart_key(smart_key_t *key) {
    uprintf("DEBUG: reset smart key\n");
    key->state.tap_count = 0;
    key->state.tap_timeout = 0;
    key->state.tap_fired = false;
    key->state.hold_fired = false;
    key->state.pressed_time = 0;
}
