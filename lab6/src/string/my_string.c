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

static int cmp(const void* l, const void* r)
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
        if (array[i].eo == NULL) {
            array[i].eo = array[i].so;
        }
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