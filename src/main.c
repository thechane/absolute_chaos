#include <stdio.h>
#include "chaos.h"

int main(void) {
    chaos_cls();
    printf("\x0CChaos demo — sprites + SFX by name\n");

    /* Cycle through all sprites; play SFX round-robin */
    uint16_t sfxi = 0;
    for (uint16_t sp = 0; sp < wizard_sprite_count(); ++sp) {
        /* arrange in a grid (16x16, 8px margin), byte-aligned X */
        uint8_t col = (sp % 10);
        uint8_t row = (sp / 10);
        uint8_t x = 8 + 16 * col;   /* multiples of 8 for tidy OR blit */
        uint8_t y = 8 + 24 * row;

        chaos_draw_sprite_idx(sp, x, y);

        /* play one sfx each sprite */
        chaos_play_sfx_idx(sfxi);
        sfxi = (uint16_t)((sfxi + 1) % chaos_sfx_count());

        chaos_ms(150);
    }

    printf("\nDone. Press Enter.\n");
    getchar();
    return 0;
}
