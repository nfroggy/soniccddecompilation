#include <SDL3/SDL.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilities.h"

void *sMemAlloc(Sint32 size) {
    return calloc(1, (size_t)size);
}

void sMemFree(void *ptr) {
    free(ptr);
}

void sMemSet(void *dst, Uint8 value, Sint32 size) {
    memset(dst, value, (size_t)size);
}

void sMemCpy(void *dst, void *src, Sint32 size) {
    memcpy(dst, src, (size_t)size);
}

Sint32 sMemCmp(void *lhs, void *rhs, Sint32 size) {
    return memcmp(lhs, rhs, (size_t)size);
}

Sint32 sRandom(void) {
    return rand();
}

char *sStrcpy(char *dst, char *src) {
    return strcpy(dst, src);
}

char *sStrncpy(char *dst, char *src, Sint32 size) {
    return strncpy(dst, src, (size_t)size);
}

Sint32 sStrncmp(char *lhs, char *rhs, Sint32 size) {
    return strncmp(lhs, rhs, (size_t)size);
}

void sPrintf(char *dst, const char *format, ...) {
    va_list args;
    va_start(args, format);
    vsprintf(dst, format, args);
    va_end(args);
}

void sOutputDebugString(char *msg) {
    if (msg) {
        SDL_Log("%s", msg);
    }
}
