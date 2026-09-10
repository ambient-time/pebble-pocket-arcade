#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
for game in games/*; do
 name=${game##*/}
 cc -std=c11 -Wall -Wextra -Werror -g -fsanitize=address,undefined -Icommon -I"$game" tests/fuzz.c "$game/game.c" -o "build/fuzz-$name"
 "build/fuzz-$name"
done
