# DinoMega8

Chrome-style "Dino" runner game firmware for the ATmega8 microcontroller,
rendered on a Nokia 5110 (PCD8544) 84x48 LCD. Written in C (avr-gcc, gnu99).

## What it does

A small state machine in `main.c` cycles through three screens: start screen
(shows the high score), game screen (dino physics, obstacle movement,
collision detection, live score), and game-over screen. The high score is
persisted in the on-chip EEPROM. Holding the jump button on the start screen
resets the high score. A second button toggles the LCD backlight LED.

## Hardware target

- MCU: ATmega8 / ATmega8A
- Clock: 10 MHz external crystal (`F_CPU` in `freq.h`); fuse bytes must be
  set for an external crystal oscillator (exact fuse values are not
  documented in this repo — derive them for your crystal)
- Display: Nokia 5110 LCD, bit-banged serial on PORTB:
  - SCE (chip enable): PB4
  - RST (reset): PB5
  - D/C (data/command): PB3
  - DIN (data in): PB2
  - CLK (clock): PB1
- Jump/UP button: PD1, active low, internal pull-up
- Backlight toggle button: PD7, active low, internal pull-up
- Backlight LED: PB0, active low
- Schematic and board photos: `.assets/sch.png`, `.assets/board.png`

## Build

Requires `gcc-avr` and `avr-libc` (Debian/Ubuntu: `sudo apt-get install -y
gcc-avr avr-libc`).

- `make` — build `main.elf`, produce `dist/out.hex` (flash) and
  `dist/eep.hex` (EEPROM), print the section size report (`avr-size`)
- `make test` — build and run host-side unit tests with the native `gcc`
  (no AVR toolchain needed)
- `make clean` — remove build outputs

## Flash

With a USBasp programmer:

```sh
make upload
# which runs:
# avrdude -p atmega8 -c usbasp -U flash:w:dist/out.hex:i -U eeprom:w:dist/eep.hex:i
```

Adjust `-c` for a different programmer.

## Tests

`tests/test_dino.c` exercises the hardware-independent game logic in
`dino/dino.c` (scoring, obstacle progression, collision detection) on the
host. `tests/stubs/` contains minimal stand-ins for `<avr/io.h>`,
`<avr/pgmspace.h>` and `<util/delay.h>`; the LCD calls (`nlcd_set_cursor`,
`write_data`) are stubbed in the test itself. CI runs `make test` and the
firmware build on every push/PR (`.github/workflows/check.yml`).

## Layout

- `main.c`, `main.h` — application state machine, buttons, backlight, EEPROM
- `freq.h` — `F_CPU` definition (10 MHz)
- `dino/` — game logic and sprites (hardware-independent, unit tested)
- `n5110/` — Nokia 5110 LCD driver (bit-banged, plus 5x7 font)
- `tests/` — host-side tests and AVR header stubs
- `img_maker/` — Python 3 + Pillow tool that converts PNG sprites to C arrays
  (`python3 img_maker/index.py YOUR_IMAGE.png`)
- `dist/` — build outputs (gitignored)

## Conventions

- Keep the firmware conservative: no refactors of hardware-touching code;
  behavior on the device must stay identical unless a change is the explicit
  goal.
- Build flags: `-Wall -Wextra -std=gnu99 -Os -mmcu=atmega8`.
- Conventional commit messages.
