#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "db/db.h"
#include "io_kit/io_kit.h"
#include "sort/sort.h"

typedef enum { QSORT,
    BUBBLE,
    SHELL,
    COCK_TAIL } sorting;

typedef enum { NAME,
    ID,
    TIME } compare;

bool name_cmp(const void* l, const void* r)
{
    char* lhs = (*(Item**)l)->name;
    char* rhs = (*(Item**)r)->name;
    return strcmp(lhs, rhs) <= 0;
}

bool id_cmp(const void* l, const void* r)
{
    char* lhs = (*(Item**)l)->id;
    char* rhs = (*(Item**)r)->id;
    return strcmp(lhs, rhs) <= 0;
}

bool time_cmp(const void* l, const void* r)
{
    time_t lhs = (*(Item**)l)->time;
    time_t rhs = (*(Item**)r)->time;
    return lhs <= rhs;
}

int main(int argc, char* argv[])
{
    DB db = (DB) { .capacity = 0, .data = NULL, .size = 0 };
    DB_create(&db);

    char* input_filename = NULL;
    char* output_filename = NULL;
    bool binary = false;
    sorting srt = QSORT;
    bool (*cmp)(const void*, const void*) = NULL;

    int c = 0;
    while ((c = getopt(argc, argv, "i:o:s:c:b")) != -1) {
        switch (c) {
        case ('b'):
            binary = true;
            break;

        case ('i'):
            input_filename = strdup(optarg);
            break;

        case ('o'):
            output_filename = strdup(optarg);
            break;

        case ('s'):
            if (strcmp(optarg, "bubble")) {
                srt = BUBBLE;
            } else if (strcmp(optarg, "shaker")) {
                srt = COCK_TAIL;
            } else if (strcmp(optarg, "shell")) {
                srt = SHELL;
            } else if (strcmp(optarg, "qsort")) {
                srt = QSORT;
            } else {
                printf("sorting provided is govno\n");
                exit(0);
            }

            break;

        case ('c'):
            if (strcmp(optarg, "name")) {
                cmp = name_cmp;
            } else if (strcmp(optarg, "id")) {
                cmp = id_cmp;
            } else if (strcmp(optarg, "time")) {
                cmp = time_cmp;
            } else {
                printf("comparator provided is govno\n");
                exit(0);
            }

            break;

        default:
            printf("dolboyob\n");
            exit(0);
        }
    }

    if (binary) {
        if (input_filename) {
            IO_binary_input(&db, input_filename);
            free(input_filename);
        } else {
            IO_terminal_input(&db);
        }
    } else {
        if (input_filename) {
            IO_text_input(&db, input_filename);
            free(input_filename);
        } else {
            IO_terminal_input(&db);
        }
    }

    if (srt == QSORT) {
        q_sort(db.data, db.size, sizeof(db.data), cmp);
    } else if (srt == COCK_TAIL) {
        shaker_sort(db.data, db.size, sizeof(db.data), cmp);
    } else if (srt == SHELL) {
        shell_sort(db.data, db.size, sizeof(db.data), cmp);
    } else {
        bubble_sort(db.data, db.size, sizeof(db.data), cmp);
    }

    if (binary) {
        if (output_filename) {
            IO_binary_output(&db, output_filename);
            free(output_filename);
        } else {
            IO_terminal_output(&db);
        }
    } else {
        if (output_filename) {
            IO_text_output(&db, output_filename);
            free(output_filename);
        } else {
            IO_terminal_output(&db);
        }
    }

    DB_destruct(&db);

    return EXIT_SUCCESS;
}