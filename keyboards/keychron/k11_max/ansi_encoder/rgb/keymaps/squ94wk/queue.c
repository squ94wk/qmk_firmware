#define PENDING_QUEUE_MAX 8
smart_key_t *pending_keys[PENDING_QUEUE_MAX] = {};

bool add_pending_key(smart_key_t *key) {
    for (int i = 0; i < PENDING_QUEUE_MAX; ++i) {
        if (pending_keys[i] == NULL) {
            pending_keys[i] = key;
            return true;
        }
        if (pending_keys[i] == key) {
            return false; // already in
        }
    }
    return false; // full
}

bool remove_pending_key(smart_key_t *key) {
    for (int i = 0; i < PENDING_QUEUE_MAX; ++i) {
        if (pending_keys[i] == key) {
            for (int j = i+1; j < PENDING_QUEUE_MAX; ++j) {
                pending_keys[j-1] = pending_keys[j];
            }
            pending_keys[PENDING_QUEUE_MAX-1] = NULL;
            return true;
        }
    }

    return false;
}
