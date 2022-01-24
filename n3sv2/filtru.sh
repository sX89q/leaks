#!/bin/bash
grep -v "DVDSrv" sparte.txt > 1
grep -v "pos01" 1 > 2
grep -v "raspberry" 2 > 3
grep -v "Error" 3 > 4
grep -v "Please login" 4 > 5
grep -v "sh-4.3" 5 > 6
grep -v "06:20:54" 6 > 7
grep -v "ERROR" 7 > 8
grep -v "22:33:07" 8 > 9
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
sed '/^\s*$/d' 26 > bune.n3s
clear
good=`grep -c . bune.n3s`
ubuntu=`grep -c ubuntu bune.n3s`
centos=`grep -c centos bune.n3s`
debian=`grep -c debian bune.n3s`

echo "[0;32m<[1;37m![0;32m>[1;37m FILTRU.SH STARTED [0;32m<[1;37m![0;32m>"
echo "[0;32m<[1;37m![0;32m>[1;37m Am gasit >[0;32m $good [1;37mservere vulnerabile! [0;32m<[1;37m![0;32m>"
echo "[0;32m<[1;37m![0;32m>[1;37m Am gasit >[0;32m $ubuntu [1;37m servere ubuntu! [0;32m<[1;37m![0;32m>"
echo "[0;32m<[1;37m![0;32m>[1;37m Am gasit >[0;32m $centos [1;37m servere centos! [0;32m<[1;37m![0;32m>"
echo "[0;32m<[1;37m![0;32m>[1;37m Am gasit >[0;32m $debian [1;37m servere debian! [0;32m<[1;37m![0;32m>"
echo "[0;32m<[1;37m![0;32m>[1;37m Foloseste >[0;32m cat bune.n3s [1;37m pentru a vedea vps.urile. [0;32m<[1;37m![0;32m>"
sleep 2
rm -rf 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26