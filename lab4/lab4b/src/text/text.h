#ifndef TEXT_H
#define TEXT_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "../mystring/mystring.h"

typedef enum { TEXT_OK = 0, TEXT_INPUT_ERROR, TEXT_ALLOC_ERROR, TEXT_NOT_SET_ERROR, TEXT_INTERNAL_ERROR } TEXT_ERROR;

typedef struct Text {
    char** text;
    size_t text_size;
    size_t text_capacity;
} Text;

TEXT_ERROR Text_create(Text* text);
TEXT_ERROR Text_get_text(Text* text, FILE* file);
TEXT_ERROR Text_lengthify(Text* text);
TEXT_ERROR Text_destruct(Text* text);

#endif /* TEXT_H */
