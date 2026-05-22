#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include "../types.h"

void *sMemAlloc(Sint32 size);
void sMemFree(void *ptr);
void sMemSet(void *dst, Uint8 value, Sint32 size);
void sMemCpy(void *dst, void *src, Sint32 size);
Sint32 sMemCmp(void *lhs, void *rhs, Sint32 size);
Sint32 sRandom(void);
char *sStrcpy(char *dst, char *src);
Sint32 sStrncmp(char *lhs, char *rhs, Sint32 size);
void sPrintf(char *dst, const char *format, ...);
void sOutputDebugString(char *msg);

#ifdef __cplusplus
}
#endif
