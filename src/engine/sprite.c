#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <z80.h>
#include "assets/wizard_sprites.h"

/* -------- Classic ZX screen (1bpp) -------- */
#define ZX_PIXELS_BASE  ((uint8_t*)0x4000)     /* 256x192 = 6144 bytes */
#define ZX_ATTR_BASE    ((uint8_t*)0x5800)     /* 32x24 = 768 bytes */

static uint8_t* zx_rowaddr(uint8_t y) {
    /* y:0..191 -> offset with Speccy’s funky row mapping */
    uint16_t off = ((y & 0xC0) << 5) | ((y & 0x07) << 8) | ((y & 0x38) << 2);
    return ZX_PIXELS_BASE + off;
}

void chaos_cls(void) {
    memset(ZX_PIXELS_BASE, 0x00, 6144);
    memset(ZX_ATTR_BASE,   0x00,  768);        /* paper black, ink black */
}

/* Draw 16x16 sprite (32 bytes) at byte-aligned X (multiples of 8). OR-blit. */
void chaos_draw_sprite_idx(uint16_t idx, uint8_t x, uint8_t y) {
    if (idx >= wizard_sprite_count()) return;
    const uint8_t* sp = wizard_sprite_bytes(idx);
    uint8_t xb = x >> 3;

    uint8_t row;
    for (row = 0; row < 16; ++row) {
        uint8_t* p = zx_rowaddr(y + row) + xb;
        /* Layout: [0..7]=top-left, [8..15]=top-right, [16..23]=bot-left, [24..31]=bot-right */
        uint8_t left  = (row < 8) ? sp[row]           : sp[16 + (row - 8)];
        uint8_t right = (row < 8) ? sp[8 + row]       : sp[24 + (row - 8)];
        p[0] |= left;
        p[1] |= right;
    }
    /* give it bright white attributes over 2x2 cells */
    uint8_t ax = x >> 3, ay = y >> 3;
    uint8_t dy;
    for (dy = 0; dy < 2; ++dy) {
        uint8_t* arow = ZX_ATTR_BASE + (ay + dy) * 32;
        arow[ax+0] = 0x47;    /* bright white on black */
        arow[ax+1] = 0x47;
    }
}

const char* wizard_sprite_name(uint16_t idx) { return wizard_sprite_ref(idx)->name; }
