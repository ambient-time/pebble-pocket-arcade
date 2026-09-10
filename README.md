# Pocket Arcade for Pebble

Six native games by Luke Steuber for Pebble Time 2 and Pebble Round 2. Each is a
separate app with buttons, touch, a pause menu, and saved progress.

[Open the local install page](release/index.html) ·
[Download the complete bundle](release/pocket-arcade-0.1.0.zip)

| Game | Objective | Session | Rules and controls |
| --- | --- | --- | --- |
| [Space Salvage](release/space-salvage/0.1.0/space-salvage-0.1.0.pbw) | Bank five crates, then reach the exit with hull remaining | 150-second limit | [Guide](games/space-salvage/README.md) |
| [Pocket Artillery](release/pocket-artillery/0.1.0/pocket-artillery-0.1.0.pbw) | Destroy the opposing tank across destructible terrain | A few minutes; no aiming timer | [Guide](games/pocket-artillery/README.md) |
| [Dungeon Pinball](release/dungeon-pinball/0.1.0/dungeon-pinball-0.1.0.pbw) | Defeat three chambers of skull targets and bosses with three balls | About 1–3 minutes | [Guide](games/dungeon-pinball/README.md) |
| [Train Dispatcher](release/train-dispatcher/0.1.0/train-dispatcher-0.1.0.pbw) | Deliver 18 trains before three mistakes | About two minutes | [Guide](games/train-dispatcher/README.md) |
| [Pocket Ecosystem](release/pocket-ecosystem/0.1.0/pocket-ecosystem-0.1.0.pbw) | Maintain plants, grazers, hunters, and water for 60 consecutive seconds | Continues after the balance milestone | [Guide](games/pocket-ecosystem/README.md) |
| [One-Screen Tactics](release/one-screen-tactics/0.1.0/one-screen-tactics-0.1.0.pbw) | Keep yourself and the beacon alive through five enemy turns | Five turns; no timer | [Guide](games/one-screen-tactics/README.md) |

## Installation and saves

Open a PBW with the Pebble phone app to install it on the connected watch. The
packages include only emery and gabbro targets. Touch requires firmware 4.33.2
or later; buttons remain available.

Back pauses and opens Resume, Restart, How to play, and Save and exit. Restart
asks before replacing progress. Launching again resumes the game. Timed play
saves roughly every ten seconds and at important transitions; an unexpected
interruption may lose the last few seconds. Closed-app time does not advance.
Saves use two checksum-verified snapshots, retaining the preceding intact copy
if a write is interrupted.

## Source and verification

Install Pebble SDK 4.33.1 and pebble-tool, then run:

```sh
python3 tools/build.py pocket-artillery
sh tests/core.sh
sh tests/fuzz.sh
```

Substitute another game directory name to build it. Each release also includes a
standalone source ZIP that builds with `pebble build` after extraction. Native
input tests are in `tests/native_*.py`; run them using pebble-tool's Python
environment. They install the PBW, operate buttons and emulated touch, capture
screenshots, and record the installed PBW's SHA-256. Diagnostics are read-only;
there is no gameplay command backdoor.

Release folders contain PBWs, source ZIPs, rules, screenshots, both target
reports, and checksum manifests. `release/validation/` holds core results,
randomized invariant results, and independent source rebuild reports.
`PLAN.md` records the acceptance checklist.

Emulator checks do not replace physical-watch testing. These are private local
release bundles; no Pebble Store listing has been published for them.

MIT license. Original code and launcher art by Luke Steuber.
