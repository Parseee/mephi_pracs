#include "my_string.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    String* so;
    String* eo;
    size_t len;
} Node;

#define MAX_WORDS_COUNT 1000

String* String_create_string(String* prev)
{
    String* str = calloc(1, sizeof(*str));

    str->let = getchar();

    if (strchr("\n\0", str->let)) {
        str->prev = prev;
        return str;
    }

    if ((str->let != '\xff')) {
        str->next = String_create_string(str);
        str->prev = prev;
    }

    return str;
}

void String_kill_string(String* str)
{
    if (str == NULL)
        return;
    String_kill_string(str->next);
    free(str);
}

void String_print_string(String* str)
{
    printf("%c", str->let);
    if (str->next) {
        String_print_string(str->next);
    }
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

static void swp(void* ptr1, void* ptr2, size_t size)
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

static void swap(Node* l, Node* r)
{
    String* tmp = NULL;
    tmp = l->so->prev->next; // = l->so
    l->so->prev->next = r->so->prev->next; // = r->so
    r->so->prev->next = tmp;

    tmp = l->so->prev;
    l->so->prev = r->so->prev;
    r->so->prev = tmp;

    tmp = l->eo->next->prev;
    l->eo->next->prev = r->eo->next->prev;
    r->eo->next->prev = tmp;

    tmp = l->eo->next;
    l->eo->next = r->eo->next;
    r->eo->next = tmp;
}

int cmp(const void* l, const void* r)
{
    return (*(Node*)l).len > (*(Node*)r).len;
}

static STRING_ERROR string_tokenize(String** str, Node* array, size_t* len)
{
    String* sptr = *str;
    String* eptr = *str;
    size_t str_len = 0;
    for (int i = 0; i < MAX_WORDS_COUNT; ++i) {
        *len += 1;
        while ((strchr(" \n", eptr->let)) == NULL && eptr->next) {
            eptr = eptr->next;
            ++str_len;
        }
        // *(array[i]) = (Node) { .so = sptr, .eo = eptr->prev, .len = str_len };
        array[i].so = sptr;
        array[i].eo = eptr->prev;
        array[i].len = str_len;
        str_len = 0;
        while (eptr->let == ' ') { // never goes out of boundaries bc of '\n'
            sptr = eptr = eptr->next;
        }
        if (eptr->let == '\n') {
            break;
        }
    }
    return STRING_OK;
}

static void remove_spaces(String* str)
{
    while ((str)->let != '\n') {
        if ((str)->let == ' ') {
            String* spc = str;
            str = str->next;
            free(spc);
        } else {
            str = (str)->next; // does not crash bc '\n'
        }
    }
    free(str);
}

void String_sort(String** str)
{
    Node* array = calloc(MAX_WORDS_COUNT, sizeof(*array));
    size_t len = 0;

    string_tokenize(str, array, &len);

    qsort(array, len, sizeof(*array), cmp);

    // memleakkkkk
    remove_spaces(*str);
    // memleakkkk

    for (int i = 0; i < len; ++i) {
        fprintf(stderr, "%ld\n", array[i].len);
    }

    *str = array[0].so;
    for (int i = 0; i < len - 1; ++i) {
        String* tmp = calloc(1, sizeof(*tmp));
        tmp->let = ' ';
        array[i].eo->next = tmp;
        tmp->prev = array[i].eo;
        tmp->next = array[i + 1].so;
    }
    String* tmp = calloc(1, sizeof(*tmp));
    tmp->let = '\n';
    array[len - 1].eo->next = tmp;
    tmp->prev = array[len - 1].eo;

    // for (int i = 0; i < len; ++i) {
    //     String* w = array[i].so;
    //     while (true) {
    //         fprintf(stderr, "%c", w->let);
    //         if (w == array[i].eo) {
    //             break;
    //         }
    //         if (w->next) {
    //             w = w->next;
    //         }
    //     }
    //     fprintf(stderr, "\n");
    // }

    // *str = array[0].so;

    free(array);
}