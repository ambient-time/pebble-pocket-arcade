"""Exercise the shared About surface through native input only. Luke Steuber."""
import time
from libpebble2.protocol.apps import AppRunState, AppRunStateRequest, AppRunStateStart


def exercise_about(button, expect, current, grab, pointer, width, watch, app):
    button('Back'); expect(screen=1)
    for _ in range(3): button('Down')
    selected = grab('about-selected')
    paused = current()
    button('Select'); expect(screen=4)
    grab('about')
    time.sleep(1.1)
    button('Up'); expect(screen=4)
    button('Down'); expect(screen=4)
    assert {k:v for k,v in current().items() if k != 'screen'} == {
        k:v for k,v in paused.items() if k != 'screen'}, 'About advanced the game'
    button('Select'); expect(screen=1)
    returned = grab('about-select-return')
    assert list(map(bytes, selected)) == list(map(bytes, returned)), 'About lost menu position'
    button('Select'); expect(screen=4)
    button('Back'); expect(screen=1)
    button('Select'); expect(screen=4)
    pointer(width//2,112,True); time.sleep(.07)
    pointer(width//2,112,False); time.sleep(.3)
    expect(screen=1)
    assert current() == paused, 'Returning from About changed game state'
    grab('about-touch-return')
    # The next row is Save and exit. Touch its actual native highlight.
    button('Down')
    pixels = grab('about-exit-selected')
    selected_rows = [y for y,row in enumerate(pixels)
        if sum(all(row[4*x+k]>240 for k in range(3)) for x in range(width)) > width//2]
    assert selected_rows, 'Exit row is not visible'
    y = selected_rows[len(selected_rows)//2]
    pointer(width//2,y,True); time.sleep(.07)
    pointer(width//2,y,False); time.sleep(.7)
    active = watch.send_and_read(AppRunState(data=AppRunStateRequest()),AppRunState)
    assert active.data.uuid != app, 'Native menu exit did not close the app'
    watch.send_packet(AppRunState(data=AppRunStateStart(uuid=app))); time.sleep(.8)
    expect(screen=0)
    resumed = current()
    assert 0 <= resumed['ticks'] - paused['ticks'] < 60, 'About time leaked into the saved game'
    if resumed['ticks'] == paused['ticks']:
        assert {k:v for k,v in resumed.items() if k != 'screen'} == {
            k:v for k,v in paused.items() if k != 'screen'}, 'About damaged the saved game'
    grab('about-exit-resumed')
    return ['About portrait and version', 'About freezes gameplay and ignores direction buttons',
            'Select/Back/touch return to the same selected menu row and scroll position',
            'repeated About bitmap load/release', 'touch Save and exit after About',
            'saved game resumes without About time or input leakage']
