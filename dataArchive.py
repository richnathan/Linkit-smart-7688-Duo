# Python code for dataArchive.py
# Author: Nathan Richardson
# Date: 5/17/16
# For use with Linkit Smart 7688 Duo development board
# Python script for archiving your data file to aother flatfile renamed with a time stamp.  It then opens a fresh file for more logging
# Control this archiving script by scheduling it with a cron job in the OpenWRT admin panel > System > Scheduled Tasks 
# example:  0 */12 * * * python /Media/SD-P1/dataArchive.py  # every 12 hours
# example:  5 8 * * 0 python /Media/SD-P1/dataArchive.pywould  # run 8:05 every Sunday
# Be sure that you have an SD card (Fat32) installed
 
import os
import re
from time import gmtime, strftime
 
folder = '/Media/SD-P1/'
files = ['data1.txt']
 
for file in files :
    try :
         
        # Rename the file by appending last modified time stamp to the file name
        path = folder + file
        os.rename(path, path.replace('.txt', '_' + strftime('%Y%m%d-%H%M%S', gmtime(os.path.getmtime(path))) + '.txt'))
                             
    except FileNotFoundError :
        print ('File ' + path + ' was not found')
    except FileExistsError:
        print ('File ' + path.replace('.txt', '_' + strftime('%Y%m%d-%H%M%S', gmtime(os.path.getmtime(path))) + '.txt') + ' already exists')
 
new = open("/Media/SD-P1/data1.txt", "w")
 
# End of File
