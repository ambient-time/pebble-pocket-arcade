# Dungeon Pinball

A three-chamber dungeon played with a pinball and two flippers.

## Play

Select launches the ball. Hold Up for the left flipper and Down for the right;
you can hold both. Touch and hold the left or right side to use that flipper,
or the center to lift both. Release and press again for another timed strike.
Lifting a flipper gives a short boost window, so leaving it held is less useful
than meeting the ball as it comes down.

Hit each of the three skull bumpers twice to open the gate, then strike the boss
three times. Defeating it opens the next chamber with a different bumper layout.
Launch again to continue. Clear all three chambers to win before losing three
balls through the drain. Balls are shared across the whole dungeon.

Most attempts are short; a successful reactive test run averaged about 69 seconds.
That is an automated-play measurement, not a guarantee of human play length.

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

The drain belongs at the bottom and both walls carry rails, so the table is
never rotated or cropped: every screen shows all of it, scaled evenly. On
144×168 screens the walls touch both edges (87%), which keeps the flippers,
skulls and ball as large as the width allows, with the chamber above and the
status below ("Balls 2 Skulls 4", "SELECT: launch"). Pebble Time Round fits the
whole table inside the circle, corners just inside the rim, with the chamber
above and the status line tucked under the bottom rail where the circle is still
wide enough. Pebble Time 2 and Round 2 are unchanged.

On black-and-white Pebbles a skull with two hits left is solid white, with one
hit left it is an outline, and a struck skull flashes a halo; the locked gate
keeps its bars and the open gate is a solid block with B. Watches without touch
drop the touch line from the rules.

## Install and build

Open the included `.pbw` with the Pebble phone app and install it on the connected
watch. Version 0.4.0 runs on every Pebble with a native build: Pebble Time 2
(emery), Pebble Round 2 (gabbro), Pebble Time and Time Steel (basalt), Pebble 2
(diorite), Pebble 2 Duo (flint) and Pebble Time Round (chalk). Touch works on
Time 2 and Round 2 with firmware 4.33.2 or later; buttons work everywhere.

The source ZIP is a standalone Pebble project. Extract it, enter its directory,
and run `pebble build` with SDK 4.33.1 and pebble-tool installed. It has no runtime
network service or phone-side JavaScript dependency.

Version 0.4.0. Created by Luke Steuber. MIT license.
Native emulator evidence covers all six targets; physical-watch validation
is still separate. This package does not imply a Pebble Store release.
