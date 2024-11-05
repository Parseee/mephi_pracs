#include "exec.h"

#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../array/array.h"

static error_state input_handle(int* res, FILE* file, size_t lengh)
{
    // char* buffer = NULL;
    // int buff_size = 1024;
    // int in_use = 0;
    // int nbytes = 0;

    // if (!(buffer = malloc(buff_size * sizeof(char)))) {
    //     report_error(ALLOC_ERROR, "");
    // }

    // while ((nbytes = fread(buffer + in_use, sizeof(char), buff_size - in_use, file))) {
    //     if (nbytes < 0) {
    //         report_error(INPUT_ERROR, "");
    //     }
    //     in_use += nbytes;
    //     if (in_use >= lengh) {
    //         report_error(INPUT_ERROR, "too lenghty input");
    //     }
    //     if (in_use >= buff_size) {
    //         char* new_buffer;
    //         if ((new_buffer = realloc(buffer, buff_size * 2)) == 0) {
    //             report_error(ALLOC_ERROR, "");
    //         }
    //         buffer = new_buffer;
    //     }
    // }

    char buffer[8];
    if ((fgets(buffer, sizeof(buffer), stdin)) < 0) {
        report_error(INPUT_ERROR, "");
    }

    char* end_ptr = NULL;
    *res = strtoll(buffer, &end_ptr, 10);
    if ((*res == INT_MAX || *res == INT_MIN) && errno != ERANGE) {
        report_error(INPUT_ERROR, "");
    }
    return OK;
}

error_state exec(Array* array)
{
    lassert(array, "");

    fprintf(stdout, "1. Create(allocate) array\n"
                    "2. Init array with keyboard input\n"
                    "3. Add element to index\n"
                    "4. Remove element on index\n"
                    "5. User func\n"
                    "6. Print array\n"
                    "7. Destroy array\n");

    fprintf(stdout, "Enter command: ");
    int mode = 0;
    input_handle(&mode, stdin, 1);
    int val = 0;
    int idx = 0;
    switch (mode) {
    case (1):
        fprintf(stdout, "How many elements to init: ");
        input_handle(&val, stdin, 1);

        if (Array_create(array, val) == OK) {
            fprintf(stdout, "\narray created\n\n");
        }
        break;
    case (2):
        fprintf(stdout, "How manny elements to init: ");
        input_handle(&val, stdin, 8);

        if (Array_init(array, val, input_handle) == OK) {
            fprintf(stderr, "\narray init\n\n");
        }
        break;
    case (3):
        fprintf(stdout, "Index of insertion (1 indexed): ");
        input_handle(&idx, stdin, 8);
        fprintf(stdout, "Element to insert: ");
        input_handle(&val, stdin, 8);
        
        if(Array_insert(array, val, idx - 1) == OK) {
            fprintf(stderr, "elemets inserted");
        }
        break;
    case (4):
        fprintf(stdout, "Index of element to remove (1 indexed): ");
        input_handle(&idx, stdin, 8);

        if (Array_remove(array, idx - 1) == OK) {
            fprintf(stderr, "element removed");
        }
        break;
    case (5):
        Array_func(array);
        break;
    case (6):
        Array_print(array);
        break;
    case (7):
        if (Array_kill(array) == OK) {
            fprintf(stderr, "\narray killed\n\n");
        }
        break;
    default:
        report_error(LOGIC_ERROR, "invalid input");
        break;
    }

    return OK;
}