enum layers {
    WIN_BASE,
    LAYER_COUNT,
};

enum custom_keycodes {
    CKC_ALPHA_LAYER_SWITCH = SAFE_RANGE,
    CKC_SMART_SHIFT,
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
    RELEASE_SAME,
    PRESS_SAME,
    PRESS_OTHER,
    RELEASE_OTHER,
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
    LAYER_NAV,
    LAYER_NUM,
    // end
    SMART_LAYER_COUNT,
};

// allows self referencing
typedef struct smart_key_t smart_key_t;

struct smart_key_t {
    keypos_t pos;
    uint16_t keycode;

    int max_tap;

    struct {
        uint16_t pressed_time;
        uint16_t tap_count;
        uint16_t tap_timeout;
        bool tap_fired;
        bool hold_fired;
    } state;

    struct {
        uint16_t keycode;
        uint8_t mask;
        uint8_t mask_oneshot;
        int layer_toggle;
        int layer_oneshot;
        void (*action)(smart_key_t *key);
    } tap;

    struct {
        uint16_t keycode;
        uint8_t mask;
        int layer;
        void (*action)(smart_key_t *key);
    } hold;

    void (*release_action)(smart_key_t *key);
    bool (*fire_on_key_press)(smart_key_t *key, keypos_t pos);
};
