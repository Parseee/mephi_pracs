#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <assert.h>

typedef enum { CODE_OK = 0, INTEGER_OVERFLOW_ERROR, LOGIC_ERROR, INPUT_ERROR} ERROR_CODE;

ERROR_CODE input_handling(const char *, long long *);
ERROR_CODE pal(long long *);
long long reverse(long long);
int HANDLE_ERROR(const ERROR_CODE);


int main(void) {
    char input[100];
    fgets(input, 100, stdin);
    long long x = 0;

    fprintf(stderr, "read: ");
    if (HANDLE_ERROR(input_handling(input, &x))) {
        return EXIT_FAILURE;
    }

    fprintf(stderr, "compute: ");
    if (HANDLE_ERROR(pal(&x))) {
        return EXIT_FAILURE;
    }

    printf("%lli\n", x);
    return EXIT_SUCCESS;
}

ERROR_CODE pal(long long *x) {
    assert(x && "palindrom is NULL");

    long long rev_x = reverse(*x);
    if (rev_x == *x) {
        return CODE_OK;
    } else {
        if (rev_x + *x > INT_MAX) {
           return INTEGER_OVERFLOW_ERROR;
        }
        *x += rev_x;
        return pal(x);
    }
}

long long reverse(long long x) {
    long long t_x = x;
    long long y = 0;

    while (t_x > 0) {
        y *= 10;
        y += t_x % 10;
        t_x /= 10;
    }

    return y;
}

int HANDLE_ERROR(const ERROR_CODE error) {
    switch (error) {
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

        case INPUT_ERROR:
            fprintf(stderr, "Input error\n");
            return EXIT_FAILURE;
            break;

        default:
            fprintf(stderr, "Something happened idk\n");
            return EXIT_FAILURE;
            break;
    }
}

ERROR_CODE input_handling(const char *input, long long *x) {
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
