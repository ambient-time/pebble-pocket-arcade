#include "draw.h"
#include "model.h"
#include <stdio.h>
// Time 2 and Round 2 keep the 0.3.1 drawing. The four compact watches (144x168
// and the Time Round, no touch) draw the same habitat smaller: sprouts are
// small filled bars, each animal sits on a black disc so it stands clear of the
// plants, the cursor is a double outline and the tool row shows an icon and
// its cost, since the tool names do not fit a 35 px tile. On black-and-white
// screens grazers are white discs with an eye, hunters white triangles with a
// dark center, and winter plants lose their leaves.
#if UI_COMPACT
// Apex-up filled triangle in field units, drawn as scanlines.
static void fill_triangle(GContext *c, int x, int top, int half, int height) {
  for (int r = 0; r <= height; r++) {
    int w = r * half / height;
    line(c, x - w, top + r, x + w, top + r);
  }
}
static void grazer(GContext *c, int x, int y, int r, GColor body, GColor eye) {
  ink(c, body);
  circle(c, x, y, r, true);
  ink(c, eye);
  circle(c, x + r / 3, y - r / 3, 1, true);
}
static void hunter(GContext *c, int x, int y, GColor body, GColor center) {
  ink(c, body);
#ifdef PBL_BW
  fill_triangle(c, x, y - 8, 8, 14);
#else
  line(c, x, y - 8, x - 8, y + 6);
  line(c, x - 8, y + 6, x + 8, y + 6);
  line(c, x + 8, y + 6, x, y - 8);
#endif
  ink(c, center);
  circle(c, x, y + PBL_IF_BW_ELSE(1, 0), 2, true);
}
static void tool_icon(GContext *c, int tool, int x, int y, bool on) {
  GColor dark = GColorBlack;
  if (tool == 0) {
    ink(c, on ? dark : PBL_IF_COLOR_ELSE(GColorGreen, GColorWhite));
    for (int k = -1; k <= 1; k += 2) { // Two sprouts, as in the habitat.
      box(c, x + 4 * k - 1, y - 4, 3, 11, true);
      box(c, x + 4 * k, y - 5, 5, 3, true);
    }
  } else if (tool == 1) {
    grazer(c, x, y, 6, on ? dark : PBL_IF_COLOR_ELSE(GColorYellow, GColorWhite),
           on ? GColorWhite : dark);
  } else if (tool == 2) {
    hunter(c, x, y, on ? dark : PBL_IF_COLOR_ELSE(GColorOrange, GColorWhite),
           on ? PBL_IF_COLOR_ELSE(dark, GColorWhite) : PBL_IF_COLOR_ELSE(GColorWhite, dark));
  } else {
    ink(c, on ? dark : PBL_IF_COLOR_ELSE(GColorCyan, GColorWhite));
    circle(c, x, y + 2, 5, true);
    fill_triangle(c, x, y - 8, 4, 8);
  }
}
#endif
void game_draw(const Game *g, GContext *c) {
  for (int i = 0; i < 24; i++) {
    int x = 4 + i % 6 * 28, y = 1 + i / 6 * 28;
    ink(c, GColorBlack);
    box(c, x, y, 27, 27, true);
#if UI_COMPACT
    // Filled bars instead of 2 px strokes: cheaper to draw every frame.
    ink(c, PBL_IF_COLOR_ELSE(g->stage == 3 ? GColorLightGray : GColorGreen, GColorWhite));
    for (int k = 0; k < g->d[PLANTS + i]; k++) {
      int px = x + 4 + (k % 3) * 8, py = y + 8 + (k / 3) * 12;
      box(c, px - 1, py, 2, 6, true);
      if (PBL_IF_BW_ELSE(g->stage != 3, true))
        box(c, px, py - 1, 4, 2, true);
    }
#else
    ink(c, g->stage == 3 ? GColorLightGray : GColorGreen);
    graphics_context_set_stroke_width(c, 2);
    for (int k = 0; k < g->d[PLANTS + i]; k++) {
      int px = x + 4 + (k % 3) * 8, py = y + 8 + (k / 3) * 12;
      line(c, px, py + 5, px, py);
      line(c, px, py + 2, px + 3, py - 1);
    }
    graphics_context_set_stroke_width(c, 1);
#endif
  }
  for (int i = 0; i < SLOTS; i++) {
    int b = ANIMALS + i * 5;
    if (!g->d[b])
      continue;
    int x = 4 + g->d[b + 1] % 6 * 28 + 14, y = 1 + g->d[b + 1] / 6 * 28 + 14;
#if UI_COMPACT
    ink(c, GColorBlack);
    circle(c, x, y, g->d[b] == 1 ? 9 : 10, true);
    if (g->d[b] == 1)
      grazer(c, x, y, 6, PBL_IF_COLOR_ELSE(GColorYellow, GColorWhite), GColorBlack);
    else
      hunter(c, x, y, PBL_IF_COLOR_ELSE(GColorOrange, GColorWhite),
             PBL_IF_COLOR_ELSE(GColorWhite, GColorBlack));
#else
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
#endif
  }
  ink(c, GColorWhite);
  box(c, 4 + g->d[CURSOR] % 6 * 28, 1 + g->d[CURSOR] / 6 * 28, 27, 27, false);
#if UI_COMPACT
  box(c, 6 + g->d[CURSOR] % 6 * 28, 3 + g->d[CURSOR] / 6 * 28, 23, 23, false);
  static const char *const costs[] = {"2", "3", "5", "3"};
  for (int i = 0; i < 4; i++) {
    bool on = g->d[TOOL] == i;
    int x = 4 + i * 42;
    ink(c, on ? GColorWhite : PBL_IF_COLOR_ELSE(GColorLightGray, GColorWhite));
    box(c, x, 116, 41, 27, on);
    tool_icon(c, i, x + 13, 129, on);
    ink(c, on ? GColorBlack : GColorWhite);
    label(c, costs[i], x + 23, 116, 14);
  }
#else
  const char *names[] = {"Leaf", "Graze", "Hunt", "Rain"};
  for (int i = 0; i < 4; i++) {
    ink(c, g->d[TOOL] == i ? GColorWhite : GColorLightGray);
    box(c, i * 44, 116, 43, 27, g->d[TOOL] == i);
    ink(c, g->d[TOOL] == i ? GColorBlack : GColorWhite);
    label(c, names[i], i * 44, 117, 43);
  }
#endif
  if (g->status == 1) {
    ink(c, GColorCyan);
    line(c, 0, 114, 176, 114);
  }
}
#if UI_COMPACT
// Compact readouts: the selected tile already shows the tool and its cost.
void game_hud_compact(const Game *g, char *a, size_t n, char *b, size_t m) {
  int pl, h, p;
  ecosystem_counts(g, &pl, &h, &p);
  // The Time Round's top chord is about 120 px: seasons shorten to three letters.
  static const char *const seasons[] = {
      PBL_IF_ROUND_ELSE("Spr", "Spring"), PBL_IF_ROUND_ELSE("Sum", "Summer"),
      PBL_IF_ROUND_ELSE("Aut", "Autumn"), PBL_IF_ROUND_ELSE("Win", "Winter")};
  snprintf(a, n, "%s L%d G%d P%d", seasons[g->stage], pl, h, p);
  long budget = (long)g->d[BUDGET], water = (long)g->d[WATER];
  if (g->status == 2)
    snprintf(b, m, !h ? "Grazers extinct" : "Hunters extinct");
  else if (!p)
    snprintf(b, m, "Add hunter  $%ld", budget);
#ifdef PBL_ROUND
  // The bottom chord is about 114 px, so these lines stay at 15 characters.
  else if (h < 3)
    snprintf(b, m, "Grazers %d/3 $%ld", h, budget);
  else if (g->status == 1)
    snprintf(b, m, "Balanced W%ld", water);
  else
    snprintf(b, m, "$%ld W%ld %ld/60", budget, water, (long)g->d[BALANCE]);
#else
  else if (h < 3)
    snprintf(b, m, "Add %d grazer%s $%ld", 3 - h, 3 - h == 1 ? "" : "s", budget);
  else if (g->status == 1)
    snprintf(b, m, "Balanced W%ld $%ld", water, budget);
  else
    snprintf(b, m, "$%ld  W%ld  %ld/60", budget, water, (long)g->d[BALANCE]);
#endif
}
#endif
