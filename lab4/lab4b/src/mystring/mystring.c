#include <assert.h>
#include <memory.h>
#include <stdbool.h>
#include <stdio.h>

#include "mystring.h"

static bool consists(char sym, const char* const pat)
{
    assert(pat && "pattern is not init");

    char* ptr = (char*)pat;

    while (*ptr != '\0') {
        if (*ptr == sym) {
            return true;
        }
        ptr += 1;
    }

    return false;
}

static size_t spans(char* src, const char* const pat)
{
    size_t cnt = 0;
    for (char* a = src; *a != '\n'; ++a) {
        if (*a == '\0') {
            return cnt;
        } else if (consists(*a, pat)) {
            cnt += 1;
        } else {
            break;
        }
    }
    return cnt;
}

static char* occurs(char* src, const char* const pat)
{
    char* ptr = src;

    while (*ptr != '\0') {
        if (consists(*ptr, pat)) {
            break;
        }
        ptr += 1;
    }

    return ptr == src ? NULL : ptr;
}

char* readline(const char* const format)
{
    assert(format && "format is empty");
    char* line = NULL;
    size_t len = 0;
    ssize_t read = 0;

    if ((read = getline(&line, &len, stdin)) == -1) {
        free(line);
        return NULL;
    }

    // size_t line_size = 256;


    // if ((read = scanf("%s", line)) == 0) {
    //     free(line);
    //     return NULL;
    // }

    line[read - 1] = '\0';

    return line;
}

char* str_tok(char* src, const char* const pattern)
{
    assert(pattern && "dest is not init");

    static char* end_ptr;

    if (src == NULL) {
        src = end_ptr;
    }

    src += spans(src, pattern);
    if (*src == '\0') {
        end_ptr = src;
        return NULL;
    }

    char* token = (char*)src;
    src = occurs(src, pattern);
    if (src == NULL) {
        src = occurs(src, "\0"); // possibly makes it slower here
    } else {
        *src = '\0';
        end_ptr = src + 1;
    }
    return token;
}

size_t str_len(const char* const src)
{
    assert(src && "source line is not init");

    size_t len = 0;
    while (src[len] != '\n' && src[len] != '\0') {
        len += 1;
    }

    return len;
}

char* str_cpy(char* dest, const char* const src)
{
    assert(src && "src is not init");
    assert(dest && "dest is not init");

    return memcpy(dest, src, strlen(src) * sizeof(src));
}