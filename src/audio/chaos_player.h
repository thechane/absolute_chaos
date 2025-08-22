#ifndef CHAOS_PLAYER_H
#define CHAOS_PLAYER_H

#include <stdint.h>

/* Low-level: play one Chaos 10-byte beeper block */
void play_sfx_block(const uint8_t s[10]);

/* Convenience: play by index from chaos_sfx.c registry */
void chaos_play_sfx_idx(uint16_t idx);

#endif /* CHAOS_PLAYER_H */