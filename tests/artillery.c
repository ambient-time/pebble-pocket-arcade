#include "model.h"
#include "save.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static unsigned char store[200][256];
static int lengths[200], fail = -1, writes;
static int read_data(uint32_t k, void *b, size_t n) {
  if (k >= 200 || lengths[k] != (int)n)
    return -1;
  memcpy(b, store[k], n);
  return n;
}
static int write_data(uint32_t k, const void *b, size_t n) {
  if (writes++ == fail)
    return -1;
  assert(k < 200 && n <= 256);
  memcpy(store[k], b, n);
  lengths[k] = n;
  return n;
}
static void settle(Game *g) {
  for (int i = 0; game_running(g) && i < 1600; i++) {
    game_tick(g);
    assert(game_valid(g));
  }
  assert(!game_running(g));
}
int main(void) {
  Game g, h;
  int wins = 0, losses = 0;
  for (int seed = 1; seed <= 200; seed++) {
    game_init(&g, seed);
    for (int shot = 0; !g.status && shot < 30; shot++) {
      int a, p;
      artillery_solution(&g, 0, &a, &p);
      artillery_launch(&g, 0, a, p);
      settle(&g);
    }
    assert(g.status);
    wins += g.status == 1;
    losses += g.status == 2;
  }
  assert(wins > 100);
  printf("Artillery aimed play: %d wins, %d losses / 200 seeds\n", wins,
         losses);
  for (int seed = 1; seed <= 50; seed++) {
    game_init(&g, seed);
    for (int shot = 0; !g.status && shot < 30; shot++) {
      artillery_launch(&g, 0, 10, 10);
      settle(&g);
    }
    assert(g.status == 2);
  }
  puts("Deliberate misses: 50/50 losses");
  for (int x = 0; x < 176; x++) {
    game_init(&g, 4);
    artillery_impact(&g, x, g.d[TERRAIN + x]);
    assert(game_valid(&g));
    assert(artillery_y(&g, 0) == g.d[TERRAIN + 18] - 4);
  }
  game_init(&g, 7);
  g.d[SHOTS] = 101;
  g.d[ENEMY_HP] = 1;
  artillery_impact(&g, 157, artillery_y(&g, 1));
  assert(g.status == 1 && g.score == 0);
  h = g;
  for (int i = 0; i < 200; i++)
    game_tick(&g);
  assert(!memcmp(&g, &h, sizeof g));
  SaveIO io = {read_data, write_data};
  game_init(&g, 42);
  assert(save_store(&io, &g));
  assert(save_load(&io, &h) && !memcmp(&g, &h, sizeof g));
  Game next = g;
  artillery_launch(&next, 0, 55, 73);
  for (int i = 0; i < 14; i++)
    game_tick(&next);
  unsigned char backup[sizeof store];
  int sizes[200];
  memcpy(backup, store, sizeof store);
  memcpy(sizes, lengths, sizeof sizes);
  for (int f = 0; f < 19; f++) {
    memcpy(store, backup, sizeof store);
    memcpy(lengths, sizes, sizeof sizes);
    writes = 0;
    fail = f;
    bool ok = save_store(&io, &next);
    assert(save_load(&io, &h));
    assert(!memcmp(&h, ok ? &next : &g, sizeof h));
  }
  fail = -1;
  assert(save_store(&io, &next));
  assert(save_load(&io, &h));
  settle(&h);
  settle(&next);
  assert(!memcmp(&h, &next, sizeof h));
  // The selected bank is corrupt; preserve the intact fallback through another
  // interrupted save, including the selector repair write.
  game_init(&g, 99);
  assert(save_store(&io, &g));
  int32_t active;
  memcpy(&active, store[50], sizeof active);
  store[100 + active * 32][0] ^= 1;
  assert(save_load(&io, &h));
  Game fallback = h;
  memcpy(backup, store, sizeof store);
  memcpy(sizes, lengths, sizeof sizes);
  for (int f = 0; f < 9; f++) {
    memcpy(store, backup, sizeof store);
    memcpy(lengths, sizes, sizeof sizes);
    writes = 0;
    fail = f;
    bool ok = save_store(&io, &g);
    assert(save_load(&io, &h));
    assert(!memcmp(&h, ok ? &g : &fallback, sizeof h));
  }
  puts("PASS: terrain bounds, terminal freeze, atomic saves at every write and "
       "exact mid-flight resume");
}
