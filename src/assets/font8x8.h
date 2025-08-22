#ifndef FONT8X8_H
#define FONT8X8_H
#include <stdint.h>

/* 8x8 mono font, ASCII 32..127 (96 glyphs). MSB = leftmost pixel.
   A 1-bit draws ink; a 0-bit leaves paper. */
extern const uint8_t FONT8X8_ZXLIKE[96 * 8];

#endif