# Dino game
![Dino](.assets/gameplay.gif)

Firmware for **Atmega8A** microcontroller: the Chrome-style Dino runner game
on a Nokia 5110 (PCD8544) 84x48 LCD. The high score is stored in EEPROM.

# Build

Requires `gcc-avr` and `avr-libc`:

```sh
sudo apt-get install -y gcc-avr avr-libc
```

1. Set up your frequency in **freq.h**, now it's **10 MHz**
2. Build project: `make` (produces `dist/out.hex` and `dist/eep.hex`,
   prints a size report)
3. Upload firmware: `make upload` (avrdude with a USBasp programmer)
4. Configure external crystal oscillator with updating fuse bytes

# Tests

Host-side unit tests for the game logic (no AVR toolchain required):

```sh
make test
```

# Emulator

Play the game on your computer, no AVR toolchain and no hardware required:

```sh
make sim
```

It renders the 84x48 Nokia 5110 screen in the terminal. Press **space** to
jump and **q** to quit.

The game logic in **dino/dino.c** talks to the display only through
`nlcd_set_cursor()` and `write_data()`, so **sim/sim.c** implements those two
functions on top of an in-memory framebuffer. The start and game over screens
from **main.c** are not emulated, `make sim` goes straight into the game.

# Notes
## Controls
- **PD1** button — jump / start game (hold on the start screen to reset the
  high score)
- **PD7** button — toggle LCD backlight (LED on **PB0**)
## img_maker
If you want to generate custom hero or barrier use **./img_maker**
1. Install dependencies:
- [Pillow](https://pillow.readthedocs.io/en/latest/installation.html)
- [Python3](https://www.python.org/downloads/)
2. Run
`python3 ./img_maker/index.py YOUR_IMAGE.png`
3. Past generated code to **dino/dino.c** DINO_PIC or BARRIER_PIC
# Board

![Board](.assets/board.png)

# Schematic

![Schema](.assets/sch.png)
