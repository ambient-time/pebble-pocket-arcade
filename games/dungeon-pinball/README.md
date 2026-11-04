# Dungeon Pinball

A three-chamber dungeon played with a pinball and two flippers.

## Play

Select launches the ball. Hold Up for the left flipper and Down for the right;
you can hold both. On Time 2 and Round 2 you can also touch and hold the left
or right side to use that flipper, or the center to lift both. Release and
press again for another timed strike.
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

The table is a wide rectangle (176 × 144 world units) with the flippers and
drain along its bottom edge, so it is drawn whole on every screen and never
stretched. On 144×168 rectangles it spans the full width. On round screens it
is scaled down until its corners sit inside the rim: 120% on Round 2 as
before, 74% on the Time Round, which leaves room for a caption line above and
below inside the circle. Physics and saves use the same coordinates everywhere.

On 144×168 the top line is "Chamber 1/3 Balls 3" and the bottom line keeps the
Time 2 wording ("SELECT: launch", "Skulls 5  UP/DN: flip", "Gate open! Boss 3").
The Time Round's top line is "Chamber 1/3"; during play the ball count joins
the bottom line ("Skulls 5  Balls 3", "Boss 3  Balls 3"). Rules use Gothic 18
Bold on these four watches and drop the touch hint.

On black-and-white Pebbles the skulls' health colours become shapes: a skull
with two hits left is a solid white face with black features, a skull with one
hit left is a hollow ring with white features, and a skull that was just struck
gains a second ring. The closed boss gate shows bars; the open gate shows "B"
and one pip per remaining boss hit. Walls, rails, flippers and the ball draw
white.

A ball lost through the drain is no longer drawn on the result screen; in 0.3.1
it left a dot over the "BACK: menu" line.

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
