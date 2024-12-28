#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../string/my_string.h"

typedef enum { TEXT_OK = 0,
    TEXT_INPUT_ERROR,
    TEXT_ALLOC_ERROR,
    TEXT_NOT_SET_ERROR } TEXT_ERROR;


typedef struct Text {
    String** text;
    size_t text_size;
    size_t text_capacity;
} Text;

TEXT_ERROR Text_create(Text* text);
TEXT_ERROR Text_get_text(Text* text);
TEXT_ERROR Text_lengthify(Text* text);
TEXT_ERROR Text_destruct(Text* text);