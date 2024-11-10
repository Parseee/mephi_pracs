#include "exec.h"

#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../array/array.h"

static error_state Array_insert_wrapper(Array* array);
static error_state Array_init_wrapper(Array* array);
static error_state Array_remove_wrapper(Array* array);
static error_state input_handle(int64_t* res, FILE* file, size_t lengh);

error_state exec(Array* array)
{
    lassert(array, "");

    fprintf(stdout, "1. Init array with keyboard input\n"
                    "2. Add element to index\n"
                    "3. Remove element on index\n"
                    "4. User func\n"
                    "5. Print array\n"
                    "6. Destroy array\n");

    fprintf(stdout, "Enter command: ");
    int64_t mode = 0;
    input_handle(&mode, stdin, 1);
    int64_t val = 0;
    int64_t idx = 0;
    error_state result = OK;
    switch (mode) {
    case (1):
        result = Array_init_wrapper(array);
        Array_print(array);
        break;
    case (2):
        result = Array_insert_wrapper(array);
        Array_print(array);
        break;
    case (3):
        result = Array_remove_wrapper(array);
        Array_print(array);
        break;
    case (4):
        val = 0;
        result = Array_func(array, &val);
        printf("%lld\n", val);
        Array_print(array);
        break;
    case (5):
        result = Array_print(array);
        break;
    case (6):
        result = Array_kill(array);
        break;
    default:
        // report_error(LOGIC_ERROR, "invalid input");
        fprintf(stdout, "Wrong input. Try another one!\n\n");
        break;
    }

    return OK;
}

static error_state input_handle(int64_t* res, FILE* file, size_t length)
{
    char buffer[16];
    if (feof(file)) {
        // signal(SIGKILL, *signal handler*);
        exit(EXIT_SUCCESS);
    }
    if (((fgets(buffer, sizeof(buffer), file)) < 0)) {
        report_error(INPUT_ERROR, "");
    }
    // if (buffer[length] != '\n') {
    //     report_error(INPUT_ERROR, "input is too long");
    // }

    char* end_ptr = NULL;
    *res = strtoll(buffer, &end_ptr, 10);
    if ((*res == INT_MAX || *res == INT_MIN) && errno != ERANGE) {
        report_error(INPUT_ERROR, "");
    }
    return OK;
}

static error_state Array_init_wrapper(Array* array)
{
    if (array->data) {
        report_error(LOGIC_ERROR, "\narray already initialized\n\n");
    }

    fprintf(stdout, "How manny elements to init: ");
    int64_t idx, val;
    input_handle(&idx, stdin, 8);
    if (0ull > idx) {
        report_error(LOGIC_ERROR, "bad amount");
    }

    for (size_t i = 0; i < idx; ++i) {
        input_handle(&val, stdin, 8);
        Array_insert(array, val, i);
    }
    return OK;
}

static error_state Array_insert_wrapper(Array* array)
{
    if (!array->data) {
        report_error(LOGIC_ERROR, "\narray is not initialized\n\n");
    }
    fprintf(stdout, "Index of insertion (1 indexed): ");
    int64_t idx, val;
    input_handle(&idx, stdin, 8);
    if (0ull > idx) {
        report_error(LOGIC_ERROR, "index out of range");
    }

    fprintf(stdout, "Element to insert: ");
    input_handle(&val, stdin, 8);

    if (Array_insert(array, val, idx - 1) == OK) {
        fprintf(stderr, "\nelemet inserted\n\n");
    }
    return OK;
}

static error_state Array_remove_wrapper(Array* array)
{
    if (!array->data) {
        report_error(LOGIC_ERROR, "\narray is not initialized\n\n");
    }
    fprintf(stdout, "Index of element to remove (1 indexed): ");
    int64_t idx;
    input_handle(&idx, stdin, 8);
    if (0ull > idx) {
        report_error(LOGIC_ERROR, "index out of range");
    }

    if (Array_remove(array, idx - 1) == OK) {
        fprintf(stderr, "\nelement removed\n\n");
    }
    return OK;
}