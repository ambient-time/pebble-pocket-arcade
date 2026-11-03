#include "draw.h"
#include "model.h"
#include "trig.h"
#include <stdio.h>
// Black-and-white screens: the hillside keeps a bright crest and a hatched
// body, your tank is solid and the opponent's is an outline with an antenna.
void game_draw(const Game *g, GContext *c) {
  for (int i = 0; i < 36; i++) {
    if (PBL_IF_BW_ELSE(i % 3, 0))
      continue;
    ink(c, PBL_IF_COLOR_ELSE(GColorDarkGray, GColorWhite));
    box(c, (i * 71 + 13) % 176, (i * 29 + 7) % 65, 1, 1, true);
  }
  for (int x = 0; x < FIELD_W; x++) {
#ifdef PBL_COLOR
    ink(c, GColorArmyGreen);
    line(c, x, g->d[TERRAIN + x], x, 143);
    ink(c, GColorLightGray);
    box(c, x, g->d[TERRAIN + x], 1, 2, true);
#else
    ink(c, GColorWhite);
    if (x % 4 == 0)
      line(c, x, g->d[TERRAIN + x] + 4, x, 143);
    box(c, x, g->d[TERRAIN + x], 1, 3, true);
#endif
  }
  for (int who = 0; who < 2; who++) {
    int x = who ? 157 : 18, y = artillery_y(g, who);
    ink(c, PBL_IF_COLOR_ELSE(who ? GColorOrange : GColorCyan, GColorWhite));
#ifdef PBL_BW
    if (who) {
      ink(c, GColorBlack);
      box(c, x - 8, y - 8, 16, 13, true);
      ink(c, GColorWhite);
      box(c, x - 7, y - 2, 14, 6, false);
      circle(c, x, y - 3, 4, false);
    } else
#endif
    {
      box(c, x - 7, y - 2, 14, 6, true);
      circle(c, x, y - 3, 4, true);
    }
    int a = who ? g->d[CPU_ANGLE] : g->d[ANGLE];
    if (!a)
      a = 45;
    line(c, x, y - 3, x + (who ? -1 : 1) * 12 * sine[90 - a] / 1000,
         y - 3 - 12 * sine[a] / 1000);
    ink(c, GColorWhite);
    line(c, x - 7, y + 4, x + 7, y + 4);
    if (who) {
      line(c, x - 3, y - 6, x, y - 9);
      line(c, x, y - 9, x + 3, y - 6);
    }
  }
  if (!g->d[PHASE] && !g->status) {
    int x = 1800, y = (artillery_y(g, 0) - 5) * 100,
        vx = g->d[POWER] * 5 * sine[90 - g->d[ANGLE]] / 1000,
        vy = -g->d[POWER] * 5 * sine[g->d[ANGLE]] / 1000;
    ink(c, GColorWhite);
    for (int i = 0; i < 24; i++) {
      x += vx;
      y += vy;
      vx += g->d[WIND];
      vy += 9;
      if (i % 3 == 0 && x >= 0 && x < 17600 && y >= 0)
        circle(c, x / 100, y / 100, 1, true);
    }
#if UI_TOUCH
    ink(c, GColorBlack);
    box(c, 119, 115, 55, 28, true);
    ink(c, GColorWhite);
    box(c, 119, 115, 55, 28, false);
    large_label(c, "FIRE", 121, 114, 51);
#endif
  }
  if (g->d[PHASE] == 1) {
    ink(c, PBL_IF_COLOR_ELSE(GColorYellow, GColorWhite));
    circle(c, g->d[SHOT_X] / 100, g->d[SHOT_Y] / 100, 3, true);
  }
  if (g->d[BLAST_AGE]) {
    int age = 18 - g->d[BLAST_AGE];
    ink(c, PBL_IF_COLOR_ELSE(age < 8 ? GColorYellow : GColorOrange, GColorWhite));
    circle(c, g->d[BLAST_X], g->d[BLAST_Y], 3 + age, false);
    for (int i = 0; i < 8; i++) {
      int a = i * TRIG_MAX_ANGLE / 8;
      box(c, g->d[BLAST_X] + cos_lookup(a) * age / TRIG_MAX_RATIO,
          g->d[BLAST_Y] + sin_lookup(a) * age / TRIG_MAX_RATIO - age, 2, 2,
          true);
    }
  }
  char b[50];
  snprintf(b, sizeof b, "%ld°  %ld%%  Wind %+ld", (long)g->d[ANGLE],
           (long)g->d[POWER], (long)g->d[WIND]);
  ink(c, GColorWhite);
  // Rectangles keep the readout at the top of the sky. On round screens the
  // sky starts above the rim, so the readout sits just under the top caption.
  int ry = PBL_IF_ROUND_ELSE((UI_HUD_TOP + 22 - field_y) * 100 / UI_FIELD_SCALE, 1);
  label(c, b, 3, ry, 170);
}
void game_hud_compact(const Game *g, char *top, size_t n, char *bottom, size_t m) {
  snprintf(top, n, "YOU %ld  CPU %ld", (long)g->d[HP], (long)g->d[ENEMY_HP]);
  if (g->status)
    snprintf(bottom, m, g->status == 1 ? "Victory!" : "Tank lost");
  else if (g->d[PHASE])
    snprintf(bottom, m, "%s firing...", g->d[SHOOTER] ? "Opponent" : "Your tank");
  else
    snprintf(bottom, m, "%s %ld  Hold: fire", g->d[MODE] ? "Power" : "Angle",
             (long)g->d[g->d[MODE] ? POWER : ANGLE]);
}
