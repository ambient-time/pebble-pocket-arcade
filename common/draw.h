#pragma once
#include "arcade.h"
#include <pebble.h>
extern int field_x, field_y;
void game_draw(const Game *, GContext *);
void ink(GContext *, GColor);
void line(GContext *, int, int, int, int);
void box(GContext *, int, int, int, int, bool);
void circle(GContext *, int, int, int, bool);
void label(GContext *, const char *, int, int, int);
