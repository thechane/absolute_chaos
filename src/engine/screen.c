#include "screen.h"
#include "config.h"

/* Bring in the per-screen entry/update functions. */
#include "screens/screen_opening.h"
#include "screens/screen_setup.h"
#include "screens/screen_menu.h"
#include "screens/screen_spellbook.h"
#include "screens/screen_game.h"

/* =========================================================================
   Simple screen manager
   -------------------------------------------------------------------------
   - A "screen" has two functions:
       * enter(): called once when the screen becomes active
       * update(): called every tick (main loop) while active
   - This file maps screen IDs to those functions and provides helpers to
     initialize, switch, and update the current screen.
   - Everything is C89-compliant (no designated initializers, no bool).
   ========================================================================= */

/* If your screen.h defines these IDs, you can remove this block.
   They are provided here for clarity and to keep this TU self-contained. */
#ifndef SCREEN_OPENING
#define SCREEN_OPENING   0
#define SCREEN_SETUP     1
#define SCREEN_MENU      2
#define SCREEN_SPELLBOOK 3
#define SCREEN_GAME      4
#define SCREEN_COUNT     5
#endif

/* Function pointer types for screen lifecycle. */
typedef void (*ScreenEnterFn)(void);
typedef void (*ScreenUpdateFn)(void);

/* Tables mapping screen ID -> enter/update handlers.
   ORDER MUST MATCH the SCREEN_* constants above. */
static ScreenEnterFn  s_enter[SCREEN_COUNT] = {
    opening_enter,
    setup_enter,
    menu_enter,
    spellbook_enter,
    game_enter
};

static ScreenUpdateFn s_update[SCREEN_COUNT] = {
    opening_update,
    setup_update,
    menu_update,
    spellbook_update,
    game_update
};

/* Current screen index (0..SCREEN_COUNT-1). */
static unsigned char s_current = SCREEN_OPENING;

/* -------------------------------------------------------------------------
   screen_init
   Initialize the screen system and show the default screen.
   The default is SCREEN_OPENING; change s_current below if you prefer a
   different starting point.
   ------------------------------------------------------------------------- */
void screen_init(void)
{
    s_current = SCREEN_OPENING;
    if (s_enter[s_current]) {
        s_enter[s_current]();
    }
}

/* -------------------------------------------------------------------------
   screen_set
   Switch to a new screen by ID and call its enter() handler.
   Invalid IDs are ignored.

   Params:
     id : 0..SCREEN_COUNT-1 (use SCREEN_* constants)
   ------------------------------------------------------------------------- */
void screen_set(unsigned char id)
{
    if (id >= SCREEN_COUNT) {
        return;
    }
    s_current = id;
    if (s_enter[s_current]) {
        s_enter[s_current]();
    }
}

/* -------------------------------------------------------------------------
   screen_current
   Returns the current screen ID (one of SCREEN_*).
   ------------------------------------------------------------------------- */
unsigned char screen_current(void)
{
    return s_current;
}

/* -------------------------------------------------------------------------
   screen_update
   Call the current screen's update() handler.
   Call this once per main loop tick or per frame.
   ------------------------------------------------------------------------- */
void screen_update(void)
{
    ScreenUpdateFn fn = s_update[s_current];
    if (fn) {
        fn();
    }
}