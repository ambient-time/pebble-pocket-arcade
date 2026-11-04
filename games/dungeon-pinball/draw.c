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
#if UI_COMPACT
static void status_line(const Game *, char *, size_t);
#endif
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
    ink(c, PBL_IF_BW_ELSE(GColorBlack, GColorMagenta));
    large_label(c, "B", 76, 7, 24);
    ink(c, PBL_IF_BW_ELSE(GColorWhite, GColorMagenta));
    for (int i = 0; i < g->d[BOSS]; i++)
      box(c, 78 + i * 7, 35, 4, 3, true);
  }
  ink(c, GColorWhite);
  circle(c, g->d[BALL_X] / 100, g->d[BALL_Y] / 100, 4, true);
#if UI_COMPACT && defined(PBL_ROUND)
  // Time Round status line: directly under the table's bottom rail (y 145),
  // where the circle is still 110 px wide. Finished rounds show the shell's
  // "BACK: menu" instead.
  if (!g->status) {
    char b[32];
    status_line(g, b, sizeof b);
    graphics_draw_text(c, b, fonts_get_system_font(UI_FONT_SMALL), GRect(35, 143, 110, 24),
                       GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
  }
#endif
}
#if UI_COMPACT
// 144x168 and the Time Round: the same facts in fewer words, without the
// touch hint (none of these watches has a touchscreen).
static void status_line(const Game *g, char *out, size_t m) {
  int alive = 0;
  for (int i = 0; i < 3; i++)
    alive += g->d[BUMPERS + i * 4 + 2];
  if (g->d[WAITING])
    snprintf(out, m, "SELECT: launch");
  else if (alive)
    snprintf(out, m, "Balls %ld  Skulls %d", (long)g->d[LIVES], alive);
  else
    snprintf(out, m, "Balls %ld  Boss %ld", (long)g->d[LIVES], (long)g->d[BOSS]);
}
void game_hud_compact(const Game *g, char *top, size_t n, char *bottom, size_t m) {
  snprintf(top, n, "Chamber %d/3", g->stage < 3 ? g->stage + 1 : 3);
#ifdef PBL_ROUND
  // The shell's bottom caption sits where the Time Round's chord is about
  // 100 px; game_draw puts the status line just under the table instead.
  (void)m;
  bottom[0] = 0;
#else
  status_line(g, bottom, m);
#endif
}
#endif
