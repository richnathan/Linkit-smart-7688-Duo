#!/usr/bin/python

# isr.py script for counting clicks from interrupt using mraa and GPIO pin.  The script counts for X seconds and then saves the number to a 
# flatfile /IoT/data/dat.txt.  


import sys
import mraa
import time

global count
count = 0

def countPulse(channel):
   global count
   count = count+1
  
pin = mraa.Gpio(17)
pin.dir(mraa.DIR_IN)
pin.isr(mraa.EDGE_BOTH, countPulse, None)
while True:

	time.sleep(5)
	target = open("/IoT/data/dat.txt", "w")
	target.truncate()
	target.write(str(count))
	target.close()
	count = 0
	pin.isrExit()
	break
	
