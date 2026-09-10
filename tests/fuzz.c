#include "arcade.h"
#include <assert.h>
#include <stdio.h>
int main(void){unsigned total=0;for(unsigned seed=1;seed<81;seed++){Game g;game_init(&g,seed);uint32_t r=seed;for(int t=0;t<6000;t++){r=r*1664525u+1013904223u;if(t%7==0)game_input(&g,(r>>8)%7);if(t%17==0)game_touch(&g,(int)(r%236)-30,(int)((r>>16)%204)-30,(r>>12)%3);game_tick(&g);assert(game_valid(&g));total++;if(g.status==2||(g.status==1&&!game_continuous))game_init(&g,r);}}printf("%s: %u randomized input/tick invariants passed\n",game_name,total);}
