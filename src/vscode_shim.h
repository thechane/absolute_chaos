#ifndef VSCODE_SHIM_H
#define VSCODE_SHIM_H
#ifdef __INTELLISENSE__
typedef volatile unsigned char __sfr;
#define __at(x)
static inline void z80_delay_tstate(unsigned short) {}
static inline void z80_delay_ms(unsigned int) {}
#endif
#endif
