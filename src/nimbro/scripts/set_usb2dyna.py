#!/usr/bin/python
#Author: Hafez Farazi <farazi@ais.uni-bonn.de>
import os, sys
import subprocess
import time
from termcolor import colored

def getCol(col, line):
    p1 = line.find(col)
    if p1<0 : return ""
    p2 = p1 + len(col) + 1
    p3 = line.find('"',p2+1)
    return line[p2+1:p3]
    
def updateUsb2dyna():
    print " -> Update device rules: usb2dyna..."
    print "it is assumed usb2dynamixel is connected to /dev/ttyUSB0"
    result = subprocess.check_output("sudo udevadm info -a -n /dev/ttyUSB0 | grep ATTRS{serial}", shell=True)
    data=[]
    toSavetty=""
    for line in result.split(os.linesep):
        serial = getCol("ATTRS{serial}=", line)
        if len(serial)==8 and "F" in serial:
            toSavetty='SUBSYSTEMS=="usb", ATTRS{idProduct}=="6014", ATTRS{idVendor}=="0403", SYMLINK+="usb2dyna", MODE="0666"'
            data.append(serial)
            print "usb2dynamixel Serial = "+serial
        
    if len(data) > 1 :
        print colored("It seems that you have more than one ttyUSB! Please check it.","red")
        sys.exit()
    if len(data) == 0 :
        print colored("It seems that you have no ttyUSB! Please check it.","red")
        sys.exit()
    os.system("sudo rm /etc/udev/rules.d/90-* > /dev/null 2>&1")
    with open("/etc/udev/rules.d/90-usb2dyna.rules","w") as fw:
        fw.write(toSavetty)
    with open("/etc/udev/rules.d/90-usb2dyna.rules","r") as fr:
        rule=fr.read()
        if(rule ==toSavetty):
            print colored("  -> Done.","green")
        else:
            print colored("Can not update device rules: usb2dyna.","red")
            sys.exit()   

if __name__ == '__main__':
    if not os.geteuid()==0:
            print colored("You must be root to run this application.","red")
            os._exit(-1)
        
    print "----------Start------------"
    
    updateUsb2dyna()
 
    print "----------Finish-----------"
    exit()
