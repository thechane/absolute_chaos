#ifndef PLATFORM_ZX_H
#define PLATFORM_ZX_H

#include <stdint.h>
#include <stddef.h>
#include "types.h"

// Classic ULA helpers (work on 128K & Next personalities)
void zx_cls_attr(uint8_t attr);
void zx_cls_pixels(void);
void zx_set_attr_cell(uint8_t col, uint8_t row, uint8_t attr);
void zx_fill_attr_rect(uint8_t col, uint8_t row, uint8_t w, uint8_t h, uint8_t attr);
void zx_out_border(uint8_t color); // HW border (port 0xFE), optional

// 8x8 blit; src = 8 bytes, MSB=left pixel
void zx_blit8(uint8_t x, uint8_t y, const uint8_t *src);

// Safe variants for 8-pixel alignment (x must be multiple of 8)
void zx_blit8_aligned(uint8_t x, uint8_t y, const uint8_t *src);

// Text writer (uses 8x8 font table you provide)
void zx_putc8(uint8_t char_col, uint8_t char_row, char c, const uint8_t *font8x8);
void zx_puts8(uint8_t char_col, uint8_t char_row, const char *s, const uint8_t *font8x8);

// Address helpers
uint8_t *zx_pixaddr(uint8_t x, uint8_t y);
uint8_t *zx_attraddr(uint8_t col, uint8_t row);

#endif
