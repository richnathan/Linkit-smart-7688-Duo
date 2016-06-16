#!/usr/bin/python 

# Python script for opening a flatfile and sending the value through the yun bridge, call with FlowMetertInterrupt.ino
# made for use with Linkit Smart 7688 Duo
# Author  Nathan Richardson
# Date    5/25/16
# Open source, use as you wish

import sys
sys.path.insert(0, '/usr/lib/python2.7/bridge/') 

from bridgeclient import BridgeClient as bridgeclient

value = bridgeclient()                              

target = open("/IoT/data/dat.txt", "r")
c = target.readline()
value.put("90",(c))	
target.close()

sys.exit()
