MCU = atmega8
SRC = main.c dino/dino.c n5110/n5110.c

DIST = dist
ELF = main.elf
HEX = $(DIST)/out.hex
EEPROM = $(DIST)/eep.hex

CC = avr-gcc
OBJCOPY = avr-objcopy
SIZE = avr-size
AVRDUDE = avrdude

CFLAGS = -Wall -Wextra -std=gnu99 -mmcu=$(MCU) -Os

HOSTCC = gcc
HOST_CFLAGS = -Wall -Wextra -std=gnu99 -O2 -I tests/stubs
TEST_BIN = $(DIST)/test_dino
SIM_BIN = $(DIST)/sim

all: build size

build: $(HEX) $(EEPROM)

$(ELF): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $@

$(HEX): $(ELF) | $(DIST)
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

$(EEPROM): $(ELF) | $(DIST)
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@

$(DIST):
	mkdir -p $(DIST)

size: $(ELF)
	$(SIZE) $(ELF)

test: | $(DIST)
	$(HOSTCC) $(HOST_CFLAGS) dino/dino.c tests/test_dino.c -o $(TEST_BIN)
	$(TEST_BIN)

$(SIM_BIN): sim/sim.c dino/dino.c | $(DIST)
	$(HOSTCC) $(HOST_CFLAGS) dino/dino.c sim/sim.c -o $@

sim: $(SIM_BIN)
	$(SIM_BIN)

upload: $(HEX) $(EEPROM)
	$(AVRDUDE) -p $(MCU) -c usbasp -U flash:w:$(HEX):i -U eeprom:w:$(EEPROM):i

clean:
	rm -f $(ELF) $(HEX) $(EEPROM) $(TEST_BIN) $(SIM_BIN)

.PHONY: all build size test sim upload clean
