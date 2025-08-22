#ifndef TEXT_H
#define TEXT_H
#include <stdint.h>
#include <stdbool.h>
#include "types.h"

// Provide a pointer to your font table (96 * 8 bytes)
extern const uint8_t *TEXT_FONT8;

void text_set_colors(uint8_t ink, uint8_t paper, bool bright);
void text_status(const char *s);     // prints on bottom status line
void text_at(uint8_t char_col, uint8_t char_row, const char *s); // anywhere (char coords)
void text_at_pixels(uint8_t x, uint8_t y, const char *s);        // inside border by pixel row aligned to 8

#endif