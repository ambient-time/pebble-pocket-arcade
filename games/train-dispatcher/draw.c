#include "draw.h"
#include "model.h"
extern const int train_paths[9][4];
#if !UI_COMPACT
// Pebble Time 2 and Round 2: the published 0.3.1 drawing, unchanged.
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
#else
#include <stdio.h>
#include <stdlib.h>
// Pebble Time, Pebble 2, Pebble 2 Duo and Pebble Time Round. The network is a
// wide rectangle, so it keeps its shape on every screen: full width on
// 144x168, inside the rim on the Time Round. Every state that colour carries
// on Time 2 also has a shape here:
//  - the route a junction sends trains along is a thick line; on
//    black-and-white screens the other branch is dotted;
//  - a free junction is a ring with its number, an occupied (locked) one a
//    filled disc wider than the train on it; the selected junction has a
//    second, heavier ring;
//  - stations and trains share shapes: square A, circle B, triangle C.
//    Stations are filled; trains are outlines with their letter inside.
static int px(int v) { return v * UI_FIELD_SCALE / 100; }
static GPoint at(int x, int y) { return GPoint(field_x + px(x), field_y + px(y)); }
static GColor hue(int kind) {
  return PBL_IF_COLOR_ELSE(kind == 0   ? GColorCyan
                           : kind == 1 ? GColorYellow
                                       : GColorMagenta,
                           GColorWhite);
}
// One capital letter or digit, centred on a world point.
static void glyph(GContext *c, char ch, int x, int y) {
  char s[] = {ch, 0};
  GPoint p = at(x, y);
  graphics_draw_text(c, s, fonts_get_system_font(UI_FONT_SMALL),
                     GRect(p.x - 12, p.y - 10, 24, 22), GTextOverflowModeFill,
                     GTextAlignmentCenter, NULL);
}
static void dotted(GContext *c, const int *p) {
  GPoint a = at(p[0], p[1]), b = at(p[2], p[3]);
  int dx = abs(b.x - a.x), dy = abs(b.y - a.y), n = (dx > dy ? dx : dy) / 4;
  if (n < 1)
    n = 1;
  for (int i = 0; i <= n; i++)
    graphics_fill_rect(c, GRect(a.x + (b.x - a.x) * i / n, a.y + (b.y - a.y) * i / n, 2, 2),
                       0, GCornerNone);
}
// Apex up; r is half the base width, in world units.
static void triangle(GContext *c, int x, int y, int r, bool fill) {
  if (fill)
    for (int dy = -r; dy <= r; dy++) {
      int half = (dy + r) / 2;
      line(c, x - half, y + dy, x + half, y + dy);
    }
  else {
    line(c, x, y - r, x - r, y + r);
    line(c, x - r, y + r, x + r, y + r);
    line(c, x + r, y + r, x, y - r);
  }
}
static void shape(GContext *c, int kind, int x, int y, int r, bool fill) {
  if (kind == 0)
    box(c, x - r, y - r, 2 * r, 2 * r, fill);
  else if (kind == 1)
    circle(c, x, y, r, fill);
  else
    triangle(c, x, y, r, fill);
}
void game_draw(const Game *g, GContext *c) {
  int active[] = {g->d[SW0] ? 1 : 2, g->d[SW1] ? 3 : 4, g->d[SW2] ? 6 : 5};
  for (int i = 0; i < 9; i++) {
    bool branch = i >= 1 && i <= 6, on = false;
    for (int j = 0; j < 3; j++)
      on = on || active[j] == i;
    ink(c, on ? GColorWhite : PBL_IF_COLOR_ELSE(GColorLightGray, GColorWhite));
    graphics_context_set_stroke_width(c, on ? 3 : 1);
    // The C branch is drawn only to the base of C's triangle (drawing only;
    // trains still run to the end of the track).
    int p[] = {train_paths[i][0], train_paths[i][1], i == 5 ? 162 : train_paths[i][2],
               i == 5 ? 41 : train_paths[i][3]};
    if (branch && !on && PBL_IF_BW_ELSE(true, false))
      dotted(c, p);
    else
      line(c, p[0], p[1], p[2], p[3]);
  }
  graphics_context_set_stroke_width(c, 1);
  // Stations sit at the ends of their branches; C's triangle sits a little
  // left of the track end so it stays on a 144-pixel screen, with its letter
  // in the wide lower half.
  const int sx[] = {48, 96, 160}, sy[] = {12, 131, 27};
  for (int i = 0; i < 3; i++) {
    ink(c, hue(i));
    shape(c, i, sx[i], sy[i], i == 2 ? 15 : 11, true);
    ink(c, GColorBlack);
    glyph(c, 'A' + i, sx[i], sy[i] + (i == 2 ? 6 : 0));
  }
  for (int j = 0; j < 3; j++) {
    int x = 48 + j * 48;
    bool locked = train_locked(g, j);
    GColor tone = PBL_IF_COLOR_ELSE(locked ? GColorOrange : GColorCyan, GColorWhite);
    // A locked junction always holds a train, so its disc is wider than a
    // train and shows as a solid band around it.
    ink(c, GColorBlack);
    circle(c, x, 72, 10, true);
    ink(c, tone);
    circle(c, x, 72, locked ? 15 : 10, locked);
    if (g->d[FOCUS] == j) {
      graphics_context_set_stroke_width(c, 2);
      circle(c, x, 72, 19, false);
      graphics_context_set_stroke_width(c, 1);
    }
    ink(c, locked ? GColorBlack : tone);
    glyph(c, '1' + j, x, 72);
  }
  for (int i = 0; i < 8; i++)
    if (g->d[TRAINS + i * TRAIN_WORDS]) {
      int x, y, k = g->d[TRAINS + i * TRAIN_WORDS + 3];
      train_position(g, i, &x, &y);
#ifndef PBL_ROUND
      // 144-pixel screens: a train at either track end (x 3 or 171) would
      // hang off the edge, so it is drawn whole just inside it (drawing only).
      x = x < 11 ? 11 : x > 165 ? 165 : x;
#endif
      ink(c, GColorBlack);
      box(c, x - 9, y - 10, 18, 20, true);
      ink(c, hue(k));
      shape(c, k, x, y, 9, false);
      glyph(c, 'A' + k, x, y + (k == 2 ? 3 : 0));
    }
}
void game_hud_compact(const Game *g, char *a, size_t n, char *b, size_t m) {
#ifdef PBL_ROUND
  // The Time Round's top chord is about 120 px, too short for "Trains 18/18
  // X0/3" once two digits arrive, so the misses move down beside the next
  // train. The selected and locked junctions are marked on the network.
  snprintf(a, n, "Trains %ld/18", (long)g->d[DELIVERED]);
  snprintf(b, m, "Next %c  X%ld/3", 'A' + (int)g->d[NEXT], (long)g->d[STRIKES]);
#else
  snprintf(a, n, "Trains %ld/18  X%ld/3", (long)g->d[DELIVERED], (long)g->d[STRIKES]);
  snprintf(b, m, train_locked(g, g->d[FOCUS]) ? "%ld LOCKED  Next %c" : "Switch %ld  Next %c",
           (long)g->d[FOCUS] + 1, 'A' + (int)g->d[NEXT]);
#endif
}
#endif
