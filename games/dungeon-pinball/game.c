#include "model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const char *game_name = "Dungeon Pinball";
const char *game_rules =
    "Clear 3 dungeon chambers.\nHit skulls to open the gate.\nThen hit the "
    "boss 3 times.\nUP: left / DOWN: right.\nSELECT launches the ball.\nHold "
    "either screen half.";
const int game_controls = 1;
static void chamber(Game *g) {
  const int positions[3][6] = {{43, 40, 132, 43, 88, 76},
                               {52, 58, 122, 35, 105, 91},
                               {39, 84, 136, 78, 88, 49}};
  for (int i = 0; i < 3; i++) {
    int b = BUMPERS + i * 4;
    g->d[b] = positions[g->stage][i * 2] * 100;
    g->d[b + 1] = positions[g->stage][i * 2 + 1] * 100;
    g->d[b + 2] = 2;
    g->d[b + 3] = 0;
  }
  g->d[BOSS] = 3;
  g->d[WAITING] = 1;
  g->d[BALL_X] = 14500;
  g->d[BALL_Y] = 11000;
  g->d[BALL_VX] = g->d[BALL_VY] = 0;
  g->d[LEFT] = g->d[RIGHT] = 0;
}
void game_init(Game *g, uint32_t seed) {
  memset(g, 0, sizeof *g);
  g->seed = seed;
  g->d[LIVES] = 3;
  chamber(g);
}
void pinball_launch(Game *g) {
  if (!g->status && g->d[WAITING]) {
    g->d[WAITING] = 0;
    g->d[BALL_X] = 14500;
    g->d[BALL_Y] = 11000;
    g->d[BALL_VX] = -80 - (int)(rnd(g) % 50);
    g->d[BALL_VY] = -500;
  }
}
void game_input(Game *g, int a) {
  if (g->status)
    return;
  if (a == ACT_UP)
    g->d[LEFT] = 1;
  if (a == ACT_DOWN)
    g->d[RIGHT] = 1;
  if (a == ACT_RELEASE_UP)
    g->d[LEFT] = 0;
  if (a == ACT_RELEASE_DOWN)
    g->d[RIGHT] = 0;
  if (a == ACT_SELECT)
    pinball_launch(g);
}
void game_touch(Game *g, int x, int y, int type) {
  (void)y;
  if (g->status)
    return;
  if (type == 2) {
    g->d[LEFT] = g->d[RIGHT] = 0;
    return;
  }
  if (g->d[WAITING])
    pinball_launch(g);
  g->d[LEFT] = x < 100;
  g->d[RIGHT] = x > 76;
}
static void rail(Game *g, int ax, int ay, int bx, int by, int active,
                 int left) {
  int px = g->d[BALL_X], py = g->d[BALL_Y], dx = bx - ax, dy = by - ay,
      len = dx * dx + dy * dy;
  int t = limit((int64_t)((px - ax) * dx + (py - ay) * dy) * 1000 / len, 0,
                1000),
      xx = ax + dx * t / 1000, yy = ay + dy * t / 1000, nx = px - xx,
      ny = py - yy, dist = nx * nx + ny * ny;
  if (dist >= 490000)
    return;
  if (!dist) {
    ny = -100;
    dist = 10000;
  }
  int dot = g->d[BALL_VX] * nx + g->d[BALL_VY] * ny;
  if (dot < 0) {
    g->d[BALL_VX] -= (int64_t)2 * dot * nx / dist;
    g->d[BALL_VY] -= (int64_t)2 * dot * ny / dist;
  }
  g->d[BALL_Y] -= 150;
  if (active && py > 10500) {
    g->d[BALL_VY] = -480 - (int)(rnd(g) % 80);
    g->d[BALL_VX] = (left ? 1 : -1) * (80 + (int)(rnd(g) % 200));
  } else if (g->d[BALL_VY] > -60)
    g->d[BALL_VY] = -80;
}
static bool bumper(Game *g, int x, int y, int radius) {
  int dx = g->d[BALL_X] - x, dy = g->d[BALL_Y] - y, dist = dx * dx + dy * dy;
  if (dist > radius * radius)
    return false;
  if (!dx && !dy)
    dy = 100;
  int max = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
  g->d[BALL_VX] = dx * 360 / max;
  g->d[BALL_VY] = dy * 360 / max;
  g->d[BALL_X] = x + dx * (radius + 100) / max;
  g->d[BALL_Y] = y + dy * (radius + 100) / max;
  return true;
}
void game_tick(Game *g) {
  if (g->status || g->d[WAITING])
    return;
  g->ticks++;
  if (g->d[COOLDOWN])
    g->d[COOLDOWN]--;
  for (int i = 0; i < 3; i++)
    if (g->d[BUMPERS + i * 4 + 3])
      g->d[BUMPERS + i * 4 + 3]--;
  g->d[BALL_VY] += 7;
  for (int sub = 0; sub < 4; sub++) {
    g->d[BALL_VX] = limit(g->d[BALL_VX], -650, 650);
    g->d[BALL_VY] = limit(g->d[BALL_VY], -650, 650);
    g->d[BALL_X] += g->d[BALL_VX] / 4;
    g->d[BALL_Y] += g->d[BALL_VY] / 4;
    if (g->d[BALL_X] < 1000) {
      g->d[BALL_X] = 1000;
      g->d[BALL_VX] = abs(g->d[BALL_VX]);
    }
    if (g->d[BALL_X] > 16600) {
      g->d[BALL_X] = 16600;
      g->d[BALL_VX] = -abs(g->d[BALL_VX]);
    }
    if (g->d[BALL_Y] < 1000) {
      g->d[BALL_Y] = 1000;
      g->d[BALL_VY] = abs(g->d[BALL_VY]);
    }
    int alive = 0;
    for (int i = 0; i < 3; i++) {
      int b = BUMPERS + i * 4;
      if (g->d[b + 2]) {
        alive++;
        if (!g->d[b + 3] && bumper(g, g->d[b], g->d[b + 1], 1300)) {
          g->d[b + 2]--;
          g->d[b + 3] = 10;
          g->score += 50;
        }
      }
    }
    if (!alive && !g->d[COOLDOWN] && bumper(g, 8800, 2300, 1600)) {
      g->d[BOSS]--;
      g->d[COOLDOWN] = 15;
      g->score += 100;
      if (!g->d[BOSS]) {
        g->stage++;
        if (g->stage == 3) {
          g->status = 1;
          return;
        }
        chamber(g);
        return;
      }
    }
    rail(g, 1000, 9800, 4200, 12400, 0, 1);
    rail(g, 16600, 9800, 13400, 12400, 0, 0);
    rail(g, 4200, 12400, 8200, g->d[LEFT] ? 10800 : 13500, g->d[LEFT], 1);
    rail(g, 13400, 12400, 9400, g->d[RIGHT] ? 10800 : 13500, g->d[RIGHT], 0);
    if (g->d[BALL_Y] > 14500) {
      g->d[LIVES]--;
      if (!g->d[LIVES])
        g->status = 2;
      else {
        g->d[WAITING] = 1;
        g->d[BALL_X] = 14500;
        g->d[BALL_Y] = 11000;
      }
      return;
    }
  }
  if (abs(g->d[BALL_VX]) + abs(g->d[BALL_VY]) < 25) {
    if (++g->d[STUCK] > 60) {
      g->d[BALL_VX] = 60;
      g->d[BALL_VY] = -100;
      g->d[STUCK] = 0;
    }
  } else
    g->d[STUCK] = 0;
}
bool game_running(const Game *g) { return !g->status && !g->d[WAITING]; }
bool game_valid(const Game *g) {
  return g->status >= 0 && g->status <= 2 && g->stage >= 0 && g->stage <= 3 &&
         g->d[LIVES] >= 0 && g->d[LIVES] <= 3 && g->d[BOSS] >= 0 &&
         g->d[BOSS] <= 3 && g->d[BALL_X] >= 0 && g->d[BALL_X] <= 18000 &&
         g->d[BALL_Y] >= 0 && g->d[BALL_Y] <= 16000;
}
void game_hud(const Game *g, char *a, size_t n, char *b, size_t m) {
  snprintf(a, n, "Chamber %d/3  Balls %ld", g->stage < 3 ? g->stage + 1 : 3,
           (long)g->d[LIVES]);
  int alive = 0;
  for (int i = 0; i < 3; i++)
    alive += g->d[BUMPERS + i * 4 + 2];
  snprintf(b, m,
           g->d[WAITING] ? "SELECT: launch"
           : alive       ? "Skulls %d  UP/DN flippers"
                         : "Gate open! Boss %d HP",
           alive ? alive : (int)g->d[BOSS]);
}

const int game_continuous = 0;
