#pragma once

#include <stdint.h>
#include "includes.h"

struct table_value {
    char *val;
    uint16_t val_len;
#ifdef DEBUG
    BOOL locked;
#endif
};

#define TABLE_MISC_DOG			1
#define TABLE_MISC_DOG1			2
#define TABLE_MISC_DOG2			3
#define TABLE_MISC_DOG3			4
#define TABLE_MISC_DOG4			5
#define TABLE_MISC_DOG5			6
#define TABLE_MISC_DOG6			7

#define TABLE_MAX_KEYS			8

void table_init(void);
void table_unlock_val(uint8_t);
void table_lock_val(uint8_t); 
char *table_retrieve_val(int, int *);

static void add_entry(uint8_t, char *, int);
static void toggle_obf(uint8_t);
