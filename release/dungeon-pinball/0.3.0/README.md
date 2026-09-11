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

Back opens the pause menu: Resume, Restart, How to play, and Save and exit.
Restart asks before replacing an unfinished game. On a finished round, press
Select once or tap the outlined replay row to start again. The result explains
the outcome and shows the round totals. The next launch resumes the saved game.
Menus support buttons and touch dragging. Active play saves periodically (about
every ten seconds), at important transitions, and when leaving the app. An
unexpected interruption may lose the last few seconds. Time does not pass while
the app is closed or paused.

## Readability

The playfield and pieces fill more of the watch. Text uses bold fonts with an
18-pixel minimum; actions and instructions use 24 pixels. Selected controls use
a white fill and black text, and important markers retain outlines or shapes
in addition to color. Touch and button controls are unchanged.

## Install and build

Open the included `.pbw` with the Pebble phone app and install it on the connected
watch. This app targets Pebble Time 2 (emery) and Pebble Round 2 (gabbro).
Touch requires firmware 4.33.2 or later; buttons remain available.

The source ZIP is a standalone Pebble project. Extract it, enter its directory,
and run `pebble build` with SDK 4.33.1 and pebble-tool installed. It has no runtime
network service or phone-side JavaScript dependency.

Version 0.3.0. Created by Luke Steuber. MIT license.
Native emulator evidence covers both watch shapes; physical-watch validation
is still separate. This package does not imply a Pebble Store release.
