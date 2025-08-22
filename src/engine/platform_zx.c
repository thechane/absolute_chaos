#include "platform_zx.h"
#include "config.h"
#include <stdint.h>

// --- Address translation (screen layout) ---
uint8_t *zx_pixaddr(uint8_t x, uint8_t y) {
  // x is 0..255 (pixel), we address bytes (8 pixels wide)
  uint16_t row_block = (y & 0xC0) << 5;      // y bits 6-7
  uint16_t row_line  = (y & 0x07) << 8;      // y bits 0-2
  uint16_t row_chunk = (y & 0x38) << 2;      // y bits 3-5
  uint16_t byte_x    = (x & 0xF8) >> 3;      // /8
  return ZX_PIX_BASE + row_block + row_line + row_chunk + byte_x;
}

uint8_t *zx_attraddr(uint8_t col, uint8_t row) {
  return ZX_ATTR_BASE + (row * ZX_ATTR_COLS) + col;
}

// --- CLS ---
void zx_cls_pixels(void) {
  uint8_t *p = (uint8_t*)ZX_PIX_BASE;
  uint16_t i;
  for (i=0; i<6144; ++i) *p++ = 0x00;
}
void zx_cls_attr(uint8_t attr) {
  uint8_t *a = (uint8_t*)ZX_ATTR_BASE;
  uint16_t i;
  for (i=0; i<768; ++i) *a++ = attr;
}

// --- Attr operations ---
void zx_set_attr_cell(uint8_t col, uint8_t row, uint8_t attr) {
  *zx_attraddr(col, row) = attr;
}
void zx_fill_attr_rect(uint8_t col, uint8_t row, uint8_t w, uint8_t h, uint8_t attr) {
  uint8_t r;
  for (r=0; r<h; ++r) {
    uint8_t *a = zx_attraddr(col, row+r);
    uint8_t c;
    for (c=0; c<w; ++c) a[c] = attr;
  }
}

// --- HW border (optional aesthetic match) ---
void zx_out_border(uint8_t color) {
  // Port 0xFE : bits 0..2 border color
  #ifdef __Z88DK__
    __asm
      ld a, l
      and 0x07
      ld bc, 0x00FE
      out (c), a
    __endasm;
  #else
    (void)color; /* suppress unused warning for non-z88dk builds */
  #endif
}

// --- Blits (8x8) ---
void zx_blit8_aligned(uint8_t x, uint8_t y, const uint8_t *src) {
  uint8_t *dst = zx_pixaddr(x, y);
  uint8_t i;
  for (i=0; i<8; ++i) {
    *dst = src[i];
    dst += 256/8; // next scanline (32 bytes)
  }
}
void zx_blit8(uint8_t x, uint8_t y, const uint8_t *src) {
  // Require x%8==0 for simplicity (tiles / font / border use multiples of 8).
  zx_blit8_aligned(x, y, src);
}

// --- 8x8 Text using user font table (96 printable ASCII) ---
void zx_putc8(uint8_t char_col, uint8_t char_row, char c, const uint8_t *font8x8) {
  if (c < 32 || c > 127) c = 32;
  const uint8_t *glyph = font8x8 + (uint16_t)(c-32)*8;
  zx_blit8_aligned(char_col*8, char_row*8, glyph);
}
void zx_puts8(uint8_t char_col, uint8_t char_row, const char *s, const uint8_t *font8x8) {
  uint8_t x = char_col;
  while (*s) {
    zx_putc8(x++, char_row, *s++, font8x8);
  }
}