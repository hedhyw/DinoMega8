#include "./n5110.h"
#include "./n5110_chars.h"

struct
{
  uint8_t x;
  uint8_t page;

} cursor = {
    .x = 0,
    .page = 0};

static void write(uint8_t bytes, uint8_t is_data)
{
  register uint8_t i;

  PORT_LCD &= ~(1 << LCD_SCE);

  /* We are sending data */
  if (is_data)
    PORT_LCD |= (1 << LCD_DC);
  /* We are sending commands */
  else
    PORT_LCD &= ~(1 << LCD_DC);

  /* Send bytes */
  for (i = 0; i < 8; i++)
  {
    /* Set data pin to byte state */
    if ((bytes >> (7 - i)) & 0x01)
      PORT_LCD |= (1 << LCD_DIN);
    else
      PORT_LCD &= ~(1 << LCD_DIN);

    /* Blink clock */
    PORT_LCD |= (1 << LCD_CLK);
    PORT_LCD &= ~(1 << LCD_CLK);
  }

  PORT_LCD |= (1 << LCD_SCE);
}

static void write_cmd(uint8_t cmd)
{
  write(cmd, 0);
}

void write_data(uint8_t data)
{
  write(data, 1);
}

/*
 * Public functions
 */

void nlcd_init(void)
{
  register unsigned p, x;
  /* Set pins as output */
  DDR_LCD |= (1 << LCD_SCE);
  DDR_LCD |= (1 << LCD_RST);
  DDR_LCD |= (1 << LCD_DC);
  DDR_LCD |= (1 << LCD_DIN);
  DDR_LCD |= (1 << LCD_CLK);

  /* Reset display */
  PORT_LCD |= (1 << LCD_RST);
  PORT_LCD |= (1 << LCD_SCE);
  _delay_ms(10);
  PORT_LCD &= ~(1 << LCD_RST);
  _delay_ms(70);
  PORT_LCD |= (1 << LCD_RST);

  /*
   * Initialize display
   */
  /* Enable controller */
  PORT_LCD &= ~(1 << LCD_SCE);
  /* -LCD Extended Commands mode- */
  write_cmd(0x21);
  /* LCD bias mode 1:48 */
  write_cmd(0x13);
  /* Set temperature coefficient */
  write_cmd(0x06);
  /* Default VOP (3.06 + 66 * 0.06 = 7V) */
  write_cmd(0xC2);
  /* Standard Commands mode, powered down */
  write_cmd(0x20);
  /* LCD in normal mode. VLCD temperature coefficient 3 */
  write_cmd(0xB);

  /* Clear LCD RAM */
  write_cmd(0x80);
  write_cmd(LCD_CONTRAST);
  for (p = 0; p < 6; p++)
  {
    for (x = 0; x < 84; x++)
    {
      write_data(0x00);
    }
  }

  /* Activate LCD */
  write_cmd(0x08);
  write_cmd(0x0C);

  PORT_LCD |= (1 << LCD_SCE);
}

void nlcd_update_cursor()
{
  write_cmd(0x80 + cursor.x);
  write_cmd(0x40 + cursor.page);
}

void nlcd_set_cursor(uint8_t x, uint8_t page)
{
  cursor.x = x % 84;
  cursor.page = page % 6;
  nlcd_update_cursor();
}

void nlcd_clear(void)
{
  register unsigned p, x;
  nlcd_set_cursor(0, 0);

  for (p = 0; p < 6; p++)
  {
    for (x = 0; x < 84; x++)
    {
      write_data(0x00);
    }
  }
}

void nlcd_write_char(char code)
{
  register uint8_t x;
  if (code < ' ' || code > '~')
    code = '?';
  if (cursor.x + 6 >= 84)
  {
    cursor.x = 0;
    cursor.page++;
  }
  if (cursor.page > 6)
    cursor.page = 0;
  nlcd_update_cursor();

  for (x = 0; x < 5; x++)
    write_data(pgm_read_byte(&CHARSET[code - ' '][x]));

  cursor.x += 6;
}

void nlcd_write_string(char *str)
{
  while (*str != '\0')
  {
    nlcd_write_char(*str++);
  }
}
