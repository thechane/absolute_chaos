#include "platform_zx.h"
#include "config.h"
#include "engine/audio.h"
#include <stdint.h>

/* =========================================================================
   ZX platform helpers (ULA bitmap + attribute map)
   -------------------------------------------------------------------------
   - Pixel bitmap: 256x192 at ZX_PIX_BASE
   - Attribute map: 32x24 cells at ZX_ATTR_BASE
   - Address mapping follows the Spectrum's split-row layout.
   ========================================================================= */

/* -------------------------------------------------------------------------
   zx_pixaddr
   Convert pixel coordinates (x,y) to a pointer into the ULA bitmap.
   The Spectrum stores scanlines in a non-linear fashion:
     byte address = 0x4000
                   + (y7..y6) << 11   ; 0..2 blocks of 64 rows
                   + (y2..y0) << 8    ; line within 8-row chunk
                   + (y5..y3) << 5    ; chunk within 64-row block
                   + (x >> 3)         ; byte column
   Params: x in 0..255, y in 0..191
   Returns: pointer to the byte that contains pixel x on line y.
   ------------------------------------------------------------------------- */
uint8_t *zx_pixaddr(uint8_t x, uint8_t y)
{
    uint16_t row_block = (uint16_t)((y & 0xC0u) << 5);  /* y bits 6-7 -> <<11 */
    uint16_t row_line  = (uint16_t)((y & 0x07u) << 8);  /* y bits 0-2 -> <<8  */
    uint16_t row_chunk = (uint16_t)((y & 0x38u) << 2);  /* y bits 3-5 -> <<5  */
    uint16_t byte_x    = (uint16_t)((x & 0xF8u) >> 3);  /* x / 8 (byte col)   */
    return ZX_PIX_BASE + (uint16_t)(row_block + row_line + row_chunk + byte_x);
}

/* -------------------------------------------------------------------------
   zx_attraddr
   Convert attribute cell coordinates (col,row) to a pointer in the attribute
   map.  Attribute cells are 8x8 pixels; the table is a simple 32x24 array.
   Params: col in 0..31, row in 0..23
   Returns: pointer to attribute byte.
   ------------------------------------------------------------------------- */
uint8_t *zx_attraddr(uint8_t col, uint8_t row)
{
    uint16_t off = (uint16_t)row * (uint16_t)ZX_ATTR_COLS + (uint16_t)col;
    return ZX_ATTR_BASE + off;
}

/* -------------------------------------------------------------------------
   zx_cls_pixels
   Clear the entire pixel bitmap to 0 (black). 6144 bytes.
   ------------------------------------------------------------------------- */
void zx_cls_pixels(void)
{
    uint8_t *p = ZX_PIX_BASE;
    uint16_t i;
    for (i = 0; i < 6144u; ++i) {
        *p++ = 0x00;
    }
}

/* -------------------------------------------------------------------------
   zx_cls_attr
   Clear (or set) the entire attribute map (768 bytes) to 'attr'.
   Use MAKE_ATTR(...) macros to build the attribute value.
   ------------------------------------------------------------------------- */
void zx_cls_attr(uint8_t attr)
{
    uint8_t *a = ZX_ATTR_BASE;
    uint16_t i;
    for (i = 0; i < 768u; ++i) {
        *a++ = attr;
    }
}

/* -------------------------------------------------------------------------
   zx_set_attr_cell
   Set a single attribute cell at (col,row) to 'attr'.
   ------------------------------------------------------------------------- */
void zx_set_attr_cell(uint8_t col, uint8_t row, uint8_t attr)
{
    *zx_attraddr(col, row) = attr;
}

/* -------------------------------------------------------------------------
   zx_fill_attr_rect
   Fill a rectangle of attribute cells with 'attr'.
   Params: col,row : top-left cell
           w,h     : width/height in cells
   ------------------------------------------------------------------------- */
void zx_fill_attr_rect(uint8_t col, uint8_t row, uint8_t w, uint8_t h, uint8_t attr)
{
    uint8_t r;
    for (r = 0; r < h; ++r) {
        uint8_t *a = zx_attraddr(col, (uint8_t)(row + r));
        uint8_t c;
        for (c = 0; c < w; ++c) {
            a[c] = attr;
        }
    }
}

/* -------------------------------------------------------------------------
   zx_out_border
   Set the hardware border colour (port 0xFE).  Only the lower 3 bits are
   meaningful (0..7).  Wrapped in __Z88DK__ so non-z88dk tools can parse it.
   ------------------------------------------------------------------------- */
void zx_out_border(uint8_t color)
{
    #ifdef __Z88DK__
        __asm
            ld a, l        ; 'color' in L
            and 0x07
            ld bc, 0x00FE
            out (c), a
        __endasm;
    #else
        (void)color;
    #endif
}

/* -------------------------------------------------------------------------
   zx_blit8_aligned
   Blit an 8x8 monochrome tile to pixel position (x,y).
   Requirements:
     - x must be a multiple of 8 (byte-aligned).
     - 'src' must point to 8 bytes, one per scanline, MSB = leftmost pixel.
   Note:
     - This does a direct store, i.e., it overwrites destination bytes.
       Use for border tiles, fonts, and sprites that live on an empty bg.
   ------------------------------------------------------------------------- */
void zx_blit8_aligned(uint8_t x, uint8_t y, const uint8_t *src)
{
    uint8_t *dst = zx_pixaddr(x, y);
    uint8_t i;
    for (i = 0; i < 8u; ++i) {
        *dst = src[i];
        dst += 256u;   /* <-- was 256/8; use 256 bytes per next scanline */
    }
}

/* -------------------------------------------------------------------------
   zx_blit8
   Convenience wrapper for aligned 8x8 blits.  For now we require x%8==0.
   ------------------------------------------------------------------------- */
void zx_blit8(uint8_t x, uint8_t y, const uint8_t *src)
{
    /* Require x%8==0 for simplicity (tiles / font / border use multiples of 8). */
    zx_blit8_aligned(x, y, src);
}

