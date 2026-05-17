#pragma once
#include <stdio.h>
#include "../types.h"

Sint32 sOpenFile(char *path);
Sint32 sReadFile(Sint32 file, void *dst, Sint32 size);
void sCloseFile(Sint32 file);
Sint32 sGetFileSize(Sint32 file);

FILE *File_Open(const char *path, const char *mode);
