#include "../../types.h"
#include "../common/hmx_types.h"
#include "vmdsprt.h"
#include "../common/hmx_oeeactl.h"

static Sint32 nMenuPosiY[5] = { 112, 128, 144, 160, 176 };
Uint16 NUM_BMP __attribute__((aligned(16))) = 68;
Uint16 NUM_GRIDBMP = 1;
extern draw_context* s_ctx;
Uint32 hSprMenu[5][11];
extern sprite_bmp infoSprtBmp[];
extern void(*hmx_sprite_set_flags_module)(hmx_sprite*, Sint32);
extern void(*hmx_sprite_set_bitmap_module)(hmx_sprite*, hmx_bitmap*);
extern void(*hmx_sprite_set_position_module)(hmx_sprite*, Sint32, Sint32);

Uint32 CreateCharSprt(Sint32 nMenuNum, Sint32 colom, Sint32 code) {
  POINT point;

if (hSprMenu[nMenuNum][colom] != 0)
    return 0;

  sprCreate((Sint32*)&hSprMenu[nMenuNum][colom]);

point.x = colom * 8 + 192;
  point.y = nMenuPosiY[nMenuNum];

hmx_sprite_set_position_module(s_ctx->sprites[hSprMenu[nMenuNum][colom]], point.x, point.y);
  hmx_sprite_set_bitmap_module(s_ctx->sprites[hSprMenu[nMenuNum][colom]], s_ctx->spr_bitmaps[code]);
  hmx_sprite_set_flags_module(s_ctx->sprites[hSprMenu[nMenuNum][colom]], 0);
  s_ctx->spr_level[hSprMenu[nMenuNum][colom]] = 60 - infoSprtBmp[code].order;
  return 1;
}

void DeleteCharSprt(Sint32 nMenuNum, Sint32 colom) {

  if (hSprMenu[nMenuNum][colom] != 0)
  {

    hmx_sprite_set_bitmap_module(s_ctx->sprites[hSprMenu[nMenuNum][colom]], 0);
    hmx_sprite_set_flags_module(s_ctx->sprites[hSprMenu[nMenuNum][colom]], 0);
    s_ctx->spr_level[hSprMenu[nMenuNum][colom]] = 0;
    sprDelete(hSprMenu[nMenuNum][colom]);
    hSprMenu[nMenuNum][colom] = 0;
  }
}

void DeleteAllCharSprt(void) {
  Sint32 n, m;

  for (n = 0; n < 5; ++n)
  {
    for (m = 0; m < 11; ++m)
    {
      DeleteCharSprt(n, m);
    }
  }
}

void StrEXIT(Sint32 kind) {
  if (kind == 0)
  {
    DeleteCharSprt(0, 0); CreateCharSprt(0, 0, 4); DeleteCharSprt(0, 1); CreateCharSprt(0, 1, 23); DeleteCharSprt(0, 2); CreateCharSprt(0, 2, 8); DeleteCharSprt(0, 3); CreateCharSprt(0, 3, 19);
  }
  else
  {
    DeleteCharSprt(0, 0); CreateCharSprt(0, 0, 30); DeleteCharSprt(0, 1); CreateCharSprt(0, 1, 49); DeleteCharSprt(0, 2); CreateCharSprt(0, 2, 34); DeleteCharSprt(0, 3); CreateCharSprt(0, 3, 45);
  }
}

void StrOPENING(Sint32 kind) {
  if (kind == 0)
  {
    DeleteCharSprt(1, 0); CreateCharSprt(1, 0, 14); DeleteCharSprt(1, 1); CreateCharSprt(1, 1, 15); DeleteCharSprt(1, 2); CreateCharSprt(1, 2, 4); DeleteCharSprt(1, 3); CreateCharSprt(1, 3, 13);
    DeleteCharSprt(1, 4); CreateCharSprt(1, 4, 8); DeleteCharSprt(1, 5); CreateCharSprt(1, 5, 13); DeleteCharSprt(1, 6); CreateCharSprt(1, 6, 6);
  }
  else
  {
    DeleteCharSprt(1, 0); CreateCharSprt(1, 0, 40); DeleteCharSprt(1, 1); CreateCharSprt(1, 1, 41); DeleteCharSprt(1, 2); CreateCharSprt(1, 2, 30); DeleteCharSprt(1, 3); CreateCharSprt(1, 3, 39);
    DeleteCharSprt(1, 4); CreateCharSprt(1, 4, 34); DeleteCharSprt(1, 5); CreateCharSprt(1, 5, 39); DeleteCharSprt(1, 6); CreateCharSprt(1, 6, 32);
  }
}

void StrGOODENDING(Sint32 kind) {
  if (kind == 0)
  {
    DeleteCharSprt(2, 0); CreateCharSprt(2, 0, 6); DeleteCharSprt(2, 1); CreateCharSprt(2, 1, 14); DeleteCharSprt(2, 2); CreateCharSprt(2, 2, 14); DeleteCharSprt(2, 3); CreateCharSprt(2, 3, 3);
    DeleteCharSprt(2, 5); CreateCharSprt(2, 5, 4); DeleteCharSprt(2, 6); CreateCharSprt(2, 6, 13); DeleteCharSprt(2, 7); CreateCharSprt(2, 7, 3);
    DeleteCharSprt(2, 8); CreateCharSprt(2, 8, 8); DeleteCharSprt(2, 9); CreateCharSprt(2, 9, 13); DeleteCharSprt(2, 10); CreateCharSprt(2, 10, 6);
  }
  else
  {
    DeleteCharSprt(2, 0); CreateCharSprt(2, 0, 32); DeleteCharSprt(2, 1); CreateCharSprt(2, 1, 40); DeleteCharSprt(2, 2); CreateCharSprt(2, 2, 40); DeleteCharSprt(2, 3); CreateCharSprt(2, 3, 29);
    DeleteCharSprt(2, 5); CreateCharSprt(2, 5, 30); DeleteCharSprt(2, 6); CreateCharSprt(2, 6, 39); DeleteCharSprt(2, 7); CreateCharSprt(2, 7, 29);
    DeleteCharSprt(2, 8); CreateCharSprt(2, 8, 34); DeleteCharSprt(2, 9); CreateCharSprt(2, 9, 39); DeleteCharSprt(2, 10); CreateCharSprt(2, 10, 32);
  }
}

void StrBADENDING(Sint32 kind) {
  if (kind == 0)
  {
    DeleteCharSprt(3, 0); CreateCharSprt(3, 0, 1); DeleteCharSprt(3, 1); CreateCharSprt(3, 1, 0); DeleteCharSprt(3, 2); CreateCharSprt(3, 2, 3);
    DeleteCharSprt(3, 4); CreateCharSprt(3, 4, 4); DeleteCharSprt(3, 5); CreateCharSprt(3, 5, 13); DeleteCharSprt(3, 6); CreateCharSprt(3, 6, 3); DeleteCharSprt(3, 7); CreateCharSprt(3, 7, 8);
    DeleteCharSprt(3, 8); CreateCharSprt(3, 8, 13); DeleteCharSprt(3, 9); CreateCharSprt(3, 9, 6);
  }
  else
  {
    DeleteCharSprt(3, 0); CreateCharSprt(3, 0, 27); DeleteCharSprt(3, 1); CreateCharSprt(3, 1, 26); DeleteCharSprt(3, 2); CreateCharSprt(3, 2, 29);
    DeleteCharSprt(3, 4); CreateCharSprt(3, 4, 30); DeleteCharSprt(3, 5); CreateCharSprt(3, 5, 39); DeleteCharSprt(3, 6); CreateCharSprt(3, 6, 29); DeleteCharSprt(3, 7); CreateCharSprt(3, 7, 34);
    DeleteCharSprt(3, 8); CreateCharSprt(3, 8, 39); DeleteCharSprt(3, 9); CreateCharSprt(3, 9, 32);
  }
}

void StrPENCILTEST(Sint32 kind) {
  if (kind == 0)
  {
    DeleteCharSprt(4, 0); CreateCharSprt(4, 0, 15); DeleteCharSprt(4, 1); CreateCharSprt(4, 1, 4); DeleteCharSprt(4, 2); CreateCharSprt(4, 2, 13); DeleteCharSprt(4, 3); CreateCharSprt(4, 3, 2);
    DeleteCharSprt(4, 4); CreateCharSprt(4, 4, 8); DeleteCharSprt(4, 5); CreateCharSprt(4, 5, 11); DeleteCharSprt(4, 7); CreateCharSprt(4, 7, 19);
    DeleteCharSprt(4, 8); CreateCharSprt(4, 8, 4); DeleteCharSprt(4, 9); CreateCharSprt(4, 9, 18); DeleteCharSprt(4, 10); CreateCharSprt(4, 10, 19);
  }
  else
  {
    DeleteCharSprt(4, 0); CreateCharSprt(4, 0, 41); DeleteCharSprt(4, 1); CreateCharSprt(4, 1, 30); DeleteCharSprt(4, 2); CreateCharSprt(4, 2, 39); DeleteCharSprt(4, 3); CreateCharSprt(4, 3, 28);
    DeleteCharSprt(4, 4); CreateCharSprt(4, 4, 34); DeleteCharSprt(4, 5); CreateCharSprt(4, 5, 37); DeleteCharSprt(4, 7); CreateCharSprt(4, 7, 45);
    DeleteCharSprt(4, 8); CreateCharSprt(4, 8, 30); DeleteCharSprt(4, 9); CreateCharSprt(4, 9, 44); DeleteCharSprt(4, 10); CreateCharSprt(4, 10, 45);
  }
}
