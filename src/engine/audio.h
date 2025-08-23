#ifndef CHAOS_H
#define CHAOS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Screen helpers */
void chaos_cls(void);

/* Sprite API (16x16, 1bpp, byte-aligned X) */
uint16_t wizard_sprite_count(void);
const char* wizard_sprite_name(uint16_t idx);
int16_t chaos_find_sprite(const char* name);                  /* -1 if not found */
void chaos_draw_sprite_idx(uint16_t idx, uint8_t x, uint8_t y);
void chaos_draw_sprite_by_name(const char* name, uint8_t x, uint8_t y);

/* Sound API (beeper SFX; Chaos 10-byte blocks) */
uint16_t chaos_sfx_count(void);
const char* chaos_sfx_name(uint16_t idx);
int16_t chaos_find_sfx(const char* name);                     /* -1 if not found */
void chaos_play_sfx_idx(uint16_t idx);
void chaos_play_sfx_by_name(const char* name);

/* Timing helpers */
void chaos_ms(uint16_t ms);

#ifdef __cplusplus
}
#endif
#endif
