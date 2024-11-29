#pragma once
#ifndef TEXT_H
#define TEXT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../logger/logger.h"

#define DEBUG

typedef enum input_state { INPUT_OK, INPUT_FAILURE, READ_FAILURE } input_state;

#define report_error(error)                                             \
    do {                                                                \
        fprintf(stderr, #error " in file: %s, on line: %d\n", __FILE__, \
                __LINE__);                                              \
        return error;                                                   \
    } while (0)

typedef struct {
    char* data;
    char** text;
    size_t data_size;
    size_t text_size;
} Text;

input_state Text_create(Text* text, const char* const filename);
input_state Text_kill(Text* text);
bool Text_strcmp(const void* l, const void* r);

#endif /* TEXT_H */