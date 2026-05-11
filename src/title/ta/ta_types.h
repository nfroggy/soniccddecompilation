#include "../../types.h"
#ifndef TA_TYPES_H
#define TA_TYPES_H
typedef struct {
  Uint8 grid;
  Uint8 x;
  Uint8 y;
  Uint8 xs;
  Uint8 ys;
  Uint16 ofs;
  Uint8 transp;
  char* fn;
}
map_info;

typedef struct {
  map_info* info;
  Sint32 cnt;
}
map_infos;

typedef struct {
  Uint32 dwTyp;
  Uint32 dwSiz;
  Uint32 dwCnt;
  Uint32 dwOff;
}
bmp_header;

#endif
