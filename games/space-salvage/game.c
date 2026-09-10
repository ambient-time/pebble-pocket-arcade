#include "model.h"
#include "trig.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const char *game_name = "Space Salvage";
const char *game_rules =
    "Bring 5 crates to the dock.\nThen enter the EXIT ring.\nUP/DOWN turn the "
    "tug.\nSELECT gives thrust.\nOr tap a destination.\nAvoid rocks. 150 "
    "seconds.";
void game_init(Game *g, uint32_t seed) {
  memset(g, 0, sizeof *g);
  g->seed = seed;
  g->d[X] = 2000;
  g->d[Y] = 7200;
  g->d[HULL] = 4;
  g->d[CARRY] = -1;
  g->d[DEADLINE] = 4500;
  const int coords[][2] = {
      {85, 22}, {146, 36}, {95, 119}, {150, 115}, {125, 76}};
  for (int i = 0; i < 5; i++) {
    int b = CARGO + i * 3;
    g->d[b] = coords[i][0] * 100;
    g->d[b + 1] = coords[i][1] * 100;
  }
  for (int i = 0; i < 6; i++) {
    int b = ROCKS + i * 4;
    g->d[b] = (60 + (int)(rnd(g) % 90)) * 100;
    g->d[b + 1] = (15 + (int)(rnd(g) % 110)) * 100;
    g->d[b + 2] = (int)(rnd(g) % 31) - 15;
    g->d[b + 3] = (int)(rnd(g) % 31) - 15;
  }
}
void salvage_target(Game *g, int x, int y) {
  g->d[TARGET_X] = limit(x, 5, 170) * 100;
  g->d[TARGET_Y] = limit(y, 6, 137) * 100;
  g->d[AUTO] = 1;
}
static void thrust(Game *g, int force) {
  int angle = g->d[ANGLE];
  g->d[VX] += cx[angle] * force / 1000;
  g->d[VY] += cy[angle] * force / 1000;
}
void game_input(Game *g, int a) {
  if (g->status)
    return;
  if (a == ACT_UP) {
    g->d[ANGLE] = (g->d[ANGLE] + 15) % 16;
    g->d[AUTO] = 0;
  } else if (a == ACT_DOWN) {
    g->d[ANGLE] = (g->d[ANGLE] + 1) % 16;
    g->d[AUTO] = 0;
  } else if (a == ACT_SELECT)
    thrust(g, 100);
  else if (a == ACT_SPECIAL)
    thrust(g, 180);
}
void game_touch(Game *g, int x, int y, int type) {
  if (type == 2 && !g->status)
    salvage_target(g, x, y);
}
void game_tick(Game *g) {
  if (g->status)
    return;
  g->ticks++;
  g->d[DEADLINE]--;
  if (g->d[SHIELD])
    g->d[SHIELD]--;
  if (g->d[FX])
    g->d[FX]--;
  if (g->d[AUTO]) {
    int dx = g->d[TARGET_X] - g->d[X], dy = g->d[TARGET_Y] - g->d[Y];
    if (abs(dx) + abs(dy) < 450) {
      g->d[AUTO] = 0;
      g->d[VX] /= 2;
      g->d[VY] /= 2;
    } else {
      int best = -2147483647, angle = 0;
      for (int a = 0; a < 16; a++) {
        int dot = dx * cx[a] + dy * cy[a];
        if (dot > best) {
          best = dot;
          angle = a;
        }
      }
      g->d[ANGLE] = angle;
      thrust(g, g->d[CARRY] < 0 ? 14 : 10);
    }
  }
  int cap = g->d[CARRY] < 0 ? 220 : 165;
  g->d[VX] = limit(g->d[VX] * 97 / 100, -cap, cap);
  g->d[VY] = limit(g->d[VY] * 97 / 100, -cap, cap);
  g->d[X] = limit(g->d[X] + g->d[VX], 500, 17000);
  g->d[Y] = limit(g->d[Y] + g->d[VY], 600, 13700);
  if (g->d[CARRY] < 0) {
    for (int i = 0; i < 5; i++) {
      int b = CARGO + i * 3;
      if (!g->d[b + 2] &&
          abs(g->d[X] - g->d[b]) + abs(g->d[Y] - g->d[b + 1]) < 1000) {
        g->d[CARRY] = i;
        g->d[b + 2] = 1;
        g->d[BEAM_X] = g->d[b];
        g->d[BEAM_Y] = g->d[b + 1];
        break;
      }
    }
  } else {
    g->d[BEAM_X] += (g->d[X] - g->d[BEAM_X]) / 5;
    g->d[BEAM_Y] += (g->d[Y] - g->d[BEAM_Y]) / 5;
    if (abs(g->d[X] - 2000) + abs(g->d[Y] - 7200) < 1500) {
      g->d[CARGO + g->d[CARRY] * 3 + 2] = 2;
      g->d[CARRY] = -1;
      g->d[BANKED]++;
      g->score += 200;
      g->d[FX] = 16;
    }
  }
  int rocks = 3 + (4500 - g->d[DEADLINE]) / 1500;
  if (rocks > 6)
    rocks = 6;
  for (int i = 0; i < rocks; i++) {
    int b = ROCKS + i * 4;
    g->d[b] += g->d[b + 2];
    g->d[b + 1] += g->d[b + 3];
    if (g->d[b] < 4500 || g->d[b] > 17000) {
      g->d[b + 2] *= -1;
      g->d[b] = limit(g->d[b], 4500, 17000);
    }
    if (g->d[b + 1] < 700 || g->d[b + 1] > 13700) {
      g->d[b + 3] *= -1;
      g->d[b + 1] = limit(g->d[b + 1], 700, 13700);
    }
    if (!g->d[SHIELD] &&
        abs(g->d[X] - g->d[b]) + abs(g->d[Y] - g->d[b + 1]) < 1100) {
      g->d[HULL]--;
      g->d[SHIELD] = 60;
      g->d[VX] = -g->d[VX];
      g->d[VY] = -g->d[VY];
      if (g->d[CARRY] >= 0) {
        int k = CARGO + g->d[CARRY] * 3;
        g->d[k] = g->d[X];
        g->d[k + 1] = g->d[Y];
        g->d[k + 2] = 0;
        g->d[CARRY] = -1;
      }
    }
  }
  if (g->d[HULL] <= 0 || g->d[DEADLINE] <= 0)
    g->status = 2;
  else if (g->d[BANKED] == 5 &&
           abs(g->d[X] - 2000) + abs(g->d[Y] - 1800) < 1300) {
    g->status = 1;
    g->score += g->d[DEADLINE] / 3;
  }
}
bool game_running(const Game *g) { return !g->status; }
bool game_valid(const Game *g) {
  return g->status >= 0 && g->status <= 2 && g->d[X] >= 500 &&
         g->d[X] <= 17000 && g->d[Y] >= 600 && g->d[Y] <= 13700 &&
         g->d[ANGLE] >= 0 && g->d[ANGLE] < 16 && g->d[CARRY] >= -1 &&
         g->d[CARRY] < 5 && g->d[BANKED] >= 0 && g->d[BANKED] <= 5 &&
         g->d[DEADLINE] >= 0 && g->d[DEADLINE] <= 4500;
}
void game_hud(const Game *g, char *a, size_t n, char *b, size_t m) {
  snprintf(a, n, "Hull %ld  Cargo %ld/5  %lds", (long)g->d[HULL],
           (long)g->d[BANKED], (long)g->d[DEADLINE] / 30);
  snprintf(b, m,
           g->d[BANKED] == 5  ? "All cargo safe! Enter EXIT"
           : g->d[CARRY] >= 0 ? "Tethered! Return to DOCK"
                              : "Turn + SELECT thrust / tap");
}

const int game_controls = 0;

const int game_continuous = 0;

void game_debug(const Game *g, char *out, size_t n) {
  snprintf(
      out, n,
      "x=%ld y=%ld carry=%ld bank=%ld hull=%ld time=%ld auto=%ld "
      "cargo=%ld,%ld,%ld,%ld,%ld pos=%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld",
      (long)g->d[X] / 100, (long)g->d[Y] / 100, (long)g->d[CARRY],
      (long)g->d[BANKED], (long)g->d[HULL], (long)g->d[DEADLINE],
      (long)g->d[AUTO], (long)g->d[CARGO + 2], (long)g->d[CARGO + 5],
      (long)g->d[CARGO + 8], (long)g->d[CARGO + 11], (long)g->d[CARGO + 14],
      (long)g->d[CARGO] / 100, (long)g->d[CARGO + 1] / 100,
      (long)g->d[CARGO + 3] / 100, (long)g->d[CARGO + 4] / 100,
      (long)g->d[CARGO + 6] / 100, (long)g->d[CARGO + 7] / 100,
      (long)g->d[CARGO + 9] / 100, (long)g->d[CARGO + 10] / 100,
      (long)g->d[CARGO + 12] / 100, (long)g->d[CARGO + 13] / 100);
}

void game_result(const Game *g, char *title, size_t n, char *detail, size_t m) {
  snprintf(title, n, "%s",
           g->status == 1    ? "Cargo secured"
           : g->d[HULL] <= 0 ? "Hull lost"
                             : "Time expired");
  snprintf(detail, m, "%ld/5 crates | %lu points", (long)g->d[BANKED],
           (unsigned long)g->score);
}
