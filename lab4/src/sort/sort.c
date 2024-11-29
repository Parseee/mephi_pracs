#include "sort.h"

#include <ctype.h>
#include <stddef.h>
#include <stdint.h>

bool is_sorted(void* array, size_t array_size, size_t size,
    bool (*compare)(const void*, const void*))
{
    lassert(array, "");

    for (int i = 0; i < array_size - 1; ++i) {
        if (!compare(array + i * size, array + (i + 1) * size)) {
            return false;
        }
    }

    return true;
}

static void vectorized_memory_swap(void** ptr1, void** ptr2, void* ptr_tmp, size_t size)
{
    lassert(ptr1, "");
    lassert(ptr2, "");
    lassert(*ptr1, "");
    lassert(*ptr2, "");
    lassert(ptr_tmp, "");

    memcpy(ptr_tmp, *ptr1, size);
    memcpy(*ptr1, *ptr2, size);
    memcpy(*ptr2, ptr_tmp, size);

    *ptr1 = (char*)(*ptr1) + size;
    *ptr2 = (char*)(*ptr2) + size;
}

void swap(void* ptr1, void* ptr2, size_t size)
{
    lassert(ptr1, "");
    lassert(ptr2, "");

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

sorted_state Text_sort(Text* text)
{
    lassert(text, "");

    for (size_t i = 0; i < text->text_size; ++i) {
        for (size_t j = i + 1; j < text->text_size; ++j) {
            // fprintf(stderr, "%s, %s\n", text->text[i], text->text[j]);
            if (!Text_strcmp((char*)text->text + i * sizeof(char*), (void*)text->text + j * sizeof(char*))) {
                swap((char*)text->text + i * sizeof(char*), (void*)text->text + j * sizeof(char*), sizeof(*text->text));
            }
        }
    }

    return OK;
}