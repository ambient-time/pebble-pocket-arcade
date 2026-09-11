#include "draw.h"
#include "model.h"
void game_draw(const Game *g, GContext *c) {
  for (int i = 0; i < 24; i++) {
    int x = 4 + i % 6 * 28, y = 1 + i / 6 * 28;
    ink(c, GColorBlack);
    box(c, x, y, 27, 27, true);
    ink(c, g->stage == 3 ? GColorLightGray : GColorGreen);
    graphics_context_set_stroke_width(c, 2);
    for (int k = 0; k < g->d[PLANTS + i]; k++) {
      int px = x + 4 + (k % 3) * 8, py = y + 8 + (k / 3) * 12;
      line(c, px, py + 5, px, py);
      line(c, px, py + 2, px + 3, py - 1);
    }
    graphics_context_set_stroke_width(c, 1);
  }
  for (int i = 0; i < SLOTS; i++) {
    int b = ANIMALS + i * 5;
    if (!g->d[b])
      continue;
    int x = 4 + g->d[b + 1] % 6 * 28 + 14, y = 1 + g->d[b + 1] / 6 * 28 + 14;
    if (g->d[b] == 1) {
      ink(c, GColorYellow);
      circle(c, x, y, 6, true);
      ink(c, GColorBlack);
      circle(c, x + 2, y - 2, 1, true);
    } else {
      ink(c, GColorOrange);
      line(c, x, y - 8, x - 8, y + 6);
      line(c, x - 8, y + 6, x + 8, y + 6);
      line(c, x + 8, y + 6, x, y - 8);
      ink(c, GColorWhite);
      circle(c, x, y, 2, true);
    }
  }
  ink(c, GColorWhite);
  box(c, 4 + g->d[CURSOR] % 6 * 28, 1 + g->d[CURSOR] / 6 * 28, 27, 27, false);
  const char *names[] = {"Leaf", "Graze", "Hunt", "Rain"};
  for (int i = 0; i < 4; i++) {
    ink(c, g->d[TOOL] == i ? GColorWhite : GColorLightGray);
    box(c, i * 44, 116, 43, 27, g->d[TOOL] == i);
    ink(c, g->d[TOOL] == i ? GColorBlack : GColorWhite);
    label(c, names[i], i * 44, 117, 43);
  }
  if (g->status == 1) {
    ink(c, GColorCyan);
    line(c, 0, 114, 176, 114);
  }
}
