#pragma once
#ifndef EXEC_H
#define EXEC_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "../array/array.h"
#include "../logger/logger.h"
#include "../utils.h"

error_state exec(Array* array);

#endif /* EXEC_H */