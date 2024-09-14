#include <stdio.h>
#include <limits.h>


enum ERROR_CODE { OK = 0, INTEGER_OVERFLOW };

enum ERROR_CODE pal(long long *);
long long reverse(long long);
int HANDLE_ERROR(const enum ERROR_CODE);


int main(const signed argc, const char* argv[]) {
    long long x;
    scanf("%lli", &x);
    if (HANDLE_ERROR(pal(&x))) {
        return 1;
    }

    printf("%lli", x);
    return 0;
}

enum ERROR_CODE pal(long long *x) {
    long long rev_x = reverse(*x);
    if (rev_x == *x) {
        return OK;
    } else {
        if (rev_x + *x > INT_MAX) {
           return INTEGER_OVERFLOW;
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

int HANDLE_ERROR(const enum ERROR_CODE e) {
    switch (e) {
        case OK:
            fprintf(stderr, "OK\n");
            return 0;
            break;
        case INTEGER_OVERFLOW:
            fprintf(stderr, "INTEGER OVERFLOW\n");
            return 1;
            break;
        default:
            fprintf(stderr, "SOMETHING HAPPENED\n");
            return 1;
            break;
    }
}
