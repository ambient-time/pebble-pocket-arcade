#include "draw.h"
#include "model.h"
#include "trig.h"
void game_draw(const Game *g, GContext *c) {
  for (int i = 0; i < 50; i++) {
    ink(c, i % 4 ? GColorDarkGray : GColorWhite);
    int x = (i * 71 + 13) % 176, y = (i * 37 + 9) % 144;
    if (g->d[FX])
      x += (88 - x) * (16 - g->d[FX]) / 200;
    box(c, x, y, 1, 1, true);
  }
  ink(c, GColorCyan);
  circle(c, 20, 72, 15, false);
  label(c, "DOCK", 0, 82, 42);
  ink(c, g->d[BANKED] == 5 ? GColorYellow : GColorDarkGray);
  circle(c, 20, 18, 12, false);
  label(c, "EXIT", 0, 25, 40);
  for (int i = 0; i < 5; i++) {
    int b = CARGO + i * 3;
    if (!g->d[b + 2]) {
      ink(c, GColorYellow);
      box(c, g->d[b] / 100 - 4, g->d[b + 1] / 100 - 4, 8, 8, false);
    }
  }
  int n = 3 + (4500 - g->d[DEADLINE]) / 1500;
  if (n > 6)
    n = 6;
  for (int i = 0; i < n; i++) {
    int x = g->d[ROCKS + i * 4] / 100, y = g->d[ROCKS + i * 4 + 1] / 100;
    ink(c, GColorLightGray);
    circle(c, x, y, 7, true);
    ink(c, GColorBlack);
    box(c, x - 5, y - 2, 3, 3, true);
    box(c, x + 2, y + 2, 2, 2, true);
  }
  int x = g->d[X] / 100, y = g->d[Y] / 100, a = g->d[ANGLE];
  if (g->d[CARRY] >= 0) {
    ink(c, GColorCyan);
    line(c, x, y, g->d[BEAM_X] / 100, g->d[BEAM_Y] / 100);
    ink(c, GColorYellow);
    box(c, g->d[BEAM_X] / 100 - 3, g->d[BEAM_Y] / 100 - 3, 6, 6, false);
  }
  ink(c, GColorCyan);
  line(c, x + cx[a] * 7 / 1000, y + cy[a] * 7 / 1000,
       x + cx[(a + 6) % 16] * 5 / 1000, y + cy[(a + 6) % 16] * 5 / 1000);
  line(c, x + cx[a] * 7 / 1000, y + cy[a] * 7 / 1000,
       x + cx[(a + 10) % 16] * 5 / 1000, y + cy[(a + 10) % 16] * 5 / 1000);
  line(c, x + cx[(a + 6) % 16] * 5 / 1000, y + cy[(a + 6) % 16] * 5 / 1000,
       x + cx[(a + 10) % 16] * 5 / 1000, y + cy[(a + 10) % 16] * 5 / 1000);
  if (g->d[SHIELD])
    circle(c, x, y, 10, false);
  if (g->d[AUTO]) {
    ink(c, GColorWhite);
    circle(c, g->d[TARGET_X] / 100, g->d[TARGET_Y] / 100, 3, false);
  }
  if (g->d[FX]) {
    ink(c, GColorYellow);
    circle(c, 20, 72, 16 + (16 - g->d[FX]) * 2, false);
  }
}
