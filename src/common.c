#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "common.h"


void logger(LogLevel level, const char* format, ...) {
    va_list args;
    va_start(args, format);
    switch(level) {
        default:
            vfprintf(stderr, format, args);
    }
    va_end(args);
    fputs("\n", stderr);

    exit(2);
}


void* siegeMalloc(size_t size) {
    return malloc(size);
}

void* siegeCalloc(size_t count, size_t size) {
    return calloc(count, size);
}

void* siegeRealloc(void* ptr, size_t newSize) {
    return realloc(ptr, newSize);
}

void  siegeFree(void* mem) {
    free(mem);
}
