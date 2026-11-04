#include "draw.h"
#include "model.h"
#include <stdio.h>
// Board letters: Gothic 24 in the full-size cells of Time 2 and Round 2,
// Gothic 18 in the smaller cells of 144x168 and the Time Round.
static void piece(GContext *c, const char *s, int x, int y) {
#if UI_COMPACT
  label(c, s, x, y - 1, 28);
#else
  large_label(c, s, x, y - 1, 28);
#endif
}
#if UI_COMPACT
// A frame of filled strips, so it stays t world units thick at any scale.
static void frame(GContext *c, int x, int y, int w, int h, int t) {
  box(c, x, y, w, t, true);
  box(c, x, y + h - t, w, t, true);
  box(c, x, y, t, h, true);
  box(c, x + w - t, y, t, h, true);
}
#endif
#ifdef PBL_BW
// Black and white: a threatened cell gets a dotted inner frame (and its "!"),
// distinct from the solid frame of the move target.
static void dotted(GContext *c, int x, int y, int w) {
  for (int i = 0; i < w; i += 5) {
    box(c, x + i, y, 2, 2, true);
    box(c, x + i, y + w - 2, 2, 2, true);
    box(c, x, y + i, 2, 2, true);
    box(c, x + w - 2, y + i, 2, 2, true);
  }
}
#endif
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
#ifdef PBL_BW
    // Orange draws black on a 1-bit screen; pits become a heavy white X.
    ink(c, GColorWhite);
    graphics_context_set_stroke_width(c, 3);
    line(c, x + 6, y + 6, x + 20, y + 20);
    line(c, x + 20, y + 6, x + 6, y + 20);
    graphics_context_set_stroke_width(c, 1);
#else
    ink(c, GColorOrange);
    line(c, x + 5, y + 5, x + 21, y + 21);
    line(c, x + 21, y + 5, x + 5, y + 21);
#endif
  }
  int bx = 18 + g->d[BEACON_X] * 28, by = 2 + g->d[BEACON_Y] * 28;
  ink(c, PBL_IF_COLOR_ELSE(GColorYellow, GColorWhite));
  circle(c, bx + 14, by + 14, 9, false);
  piece(c, "B", bx, by);
  for (int i = 0; i < 3; i++) {
    int b = INTENTS + i * 3;
    if (g->d[ENEMIES + i * 3 + 2] && g->d[b + 2]) {
      int x = 18 + g->d[b] * 28, y = 2 + g->d[b + 1] * 28;
#ifdef PBL_BW
      ink(c, GColorWhite);
      dotted(c, x + 3, y + 3, 21);
      ink(c, GColorBlack); // A notch in the frame keeps the "!" readable.
      box(c, x + 19, y + 1, 7, 17, true);
      ink(c, GColorWhite);
      label(c, "!", x + 17, y - 3, 10);
#else
      ink(c, GColorOrange);
      graphics_context_set_stroke_width(c, 2);
      box(c, x + 2, y + 2, 23, 23, false);
      label(c, "!", x + 17, y - 3, 10);
      graphics_context_set_stroke_width(c, 1);
#endif
    }
  }
  for (int i = 0; i < 3; i++)
    if (g->d[ENEMIES + i * 3 + 2]) {
      int x = 18 + g->d[ENEMIES + i * 3] * 28,
          y = 2 + g->d[ENEMIES + i * 3 + 1] * 28;
      // Foes are filled tiles: red with a white letter, or on black and white
      // a white tile with a black letter, unlike the outlined beacon and player.
      ink(c, PBL_IF_COLOR_ELSE(GColorDarkCandyAppleRed, GColorWhite));
      box(c, x + 5, y + 5, 18, 18, true);
      ink(c, PBL_IF_COLOR_ELSE(GColorWhite, GColorBlack));
      piece(c, i == 0 ? "G" : i == 1 ? "L" : "S", x, y);
    }
  int px = 18 + g->d[PX] * 28, py = 2 + g->d[PY] * 28;
  ink(c, PBL_IF_COLOR_ELSE(GColorCyan, GColorWhite));
  circle(c, px + 14, py + 14, 9, false);
  circle(c, px + 14, py + 14, 7, false);
  piece(c, "@", px, py);
  static const int dx[] = {0, 1, 0, -1, 0}, dy[] = {-1, 0, 1, 0, 0};
  int tx = g->d[PX] + dx[g->d[CHOICE]], ty = g->d[PY] + dy[g->d[CHOICE]];
  if (tx >= 0 && tx < 5 && ty >= 0 && ty < 5) {
    ink(c, GColorWhite);
#if UI_COMPACT
    // The smaller grid's 1 px outline would hide the target, so it is a
    // heavy frame here (the grid itself is white on black and white).
    frame(c, 18 + tx * 28, 2 + ty * 28, 28, 28, 3);
#else
    box(c, 18 + tx * 28, 2 + ty * 28, 27, 27, false);
#endif
  }
}
void game_hud_compact(const Game *g, char *a, size_t n, char *b, size_t m) {
#ifdef PBL_ROUND
  // The Time Round's chords are short above and below the board: B is the
  // beacon, as on the board, and the direction stands alone.
  static const char *names[] = {"UP", "RIGHT", "DOWN", "LEFT", "WAIT"};
  snprintf(a, n, "HP %ld  B %ld  %ld/5", (long)g->d[HEALTH],
           (long)g->d[INTEGRITY], (long)g->d[ROUND]);
  snprintf(b, m, "%s", names[g->d[CHOICE]]);
#else
  game_hud(g, a, n, b, m);
#endif
}
