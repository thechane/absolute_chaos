#ifndef BORDER_H
#define BORDER_H

#include <stdint.h>
#include "types.h"
#include "config.h"

/* Theme for attribute colours (used by border_recolor / border_draw) */
typedef struct {
    uint8_t primary_ink;    /* inner ring ink  (0..7) */
    uint8_t secondary_ink;  /* outer ring ink  (0..7) */
    uint8_t paper;          /* background paper (0..7) */
    uint8_t bright;         /* 0/1 */
} BorderTheme;

/* Common 8x8 patterns (defined in border.c) */
extern const uint8_t PAT_VSTRIPE_THIN[8];   /* 10101010 vertical bars */
extern const uint8_t PAT_VSTRIPE_MED [8];   /* 11001100 vertical bars */
extern const uint8_t PAT_VSTRIPE_THICK[8];  /* 11110000 vertical bars */
extern const uint8_t PAT_CHECKER_A   [8];   /* AA/55 checker */
extern const uint8_t PAT_CHECKER_B   [8];   /* CC/33 checker */
extern const uint8_t PAT_SOLID       [8];   /* FF solid */

/* Compute inner rectangle (pixels) inside the decorative frame */
void border_get_inner(uint8_t *x, uint8_t *y, uint8_t *w, uint8_t *h);

/* Recolour existing border instantly by touching attributes only */
void border_recolor(BorderTheme theme);

/* Draw full two‑ring border with chosen pixel patterns.
   pat_outer / pat_inner: pointers to 8‑byte tiles (may pass PAT_* above).
   sfx_index: pass >=0 to play that SFX, or <0 to stay silent. */
void border_draw(BorderTheme theme,
                 const uint8_t *pat_outer,
                 const uint8_t *pat_inner,
                 int sfx_index);

#endif /* BORDER_H */