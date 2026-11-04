#!/usr/bin/env python3
"""Assemble portable native source from the shared shell and a game module."""
import json,re,shutil,subprocess,sys,uuid
from pathlib import Path
root=Path(__file__).resolve().parent.parent
name=sys.argv[1]
source=root/'games'/name
if not source.is_dir(): raise SystemExit('Unknown game: '+name)
project=root/'build'/name
src=project/'src/c'
src.mkdir(parents=True,exist_ok=True)
for stale in src.iterdir():
    if stale.is_file(): stale.unlink()
for folder in [root/'common',source]:
    for path in folder.iterdir():
        if path.suffix in ['.h','.c']:shutil.copy2(path,src/path.name)
# Games with layouts for 144x168 and the Time Round ship on all six watches;
# the rest stay on Time 2 and Round 2 until their screens are designed.
ALL_WATCHES={'pocket-artillery','space-salvage','one-screen-tactics'}
platforms=['basalt','chalk','diorite','emery','flint','gabbro'] if name in ALL_WATCHES else ['emery','gabbro']
version=re.search(r'#define UI_APP_VERSION "([^"]+)"',(root/'common/ui_style.h').read_text()).group(1)
meta={'name':name,'version':version,'author':'Luke Steuber','private':True,'dependencies':{},'pebble':{'displayName':name.replace('-',' ').title(),'uuid':str(uuid.uuid5(uuid.NAMESPACE_URL,'https://lukesteuber.com/pebble/'+name)),'sdkVersion':'3','enableMultiJS':False,'targetPlatforms':platforms,'watchapp':{'watchface':False},'messageKeys':[],'resources':{'media':[]}}}
shutil.copytree(source/'resources',project/'resources',dirs_exist_ok=True)
shutil.copytree(root/'common/resources',project/'resources',dirs_exist_ok=True)
# One portrait name; 96 px on Time 2 / Round 2, 72 px elsewhere, line art on black-and-white screens.
meta['pebble']['resources']['media']=[{'type':'bitmap','name':'MENU_ICON','file':'menu.png','menuIcon':True},
    {'type':'bitmap','name':'LUKE_PORTRAIT','file':'luke-96.png','targetPlatforms':['emery','gabbro']},
    {'type':'bitmap','name':'LUKE_PORTRAIT','file':'luke-72.png','targetPlatforms':['basalt','chalk']},
    {'type':'bitmap','name':'LUKE_PORTRAIT','file':'luke-72-bw.png','targetPlatforms':['diorite','flint']}]
(project/'package.json').write_text(json.dumps(meta,indent=2)+'\n')
shutil.copy2(root/'tools/wscript',project/'wscript')
subprocess.run([shutil.which('pebble') or 'pebble','build'],cwd=project,check=True)
