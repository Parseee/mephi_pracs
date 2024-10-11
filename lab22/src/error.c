#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <limits.h>
#include <assert.h>
#include <math.h>
#include <sys/errno.h>

#include "error.h"

#define report_error(err)                                                \
    fprintf(stderr, err" in file %s on line %d\n", __FILE__, __LINE__); \
    return EXIT_FAILURE;

int handle_error(ERROR_CODE const error) {
    switch (error) {
        case CODE_OK:
            return EXIT_SUCCESS;
            break;

        case INTEGER_OVERFLOW_ERROR:
            report_error("Integer overflow");
            break;

        case LOGIC_ERROR:
            report_error("Logic error");
            break;

        case INPUT_ERROR:
            report_error("Input reading error");            
            break;

        case INVALID_INPUT_ERROR:
            report_error("Invalid input data");
            break;

        default:
            report_error("Something happened idk");
            break;
    }
}

static int check_EOF(char const * const str) { // what to return
    int idx = 0;
    while (str[idx] != '\0') {
        if (str[idx] == EOF) {
            return EOF;
        }
        ++idx;
    }
    return 0;
}

// static ERROR_CODE flush_stdin() {
//     int symbol = getchar();
//     for (;symbol != '\n' && symbol != EOF && ferror(stdin) == 0;
//         symbol = getchar());

//     return ferror(stdin) ? INPUT_ERROR : (symbol == EOF ? INPUT_ERROR : CODE_OK);
// }

ERROR_CODE handle_input_floating(char const * const input, float * const x) {
    assert(input && "input string is null");
    assert(x && "result is null");

    if (check_EOF(input)) {
        return INPUT_ERROR;
    }

    char *end_ptr;
    
    *x = strtof(input, &end_ptr);

    if ((*x == HUGE_VALF) && errno == ERANGE) {
        return INTEGER_OVERFLOW_ERROR;
    }

    if (errno == EINVAL) {
        return INVALID_INPUT_ERROR;
    }

    if (fflush(stdin)) {
        if (errno == EBADF) {
            return INPUT_ERROR;
        } else {
            return OTHER_ERROR; // too many errors from write()
        }
    }
    return CODE_OK;
}

ERROR_CODE handle_input_int(char const * const input, int64_t * const x) {
    assert(input && "input string is null");
    assert(x && "result is null");

    if (check_EOF(input)) {
        return INPUT_ERROR;
    }

    char *end_ptr;
    
    *x = strtoll(input, &end_ptr, 10);

    if ((*x == LLONG_MIN || *x == LLONG_MAX) && errno == ERANGE) {
        return INTEGER_OVERFLOW_ERROR;
    }

    if (errno == EINVAL) {
        return INVALID_INPUT_ERROR;
    }

    if (fflush(stdin)) {
        if (errno == EBADF) {
            return INPUT_ERROR;
        } else {
            return OTHER_ERROR; // too many errors from write()
        }
    }
    return CODE_OK;
}
