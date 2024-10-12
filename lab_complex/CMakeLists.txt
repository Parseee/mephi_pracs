cmake_minimum_required(VERSION 3.5.0)
project(complex VERSION 0.1.0 LANGUAGES C)

add_executable(complex src/main.c src/error.c src/logger/logger.c)

