#  Custom QMK Firmware for Keychron K11 Max

This is keyboard firmware based on the [keychron qmk fork](https://github.com/keychron/qmk_firmware).

It adds an entirely separate event processing layer on top of basic QMK features.

## Intention

The initial motivation for this firmware was wrist pain from programming and the search for ways to reduce contortions.
The main contributors were the following:

- Ctrl + [

    I used this predominantly over ESC or caps lock.
    At the time it was the only way for me to differentiate between exiting vim insert mode and switching back to the editor in the Goland IDE.

- Backspace

    I would quickly fling my wrist to reach this key.

- Shift/Control combinations

    This not only stretched out my left hand, but also constantly misaligned my fingers from their optimal position.
    I noticed afterward that the fingers I used for A, S & D were shifted one key to the left.
    This meant constant re-alignment and searching for keys.

- Number line and symbols

    As a programmer, these keys are part of the day-to-day vocabulary, probably sometimes more than the alpha keys.
    But they required both, a shift hold and reaching up two rows with the respective fingers.

I decided to buy a more ergonomic mechanical keyboard and one that I could program to improve on some of these aspects.
Me choice fell on the Keychron K11 Max, since it offered good connectivity, good support (QMK) and came in an Alice layout.

After a little bit of time with the firmware, I quickly noticed that I wanted to drop far more than just the function row.
And after trying to get my layout and its main concepts to work with QMK, I learned that the only way to get what I wanted was to write my own customized firmware.

## Rationale

The layout has some fundamental concepts that I came up with very early (even before getting my board):

- Put brackets (`({[<>]})`) on pairs of keys that produce the opening or closing variants depending on which is held and which is tapped
- Use separate layers for numbers, different sets of symbols
- Put layering keys close to home row
- Use home row combos as modifier keys

Later, after reading a lot on different layouts, I made the radical decision to switch my alpha layout, too.
I started with something close to Colemak-DH except for mapping it on only 20 keys and a second alpha layer.
This alleviates any lateral finger movement, and the pinky never moves at all.

Edit: I later refined the layout more and more and dropped some (pinky) but kept other constraints (no lateral).

## Layout

It took a lot of trial and error to settle on my current layout.

I tested this layout against many other popular modern layouts synthetically (using my own [analyzer](https://github.com/squ94wk/keyboard-analyzer/tree/main) I vibe coded from scratch).
I took the second alpha layer and thumb keys into account.

While the extra layer switch obviously means more keys to press (5%), this layout achieves excellent stats and boasts high quality & comfortable typing (adjusted to the extra presses):

- No (that is 0%) lateral movement
- Very low SFBs (0.68%) with strong fingers
- Very good redirects (2.7%)
- Minimal scissors
- Evenly balanced (52%/48%)
- Prefers strong fingers (thumb -> pinky 22.6%, 26.8%, 26.5%, 13.7%, 10.4%)
- Mid rolls (this I'm not sure, I calculated right).

I tried two letters on thumbs for a while, and it's true, it sucks.
I also tried using more keys, but lateral movement feels so much less zen, I take rolling sequences any day over that.
I even removed letters from the vowel cluster side, because consonants, even rare ones, drive up SFBs drastically.

So I have a few keys left over now that I am still unsure what to put on there.
Maybe punctuation, but I like that on my second layer.

#### Mod keys

I like the idea of home row mods.
However, modifier keys have a secondary role in my keyboard use.
I switched almost all the shortcuts in editors, shell etc. to behave like vim, and I prefer key sequences over combinations with modifiers.
E.g., I previously used `CTRL+E` to switch files, now I use `g e`.

Also, I'd rather mis**type** than trigger some unintended shortcut in a program/terminal.

The best of both worlds is home row combos for modifiers.
So I have:

- `- - M I` = Shift
- `- R M -` = Control/Command
- `- R - I` = Alt/Option
- `- R M I` = Shift + Control/Command

All of these are mirrored on both sides.

And I go a step further:

- Tapping any of these makes them oneshot keys ("sticky")
- I can chain any oneshot keys with other modifiers and also "transfer" them on (layer) key holds

    E.g. `SHIFT CTRL+(any thing here)` means SHIFT is held until CTRL is released even after shift itself is released.

- Mod keys are OS "aware". I cannot use the integration to detect it and instead have a switch for it (`SPC+R_SPC`).

I later added other keys as additional shift keys when held for more comfortable typing.

#### Alpha

~~My alpha layer is roughly copied from Colemak~~
~~I had to find additional candidates to remove from my primary alpha layer, and some other layouts would have made that difficult.~~

My layout no longer closely resembles any well-known layout.
The most outstanding feature even for sub-26 key layouts is that I map two home row keys as my alpha layer change keys.
I just find that way more comfortable than a thumb key.
But this bends a lot of the rules for layouts.

When you add up the frequencies of the letters I must omit in my primary layer, it's roughly in line with that of letters that would deserve a spot on the ring finger home row.
Also, it should roll well.

```
      ╭─────┬─────┬─────╮                   ╭─────┬─────┬─────╮
      │  B  │  L  │  W  │                   │  F  │  O  │  U  │
╭─────┼─────┼─────┼─────┤                   │─────┼─────┼─────┼─────╮
│  C  │ ESC │  S  │  T  │                   │  N  │  E  │MAGIC│  I  │
│─────┼─────┼─────┼─────┤                   │─────┼─────┼─────┼─────╯
│  M  │  R  │  G  │  D  │                   │  H  │     │  /  │
╰─────┴─────┴───┬─┴─────┴─┬─────╮   ╭─────┬─┴─────┴─┬───┴─────╯
                │  SPACE  │ REP │   │     │    A    │
                ╰─────────┴─────╯   ╰─────┴─────────╯
```

Tapping `REP` repeats the last key. Holding it activates the number layer.

The second alpha layer may alternatively be activated with the right pinky.
This additional option proves valuable when typing certain words.
E.g., in the otherwise one-finger bigram "ry", the `y` can instead be an inward roll with the right hand.

#### Alpha 2 (`ESC` or `MAGIC`)


```
      ╭─────┬─────┬─────╮                   ╭─────┬─────┬─────╮ 
      │  @  │  X  │  ?  │                   │  P  │  ~  │  Q  │ 
╭─────┼─────┼─────┼─────┤                   │─────┼─────┼─────┼─────╮
│     │ALPH2│  ,  │  .  │                   │  Y  │  K  │MAGC2│  J  │
│─────┼─────┼─────┼─────┤                   │─────┼─────┼─────┼─────╯
│     │     │     │  V  │                   │  Z  │  :  │  ;  │
╰─────┴─────┴───┬─┴─────┴─┬─────╮   ╭─────┬─┴─────┴─┬───┴─────╯
                │  SMART  │     │   │     │         │
                ╰─────────┴─────╯   ╰─────┴─────────╯
```

- The SMART key enters [smart word mode](#smart-word).

#### Symbols 1 (`T`)

```
      ╭─────┬─────┬─────╮                   ╭─────┬─────┬─────╮ 
      │     │     │     │                   │  !  │  /  │  &  │ 
╭─────┼─────┼─────┼─────┤                   │─────┼─────┼─────┼─────╮
│     │     │     │  T  │                   │  :  │  -  │  =  │  |  │
│─────┼─────┼─────┼─────┤                   │─────┼─────┼─────┼─────╯
│     │     │     │     │                   │  _  │  *  │  \  │
╰─────┴─────┴───┬─┴─────┴─┬─────╮   ╭─────┬─┴─────┴─┬───┴─────╯
                │         │     │   │     │         │
                ╰─────────┴─────╯   ╰─────┴─────────╯
```

This is optimized mainly for Go.
Sequences like `:=`, `!=` are comfortable rolls.

#### Symbols 2 (`D`)

```
      ╭─────┬─────┬─────╮                   ╭─────┬─────┬─────╮ 
      │     │     │     │                   │  "  │  '  │  `  │ 
╭─────┼─────┼─────┼─────┤                   │─────┼─────┼─────┼─────╮
│     │     │     │     │                   │  (  │  {  │  !  │  #  │
│─────┼─────┼─────┼─────┤                   │─────┼─────┼─────┼─────╯
│     │     │     │  D  │                   │  <  │  [  │  $  │
╰─────┴─────┴───┬─┴─────┴─┬─────╮   ╭─────┬─┴─────┴─┬───┴─────╯
                │         │     │   │     │         │
                ╰─────────┴─────╯   ╰─────┴─────────╯
```

This has all the brackets positioned according to their frequency.
The additional `!$` and strings lend themselves well for shell code like: `"${}`, `$()`, `!!` or `!$`.

Also, double tapping `(`, writes `()`. Function calls or definitions with empty arg lists are quick.
Other ones like `{{`, `[[` or `<<` would be counter-productive because they appear in templating or are vim commands I use a lot.

#### Numbers (`NUM`)

```
      ╭─────┬─────┬─────╮                   ╭─────┬─────┬─────╮ 
      │  ^  │  %  │  :  │                   │  ⌫  │  -  │  +  │
╭─────┼─────┼─────┼─────┤                   │─────┼─────┼─────┼─────╮
│  8  │  7  │  6  │  5  │                   │  1  │  2  │  3  │  4  │
│─────┼─────┼─────┼─────┤                   │─────┼─────┼─────┼─────╯
│     │  ,  │  .  │  9  │                   │  0  │  *  │  /  │
╰─────┴─────┴───┬─┴─────┴─┬─────╮   ╭─────┬─┴─────┴─┬───┴─────╯
                │         │ NUM │   │     │         │
                ╰─────────┴─────╯   ╰─────┴─────────╯
```

The numbers are arranged to use the index finger for the most frequent numbers.
The additional symbols mean there's no need to switch when typing math terms, incl. decimals, or IP addresses.

Double tapping and holding the number layer key switches to the function key layer instead.
I rarely use them, so I don't mind the indirection.

#### SYS (`SPC`)

```
      ╭─────┬─────┬─────╮                   ╭─────┬─────┬─────╮
      │⇧TAB │     │  `  │                   │  ⌫  │  ↑  │  ⌦  │
╭─────┼─────┼─────┼─────┤                   │─────┼─────┼─────┼─────╮
│ ^R  │ ESC │ ENT │ TAB │                   │  ←  │  ↓  │  →  │ ^Z  │
│─────┼─────┼─────┼─────┤                   │─────┼─────┼─────┼─────╯
│     │SYS 2│W-MNG│ TMX │                   │ ^C  │ ^X  │ ^V  │
╰─────┴─────┴───┬─┴─────┴─┬─────╮   ╭─────┬─┴─────┴─┬───┴─────╯
                │  SPACE  │     │   │     │   ^A    │
                ╰─────────┴─────╯   ╰─────┴─────────╯
```

- The `SYS 2` layer switch enables word/page wise navigation and backspace.
- The `W-MNG` key is like the "MEH" key except it omits shift, which can be added to support additional combos. I use this key as my window manager.
- The `TMX` key is programmed to `CTRL+B`, which is my tmux leader key. It activates yet another (oneshot) layer with key codes used for various tmux bindings.

#### Additional layers

There's additional not mentioned here for:

- function keys
- tmux layer
- vim layers

    I have one layer for vim navigation (shift versions of movements, line start/end, up/down screen)
    and another for text operations (select normal/line/block, yank, delete, blackhole register).
    They were really quick to get used to and because they don't require changing vim config, they work everywhere (remote editors, shell, tools, ...).

- gaming layer

    The left side of the keyboard cannot really be used in games (even with remapping bindings), since a lot of the keys are tap-hold keys and don't produce output when held.
    I call these layers `DUMB_*`, because they replace otherwise "smart" keys.

- mouse layer

    I also added mouse support, but rarely used it.
    I rarely find myself needing both keyboard and mouse at the same time.

## Custom features

The original QMK firmware quickly fell short when I was trying to create the layout I had in mind.
The main issues were:

- Missing tap-hold functionality for layering
- Limited layer count (and fixed layer "ranks")
- Awkward separation of "smart" keycodes and custom handling in `process_record_user`
- Lacking control for tap-hold keys in roll scenarios

This drove me quickly to implement my own set of features on top of the native qmk event processing (`process_record_user()`).
It supports:

- layers (as matrix position to "smart key" mappings)
- single and multi tap variants of any combination of _tap-only_, _hold-only_ & _tap-hold_ keys
- roll and sequencing detection through an event deferring system, see [below](#deferring-system)
- speculative hold for immediate modifier activation (zero latency on tap-hold modifiers)
- implementation of [magic key(s)](#magic-keys) incl. history
- repeat key
- Tapping mod keys makes them sticky (oneshot)
- Chaining one shot mod keys
- Holding a layer with oneshot keys keeps the oneshot modifier held with the layer key

    This makes things like CTRL ALT+(TAB TAB) much more comfortable.
    Or SHIFT SYS+(LEFT LEFT) for selecting text.

**Disclaimer:** This firmware does not aim to be generic and flexible enough to be used by anyone with a different layout.

## Deferring system

A key may have different behavior depending on what happens after its tap or release.
All of these are optional may be configured to accommodate keys with different nature or intent.
All of these refer to variations of the tap-hold keys.

If a second key is pressed within `TAPPING_TERM`, it may be registered as:

- Roll: if the original key is released before the latter.

    This enables the use of tap-hold keys for frequently used key sequences without tuning `TAPPING_TERM` or high chance of misfires.
    
- Hold: if the latter released first or (optionally) if a third key is pressed while the original key is still in down state.

If a key is released just before another one is pressed:

- Hold: if the time since the original key's press has not exceeded `TAPPING_TERM`

    This enables keys to be pressed in (quick) succession when they would otherwise have to be held together.
    Without this, the second alpha layer would have a much bigger impact on typing (holding vs. tapping).

## Magic keys

I have a "magic" key that performs actions based on what was previously typed.
I have two separate functions for it.
The idea is one acts on only what was typed just before:

- Transform "ue", "ae", "oe", "ss" into their german _Umlaute_ "üäöß"
- Expand abbreviations like "eg", "zB", "iferr" :), ...

and the other may look back until it finds "something":

- Close "brackets" like `({[<` or strings `"'` or the grave (or triple) char

    Matching one removes it from the history stack.
    Thus, subsequent magic presses can close entire sequences typed previously, like `"[[ "$(` with `)"]]"`.
    In an IDE scenario with auto-closing brackets, this also makes for smooth typing over closing brackets without thinking about what bracket it is.

## Smart word

This mode is like caps word on steroids.
The idea isn't new by any means.
When entering the mode, the behavior depends on the next key pressed:

- Alpha key: CAPS_WORD
- Space: pascalCase
- Delimiter (`/`, `-`, `_`): path/mode, kebap-case, snake_case
- Number: keep number layer active (great for IPs etc.)
