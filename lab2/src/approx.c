#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <assert.h>

#include "error.h"

ERROR_CODE handle_input(const char *, int64_t *);
ERROR_CODE compute(int64_t *, int64_t *, int64_t *);

int main(void) {
    printf("Enter the equation's agrs\n");
    long long x;
    long long accuracy;
    char input[100];
    
    printf("x: ");
    fgets(input, sizeof(input), stdin);
    handle_error(handle_input(input, &x));

    printf("accuracy (10^-n): ");
    fgets(input, sizeof(input), stdin);
    handle_error(handle_input(input, &accuracy));
    
    return EXIT_SUCCESS;
}

/*
ERROR_CODE compute(int64_t *x, int64_t *m, int64_t *res) {
    
    
    return CODE_OK;
}
*/

ERROR_CODE handle_input(const char *input, int64_t *x) {
    assert(input && "input string is null");
    assert(x && "result is null");

    char *end_ptr;
    
    *x = strtoll(input, &end_ptr, 10);

    if ((*x == LLONG_MIN || *x == LLONG_MAX) && errno == ERANGE) {
        return INTEGER_OVERFLOW_ERROR;
    }

    if (errno == EINVAL) {
        return LOGIC_ERROR;
    }

    return CODE_OK;
}
