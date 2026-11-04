#pragma once
#include "arcade.h"
#include <pebble.h>
#include "ui_style.h"
extern int field_x, field_y;
void game_draw(const Game *, GContext *);
void ink(GContext *, GColor);
void line(GContext *, int, int, int, int);
void box(GContext *, int, int, int, int, bool);
void circle(GContext *, int, int, int, bool);
void label(GContext *, const char *, int, int, int);
void large_label(GContext *, const char *, int, int, int);
#ifdef GAME_HUD_COMPACT
// Shorter HUD lines for 144x168 and the Time Round.
void game_hud_compact(const Game *, char *, size_t, char *, size_t);
#endif
#ifdef GAME_RESULT_COMPACT
// A shorter result title for the 132 px card on 144x168 and the Time Round.
void game_result_compact(const Game *, char *, size_t, char *, size_t);
#endif
