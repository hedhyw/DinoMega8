#ifndef __NOKIA_5110_H__
#define __NOKIA_5110_H__

#include <stdint.h>

#define F_CPU 1000000

#define PORT_LCD PORTB
#define DDR_LCD DDRB
#define LCD_SCE PB2
#define LCD_RST PB4
#define LCD_DC PB1
#define LCD_DIN PB3
#define LCD_CLK PB5

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
