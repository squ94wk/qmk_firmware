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
    IDLE, // defer release
};

enum smart_layer {
    LAYER_ALPHA_1,
    LAYER_ALPHA_2,
    LAYER_SYMBOLS,
    LAYER_STRINGS,
    LAYER_BRACKETS,
    LAYER_J_HOLD,
    LAYER_K_HOLD,
    LAYER_L_HOLD,
    LAYER_H_HOLD,
    LAYER_SEARCH,
    LAYER_SEARCH_2,
    LAYER_O_HOLD,
    LAYER_I_HOLD,
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
    // end
    SMART_LAYER_COUNT,
};

enum smart_keys {
    SMART_KEY_SMART_SHIFT,
    SMART_KEY_SMART_CTRL,
    SMART_KEY_SMART_ALT,
    SMART_KEY_SMART_GUI,
    SMART_KEY_SMART_CTRL_SHIFT,
    SMART_KEY_LOCK_KEY,
    // end
    SMART_KEY_COUNT,
};

// allows self referencing
typedef struct smart_key_t smart_key_t;

struct smart_key_t {
    keypos_t pos;
    uint16_t keycode;

    int  max_tap;
    bool defer_release;

    struct {
        uint16_t pressed_time;
        uint16_t tap_count;
        uint16_t tap_timeout;
        bool     fired;

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

// allows self referencing
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
