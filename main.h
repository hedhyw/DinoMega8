#ifndef __MAIN_H__
#define __MAIN_H__

#include "./freq.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <avr/eeprom.h>
#include "./n5110/n5110.h"
#include "./dino/dino.h"

#define FRAME_SLEEP 50

#define UP_BTN_DDR DDRD
#define UP_BTN_PORT PORTD
#define UP_BTN_PIN PIND
#define UP_BTN_BIT PD1

#define LIGHT_BTN_DDR DDRD
#define LIGHT_BTN_PORT PORTD
#define LIGHT_BTN_PIN PIND
#define LIGHT_BTN_BIT PD7

#define LIGHT_LED_DDR DDRB
#define LIGHT_LED_PORT PORTB
#define LIGHT_LED_BIT PB0

#define UP_BTN_PRESSED() ((UP_BTN_PIN & (1 << UP_BTN_BIT)) == 0)
#define LIGHT_BTN_PRESSED() ((LIGHT_BTN_PIN & (1 << LIGHT_BTN_BIT)) == 0)
#define LIGHT_LED_ON() (LIGHT_LED_PORT &= ~(1 << LIGHT_LED_BIT))
#define LIGHT_LED_OFF() (LIGHT_LED_PORT |= 1 << LIGHT_LED_BIT)

#endif