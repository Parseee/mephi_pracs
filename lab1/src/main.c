#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <assert.h>

typedef enum { CODE_OK = 0, INTEGER_OVERFLOW_ERROR, LOGIC_ERROR, INPUT_ERROR} ERROR_CODE;

ERROR_CODE input_handling(const char *, int64_t *);
ERROR_CODE compute(int64_t *, int64_t *, int64_t *);
ERROR_CODE flushcycle();
int HANDLE_ERROR(const ERROR_CODE);


int main(void) {
    char input[200];
    int64_t inp_val_x = 0;
    int64_t inp_val_m = 0;

    fgets(input, 200, stdin);
    fprintf(stderr, "read 1st: ");
    if (HANDLE_ERROR(input_handling(input, &inp_val_x))) {
        return EXIT_FAILURE;
    }

    fgets(input, 200, stdin);
    fprintf(stderr, "read 2nd: ");
    if (HANDLE_ERROR(input_handling(input, &inp_val_m))) {
        return EXIT_FAILURE;
    }

    printf("\n%lli %lli\n", inp_val_m, inp_val_x);

    int64_t result = 0;
    fprintf(stderr, "compute: ");
    if (HANDLE_ERROR(compute(&inp_val_x, &inp_val_m, &result))) {
        return EXIT_FAILURE;
    }

    printf("the result is: %lli\n", result);
    return EXIT_SUCCESS;
}

ERROR_CODE compute(int64_t *x, int64_t *m, int64_t *res) {
    assert(x && "bad inp_val");
    assert(m && "bad add_val");
    assert(res && "bad res_val");

    int64_t tmp_x = *x;
    int64_t base = 1;

    while (tmp_x > 0) {
        *res += ((int64_t)((tmp_x % 10) + *m) % 10) * base;
        tmp_x /= 10;
        base *= 10;
    }
    return CODE_OK;
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

ERROR_CODE flushcycle() {
    int symbol = getchar();
    for (;symbol != '\n' && symbol != EOF && ferror(stdin) == 0;
        symbol = getchar());

    return ferror(stdin) ? INPUT_ERROR : 
                        (symbol == EOF ? LOGIC_ERROR : CODE_OK);
}

ERROR_CODE input_handling(const char *input, int64_t *x) {
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

//     // flushing stdin
// #if __linux__
//     fpurge(stdin);
//     return CODE_OK;
// #else
//     return HANDLE_ERROR(flushcycle());
// #endif // __linux__
    return CODE_OK;
}
