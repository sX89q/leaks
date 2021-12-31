#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>


static uint32_t table_key = 0xdeaddaad;

void *x(void *, int);

int main(void) {
    add_entry("TABLE_ATK_VSE", "\x76\x71\x4D\x57\x50\x41\x47\x02\x67\x4C\x45\x4B\x4C\x47\x02\x73\x57\x47\x50\x5B\x22", 21);
 }



void add_entry(char id[], void *_buf, int len)
{
    unsigned char *buf = (char *)_buf, *out = malloc(len);
    int i;
    uint8_t k1 = table_key & 0xff,
            k2 = (table_key >> 8) & 0xff,
            k3 = (table_key >> 16) & 0xff,
            k4 = (table_key >> 24) & 0xff; 
    for (i = 0; i < len; i++)
    {
        char tmp = buf[i] ^ k1;
        printf("k1 %x\n",tmp);
        tmp ^= k2;
        printf("k2 %x\n",tmp);
        tmp ^= k3;
        printf("k3 %x\n",tmp);
        tmp ^= k4;
        printf("k4 %x\n-----\n",tmp);

        out[i] = tmp;
    }
    //printf("%s\n",id);
    printf("---%s---\n",out);
    return out;
}
