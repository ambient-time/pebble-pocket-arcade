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

The track is a wide loop with the main line across the middle, A above it, B
below it and C at the upper right. Rectangles show it whole: on 144×168 screens
it spans the full width, with the score above and the switch line below. On
round screens the main line sits on the circle's widest chord and the stations,
the loop and both text lines stay inside the rim; C is the farthest point. The
track is scaled evenly, never stretched. Junction rings are a little larger on
the smaller screens so the junction number stays inside its ring.

The bottom line is shorter where space is tight. On 144×168 it reads "Switch 1
Next C", or "1 LOCKED  Next C" while a train holds the selected junction. The
Pebble Time Round reads "Sw 1  Next C" or "Sw 1 LOCKED". On Pebble Round 2 the
locked line now reads "1 LOCKED  Next B" so it no longer runs past the rim.

On black-and-white Pebbles every station is white with a black letter, and
every train is a white outline with its letter: square A, circle B, triangle C.
A junction's set route is a thick solid line and the route it is not set to is
dotted. A locked junction gets a thick ring drawn over the train holding it.
Strikes are counted in the top line on every watch. Watches without touch drop
the touch hint from the rules.

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
