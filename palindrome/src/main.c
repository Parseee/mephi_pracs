#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>


typedef enum { CODE_OK = 0, INTEGER_OVERFLOW_ERROR, LOGIC_ERROR, INPUT_ERROR} ERROR_CODE;

ERROR_CODE input_handling(const char *, long long *);
ERROR_CODE scan(long long *);
ERROR_CODE pal(long long *);
long long reverse(long long);
int HANDLE_ERROR(const ERROR_CODE);


int main(void) {
    /*
    char input[100];
    fgets(input, 100, stdin);
    long long x = 0;

    if (HANDLE_ERROR(input_handling(input, &x))) {
        return 1;
    }
    */

    if (HANDLE_ERROR(pal(&x))) {
        return 1;
    }

    printf("%lli", x);
    return 0;
}

ERROR_CODE pal(long long *x) {
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

int HANDLE_ERROR(const ERROR_CODE e) {
    switch (e) {
        case CODE_OK:
            fprintf(stderr, "OK\n");
            return 0;
            break;

        case INTEGER_OVERFLOW_ERROR:
            fprintf(stderr, "Integer overflow\n");
            return 1;
            break;

        case LOGIC_ERROR:
            perror("Logic error happened");
            return 1;
            break;

        default:
            fprintf(stderr, "Something happened idk\n");
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

ERROR_CODE scan(long long *result) {
    assert(result && "result is NULL");

    int correct_scan_count = scanf("%lli", result);
    int nxt = (feof(stdin) ? EOF : getchar());

    if (ferror(stdin) == 0) {
        if (correct_scan_count == 1 && nxt == (int)'\n') {
            return CODE_OK;
        } else if (correct_scan_count == EOF) {
            return INPUT_ERROR;
        } else {
            return LOGIC_ERROR; // here should have been INT_OWF error but uh duh eah
        } 
    }

    return CODE_OK;
}
