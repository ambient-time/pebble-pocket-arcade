#include "draw.h"
#include "model.h"
#include <stdio.h>
#include <stdlib.h>
extern const int train_paths[9][4];
// Compact screens (144x168 and the Time Round, about 80% scale): larger
// junction rings so the digit stays inside, and letters lifted so they sit in
// the middle of their station or train. Time 2 and Round 2 are unchanged.
#if UI_COMPACT
#define RING_SMALL 10
#define RING_FOCUS 14
#define DIGIT_Y 58
#define STATION_LIFT 15
#else
#define RING_SMALL 8
#define RING_FOCUS 12
#define DIGIT_Y 61
#define STATION_LIFT 11
#endif
#ifdef PBL_BW
// Black-and-white screens: the route a junction is not set to is dotted, the
// set route is solid and thick; a locked junction gets a thick ring drawn over
// the train that holds it; stations are white with black letters and trains
// are white outlines (square A, circle B, triangle C) with white letters.
static void dotted(GContext *c, int x0, int y0, int x1, int y1) {
  int ax = field_x + x0 * UI_FIELD_SCALE / 100, ay = field_y + y0 * UI_FIELD_SCALE / 100,
      bx = field_x + x1 * UI_FIELD_SCALE / 100, by = field_y + y1 * UI_FIELD_SCALE / 100;
  int dx = bx - ax, dy = by - ay, n = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
  if (!n) return;
  for (int i = 0; i <= n; i += 3)
    graphics_draw_pixel(c, GPoint(ax + dx * i / n, ay + dy * i / n));
}
#endif
#if UI_COMPACT
// A train's letter, centred on the train in screen pixels. The shell's label()
// widens short labels and clamps them at the left edge, which pushed the letter
// off a train entering at the far left.
static void train_letter(GContext *c, char ch, int x, int y) {
  char b[] = {ch, 0};
  int cx = field_x + x * UI_FIELD_SCALE / 100, cy = field_y + y * UI_FIELD_SCALE / 100;
  graphics_draw_text(c, b, fonts_get_system_font(UI_FONT_SMALL), GRect(cx - 12, cy - 12, 24, 24),
                     GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
}
#endif
static int route(const Game *g, int j) {
  return j == 0 ? (g->d[SW0] ? 1 : 2) : j == 1 ? (g->d[SW1] ? 3 : 4) : (g->d[SW2] ? 6 : 5);
}
void game_draw(const Game *g, GContext *c) {
#ifdef PBL_BW
  ink(c, GColorWhite);
  for (int i = 0; i < 9; i++) {
    bool branch = i >= 1 && i <= 6;
    bool unset = branch && route(g, 0) != i && route(g, 1) != i && route(g, 2) != i;
    if (unset)
      dotted(c, train_paths[i][0], train_paths[i][1], train_paths[i][2], train_paths[i][3]);
    else
      line(c, train_paths[i][0], train_paths[i][1], train_paths[i][2], train_paths[i][3]);
  }
#else
  for (int i = 0; i < 9; i++) {
    ink(c, GColorLightGray);
    line(c, train_paths[i][0], train_paths[i][1], train_paths[i][2],
         train_paths[i][3]);
  }
#endif
  for (int j = 0; j < 3; j++) {
    int path = route(g, j);
    ink(c, GColorWhite);
    graphics_context_set_stroke_width(c, 2);
    line(c, train_paths[path][0], train_paths[path][1], train_paths[path][2],
         train_paths[path][3]);
    graphics_context_set_stroke_width(c, 1);
    ink(c, PBL_IF_COLOR_ELSE(train_locked(g, j) ? GColorOrange : GColorCyan, GColorWhite));
    circle(c, 48 + j * 48, 72, g->d[FOCUS] == j ? RING_FOCUS : RING_SMALL, false);
    char b[] = {'1' + j, 0};
    label(c, b, 38 + j * 48, DIGIT_Y, 20);
  }
  const int sx[] = {48, 96, 166}, sy[] = {12, 132, 26};
  GColor colors[] = {GColorCyan, GColorYellow, GColorMagenta};
  (void)colors; // Unused on black-and-white screens.
  for (int i = 0; i < 3; i++) {
    ink(c, PBL_IF_COLOR_ELSE(colors[i], GColorWhite));
    box(c, sx[i] - 9, sy[i] - 9, 18, 18, true);
    ink(c, GColorBlack);
    char b[] = {'A' + i, 0};
    label(c, b, sx[i] - 8, sy[i] - STATION_LIFT, 16);
  }
  for (int i = 0; i < 8; i++)
    if (g->d[TRAINS + i * TRAIN_WORDS]) {
      int x, y, k = g->d[TRAINS + i * TRAIN_WORDS + 3];
      train_position(g, i, &x, &y);
      ink(c, GColorBlack);
      box(c, x - 7, y - 8, 14, 16, true);
      ink(c, PBL_IF_COLOR_ELSE(colors[k], GColorWhite));
      if (k == 0)
        box(c, x - 7, y - 8, 14, 16, false);
      else if (k == 1)
        circle(c, x, y, 8, false);
      else {
        line(c, x, y - 9, x - 8, y + 8);
        line(c, x - 8, y + 8, x + 8, y + 8);
        line(c, x + 8, y + 8, x, y - 9);
      }
#if UI_COMPACT
      train_letter(c, 'A' + k, x, y);
#else
      char b[] = {'A' + k, 0};
      label(c, b, x - 7, y - 10, 14);
#endif
    }
#ifdef PBL_BW
  graphics_context_set_stroke_width(c, 3);
  ink(c, GColorWhite);
  for (int j = 0; j < 3; j++)
    if (train_locked(g, j))
      circle(c, 48 + j * 48, 72, (g->d[FOCUS] == j ? RING_FOCUS : RING_SMALL) + 3, false);
  graphics_context_set_stroke_width(c, 1);
#endif
}
// 144x168: "Switch 1  Next C", or "1 LOCKED  Next C" while a train holds the
// selected junction ("Switch 1 LOCKED" no longer fits). The Time Round's lower
// chord is about 105 px: "Sw 1  Next C", or "Sw 1 LOCKED".
void game_hud_compact(const Game *g, char *a, size_t n, char *b, size_t m) {
  // The Time Round's top chord is 120 px: "Trains 18/18  X0/3" needs 128.
  snprintf(a, n, PBL_IF_ROUND_ELSE("Trains %ld/18  X%ld", "Trains %ld/18  X%ld/3"),
           (long)g->d[DELIVERED], (long)g->d[STRIKES]);
  long sw = (long)g->d[FOCUS] + 1;
  char next = 'A' + (int)g->d[NEXT];
  bool locked = train_locked(g, g->d[FOCUS]);
#ifdef PBL_ROUND
  if (locked)
    snprintf(b, m, "Sw %ld LOCKED", sw);
  else
    snprintf(b, m, "Sw %ld  Next %c", sw, next);
#else
  snprintf(b, m, locked ? "%ld LOCKED  Next %c" : "Switch %ld  Next %c", sw, next);
#endif
}
