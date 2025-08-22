

/* 
 * wait_frames (SleepyTime) - FRAME-ACCURATE BUSY-SLEEP FOR Z80 TARGETS
 *
 * wait_frames(frames) halts the Z80 CPU for a given number of video frames by
 * executing HALT in a loop. On the ZX Spectrum (and most Z80-based systems),
 * a maskable interrupt (INT) is generated once per video frame (VBlank), at:
 *
 *   • 50 Hz on PAL Spectrum models  → ~20.00 ms per frame
 *   • 60 Hz on NTSC-like targets    → ~16.67 ms per frame
 *
 * HOW IT WORKS
 * ------------
 * 1) HALT stops the CPU clocking instructions and places the CPU in a low-power
 *    state until *any* maskable interrupt is accepted.
 * 2) At the start of each video frame, the ULA generates an INT; when IFF1=1
 *    (interrupts enabled), the CPU wakes, services the ISR, then resumes at the
 *    next instruction after HALT.
 * 3) We wrap HALT in a decrementing loop; each accepted frame interrupt consumes
 *    exactly one iteration. Therefore:
 *
 *      frames = 1  → ~20 ms (PAL)   / ~16.7 ms (NTSC)
 *      frames = 25 → ~0.5 s (PAL)
 *      frames = 50 → ~1.0 s (PAL)
 *
 * REQUIREMENTS & CAVEATS
 * ----------------------
 * • Interrupts must be enabled (EI) before calling; with DI active, HALT will
 *   sleep indefinitely (until an NMI occurs).
 * • A periodic maskable interrupt source must be configured (the Spectrum ROM
 *   does this by default; IM 1 or IM 2 both work).
 * • The ISR should return with interrupts enabled; if your handler leaves IFF1=0
 *   (e.g., disables interrupts too long), timing will stretch or stall.
 * • Timing is per-frame, not per-millisecond. If your program changes the video
 *   rate (e.g., runs on 60 Hz hardware/emulator), the per-frame duration changes
 *   accordingly.
 * • This is a sleep, not a frame-skip: the CPU truly idles. If you need to poll
 *   or do work between frames, integrate that into your ISR or loop logic.
 *
 * WHEN TO USE
 * -----------
 * • Title screens, splash delays, fade steps.
 * • Simple pacing between animation frames where strict millisecond precision is
 *   unnecessary but frame granularity is ideal.
 * • CPU power saving between periodic tasks on systems with regular frame IRQs.
 *
 * NON-Z88DK FALLBACK
 * ------------------
 * For non-z88dk builds this function is a no-op placeholder so projects that
 * share code can still compile. If you need it to work elsewhere, replace the
 * no-op with your platform’s frame or timer wait.
 */

#include <stdint.h>

void wait_frames(uint8_t frames)
{
#ifdef __Z88DK__
    while (frames--) {
        __asm
            halt        ; CPU sleeps until next maskable interrupt
        __endasm;
    }
#else
    (void)frames; /* no-op for non-z88dk */
#endif
}