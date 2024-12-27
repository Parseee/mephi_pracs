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

    char* output_filename = NULL;
    ssize_t len = 20;
    ssize_t quantity = 10;
    bool descending = false;
    void (*sorting)(void* const array, const ssize_t array_size, const ssize_t size, bool (*compare)(const void*, const void*));
    bool (*cmp)(const void*, const void*) = name_cmp;
    bool (*n_cmp)(const void*, const void*) = n_name_cmp;

    int c = 0;
    while ((c = getopt(argc, argv, "q:l:o:s:c:d")) != -1) {
        switch (c) {

        case ('d'):
            descending = true;
            break;

        case ('o'):
            output_filename = strdup(optarg);
            break;

        case ('s'):
            if (strcmp(optarg, "bubble")) {
                sorting = bubble_sort;
            } else if (strcmp(optarg, "shaker")) {
                sorting = shaker_sort;
            } else if (strcmp(optarg, "shell")) {
                sorting = shell_sort;
            } else if (strcmp(optarg, "qsort")) {
                sorting = q_sort;
            } else {
                fprintf(stderr, "sorting provided is govno\n");
                exit(0);
            }
            break;

        case ('c'):
            if (strcmp(optarg, "name")) {
                cmp = name_cmp;
                n_cmp = n_name_cmp;
            } else if (strcmp(optarg, "id")) {
                cmp = id_cmp;
                n_cmp = n_id_cmp;
            } else if (strcmp(optarg, "time")) {
                cmp = time_cmp;
                n_cmp = n_time_cmp;
            } else {
                printf("comparator provided is govno\n");
                exit(0);
            }
            break;

        case ('l'):
            len = strtoll(optarg, NULL, 10);
            break;

        case ('q'):
            quantity = strtoll(optarg, NULL, 10);
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

    FILE* fd = fopen(output_filename, "w+");
    free(output_filename);
    fprintf(fd, "x y\n");
    time_t begin, end;
    DB db = (DB) { .capacity = 0, .data = NULL, .size = 0 };
    for (int i = 1; i <= 10; ++i) {
        begin = clock();
        DB_create(&db);

        IO_generate_input(&db, quantity, len);

        if (descending) {
            sorting(db.data, db.size, sizeof(db.data), n_cmp);
        } else {
            sorting(db.data, db.size, sizeof(db.data), cmp);
        }

        DB_destruct(&db);
        quantity *= 2;

        end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        fprintf(fd, "%ld %lf\n", quantity, time_spent);
    }

    fclose(fd);

    return EXIT_SUCCESS;
}