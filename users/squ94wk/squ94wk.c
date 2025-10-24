#include "squ94wk.h"
#include "smart_case.h"
#include "history.h"
#include "magickey.h"

// Stub for QMK's missing magic() function
__attribute__((weak)) void magic(void) {
    // QMK's bootmagic/magic feature - not needed for this keyboard
}

// Global state
int layer_activations[SMART_LAYER_COUNT];
smart_layer_t *smart_layers[SMART_LAYER_COUNT];
smart_key_t smart_keys[2][SMART_KEY_COUNT];
int active_layers[SMART_LAYER_COUNT];
uint32_t last_input;
bool magic_complete_active = false;

// Pending keys queue
pending_key_t pending_keys[PENDING_QUEUE_MAX] = {};

// Deferred event handling
static keyevent_t deferred_event;
static uint16_t deferred_keycode;

// Housekeeping flag
static bool run_housekeeping = false;

// Forward declarations for internal functions
static bool process_continuation(smart_key_t *key, enum continuation_type cont);
static bool process_event(uint16_t keycode, keyevent_t event);
static bool process_event_with_key(smart_key_t *key, uint16_t keycode, keyevent_t event);
static void reset_smart_key(smart_key_t *smart_key);
static enum smart_key_type get_key_type(smart_key_t *key);
static enum event_type get_event_type(smart_key_t *key, keyevent_t event);
static enum continuation_type get_continuation_type(smart_key_t *key, keyevent_t deferred, keyevent_t cont);
static void speculative_hold_to_tap(smart_key_t *key);
static void speculative_hold_to_hold(smart_key_t *key);

// ============================================================================
// Utility functions (from helper.c)
// ============================================================================

bool is_same_pos(keypos_t a, keypos_t b) {
    return a.row == b.row && a.col == b.col;
}

// ============================================================================
// Pending key queue management (from pending.c)
// ============================================================================

bool add_pending_key(smart_key_t *key) {
    for (int i = 0; i < PENDING_QUEUE_MAX; ++i) {
        if (pending_keys[i].key == NULL) {
            pending_keys[i] = (pending_key_t){
                .key = key,
            };
            uprintf("DEBUG: pending keys:");
            for (int j=0; j<PENDING_QUEUE_MAX; ++j) {
                if (!pending_keys[j].key) {
                    break;
                }
                uprintf(" %s", keycode_to_string(pending_keys[j].key->keycode));
            }
            uprintf("\n");
            uprintf("DEBUG: added pending key %s in position %d\n", keycode_to_string(key->keycode), i);
            return true;
        }
        if (pending_keys[i].key == key) {
            return false;
        }
    }
    return false;
}

bool remove_pending_key(smart_key_t *key) {
    for (int i = 0; i < PENDING_QUEUE_MAX; ++i) {
        if (pending_keys[i].key == key) {
            uprintf("DEBUG: remove pending key in position %d\n", i);
            memmove(&pending_keys[i], &pending_keys[i+1], sizeof(pending_keys[0]) * (PENDING_QUEUE_MAX-i-1));
            pending_keys[PENDING_QUEUE_MAX-1].key = NULL;

            uprintf("DEBUG: pending keys:");
            for (int j=0; j<PENDING_QUEUE_MAX; ++j) {
                if (!pending_keys[j].key) {
                    break;
                }
                uprintf(" %s", keycode_to_string(pending_keys[j].key->keycode));
            }
            uprintf("\n");

            return true;
        }
    }

    return false;
}

// ============================================================================
// Virtual/register functions (from extern.c/virtual.c)
// ============================================================================

void register_with_mods(uint16_t *keycode, uint16_t mask, uint16_t *release_mask) {
    uint16_t current_mods = get_mods();

    if (handle_magic_complete(keycode, mask)) {
        return;
    }

    if (handle_smart_case(keycode, &mask, release_mask)) {
        return;
    }

    if (get_oneshot_mods()) {
        mask |= get_oneshot_mods();
        uprintf("DEBUG: apply oneshot mask: %d\n", get_oneshot_mods());
        clear_oneshot_mods();
    }

    uint32_t time = timer_read32();
    uint32_t last_key_time = history[0].time;
    if (!mask && keycode && *keycode >= KC_A && *keycode <= KC_Z && last_key_time && time < last_key_time + 1000 && history_matches_string(PATTERN("(z|y|x|w|v|u|t|s|r|q|p|o|n|m|l|k|j|i|h|g|f|e|d|c|b|a)(?|!|.) "))) {
        // Don't auto-shift for common abbreviations
        if (history_matches_string(PATTERN("e.g. ")) ||
            history_matches_string(PATTERN("i.e. ")) ||
            history_matches_string(PATTERN("z.B. ")) ||
            history_matches_string(PATTERN("etc. ")) ||
            history_matches_string(PATTERN("usw. ")) ||
            history_matches_string(PATTERN("vs. ")) ||
            history_matches_string(PATTERN("sth. "))) {
            // Skip auto-shift for abbreviations
        } else {
            mask |= MOD_MASK_SHIFT;
        }
    }

    uprintf("DEBUG: register %s with mask %d\n", keycode_to_string(*keycode), mask);

    set_mods(current_mods | mask);
    register_code(*keycode);

    if (IS_MODIFIER_KEYCODE(*keycode)) {
        *release_mask = mask & ~current_mods;
    } else {
        add_key_to_history(*keycode, get_mods());
        set_mods(current_mods);
    }

    for (int i=0; i < SMART_LAYER_COUNT && active_layers[i]; ++i) {
        smart_layer_t *l = smart_layers[active_layers[i]];
        if (l->oneshot_on_key_press(l, *keycode, mask)) {
            if (layer_activations[active_layers[i]] > 0) {
                layer_activations[active_layers[i]]--;
            }
        }
    }
}

// ============================================================================
// Layer management
// ============================================================================

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
    layer_activations[layer] = -1;
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

// ============================================================================
// Key state management
// ============================================================================

void press_key(smart_key_t *key, keyevent_t event) {
    key->state.pressed_time = event.time;
    add_pending_key(key);
}

void release_key(smart_key_t *key) {
    key->state.pressed_time = 0;
}

// ============================================================================
// Key actions
// ============================================================================

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
        set_oneshot_mods(get_oneshot_mods() | key->tap.mask_oneshot);
        return;
    }
    uint16_t kc = key->tap.keycode;
    register_with_mods(&kc, key->tap.mask, &key->state.release.mask);
    key->state.release.keycode = kc;
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

        // carry oneshot mods over to layers
        if (get_oneshot_mods()) {
            key->state.release.mask = get_oneshot_mods();
            for (uint16_t kc = KC_LEFT_CTRL; kc <= KC_RIGHT_GUI; kc++) {
                if (key->state.release.mask & MOD_BIT(kc)) {
                    register_code(kc);
                }
            }
            clear_oneshot_mods();
        }

        uprintf("DEBUG: remember to deactivate layer %s on release\n", layer_to_string(key->state.release.layer));
        return;
    }

    uint16_t kc = key->hold.keycode;
    uint16_t release_mask;
    uint8_t oneshot_mask = get_oneshot_mods();
    register_with_mods(&kc, key->hold.mask, &release_mask);

    if (key->speculative_hold) {
        key->state.speculative_oneshot_mask = oneshot_mask;
        key->state.release.mask = release_mask & ~oneshot_mask;
    } else {
        key->state.release.mask = release_mask;
    }

    uprintf("DEBUG: remembered mask: %d\n", key->state.release.mask);
    key->state.release.keycode = kc;
    if (key->hold.tap_keycode) {
        key->state.pressed_time = 0;
    }
}

void release_action(smart_key_t *key) {
    if (key->release_action) {
        key->release_action(key);
        return;
    }

    if (!key->state.fired && !key->state.is_speculative_hold) {
        uprintf("DEBUG: releasing key that hasn't fired\n");
        return;
    }

    if (key->state.is_speculative_hold) {
        if (key->state.fired) {
            key->state.release.mask |= key->state.speculative_oneshot_mask;
            uprintf("DEBUG: releasing after confirmed hold - added captured oneshot mask %d\n", key->state.speculative_oneshot_mask);
        } else {
            set_oneshot_mods(get_oneshot_mods() | key->state.speculative_oneshot_mask);
            key->state.release.mask |= key->state.speculative_oneshot_mask;
            uprintf("DEBUG: releasing and transition to tap - restored oneshot mask %d\n", key->state.speculative_oneshot_mask);
        }
    }

    if (key->state.release.keycode) {
        unregister_code(key->state.release.keycode);
    }
    if (key->state.release.mask) {
        for (uint16_t kc = KC_LEFT_CTRL; kc <= KC_RIGHT_GUI; kc++) {
            if (key->state.release.mask & MOD_BIT(kc)) {
                unregister_code(kc);
            }
        }
    }
    if (key->state.release.layer) {
        if (layer_activations[key->state.release.layer] != -2) {
            deactivate_layer(key->state.release.layer);
        }
    }

    return;
}

// ============================================================================
// Speculative hold transitions
// ============================================================================

static void speculative_hold_to_tap(smart_key_t *key) {
    uprintf("DEBUG: speculative hold -> tap\n");
    release_action(key);
    key->state.is_speculative_hold = false;
    tap_action(key);
}

static void speculative_hold_to_hold(smart_key_t *key) {
    uprintf("DEBUG: speculative hold confirmed\n");
    key->state.fired = true;
}

// ============================================================================
// Core event processing (from smart.c)
// ============================================================================

bool process_smart_key(uint16_t keycode, keyrecord_t *record) {
    run_housekeeping = true;

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
            if (key->state.is_speculative_hold) {
                speculative_hold_to_tap(key);
            } else if (!key->state.fired) {
                tap_action(key);
            }
            return true;

        case TAP:
        case PRESS_THIRD:
        case HOLD:
            if (key->state.is_speculative_hold) {
                speculative_hold_to_hold(key);
            } else {
                hold_action(key);
            }
            return true;

        case TAP_ORIGINAL:
            return false;

        case IDLE:
            if (key->state.is_speculative_hold) {
                speculative_hold_to_tap(key);
            } else {
                tap_action(key);
            }
            return true;

        case RELEASE_THIRD:
            return false;
        }
    default:
        return false;
    }
    return false;
}

static bool process_event(uint16_t keycode, keyevent_t event) {
    for (int i = 0; i < PENDING_QUEUE_MAX && pending_keys[i].key; ++i) {
        smart_key_t *key = pending_keys[i].key;
        if (key->state.fired) {
            continue;
        }
        if (process_event_with_key(key, keycode, event)) {
            return true;
        }
    }

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
            register_with_mods(&keycode, 0, NULL);
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
            if (key->speculative_hold) {
                hold_action(key);
                key->state.fired = false;
                key->state.is_speculative_hold = true;
                uprintf("DEBUG: speculative hold activated for %s\n", keycode_to_string(key->keycode));
            }
            return true;
        case PRESS_OTHER:
            if (key->state.fired) {
                return false;
            }
            if (key->tap_on_key_press && key->tap_on_key_press(key, event.key)) {
                uprintf("DEBUG: immediately tap key %s\n", keycode_to_string(key->keycode));
                if (key->state.is_speculative_hold) {
                    speculative_hold_to_tap(key);
                } else {
                    tap_action(key);
                }
                return false;
            }
            if (key->hold_on_key_press && key->hold_on_key_press(key, event.key)) {
                uprintf("DEBUG: immediately hold key %s\n", keycode_to_string(key->keycode));
                if (key->state.is_speculative_hold) {
                    speculative_hold_to_hold(key);
                } else {
                    hold_action(key);
                }
                return false;
            }
            uprintf("DEBUG: defer press of key %s\n", keycode_to_string(keycode));
            deferred_event = event;
            deferred_keycode = keycode;
            return true;
        case RELEASE_OTHER:
            return false;
        case RELEASE_SAME:
            if (key->state.is_speculative_hold && !key->state.fired) {
                speculative_hold_to_tap(key);
                release_key(key);
                return true;
            }

            if (key->state.fired) {
                release_key(key);
                return true;
            }

            if (deferred_event.time == event.time) {
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
            if (key->speculative_hold && key->state.tap_count == key->max_tap) {
                hold_action(key);
                key->state.fired = false;
                key->state.is_speculative_hold = true;
                uprintf("DEBUG: speculative hold activated for %s (N_TAP_HOLD)\n", keycode_to_string(key->keycode));
            }
            return true;
        case RELEASE_SAME:
            if (key->state.is_speculative_hold && !key->state.fired) {
                speculative_hold_to_tap(key);
                release_key(key);
                return true;
            }

            release_key(key);
            if (key->state.fired) {
                return true;
            }

            if (deferred_event.time == event.time) {
                release_key(key);
                return true;
            }

            if (key->defer_release) {
                uprintf("DEBUG: defer release of key %s\n", keycode_to_string(keycode));
                deferred_event = event;
                deferred_keycode = keycode;
                return true;
            }

            return true;
        case PRESS_OTHER:
            if (key->state.fired) {
                return false;
            }
            if (key->state.pressed_time) {
                if (key->tap_on_key_press && key->tap_on_key_press(key, event.key)) {
                    uprintf("DEBUG: immediately tap key %s\n", keycode_to_string(key->keycode));
                    if (key->state.is_speculative_hold) {
                        speculative_hold_to_tap(key);
                    } else {
                        tap_action(key);
                    }
                    return false;
                }
                if (key->hold_on_key_press && key->hold_on_key_press(key, event.key)) {
                    uprintf("DEBUG: immediately hold key %s\n", keycode_to_string(key->keycode));
                    if (key->state.is_speculative_hold) {
                        speculative_hold_to_hold(key);
                    } else {
                        hold_action(key);
                    }
                    return false;
                }
                uprintf("DEBUG: defer press of key %s\n", keycode_to_string(keycode));
                deferred_event = event;
                deferred_keycode = keycode;
                return true;
            } else {
                key->state.fired = true;
                tap_action(key);
            }
            return false;
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

    for (int i=0; i < PENDING_QUEUE_MAX && pending_keys[i].key; ++i) {
        pending_key_t key = pending_keys[i];
        if (!key.key->state.fired) {
            continue;
        }
        if (key.key->state.pressed_time) {
            continue;
        }

        release_action(key.key);
        reset_smart_key(key.key);
        remove_pending_key(key.key);
        --i;
    }

    for (int i=0; i < SMART_LAYER_COUNT; ++i) {
        if (!layer_activations[i]) {
            deactivate_layer(i);
        }
    }
}

void matrix_scan_user(void) {
    for (int i = 0; i < PENDING_QUEUE_MAX && pending_keys[i].key; ++i) {
        smart_key_t *key = pending_keys[i].key;
        if (key->state.fired) {
            continue;
        }

        if (key->state.pressed_time && timer_elapsed(key->state.pressed_time) > TAPPING_TERM) {
            run_housekeeping = true;
            if (deferred_keycode) {
                uint16_t code = deferred_keycode;
                deferred_keycode = KC_NO;
                process_event_with_key(key, code, deferred_event);
                continue;
            }

            if (key->state.is_speculative_hold) {
                speculative_hold_to_hold(key);
                continue;
            }

            process_continuation(key, HOLD);
            continue;
        }

        if (key->state.tap_timeout && timer_read() > key->state.tap_timeout) {
            run_housekeeping = true;
            process_continuation(key, IDLE);
            continue;
        }
    }

    if (rgb_matrix_is_enabled() && timer_read32() > last_input + RGB_MATRIX_IDLE_TIMEOUT) {
        rgb_matrix_disable();
    }
}

// ============================================================================
// Helper functions
// ============================================================================

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
    bool same_key = is_same_pos(cont.key, deferred.key);

    if (deferred.pressed) {
        if (cont.pressed) {
            return PRESS_THIRD;
        }
        if (same_key) {
            return TAP;
        }
        if (original_key) {
            if (key->hold_on_key_press && key->hold_on_key_press(key, cont.key)) {
                return TAP;
            }
            return ROLL;
        }
        return RELEASE_OTHER;
    }

    if (!cont.pressed) {
        return RELEASE_OTHER;
    }

    if (original_key) {
        return TAP_ORIGINAL;
    }

    if (key->hold_on_key_press && key->hold_on_key_press(key, cont.key)) {
        return TAP;
    }
    return ROLL;
}

static void reset_smart_key(smart_key_t *key) {
    key->state = ((smart_key_t){}).state;
}

// ============================================================================
// Debug helpers (from tostring.c)
// ============================================================================

const char* keycode_to_string(uint16_t keycode) {
    switch (keycode) {
        case KC_NO: return "KC_NO";
        case KC_TRANSPARENT: return "KC_TRANSPARENT";
        case KC_A: return "KC_A";
        case KC_B: return "KC_B";
        case KC_C: return "KC_C";
        case KC_D: return "KC_D";
        case KC_E: return "KC_E";
        case KC_F: return "KC_F";
        case KC_G: return "KC_G";
        case KC_H: return "KC_H";
        case KC_I: return "KC_I";
        case KC_J: return "KC_J";
        case KC_K: return "KC_K";
        case KC_L: return "KC_L";
        case KC_M: return "KC_M";
        case KC_N: return "KC_N";
        case KC_O: return "KC_O";
        case KC_P: return "KC_P";
        case KC_Q: return "KC_Q";
        case KC_R: return "KC_R";
        case KC_S: return "KC_S";
        case KC_T: return "KC_T";
        case KC_U: return "KC_U";
        case KC_V: return "KC_V";
        case KC_W: return "KC_W";
        case KC_X: return "KC_X";
        case KC_Y: return "KC_Y";
        case KC_Z: return "KC_Z";
        case KC_1: return "KC_1";
        case KC_2: return "KC_2";
        case KC_3: return "KC_3";
        case KC_4: return "KC_4";
        case KC_5: return "KC_5";
        case KC_6: return "KC_6";
        case KC_7: return "KC_7";
        case KC_8: return "KC_8";
        case KC_9: return "KC_9";
        case KC_0: return "KC_0";
        case KC_ENTER: return "KC_ENTER";
        case KC_ESCAPE: return "KC_ESCAPE";
        case KC_BACKSPACE: return "KC_BACKSPACE";
        case KC_TAB: return "KC_TAB";
        case KC_SPACE: return "KC_SPACE";
        case KC_MINUS: return "KC_MINUS";
        case KC_EQUAL: return "KC_EQUAL";
        case KC_LEFT_BRACKET: return "KC_LEFT_BRACKET";
        case KC_RIGHT_BRACKET: return "KC_RIGHT_BRACKET";
        case KC_BACKSLASH: return "KC_BACKSLASH";
        case CKC_SMART_SHIFT: return "CKC_SMART_SHIFT";
        case CKC_SMART_CTRL: return "CKC_SMART_CTRL";
        case CKC_SMART_ALT: return "CKC_SMART_ALT";
        case CKC_SMART_CTRL_SHIFT: return "CKC_SMART_CTRL_SHIFT";
        case CKC_LOCK_KEY: return "CKC_LOCK_KEY";
        case CKC_MAGIC: return "CKC_MAGIC";
    }
    return "UNDEFINED";
}

const char* key_type_to_string(enum smart_key_type type) {
    switch (type) {
        case DUMB: return "DUMB";
        case TAP_ONLY: return "TAP_ONLY";
        case TAP_HOLD: return "TAP_HOLD";
        case N_TAP: return "N_TAP";
        case N_TAP_HOLD: return "N_TAP_HOLD";
        case HOLD_ONLY: return "HOLD_ONLY";
        case N_HOLD: return "N_HOLD";
    }
    return "UNKNOWN";
}

const char* event_to_string(enum event_type event) {
    switch (event) {
        case PRESS_SAME: return "PRESS_SAME";
        case RELEASE_SAME: return "RELEASE_SAME";
        case RELEASE_OTHER: return "RELEASE_OTHER";
        case PRESS_OTHER: return "PRESS_OTHER";
    }
    return "UNKNOWN";
}

const char* continuation_to_string(enum continuation_type event) {
    switch (event) {
        case RELEASE_THIRD: return "RELEASE_THIRD";
        case PRESS_THIRD: return "PRESS_THIRD";
        case TAP_ORIGINAL: return "TAP_ORIGINAL";
        case TAP: return "TAP";
        case ROLL: return "ROLL";
        case HOLD: return "HOLD";
        case IDLE: return "IDLE";
    }
    return "UNKNOWN";
}

const char* layer_to_string(int layer) {
    switch (layer) {
        case LAYER_ALPHA_1: return "LAYER_ALPHA_1";
        case LAYER_ALPHA_2: return "LAYER_ALPHA_2";
        case LAYER_SYMBOLS: return "LAYER_SYMBOLS";
        case LAYER_SHIFT_MODES: return "LAYER_SHIFT_MODES";
        case LAYER_SYMBOLS_2: return "LAYER_SYMBOLS_2";
        case LAYER_VIM_TEXT: return "LAYER_VIM_TEXT";
        case LAYER_VIM_NAV: return "LAYER_VIM_NAV";
        case LAYER_L_HOLD: return "LAYER_L_HOLD";
        case LAYER_H_HOLD: return "LAYER_H_HOLD";
        case LAYER_SYS: return "LAYER_SYS";
        case LAYER_NUM: return "LAYER_NUM";
        case LAYER_FUNCTION_KEYS: return "LAYER_FUNCTION_KEYS";
        case LAYER_TMUX: return "LAYER_TMUX";
#ifdef MOUSEKEY_ENABLE
        case LAYER_MOUSE: return "LAYER_MOUSE";
#endif
        default: return "UNKNOWN";
    }
}

