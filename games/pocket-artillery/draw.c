#include "draw.h"
#include "model.h"
#include "trig.h"
#include <stdio.h>
void game_draw(const Game *g, GContext *c) {
  for (int i = 0; i < 36; i++) {
    ink(c, GColorDarkGray);
    box(c, (i * 71 + 13) % 176, (i * 29 + 7) % 65, 1, 1, true);
  }
  for (int x = 0; x < FIELD_W; x++) {
    ink(c, x % 4 ? GColorDarkGreen : GColorArmyGreen);
    line(c, x, g->d[TERRAIN + x], x, 143);
  }
  for (int who = 0; who < 2; who++) {
    int x = who ? 157 : 18, y = artillery_y(g, who);
    ink(c, who ? GColorOrange : GColorCyan);
    box(c, x - 7, y - 2, 14, 6, true);
    circle(c, x, y - 3, 4, true);
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
    ink(c, GColorBlack);
    box(c, 119, 122, 55, 21, true);
    ink(c, GColorWhite);
    box(c, 119, 122, 55, 21, false);
    label(c, "FIRE", 124, 123, 46);
  }
  if (g->d[PHASE] == 1) {
    ink(c, GColorYellow);
    circle(c, g->d[SHOT_X] / 100, g->d[SHOT_Y] / 100, 3, true);
  }
  if (g->d[BLAST_AGE]) {
    int age = 18 - g->d[BLAST_AGE];
    ink(c, age < 8 ? GColorYellow : GColorOrange);
    circle(c, g->d[BLAST_X], g->d[BLAST_Y], 3 + age, false);
    for (int i = 0; i < 8; i++) {
      int a = i * TRIG_MAX_ANGLE / 8;
      box(c, g->d[BLAST_X] + cos_lookup(a) * age / TRIG_MAX_RATIO,
          g->d[BLAST_Y] + sin_lookup(a) * age / TRIG_MAX_RATIO - age, 2, 2,
          true);
    }
  }
  char b[50];
  snprintf(b, sizeof b, "%ld deg  %ld%%   wind %+ld", (long)g->d[ANGLE],
           (long)g->d[POWER], (long)g->d[WIND]);
  ink(c, GColorWhite);
  label(c, b, 3, 1, 170);
}
