#include "../../types.h"
#ifndef HMX_TYPES_H
#define HMX_TYPES_H

typedef struct hmx_environment hmx_environment;
typedef struct hmx_sprite hmx_sprite;
typedef struct hmx_grid hmx_grid;
typedef struct hmx_bitmap hmx_bitmap;
typedef struct hmx_ddagrid hmx_ddagrid;
typedef struct hmx_renderer_base hmx_renderer_base;
typedef struct hmx_renderer_context hmx_renderer_context;
typedef struct hmx_surface hmx_surface;
typedef struct hmx_background hmx_background;

typedef struct draw_context {
  hmx_surface* screen;
  hmx_renderer_context* context;
  hmx_background* background;
  hmx_sprite** sprites;
  Sint32 spr_bitmap_count;
  hmx_bitmap* spr_bitmaps[1000];
  Sint32 spr_level[1000];
  hmx_grid* grids[3];
  Sint32 grd_bitmap_count;
  hmx_bitmap* grd_bitmaps[2048];
  Sint32 grd_change_count;
  hmx_bitmap* grd_changes[2048];
  hmx_grid* sp_grids[8];
  hmx_ddagrid* rot_grid;
  Sint32 rot_bitmap_count;
  hmx_bitmap* rot_bitmaps[512];
}
draw_context;

typedef struct {
  Uint16 totalNum;
  Uint16 w;
  Uint16 h;
  POINT point;
  Sint32 order;
  Uint8 palOff;
  Uint32** lphBmp;
  Uint32* lphTile;
  Uint32* lphGrid;
}
grid_bmp;

typedef struct {
  Uint16 w;
  Uint16 h;
  POINT point;
  Sint32 order;
  Uint8 palOff;
  Uint32* hBmp;
  Uint16 timing;
  Uint32* lphSpr;
}
sprite_bmp;

typedef struct _POINTL {
  Sint32 x;
  Sint32 y;
}
POINTL;

typedef struct _RECT {
  Sint32 left;
  Sint32 top;
  Sint32 right;
  Sint32 bottom;
}
RECT;

typedef struct _RECTL {
  Sint32 left;
  Sint32 top;
  Sint32 right;
  Sint32 bottom;
}
RECTL;

typedef struct {
  Sint32 x;
  Sint32 y;
  Sint32 dx;
  Sint32 dy;
}
grid_rect;

typedef struct {
  Uint16 mTile;
  Uint16 nTile;
}
tile_location;

#endif
