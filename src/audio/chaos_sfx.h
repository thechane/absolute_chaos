#ifndef CHAOS_SFX_H
#define CHAOS_SFX_H
#include <stdint.h>

typedef struct { const char* name; const uint8_t* bytes; } ChaosSfxRef;

uint16_t chaos_sfx_count(void);
const ChaosSfxRef* chaos_sfx_ref(uint16_t idx);
const uint8_t* chaos_sfx_bytes(uint16_t idx);

#endif