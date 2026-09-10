# Space Salvage

Five crates, a fragile tug, and a wreck field that grows more dangerous.

## Play

Up/Down turns the tug; Select gives thrust, and holding Select gives a stronger
burst. Or tap a destination and let the tug steer toward it. Pass close to a
crate to attach its tether. Return to the dock at the left to bank it, then
head back for another. Carrying cargo slows the tug.

After banking all five crates, enter the EXIT ring at the upper left. You have
four hull and 150 seconds of active simulation time. A rock collision costs
hull and drops carried cargo; a brief shield prevents repeated damage from the
same impact. More rocks enter as the deadline approaches. Losing all hull or
running out of time ends the mission.

Expect a one-to-two-and-a-half-minute attempt. Docking produces a brief expanding
star distortion; the tether and cargo markings remain visible during flight.

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
