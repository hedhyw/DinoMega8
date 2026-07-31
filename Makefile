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

upload: $(HEX) $(EEPROM)
	$(AVRDUDE) -p $(MCU) -c usbasp -U flash:w:$(HEX):i -U eeprom:w:$(EEPROM):i

clean:
	rm -f $(ELF) $(HEX) $(EEPROM)

.PHONY: all build size upload clean
