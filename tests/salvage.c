#include "model.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
static void travel(Game *g, int x, int y) {
  salvage_target(g, x, y);
  for (int i = 0; i < 250 && g->d[AUTO] && !g->status; i++) {
    game_tick(g);
    assert(game_valid(g));
  }
}
int main(void) {
  int wins = 0;
  for (int seed = 0; seed < 100; seed++) {
    Game g;
    game_init(&g, seed);
    for (int n = 0; n < 15 && !g.status && g.d[BANKED] < 5; n++) {
      if (g.d[CARRY] < 0) {
        int i;
        for (i = 0; i < 5; i++)
          if (!g.d[CARGO + i * 3 + 2])
            break;
        if (i == 5)
          break;
        travel(&g, g.d[CARGO + i * 3] / 100, g.d[CARGO + i * 3 + 1] / 100);
      }
      travel(&g, 20, 72);
    }
    travel(&g, 20, 18);
    wins += g.status == 1;
  }
  printf("Salvage direct-route policy: %d/100 wins\n", wins);
  assert(wins > 30);
  Game g;
  game_init(&g, 42);
  for (int i = 0; i < 4500; i++)
    game_tick(&g);
  assert(g.status == 2);
  game_init(&g, 5);
  travel(&g, 20, 18);
  assert(!g.status);
  g.d[ROCKS] = g.d[X];
  g.d[ROCKS + 1] = g.d[Y];
  g.d[ROCKS + 2] = g.d[ROCKS + 3] = 0;
  game_tick(&g);
  int hull = g.d[HULL];
  for (int i = 0; i < 20; i++)
    game_tick(&g);
  assert(g.d[HULL] == hull);
  puts("PASS: routes, bounds, deadline failure, quota-gated exit, impact "
       "invulnerability");
}
