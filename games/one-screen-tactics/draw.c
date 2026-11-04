#include "draw.h"
#include "model.h"
#include <stdio.h>
#if !UI_COMPACT
// Time 2 and Round 2: the published 0.3.1 board, unchanged.
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
#else
// Pebble Time, Pebble 2, Pebble 2 Duo and the Time Round draw the board in
// screen pixels so every cell is the same size. 144x168: 24 px cells, a
// 121 px board between the two caption rows. Time Round: 22 px cells,
// centred, corners 12 px inside the rim, captions in the caps above and below.
#ifdef PBL_ROUND
#define CELL 22
#define BOARD_X 35
#define BOARD_Y 35
#else
#define CELL 24
#define BOARD_X 11
#define BOARD_Y 24
#endif
#define SIDE (5 * CELL)
static const int dx[] = {0, 1, 0, -1, 0}, dy[] = {-1, 0, 1, 0, 0};
static int left(int x) { return BOARD_X + x * CELL; }
static int top(int y) { return BOARD_Y + y * CELL; }
static GPoint middle(int x, int y) { return GPoint(left(x) + CELL / 2, top(y) + CELL / 2); }
static void outline(GContext *c, int x, int y, int inset) {
  graphics_draw_rect(c, GRect(left(x) + inset, top(y) + inset, CELL + 1 - 2 * inset,
                              CELL + 1 - 2 * inset));
}
// One Gothic 24 letter, centred on the cell.
static void glyph(GContext *c, const char *s, int x, int y, GColor color) {
  graphics_context_set_text_color(c, color);
  graphics_draw_text(c, s, fonts_get_system_font(UI_FONT_BODY),
                     GRect(left(x), top(y) + CELL / 2 - 17, CELL + 1, 28),
                     GTextOverflowModeFill, GTextAlignmentCenter, NULL);
}
static bool threatened(const Game *g, int x, int y) {
  for (int i = 0; i < 3; i++) {
    int b = INTENTS + i * 3;
    if (g->d[ENEMIES + i * 3 + 2] && g->d[b + 2] && g->d[b] == x && g->d[b + 1] == y)
      return true;
  }
  return false;
}
// A black disc inside the ring keeps a piece readable on a shaded
// (threatened) cell; the shading shows around it.
static void backing(GContext *c, const Game *g, int x, int y, int r) {
#ifdef PBL_BW
  if (threatened(g, x, y)) {
    ink(c, GColorBlack);
    graphics_fill_circle(c, middle(x, y), r);
  }
#else
  (void)c; (void)g; (void)x; (void)y; (void)r;
#endif
}
void game_draw(const Game *g, GContext *c) {
  // Cells. Colour: the Time 2 checker and grey grid. Black and white: black
  // cells, a dotted grid and a solid frame, so a solid outline means "chosen".
  for (int y = 0; y < 5; y++)
    for (int x = 0; x < 5; x++) {
#ifdef PBL_COLOR
      ink(c, (x + y) % 2 ? GColorOxfordBlue : GColorBlack);
      graphics_fill_rect(c, GRect(left(x), top(y), CELL + 1, CELL + 1), 0, GCornerNone);
      ink(c, GColorLightGray);
      outline(c, x, y, 0);
#endif
    }
#ifdef PBL_BW
  ink(c, GColorWhite);
  for (int i = 1; i < 5; i++)
    for (int k = 0; k <= SIDE; k += 2) {
      graphics_draw_pixel(c, GPoint(BOARD_X + i * CELL, BOARD_Y + k));
      graphics_draw_pixel(c, GPoint(BOARD_X + k, BOARD_Y + i * CELL));
    }
  graphics_draw_rect(c, GRect(BOARD_X, BOARD_Y, SIDE + 1, SIDE + 1));
#endif
  // Pits: a heavy X.
  ink(c, PBL_IF_COLOR_ELSE(GColorOrange, GColorWhite));
  graphics_context_set_stroke_width(c, 2);
  for (int i = 0; i < 2; i++) {
    int x = left(g->d[HAZARDS + i * 2]), y = top(g->d[HAZARDS + i * 2 + 1]), e = CELL - 5;
    graphics_draw_line(c, GPoint(x + 5, y + 5), GPoint(x + e, y + e));
    graphics_draw_line(c, GPoint(x + e, y + 5), GPoint(x + 5, y + e));
  }
  graphics_context_set_stroke_width(c, 1);
#ifdef PBL_BW
  // Attacks due after your action: the cell is shaded (a 50% dither).
  ink(c, GColorWhite);
  for (int y = 0; y < 5; y++)
    for (int x = 0; x < 5; x++)
      if (threatened(g, x, y))
        for (int i = 2; i < CELL - 1; i++)
          for (int j = 2; j < CELL - 1; j++)
            if ((i + j) % 2 == 0)
              graphics_draw_pixel(c, GPoint(left(x) + i, top(y) + j));
#endif
  int bx = g->d[BEACON_X], by = g->d[BEACON_Y];
  backing(c, g, bx, by, 8);
  ink(c, PBL_IF_COLOR_ELSE(GColorYellow, GColorWhite));
  graphics_draw_circle(c, middle(bx, by), 8);
  glyph(c, "B", bx, by, PBL_IF_COLOR_ELSE(GColorYellow, GColorWhite));
#ifdef PBL_COLOR
  // Attacks due after your action: the Time 2 orange outline.
  ink(c, GColorOrange);
  graphics_context_set_stroke_width(c, 2);
  for (int y = 0; y < 5; y++)
    for (int x = 0; x < 5; x++)
      if (threatened(g, x, y))
        outline(c, x, y, 2);
  graphics_context_set_stroke_width(c, 1);
#endif
  // Foes: solid squares with their initial (red on colour, white on black and white).
  for (int i = 0; i < 3; i++)
    if (g->d[ENEMIES + i * 3 + 2]) {
      int x = g->d[ENEMIES + i * 3], y = g->d[ENEMIES + i * 3 + 1];
      ink(c, PBL_IF_COLOR_ELSE(GColorDarkCandyAppleRed, GColorWhite));
      graphics_fill_rect(c, GRect(left(x) + 4, top(y) + 4, CELL - 7, CELL - 7), 0, GCornerNone);
      glyph(c, i == 0 ? "G" : i == 1 ? "L" : "S", x, y,
            PBL_IF_COLOR_ELSE(GColorWhite, GColorBlack));
    }
  int px = g->d[PX], py = g->d[PY];
  backing(c, g, px, py, 9);
  ink(c, PBL_IF_COLOR_ELSE(GColorCyan, GColorWhite));
  graphics_draw_circle(c, middle(px, py), 9);
  graphics_draw_circle(c, middle(px, py), 7);
  glyph(c, "@", px, py, PBL_IF_COLOR_ELSE(GColorCyan, GColorWhite));
  // "!" in the top-right corner of each attacked cell, over a black tab.
  for (int y = 0; y < 5; y++)
    for (int x = 0; x < 5; x++)
      if (threatened(g, x, y)) {
        GRect tab = GRect(left(x) + CELL - 8, top(y) + 2, 6, 13);
        ink(c, GColorBlack);
        graphics_fill_rect(c, tab, 0, GCornerNone);
        graphics_context_set_text_color(c, PBL_IF_COLOR_ELSE(GColorOrange, GColorWhite));
        graphics_draw_text(c, "!", fonts_get_system_font(UI_FONT_SMALL),
                           GRect(tab.origin.x - 4, tab.origin.y - 5, 14, 20),
                           GTextOverflowModeFill, GTextAlignmentCenter, NULL);
      }
  // The chosen move: a solid white outline (two pixels on black and white).
  int tx = px + dx[g->d[CHOICE]], ty = py + dy[g->d[CHOICE]];
  if (tx >= 0 && tx < 5 && ty >= 0 && ty < 5) {
    ink(c, GColorWhite);
    outline(c, tx, ty, 0);
#ifdef PBL_BW
    outline(c, tx, ty, 1);
#endif
  }
}
void game_hud_compact(const Game *g, char *a, size_t n, char *b, size_t m) {
  snprintf(a, n, PBL_IF_ROUND_ELSE("HP %ld  B %ld  %ld/5", "HP %ld  Beacon %ld  %ld/5"),
           (long)g->d[HEALTH], (long)g->d[INTEGRITY], (long)g->d[ROUND]);
  static const char *names[] = {"UP", "RIGHT", "DOWN", "LEFT", "WAIT"};
  // The Time Round's bottom chord is shorter: "SELECT: RIGHT".
  snprintf(b, m, PBL_IF_ROUND_ELSE("SELECT: %s", "%s  SELECT: act"), names[g->d[CHOICE]]);
}
#endif
// The compact result card (144x168 and Time Round) is 132 px wide;
// "Beacon destroyed" does not fit it.
void game_result_compact(const Game *g, char *title, size_t n, char *detail, size_t m) {
  game_result(g, title, n, detail, m);
  snprintf(title, n, "%s", g->status == 1         ? "Beacon held"
                           : g->d[INTEGRITY] <= 0 ? "Beacon lost"
                                                  : "Defender lost");
}
