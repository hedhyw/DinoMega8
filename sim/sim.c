/* Host-side "emulator" for DinoMega8.
 *
 * dino/dino.c only touches the LCD through nlcd_set_cursor()/write_data(),
 * so we back those with a PCD8544-shaped framebuffer (84x48, 6 pages) and
 * draw it to the terminal. No AVR toolchain, no hardware.
 *
 * Build: gcc -Wall -std=gnu99 -I tests/stubs dino/dino.c sim/sim.c -o dist/sim
 * Run:   make sim       (space = jump, q = quit)
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include "../dino/dino.h"

#define LCD_W 84
#define LCD_PAGES 6

static uint8_t fb[LCD_PAGES][LCD_W];
static uint8_t cur_x, cur_page;

void nlcd_set_cursor(uint8_t x, uint8_t page)
{
  cur_x = x;
  cur_page = page;
}

void write_data(uint8_t data)
{
  if (cur_page < LCD_PAGES && cur_x < LCD_W)
    fb[cur_page][cur_x] = data;
  cur_x++;
}

static int pixel(int x, int y)
{
  return (fb[y / 8][x] >> (y % 8)) & 1;
}

static void draw(uint16_t score)
{
  printf("\033[H");
  for (int y = 0; y < LCD_PAGES * 8; y += 2)
  {
    for (int x = 0; x < LCD_W; x++)
    {
      int top = pixel(x, y), bot = pixel(x, y + 1);
      fputs(top && bot ? "█" : top ? "▀" : bot ? "▄" : " ", stdout);
    }
    putchar('\n');
  }
  printf("score: %u   [space] jump  [q] quit\033[K\n", score);
  fflush(stdout);
}

static struct termios saved_tio;

static void restore(void)
{
  tcsetattr(STDIN_FILENO, TCSANOW, &saved_tio);
  printf("\033[?25h\n");
}

int main(void)
{
  struct termios tio;
  tcgetattr(STDIN_FILENO, &saved_tio);
  tio = saved_tio;
  tio.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &tio);
  fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
  atexit(restore);
  printf("\033[2J\033[?25l");

  for (;;)
  {
    memset(fb, 0, sizeof(fb));
    dino_init();

    for (;;)
    {
      char c;
      int jump = 0, quit = 0, n;
      while ((n = read(STDIN_FILENO, &c, 1)) != 0)
      {
        if (n < 0) break; /* nothing buffered */
        if (c == ' ') jump = 1;
        if (c == 'q') quit = 1;
      }
      if (quit || n == 0) return 0; /* q pressed or stdin closed */
      if (jump) dino_jump();

      if (tick() == 0) break; /* crashed */
      draw(dino_get_score());
      usleep(60000);
    }

    printf("GAME OVER - score %u. [space] retry, [q] quit\033[K\n", dino_get_score());
    fflush(stdout);
    for (;;)
    {
      char c;
      int n = read(STDIN_FILENO, &c, 1);
      if (n == 0) return 0; /* stdin closed */
      if (n == 1)
      {
        if (c == 'q') return 0;
        if (c == ' ') break;
      }
      usleep(30000);
    }
    printf("\033[2J");
  }
}
