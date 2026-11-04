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

The habitat is a grid of six by four square cells, wider than tall, with the
tools underneath, so it is drawn for rectangles first. On 144×168 screens the
grid spans the full width with the season and species counts above it and the
goal line below. The Time Round shows the same grid at 79% with its corners
inside the rim. Its top line keeps the season and the grazer and hunter counts,
and the plant total moves to the goal line when plants run short.

Watches without touch show the tools as a legend: each tool has the mark it
puts on the board and its cost, and the selected tool is inverted. Hold Select
to change it. The goal line names what the habitat lacks (a hunter, grazers,
plants or rain); otherwise it shows energy, water and the balance count. The
rules drop the touch line.

On black-and-white Pebbles the species differ by shape: grazers are solid discs
on a black ring and hunters are hollow triangles with a centre dot. Winter
plants use thin strokes instead of grey, and the cursor is a double outline.

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
