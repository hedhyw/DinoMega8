#ifndef __MAIN_H__
#define __MAIN_H__

#define F_CPU 1000000

#define FRAME_SLEEP 50

#define UP_BTN_DDR DDRC
#define UP_BTN_PORT PORTC
#define UP_BTN_PIN PC5

#define LIGHT_BTN_DDR DDRC
#define LIGHT_BTN_PORT PORTC
#define LIGHT_BTN_PIN PC4

#define LIGHT_LED_DDR DDRB
#define LIGHT_LED_PORT PORTB
#define LIGHT_LED_PIN PB0

#define UP_BTN_PRESSED() (UP_BTN_PORT & (1 << UP_BTN_PIN))
#define LIGHT_BTN_PRESSED() (LIGHT_BTN_PORT & (1 << LIGHT_BTN_PIN))
#define LIGHT_LED_ON() (LIGHT_LED_PORT |= 1 << LIGHT_LED_PIN)
#define LIGHT_LED_OFF() (LIGHT_LED_PORT &= ~(1 << LIGHT_LED_PIN))

#endif