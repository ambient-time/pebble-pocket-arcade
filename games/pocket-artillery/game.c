// Pocket Artillery rules. Luke Steuber.
#include "model.h"
#include "trig.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const char *game_name = "Pocket Artillery";
const char *game_rules =
    "Destroy the other tank.\nUP/DOWN adjust angle.\nSELECT switches to "
    "power.\nHold SELECT to fire.\nDrag the sky to aim; tap FIRE.\nBACK pauses "
    "and saves.";
int artillery_y(const Game *g, int who) {
  return g->d[TERRAIN + (who ? 157 : 18)] - 4;
}
void game_init(Game *g, uint32_t seed) {
  memset(g, 0, sizeof *g);
  g->seed = seed;
  g->d[ANGLE] = 45;
  g->d[POWER] = 70;
  g->d[HP] = g->d[ENEMY_HP] = 100;
  g->d[WIND] = (int)(rnd(g) % 5) - 2;
  int y = 105;
  for (int x = 0; x < FIELD_W; x++) {
    if (x % 7 == 0)
      y = limit(y + (int)(rnd(g) % 13) - 6, 78, 120);
    g->d[TERRAIN + x] = y;
  }
  for (int x = 10; x <= 26; x++)
    g->d[TERRAIN + x] = 105;
  for (int x = 149; x <= 165; x++)
    g->d[TERRAIN + x] = 105;
}
void artillery_launch(Game *g, int who, int angle, int power) {
  if (g->status)
    return;
  g->d[SHOOTER] = who;
  g->d[SHOT_X] = (who ? 157 : 18) * 100;
  g->d[SHOT_Y] = (artillery_y(g, who) - 5) * 100;
  g->d[VX] = (who ? -1 : 1) * power * 5 * sine[90 - angle] / 1000;
  g->d[VY] = -power * 5 * sine[angle] / 1000;
  g->d[PHASE] = 1;
  g->d[DELAY] = 0;
  g->d[SHOTS]++;
}
void artillery_impact(Game *g, int x, int y) {
  g->d[BLAST_X] = x;
  g->d[BLAST_Y] = y;
  g->d[BLAST_AGE] = 18;
  for (int who = 0; who < 2; who++) {
    int dist = abs(x - (who ? 157 : 18)) + abs(y - artillery_y(g, who)) / 2;
    if (dist < 24) {
      int damage = limit(55 - dist * 2, 5, 55);
      int id = who ? ENEMY_HP : HP;
      g->d[id] = limit(g->d[id] - damage, 0, 100);
    }
  }
  for (int i = limit(x - 13, 0, 175); i <= limit(x + 13, 0, 175); i++) {
    int crater = y + 14 - abs(i - x);
    if (crater > g->d[TERRAIN + i])
      g->d[TERRAIN + i] = limit(crater, 0, 139);
  }
  if (!g->d[HP])
    g->status = 2;
  else if (!g->d[ENEMY_HP]) {
    g->status = 1;
    g->score = g->d[SHOTS] >= 100 ? 0 : 1000 - g->d[SHOTS] * 10;
  }
  g->d[PHASE] = 2;
  g->d[DELAY] = 20;
}
static bool fly(Game *g) {
  g->d[SHOT_X] += g->d[VX];
  g->d[SHOT_Y] += g->d[VY];
  g->d[VX] += g->d[WIND];
  g->d[VY] += 9;
  g->d[DELAY]++;
  int x = g->d[SHOT_X] / 100, y = g->d[SHOT_Y] / 100;
  if (x < 0 || x >= FIELD_W || y >= FIELD_H || g->d[DELAY] > 500) {
    g->d[PHASE] = 2;
    g->d[DELAY] = 15;
    return false;
  }
  if (y >= 0 && y >= g->d[TERRAIN + x]) {
    artillery_impact(g, x, y);
    return false;
  }
  return true;
}
void artillery_solution(const Game *g, int who, int *a, int *p) {
  int best = 10000;
  *a = 45;
  *p = 70;
  for (int angle = 18; angle <= 78; angle += 3)
    for (int power = 30; power <= 95; power += 2) {
      int x = (who ? 157 : 18) * 100, y = (artillery_y(g, who) - 5) * 100,
          vx = (who ? -1 : 1) * power * 5 * sine[90 - angle] / 1000,
          vy = -power * 5 * sine[angle] / 1000;
      for (int i = 0; i < 500; i++) {
        x += vx;
        y += vy;
        vx += g->d[WIND];
        vy += 9;
        int xx = x / 100, yy = y / 100;
        if (xx < 0 || xx >= FIELD_W || yy >= FIELD_H)
          break;
        if (yy >= 0 && yy >= g->d[TERRAIN + xx]) {
          int error =
              abs(xx - (who ? 18 : 157)) + abs(yy - artillery_y(g, !who)) / 2;
          if (error < best) {
            best = error;
            *a = angle;
            *p = power;
          }
          break;
        }
      }
    }
}
void game_tick(Game *g) {
  if (g->status)
    return;
  g->ticks++;
  if (g->d[BLAST_AGE])
    g->d[BLAST_AGE]--;
  if (g->d[PHASE] == 1) {
    fly(g);
    return;
  }
  if (g->d[PHASE] == 2 && !--g->d[DELAY]) {
    if (g->d[SHOOTER] == 0) {
      int a, p;
      artillery_solution(g, 1, &a, &p);
      int spread = g->d[SHOTS] < 5 ? 8 : 4;
      p = limit(p + (int)(rnd(g) % (spread * 2 + 1)) - spread, 20, 95);
      g->d[CPU_ANGLE] = a;
      g->d[CPU_POWER] = p;
      artillery_launch(g, 1, a, p);
    } else {
      g->d[PHASE] = 0;
      g->d[WIND] = (int)(rnd(g) % 5) - 2;
    }
  }
}
void game_input(Game *g, int action) {
  if (g->status || g->d[PHASE])
    return;
  int i = g->d[MODE] ? POWER : ANGLE;
  if (action == ACT_UP)
    g->d[i] = limit(g->d[i] + 1, 10, g->d[MODE] ? 95 : 80);
  if (action == ACT_DOWN)
    g->d[i] = limit(g->d[i] - 1, 10, g->d[MODE] ? 95 : 80);
  if (action == ACT_SELECT)
    g->d[MODE] ^= 1;
  if (action == ACT_SPECIAL)
    artillery_launch(g, 0, g->d[ANGLE], g->d[POWER]);
}
void game_touch(Game *g, int x, int y, int type) {
  if (type != 2 || g->status || g->d[PHASE])
    return;
  if (y > 120 && x > 110) {
    game_input(g, ACT_SPECIAL);
    return;
  }
  int dx = limit(x - 18, 1, 170), dy = limit(artillery_y(g, 0) - y, 1, 144),
      best = 100000, a = 45;
  for (int i = 10; i <= 80; i++) {
    int err = abs(dy * sine[90 - i] - dx * sine[i]);
    if (err < best) {
      best = err;
      a = i;
    }
  }
  g->d[ANGLE] = a;
  g->d[POWER] = limit(25 + (dx + dy) / 2, 10, 95);
}
bool game_running(const Game *g) { return !g->status && g->d[PHASE] != 0; }
bool game_valid(const Game *g) {
  if (g->status < 0 || g->status > 2 || g->d[ANGLE] < 10 || g->d[ANGLE] > 80 ||
      g->d[POWER] < 10 || g->d[POWER] > 95 || g->d[PHASE] < 0 ||
      g->d[PHASE] > 2 || g->d[HP] < 0 || g->d[HP] > 100 || g->d[ENEMY_HP] < 0 ||
      g->d[ENEMY_HP] > 100)
    return false;
  for (int x = 0; x < FIELD_W; x++)
    if (g->d[TERRAIN + x] < 0 || g->d[TERRAIN + x] > 139)
      return false;
  return true;
}
void game_hud(const Game *g, char *top, size_t n, char *bottom, size_t m) {
  snprintf(top, n, "YOU %ld   CPU %ld", (long)g->d[HP], (long)g->d[ENEMY_HP]);
  if (g->status)
    snprintf(bottom, m,
             g->status == 1 ? "Victory! SELECT: rematch"
                            : "Tank lost. SELECT: rematch");
  else if (g->d[PHASE])
    snprintf(bottom, m, "%s firing...",
             g->d[SHOOTER] ? "Opponent" : "Your tank");
  else
    snprintf(bottom, m, "%s %ld   Hold SEL: fire",
             g->d[MODE] ? "Power" : "Angle",
             (long)g->d[g->d[MODE] ? POWER : ANGLE]);
}

const int game_controls = 0;

const int game_continuous = 0;

void game_debug(const Game *g, char *out, size_t n) {
  (void)g;
  if (n)
    out[0] = 0;
}
