#ifndef BEEPER_H
#define BEEPER_H

#include <stdint.h>

/* Port 0xFE (ULA) mapping for SDCC/z88dk.
   On z88dk+SDCC, __sfr maps an 8-bit I/O port. */
#ifdef __Z88DK__
__sfr __at 0xFE ULA_FE;
#else
/* Non-z88dk (e.g., clangd/IntelliSense) see a dummy symbol so it links/parses. */
extern volatile uint8_t ULA_FE;
#endif

/* Latch used by your toggle routine. Exported so the player can link. */
extern volatile uint8_t fe_latch;

/* Your routines */
void beeper_toggle(void);
void z80_delay_tstate(unsigned short t);

#endif /* BEEPER_H */