#!/usr/bin/env python

print "aaa"

def pairwise(xs):
  itnext = iter(xs).next
  while True:
    yield itnext(), itnext()

def aeha(high_low_list,tus=425.0):
  signal_to_bit = {(8,4):"L", (1,1):"0", (1,3):"1", (1,0):"T"}
  return ''.join(
    map(lambda x: signal_to_bit[x],
      tuple( 
        pairwise(
          ([int(round(x / tus)) for x in high_low_list + [0] ])))))

def aeha_hex(hls, tus=425.0):
  xs = aeha(hls,tus)
  def hexstr(xs):
    itn = iter(xs).next
    while True:
      b0 = itn()
      b1 = itn()
      b2 = itn()
      b3 = itn()
      yield hex(int(b3+b2+b1+b0, 2))[2]
  return xs[0] + ''.join(list(hexstr(xs[1:-1]))) + xs[-1]

def aeha_pp(hls, tus=425.0):
  hs = aeha_hex(hls, tus)
  print hs[0], hs[2]+hs[1], hs[4]+hs[3], hs[5]
  print hs[6]
  for i in range(7,len(hs)-1,4):
    print hs[i+1]+hs[i],
  print ''
  for i in range(7,len(hs)-1,4):
    print hs[i+3]+hs[i+2],
  print ''
  print hs[-1]

import serial

ard = serial.Serial(port='/dev/tty.usbmodem1431',baudrate=115200)

print "aeha reader"

while True:
  aeha_pp(map(lambda x: int(x), ard.readline().split(', ')[:-1])) 
  
