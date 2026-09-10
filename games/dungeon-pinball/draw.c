#include "draw.h"
#include "model.h"
void game_draw(const Game *g, GContext *c) {
  ink(c, GColorDarkGray);
  box(c, 6, 6, 164, 133, false);
  ink(c, GColorLightGray);
  line(c, 10, 98, 42, 124);
  line(c, 166, 98, 134, 124);
  ink(c, GColorCyan);
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
    ink(c, g->d[b + 3]        ? GColorYellow
           : g->d[b + 2] == 2 ? GColorRed
                              : GColorOrange);
    circle(c, x, y, 10, true);
    ink(c, GColorBlack);
    box(c, x - 5, y - 3, 3, 3, true);
    box(c, x + 2, y - 3, 3, 3, true);
    line(c, x - 3, y + 5, x + 3, y + 5);
  }
  ink(c, alive ? GColorDarkGray : GColorMagenta);
  box(c, 75, 10, 26, 25, false);
  if (alive) {
    line(c, 78, 10, 78, 34);
    line(c, 85, 10, 85, 34);
    line(c, 92, 10, 92, 34);
    line(c, 99, 10, 99, 34);
  } else {
    label(c, "B", 76, 11, 24);
    for (int i = 0; i < g->d[BOSS]; i++)
      box(c, 78 + i * 7, 35, 4, 3, true);
  }
  ink(c, GColorWhite);
  circle(c, g->d[BALL_X] / 100, g->d[BALL_Y] / 100, 3, true);
}
