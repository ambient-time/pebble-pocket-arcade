#include "draw.h"
#include "model.h"
#include <stdio.h>
// Compact screens (144x168 and the Time Round) have no touch, so the tool row
// is a legend rather than buttons: each tool shows the glyph it leaves on the
// board and its cost. Black-and-white screens tell the species apart by shape
// alone: grazers are solid discs, hunters hollow triangles with a centre dot.
static void sprout(GContext *c, int px, int py) {
  line(c, px, py + 5, px, py);
  line(c, px, py + 2, px + 3, py - 1);
}
static void grazer(GContext *c, int x, int y) {
#if UI_COMPACT
  // A slightly larger disc on a black ring, so it keeps its edge among plants.
  ink(c, GColorBlack);
  circle(c, x, y, 10, true);
  ink(c, PBL_IF_COLOR_ELSE(GColorYellow, GColorWhite));
  circle(c, x, y, 7, true);
  ink(c, GColorBlack);
  box(c, x + 1, y - 4, 3, 3, true);
#else
  ink(c, PBL_IF_COLOR_ELSE(GColorYellow, GColorWhite));
  circle(c, x, y, 6, true);
  ink(c, GColorBlack);
  circle(c, x + 2, y - 2, 1, true);
#endif
}
static void triangle(GContext *c, int x, int y) {
  line(c, x, y - 8, x - 8, y + 6);
  line(c, x - 8, y + 6, x + 8, y + 6);
  line(c, x + 8, y + 6, x, y - 8);
}
#if UI_COMPACT
// Screen point for a world point, matching the shared shell's projection.
static GPoint at(int x, int y) {
  return GPoint(field_x + x * UI_FIELD_SCALE / 100, field_y + y * UI_FIELD_SCALE / 100);
}
#endif
static void hunter(GContext *c, int x, int y) {
#if UI_COMPACT
  // A black triangle a little larger than the outline clears the plants
  // inside and around it, so the hunter stays a hollow triangle on every screen.
  GPoint halo[] = {at(x, y - 12), at(x - 12, y + 9), at(x + 12, y + 9)};
  GPath path = {.num_points = 3, .points = halo};
  ink(c, GColorBlack);
  gpath_draw_filled(c, &path);
  graphics_context_set_stroke_width(c, 2);
#endif
  ink(c, PBL_IF_COLOR_ELSE(GColorOrange, GColorWhite));
  triangle(c, x, y);
  graphics_context_set_stroke_width(c, 1);
  ink(c, GColorWhite);
  circle(c, x, y, 2, true);
}
#if UI_COMPACT
static void raindrop(GContext *c, int x, int y) {
  circle(c, x, y + 3, 5, true);
  graphics_context_set_stroke_width(c, 2);
  line(c, x - 4, y + 1, x, y - 7);
  line(c, x + 4, y + 1, x, y - 7);
  graphics_context_set_stroke_width(c, 1);
}
// Tool legend. The Time Round insets it so its corners stay inside the rim.
#define TOOL_X PBL_IF_ROUND_ELSE(8, 0)
#define TOOL_W PBL_IF_ROUND_ELSE(40, 44)
static void tool_row(const Game *g, GContext *c) {
  static const char *const costs[] = {"2", "3", "5", "3"};
  for (int i = 0; i < 4; i++) {
    bool on = g->d[TOOL] == i;
    int x = TOOL_X + i * TOOL_W, gx = x + TOOL_W * 2 / 5, gy = 130;
    ink(c, on ? GColorWhite : PBL_IF_COLOR_ELSE(GColorLightGray, GColorWhite));
    box(c, x, 116, TOOL_W - 1, 27, on);
    // The selected tool is inverted: black glyph on white.
    if (i == 0) {
      ink(c, on ? GColorBlack : PBL_IF_COLOR_ELSE(GColorGreen, GColorWhite));
      graphics_context_set_stroke_width(c, 2);
      sprout(c, gx - 5, gy - 2);
      sprout(c, gx + 2, gy - 1);
      graphics_context_set_stroke_width(c, 1);
    } else if (i == 1) {
      if (on) {
        ink(c, GColorBlack);
        circle(c, gx, gy, 7, true);
      } else
        grazer(c, gx, gy);
    } else if (i == 2) {
      if (on) {
        ink(c, GColorBlack);
        graphics_context_set_stroke_width(c, 2);
        triangle(c, gx, gy);
        graphics_context_set_stroke_width(c, 1);
        circle(c, gx, gy, 2, true);
      } else
        hunter(c, gx, gy);
    } else {
      ink(c, on ? GColorBlack : PBL_IF_COLOR_ELSE(GColorPictonBlue, GColorWhite));
      raindrop(c, gx, gy - 1);
    }
    ink(c, on ? GColorBlack : GColorWhite);
    label(c, costs[i], x + TOOL_W * 3 / 5, 115, TOOL_W * 2 / 5);
  }
}
#endif
void game_draw(const Game *g, GContext *c) {
  for (int i = 0; i < 24; i++) {
    int x = 4 + i % 6 * 28, y = 1 + i / 6 * 28;
    ink(c, GColorBlack);
    box(c, x, y, 27, 27, true);
#ifdef PBL_COLOR
    ink(c, g->stage == 3 ? GColorLightGray : GColorGreen);
    graphics_context_set_stroke_width(c, 2);
#else
    // Winter plants are dormant: thin strokes stand in for the grey tint.
    ink(c, GColorWhite);
    graphics_context_set_stroke_width(c, g->stage == 3 ? 1 : 2);
#endif
    for (int k = 0; k < g->d[PLANTS + i]; k++)
      sprout(c, x + 4 + (k % 3) * 8, y + 8 + (k / 3) * 12);
    graphics_context_set_stroke_width(c, 1);
  }
  for (int i = 0; i < SLOTS; i++) {
    int b = ANIMALS + i * 5;
    if (!g->d[b])
      continue;
    int x = 4 + g->d[b + 1] % 6 * 28 + 14, y = 1 + g->d[b + 1] / 6 * 28 + 14;
    if (g->d[b] == 1)
      grazer(c, x, y);
    else
      hunter(c, x, y);
  }
  int cx = 4 + g->d[CURSOR] % 6 * 28, cy = 1 + g->d[CURSOR] / 6 * 28;
  ink(c, GColorWhite);
  box(c, cx, cy, 27, 27, false);
#if UI_COMPACT
  box(c, cx + 2, cy + 2, 23, 23, false); // Double outline: the cursor reads over plants.
  tool_row(g, c);
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
    ink(c, PBL_IF_COLOR_ELSE(GColorCyan, GColorWhite));
    line(c, 0, 114, 176, 114);
  }
}
#if UI_COMPACT
// The tool and its cost live in the legend, so the bottom line keeps the goal,
// energy, water and the balance count. It names whichever balance condition is
// missing. The Time Round's top chord is too short for the plant total, so
// round screens show season and species there and leave plants to the hint.
void game_hud_compact(const Game *g, char *a, size_t n, char *b, size_t m) {
  int pl, h, p;
  ecosystem_counts(g, &pl, &h, &p);
  static const char *const seasons[] = {"Spring", "Summer", "Autumn", "Winter"};
#ifdef PBL_ROUND
  snprintf(a, n, "%s  G%d P%d", seasons[g->stage], h, p);
#else
  snprintf(a, n, "%s L%d G%d P%d", seasons[g->stage], pl, h, p);
#endif
  long energy = g->d[BUDGET], water = g->d[WATER];
  if (!p)
    snprintf(b, m, "Add hunter  $%ld", energy);
  else if (h < 3)
    snprintf(b, m, PBL_IF_ROUND_ELSE("Add grazers $%ld", "Need 3 grazers $%ld"), energy);
  else if (pl < 20)
    snprintf(b, m, "Need plants $%ld", energy);
  else if (water <= 10)
    snprintf(b, m, "Need rain  $%ld", energy);
  else if (g->status == 1)
    snprintf(b, m, "Balanced $%ld W%ld", energy, water);
  else
    snprintf(b, m, PBL_IF_ROUND_ELSE("$%ld W%ld %ld/60", "$%ld  W%ld  %ld/60"), energy, water,
             (long)g->d[BALANCE]);
}
#endif
