#ifndef STRING_H
#define STRING_H

#include <stdio.h>
#include <stdlib.h>

char* freadline(const char* const format, FILE* f);
ssize_t get_line(char** string, size_t* len, FILE* stream);
char* str_tok(char* src, const char* const pattern);
size_t str_len(const char* const src);
char* str_cpy(char* src, const char* dest);

#endif /* STRING_H */