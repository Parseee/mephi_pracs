#pragma once

#include <stdio.h>

typedef struct String {
    char let;
    struct String* next;
} String;

typedef enum { STRING_OK,
    STRING_INTERNAL_ERROR } STRING_ERROR;

String* String_create_string();
void String_kill_string(String* str);
void String_print_string(String* str);