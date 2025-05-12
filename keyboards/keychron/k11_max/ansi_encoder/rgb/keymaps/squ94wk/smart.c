#include "pending.c"
#include "virtual.c"

static keyevent_t deferred_event;
static uint16_t deferred_keycode;

int layer_activations[SMART_LAYER_COUNT];

smart_layer_t *smart_layers[SMART_LAYER_COUNT];
smart_key_t smart_keys[];
int active_layers[SMART_LAYER_COUNT];
smart_key_t *lookup_key(uint16_t keycode, keypos_t key);

static bool process_continuation(smart_key_t *key, enum continuation_type cont);
static bool process_event(uint16_t keycode, keyevent_t event);
static bool process_event_with_key(smart_key_t *key, uint16_t keycode, keyevent_t event);

void tap_action(smart_key_t *key);
void hold_action(smart_key_t *key);
void release_action(smart_key_t *key);

static void reset_smart_key(smart_key_t *smart_key);
static enum smart_key_type get_key_type(smart_key_t *key);
static enum event_type get_event_type(smart_key_t *key, keyevent_t event);
static enum continuation_type get_continuation_type(smart_key_t *key, keyevent_t event, keyevent_t cont);

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
        memmove(&active_layers[i], &active_layers[i+1], sizeof(active_layers[0]) * (SMART_LAYER_COUNT-i-1));
        active_layers[SMART_LAYER_COUNT-1] = 0;
        uprintf("DEBUG: layer %s deactivated\n", layer_to_string(layer));
        return true;
    }
    return false;
}

bool activate_layer(int layer) {
    if (is_layer_active(layer)) {
        return false;
    }
    memmove(&active_layers[1], &active_layers[0], sizeof(active_layers[0]) * (SMART_LAYER_COUNT-1));
    active_layers[0] = layer;
    layer_activations[layer] = -1; // assume not oneshot, may be overridden
    uprintf("DEBUG: layer %s activated\n", layer_to_string(layer));
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
    add_pending_key(key);
}

void release_key(smart_key_t *key) {
    key->state.pressed_time = 0;
}

void tap_action(smart_key_t *key) {
    key->state.fired = true;
    if (key->tap.action) {
        key->tap.action(key);
        return;
    }
    if (key->tap.layer_toggle) {
        toggle_layer(key->tap.layer_toggle);
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
    register_with_mods(key->tap.keycode, key->tap.mask, &key->state.release.mask);
    key->state.release.keycode = key->tap.keycode;
}

void hold_action(smart_key_t *key) {
    key->state.fired = true;
    if (key->hold.action) {
        key->hold.action(key);
        return;
    }
    if (key->hold.layer) {
        activate_layer(key->hold.layer);
        key->state.release.layer = key->hold.layer;

        // apply oneshot mods to layer holds
        // this makes some things like alt+tab or ctrl+p more convenient
        if (get_oneshot_mods()) {
            key->state.release.mask = get_oneshot_mods();
            set_mods(get_mods() | get_oneshot_mods());
            clear_oneshot_mods();
        }

        uprintf("DEBUG: remember to deactivate layer %s on release\n", layer_to_string(key->state.release.layer));
        return;
    }

    register_with_mods(key->hold.keycode, key->hold.mask, &key->state.release.mask);
    uprintf("DEBUG: remembered mask: %d\n", key->state.release.mask);
    key->state.release.keycode = key->hold.keycode;
    if (key->hold.tap_keycode) {
        key->state.pressed_time = 0; // mark as released
    }
}

void release_action(smart_key_t *key) {
    if (key->release_action) {
        key->release_action(key);
        return;
    }

    if (!key->state.fired) {
        uprintf("DEBUG: releasing key that hasn't fired\n");
        return;
    }

    if (key->state.release.keycode) {
        unregister_code(key->state.release.keycode);
    }
    if (key->state.release.mask) {
        set_mods(get_mods() & ~key->state.release.mask);
    }
    if (key->state.release.layer) {
        deactivate_layer(key->state.release.layer);
    }

    return;
}

static bool process_smart_key(uint16_t keycode, keyrecord_t *record) {
    run_housekeeping = true;

    // handle deferred event
    if (deferred_keycode) {
        for (int i = 0; i < PENDING_QUEUE_MAX && pending_keys[i].key; ++i) {
            if (pending_keys[i].key->state.fired) {
                continue;
            }
            smart_key_t *key = pending_keys[i].key;
            enum continuation_type cont = get_continuation_type(key, deferred_event, record->event);
            if (process_continuation(key, cont)) {
                break;
            }
        }

        uint16_t code = deferred_keycode;
        deferred_keycode = KC_NO;
        process_event(code, deferred_event);
    }

    return process_event(keycode, record->event);
}

static bool process_continuation(smart_key_t *key, enum continuation_type cont) {
    enum smart_key_type key_type = get_key_type(key);
    uprintf("DEBUG: handle continuation %s for %s\n", continuation_to_string(cont), key_type_to_string(key_type));

    switch (key_type) {
    case N_TAP:
        switch (cont) {
        case HOLD:
        case IDLE:
            tap_action(key);
            return true;
        default:
            return false;
        }
    case TAP_HOLD:
    case N_TAP_HOLD:
        switch (cont) {
        case ROLL:
            if (!key->state.fired) {
                tap_action(key);
            }
            return true;

        case TAP:
        case PRESS_THIRD:
        case HOLD:
            hold_action(key);
            return true;

        case TAP_ORIGINAL:
            tap_action(key);
            return true;

        case IDLE:
            tap_action(key);
            return true;

        case RELEASE_THIRD:
            return false;
        }
    default:
        return false;
    }
    return false;
}

bool process_event(uint16_t keycode, keyevent_t event) {
    // give pending keys a chance to see and process the event first
    for (int i = 0; i < PENDING_QUEUE_MAX && pending_keys[i].key; ++i) {
        smart_key_t *key = pending_keys[i].key;
        if (key->state.fired) {
            continue;
        }
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
    uprintf("DEBUG: handle %s event for %s key (%s)\n", event_to_string(event_type), key ? keycode_to_string(key->keycode) : "DUMB", key_type_to_string(key_type));

    switch (key_type) {
    case DUMB: {
        switch (event_type) {
        case PRESS_SAME: {
            register_with_mods(keycode, 0, NULL);
            return true;
        }
        case RELEASE_SAME:
            unregister_code(keycode);
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
            release_key(key);
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
            release_key(key);
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
            if (key->state.fired) {
                return false; // ignore
            }
            if (key->tap_on_key_press && key->tap_on_key_press(key, event.key)) {
                uprintf("DEBUG: immediately tap key %s\n", keycode_to_string(key->keycode));
                tap_action(key);
                return false;
            }
            if (key->hold_on_key_press && key->hold_on_key_press(key, event.key)) {
                uprintf("DEBUG: immediately hold key %s\n", keycode_to_string(key->keycode));
                hold_action(key);
                return false;
            }
            uprintf("DEBUG: defer press of key %s\n", keycode_to_string(keycode));
            deferred_event = event;
            deferred_keycode = keycode;
            return true;
        case RELEASE_OTHER:
            // ignore
            return false;
        case RELEASE_SAME:
            if (key->state.fired) {
                release_key(key);
                return true;
            }

            if (deferred_event.time == event.time) { // we're handling the deferred event itself
                tap_action(key);
                release_key(key);
                return true;
            }

            if (key->defer_release) {
                uprintf("DEBUG: defer release of key %s\n", keycode_to_string(keycode));
                deferred_event = event;
                deferred_keycode = keycode;
                return true;
            }

            tap_action(key);
            release_key(key);
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
            release_key(key);
            if (key->state.fired) {
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
            release_key(key);
            if (key->state.fired) {
                return true;
            }
            key->state.pressed_time = 0;
            return true;
        case PRESS_OTHER:
            hold_action(key);
            return false;
        default:
            break;
        }

    case N_TAP_HOLD:
        switch (event_type) {
        case PRESS_SAME:
            press_key(key, event);
            key->state.tap_count++;
            uprintf("DEBUG: increased tap count to %d\n", key->state.tap_count);
            if (key->state.tap_count > key->max_tap) {
                uprintf("DEBUG: max tap count exceeded: tap\n");
                tap_action(key);
                return true;
            }
            key->state.tap_timeout = event.time + TAPPING_TERM;
            return true;
        case RELEASE_SAME:
            release_key(key);
            if (key->state.fired) {
                return true;
            }
            key->state.pressed_time = 0;
            return true;
        case PRESS_OTHER:
            if (key->state.fired) {
                return false;
            }
            if (key->state.pressed_time) { // pressed
                if (key->tap_on_key_press && key->tap_on_key_press(key, event.key)) {
                    uprintf("DEBUG: immediately tap key %s\n", keycode_to_string(key->keycode));
                    tap_action(key);
                    return false;
                }
                if (key->hold_on_key_press && key->hold_on_key_press(key, event.key)) {
                    uprintf("DEBUG: immediately hold key %s\n", keycode_to_string(key->keycode));
                    hold_action(key);
                    return false;
                }
                uprintf("DEBUG: defer press of key %s\n", keycode_to_string(keycode));
                deferred_event = event;
                deferred_keycode = keycode;
                return true;
            } else { // released
                key->state.fired = true;
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
    for (int i=0; i < PENDING_QUEUE_MAX && pending_keys[i].key; ++i) {
        pending_key_t key = pending_keys[i];
        if (!key.key->state.fired) {
            continue;
        }
        // skip if still pressed
        if (key.key->state.pressed_time) {
            continue;
        }

//        uprintf("DEBUG: pending key %s is no longer pressed\n", keycode_to_string(key.key->keycode));
        release_action(key.key);
        reset_smart_key(key.key);
        remove_pending_key(key.key);
        --i;
    }

    // handle one shot layers
    for (int i=0; i < SMART_LAYER_COUNT; ++i) {
        if (!layer_activations[i]) {
            deactivate_layer(i);
        }
    }
}

void matrix_scan_user() {
    for (int i = 0; i < PENDING_QUEUE_MAX && pending_keys[i].key; ++i) {
        smart_key_t *key = pending_keys[i].key;
        if (key->state.fired) {
            continue;
        }

        if (key->state.pressed_time && timer_elapsed(key->state.pressed_time) > TAPPING_TERM) {
            run_housekeeping = true;
            // had any other event come in, it would've been handled already
            // TODO: handle only if the deferred key is this one
            if (deferred_keycode) {
                uint16_t code = deferred_keycode;
                deferred_keycode = KC_NO;
                process_event_with_key(key, code, deferred_event);
                continue;
            }

            process_continuation(key, HOLD);
            continue;
        }

        // check if deferred key is multi tap and hasn't been tapped again recently
        if (key->state.tap_timeout && timer_read() > key->state.tap_timeout) {
            run_housekeeping = true;
            process_continuation(key, IDLE);
            continue;
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

static enum continuation_type get_continuation_type(smart_key_t *key, keyevent_t deferred, keyevent_t cont) {
    bool original_key = is_same_pos(key->pos, cont.key);
    bool same_key = is_same_pos(key->pos, deferred.key);

    // deferred press
    if (deferred.pressed) {
        if (cont.pressed) {
            return PRESS_OTHER;
        }
        if (same_key) {
            return TAP;
        }
        if (original_key) {
            if (key->hold_on_key_press && key->hold_on_key_press(key, cont.key)) {
                return TAP; // TODO: add extra keycode
            }
            return ROLL;
        }
        return RELEASE_OTHER;
    }

    // deferred release
    if (!cont.pressed) {
        return RELEASE_OTHER;
    }

    if (same_key) {
        return TAP;
    }

    return PRESS_OTHER;
}

static void reset_smart_key(smart_key_t *key) {
//    uprintf("DEBUG: reset smart key\n");
    key->state = ((smart_key_t){}).state;
}
