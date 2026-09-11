#include "draw.h"
#include "model.h"
void game_draw(const Game *g, GContext *c) {
  for (int y = 0; y < 5; y++)
    for (int x = 0; x < 5; x++) {
      ink(c, (x + y) % 2 ? GColorOxfordBlue : GColorBlack);
      box(c, 18 + x * 28, 2 + y * 28, 27, 27, true);
      ink(c, GColorLightGray);
      box(c, 18 + x * 28, 2 + y * 28, 27, 27, false);
    }
  for (int i = 0; i < 2; i++) {
    int x = 18 + g->d[HAZARDS + i * 2] * 28,
        y = 2 + g->d[HAZARDS + i * 2 + 1] * 28;
    ink(c, GColorOrange);
    line(c, x + 5, y + 5, x + 21, y + 21);
    line(c, x + 21, y + 5, x + 5, y + 21);
  }
  int bx = 18 + g->d[BEACON_X] * 28, by = 2 + g->d[BEACON_Y] * 28;
  ink(c, GColorYellow);
  circle(c, bx + 14, by + 14, 9, false);
  large_label(c, "B", bx, by - 1, 28);
  for (int i = 0; i < 3; i++) {
    int b = INTENTS + i * 3;
    if (g->d[ENEMIES + i * 3 + 2] && g->d[b + 2]) {
      int x = 18 + g->d[b] * 28, y = 2 + g->d[b + 1] * 28;
      ink(c, GColorOrange);
      graphics_context_set_stroke_width(c, 2);
      box(c, x + 2, y + 2, 23, 23, false);
      label(c, "!", x + 17, y - 3, 10);
      graphics_context_set_stroke_width(c, 1);
    }
  }
  for (int i = 0; i < 3; i++)
    if (g->d[ENEMIES + i * 3 + 2]) {
      int x = 18 + g->d[ENEMIES + i * 3] * 28,
          y = 2 + g->d[ENEMIES + i * 3 + 1] * 28;
      ink(c, GColorDarkCandyAppleRed);
      box(c, x + 5, y + 5, 18, 18, true);
      ink(c, GColorWhite);
      large_label(c, i == 0 ? "G" : i == 1 ? "L" : "S", x, y - 1, 28);
    }
  int px = 18 + g->d[PX] * 28, py = 2 + g->d[PY] * 28;
  ink(c, GColorCyan);
  circle(c, px + 14, py + 14, 9, false);
  circle(c, px + 14, py + 14, 7, false);
  large_label(c, "@", px, py - 1, 28);
  static const int dx[] = {0, 1, 0, -1, 0}, dy[] = {-1, 0, 1, 0, 0};
  int tx = g->d[PX] + dx[g->d[CHOICE]], ty = g->d[PY] + dy[g->d[CHOICE]];
  if (tx >= 0 && tx < 5 && ty >= 0 && ty < 5) {
    ink(c, GColorWhite);
    box(c, 18 + tx * 28, 2 + ty * 28, 27, 27, false);
  }
}
