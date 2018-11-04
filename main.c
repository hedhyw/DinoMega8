// Atmega8
#include "./main.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <avr/eeprom.h>
#include "./n5110/n5110.h"
#include "./dino.h"

uint16_t max_score_ee EEMEM = 0;

char buf[5];      // for formating int
int8_t pressed;   // UP Button
int8_t state = 0; // screen
uint16_t max_score;
uint16_t score;

void init()
{
  UP_BTN_DDR = UP_BTN_DDR & ~(1 << UP_BTN_PIN); // IN-mode
  UP_BTN_PORT = 1 << UP_BTN_PIN;                // VCC pull-up

  LIGHT_BTN_DDR = LIGHT_BTN_DDR & ~(1 << LIGHT_BTN_PIN); // IN-mode
  LIGHT_BTN_PORT = 1 << LIGHT_BTN_PIN;                   // VCC pull-up

  LIGHT_LED_DDR |= 1 << LIGHT_LED_PIN; // OUT-mode

  max_score = eeprom_read_word(&max_score_ee);

  nlcd_init();
  nlcd_clear();
}

void save_score()
{
  eeprom_write_word(&max_score_ee, score);
}

inline void draw_score(uint8_t pos_x, uint8_t pos_y)
{
  nlcd_set_cursor(pos_x, pos_y);
  score = dino_get_score();
  sprintf(buf, "%d", score);
  nlcd_write_string(buf);
}

void end_game_screen()
{
  nlcd_set_cursor(17, 2);
  nlcd_write_string("Game over");

  if (max_score < score)
  {
    max_score = score;
    save_score();
  }
  draw_score(27, 3);

  if (pressed)
  {
    state = 0;
    nlcd_clear();
  }
}

void game_screen()
{
  if (tick() == 0) // draw frame
  {                // crashed. game end
    _delay_ms(200);
    state = 2;
    nlcd_clear();
    return;
  }

  draw_score(27, 2);

  if (pressed)
  {
    dino_jump();
  }
}

void start_screen()
{
  nlcd_set_cursor(27, 1);
  nlcd_write_string("start");

  nlcd_set_cursor(27, 2);
  sprintf(buf, "%d", max_score);
  nlcd_write_string(buf);

  nlcd_set_cursor(24, 3);
  nlcd_write_string("Krivchun M.");

  nlcd_set_cursor(27, 4);
  nlcd_write_string("ENE-151");

  nlcd_set_cursor(27, 5);
  nlcd_write_string("2018");

  if (pressed)
  {
    state = 1;
    nlcd_clear();

    dino_init();

    _delay_ms(600);
    while (UP_BTN_PRESSED())
    { // reset score on long up-button press
      max_score = 0;
      state = 0;
    }
    save_score();

    _delay_ms(200);
  }
}

int main()
{
  register uint8_t light = 0; // off by default

  _delay_ms(100);
  init();

  while (1)
  {
    if (LIGHT_BTN_PRESSED())
    {
      light ^= 1;
    }
    if (light)
    {
      LIGHT_LED_ON();
    }
    else
    {
      LIGHT_LED_OFF();
    }

    pressed = UP_BTN_PRESSED();
    switch (state)
    {
    case 0:
      start_screen();
      break;
    case 1:
      game_screen();
      break;
    case 2:
      end_game_screen();
      break;
    }
    _delay_ms(FRAME_SLEEP);
  }
}