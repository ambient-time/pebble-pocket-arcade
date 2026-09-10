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
