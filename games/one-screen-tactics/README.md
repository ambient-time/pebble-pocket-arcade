# One-Screen Tactics

Five turns to keep a beacon alive on a tiny battlefield.

## Play

Up/Down cycles north, east, south, west, and wait. Select commits the action.
Tap a neighboring tile to move or shove, or tap your own tile to wait.
Red outlined cells and exclamation marks show the attacks due after your action.
Push an enemy into an X pit to remove it before its attack resolves.

You and the beacon each have three health. Survive five turns with both intact
to win; either reaching zero ends the attempt. A goblin threatens the beacon
from nearby, a lancer attacks on alternate turns, and a sentry periodically
strikes the player's previously marked position. There are eight rotated or
mirrored orientations of the authored board, each with a verified winning route.

A puzzle takes roughly 30 seconds to a few minutes. Nothing moves until you act.

## Pause and resume

Back opens the pause menu: Resume, Restart, How to play, About, and Save and exit.
Restart asks before replacing an unfinished game. On a finished round, press
Select once or tap the outlined replay row to start again. The result explains
the outcome and shows the round totals. The next launch resumes the saved game.
Menus support buttons and touch dragging. Active play saves periodically (about
every ten seconds), at important transitions, and when leaving the app. An
unexpected interruption may lose the last few seconds. Time does not pass while
the app is closed or paused.

About shows Luke Steuber’s portrait and this app’s version. Tap, Select, or
Back returns to the same menu row without advancing the game.

## Readability

The playfield and pieces fill more of the watch. Text uses bold fonts with an
18-pixel minimum; actions and instructions use 24 pixels. Selected controls use
a white fill and black text, and important markers retain outlines or shapes
in addition to color. Touch and button controls are unchanged.

## Screens and shapes

The board is a square, so it suits both shapes: it spans the width of a
rectangle and sits inside the circle on round screens, with the score line
above and the chosen move below. Pebble Time 2 and Round 2 keep the 0.3.1
board. On 144×168 screens every cell is 24 pixels and the board spans the
width. On Pebble Time Round the cells are 22 pixels, the board is centred with
its corners inside the rim, and the lines shorten to "HP 3  B 3  1/5" and
"SELECT: RIGHT". On both, the result card says "Beacon held", "Beacon lost" or
"Defender lost".

On black-and-white Pebbles the cells are black with a dotted grid. Pits are a
heavy X, foes are solid white squares with a black letter, the beacon is a
ring and you are a double ring. A cell attacked after your action is shaded
and marked "!", and the chosen move has a solid outline. Watches without touch
drop the tap instruction from How to play.

## Install and build

Open the included `.pbw` with the Pebble phone app and install it on the connected
watch. This source runs on every Pebble with a native build: Pebble Time 2
(emery), Pebble Round 2 (gabbro), Pebble Time and Time Steel (basalt), Pebble 2
(diorite), Pebble 2 Duo (flint) and Pebble Time Round (chalk). Touch works on
Time 2 and Round 2 with firmware 4.33.2 or later; buttons work everywhere.

The source ZIP is a standalone Pebble project. Extract it, enter its directory,
and run `pebble build` with SDK 4.33.1 and pebble-tool installed. It has no runtime
network service or phone-side JavaScript dependency.

Version 0.4.0. Created by Luke Steuber. MIT license.
Native emulator evidence covers all six targets; physical-watch validation
is still separate. This package does not imply a Pebble Store release.
