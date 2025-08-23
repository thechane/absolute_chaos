#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <z80.h>
#include "engine/audio.h"
#include "assets/chaos_sfx.h"


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

/* -------- Misc -------- */
void chaos_ms(uint16_t ms) { z80_delay_ms(ms); }

