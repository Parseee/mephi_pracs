#include <ctype.h>
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

bool n_name_cmp(const void* l, const void* r)
{
    char* lhs = (*(Item**)l)->name;
    char* rhs = (*(Item**)r)->name;
    return strcmp(rhs, lhs) <= 0;
}

bool n_id_cmp(const void* l, const void* r)
{
    char* lhs = (*(Item**)l)->id;
    char* rhs = (*(Item**)r)->id;
    return strcmp(rhs, lhs) <= 0;
}

bool n_time_cmp(const void* l, const void* r)
{
    time_t lhs = (*(Item**)l)->time;
    time_t rhs = (*(Item**)r)->time;
    return lhs >= rhs;
}

int main(int argc, char* argv[])
{
    DB db = (DB) { .capacity = 0, .data = NULL, .size = 0 };
    DB_create(&db);

    char* input_filename = NULL;
    char* output_filename = NULL;
    bool binary = false;
    bool descending = false;
    void (*sorting)(void* const array, const ssize_t array_size, const ssize_t size, bool (*compare)(const void*, const void*)) = q_sort;
    bool (*cmp)(const void*, const void*) = name_cmp;
    bool (*n_cmp)(const void*, const void*) = n_name_cmp;

    int c = 0;
    while ((c = getopt(argc, argv, "i:o:s:c:bd")) != -1) {
        switch (c) {
        case ('b'):
            binary = true;
            break;

        case ('d'):
            descending = true;
            break;

        case ('i'):
            input_filename = strdup(optarg);
            break;

        case ('o'):
            output_filename = strdup(optarg);
            break;

        case ('s'):
            if (strcmp(optarg, "bubble") == 0) {
                sorting = bubble_sort;
            } else if (strcmp(optarg, "shaker") == 0) {
                sorting = shaker_sort;
            } else if (strcmp(optarg, "shell") == 0) {
                sorting = shell_sort;
            } else if (strcmp(optarg, "qsort") == 0) {
                sorting = q_sort;
            } else {
                fprintf(stderr, "sorting provided is govno\n");
                exit(0);
            }

            break;

        case ('c'):
            if (strcmp(optarg, "name") == 0) {
                cmp = name_cmp;
                n_cmp = n_name_cmp;
            } else if (strcmp(optarg, "id") == 0) {
                cmp = id_cmp;
                n_cmp = n_id_cmp;
            } else if (strcmp(optarg, "time") == 0) {
                cmp = time_cmp;
                n_cmp = n_time_cmp;
            } else {
                printf("comparator provided is govno\n");
                exit(0);
            }

            break;

        case ('?'):
            if (optopt == 's')
                fprintf(stderr, "option -%c requires an argument\n", optopt);
            else if (isprint(optopt)) {
                fprintf(stderr, "unknown option -%c\n", optopt);
            } else {
                fprintf(stderr, "unknown option character -%x\n", optopt);
            }

        default:
            printf("durachok\n");
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

    if (descending) {
        sorting(db.data, db.size, sizeof(db.data), n_cmp);
    } else {
        sorting(db.data, db.size, sizeof(db.data), cmp);
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