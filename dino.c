#include "./dino.h"

struct
{
  int8_t y;
  uint16_t x;
  int8_t speed_x;
  int8_t max_speed_x;
  int8_t speed_y;
  int8_t acceleration_y;
  int8_t acceleration_x;
  uint8_t level;
} dino;

struct
{
  int8_t x;
} barrier;

const uint8_t DINO_PIC[][DINO_WIDTH] PROGMEM = {
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xe0, 0xf0, 0xf8, 0xd8, 0xf8, 0xf8, 0xf8, 0xf0, 0xe0},
    {0x78, 0x60, 0xc0, 0xc0, 0xe0, 0xf0, 0xf0, 0xff, 0xff, 0xff, 0x13, 0x32, 0x2, 0x2, 0x0, 0x0},
    {0x0, 0x0, 0x1, 0x1f, 0x17, 0x7, 0x1f, 0x13, 0x3, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};

const uint8_t BARRIER_PIC[][BARRIER_WIDTH] PROGMEM = {
    {0xf0, 0x0, 0x80, 0xf0, 0xf0, 0x0, 0x0},
    {0x7, 0x4, 0xff, 0xff, 0xff, 0x10, 0x10}};

void dino_init()
{
  dino.y = 0;
  dino.x = 0;
  dino.speed_y = 0;
  dino.speed_x = 4;
  dino.max_speed_x = 8;
  dino.acceleration_y = -1;
  dino.acceleration_x = +1;
  dino.level = 0;
  barrier.x = 50;
}

void dino_jump()
{
  if (dino.y == 0)
    dino.speed_y = 7;
}

void draw_barrier(int8_t clear)
{
  register uint8_t p, x;
  for (p = 0; p < BARRIER_PAGES; p++)
  {
    nlcd_set_cursor(barrier.x, 6 - BARRIER_PAGES + p);
    for (x = 0; x < BARRIER_WIDTH; x++)
    {
      if (barrier.x + x >= 84)
        break;
      if (clear == 1)
        write_data(0);
      else
        write_data(pgm_read_byte(&BARRIER_PIC[p][x]));
    }
  }
}

void draw_dino()
{
  register uint8_t p, x, y, page, byte, last_byte, delta;
  y = 48 - DINO_PAGES * 8 - dino.y;
  page = y / 8;
  delta = y % 8;
  last_byte = 0;
  for (p = 0; p < page; p++)
  {
    nlcd_set_cursor(0, p);
    for (x = 0; x < DINO_WIDTH; x++)
    {
      write_data(0);
    }
  }
  for (p = page + DINO_PAGES; p < 6; p++)
  {
    nlcd_set_cursor(0, p);
    for (x = 0; x < DINO_WIDTH; x++)
    {
      write_data(0);
    }
  }
  for (p = 0; p < DINO_PAGES; p++)
  {
    nlcd_set_cursor(0, page + p);
    for (x = 0; x < DINO_WIDTH; x++)
    {
      byte = pgm_read_byte(&DINO_PIC[p][x]);
      last_byte = (p == 0 ? 0 : pgm_read_byte(&DINO_PIC[p - 1][x]));
      write_data((byte << delta) | (last_byte >> (7 - delta)));
    }
  }
  if (delta != 0)
  {
    nlcd_set_cursor(0, page + DINO_PAGES);
    for (x = 0; x < DINO_WIDTH; x++)
    {
      last_byte = pgm_read_byte(&DINO_PIC[p - 1][x]);
      write_data(last_byte >> (7 - delta));
    }
  }
}

int8_t barrier_tick()
{
  draw_barrier(1);
  barrier.x -= dino.speed_x;
  if (barrier.x <= 0)
    barrier.x = 84;
  draw_barrier(0);

  if (dino.level++ == 0xFF)
  {
    dino.speed_x += dino.acceleration_x;
    if (dino.speed_x > dino.max_speed_x)
      dino.speed_x = dino.max_speed_x;
  }

  if (barrier.x + 3 < DINO_WIDTH)
  {
    if (BARRIER_HEIGHT > dino.y)
    {
      return 0;
    }
  }
  return 1;
}

uint16_t dino_get_score()
{
  return dino.x / 10;
}

void dino_tick()
{
  dino.speed_y += dino.acceleration_y;
  dino.y += dino.speed_y;
  dino.x += dino.speed_x;
  if (dino.y < 0)
  {
    dino.y = 0;
    dino.speed_y = 0;
  }
  draw_dino();
}

int8_t tick()
{
  dino_tick();
  return barrier_tick();
}
