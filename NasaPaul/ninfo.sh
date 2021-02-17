#!/bin/bash


BLK='[1;30m'
RED='[1;31m'
GRN='[1;32m'
YEL='[1;33m'
DBLU='[1;34m'
MAG='[1;35m'
CYN='[1;36m'
WHI='[1;37m'
DRED='[0;31m'
DGRN='[0;32m'
DYEL='[0;33m'
DBLU='[0;34m'
DMAG='[0;35m'
DCYN='[0;36m'
DWHI='[0;37m'
RES='[0m'


 
CPU=$(grep -m 1 "model name" /proc/cpuinfo | cut -d: -f2 | sed -e 's/^ *//' | sed -e 's/$//')
CPUS=$(grep -c ^processor /proc/cpuinfo)
STEP=$(grep -m 1 "stepping" /proc/cpuinfo | cut -d: -f2 | sed -e 's/^ *//' | sed -e 's/$//')
BOGO=$(grep -m 1 "bogomips" /proc/cpuinfo | cut -d: -f2 | sed -e 's/^ *//' | sed -e 's/$//')
OS=$(lsb_release -si)
ram=$(free -m | grep -oP '\d+' | head -n 1)
VER=$(uname -a )
uptime=$(</proc/uptime)
uptime=${uptime%%.*} bold=$(tput bold)
zile=$(( uptime/60/60/24 )) 
secunde=$(( uptime%60 ))
minute=$(( uptime/60%60 ))
ore=$(( uptime/60/60%24 ))
vid=$(lspci | grep VGA | cut -f5- -d ' ') 
DISK=$(df -h --total | grep total |awk '{ printf "" $2 "B\n\n" }')




sleep 1
echo "${DRED}--------------------------------------------------------------------------${WHI}" 
echo "${WHI}|                    ${WHI} NasaPaul.com Official Website                       |${WHI}"          
echo "${DRED}--------------------------------------------------------------------------${WHI}" 
sleep 1
echo "${DRED} ->${WHI} Loading Resurces... ${DGRN} 34% ${WHI}"
echo "${DRED} ->${WHI} Loading Resurces... ${DGRN} 68% ${WHI}"
echo "${DRED} ->${WHI} Loading Resurces... ${DGRN} 100%${WHI}"
echo ""
echo ""
echo "${DRED} ->${WHI} Resource Loaded... ${DGRN} 100%${WHI}"
echo ""
sleep 2
echo "${WHI}# ${DRED}CPU           ${DRED} -> ${WHI}${CPU}${RES}"             #${WHI}"
echo "${WHI}# ${DRED}CPU CORE      ${DRED} -> ${WHI}${CPUS}${RES}"            #${WHI}"
echo "${WHI}# ${DRED}Stepping      ${DRED} -> ${WHI}${STEP}${RES}"            #${WHI}"
echo "${WHI}# ${DRED}Bogomips      ${DRED} -> ${WHI}${BOGO}${RES}"            #${WHI}"
echo "${WHI}# ${DRED}Ram           ${DRED} -> ${WHI}${ram}MB  [1024MB = 1GB]" #${WHI}"
echo "${WHI}# ${DRED}GPU           ${DRED} -> ${WHI}${vid}"
echo "${WHI}# ${DRED}DISK SPACE    ${DRED} -> ${WHI}${DISK}"
echo "${WHI}# ${DRED}Versiune      ${DRED} -> ${WHI}${VER}"
echo "${WHI}# ${DRED}Uptime        ${DRED} -> ${WHI}${zile} Zile"
sleep 2
if ((${EUID:-0} || "$(id -u)")); then

  echo "${WHI}#${DRED} Drept de root  -> ${WHI}Nu ai""${WHI}                                 ${WHI}"
sleep 3
else
echo "${WHI}# ${DRED}Drept de root  -> ${WHI}Ai                                            ${WHI}"
fi
sleep 3
echo "${DRED}--------------------------------------------------------------------------${WHI}" 
echo "${WHI}|                   ${WHI}SPEED TESTUL INCEPE IN 3 SECUNDE                     |${WHI}"
echo "${DRED}--------------------------------------------------------------------------${WHI}" 
sleep 1
echo "${DRED} ->${WHI}1${WHI}"
sleep 1
echo "${DRED} ->${WHI}2${WHI}"
sleep 1
echo "${DRED} ->${WHI}3${WHI}"
sleep 1
wget nasapaul.com/v.py
perl v.py 