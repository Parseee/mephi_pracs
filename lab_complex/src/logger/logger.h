#pragma once
#ifndef LOGGER_H
#define LOGGER_H

#include <assert.h>
#include <stdarg.h>

typedef struct {
    const char* file;
    const char* func;
    const int line;
} place_in_code_t;

typedef enum {
    LOGGER_OK,
    LOGGER_FAILURE
} logger_state;

typedef enum {
    LOGGER_ZERO_MODE = 0b0000u,
    LOGGER_INFO_MODE = 0b0001u,
    LOGGER_ERROR_MODE = 0b0010u,
    LOGGER_ALL_MODE = 0b1111u
} logging_mode;

void log_is_init(void);
logger_state log_create(void);
logger_state log_destruct(void);
logger_state log_set_level_details(logging_mode logging_level);
logger_state log_set_logout_file(const char* const filename);
logger_state log_write(const char* log_name_str, const char* format,
    place_in_code_t info, va_list* const args);
logger_state log_func(place_in_code_t info, const logging_mode logging_level, const char* const format,
    ...);
logger_state log_lassert(const char* const format, place_in_code_t info, va_list* const args);

#ifdef NDEBUG

#define lassert(check, format, ...) \
    do {                            \
    } while (0)

#else /* NDEBUG */

#define lassert(check, ...)                                           \
    do {                                                              \
        if (!(check)) {                                               \
            log_func((place_in_code_t){.func=__func__, .file=__FILE__, .line=__LINE__}, LOGGER_ERROR_MODE, \
                ##__VA_ARGS__);                                       \
            if (log_destruct())                                       \
                fprintf(stderr, "Can't destroy logger\n");            \
            assert(0);                                                \
        }                                                             \
    } while (0)

#endif /* NDEBUG */

#endif /* LOGGER_H */