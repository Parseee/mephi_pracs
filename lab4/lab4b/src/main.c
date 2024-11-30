#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "text/text.h"


int main(void)
{
    Text tx;

    if (Text_create(&tx)) {
        Text_destruct(&tx);
        return EXIT_FAILURE;
    } // how to track error properly here?

    Text_lengthify(&tx);

    Text_get_text(&tx, stdout);

    Text_destruct(&tx);
    return EXIT_SUCCESS;
}