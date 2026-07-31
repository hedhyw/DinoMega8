/* Host-side tests for the hardware-independent game logic in dino/dino.c.
 *
 * The dino module only talks to the display through nlcd_set_cursor() and
 * write_data(), which are stubbed out below, so the whole game simulation
 * (physics, obstacle movement, collision detection, scoring) runs natively.
 *
 * Build and run: make test
 */
#include <stdio.h>
#include <stdint.h>

#include "../dino/dino.h"

/* --- LCD stubs -------------------------------------------------------- */

static unsigned long stub_cursor_calls;
static unsigned long stub_data_calls;

void nlcd_set_cursor(uint8_t x, uint8_t page)
{
  (void)x;
  (void)page;
  stub_cursor_calls++;
}

void write_data(uint8_t data)
{
  (void)data;
  stub_data_calls++;
}

/* --- Tiny test harness ------------------------------------------------ */

static int failures;

#define CHECK(cond)                                                     \
  do                                                                    \
  {                                                                     \
    if (cond)                                                           \
    {                                                                   \
      printf("ok   %s\n", #cond);                                       \
    }                                                                   \
    else                                                                \
    {                                                                   \
      printf("FAIL %s (%s:%d)\n", #cond, __FILE__, __LINE__);           \
      failures++;                                                       \
    }                                                                   \
  } while (0)

/* Run the game without jumping; return how many ticks the dino survives
 * before colliding (tick() == 0), capped at `limit`. */
static unsigned long run_without_jumping(unsigned long limit)
{
  unsigned long t;
  dino_init();
  for (t = 0; t < limit; t++)
  {
    if (tick() == 0)
      return t;
  }
  return limit;
}

/* Run the game jumping on every tick (dino_jump is a no-op while
 * airborne); return survived ticks, capped at `limit`. */
static unsigned long run_jumping(unsigned long limit)
{
  unsigned long t;
  dino_init();
  for (t = 0; t < limit; t++)
  {
    dino_jump();
    if (tick() == 0)
      return t;
  }
  return limit;
}

int main(void)
{
  const unsigned long limit = 1000;
  unsigned long survived_idle;
  unsigned long survived_jumping;
  uint16_t score_after_one_tick;
  uint16_t score_later;

  /* A fresh game starts with score 0 and no immediate collision. */
  dino_init();
  CHECK(dino_get_score() == 0);
  CHECK(tick() == 1);
  score_after_one_tick = dino_get_score();

  /* Score grows as the game advances. */
  tick();
  tick();
  tick();
  score_later = dino_get_score();
  CHECK(score_later >= score_after_one_tick);
  CHECK(dino_get_score() > 0);

  /* Ticks actually draw to the display through the stubs. */
  CHECK(stub_cursor_calls > 0);
  CHECK(stub_data_calls > 0);

  /* Never jumping ends the game: the first barrier hits the dino. */
  survived_idle = run_without_jumping(limit);
  CHECK(survived_idle < limit);

  /* Jumping lets the dino clear barriers and survive longer. */
  survived_jumping = run_jumping(limit);
  CHECK(survived_jumping > survived_idle);

  /* Re-init resets the score after a crashed game. */
  dino_init();
  CHECK(dino_get_score() == 0);

  if (failures)
  {
    printf("%d test(s) FAILED\n", failures);
    return 1;
  }
  printf("all tests passed (idle survived %lu ticks, jumping %lu)\n",
         survived_idle, survived_jumping);
  return 0;
}
