#pragma once
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "../db/db.h"

typedef enum { OK, NULL_PTR_ERROR, LOGIC_ERROR } sorted_state;


bool is_sorted(void* array, size_t array_size, size_t size, bool (*compare)(const void* l, const void* r));
sorted_state Text_sort(DB* text);
void swap(void* ptr1, void* ptr2, size_t size);