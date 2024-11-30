#ifndef STRING_H
#define STRING_H

#include <stdlib.h>

char* readline(const char* const format);
char* str_tok(char* src, const char* const pattern);
size_t str_len(const char* const src);
char* str_cpy(char* src, const char* const dest);

#endif /* STRING_H */