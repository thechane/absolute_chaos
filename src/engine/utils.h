#ifndef ENGINE_UTILS_H
#define ENGINE_UTILS_H

#include <stdint.h>

/* Sleep for a number of video frames using HALT/IRQ (ZX Spectrum: 50 Hz). */
void wait_frames(uint8_t frames);

#endif /* ENGINE_UTILS_H */