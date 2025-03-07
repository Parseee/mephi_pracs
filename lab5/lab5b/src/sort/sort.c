#include "sort.h"

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

void bubble_sort(void *const array, const ssize_t array_size, const ssize_t size, bool (*compare)(const void *, const void *))
{
    for (ssize_t i = 0; i < array_size; ++i) {
        for (ssize_t j = i + 1; j < array_size; ++j) {
            // fprintf(stderr, "%s, %s\n", *((char**)(array) + i), *((char**)(array) + j));
            if (!compare(array + (i * size), array + (j * size))) {
                swap(array + (i * size), array + (j * size), size);
            }
        }
    }

    return;
}

static size_t partition(void* array, ssize_t l, ssize_t r, size_t array_size, size_t size, bool (*compare)(const void*, const void*))
{
    void* pivot = array + l * size;

    ssize_t i = l + 1;
    ssize_t j = r;
    while (true) {
        while (i <= j && compare(array + i * size, pivot)) {
            ++i;
        }
        while (i <= j && compare(pivot, array + j * size)) {
            --j;
        }
        if (i > j) {
            break;
        } else {
            swap(array + i * size, array + j * size, size);
        }
    }

    swap(pivot, array + j * size, size);
    return j;
}

static void _q_sort(void* array, ssize_t l, ssize_t r, size_t array_size, size_t size, bool (*compare)(const void*, const void*))
{
    if (array == NULL || array_size == 1) {
        return;
    }

    if (l < r) {
        ssize_t pivot = partition(array, l, r, array_size, size, compare);
        _q_sort(array, l, pivot - 1, array_size, size, compare);
        _q_sort(array, pivot + 1, r, array_size, size, compare);
    }
}

void q_sort(void* const array, const ssize_t array_size, const ssize_t size, bool (*compare)(const void* l, const void* r))
{
    _q_sort(array, 0, array_size - 1, array_size, size, compare);
}

void shaker_sort(void* const array, const ssize_t array_size, const ssize_t size, bool (*compare)(const void* l, const void* r))
{
    if (!array || array_size == 1) {
        return;
    }

    bool backwards = false;
    for (ssize_t i = 0; i < array_size; ++i) {
        if (backwards) {
            for (ssize_t j = array_size - 1; j > i; --j) {
                if (!compare(array + i * size, array + j * size)) {
                    swap(array + i * size, array + j * size, size);
                }
            }
        } else {
            for (ssize_t j = i + 1; j < array_size; ++j) {
                if (!compare(array + i * size, array + j * size)) {
                    swap(array + i * size, array + j * size, size);
                }
            }
        }
    }
}

void shell_sort(void *const array, const ssize_t array_size, const ssize_t size, bool (*compare)(const void *, const void *))
{
    if (!array || array_size == 1) {
        return;
    }

    for (ssize_t gap = array_size / 2; gap > 0; gap >>= 1) {
        for (ssize_t i = gap; i < array_size; ++i) {
            void* tmp = array + i * size;

            int j;            
            for (j = i; j >= gap && compare(array + (j - gap) * size, tmp); j -= gap) {
                void* t = ((char**)array + (j - gap) * size);
                ((char**)array)[j * size] = t;
            }

            ((char**)array)[j * size] = tmp;
        }
    }
}