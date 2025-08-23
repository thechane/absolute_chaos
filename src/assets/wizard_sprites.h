#ifndef wizard_spriteS_H
#define wizard_spriteS_H
#include <stdint.h>

typedef struct { const char* name; const uint8_t* bytes; } ChaosSpriteRef;

/* Registry accessors implemented in wizard_sprites.c */
uint16_t wizard_sprite_count(void);
const ChaosSpriteRef* wizard_sprite_ref(uint16_t idx);
const uint8_t* wizard_sprite_bytes(uint16_t idx);

#endif
