#include "draw.h"
#include "model.h"
#include "trig.h"
#include <stdio.h>
#ifdef PBL_BW
// Black-and-white screens cannot show the exit's colour change, so a closed
// exit is a dotted ring and an open one is a solid double ring.
static void dotted_ring(GContext *c, int x, int y, int r) {
  for (int i = 0; i < 16; i++) {
    int a = i * TRIG_MAX_ANGLE / 16;
    box(c, x + cos_lookup(a) * r / TRIG_MAX_RATIO, y + sin_lookup(a) * r / TRIG_MAX_RATIO,
        2, 2, true);
  }
}
#endif
void game_draw(const Game *g, GContext *c) {
  for (int i = 0; i < 50; i++) {
    if (PBL_IF_BW_ELSE(i % 4, 0))
      continue;
    ink(c, i % 4 ? GColorDarkGray : GColorWhite);
    int x = (i * 71 + 13) % 176, y = (i * 37 + 9) % 144;
    if (g->d[FX])
      x += (88 - x) * (16 - g->d[FX]) / 200;
    box(c, x, y, 1, 1, true);
  }
  ink(c, PBL_IF_COLOR_ELSE(GColorCyan, GColorWhite));
  circle(c, 20, 72, 15, false);
  label(c, "DOCK", 0, 82, 42);
#ifdef PBL_COLOR
  ink(c, g->d[BANKED] == 5 ? GColorYellow : GColorLightGray);
  circle(c, 20, 18, 12, false);
#else
  ink(c, GColorWhite);
  if (g->d[BANKED] == 5) {
    circle(c, 20, 18, 12, false);
    circle(c, 20, 18, 9, false);
  } else
    dotted_ring(c, 20, 18, 12);
#endif
  label(c, "EXIT", 0, 25, 40);
  for (int i = 0; i < 5; i++) {
    int b = CARGO + i * 3;
    if (!g->d[b + 2]) {
      ink(c, PBL_IF_COLOR_ELSE(GColorYellow, GColorWhite));
      box(c, g->d[b] / 100 - 4, g->d[b + 1] / 100 - 4, 8, 8, false);
    }
  }
  int n = 3 + (4500 - g->d[DEADLINE]) / 1500;
  if (n > 6)
    n = 6;
  for (int i = 0; i < n; i++) {
    int x = g->d[ROCKS + i * 4] / 100, y = g->d[ROCKS + i * 4 + 1] / 100;
    ink(c, PBL_IF_COLOR_ELSE(GColorLightGray, GColorWhite));
    circle(c, x, y, 7, true);
    ink(c, GColorBlack);
    box(c, x - 5, y - 2, 3, 3, true);
    box(c, x + 2, y + 2, 2, 2, true);
  }
  int x = g->d[X] / 100, y = g->d[Y] / 100, a = g->d[ANGLE];
  if (g->d[CARRY] >= 0) {
    ink(c, PBL_IF_COLOR_ELSE(GColorCyan, GColorWhite));
    line(c, x, y, g->d[BEAM_X] / 100, g->d[BEAM_Y] / 100);
    ink(c, PBL_IF_COLOR_ELSE(GColorYellow, GColorWhite));
    box(c, g->d[BEAM_X] / 100 - 3, g->d[BEAM_Y] / 100 - 3, 6, 6, false);
  }
  ink(c, PBL_IF_COLOR_ELSE(GColorCyan, GColorWhite));
  graphics_context_set_stroke_width(c, 2);
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
    ink(c, PBL_IF_COLOR_ELSE(GColorYellow, GColorWhite));
    circle(c, 20, 72, 16 + (16 - g->d[FX]) * 2, false);
  }
  graphics_context_set_stroke_width(c, 1);
}
void game_hud_compact(const Game *g, char *a, size_t n, char *b, size_t m) {
  snprintf(a, n, PBL_IF_ROUND_ELSE("HP%ld  %ld/5  %lds", "HP%ld %ld/5 crates %lds"),
           (long)g->d[HULL], (long)g->d[BANKED], (long)g->d[DEADLINE] / 30);
  snprintf(b, m, g->d[BANKED] == 5  ? "Fly to EXIT"
                 : g->d[CARRY] >= 0 ? "Return to DOCK"
                                    : "SELECT: thrust");
}
