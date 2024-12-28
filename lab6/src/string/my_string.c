#include "my_string.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_WORDS_COUNT 1000

String* String_create_string()
{
    String* str = calloc(1, sizeof(*str));

    str->let = getchar();
    str->next = NULL;

    if ((str->let != '\xff') && (strchr("\n\0", str->let) == NULL)) {
        str->next = String_create_string();
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

// void String_sort(String *str) {
//     typedef struct Node {
//         String *so;
//         String *eo;
//     } Node;

//     Node* array = calloc(MAX_WORDS_COUNT, sizeof(*array));

//     String* sptr = str;
//     String* eptr = str;
//     for (int i = 0; i < MAX_WORDS_COUNT; ++i)  {
//         while((strchr(" \n\0", eptr->let)) == NULL) {
//             ++eptr;
//         }
//         array[i] = (Node){sptr, eptr};
//         if (eptr->let == ' ') {
//             sptr = eptr = eptr + 1;
//         } else {
//             break;
//         }
//     }


// }