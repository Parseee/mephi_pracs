#include "text.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_TEXT_SIZE 100
#define NULSIZE 1

static TEXT_ERROR Text_add_line_to_text(Text* text, String* input_line);

TEXT_ERROR Text_create(Text* text)
{
    assert(text && "text is not init");

    text->text_size = 0;
    text->text_capacity = DEFAULT_TEXT_SIZE;
    text->text = calloc(DEFAULT_TEXT_SIZE, sizeof(text->text));

    String* str = NULL;
    while ((str = String_create_string(NULL))->let != EOF) {
        if (Text_add_line_to_text(text, str)) {
            String_kill_string(str);
            fprintf(stderr, "can't add line\n");
        }
    }
    String_kill_string(str);

    return TEXT_OK;
}

static TEXT_ERROR Text_add_line_to_text(Text* text, String* input_line)
{
    assert(text && "text is not init");

    if (text->text_size + 1 >= text->text_capacity) {
        String** new_text = NULL;
        if ((new_text = realloc(text->text, text->text_capacity * 2 * sizeof(text->text))) == NULL) {
            Text_destruct(text);
            return TEXT_ALLOC_ERROR;
        }

        text->text = new_text;
        new_text = NULL;
        text->text_capacity *= 2;
    }

    text->text[text->text_size] = input_line;
    text->text_size += 1;
    input_line = NULL;

    return TEXT_OK;
}

TEXT_ERROR Text_get_text(Text* text)
{
    assert(text && "text is not init");

    if (!text->text) {
        return TEXT_NOT_SET_ERROR;
    }

    for (size_t i = 0; i < text->text_size; ++i) {
        String_print_string(text->text[i]);
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
        if (text->text[i]) {
            String_kill_string(text->text[i]);
        }
    }

    free(text->text);

    text->text = NULL;
    text->text_size = 0;
    text->text_capacity = 0;
    return TEXT_OK;
}

void Text_sort(Text* text)
{
    for (int i = 0; i < text->text_size; ++i) {
        if (text->text[i]->let != '\n') {
            String_sort(&text->text[i]);
        }
    }
}

/*
ldfkaldkmas as as as das das dsdasd
aslkd 124 12343kn4 234
d
23       f
*/

/*
dsdasd das das as as as ldfkaldkmas
234 12343kn4 124 aslkd
d
f 23
*/