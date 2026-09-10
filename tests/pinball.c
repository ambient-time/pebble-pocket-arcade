#include "model.h"
#include <assert.h>
#include <stdio.h>
int main(void) {
  int wins = 0, maxstage = 0;
  unsigned total = 0;
  for (int seed = 0; seed < 100; seed++) {
    Game g;
    game_init(&g, seed);
    for (int t = 0; t < 24000 && !g.status; t++) {
      if (g.d[WAITING])
        pinball_launch(&g);
      int ready = g.d[BALL_Y] > 10500 && g.d[BALL_VY] > 0;
      game_input(&g, ready && g.d[BALL_X] < 9000 ? ACT_UP : ACT_RELEASE_UP);
      game_input(&g, ready && g.d[BALL_X] > 8600 ? ACT_DOWN : ACT_RELEASE_DOWN);
      game_tick(&g);
      assert(game_valid(&g));
    }
    wins += g.status == 1;
    if (g.status == 1)
      total += g.ticks;
    if (g.stage > maxstage)
      maxstage = g.stage;
  }
  printf("Pinball reactive flipper policy: %d wins/100, furthest chamber %d\n",
         wins, maxstage);
  printf("Mean winning run: %u seconds\n", total / (wins ? wins : 1) / 30);
  assert(wins >= 30 && maxstage == 3);
  for (int seed = 0; seed < 100; seed++) {
    Game passive;
    game_init(&passive, seed);
    for (int t = 0; t < 24000 && !passive.status; t++) {
      if (passive.d[WAITING])
        pinball_launch(&passive);
      game_tick(&passive);
    }
    assert(passive.status == 2);
  }
  puts("100/100 hands-off runs lose: flipper timing is required");
  Game g;
  game_init(&g, 3);
  pinball_launch(&g);
  g.d[BALL_Y] = 14600;
  g.d[BALL_VY] = 500;
  game_tick(&g);
  assert(g.d[LIVES] == 2 && g.d[WAITING]);
  for (int i = 0; i < 100; i++)
    game_tick(&g);
  assert(g.d[LIVES] == 2);
  game_input(&g, ACT_UP);
  game_input(&g, ACT_DOWN);
  assert(g.d[LEFT] && g.d[RIGHT]);
  game_input(&g, ACT_RELEASE_UP);
  assert(!g.d[LEFT] && g.d[RIGHT]);
  puts("PASS: bounded substep physics, drain counted once, independently held "
       "flippers");
}
