#ifndef COMMON_H
#define COMMON_H

typedef enum LogLevel {
    INFO_LEVEL,
    DEBUG_LEVEL,
    ERROR_LEVEL,
    FATAL_LEVEL,
} LogLevel;

void logger(LogLevel level, const char* format, ...);

void* siegeMalloc(size_t size);
void* siegeCalloc(size_t count, size_t size);
void* siegeRealloc(void* ptr, size_t newSize);
void  siegeFree(void* mem);

#endif
