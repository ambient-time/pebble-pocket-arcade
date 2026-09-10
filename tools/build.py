#!/usr/bin/env python3
"""Assemble portable native source from the shared shell and a game module."""
import json,shutil,subprocess,sys,uuid
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
meta={'name':name,'version':'0.1.0','author':'Luke Steuber','private':True,'dependencies':{},'pebble':{'displayName':name.replace('-',' ').title(),'uuid':str(uuid.uuid5(uuid.NAMESPACE_URL,'https://lukesteuber.com/pebble/'+name)),'sdkVersion':'3','enableMultiJS':False,'targetPlatforms':['emery','gabbro'],'watchapp':{'watchface':False},'messageKeys':[],'resources':{'media':[]}}}
shutil.copytree(source/'resources',project/'resources',dirs_exist_ok=True)
meta['pebble']['resources']['media']=[{'type':'bitmap','name':'MENU_ICON','file':'menu.png','menuIcon':True}]
(project/'package.json').write_text(json.dumps(meta,indent=2)+'\n')
shutil.copy2(root/'tools/wscript',project/'wscript')
subprocess.run(['/Users/luke/.local/bin/pebble','build'],cwd=project,check=True)
