#ifndef SPRITE_H
#define SPRITE_H

/* Screen helpers */
void chaos_cls(void);

/* Sprite API (16x16, 1bpp, byte-aligned X) */
uint16_t wizard_sprite_count(void);
void chaos_draw_sprite_idx(uint16_t idx, uint8_t x, uint8_t y);

#endif
