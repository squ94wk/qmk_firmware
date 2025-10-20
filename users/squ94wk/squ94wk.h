#pragma once

#include "quantum.h"
#include "print.h"

// Forward declarations for features
#include "smart_case.h"
#include "history.h"
#include "magickey.h"

enum layers {
    WIN_BASE,
    LAYER_COUNT,
};

enum custom_keycodes {
    CKC_ALPHA_LAYER_SWITCH = SAFE_RANGE,
    CKC_MAGIC,
    CKC_SMART_SHIFT,
    CKC_SMART_CTRL,
    CKC_SMART_ALT,
    CKC_SMART_GUI,
    CKC_SMART_CTRL_SHIFT,
    CKC_LOCK_KEY,
};

enum smart_key_type {
    DUMB,
    TAP_ONLY,
    TAP_HOLD,
    N_TAP,
    N_TAP_HOLD,
    HOLD_ONLY,
    N_HOLD,
};

enum event_type {
    PRESS_SAME,
    RELEASE_SAME,
    PRESS_OTHER,
    RELEASE_OTHER,
};

enum continuation_type {
    TAP_ORIGINAL,
    TAP,
    ROLL,
    HOLD,
    PRESS_THIRD,
    RELEASE_THIRD,
    IDLE,
};

enum smart_layer {
    LAYER_ALPHA_1,
    LAYER_ALPHA_2,
    LAYER_SYMBOLS,
    LAYER_SYMBOLS_2,
    LAYER_SHIFT_MODES,
    LAYER_VIM_TEXT,
    LAYER_VIM_NAV,
    LAYER_L_HOLD,
    LAYER_H_HOLD,
    LAYER_SYS,
    LAYER_NUM,
    LAYER_FUNCTION_KEYS,
    LAYER_TMUX,
    LAYER_DUMB,
    LAYER_DUMB_2,
#ifdef MOUSEKEY_ENABLE
    LAYER_MOUSE,
#endif
    LAYER_JUMP_MAC,
    LAYER_JUMP_WIN,
    SMART_LAYER_COUNT,
};

enum smart_keys {
    SMART_KEY_SMART_SHIFT,
    SMART_KEY_SMART_CTRL,
    SMART_KEY_SMART_ALT,
    SMART_KEY_SMART_GUI,
    SMART_KEY_SMART_CTRL_SHIFT,
    SMART_KEY_LOCK_KEY,
    SMART_KEY_COUNT,
};

typedef struct smart_key_t smart_key_t;

struct smart_key_t {
    keypos_t pos;
    uint16_t keycode;

    int  max_tap;
    bool defer_release;
    bool speculative_hold;

    struct {
        uint16_t pressed_time;
        uint16_t tap_count;
        uint16_t tap_timeout;
        bool     fired;
        bool     is_speculative_hold;
        uint8_t  speculative_oneshot_mask;

        struct {
            uint16_t keycode;
            uint16_t mask;
            int      layer;
        } release;
    } state;

    struct {
        uint16_t keycode;
        uint8_t  mask;
        uint8_t  mask_oneshot;
        int      layer_toggle;
        int      layer_oneshot;
        void (*action)(smart_key_t *key);
    } tap;

    struct {
        uint16_t keycode;
        bool     tap_keycode;
        uint8_t  mask;
        int      layer;
        void (*action)(smart_key_t *key);
    } hold;

    void (*release_action)(smart_key_t *key);
    bool (*tap_on_key_press)(smart_key_t *key, keypos_t pos);
    bool (*hold_on_key_press)(smart_key_t *key, keypos_t pos);
};

typedef struct smart_layer_t smart_layer_t;

struct smart_layer_t {
    smart_key_t *map[MATRIX_ROWS][MATRIX_COLS];
    bool (*on_layer_activate)(smart_layer_t *layer, int layer_index);
    bool (*oneshot_on_key_press)(smart_layer_t *layer, uint16_t keycode, uint16_t modmask);
};

typedef struct pending_key_t pending_key_t;
struct pending_key_t {
    smart_key_t *key;
};

// Global state
extern int layer_activations[SMART_LAYER_COUNT];
extern smart_layer_t *smart_layers[SMART_LAYER_COUNT];
extern smart_key_t smart_keys[2][SMART_KEY_COUNT];
extern int active_layers[SMART_LAYER_COUNT];
extern uint32_t last_input;

// Forward declaration for QMK keymap array
extern const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS];

#define PENDING_QUEUE_MAX 8
extern pending_key_t pending_keys[PENDING_QUEUE_MAX];

// Core engine functions
bool process_smart_key(uint16_t keycode, keyrecord_t *record);
smart_key_t *lookup_key(uint16_t keycode, keypos_t key);

// Layer management
bool is_layer_active(int layer);
bool activate_layer(int layer);
bool deactivate_layer(int layer);
void toggle_layer(int layer);

// Key actions
void press_key(smart_key_t *key, keyevent_t event);
void release_key(smart_key_t *key);
void tap_action(smart_key_t *key);
void hold_action(smart_key_t *key);
void release_action(smart_key_t *key);

// Pending key queue
bool add_pending_key(smart_key_t *key);
bool remove_pending_key(smart_key_t *key);

// Utility functions
bool is_same_pos(keypos_t a, keypos_t b);
void register_with_mods(uint16_t *keycode, uint16_t mask, uint16_t *release_mask);

// Debug helpers
const char* keycode_to_string(uint16_t keycode);
const char* layer_to_string(int layer);
const char* key_type_to_string(enum smart_key_type type);
const char* event_to_string(enum event_type type);
const char* continuation_to_string(enum continuation_type type);

#define PTR_TO(x) (&(uint16_t){ (x) })

