#ifndef PLATFORM_ZX_H
#define PLATFORM_ZX_H

#include <stdint.h>
#include <stddef.h>
#include "types.h"

void zx_cls_attr(uint8_t attr);
void zx_cls_pixels(void);
void zx_set_attr_cell(uint8_t col, uint8_t row, uint8_t attr);
void zx_fill_attr_rect(uint8_t col, uint8_t row, uint8_t w, uint8_t h, uint8_t attr);
void zx_out_border(uint8_t color); // HW border (port 0xFE), optional

void zx_blit8(uint8_t x, uint8_t y, const uint8_t *src);

void zx_blit8_aligned(uint8_t x, uint8_t y, const uint8_t *src);

uint8_t *zx_pixaddr(uint8_t x, uint8_t y);
uint8_t *zx_attraddr(uint8_t col, uint8_t row);

#endif // PLATFORM_ZX_H