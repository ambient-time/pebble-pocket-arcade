# Pocket Artillery

A tank duel across a hillside that changes with every explosion.

Pocket Artillery is the preferred artillery game in Pocket Arcade. Crosswind
is a separate legacy game; its identity and saved games remain separate.

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

The duel is a wide hillside, so rectangles show the whole field with the score
above and the selected value below. On 144×168 screens the field spans the full
width and the text is shorter: "Angle 45  Hold: fire". Round screens put the
tank line on the circle's widest chord, so both tanks clear the rim and the
shot arcs into the dome above; the score and readout sit inside the top of the
circle and the selected value below the ground. Pebble Round 2 uses the same
arrangement, which also keeps the FIRE button and the bottom line inside the
rim. The result card keeps its centred place on every screen.

On black-and-white Pebbles the hillside has a bright crest and a hatched body,
your tank is solid, and the opponent's tank is an outline with an antenna.
Watches without touch hide the FIRE button and the touch hints.

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
