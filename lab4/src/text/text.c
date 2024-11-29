#include "text.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>

static input_state Text_set_size(Text* const text, FILE** const fd)
{
    lassert(text, "Text is NULL");
    lassert(*fd, "input file ptr is NULL");

    if (fseek(*fd, 0, SEEK_END)) {
        perror("can't reach end of file");
        report_error(INPUT_FAILURE);
    }

    long data_size = 0;
    if ((data_size = ftell(*fd)) < 0) {
        perror("error while telling the location");
        report_error(INPUT_FAILURE);
    }
    text->data_size = (size_t)data_size + 1;
    fprintf(stderr, "%ld\n", text->data_size);

    if (fseek(*fd, 0, SEEK_SET)) {
        perror("can't reach beginning of file");
        report_error(INPUT_FAILURE);
    }

    return INPUT_OK;
}

static input_state Text_set_text_data(Text* text, FILE** filename)
{
    lassert(text, "ptr");
    lassert(filename, "ptr");
    lassert(!(text->data), "prev defined ptr");
    lassert(!(text->text), "prev defined ptr");

    text->data = malloc(text->data_size * sizeof(*text->data));
    lassert(text->data, "");

    if (fread(text->data, 1, text->data_size - 1, *filename) != (text->data_size - 1)) {
        report_error(INPUT_FAILURE);
    }

    text->data[text->data_size - 1] = '\0';

    return INPUT_OK;
}

static input_state Text_split_data(Text* text)
{
    lassert(text, "ptr");
    lassert(text->data, "ptr");

    for (size_t i = 0; i < text->data_size; ++i) {
        if (text->data[i] == '\n') {
            text->data[i] = '\0';
        }
        if (text->data[i] == '\0') {
            ++text->text_size;
        }
    }

    return INPUT_OK;
}

static input_state Text_set_text(Text* text)
{
    lassert(text, "ptr");
    lassert(text->data, "ptr");
    lassert(!text->text, "already assigned ptr");

    Text_split_data(text);

    text->text = malloc(sizeof(char**) * text->text_size);
    lassert(text->text, "");

    size_t text_idx = 1;
    text->text[0] = text->data;
    for (size_t i = 1; (i < text->data_size) && (text_idx <= text->text_size);
         ++i) {
        if (text->data[i - 1] == '\0') {
            text->text[text_idx] = (text->data) + i;
            ++text_idx;
        }
    }

    return INPUT_OK;
}

input_state Text_create(Text* text, const char* const filename)
{
    lassert(text, "Text is NULL");
    lassert(filename, "Filename is NULL");

    FILE* fd = fopen(filename, "r");
    if (!fd) {
        perror("cant open file");
        report_error(READ_FAILURE);
    }

    Text_set_size(text, &fd); // TODO: how to return error
    Text_set_text_data(text, &fd); // TODO: how to return error
    Text_set_text(text); // TODO: how to return errort

    return INPUT_OK;
}

input_state Text_kill(Text* text)
{
    assert(text && "Text is NULL");

    lassert(text->data, "");
    free(text->data);
    text->data = NULL;

    free(text->text);
    text->text = NULL;

    text->text_size = 0;
    text->data_size = 0;

    return INPUT_OK;
}

static char* ignore_n(const char* str, char* endptr, size_t* size)
{
    endptr = (char*)str;
    for (; *endptr != '\0'; ++endptr) {
        if (isalnum(*endptr))
            ++(*size);
    }

    char* format = malloc(sizeof(char) * (*size) + 1);
    lassert(format, "");

    size_t format_idx = 0;
    for (char* ptr = (char*)str; *ptr != '\0'; ++ptr) {
        if (isalnum(*ptr)) {
            format[format_idx++] = *ptr;
        }
    }

    format[*size] = '\0';

    return format;
}

bool Text_strcmp(const void* l_t, const void* r_t) {
    lassert(l_t, "");
    lassert(r_t, "");

    const char* l = *(char**)l_t;
    const char* r = *(char**)r_t;

    size_t len_l = 0;
    char* end_l = NULL;
    char* new_l = ignore_n(l, end_l, &len_l);

    size_t len_r = 0;
    char* end_r = NULL;
    char* new_r = ignore_n(r, end_r, &len_r);

    bool flag = false;
    if (len_l == len_r) {
        for (int i = 0; i < len_l; ++i) {
            if (tolower(new_l[i]) > tolower(new_r[i])) {
                flag = false;
                break;
            }
        }
        flag = true;
    } else if (len_l < len_r) {
        flag = true;
    } else {
        flag = false;
    }

    free(new_l);
    free(new_r);
    return flag;
}