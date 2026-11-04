#include "draw.h"
#include "model.h"
#include <stdio.h>
// Pebble Time 2 and Round 2 draw exactly as in 0.3.1. Black-and-white screens
// (Pebble 2, Pebble 2 Duo) replace colour with shape: a skull with two hits
// left is solid, with one hit left it is an outline, and a struck skull gets a
// halo. The locked gate keeps its bars; the open gate is a solid block with B.
static void skull(GContext *c, int x, int y, int hits, bool struck) {
#ifdef PBL_BW
  bool solid = hits == 2;
  ink(c, GColorWhite);
  circle(c, x, y, 10, solid);
  if (struck)
    circle(c, x, y, 13, false);
  ink(c, solid ? GColorBlack : GColorWhite);
#else
  ink(c, struck      ? GColorYellow
         : hits == 2 ? GColorRed
                     : GColorOrange);
  circle(c, x, y, 10, true);
  ink(c, GColorWhite);
  circle(c, x, y, 10, false);
  ink(c, GColorBlack);
#endif
  box(c, x - 5, y - 3, 3, 3, true);
  box(c, x + 2, y - 3, 3, 3, true);
  line(c, x - 3, y + 5, x + 3, y + 5);
}
void game_draw(const Game *g, GContext *c) {
  GColor rail = PBL_IF_COLOR_ELSE(GColorLightGray, GColorWhite);
  ink(c, rail);
  box(c, 6, 6, 164, 133, false);
  line(c, 10, 98, 42, 124);
  line(c, 166, 98, 134, 124);
  ink(c, PBL_IF_COLOR_ELSE(GColorCyan, GColorWhite));
  graphics_context_set_stroke_width(c, 4);
  line(c, 42, 124, 78, g->d[LEFT] ? 108 : 135);
  line(c, 134, 124, 98, g->d[RIGHT] ? 108 : 135);
  graphics_context_set_stroke_width(c, 1);
  int alive = 0;
  for (int i = 0; i < 3; i++) {
    int b = BUMPERS + i * 4;
    if (!g->d[b + 2])
      continue;
    alive++;
    skull(c, g->d[b] / 100, g->d[b + 1] / 100, g->d[b + 2], g->d[b + 3]);
  }
  ink(c, alive ? rail : PBL_IF_COLOR_ELSE(GColorMagenta, GColorWhite));
  box(c, 75, 10, 26, 25, PBL_IF_BW_ELSE(!alive, false));
  if (alive) {
    line(c, 78, 10, 78, 34);
    line(c, 85, 10, 85, 34);
    line(c, 92, 10, 92, 34);
    line(c, 99, 10, 99, 34);
  } else {
#ifdef PBL_BW
    // A black B inside the white block, with the boss's hits left as bars
    // under it, clear of the block.
    ink(c, GColorBlack);
    label(c, "B", 76, 8, 24);
    ink(c, GColorWhite);
    for (int i = 0; i < g->d[BOSS]; i++)
      box(c, 78 + i * 7, 37, 4, 3, true);
#else
    large_label(c, "B", 76, 7, 24);
    for (int i = 0; i < g->d[BOSS]; i++)
      box(c, 78 + i * 7, 35, 4, 3, true);
#endif
  }
  ink(c, GColorWhite);
  circle(c, g->d[BALL_X] / 100, g->d[BALL_Y] / 100, 4, true);
}
#if UI_COMPACT
// 144x168 and the Time Round: the same facts in fewer words, without the
// touch hint (none of these watches has a touchscreen).
void game_hud_compact(const Game *g, char *top, size_t n, char *bottom, size_t m) {
  int alive = 0;
  for (int i = 0; i < 3; i++)
    alive += g->d[BUMPERS + i * 4 + 2];
  snprintf(top, n, "Chamber %d/3", g->stage < 3 ? g->stage + 1 : 3);
  if (g->d[WAITING])
    snprintf(bottom, m, "SELECT: launch");
  else // Skulls left to hit, then the boss's hits left once the gate opens.
    snprintf(bottom, m, "Balls %ld  %s %ld", (long)g->d[LIVES], alive ? "Skulls" : "Boss",
             (long)(alive ? alive : g->d[BOSS]));
}
#endif
