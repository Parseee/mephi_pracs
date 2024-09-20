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

    if (HANDLE_ERROR(input_handling(input, &x))) {
        return 1;
    }

    long long sum = 0;
    if (HANDLE_ERROR(dij_sum(&sum, x))) {
        return 1;
    }

    printf("%lli", sum);
    return EXIT_FAILURE;
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
            fprintf(stderr, "CODE_OK\n");
            return 0;
            break;

        case INTEGER_OVERFLOW_ERROR:
            fprintf(stderr, "\n");
            return 1;
            break;

        case LOGIC_ERROR:
            fprintf(stderr, "Logic error\n");
            return 1;
            break;

        default:
            fprintf(stderr, "SOMETHING HAPPENED\n");
            return 1;
            break;
    }
}

ERROR_CODE input_handling(const char *input, long long *x) {
    assert(input && "input string is null");
    assert(x && "result is null");

    char *eptr;
            
    *x = strtol(input, &eptr, 10);

    if ((*x == LLONG_MIN || *x == LLONG_MAX) && errno == ERANGE) {
        return INTEGER_OVERFLOW_ERROR;
    }

    if (errno == EINVAL) {
        return LOGIC_ERROR;
    }

    return CODE_OK;
}
