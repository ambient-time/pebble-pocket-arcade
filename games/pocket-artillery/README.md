# Pocket Artillery

A tank duel across a hillside that changes with every explosion.

## Play

Up/Down adjusts the selected value. Select switches between angle and power;
hold Select to fire. With touch, drag in the sky to set your shot, then tap FIRE.
The dotted preview shows the beginning of the trajectory. Wind bends the shot.
Each shot gives the opposing tank a turn. Explosions gouge craters, and tanks
settle with the ground beneath them.

Destroy the other tank before it destroys yours. Both start at 100 health.
Damage depends on how close the explosion lands. A win scores 1,000 minus ten
per shot, with a minimum of zero. A duel usually takes a few minutes, depending
on how long you spend aiming; there is no turn timer.

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
