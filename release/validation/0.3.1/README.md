# Pocket Arcade 0.3.1 verification

All six PBWs passed their complete native input campaigns on emery and gabbro.
Every report records the exact installed PBW SHA-256, checks, logs, and native
screenshots. All six standalone source ZIPs were extracted and rebuilt with
SDK 4.33.1. Source, build script, and raw resource contents match source commit
6eb07a9b690b181af68c3b3e27f0b89689aaf46b.

About is one screen with the original 96-pixel Luke Steuber portrait, 28-pixel
bold heading, 24-pixel bold name, and 18-pixel bold version and return hint.
The original PNG is included unchanged in every source ZIP. Each target has a
canonical `screenshots/<target>-about.png`; these were inspected at native size.
All same-target About captures are also byte-identical across the six games.

The About checks in every native report cover:

- Paused gameplay, with direction input ignored and unchanged recorded state.
- Select, Back, and touch return to the same menu row and scroll position.
- Repeated portrait creation and release through three open/close cycles.
- Touch Save and exit after About, followed by saved-game resume without time
  or input leakage from the About screen.

The portrait is loaded on demand, destroyed on return, and safe to omit if
allocation fails. The deferred native-menu exit fix is retained. UUIDs, game
logic, renderer geometry, and save format sources are unchanged from 0.3.0.
All original native game campaigns remain, including win/loss or continued play,
pause, touch controls, replay, and save/resume.

Core AddressSanitizer and UndefinedBehaviorSanitizer checks pass for all six
games. Randomized checks pass 480,000 input/tick invariants per game, 2.88 million
total. The compiler enforces a 512-byte application stack-frame maximum; the
largest reported frame is 368 bytes. SDK-reported static RAM footprints
range from 12,101 to 13,513 bytes. These are build measurements; they do
not measure dynamic portrait allocation or physical-watch runtime memory.

`candidate-audit.json` records current hashes, targets, UUIDs, and build memory.
`source-provenance.json` records source and resource comparisons, the original
portrait hash, and unchanged gameplay/save source checks. All versioned release
checksums verify. The Time 2 Trains emulator initially refused its local
connection before installation; its successful retry used the same PBW. That
environment-only failure is retained in `emulator-launch-retry.log`.

Physical-watch readability, contrast, and runtime memory remain separate.
This is a local candidate: no 0.3.1 upload or publication was performed. The
local catalog prefers Pocket Artillery; Crosswind remains a separate legacy
identity and its external listing was not changed.
