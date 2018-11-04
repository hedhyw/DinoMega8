#ifndef __DINO_H__
#define __DINO_H__

#include <avr/pgmspace.h>
#include "./n5110/n5110.h"

#define DINO_WIDTH 16
#define DINO_PAGES 3

#define BARRIER_WIDTH 7
#define BARRIER_HEIGHT 12
#define BARRIER_PAGES 2

void dino_jump();
int8_t tick();
uint16_t dino_get_score();
void dino_init();

#endif
