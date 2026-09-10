# Train Dispatcher

Keep a small railway moving through an increasingly busy shift.

## Play

Up/Down selects one of three junctions. Select toggles its route, or tap the
junction directly. A train occupying a junction locks it until it passes.
The first branch leads to A, the next to B, and the third to C or the return loop.
Match the letter and shape on each train to its station: square A, circle B,
and triangle C. The symbols supplement the colors.

Deliver 18 trains to finish the shift. A wrong station or a collision adds a
strike; three strikes end it. Traffic speeds up after six and twelve deliveries,
with shorter gaps between arrivals. A completed shift takes roughly two minutes.

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
