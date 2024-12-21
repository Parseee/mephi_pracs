#pragma once
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "../db/db.h"

typedef enum { OK,
    NULL_PTR_ERROR,
    LOGIC_ERROR } sorted_state;

bool is_sorted(void* array, size_t array_size, size_t size, bool (*compare)(const void* l, const void* r));
void q_sort(void* array, size_t array_size, size_t size, bool (*compare)(const void* l, const void* r));
void bubble_sort(void* array, size_t array_size, size_t size, bool (*compare)(const void* l, const void* r));
void shaker_sort(void* array, size_t array_size, size_t size, bool (*compare)(const void* l, const void* r));
void shell_sort(void* array, size_t array_size, size_t size, bool (*compare)(const void* l, const void* r));
void swap(void* ptr1, void* ptr2, size_t size);