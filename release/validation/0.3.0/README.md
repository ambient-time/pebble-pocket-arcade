# Pocket Arcade 0.3.0 verification

All six PBWs passed their complete native input campaigns on emery and gabbro.
Each game's reports record the exact installed PBW SHA-256, checks, logs and
native-size screenshots. The standalone source ZIPs were independently extracted
and rebuilt; their contents also match the committed authored sources.

The font floor is Gothic 18 Bold for compact counters and labels, with Gothic
24 Bold for instructions/actions and Gothic 28 Bold for short titles. Time 2
worlds are 8% larger. Round 2 uses 15% for Salvage and Ecosystem, 20% for Pinball,
and 25% for Artillery, Trains and Tactics to keep required geometry on-screen.
Touch and replay hit tests use the same scale as rendering.

- Core AddressSanitizer/UndefinedBehaviorSanitizer checks: pass for all six games.
- Randomized invariants: 480,000 per game, 2.88 million total, pass.
- Native campaigns: all twelve pass, including outcomes, pause, save/resume and
  game-specific touch controls.
- Shared native-menu exit regression: three touch exits and exact resumed states
  on each shape, exercised with Tactics.
- Stack guard: compiler rejects application frames over 512 bytes; largest is
  368 bytes in every game. SDK-reported RAM footprints are 11,625–13,037 bytes.
- Portable source rebuilds, source-content comparisons and release checksums:
  pass for all six games.
- Screenshots: inspected at native size for instructions, playfields, menu rows,
  outcomes and replay controls on both screen shapes.

`candidate-audit.json` records targets, UUIDs, hashes, sizes and build memory
measurements. `source-provenance.json` records source ZIP content comparisons to
commit e1cdfb972610abd0c17f9147e1f24940ed5e415d. The native report for each game
is in its versioned release folder. Closed-app time and game save formats are
unchanged.

Physical-watch readability and runtime memory measurements remain separate.
No 0.3.0 upload or publication is implied; the existing Store drafts remain 0.2.0.
