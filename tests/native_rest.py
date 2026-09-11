"""Luke Steuber. Drive the installed PBW through native buttons and QMP touch.

Run with pebble-tool's Python after `pebble build`. Each run owns a fresh profile.
No AppMessage test backdoor is compiled into the game.
"""
import argparse
import sys
import datetime
import hashlib
import json
import os
from pathlib import Path
import re
import socket
import subprocess
import tempfile
import time
from types import SimpleNamespace
from uuid import UUID

SDK_VERSION = '4.33.1'
SDK = Path.home() / 'Library/Application Support/Pebble SDK/SDKs' / SDK_VERSION
os.environ['PEBBLE_QEMU_PATH'] = str(SDK / 'toolchain/bin/qemu-pebble')
os.environ['PATH'] = str(SDK / 'toolchain/bin') + os.pathsep + os.environ['PATH']
import png
from pebble_tool.commands.screenshot import ScreenshotCommand
from pebble_tool.commands.emucontrol import send_data_to_qemu
import pebble_tool.sdk.emulator as emulator
from libpebble2.services.install import AppInstaller
from libpebble2.protocol.apps import AppRunState, AppRunStateStart, AppRunStateStop
from libpebble2.protocol.logs import AppLogMessage, AppLogShippingControl
from about import exercise_about
from libpebble2.communication.transports.qemu.protocol import QemuButton

ROOT = Path(__file__).resolve().parent.parent / 'build' / sys.argv[1]
PBW = ROOT / 'build' / (ROOT.name + '.pbw')
APP = UUID(json.loads((ROOT / 'package.json').read_text())['pebble']['uuid'])


def run(platform):
    installed_sha = hashlib.sha256(PBW.read_bytes()).hexdigest()
    out = ROOT / 'build/evidence'
    out.mkdir(exist_ok=True)
    state = Path(tempfile.mkdtemp(prefix='emu-', dir=ROOT / 'build'))
    # macOS Unix sockets have short paths; a loopback QMP port avoids that limit.
    with socket.socket() as reservation:
        reservation.bind(('127.0.0.1', 0))
        qmp_port = reservation.getsockname()[1]
    def persist(target, version=None):
        p = state / target
        p.mkdir(exist_ok=True)
        return str(p)
    emulator.get_sdk_persist_dir = persist
    emulator.get_emulator_info_path = lambda: str(state / 'emulators.json')
    emulator.get_default_account = lambda: SimpleNamespace(is_logged_in=False)
    bridge = (out / f'{platform}-bridge.log').open('w')
    emulator.ManagedEmulatorTransport._get_output = lambda self: bridge
    cmd = ScreenshotCommand()
    cmd._set_debugging(0)
    def shutdown():
        info = emulator.get_emulator_info(platform, SDK_VERSION)
        if info:
            for key in ('qemu', 'pypkjs', 'websockify'):
                pid = info.get(key, {}).get('pid')
                if pid:
                    ps = subprocess.run(['ps', '-p', str(pid), '-o', 'command='], capture_output=True, text=True)
                    if ps.returncode == 0 and str(state / platform) not in ps.stdout:
                        raise RuntimeError(f'PID {pid} belongs to a different profile')
        cmd._shutdown_platform_emulator(platform, SDK_VERSION)
    original_popen = subprocess.Popen
    def popen(argv, *args, **kwargs):
        if isinstance(argv, list) and Path(argv[0]).name == 'qemu-pebble':
            argv = argv + ['-qmp', f'tcp:127.0.0.1:{qmp_port},server=on,wait=off']
        return original_popen(argv, *args, **kwargs)
    try:
        subprocess.Popen = popen
        watch = cmd._connect_emulator(platform, SDK_VERSION)
        cmd.pebble = watch
    except BaseException:
        shutdown()
        bridge.close()
        raise
    finally:
        subprocess.Popen = original_popen
    qmp_socket = socket.create_connection(('127.0.0.1', qmp_port), timeout=5)
    qmp = qmp_socket.makefile('rwb')
    assert 'QMP' in json.loads(qmp.readline())
    def execute(command, arguments=None):
        qmp.write((json.dumps({'execute': command, 'arguments': arguments or {}}) + '\n').encode())
        qmp.flush()
        while True:
            response = json.loads(qmp.readline())
            if 'error' in response:
                raise RuntimeError(response)
            if 'return' in response:
                return response['return']
    execute('qmp_capabilities')
    logs, frames = [], []
    def log(packet):
        line = str(packet.message)
        logs.append(line)
        if 'AR ' in line:
            print(line, flush=True)
    handle = watch.register_endpoint(AppLogMessage, log)
    watch.send_packet(AppLogShippingControl(enable=True))
    def current():
        for line in reversed(logs):
            if 'AR s=' in line:
                keys={'s':'screen','st':'status','t':'ticks','sc':'score','a':'d0','b':'d1','c':'d2','d':'d3','e':'hp','f':'enemy'}
                state = {keys.get(k,k):int(v) for k,v in re.findall(r'(\w+)=(-?\d+)',line)}
                return state
        raise AssertionError('No native state received')
    def count():
        return sum('AR s=' in line for line in logs)
    def response(previous):
        deadline = time.monotonic() + 4
        while count() == previous and time.monotonic() < deadline:
            time.sleep(.02)
        assert count() > previous, 'Native input did not respond'
        time.sleep(.1)
    def button(name, hold=.07):
        previous = count()
        send_data_to_qemu(watch.transport, QemuButton(state=getattr(QemuButton.Button, name)))
        time.sleep(hold)
        send_data_to_qemu(watch.transport, QemuButton(state=0))
        if previous:
            response(previous)
        else:
            time.sleep(.2)
    def expect(**values):
        actual = current()
        assert all(actual[k] == v for k, v in values.items()), (values, actual)
    def grab(name):
        time.sleep(.15)
        pixels = cmd._grab_processed_image(argparse.Namespace(no_correction=True, scale=1, no_open=True, v=0), show_progress=False)
        file = out / f'{platform}-{name}.png'
        png.from_array(pixels, mode='RGBA;8').save(str(file))
        frames.append(str(file.relative_to(ROOT)))
        return pixels
    width, height = (200, 228) if platform == 'emery' else (260, 260)
    x, y = width // 2, 105 if platform == 'emery' else 126
    round_scale={'space-salvage':115,'pocket-ecosystem':115,'dungeon-pinball':120,'train-dispatcher':125}
    factor=108 if platform=='emery' else round_scale[ROOT.name]
    fx=(width-176*factor//100)//2;fy=(height-144*factor//100)//2
    def game_point(x,y):
        return fx+x*factor//100,fy+y*factor//100
    def pointer(px, py, down):
        events = [{'type': 'abs', 'data': {'axis': axis, 'value': round(value * 32767 / (size - 1))}}
                  for axis, value, size in [('x', px, width), ('y', py, height)]]
        events.append({'type': 'btn', 'data': {'button': 'left', 'down': down}})
        execute('input-send-event', {'events': events})
    def touch(points, interval=.05):
        previous = count()
        for px, py in points:
            pointer(px, py, True)
            time.sleep(interval)
        pointer(*points[-1], False)
        response(previous)
    def world():
        ends=[i for i,s in enumerate(logs) if 'AR end' in s]
        if not ends:return ''
        end=ends[-1];start=ends[-2]+1 if len(ends)>1 else 0
        return ''.join(s.split('AR part ',1)[1] for s in logs[start:end] if 'AR part ' in s)
    def restart():
        if current()['screen']==3:
            button('Select');expect(screen=0,status=0);return
        if current()['status'] and not (ROOT.name=='pocket-ecosystem' and current()['status']==1):
            time.sleep(.6);button('Select')
        else:
            button('Back');button('Down');button('Select');expect(screen=3);button('Select')
        expect(screen=0,status=0)
    def direct_tap(x,y):
        x,y=game_point(x-fx,y-fy)
        pointer(x,y,True);time.sleep(.04);pointer(x,y,False);time.sleep(.15)
    checks=[]
    try:
        time.sleep(4);button('Back')
        AppInstaller(watch,str(PBW)).install()
        watch.send_packet(AppRunState(data=AppRunStateStart(uuid=APP)))
        time.sleep(1);expect(screen=2);grab('rules')
        button('Select');expect(screen=0);grab('play')
        checks+=exercise_about(button,expect,current,grab,pointer,width,watch,APP)
        button('Back');expect(screen=1);grab('pause')
        before=current()['ticks'];time.sleep(1);assert current()['ticks']==before
        # Finger scrolls the actual native list without activating a row.
        first=grab('menu-before')
        pointer(width//2,height-70,True);time.sleep(.7)
        for y in range(height-70,55,-10):pointer(width//2,y,True);time.sleep(.04)
        pointer(width//2,55,False);time.sleep(.3)
        expect(screen=1);grab('menu-scrolled')
        button('Back');expect(screen=0)
        watch.send_packet(AppRunState(data=AppRunStateStop(uuid=APP)));time.sleep(.4)
        previous=current()['ticks']
        watch.send_packet(AppRunState(data=AppRunStateStart(uuid=APP)));time.sleep(.7)
        expect(screen=0);assert current()['ticks']>=previous
        checks+=['rules','playfield','pause freezes','native touch menu scroll without activation','save and relaunch']
        game=ROOT.name
        if game=='train-dispatcher':
            button('Down');expect(d3=1);button('Select');expect(d1=1)
            button('Up');expect(d3=0)
            deadline=time.monotonic()+180
            captured=False
            while not current()['status'] and time.monotonic()<deadline:
                trains=[list(map(int,t.split(','))) for t in world().strip(';').split(';') if t and ',' in t]
                for junction in range(3):
                    incoming=[t for t in trains if t[1]==junction*2]
                    if not incoming:continue
                    kind=max(incoming,key=lambda t:t[2])[3]
                    desired=int(kind==0) if junction==0 else int(kind==1) if junction==1 else 0
                    if current()['d'+str(junction)]!=desired:direct_tap(fx+48+junction*48,fy+72)
                if not captured and current()['score']>=300:grab('traffic');captured=True
                time.sleep(.12)
            expect(status=1);grab('completed-shift')
            checks+=['button switch selection','touch routing','18 deliveries','completed shift']
            restart();deadline=time.monotonic()+90
            while not current()['status'] and time.monotonic()<deadline:time.sleep(.2)
            expect(status=2);grab('failed-shift');checks+=['misrouting loss']
        elif game=='space-salvage':
            button('Down');button('Select');grab('button-thrust')
            def info():
                s=world();d={k:int(v) for k,v in re.findall(r'(\w+)=(-?\d+)',s)}
                d['cargo']=list(map(int,re.search(r'cargo=([\d,]+)',s).group(1).split(',')))
                d['pos']=list(map(int,re.search(r'pos=([\d,]+)',s).group(1).split(',')))
                return d
            def travel(x,y):
                direct_tap(fx+x,fy+y)
                until=time.monotonic()+12
                while not current()['status'] and info()['auto'] and time.monotonic()<until:time.sleep(.12)
            for attempt in range(4):
                for trip in range(18):
                    if current()['status'] or info()['bank']==5:break
                    d=info()
                    if d['carry']<0:
                        missing=next((i for i,v in enumerate(d['cargo']) if v==0),None)
                        if missing is None:break
                        travel(d['pos'][missing*2],d['pos'][missing*2+1])
                        if trip==0:grab('tether')
                    travel(20,72)
                    if trip==0:grab('first-delivery')
                if not current()['status'] and info()['bank']==5:travel(20,18)
                if current()['status']==1:break
                grab('attempt-ended')
                restart()
            expect(status=1);grab('escaped');checks+=['button turn and thrust','touch destination steering','cargo tether','five deliveries','quota gated escape']
        elif game=='dungeon-pinball':
            button('Select');time.sleep(.3)
            send_data_to_qemu(watch.transport,QemuButton(state=QemuButton.Button.Up|QemuButton.Button.Down))
            time.sleep(.3);grab('both-flippers')
            send_data_to_qemu(watch.transport,QemuButton(state=0))
            pointer(*game_point(45,122),True);time.sleep(.3);expect(hp=1,enemy=0);grab('touch-left');pointer(*game_point(45,122),False);time.sleep(.15)
            pointer(*game_point(135,122),True);time.sleep(.3);expect(hp=0,enemy=1);grab('touch-right');pointer(*game_point(135,122),False);time.sleep(.15)
            watch.send_packet(AppRunState(data=AppRunStateStop(uuid=APP)));time.sleep(.4)
            watch.send_packet(AppRunState(data=AppRunStateStart(uuid=APP)));time.sleep(.7)
            expect(screen=0,hp=0,enemy=0);checks+=['flippers released after resume']
            for attempt in range(8):
                deadline=time.monotonic()+180;mask=-1;stage=-1;last_launch=0
                while not current()['status'] and time.monotonic()<deadline:
                    d=current();now=time.monotonic()
                    ready=d['d1']>10000 and d['d3']>0
                    wanted=(QemuButton.Button.Up if ready and d['d0']<9000 else 0)|(QemuButton.Button.Down if ready and d['d0']>8600 else 0)
                    if wanted!=mask:send_data_to_qemu(watch.transport,QemuButton(state=wanted));mask=wanted
                    if 'wait=1' in world() and now-last_launch>.5:
                        send_data_to_qemu(watch.transport,QemuButton(state=wanted|QemuButton.Button.Select));time.sleep(.05)
                        send_data_to_qemu(watch.transport,QemuButton(state=wanted));last_launch=now
                    if d['stage']!=stage:stage=d['stage'];grab('attempt-'+str(attempt)+'-chamber-'+str(stage+1))
                    time.sleep(.03)
                send_data_to_qemu(watch.transport,QemuButton(state=0))
                if current()['status']==1:break
                grab('lost-run-'+str(attempt));restart()
            expect(status=1);grab('dungeon-cleared');checks+=['launch','simultaneous physical flippers','independent held touch flippers','reactive flipper timing','three distinct chambers','monster targets and bosses','complete dungeon']
            # Hold Select before the final drain; its eventual release must not replay.
            restart();held=False;deadline=time.monotonic()+100
            while not current()['status'] and time.monotonic()<deadline:
                if 'wait=1' in world():
                    last_ball='lives=1' in world()
                    button('Select')
                    if last_ball:
                        send_data_to_qemu(watch.transport,QemuButton(state=QemuButton.Button.Select))
                        held=True
                time.sleep(.08)
            expect(status=2);assert held
            time.sleep(.8)
            send_data_to_qemu(watch.transport,QemuButton(state=0));time.sleep(.4)
            expect(status=2,screen=0);grab('held-select-result')
            button('Select');expect(status=0,screen=0)
            checks+=['passive last-ball loss','held Select crossing terminal does not replay','fresh Select replays once']

        elif game=='pocket-ecosystem':
            button('Down');expect(d0=1)
            button('Select',hold=.6);expect(d1=1)
            direct_tap(fx+22,fy+130);expect(d1=0)
            direct_tap(fx+18,fy+15);grab('seeded-cell')
            for attempt in range(4):
                for tool, preferred in [(1,15),(2,5)]:
                    direct_tap(fx+22+44*tool,fy+130)
                    budget=current()['d3'];cost=3 if tool==1 else 5
                    while budget < cost:
                        time.sleep(.2);budget=current()['d3']
                    for offset in range(24):
                        cell=(preferred+offset)%24
                        direct_tap(fx+4+(cell%6)*28+14,fy+1+(cell//6)*28+14)
                        if current()['d3']<=budget-cost:break
                    assert current()['d3']<=budget-cost
                grab('introduced-species-'+str(attempt))
                deadline=time.monotonic()+200;season=-1
                while current()['status']==0 and time.monotonic()<deadline:
                    if current()['d2']<30 and current()['d3']>=3:
                        direct_tap(fx+154,fy+130)
                    if current()['stage']!=season:season=current()['stage'];grab('attempt-'+str(attempt)+'-season-'+str(season))
                    time.sleep(.15)
                if current()['status']==1:break
                grab('habitat-ended-'+str(attempt));restart()
            expect(status=1);grab('balanced')
            before=current()['ticks'];time.sleep(4);assert current()['ticks']>before
            checks+=['button cursor and tool','touch tool and planting','introduce grazers and predators','seasonal population dynamics','60-second balanced habitat','continued simulation after goal']
            restart();deadline=time.monotonic()+65
            while current()['status']==0 and time.monotonic()<deadline:time.sleep(.2)
            expect(status=2);time.sleep(.6);grab('extinction-result')
            direct_tap(fx+88,fy+115);expect(status=0,screen=0)
            checks+=['extinction result','ongoing habitat restart confirmation','one-tap new habitat']
        assert not any('fault' in s.lower() or 'crash' in s.lower() for s in logs)
        report={'platform':platform,'sdk':SDK_VERSION,'pbwSHA256':installed_sha,'passed':True,'checks':checks,'frames':frames,'logs':logs}
        (out/f'{platform}-report.json').write_text(json.dumps(report,indent=2));print('PASS',game,platform)
    finally:
        try:grab('final-screen')
        except Exception:pass
        (out/f'{platform}-app.log').write_text('\n'.join(logs))
        watch.unregister_endpoint(handle);cmd._close_pebble_connection(watch);cmd.pebble=None
        qmp.close();qmp_socket.close();shutdown();bridge.close()
if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('game',choices=['train-dispatcher','space-salvage','dungeon-pinball','pocket-ecosystem']);parser.add_argument('platform',choices=['emery','gabbro'])
    run(parser.parse_args().platform)
