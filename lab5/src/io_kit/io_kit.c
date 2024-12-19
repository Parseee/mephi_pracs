#include "io_kit.h"
#include "../util.h"
#include <stdio.h>
#include <string.h>
#include <regex.h>

// static IO_state construct_item(Item* item, char* str);

IO_state IO_keyboard_input(DB* db)
{
    assert(db);

    ssize_t read = 0;
    size_t len = 0;
    char* line = NULL;
    while ((read = getline(&line, &len, stdin)) != -1) {
        Item* new_item = NULL;
        construct_item(new_item, line);
        DB_append(db, new_item);
    }

    return IO_OK;
}

IO_state IO_text_input(DB* db, const char* const filename)
{
    assert(db);

    FILE* fd = NULL;
    if ((fd = fopen(filename, "r+"))) {
        report_error("can't open text input file", IO_INPUT_ERROR);
    }

    ssize_t read = 0;
    size_t len = 0;
    char* line = NULL;
    while ((read = getline(&line, &len, fd)) != -1) {
        Item* new_item = NULL;
        construct_item(new_item, line);
        DB_append(db, new_item);
    }

    return IO_OK;
}

IO_state IO_binary_input(DB* db, const char* const filename)
{
    assert(db);

    FILE* fd = NULL;
    if ((fd = fopen(filename, "rb"))) {
        report_error("can't open binary input file", IO_INPUT_ERROR);
    }

    // some read here

    return IO_OK;
    return IO_OK;
}

IO_state construct_item(Item* item, char* str)
{
    item = calloc(1, sizeof(*item));

    int reg_i = 0;
    regex_t name;
    if ((reg_i = regcomp(&name, "^\\w+\\s\\w+\\s\\w+$", REG_EXTENDED)) != 0) {
        char msgbuf[256];
        regerror(reg_i, &name, msgbuf, sizeof(msgbuf));
        report_error(msgbuf, IO_INTERNAL_ERROR);
    }

    reg_i = regexec(&name, str, 0, NULL, 0);

    // char *msgbuf = NULL;
    // msgbuf = calloc(regerror(reg_i, &name, msgbuf, sizeof(msgbuf)), sizeof(msgbuf));

    fprintf(stderr, "%d\n", reg_i);;

    return IO_OK;
}