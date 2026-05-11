#include "../../types.h"
#include "../common/hmx_types.h"
#include "stgsprt.h"
#include "../common/hmx_oeeactl.h"

static void DeleteCharSprt(Sint32 colom);
static Uint32 CreateCharSprt(Sint32 colom, char code);

Uint32 hSprMenu[12];
extern draw_context* s_ctx;
extern void(*hmx_sprite_set_flags_module)(hmx_sprite*, Sint32);
extern void(*hmx_sprite_set_bitmap_module)(hmx_sprite*, hmx_bitmap*);
extern void(*hmx_sprite_set_position_module)(hmx_sprite*, Sint32, Sint32);
extern sprite_bmp infoSprtBmp[];

static void DeleteCharSprt(Sint32 colom) {
  if (hSprMenu[colom] != 0)
  {
    sprDelete(hSprMenu[colom]);
    s_ctx->spr_level[hSprMenu[colom]] = 0;
    hSprMenu[colom] = 0;
  }
}

static Uint32 CreateCharSprt(Sint32 colom, char code) {
  RECTL SprRect = { 0, 0, 0, 0 };
  Uint32 mask[4] = { 0, 0, 0, 0 };
  Sint32 indx;
  POINT point;

if (hSprMenu[colom] != 0)
  {
    DeleteCharSprt(colom);
  }
  if (code == 45) indx = 47;
  else if (code < 65) indx = code - 37;
  else indx = code - 44;

  sprCreate((Sint32*)&hSprMenu[colom]);

s_ctx->spr_level[hSprMenu[colom]] = infoSprtBmp[indx].order;
  point.x = colom * 8 + 120;
  point.y = 80;

  hmx_sprite_set_position_module(s_ctx->sprites[hSprMenu[colom]], point.x, point.y);

  hmx_sprite_set_bitmap_module(s_ctx->sprites[hSprMenu[colom]], s_ctx->spr_bitmaps[indx]);
  hmx_sprite_set_flags_module(s_ctx->sprites[hSprMenu[colom]], 0);
  return 1;
}

void DeleteAllCharSprt(void) {
  Sint32 m;

  for (m = 0; m < 12; ++m)
  {
    DeleteCharSprt(m);
  }
}

void StrMenu(char* lpStr) {
  Sint32 n = 0;

  while (*lpStr)
  {
    CreateCharSprt(n, *lpStr);
    ++n;
    ++lpStr;
  }
}
