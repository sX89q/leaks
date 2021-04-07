#!/bin/bash
YEL='[1;33m'
WHI='[1;37m'
grep -v "DVDSrv" gasite.txt > 1
grep -v "pos01" 1 > 2
grep -v "raspberry" 2 > 3
grep -v "Error" 3 > 4
grep -v "Please login" 4 > 5
grep -v "sh-4.3" 5 > 6
grep -v "06:20:54" 6 > 7
grep -v "ERROR" 7 > 8
grep -v "^G" 8 > 9
grep -v "3.2.68-1+deb7u1" 9 > 10
grep -v "PREEMPT" 10 > 11
grep -v "This service allows sftp connections only." 11 > 12
grep -v "This account is currently not available." 12 > 13
grep -v "listensocks" 13 > 14
grep -v "wrong" 14 > 15
grep -v "/dev/pts" 15 > 16
grep -v "reconnect" 16 > 17
grep -v "Welcome to" 17 > 18
grep -v "Unknown command" 18 > 19
grep -v "JUNOS" 19 > 20
grep -v "ppc unknown" 20 > 21
grep -v "mips" 21 > 22
grep -v "uname -a" 22 > 23
grep -v "4.15.0-38-generic #41-Ubuntu SMP x86_64 GNU/Linux" 23 > 24
grep -v "4.9.0-8-amd64 #1 SMP Debian 4.9.110-3+deb9u6 (2018-10-08) x86_64 GNU/Linux" 24 > 25
grep -v "UTC 2017 x86_64 x86_64 x86_64 GNU/Linux" 25 > 26
grep -v "cat: /proc/version: No such file or directory" 26 > 27
grep -v "pos01" 27 > 28
grep -v "raspberry" 28 > 29
grep -v "Error" 29 > 30
grep -v "Please login" 30 > 31
grep -v "sh-4.3" 31 > 32
grep -v "06:20:54" 32 > 33
grep -v "ERROR" 33 > 34
grep -v "22:33:07" 34 > 35
grep -v "3.2.68-1+deb7u1" 35 > 36
grep -v "PREEMPT" 36 > 37
grep -v "This service allows sftp connections only." 37 > 38
grep -v "This account is currently not available." 38 > 39
grep -v "uname: Invalid argument" 39 > 40
sed '/^\s*$/d' 40 > bune
sleep 2
rm -rf 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40
sleep 1
clear
printf "${YEL}++ Gata fraiere, folosim \"nano bune\" pentru a vedea vps-urile prinse ++ ${WHI}\n"
