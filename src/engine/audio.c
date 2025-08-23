#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <z80.h>

#include "engine/audio.h"
#include "assets/chaos_sfx.h"
#include "assets/wizard_sprites.h"

/* -------- ULA beeper/border (port $FE) -------- */
static __sfr __at(0xFE) ULA_FE;
static uint8_t fe_latch = 0x00; /* border black; bits 4/5 used for beeper like Chaos (xor $30) */

static inline void beeper_toggle(void) {
    fe_latch ^= 0x30;
    ULA_FE = fe_latch;
}

/* Chaos tiny delay: approx (22 + 13*B) t-states per invocation */
static inline void chaos_delay_b(uint8_t b) {
    unsigned short t = (unsigned short)(22u + 13u * (unsigned short)b);
    z80_delay_tstate(t);
}

/* -------- Classic ZX screen (1bpp) -------- */
#define ZX_PIXELS_BASE  ((uint8_t*)0x4000)     /* 256x192 = 6144 bytes */
#define ZX_ATTR_BASE    ((uint8_t*)0x5800)     /* 32x24 = 768 bytes */

static inline uint8_t* zx_rowaddr(uint8_t y) {
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

    for (uint8_t row = 0; row < 16; ++row) {
        uint8_t* p = zx_rowaddr(y + row) + xb;
        /* Layout: [0..7]=top-left, [8..15]=top-right, [16..23]=bot-left, [24..31]=bot-right */
        uint8_t left  = (row < 8) ? sp[row]           : sp[16 + (row - 8)];
        uint8_t right = (row < 8) ? sp[8 + row]       : sp[24 + (row - 8)];
        p[0] |= left;
        p[1] |= right;
    }
    /* give it bright white attributes over 2x2 cells */
    uint8_t ax = x >> 3, ay = y >> 3;
    for (uint8_t dy = 0; dy < 2; ++dy) {
        uint8_t* arow = ZX_ATTR_BASE + (ay + dy) * 32;
        arow[ax+0] = 0x47;    /* bright white on black */
        arow[ax+1] = 0x47;
    }
}

/* -------- SFX playback (Chaos 10-byte block) -------- */
static void play_sfx_block(const uint8_t s[10]) {
    uint8_t outer = s[0];
    uint8_t middle0 = s[1];
    uint8_t d0 = s[2], d1 = s[3], d2 = s[4], d3 = s[5];
    uint8_t a0 = s[6], a1 = s[7], a2 = s[8], a3 = s[9];

    while (outer--) {
        uint8_t middle = middle0;
        while (middle--) {
            chaos_delay_b(d0); beeper_toggle();
            chaos_delay_b(d1); beeper_toggle();
            chaos_delay_b(d2); beeper_toggle();
            chaos_delay_b(d3); beeper_toggle();
            d0 = (uint8_t)(d0 + a0);
            d1 = (uint8_t)(d1 + a1);
            d2 = (uint8_t)(d2 + a2);
            d3 = (uint8_t)(d3 + a3);
        }
    }
}

void chaos_play_sfx_idx(uint16_t idx) {
    if (idx >= chaos_sfx_count()) return;
    play_sfx_block(chaos_sfx_bytes(idx));
}

/* -------- Lookup by name (case-insensitive) -------- */
static int ci_eq(const char* a, const char* b) {
    uint8_t ca, cb;
    while (*a && *b) {
        ca = (uint8_t)tolower((unsigned char)*a++);
        cb = (uint8_t)tolower((unsigned char)*b++);
        if (ca != cb) return 0;
    }
    return (*a == '\0' && *b == '\0');
}

int16_t chaos_find_sprite(const char* name) {
    for (uint16_t i = 0; i < wizard_sprite_count(); ++i)
        if (ci_eq(name, wizard_sprite_name(i))) return (int16_t)i;
    return -1;
}

int16_t chaos_find_sfx(const char* name) {
    for (uint16_t i = 0; i < chaos_sfx_count(); ++i)
        if (ci_eq(name, chaos_sfx_name(i))) return (int16_t)i;
    return -1;
}

void chaos_draw_sprite_by_name(const char* name, uint8_t x, uint8_t y) {
    int16_t idx = chaos_find_sprite(name);
    if (idx >= 0) chaos_draw_sprite_idx((uint16_t)idx, x, y);
}

void chaos_play_sfx_by_name(const char* name) {
    int16_t idx = chaos_find_sfx(name);
    if (idx >= 0) chaos_play_sfx_idx((uint16_t)idx);
}

/* -------- Misc -------- */
void chaos_ms(uint16_t ms) { z80_delay_ms(ms); }

const char* wizard_sprite_name(uint16_t idx) { return wizard_sprite_ref(idx)->name; }
const char* chaos_sfx_name(uint16_t idx)    { return chaos_sfx_ref(idx)->name;  }
