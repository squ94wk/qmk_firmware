#include "magickey.h"
#include "squ94wk.h"
#include "history.h"
#include "metrics.h"

void magickey_action(smart_key_t *key) {
    int parens = 0;
    int brackets = 0;
    int braces = 0;
    int angle = 0;
    for (int i=0; i < KEY_HISTORY_MAX && history[i].c; i++) {
        switch (history[i].c) {
        case '"':
            SEND_STRING("\"");
            drop_key_from_history(i);
            return;
        case '\'':
            SEND_STRING("'");
            drop_key_from_history(i);
            return;
        case '`':
            if (i >= 2 && history[i-1].c == '`' && history[i-2].c == '`') {
                SEND_STRING("```");
                drop_keys_from_history(3, i);
                return;
            }
            SEND_STRING("`");
            drop_key_from_history(i);
            return;
        case ')':
            parens++;
            break;
        case '(':
            if (parens == 0) {
                SEND_STRING(")");
                add_entry_to_history(')', 0);
                return;
            }
            parens--;
            break;
        case ']':
            brackets++;
            break;
        case '[':
            if (brackets == 0) {
                SEND_STRING("]");
                add_entry_to_history(']', 0);
                return;
            }
            brackets--;
            break;
        case '}':
            braces++;
            break;
        case '{':
            if (braces == 0) {
                SEND_STRING("}");
                add_entry_to_history('}', 0);
                return;
            }
            braces--;
            break;
        case '>':
            angle++;
            break;
        case '<':
            if (angle == 0) {
                SEND_STRING(">");
                add_entry_to_history('>', 0);
                return;
            }
            angle--;
            break;
        }
    }
}

typedef struct {
    char *pattern;
    char *expansion;
} magic_expansion_t;
static magic_expansion_t magic_expansions[] = {
    {"ae", "\b\b" SS_RALT("q")},
    {"oe", "\b\b" SS_RALT("p")},
    {"ue", "\b\b" SS_RALT("y")},
    {"Ae", "\b\b" SS_RALT(SS_RSFT("q"))},
    {"Oe", "\b\b" SS_RALT(SS_RSFT("p"))},
    {"Ue", "\b\b" SS_RALT(SS_RSFT("y"))},
    {"ss", "\b\b" SS_RALT("s")},

    {", ", "\b\b. "},
    {". ", "\b\b! "},
    {" ", "\b, "},


    {"(I|i)m", "\b\bI'm"},
    {"((h|H)ow|(w|W)hy|(w|W)ho|(w|W)here|(w|W)hen|(w|W)hat|(t|T)here|(h|H)ere|(t|T)hat|(h|H)e|(l|L)et|(i|I)t)s", "\b's"},
    {"((w|W)hat|(t|T)hey|(w|W)e|(y|Y)ou)re", "\b\b're"},
    {"((d|D)o|(d|D)oes|(w|W)o|(c|C)a|((w|W)ould|(c|C)ould|(s|S)hould)|(m|M)ust|(d|D)id|(h|H)a(s|d|ve)|((i|I)s|(a|A)re|(w|W)(as|ere)))nt", "\b't"},
    {"((i|I)t|(h|H)e|(t|T)hey|(w|W)e|(y|Y)ou|I|(t|T)(hat|his))ll", "\b\b'll"},
    {"((i|I)t|(h|H)ow|(w|W)hy|(w|W)ho|(w|W)here|(w|W)hen|(w|W)hat|(h|H)e|(sh|sH)e|(w|W)e|(t|T)hey|(y|Y)ou|I|(t|T)hat|(t|T)here)d", "\b'd"},
    {"((m|M)ust|(m|M)ight|((w|W)ould|(c|C)ould|(s|S)hould)|(t|T)hey|(w|W)e|(y|Y)ou|I)ve", "\b\b've"},

    {"ful", "ly"},
    {"ize", "\bation"},
    {"(ing|ive)", "ly"},
    {"ence", "ment"},
    {"ment", "ation"},
    {"ure", "\bation"},

    {"(a|A)dd", "itional"},
    {"(a|A)rch", "itecture"},
    {"(d|D)if", "fer"},
    {"iffer", "ent"},
    {"ifferent", "ly"},
    {"(d|D)eps", "\bendencies"},
    {"(d|D)ep", "end"},
    {"epend", "ency"},
    {"(d|D)oc", "ument"},
    {"ocument", "ation"},
    {"(a|A)tt", "ach"},
    {"ttach", "ment"},
    {"(d|D)ef", "ine"},
    {"efine", "\bition"},
    {"(d|D)yn", "amic"},
    {"ynamic", "ally"},
    {"(e|E)p", "\bndpoint"},
    {"(e|E)ndp", "oint"},
    {"(e|E)ndpoint", "Slice"},
    {"(e|E)ps", "\bndpointSlice"},
    {"(r|R)et", "urn"},
    {"(r|R)eq", "uest"},
    {"(p|P)k", "\backage"},
    {"(l|L)ib", "rary"},
    {"(r|R)ef", "erence"},
    {"(m|M)an", "age"},
    {"anage", "ment"},
    {"(n|N)et", "work"},
    {"(n|N)ec", "essary"},
    {"ecessary", "\bily"},
    {"(s|S)uc", "cess"},
    {"uccess", "ful"},
    {"(p|P)os", "sible"},
    {"osible", "\b\bility"},
    {"(p|P)ol", "icy"},
    {"(p|P)rev", "ious"},
    {"(p|P)rov", "ider"},
    {"revious", "ly"},
    {"(p|P)rop", "erty"},
    {"roperty", "\bies"},
    {"(t|T)emp", "late"},
    {"(t|T)mp", "\b\bemplate"},
    {"grpc", "\b\b\bRPC"},
    {"(n|N)ad", "\b\betworkAttachmentDefinition"},
    {"api", "\b\b\bAPI"},
    {"app", "lication"},
    {"sq", "u94wk"},
    {"squ94wk", "@gmail.com"},
    {"(c|C)ont", "roller"},
    {"(c|C)f", "\bonfig"},
    {"(c|C)onf", "ig"},
    {"onfig", "ure"},
    {"onfigure", "\bation"},
    {"(i|I)mp", "lement"},
    {"(i|I)nfo", "rmation"},
    {"pl", "atform"},
    {"(i|I)nit", "ialize"},
    {"(a|A)uto", "matic"},
    {"utomatic", "ally"},
    {"(c|C)on", "nect"},
    {"onnect", "ion"},
    {"(d|D)isc", "onnect"},
    {"(f|F)unc", "tion"},
    {"(e|E)nv", "ironment"},
    {"(s|S)ys", "tem"},
    {"(v|V)ar", "iable"},
    {"(i|I)nt", "erface"},
    {"(o|O)b", "ject"},
    {"(k|K)ube", "rnetes"},
    {"v(c|C)", "luster"},
    {"(n|N)s", "\bamespace"},
    {"(v|V)ol", "ume"},
    {"-o", " yaml"},
    {"-o yaml", "\b\b\b\bwide"},
    {"-o wide", "\b\b\b\bjson"},
    {"-o json", "\b\b\b\byaml"},
    {"nc ", "-w1 -zv "},
    {"~", "/"},
    {"~/", "."},

    {"(e|E)g", "\b.g."},
    {"(i|I)e", "\b.e."},
    {"(z|Z)(b|B)", "\b.B."},
    {"(d|D)h", "\b.h."},
    {"ip", "\b\bIP"},
};

void magickey_action2(smart_key_t *key) {
    uint16_t start_time = timer_read();

    for (size_t i = 0; i < sizeof(magic_expansions) / sizeof(magic_expansions[0]); i++) {
        if (history_matches_string(magic_expansions[i].pattern)) {
            SEND_STRING(magic_expansions[i].expansion);
            add_string_to_history(magic_expansions[i].expansion);

            break;
        }
    }

    uint16_t elapsed = timer_elapsed(start_time);
    metrics_observe(&magic_timing_histogram, elapsed);
}

void magickey_complete(smart_key_t *key) {
    magic_complete_active = true;
}

bool handle_magic_complete(uint16_t *keycode, uint16_t mask) {
    if (!magic_complete_active) {
        return false;
    }
    if (mask) {
        magic_complete_active = false;
        return false;
    }

    char *expansion = NULL;

    switch (*keycode) {
    case KC_A:
        expansion = "ial";
        break;
    case KC_B:
        expansion = "ble";
        break;
    case KC_C:
        expansion = "ence";
        break;
    case KC_D: // D is V on the alpha 2 layer
        expansion = "ive";
        break;
    case KC_E:
        expansion = "ere";
        break;
    case KC_F:
        expansion = "ful";
        break;
    case KC_G:
        expansion = "ing";
        break;
    case KC_H:
        expansion = "ght";
        break;
    case KC_I:
        expansion = "ity";
        break;
    case KC_L:
        expansion = "ly";
        break;
    case KC_M:
        expansion = "ment";
        break;
    case KC_N:
        expansion = "ion";
        break;
    case KC_O:
        expansion = "ous";
        break;
    case KC_P:
        expansion = "ple";
        break;
    case KC_S:
        expansion = "ess";
        break;
    case KC_T:
        expansion = "ent";
        break;
    case KC_U:
        expansion = "ure";
        break;
    case KC_V:
        expansion = "ive";
        break;
    case KC_Z:
        expansion = "ize";
        break;
    }

    magic_complete_active = false;

    if (expansion) {
        SEND_STRING(expansion);
        add_string_to_history(expansion);
        uprintf("DEBUG: magic complete: sent '%s'\n", expansion);
        return true;
    }

    uprintf("DEBUG: magic complete: deactivated\n");
    return true;
}

