#include "sort.h"

#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

bool is_sorted(void* array, size_t array_size, size_t size,
    bool (*compare)(const void*, const void*))
{
    assert(array && "");

    for (int i = 0; i < array_size - 1; ++i) {
        if (!compare(array + i * size, array + (i + 1) * size)) {
            return false;
        }
    }

    return true;
}

static void vectorized_memory_swap(void** ptr1, void** ptr2, void* ptr_tmp, size_t size)
{
    assert(ptr1 && "");
    assert(ptr2 && "");
    assert(*ptr1 && "");
    assert(*ptr2 && "");
    assert(ptr_tmp && "");

    memcpy(ptr_tmp, *ptr1, size);
    memcpy(*ptr1, *ptr2, size);
    memcpy(*ptr2, ptr_tmp, size);

    *ptr1 = (char*)(*ptr1) + size;
    *ptr2 = (char*)(*ptr2) + size;
}

void swap(void* ptr1, void* ptr2, size_t size)
{
    assert(ptr1 && "");
    assert(ptr2 && "");

    while (size >= sizeof(uint64_t)) {
        uint64_t ptr_tmp;
        vectorized_memory_swap(&ptr1, &ptr2, (void*)&ptr_tmp, sizeof(uint64_t));
        size -= sizeof(uint64_t);
    }
    if (size & sizeof(uint64_t)) {
        uint32_t ptr_tmp = 0;
        vectorized_memory_swap(&ptr1, &ptr2, (void*)&ptr_tmp, 4);
        size -= sizeof(uint32_t);
    }
    if (size & sizeof(uint16_t)) {
        uint16_t ptr_tmp = 0;
        vectorized_memory_swap(&ptr1, &ptr2, (void*)&ptr_tmp, 2);
        size -= sizeof(uint16_t);
    }
    if (size & sizeof(uint8_t)) {
        uint8_t ptr_tmp = 0;
        vectorized_memory_swap(&ptr1, &ptr2, (void*)&ptr_tmp, 1);
        size -= sizeof(uint8_t);
    }

    return;
}

void bubble_sort(void* array, size_t array_size, size_t size, bool (*compare)(const void* l, const void* r))
{
    for (size_t i = 0; i < array_size; ++i) {
        for (size_t j = i + 1; j < array_size; ++j) {
            // fprintf(stderr, "%s, %s\n", text->text[i], text->text[j]);
            if (!compare(array + (i * size), array + (j * size))) {
                swap(array + (i * size), array + (j * size), size);
            }
        }
    }

    return;
}

static size_t partition(void *array, size_t l, size_t r, size_t array_size, size_t size, bool (*compare)(const void *, const void *)) {
    void* v = array + ((l + r) / 2) * size;
    size_t i = l;
    size_t j = r;

    while (i < j) {
        while (compare(array + i * size, v) && i + 1 < array_size) {
            ++i;
        }
        while (compare(v, array + j * size) && j < array_size) {
            --j;
        }
        if (i >= j) break;
        swap(array + (i++) * size, array + (j--) * size, size);
    }
    return j;
}

static void _q_sort(void *array, size_t l, size_t r , size_t array_size, size_t size, bool (*compare)(const void *, const void *)) {
    if (array == NULL) {
        return;
    }

    if (l < r) {
        size_t pivot = partition(array, l, r, array_size, size, compare);
        _q_sort(array, l, pivot, array_size, size, compare);
        _q_sort(array, pivot, r, array_size, size, compare);
    }
}

void q_sort(void *array, size_t array_size, size_t size, bool (*compare)(const void *, const void *)) {
    _q_sort(array, 0, array_size - 1, array_size, size, compare);
}
