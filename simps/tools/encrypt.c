#define _GNU_SOURCE

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

static uint32_t key = 0xE0A4CBD6;

enum
{
    TRUE = 1,
    FALSE = 0
};

static uint8_t dec_table[67] =
{
    0x18, 0x1b, 0x1a, 0x1d, 0x1c, 0x1f, 0x1e, 0x11, 0x10, 0x13,
    0x12, 0x15, 0x14, 0x17, 0x16, 0x09, 0x08, 0x0b, 0x0a, 0x0d,
    0x0c, 0x0f, 0x0e, 0x01, 0x00, 0x03, 0x38, 0x3b, 0x3a, 0x3d,
    0x3c, 0x3f, 0x3e, 0x31, 0x30, 0x33, 0x32, 0x35, 0x34, 0x37,
    0x36, 0x29, 0x28, 0x2b, 0x2a, 0x2d, 0x2c, 0x2f, 0x2e, 0x21,
    0x20, 0x23, 0x69, 0x68, 0x6b, 0x6a, 0x6d, 0x6c, 0x6f, 0x6e,
    0x61, 0x60, 0x77, 0x76, 0x79, 0x74, 0x63
};

static uint8_t enc_table[67] =
{
    0x14, 0x15, 0x0a, 0x1d, 0x1f, 0x08, 0x0e, 0x00, 0x01, 0x17,
    0x1a, 0x03, 0x0b, 0x09, 0x16, 0x12, 0x1e, 0x10, 0x0c, 0x0d,
    0x18, 0x1b, 0x0f, 0x11, 0x1c, 0x13, 0x2d, 0x3f, 0x28, 0x36,
    0x34, 0x38, 0x3c, 0x3a, 0x31, 0x35, 0x20, 0x37, 0x2c, 0x3d,
    0x2e, 0x2f, 0x33, 0x2b, 0x21, 0x30, 0x3e, 0x32, 0x23, 0x2a,
    0x3b, 0x29, 0x6e, 0x61, 0x60, 0x69, 0x6b, 0x6c, 0x6d, 0x68,
    0x6f, 0x6a, 0x64, 0x19, 0x07, 0x7d, 0x05
};

static void xor(uint8_t *data, int data_len)
{
    int i = 0;
    uint8_t k1 = key & 0xff, k2 = (key >> 8) & 0xff, k3 = (key >> 16) & 0xff, k4 = (key >> 24) & 0xff;

    for(i = 0; i < data_len; i++)
    {
        data[i] ^= k1;
        data[i] ^= k2;
        data[i] ^= k3;
        data[i] ^= k4;
    }

    return;
}

static char *encrypt(char *arg, int arg_len)
{
    int p = 0;
    int x = 0;
    char *alloc;

    alloc = (char *)malloc(arg_len);
    if(!alloc)
        return NULL;

    while(TRUE)
    {
        if(!*arg)
            break;

        for(x = 0; x < sizeof(dec_table); x++)
        {
            if(*arg == dec_table[x])
                alloc[p++] = enc_table[x];
        }

        arg++;
    }

    if(p != arg_len)
    {
        return NULL;
    }

    return alloc;
}

static char *decrypt(char *arg, int arg_len)
{
    int p = 0;
    int x = 0;
    char *alloc;

    alloc = (char *)malloc(arg_len);
    if(!alloc)
        return NULL;

    while(TRUE)
    {
        if(!*arg)
            break;

        for(x = 0; x < sizeof(enc_table); x++)
        {
            if(*arg == enc_table[x])
                alloc[p++] = dec_table[x];
        }

        arg++;
    }

    if(p != arg_len)
    {
        return NULL;
    }

    return alloc;
}

void main(int argc, char **args)
{
    char *r;
    int len = 0;

    if(argc != 3)
    {
        printf("Invalid arguments!\n");
        return;
    }

    xor(dec_table, sizeof(dec_table));
    xor(enc_table, sizeof(enc_table));

    if(!strcmp(args[1], "encrypt"))
    {
        len = strlen(args[2]);
        r = encrypt(args[2], len);
        printf("Encrypted \"%s\"\n", r);
        printf("Length %d\n", len);
        free(r);
        return;
    }

    if(!strcmp(args[1], "decrypt"))
    {
        len = strlen(args[2]);
        r = decrypt(args[2], len);
        printf("Decrypted \"%s\"\n", r);
        printf("Length %d\n", len);
        free(r);
        return;
    }

    return;
}
