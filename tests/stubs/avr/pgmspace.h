/* Host-side stub of <avr/pgmspace.h> for unit tests.
 * On the host, program memory is just regular memory. */
#ifndef TEST_STUB_AVR_PGMSPACE_H
#define TEST_STUB_AVR_PGMSPACE_H

#include <stdint.h>

#define PROGMEM
#define pgm_read_byte(addr) (*(const uint8_t *)(addr))

#endif
