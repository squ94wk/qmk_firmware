# squ94wk's Userspace

Custom smart keys engine for QMK firmware.

## Features

- **Smart Keys**: Advanced tap-hold with multi-tap support and speculative hold
- **Speculative Hold**: Immediate modifier activation with deferred tap/hold decision
- **Smart Case**: Automatic case conversion (CAPS word, snake_case, kebab-case)
- **Key History**: Tracks recent keystrokes for context-aware features
- **Magic Key**: Auto-completion for brackets, quotes, and common patterns
- **Custom Layers**: Dynamic layer management with oneshot support

## Usage

To use this userspace in your keymap, include the main header:

```c
#include "squ94wk.h"
```

The build system will automatically include this userspace when building a keymap named `squ94wk`.

## Structure

- `squ94wk.h/c` - Core smart key engine
- `smart_case.h/c` - Smart case conversion feature
- `history.h/c` - Key history tracking
- `magickey.h/c` - Magic key auto-completion

## Speculative Hold

Speculative hold is a feature that activates modifiers immediately on key press, then decides whether to keep the modifier held or convert to a tap based on subsequent events.

### How It Works

1. **On Press**: If `speculative_hold` is enabled, the hold action fires immediately
2. **Decision Points**:
   - **Quick Release**: Converts to tap action (modifier is released, tap fires)
   - **Timeout**: Confirms hold (modifier stays active)
   - **Other Key Press**: Confirms hold (modifier applies to next key)
   - **Roll**: Converts to tap (modifier released before next key taps)

### Configuration

Enable per-key by setting the `speculative_hold` flag:

```c
smart_key_t my_shift = {
    .keycode = CKC_SMART_SHIFT,
    .speculative_hold = true,
    .tap.mask_oneshot = MOD_BIT_LSHIFT,
    .hold.keycode = KC_LEFT_SHIFT,
};
```

### Oneshot Modifier Chaining

Speculative hold properly handles oneshot modifier chaining. When transitioning from speculative hold to tap, previously active oneshot modifiers are preserved, allowing you to chain multiple oneshot modifiers together (e.g., tap Shift → tap Ctrl adds both as oneshot).

