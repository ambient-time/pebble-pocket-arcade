#include "draw.h"
#include "model.h"
extern const int train_paths[9][4];
void game_draw(const Game *g, GContext *c) {
  for (int i = 0; i < 9; i++) {
    ink(c, GColorLightGray);
    line(c, train_paths[i][0], train_paths[i][1], train_paths[i][2],
         train_paths[i][3]);
  }
  for (int j = 0; j < 3; j++) {
    int path = j == 0   ? (g->d[SW0] ? 1 : 2)
               : j == 1 ? (g->d[SW1] ? 3 : 4)
                        : (g->d[SW2] ? 6 : 5);
    ink(c, GColorWhite);
    graphics_context_set_stroke_width(c, 2);
    line(c, train_paths[path][0], train_paths[path][1], train_paths[path][2],
         train_paths[path][3]);
    graphics_context_set_stroke_width(c, 1);
    ink(c, train_locked(g, j) ? GColorOrange : GColorCyan);
    circle(c, 48 + j * 48, 72, g->d[FOCUS] == j ? 12 : 8, false);
    char b[] = {'1' + j, 0};
    label(c, b, 38 + j * 48, 61, 20);
  }
  const int sx[] = {48, 96, 166}, sy[] = {12, 132, 26};
  GColor colors[] = {GColorCyan, GColorYellow, GColorMagenta};
  for (int i = 0; i < 3; i++) {
    ink(c, colors[i]);
    box(c, sx[i] - 9, sy[i] - 9, 18, 18, true);
    ink(c, GColorBlack);
    char b[] = {'A' + i, 0};
    label(c, b, sx[i] - 8, sy[i] - 11, 16);
  }
  for (int i = 0; i < 8; i++)
    if (g->d[TRAINS + i * TRAIN_WORDS]) {
      int x, y, k = g->d[TRAINS + i * TRAIN_WORDS + 3];
      train_position(g, i, &x, &y);
      ink(c, GColorBlack);
      box(c, x - 7, y - 8, 14, 16, true);
      ink(c, colors[k]);
      if (k == 0)
        box(c, x - 7, y - 8, 14, 16, false);
      else if (k == 1)
        circle(c, x, y, 8, false);
      else {
        line(c, x, y - 9, x - 8, y + 8);
        line(c, x - 8, y + 8, x + 8, y + 8);
        line(c, x + 8, y + 8, x, y - 9);
      }
      char b[] = {'A' + k, 0};
      label(c, b, x - 7, y - 10, 14);
    }
}
