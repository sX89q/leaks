#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

uint32_t xor_key = 0xDEDEEAED;

void *xor(void *str, int str_len)
{
    char *buf = (char *)str;
    char *r = malloc(str_len);
    int i = 0;
    uint8_t k1 = xor_key & 0xff, k2 = (xor_key >> 8) & 0xff, k3 = (xor_key >> 16) & 0xff, k4 = (xor_key >> 24) & 0xff;
    char t;
    for(i = 0; i < str_len; i++)
    {
        t = buf[i] ^ k1;
        t ^= k2;
        t ^= k3;
        t ^= k4;
        r[i] = t;
    }
    return r;
}

int main(int argc, char **args)
{
    if(argc < 1)
    {
        fprintf(stderr, "Usage: %s [string]\n", args[0]);
        return 1;
    }
    void *buf = args[1];
    int len = strlen(args[1]) + 1;
    buf = xor(buf, len);
    int i = 0;
    printf("XOR'ing %d bytes\n", len);
    for(i = 0; i < len; i++)
    {
        printf("\\x%02X", ((unsigned char *)buf)[i]);
    }
    printf("\n");
    return 0;
}
