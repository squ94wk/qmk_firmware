# squ94wk's Userspace

Custom smart keys engine for QMK firmware.

## Features

- **Smart Keys**: Advanced tap-hold with multi-tap support
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
- `magic.h/c` - Magic key auto-completion

