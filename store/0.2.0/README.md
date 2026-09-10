# Pebble drafts — 0.2.0

All six apps were uploaded on September 10, 2026 to Luke Steuber’s Pebble developer account. Each app is unlisted (`visible: false`), and each 0.2.0 release is Draft (`is_published: false`). No publication action was taken.

`publication.json` records the app and release IDs, edit links, target support, uploaded PBW hashes, and fresh verification. The per-game JSON files contain the exact listing copy and asset sources; `*-upload.json` preserves the server’s creation receipts.

Each listing has five original native PNG screenshots for Time 2 and five for Round 2. Both saved gallery panels were reopened. The two icon slots use the original 25×25 launcher artwork accepted by the store. Optional banners and higher-resolution promotional icons remain unset.

The packages are the frozen 0.2.0 PBWs from `release/`; they were neither rebuilt nor rewritten. The server accepted each PBW, extracted its stable UUID, version and two targets, and returned `has_pbw: true`. Attempts to download the returned PBW asset URLs returned HTTP 404, so a server-download checksum match has not been established. Uploaded-byte hashes are recorded separately. Physical-watch testing remains unperformed.

## Repeat uploads safely

The installed pebble-tool new-app creation method hardcodes `visible` and `isPublished` to `true`, even though its command help advertises unpublished defaults. Do not use that path for drafts without correcting and checking it first.

These uploads used the same authenticated dashboard endpoint with both fields explicitly set to `false`, followed by fresh account reads and browser checks. Authentication came from the existing Pebble CLI account; no credentials are stored here. Before a retry, check the saved UUID and app ID to avoid duplicate listings. Do not publish these releases without Luke’s authorization.
