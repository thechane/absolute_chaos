#include "chaos_player.h"
#include "chaos_sfx.h"
#include "beeper.h"      /* <-- use your beeper + delay */
#include <stdint.h>

/* C89-safe helper (no inline) */
static void chaos_delay_b(uint8_t b)
{
    /* Matches your formula */
    unsigned short t = (unsigned short)(22u + (unsigned short)13u * (unsigned short)b);
    z80_delay_tstate(t);
}

/* -------- SFX playback (Chaos 10-byte block) -------- */
void play_sfx_block(const uint8_t s[10])
{
    uint8_t outer   = s[0];
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

/* Play by index from your registry */
void chaos_play_sfx_idx(uint16_t idx)
{
    const uint8_t *blk;
    if (idx >= chaos_sfx_count()) return;
    blk = chaos_sfx_bytes(idx);
    if (!blk) return;
    play_sfx_block(blk);
}