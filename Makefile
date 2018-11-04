MMCU=atmega8
CC=./main.c ./dino/dino.c ./n5110/n5110.c
OBJ=./main.elf

DIST=./dist
HEX=$(DIST)/out.hex
EEPROM=$(DIST)/eep.hex

all: build clean
build:
	avr-gcc -Wall -std=gnu99 -mmcu=$(MMCU) -Os $(CC) -L. -o $(OBJ)
	avr-objcopy -j .text -j .data -O ihex $(OBJ) $(HEX)
	avr-objcopy -j .eeprom --change-section-lma .eeprom=0 -O ihex $(OBJ) $(EEPROM)
upload:
	avrdude -p $(MMCU) -c usbasp -U flash:w:$(HEX) -U eeprom:w:$(EEPROM)
clean:
	rm -f $(OBJ)