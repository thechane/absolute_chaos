#include "beeper.h"

/* Provide the latch here (single definition). */
volatile uint8_t fe_latch = 0x00;

#ifndef __Z88DK__
/* Dummy for non-z88dk parsing (e.g., clangd). Not used at runtime. */
volatile uint8_t ULA_FE = 0x00;
#endif

/* Your exact toggle (C89-safe, no inline). */
void beeper_toggle(void)
{
    fe_latch ^= 0x30;
    ULA_FE = fe_latch;
}

/* Your delay; keep as a stub unless you want real tstate timing. */
void z80_delay_tstate(unsigned short t)
{
    (void)t;  /* no-op; you can add NOP loops or HALT here if needed */
}