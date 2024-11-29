#include "logger.h"
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static struct {
    unsigned output_flags;
    const char* logout_name;
    FILE* logout;
    bool is_used;
} LOGGER = { .output_flags = LOGGER_INFO_MODE, .logout_name = NULL, .logout = NULL, .is_used = false };

void log_is_init(void)
{
    assert(LOGGER.logout_name && "Logout name is NULL");
    assert(LOGGER.logout && "Logout file is NULL");
}

logger_state log_create(void)
{
    assert(!LOGGER.logout_name || !LOGGER.logout);

    LOGGER.logout_name = ".log/logout.log";
    if (!(LOGGER.logout = fopen(LOGGER.logout_name, "a"))) {
        perror("Can't open file");
        return LOGGER_FAILURE;
    }

    LOGGER.output_flags = LOGGER_ERROR_MODE;

    return LOGGER_OK;
}

logger_state log_destruct(void)
{
    if (LOGGER.is_used) {
        fprintf(LOGGER.logout, "\n");
    }

    log_set_level_details(LOGGER_ZERO_MODE);

    LOGGER.logout_name = NULL;

    if (LOGGER.logout && fclose(LOGGER.logout)) {
        perror("Can't close the file");
        return LOGGER_FAILURE;
    }

    LOGGER.logout = NULL;

    return LOGGER_OK;
}

logger_state log_set_level_details(logging_mode logging_level)
{
    log_is_init();
    assert((logging_level <= LOGGER_ALL_MODE) && "Incorrect level details flag");

    LOGGER.output_flags = logging_level;

    return LOGGER_OK;
}

logger_state log_set_logout_file(const char* const filename)
{
    log_is_init();
    assert(filename);

    if (LOGGER.logout) {
        fprintf(LOGGER.logout, "\n");
    }

    LOGGER.logout_name = filename;

    if (LOGGER.logout && fclose(LOGGER.logout)) {
        perror("Can't close file");
        return LOGGER_FAILURE;
    }

    if (!(LOGGER.logout = fopen(filename, "ab"))) {
        perror("Can't open file");
        return LOGGER_FAILURE;
    }

    return LOGGER_OK;
}

logger_state log_func(place_in_code_t info, const logging_mode logging_level,
    const char* const format, ...)
{
    log_is_init();
    assert(info.file);
    assert(info.func);

    va_list args;
    va_start(args, format);

    if (LOGGER.output_flags & LOGGER_INFO_MODE & logging_level) {
        if (log_write("Log INFO", format, info, &args) == LOGGER_FAILURE) {
            fprintf(stderr, "log_info failure");
        }
    } else if (LOGGER.output_flags & LOGGER_ERROR_MODE & logging_level) {
        if (log_write("Log ERROR", format, info, &args) == LOGGER_FAILURE) {
            fprintf(stderr, "log_error failure");
        }

        va_start(args, format);

        if (log_lassert(format, info, &args) == LOGGER_FAILURE) {
            fprintf(stderr, "log_lassert error");
            return LOGGER_FAILURE;
        }
    }

    va_end(args);

    return LOGGER_OK;
}

#define MAX_TIME_STR_LEN_ 64
logger_state log_write(const char* log_name_str, const char* format,
    place_in_code_t info, va_list* const args)
{
    log_is_init();
    assert(log_name_str);
    assert(format);
    assert(args);
    assert(info.func);
    assert(info.file);

    LOGGER.is_used = true;

    const time_t current_time = time(NULL);
    const struct tm* const current_local_time = localtime(&current_time);
    char current_time_str[MAX_TIME_STR_LEN_] = {};
    if (strftime(current_time_str, MAX_TIME_STR_LEN_, "%Y %b %d %X", current_local_time) <= 0) {
        perror("strftime format error");
        return LOGGER_FAILURE;
    }

    if (fprintf(LOGGER.logout, "%-12sLOGGER.logout, %s. Func - %s() in %s:%d:  ",
            log_name_str, current_time_str, info.func, info.file, info.line)
        <= 0) {
        perror("fprintf error");
        return LOGGER_FAILURE;
    }

    if (vfprintf(LOGGER.logout, format, *args) < 0) {
        perror("vprintf error");
        return LOGGER_FAILURE;
    }

    fprintf(LOGGER.logout, "\n");

    return LOGGER_OK;
}
#undef MAX_TIME_STR_LEN_

logger_state log_lassert(const char* format, place_in_code_t info,
    va_list* const args)
{
    log_is_init();
    assert(format);
    assert(args);
    assert(info.func);
    assert(info.file);

    if (fprintf(stderr, "\nLASSERT ERROR. Func - %s() in %s:%d:  ",
            info.func, info.file, info.line)
        <= 0) {
        perror("fprintf error");
        return LOGGER_FAILURE;
    }

    if (vfprintf(stderr, format, *args) < 0) {
        perror("vprintf error");
        return LOGGER_FAILURE;
    }

    return LOGGER_OK;
}