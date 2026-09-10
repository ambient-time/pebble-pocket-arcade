# Pocket Ecosystem

Build a habitat that can support plants, grazers, and hunters through the seasons.

## Play

You start with plants and two grazers. Introduce a hunter and enough grazers to
establish balance. Up/Down moves the cell cursor; Select uses the selected tool.
Hold Select to cycle tools. With touch, tap a tool and then a cell; tapping Rain
applies it immediately. Animals need an unoccupied cell.

Leaf costs two energy, a grazer three, a hunter five, and rain three. Energy
regenerates by one every eight seconds, up to ten. Grazers eat and reproduce;
hunters pursue them. Both can starve. Rain supports plant growth. Seasons change
every thirty seconds, affecting moisture and growth.

Maintain at least 20 plant biomass, three grazers, one hunter, and water above ten
for 60 consecutive seconds to earn Balanced. The simulation keeps going afterward.
Forty seconds without either grazers or hunters ends the habitat, even after a
previous balance milestone. Expect at least a minute to establish it, then play
for as long as you want. Closed-app time does not advance the habitat.

## Pause and resume

Back opens the pause menu: Resume, Restart, How to play, and Save and exit.
Restart asks before replacing an unfinished game. On a finished round, press
Select once or tap the outlined replay row to start again. The result explains
the outcome and shows the round totals. The next launch resumes the saved game.
Menus support buttons and touch dragging. Active play saves periodically (about
every ten seconds), at important transitions, and when leaving the app. An
unexpected interruption may lose the last few seconds. Time does not pass while
the app is closed or paused.

## Install and build

Open the included `.pbw` with the Pebble phone app and install it on the connected
watch. This app targets Pebble Time 2 (emery) and Pebble Round 2 (gabbro).
Touch requires firmware 4.33.2 or later; buttons remain available.

The source ZIP is a standalone Pebble project. Extract it, enter its directory,
and run `pebble build` with SDK 4.33.1 and pebble-tool installed. It has no runtime
network service or phone-side JavaScript dependency.

Version 0.2.0. Created by Luke Steuber. MIT license.
Native emulator evidence covers both watch shapes; physical-watch validation
is still separate. This package does not imply a Pebble Store release.
