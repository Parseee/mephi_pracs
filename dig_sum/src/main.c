#include <stdio.h>
#include <limits.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>


typedef enum { CODE_OK = 0, INTEGER_OVERFLOW_ERROR, LOGIC_ERROR} ERROR_CODE;

ERROR_CODE dij_sum(long long *, long long);
ERROR_CODE input_handling(const char *, long long *);
int HANDLE_ERROR(const ERROR_CODE);


int main(void) {
    char input[100];
    fgets(input, 100, stdin);
    
    long long x = 0;
    fprintf(stderr, "read: ");
    if (HANDLE_ERROR(input_handling(input, &x))) {
        return EXIT_FAILURE;
    }

    long long sum = 0;
    fprintf(stderr, "compute: ");
    if (HANDLE_ERROR(dij_sum(&sum, x))) {
        return EXIT_FAILURE;
    }

    printf("%lli\n", sum);
    return EXIT_SUCCESS;
}

ERROR_CODE dij_sum(long long *sum, long long x) {
    while(x > 0) {
        *sum += x % 10;
        x /= 10;
    }
    return CODE_OK;

}

int HANDLE_ERROR(const ERROR_CODE e) {
    switch (e) {
        case CODE_OK:
            fprintf(stderr, "OK\n");
            return EXIT_SUCCESS;
            break;

        case INTEGER_OVERFLOW_ERROR:
            fprintf(stderr, "Integer overflow\n");
            return EXIT_FAILURE;
            break;

        case LOGIC_ERROR:
            fprintf(stderr, "Logic error\n");
            return EXIT_FAILURE;
            break;

        default:
            fprintf(stderr, "Something happened\n");
            return EXIT_FAILURE;
            break;
    }
}

ERROR_CODE input_handling(const char *input, long long *x) {
    assert(input && "input string is null");
    assert(x && "result is null");

    char *end_ptr;
            
    *x = strtol(input, &end_ptr, 10);

    if ((*x == LLONG_MIN || *x == LLONG_MAX) && errno == ERANGE) {
        return INTEGER_OVERFLOW_ERROR;
    }

    if (errno == EINVAL) {
        return LOGIC_ERROR;
    }

    return CODE_OK;
}
