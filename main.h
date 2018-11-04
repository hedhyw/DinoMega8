#ifndef __MAIN_H__
#define __MAIN_H__

#define F_CPU 1000000

#define FRAME_SLEEP 50
#define UP_BTN_DDR DDRC
#define UP_BTN_PORT PORTC
#define UP_BTN_PIN PC5

#define UP_BTN_PRESSED() (UP_BTN_PORT & (1 << UP_BTN_PIN))

#endif