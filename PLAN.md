# Six-game completion contract

User objective: execute all six proposed concepts to completion. Native Pebble
apps are the intended deliverables, with buttons and touch on emery/gabbro.

## Required for every game

- A complete playable loop, discoverable controls, purpose and termination or
  sustained simulation conditions. No placeholders or shared rules reskinned.
- Distinct readable graphics; important states indicated by shape as well as color.
- Back pauses, native menus scroll with touch, and saved games restore safely.
- Core rules tested for invariants and meaningful play outcomes, including losses.
- Exact PBW installed and driven on both native emulators; screenshots and logs
  inspected. Physical-watch validation is reported separately and never inferred.
- Versioned PBW, portable source, SHA-256 manifest, controls and game rules.
- Local install links provided. Store publication is outside this new request.

## Game-specific requirements

1. Space Salvage: steering/thrust, cargo pickup and delivery, hull/fuel or danger,
   a clear escape condition, collision geometry and brief tractor/delivery effects.
2. Pocket Artillery: angle and power, visible trajectory, opposing tank AI,
   destructible terrain, health, player win and loss, short rematches.
3. Dungeon Pinball: independently usable flippers, ball/table collisions,
   monster bumpers and targets, gates to changed tables, final boss, ball limit.
4. Train Dispatcher: tappable switches, moving trains, matching symbol stations,
   collision/misrouting penalties, increasing traffic and a completed shift.
5. Pocket Ecosystem: interacting plants, grazers and predators, interventions,
   resource limits, seasons, persistence, readable population feedback and both
   thriving and collapse dynamics. Ongoing play after initial seasonal challenge.
6. One-Screen Tactics: compact board, three enemies, readable next-turn intent,
   moving/pushing into hazards, protect an objective for five turns, wins/losses
   and multiple solvable layouts.

## Verified completion — 2026-09-10

All six 0.1.0 games are implemented, packaged, and checked. The release PBWs
were installed and driven on emery and gabbro; reports bind each run to the exact
PBW SHA-256. Every standalone source ZIP was extracted and rebuilt independently.

| Game | Core rules | Native emery + gabbro | Versioned PBW + source rebuild |
| --- | --- | --- | --- |
| Pocket Artillery | 200 aimed wins; 50 deliberate-miss losses; crater bounds; score clamp | Pass: controls, flight, opponent, win/loss, mid-flight resume | Pass |
| Space Salvage | 100 direct-route trials, 62 wins; collision, quota and deadline checks | Pass: buttons, touch navigation, cargo and escape | Pass |
| Dungeon Pinball | 47/100 reactive wins; 100/100 hands-off losses; bounded physics | Pass: independent touch and physical flippers, three chambers, bosses, resume | Pass |
| Train Dispatcher | 100 complete shifts; 50 unattended failures; junction locking | Pass: touch routing, 18 deliveries and misrouting loss | Pass |
| Pocket Ecosystem | 89/100 balanced introduction/rain trials; collapse and costs | Pass: planting, introductions, seasons, balance and continued simulation | Pass |
| One-Screen Tactics | All eight orientations solvable; idle losses; shove and bounds checks | Pass: five-turn win, touch wait loss, restart and resume | Pass |

The randomized suite ran 480,000 input/tick checks per engine under AddressSanitizer
and UndefinedBehaviorSanitizer. Persistence tests cover every interrupted write,
corrupt-selected-bank recovery with an intact fallback, and exact mid-flight
restore. Shared menu drag, pause, and relaunch were exercised natively. Watch
screenshots were visually inspected for layout and legibility on both shapes.

Artifacts and evidence are under `release/`, with a local install page and one
ZIP containing all six games. Source is in the private canonical repository
`lukeslp/pebble-pocket-arcade`. No public discovery or store publication was done.

Physical-watch validation remains unclaimed. Native emulator timing and policy
success rates are engineering checks, not measurements of human play difficulty.
