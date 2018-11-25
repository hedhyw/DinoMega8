#ifndef __NOKIA_5110_H__
#define __NOKIA_5110_H__

#include "../freq.h"
#include <stdint.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>

#define PORT_LCD PORTB
#define DDR_LCD DDRB
#define LCD_SCE PB4
#define LCD_RST PB5
#define LCD_DC PB3
#define LCD_DIN PB2
#define LCD_CLK PB1

#define LCD_CONTRAST 0x40

void nlcd_init(void);
void nlcd_clear(void);
void nlcd_write_char(char code);
void nlcd_write_string(char *str);
void nlcd_update_cursor();
void nlcd_set_cursor(uint8_t x, uint8_t page);
void nlcd_draw_dino();
void write_data(uint8_t data);

#endif
