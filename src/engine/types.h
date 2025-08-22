/* src/engine/types.h — C89-safe */
#ifndef TYPES_H
#define TYPES_H

#include <stdint.h> /* if your toolchain lacks this in C89 mode, we can typedef u8/u16 instead */

/* bool replacement for C89 */
#ifndef CHAOS_BOOL_DEFINED
typedef unsigned char chaos_bool;
#define CHAOS_TRUE  1
#define CHAOS_FALSE 0
#define CHAOS_BOOL_DEFINED
#endif

typedef struct {
  uint8_t    ink;    /* 0..7 */
  uint8_t    paper;  /* 0..7 */
  chaos_bool bright; /* 0/1 */
  chaos_bool flash;  /* 0/1 */
} Attr;

/* Fast macro to build a Spectrum attribute byte from an Attr */
#define MAKE_ATTR_FROM(a)   ( ((a).flash ? 0x80 : 0) | ((a).bright ? 0x40 : 0) | (((a).paper & 7) << 3) | ((a).ink & 7) )
#define MAKE_ATTR(ink,paper,bright,flash) ( ((flash)?0x80:0) | ((bright)?0x40:0) | (((paper)&7)<<3) | ((ink)&7) )

/* Optional non-inline helper if you prefer a function call in C89 */
static uint8_t make_attr(Attr a) { return MAKE_ATTR_FROM(a); }

#endif /* TYPES_H */