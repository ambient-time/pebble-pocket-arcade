#!/usr/bin/env python3
"""Create the local six-game install page and complete download bundle."""
from pathlib import Path
import hashlib, html, json, zipfile
ROOT = Path(__file__).resolve().parent.parent
out = ROOT / 'release'
rows = [
    ('space-salvage', 'Space Salvage', 'Tow five crates home, then escape the wreck field.', '150-second limit', 'play'),
    ('pocket-artillery', 'Pocket Artillery', 'Trade tank shots across a hillside that crumbles with each hit.', 'A few minutes', 'aim'),
    ('dungeon-pinball', 'Dungeon Pinball', 'Time your flippers through three skull-filled chambers.', 'About 1–3 minutes', 'play'),
    ('train-dispatcher', 'Train Dispatcher', 'Route eighteen trains before the third mistake ends your shift.', 'About 2 minutes', 'traffic'),
    ('pocket-ecosystem', 'Pocket Ecosystem', 'Introduce species and tend a habitat through four seasons.', '60-second balance goal; ongoing', 'balanced'),
    ('one-screen-tactics', 'One-Screen Tactics', 'Shove enemies into pits and protect the beacon for five turns.', 'Five turns, no timer', 'board'),
]
cards=[]
for slug,title,desc,duration,frame in rows:
    base=out/slug/'0.1.0'
    manifest=json.loads((base/'manifest.json').read_text())
    assert json.loads((out/'validation'/f'{slug}-portable.json').read_text())['passed']
    prefix=f'{slug}/0.1.0'
    image=f'{prefix}/screenshots/emery-{frame}.png'
    assert (out/image).exists(),image
    cards.append(f'<article><img width="200" height="228" src="{image}" alt="{html.escape(title)} running on Pebble Time 2"><div><h2>{title}</h2><p>{desc}</p><p class="meta">{duration}</p><a class="install" href="{prefix}/{slug}-0.1.0.pbw" download>Download PBW<span class="sr"> for {title}</span></a><p class="links"><a href="{prefix}/README.md">Rules and controls</a> · <a href="{prefix}/{slug}-0.1.0-source.zip" download>Source ZIP</a></p></div></article>')
page="""<!doctype html><html lang="en"><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>Pocket Arcade · Luke Steuber</title><style>
:root{color-scheme:dark}*{box-sizing:border-box}body{margin:0;background:#0c1020;color:#eef4ff;font:16px/1.6 system-ui,sans-serif}main{max-width:1120px;margin:auto;padding:44px 24px}h1{font-size:clamp(32px,6vw,58px);line-height:1.1;margin:10px 0 20px}h2{font-size:23px;margin:0}.kicker{color:#75e7ce;letter-spacing:.15em;font-size:12px;text-transform:uppercase}.intro{max-width:750px;color:#c1cee4}a{color:#82e8d1;text-underline-offset:4px}a:focus-visible{outline:3px solid #ffd17a;outline-offset:5px}.deck{display:grid;grid-template-columns:1fr 1fr;gap:18px;margin:34px 0}article{display:flex;gap:22px;align-items:center;border:1px solid #34415c;border-radius:12px;padding:22px;background:#141b2e}article img{width:120px;height:auto;image-rendering:pixelated;flex-shrink:0}article p{margin:10px 0}.meta,.links{font-size:13px;color:#b4c4dc}.install{display:inline-block;background:#80e3ce;color:#072019;padding:9px 15px;border-radius:6px;font-weight:700;text-decoration:none}.bundle{display:inline-block;margin:12px 0}.sr{position:absolute;width:1px;height:1px;padding:0;margin:-1px;overflow:hidden;clip:rect(0,0,0,0);white-space:nowrap;border:0}footer{border-top:1px solid #34415c;padding-top:20px;color:#b4c4dc;font-size:14px}@media(max-width:900px){.deck{grid-template-columns:1fr}}@media(max-width:440px){main{padding:28px 16px}article{padding:16px;gap:14px}article img{width:85px}h2{font-size:20px}}
</style><main><div class="kicker">Luke Steuber · Version 0.1.0</div><h1>Pocket Arcade</h1><p class="intro">Six small games for Pebble Time 2 and Pebble Round 2. Each has buttons, touch, a pause menu, and a saved game to return to.</p><a class="bundle" href="pocket-arcade-0.1.0.zip" download>Download all six games, source, and evidence</a><p class="intro">Open a PBW with the Pebble phone app to install it on your connected watch. Touch requires firmware 4.33.2 or later. These files passed native emulator checks on both watch shapes; physical-watch testing is still pending.</p><section class="deck" aria-label="Games">""" + ''.join(cards) + '</section><footer>Local release bundle · MIT license · No Pebble Store publication. Each game folder includes checksums, controls, source, and verification evidence.</footer></main></html>'
(out/'index.html').write_text(page)
archive=out/'pocket-arcade-0.1.0.zip'
with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED) as z:
    for path in sorted(out.rglob('*')):
        if path.is_file() and path != archive and path.name != 'SHA256SUMS.txt':
            z.write(path,str(Path('pocket-arcade-0.1.0')/path.relative_to(out)))
    for slug,*_ in rows:
        path=out/slug/'0.1.0/SHA256SUMS.txt'
        z.write(path,str(Path('pocket-arcade-0.1.0')/path.relative_to(out)))
files=sorted(p for p in out.rglob('*') if p.is_file() and p != out/'SHA256SUMS.txt')
(out/'SHA256SUMS.txt').write_text(''.join(f'{hashlib.sha256(p.read_bytes()).hexdigest()}  {p.relative_to(out)}\n' for p in files))
print(archive)
