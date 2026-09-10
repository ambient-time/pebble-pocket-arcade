#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
mkdir -p build
for entry in 'artillery pocket-artillery' 'tactics one-screen-tactics' 'trains train-dispatcher' 'salvage space-salvage' 'pinball dungeon-pinball' 'ecosystem pocket-ecosystem'; do
  set -- $entry
  cc -std=c11 -Wall -Wextra -Werror -g -fsanitize=address,undefined -Icommon -I"games/$2" "tests/$1.c" "games/$2/game.c" common/save.c -o "build/$1-test"
  "build/$1-test"
done
