#!/usr/bin/env python3
"""Original 25px launcher silhouettes by Luke Steuber."""
from pathlib import Path
import struct,zlib
icons={
'space-salvage':['0001000','0011100','0011100','0111110','1111111','0010100','0100010'],
'pocket-artillery':['0000110','0001100','0011000','0111100','1111110','1111111','0101010'],
'dungeon-pinball':['0011100','0100010','0101010','0100010','0100010','0110110','0001000'],
'train-dispatcher':['0111110','0101010','0111110','0011100','0111110','0101010','1000001'],
'pocket-ecosystem':['0000010','0001110','0111100','0011000','0111110','1101001','0001000'],
'one-screen-tactics':['0011100','0101010','0101010','0111110','0011100','0011100','0111110']}
def chunk(kind,data):return struct.pack('!I',len(data))+kind+data+struct.pack('!I',zlib.crc32(kind+data)&0xffffffff)
for name,rows in icons.items():
 data=bytearray()
 for y in range(25):
  data.append(0)
  for x in range(25):
   on=2<=x<23 and 2<=y<23 and rows[(y-2)//3][(x-2)//3]=='1'
   data.extend((255,255,255,255) if on else (0,0,0,0))
 png=b'\x89PNG\r\n\x1a\n'+chunk(b'IHDR',struct.pack('!IIBBBBB',25,25,8,6,0,0,0))+chunk(b'IDAT',zlib.compress(data))+chunk(b'IEND',b'')
 target=Path(__file__).resolve().parent.parent/'games'/name/'resources/menu.png'
 target.parent.mkdir(exist_ok=True);target.write_bytes(png)
