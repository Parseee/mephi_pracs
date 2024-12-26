#include "text.h"
#include <stddef.h>
#include <stdio.h>
// #include <string.h>

#define DEFAULT_TEXT_SIZE 100
#define NULSIZE 1

static TEXT_ERROR Text_add_line_to_text(Text* text, char* input_line);

TEXT_ERROR Text_create(Text* text)
{
    assert(text && "text is not init");

    text->text_size = 0;
    text->text_capacity = DEFAULT_TEXT_SIZE;
    text->text = (char**)malloc(sizeof(text->text) * text->text_capacity);

    char* input_line = NULL;
    FILE* f = fopen("Onegin.txt", "r+");
    // FILE* f = stdin;
    while ((input_line = freadline("", f)) != NULL) {
        if (Text_add_line_to_text(text, input_line)) {
            free(input_line);
            Text_destruct(text);
            return TEXT_ALLOC_ERROR;
        }
        // fprintf(stderr, "line %lu read with it's contents: \"%s\"\n", text->text_size, text->text[text->text_size - 1]);
    }
    return TEXT_OK;
}

static TEXT_ERROR Text_add_line_to_text(Text* text, char* input_line)
{
    assert(text && "text is not init");

    if (text->text_size + 1 >= text->text_capacity) {
        char** new_text = NULL;
        if ((new_text = realloc(text->text, text->text_capacity * 2 * sizeof(text->text))) == NULL) {
            return TEXT_ALLOC_ERROR;
        }

        text->text = new_text;
        new_text = NULL;
        text->text_capacity *= 2;
    }

    text->text[text->text_size] = input_line;
    text->text_size += NULSIZE;
    input_line = NULL;

    return TEXT_OK;
}

TEXT_ERROR Text_get_text(Text* text, FILE* file)
{
    assert(text && "text is not init");

    if (!text->text) {
        return TEXT_NOT_SET_ERROR;
    }

    fprintf(file, "\n");
    for (size_t i = 0; i < text->text_size; ++i) {
        fprintf(file, "%s\n", text->text[i]);
    }

    return TEXT_OK;
}

TEXT_ERROR Text_lengthify(Text* text)
{
    assert(text && "text is not init");

    if (!text->text) {
        return TEXT_NOT_SET_ERROR;
    }

    for (size_t i = 0; i < text->text_size; ++i) {
        char* new_line = malloc((str_len(text->text[i]) + NULSIZE) * 3 * sizeof(text->text[i]));
        char* end_ptr = new_line;
        char* token = str_tok(text->text[i], " \n\0");

        while (token != NULL) {
            size_t len = str_len(token);
            str_cpy(end_ptr, token);
            end_ptr += len;

            int cx = snprintf(end_ptr, 100, " %lu ", len);
            end_ptr += cx;

            token = str_tok(NULL, " \n\0");
        }
        *end_ptr = '\0';
        free(text->text[i]);
        text->text[i] = new_line;
        new_line = NULL;
    }

    return TEXT_OK;
}

TEXT_ERROR Text_destruct(Text* text)
{
    assert(text && "text is not init");

    if (text->text == NULL && text->text_capacity == 0 && text->text_size == 0) {
        return TEXT_OK;
    }

    for (size_t i = 0; i < text->text_capacity; ++i) {
        free(text->text[i]);
    }

    free(text->text);

    text->text = NULL;
    text->text_size = 0;
    text->text_capacity = 0;
    return TEXT_OK;
}
