#!/usr/bin/python

import sys
import mraa
import time

global count
count = 0

def countPulse(channel):
   global count
   count = count+1
  
pin = mraa.Gpio(16)
pin.dir(mraa.DIR_IN)
pin.isr(mraa.EDGE_BOTH, countPulse, None)
while True:

	time.sleep(15)
	target = open("/IoT/data/dat.txt", "w")
	target.truncate()
	target.write(str(count))
	target.close()
	count = 0
	pin.isrExit()
	break
	
