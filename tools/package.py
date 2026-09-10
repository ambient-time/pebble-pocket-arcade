#!/usr/bin/env python3
"""Package only the exact native PBW that passed on both targets."""
from pathlib import Path
import hashlib,json,shutil,sys,zipfile,subprocess
ROOT=Path(__file__).resolve().parent.parent
name=sys.argv[1];project=ROOT/'build'/name;binary=project/'build'/(name+'.pbw')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
version=json.loads((project/'package.json').read_text())['version']
reports=[]
for platform in ['emery','gabbro']:
 p=project/'build/evidence'/f'{platform}-report.json';r=json.loads(p.read_text())
 if not r.get('passed') or r['pbwSHA256']!=sha(binary):raise SystemExit(f'{name}: no passing exact-PBW report for {platform}')
 reports.append((p,r))
with zipfile.ZipFile(binary) as z:
 for platform in ['emery','gabbro']:
  if f'{platform}/pebble-app.bin' not in z.namelist():raise SystemExit('Missing target '+platform)
source_hashes={str(p.relative_to(project)):sha(p) for p in sorted((project/'src/c').glob('*'))}
for folder in [ROOT/'common',ROOT/'games'/name]:
 for p in folder.iterdir():
  if p.suffix in ['.h','.c'] and sha(p)!=source_hashes['src/c/'+p.name]:raise SystemExit('Generated source drift: '+str(p))
out=ROOT/'release'/name/version
if out.exists():raise SystemExit('Immutable release already exists: '+str(out))
out.mkdir(parents=True)
shutil.copy2(binary,out/f'{name}-{version}.pbw')
shutil.copy2(ROOT/'games'/name/'README.md',out/'README.md')
shutil.copy2(ROOT/'LICENSE',out/'LICENSE')
for p,r in reports:
 shutil.copy2(p,out/p.name)
 for frame in r['frames']:
  image=project/frame
  (out/'screenshots').mkdir(exist_ok=True)
  shutil.copy2(image,out/'screenshots'/image.name)
with zipfile.ZipFile(out/f'{name}-{version}-source.zip','w',zipfile.ZIP_DEFLATED) as z:
 for p in [project/'package.json',project/'wscript',*(project/'src').rglob('*'),*(project/'resources').rglob('*')]:
  if p.is_file():z.write(p,str(Path(name)/p.relative_to(project)))
 z.write(ROOT/'games'/name/'README.md',f'{name}/README.md');z.write(ROOT/'LICENSE',f'{name}/LICENSE')
manifest={'game':name,'version':version,'author':'Luke Steuber','targets':['emery','gabbro'],'pbwSHA256':sha(binary),'sourceCommit':subprocess.check_output(['git','rev-parse','HEAD'],cwd=ROOT,text=True).strip(),'nativeChecks':{r['platform']:r['checks'] for _,r in reports},'physicalWatchValidated':False}
(out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
files=sorted(p for p in out.rglob('*') if p.is_file())
(out/'SHA256SUMS.txt').write_text(''.join(f'{sha(p)}  {p.relative_to(out)}\n' for p in files))
print(out)
