#include "draw.h"
#include "model.h"
#include <stdio.h>
#ifdef PBL_BW
// Black-and-white screens lose the skulls' red/orange health colours (dark
// colours draw black). A two-hit skull is a solid white face with black
// features; a one-hit skull is a hollow ring with white features; a skull
// that was just struck gets a second ring.
static void skull_bw(GContext *c, int x, int y, int hp, bool struck) {
  ink(c, GColorWhite);
  circle(c, x, y, 10, hp == 2);
  circle(c, x, y, 10, false);
  if (struck)
    circle(c, x, y, 13, false);
  ink(c, hp == 2 ? GColorBlack : GColorWhite);
  box(c, x - 5, y - 3, 3, 3, true);
  box(c, x + 2, y - 3, 3, 3, true);
  line(c, x - 3, y + 5, x + 3, y + 5);
}
#endif
void game_draw(const Game *g, GContext *c) {
  ink(c, PBL_IF_COLOR_ELSE(GColorLightGray, GColorWhite));
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
    int x = g->d[b] / 100, y = g->d[b + 1] / 100;
#ifdef PBL_BW
    skull_bw(c, x, y, g->d[b + 2], g->d[b + 3]);
#else
    ink(c, g->d[b + 3]        ? GColorYellow
           : g->d[b + 2] == 2 ? GColorRed
                              : GColorOrange);
    circle(c, x, y, 10, true);
    ink(c, GColorWhite);
    circle(c, x, y, 10, false);
    ink(c, GColorBlack);
    box(c, x - 5, y - 3, 3, 3, true);
    box(c, x + 2, y - 3, 3, 3, true);
    line(c, x - 3, y + 5, x + 3, y + 5);
#endif
  }
  // Closed gate: bars. Open gate: a "B" with one pip per remaining boss hit.
  ink(c, PBL_IF_COLOR_ELSE(alive ? GColorLightGray : GColorMagenta, GColorWhite));
  box(c, 75, 10, 26, 25, false);
  if (alive) {
    line(c, 78, 10, 78, 34);
    line(c, 85, 10, 85, 34);
    line(c, 92, 10, 92, 34);
    line(c, 99, 10, 99, 34);
  } else {
    large_label(c, "B", 76, 7, 24);
    for (int i = 0; i < g->d[BOSS]; i++)
      box(c, 78 + i * 7, 35, 4, 3, true);
  }
  // A ball lost through the drain is off the table; drawing it put a dot over
  // the result screen's "BACK: menu" line.
  if (g->d[BALL_Y] <= 14500) {
    ink(c, GColorWhite);
    circle(c, g->d[BALL_X] / 100, g->d[BALL_Y] / 100, 4, true);
  }
}
void game_hud_compact(const Game *g, char *a, size_t n, char *b, size_t m) {
  int room = g->stage < 3 ? g->stage + 1 : 3, alive = 0;
  long balls = g->d[LIVES];
  for (int i = 0; i < 3; i++)
    alive += g->d[BUMPERS + i * 4 + 2];
#ifdef PBL_ROUND
  // The Time Round's top chord is too short for the chamber and the balls
  // together, so the ball count moves to the wider bottom line during play.
  snprintf(a, n, "Chamber %d/3", room);
  if (g->d[WAITING])
    snprintf(b, m, "SELECT: launch");
  else if (alive)
    snprintf(b, m, "Skulls %d  Balls %ld", alive, balls);
  else
    snprintf(b, m, "Boss %d  Balls %ld", (int)g->d[BOSS], balls);
#else
  // 144 px: one space fewer than Time 2 keeps the top line whole.
  snprintf(a, n, "Chamber %d/3 Balls %ld", room, balls);
  if (g->d[WAITING])
    snprintf(b, m, "SELECT: launch");
  else if (alive)
    snprintf(b, m, "Skulls %d  UP/DN: flip", alive);
  else
    snprintf(b, m, "Gate open! Boss %d", (int)g->d[BOSS]);
#endif
}
