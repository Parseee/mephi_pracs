#define report_error(format, error)                                     \
    do {                                                                \
        fprintf(stderr, #format " in file: %s, func %s, on line: %d\n", \
            __FILE__, __func__, __LINE__);                               \
        return error;                                                   \
    } while (0)