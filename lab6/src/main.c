#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "text/text.h"

int main(void)
{
    Text tx;

    if (Text_create(&tx)) {
        Text_destruct(&tx);
        return EXIT_FAILURE;
    }

    Text_sort(&tx);

    Text_get_text(&tx);

    Text_destruct(&tx);

    return EXIT_SUCCESS;
}
