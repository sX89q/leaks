#pragma once

#include "includes.h"

#define KILLER_MIN_PID              400
#define KILLER_RESTART_SCAN_TIME    120

void killer_init(void);
void killer_kill(void);
BOOL killer_kill_by_port(port_t);

static BOOL cmdline_scan_match(char *);
static BOOL mem_exists(char *, int, char *, int);
