bool is_same_pos(keypos_t a, keypos_t b) {
    return a.row == b.row && a.col == b.col;
}

#define PTR_TO(x) (&(uint16_t){ (x) })
