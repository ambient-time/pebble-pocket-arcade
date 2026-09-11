#!/usr/bin/env python3
"""Check release checksums and rebuild each standalone source package."""
from pathlib import Path
import hashlib, json, shutil, subprocess, sys, zipfile
ROOT = Path(__file__).resolve().parent.parent
name = sys.argv[1]
version = sys.argv[2] if len(sys.argv) > 2 else '0.3.0'
base = ROOT / 'release' / name / version
sha = lambda p: hashlib.sha256(p.read_bytes()).hexdigest()
for line in (base / 'SHA256SUMS.txt').read_text().splitlines():
    expected, relative = line.split('  ', 1)
    assert sha(base / relative) == expected, relative
work = ROOT / 'build' / 'portable-check' / name
if work.exists():
    shutil.rmtree(work)
with zipfile.ZipFile(base / f'{name}-{version}-source.zip') as archive:
    archive.extractall(work.parent)
with (work / 'rebuild.log').open('w') as log:
    subprocess.run(['pebble', 'build'], cwd=work, stdout=log, stderr=subprocess.STDOUT, check=True)
pbw = work / 'build' / f'{name}.pbw'
with zipfile.ZipFile(pbw) as archive:
    for target in ['emery', 'gabbro']:
        assert len(archive.read(f'{target}/pebble-app.bin')) > 100
        assert len(archive.read(f'{target}/app_resources.pbpack')) > 0
    appinfo = json.loads(archive.read('appinfo.json'))
    assert set(appinfo['targetPlatforms']) == {'emery', 'gabbro'}
report = {'game': name, 'passed': True, 'checks': ['all release checksums', 'standalone source extraction', 'standalone SDK build', 'both native targets and resources'], 'releasePBWSHA256': sha(base / f'{name}-{version}.pbw'), 'sourceZIPSHA256': sha(base / f'{name}-{version}-source.zip'), 'rebuiltPBWSHA256': sha(pbw), 'note': 'SDK rebuild metadata may change binary hashes; native gameplay reports cover the release PBW.'}
out = ROOT / 'release' / 'validation' / version
out.mkdir(parents=True, exist_ok=True)
(out / f'{name}-portable.json').write_text(json.dumps(report, indent=2) + '\n')
print('PASS portable release', name)
