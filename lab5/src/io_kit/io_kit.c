#include "io_kit.h"
#include "../util.h"
#include <errno.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static IO_state construct_item(Item** item, char* str);

IO_state IO_terminal_input(DB* db)
{
    assert(db);

    ssize_t read = 0;
    size_t len = 0;
    char* line = NULL;
    while ((read = getline(&line, &len, stdin)) != -1) {
        Item* new_item = NULL;
        construct_item(&new_item, line);
        if (new_item == NULL) {
            report_error("can't create item", IO_INTERNAL_ERROR);
        } else {
            DB_append(db, new_item);
        }
    }

    return IO_OK;
}

IO_state IO_text_input(DB* db, const char* const filename)
{
    assert(db);

    FILE* fd = NULL;
    if ((fd = fopen(filename, "r+")) == NULL) {
        report_error("can't open text input file", IO_INPUT_ERROR);
        return IO_INPUT_ERROR;
    }

    ssize_t read = 0;
    size_t len = 0;
    char* line = NULL;
    while ((read = getline(&line, &len, fd)) != -1) {
        Item* new_item = NULL;
        construct_item(&new_item, line);
        if (new_item == NULL) {
            report_error("can't create item", IO_INTERNAL_ERROR);
        } else {
            DB_append(db, new_item);
        }
    }

    fclose(fd);

    return IO_OK;
}

IO_state IO_binary_input(DB* db, const char* const filename)
{
    assert(db);

    FILE* fd = NULL;
    if ((fd = fopen(filename, "rb")) == NULL) {
        report_error("can't open binary input file", IO_INPUT_ERROR);
        return IO_INPUT_ERROR;
    }

    fseek(fd, SEEK_END, SEEK_END);
    ssize_t len = ftell(fd);
    rewind(fd);

    char* buf = calloc(len, sizeof(*buf));

    if (fread(buf, sizeof(char), len, fd) < 0) {
        report_error("can't read file", IO_INPUT_ERROR);
        return IO_INPUT_ERROR;
    }

    ssize_t read = 0;
    size_t len_r = 0;
    char* line = strtok(buf, "\n");
    while (line) {
        Item* new_item = NULL;
        construct_item(&new_item, line);
        if (new_item == NULL) {
            report_error("can't create item", IO_INTERNAL_ERROR);
        } else {
            DB_append(db, new_item);
        }
        line = strtok(NULL, "\n");
    }

    return IO_OK;
}

static IO_state construct_item(Item** item, char* str)
{

    regex_t name;
    int reg_i = 0;
    // sry for regex POSIX sucks ass
    const char* pattern = "^([[:alpha:]]*[[:space:]][[:alpha:]]*[[:space:]][[:alpha:]]*)[[:space:]]([[:digit:]]{2}[[:upper:]]{2}-[[:upper:]]{4})[[:space:]]([[:digit:]]*)[\n]?$";
    regmatch_t pmatch[4];
    reg_i = regcomp(&name, pattern, REG_EXTENDED);
    if (reg_i) {
        char msgbuf[256];
        regerror(reg_i, &name, msgbuf, sizeof(msgbuf));
        report_error(msgbuf, IO_INTERNAL_ERROR);
    }

    reg_i = regexec(&name, str, 4, pmatch, 0);

    if (reg_i == 0) {
        *item = calloc(1, sizeof(**item));
        (*item)->name = calloc(pmatch[1].rm_eo - pmatch[1].rm_so + 1, sizeof((*item)->name));
        for (size_t i = 0; i < pmatch[1].rm_eo - pmatch[1].rm_so; ++i) {
            (*item)->name[i] = *(str + pmatch[1].rm_so + i);
        }
        (*item)->name[pmatch[1].rm_eo - pmatch[1].rm_so + 1] = '\0';

        (*item)->id = calloc(pmatch[2].rm_eo - pmatch[2].rm_so + 1, sizeof((*item)->id));
        for (size_t i = 0; i < pmatch[2].rm_eo - pmatch[2].rm_so; ++i) {
            (*item)->id[i] = *(str + pmatch[2].rm_so + i);
        }
        (*item)->id[pmatch[2].rm_eo - pmatch[2].rm_so + 1] = '\0';

        char* eptr = NULL;
        (*item)->time = strtoll(str + pmatch[3].rm_so, &eptr, 10);
        if (errno == ERANGE) {
            report_error("time overflow", IO_INTERNAL_ERROR);
        }
    } else if (reg_i == REG_NOMATCH) {
        item = NULL;
    } else {
        char msgbuf[256];
        regerror(reg_i, &name, msgbuf, sizeof(msgbuf));
        report_error(msgbuf, IO_INTERNAL_ERROR);
    }

    return IO_OK;
}

IO_state IO_terminal_output(DB* db)
{
    for (int i = 0; i < db->size; ++i) {
        printf("%s %s %ld\n", db->data[i]->name, db->data[i]->id, db->data[i]->time);
    }
    return IO_OK;
}

IO_state IO_text_output(DB* db, const char* const filename)
{
    FILE* fd;
    if ((fd = fopen(filename, "w")) == NULL) {
        report_error("can't open text output file", IO_OUTPUT_ERROR);
        return IO_INPUT_ERROR;
    }
    for (int i = 0; i < db->size; ++i) {
        fprintf(fd, "%s %s %ld\n", db->data[i]->name, db->data[i]->id, db->data[i]->time);
    }
    return IO_OK;
}

static void add_to_binary(char** buf, size_t* bufsize, size_t* bufcap, char* str, char** eptr)
{
    size_t len = strlen(str) + 1;
    if (*bufsize + len > *bufcap) {
        char* new_buf = NULL;
        if ((new_buf = realloc(*buf, *bufcap * 2)) == NULL) {
            report_error("alloc error\n", IO_INTERNAL_ERROR);
            // return IO_INTERNAL_ERROR;
        }
        *buf = new_buf;
        new_buf = NULL;
    }
    strcpy(*eptr, str);
    *eptr += len;
    *((*eptr) - 1) = ' ';
    *bufsize += len;
}

IO_state IO_binary_output(DB* db, const char* const filename)
{
    FILE* fd;
    if ((fd = fopen(filename, "wb")) == NULL) {
        report_error("can't open binary output file", IO_OUTPUT_ERROR);
        return IO_OUTPUT_ERROR;
    }
    // size_t bufsize = 0;
    // size_t bufcap = db->size * 1000;
    // char* buf = calloc(bufcap, sizeof(char));
    // char* eptr = buf;

    // for (int i = 0; i < db->size; ++i) {
    //     add_to_binary(&buf, &bufsize, &bufcap, db->data[i]->name, &eptr);
    //     add_to_binary(&buf, &bufsize, &bufcap, db->data[i]->id, &eptr);
    //     char* timebuf = calloc(18, sizeof(*timebuf));
    //     snprintf(timebuf, 18, "%ld", db->data[i]->time);
    //     add_to_binary(&buf, &bufsize, &bufcap, timebuf, &eptr);
    //     free(timebuf);
    //     *eptr = '\0';
    //     ++eptr;
    //     printf("%s\n", buf);
    // }

    // fwrite(buf, sizeof(*buf), bufsize, fd);

    for (int i = 0; i < db->size; ++i) {
        fprintf(fd, "%s %s %ld\n", db->data[i]->name, db->data[i]->id, db->data[i]->time);
    }
    return IO_OK;
}