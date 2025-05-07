#define PENDING_QUEUE_MAX 8
pending_key_t pending_keys[PENDING_QUEUE_MAX] = {};

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
            return false; // already in
        }
    }
    return false; // full
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
