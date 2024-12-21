#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum DB_state { DB_OK,
    DB_WRITE_ERROR,
    DB_READ_ERROR,
    DB_INTERNAL_ERROR } DB_state;

typedef struct Item {
    char* name;
    char* id;
    time_t time;
} Item;

typedef struct DB {
    Item** data;
    size_t size;
    size_t capacity;
} DB;

DB_state DB_create(DB* db);
DB_state DB_destruct(DB* db);
DB_state DB_append(DB* db, Item* item);

bool Text_strcmp(const void* l, const void* r);