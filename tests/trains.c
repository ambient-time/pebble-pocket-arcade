#include "model.h"
#include <assert.h>
#include <stdio.h>
int main(void) {
  for (int seed = 0; seed < 100; seed++) {
    Game g;
    game_init(&g, seed);
    for (int i = 0; i < 20000 && !g.status; i++) {
      train_auto(&g);
      game_tick(&g);
      assert(game_valid(&g));
    }
    assert(g.status == 1);
    assert(g.d[STRIKES] == 0);
  }
  for (int seed = 0; seed < 50; seed++) {
    Game g;
    game_init(&g, seed);
    for (int i = 0; i < 20000 && !g.status; i++)
      game_tick(&g);
    assert(g.status == 2);
  }
  Game g;
  game_init(&g, 1);
  g.d[TRAINS] = 1;
  g.d[TRAINS + 1] = 0;
  g.d[TRAINS + 2] = 4300;
  assert(train_locked(&g, 0));
  game_input(&g, ACT_SELECT);
  assert(!g.d[SW0]);
  puts("PASS: 100 zero-strike complete shifts with routing, 50 unattended "
       "failures, occupied switch lock");
}
