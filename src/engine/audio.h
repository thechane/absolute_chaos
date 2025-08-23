#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>

/* Sound API (beeper SFX; Chaos 10-byte blocks) */
const char* chaos_sfx_name(uint16_t idx);
void chaos_play_sfx_idx(uint16_t idx);
void chaos_play_sfx_by_name(const char* name);

/* Timing helpers */
void chaos_ms(uint16_t ms);

#endif
