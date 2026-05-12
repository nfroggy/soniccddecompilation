#include "../../types.h"
#include "../common/hmx_types.h"
#include "bestsprt.h"
#include "../common/hmx_oeeactl.h"

static void DeleteCharSprt(Sint32 y, Sint32 colom);
static Uint32 CreateCharSprt(Sint32 colom, Sint32 y, char code);
static Uint32 CreateCharSprt2(Sint32 colom, Sint32 y, char code);

Sint32 menu_y[11] = {7, 31, 55, 79, 103, 127, 151, 175, 199, 0, 0};
Uint32 hSprMenu[11][28];
extern draw_context *s_ctx;
extern void (*hmx_sprite_set_flags_module)(hmx_sprite *, Sint32);
extern void (*hmx_sprite_set_bitmap_module)(hmx_sprite *, hmx_bitmap *);
extern void (*hmx_sprite_set_position_module)(hmx_sprite *, Sint32, Sint32);
extern Sint32 ScrollCount;
extern sprite_bmp infoSprtBmp[];

static void DeleteCharSprt(Sint32 y, Sint32 colom) {
    if (hSprMenu[y][colom] != 0) {
        hmx_sprite_set_bitmap_module(s_ctx->sprites[hSprMenu[y][colom]], 0);
        hmx_sprite_set_flags_module(s_ctx->sprites[hSprMenu[y][colom]], 0);
        s_ctx->spr_level[hSprMenu[y][colom]] = 0;
        sprDelete(hSprMenu[y][colom]);
        hSprMenu[y][colom] = 0;
    }
}

static Uint32 CreateCharSprt(Sint32 colom, Sint32 y, char code) {
    Sint32 indx;
    POINT point;

    if (hSprMenu[y][colom] != 0) {
        DeleteCharSprt(y, colom);
    }
    if (code == 32)
        indx = 89;
    else if (code == 44)
        indx = 90;
    else if (code == 46)
        indx = 91;
    else if (code == 45)
        indx = 72;
    else if (code < 65)
        indx = code - 48;
    else
        indx = code - 55;

    sprCreate((Sint32 *)&hSprMenu[y][colom]);

    s_ctx->spr_level[hSprMenu[y][colom]] = 60 - infoSprtBmp[indx].order;
    point.x = colom * 8 + 56;

    point.y = ScrollCount * 8 + menu_y[y];

    hmx_sprite_set_position_module(s_ctx->sprites[hSprMenu[y][colom]], point.x,
                                   point.y);

    hmx_sprite_set_bitmap_module(s_ctx->sprites[hSprMenu[y][colom]],
                                 s_ctx->spr_bitmaps[indx]);
    hmx_sprite_set_flags_module(s_ctx->sprites[hSprMenu[y][colom]], 0);

    return 1;
}

static Uint32 CreateCharSprt2(Sint32 colom, Sint32 y, char code) {
    Sint32 indx;
    POINT point;

    if (hSprMenu[y][colom] != 0) {
        DeleteCharSprt(y, colom);
    }
    if (code == 32)
        indx = 89;
    else if (code == 44)
        indx = 90;
    else if (code == 46)
        indx = 91;
    else if (code == 45)
        indx = 72;
    else if (code < 65)
        indx = code - 12;
    else
        indx = code - 19;

    sprCreate((Sint32 *)&hSprMenu[y][colom]);

    s_ctx->spr_level[hSprMenu[y][colom]] = 60 - infoSprtBmp[indx].order;
    point.x = colom * 8 + 56;

    point.y = ScrollCount * 8 + menu_y[y];

    hmx_sprite_set_position_module(s_ctx->sprites[hSprMenu[y][colom]], point.x,
                                   point.y);

    hmx_sprite_set_bitmap_module(s_ctx->sprites[hSprMenu[y][colom]],
                                 s_ctx->spr_bitmaps[indx]);
    hmx_sprite_set_flags_module(s_ctx->sprites[hSprMenu[y][colom]], 0);
    return 1;
}

void DeleteAllCharSprt(void) {
    Sint32 m, n;

    for (n = 0; n < 11; ++n)
        for (m = 0; m < 28; ++m) {
            DeleteCharSprt(n, m);
        }
}

void StrMenu(Sint32 y, char *lpStr) {
    Sint32 n = 0;

    while (*lpStr) {
        CreateCharSprt(n, y, *lpStr);
        ++n;
        ++lpStr;
    }
}

void StrMenu2(Sint32 y, char *lpStr) {
    Sint32 n = 0;

    while (*lpStr) {
        CreateCharSprt2(n, y, *lpStr);
        ++n;
        ++lpStr;
    }
}
