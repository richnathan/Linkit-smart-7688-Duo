#!/usr/bin/python 

import sys
sys.path.insert(0, '/usr/lib/python2.7/bridge/') 

from bridgeclient import BridgeClient as bridgeclient

value = bridgeclient()                              

target = open("/IoT/data/dat.txt", "r")
c = target.readline()
value.put("90",(c))	
target.close()

sys.exit()
