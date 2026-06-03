#include "../../types.h"
#include "../common/hmx_types.h"
#include "sndsprt.h"
#include "../common/hmx_oeeactl.h"

static void DeleteCharSprt(Sint32 nMenuNum, Sint32 colom);
static Uint32 CreateCharSprt(Sint32 nMenuNum, Sint32 colom, char code,
                             Sint32 color);

static Sint32 nMenuPosiX[2] = {44, 208};
Uint32 hSprMenu[3][10];
extern void (*hmx_sprite_set_flags_module)(hmx_sprite *, Sint32);
extern draw_context *s_ctx;
extern void (*hmx_sprite_set_bitmap_module)(hmx_sprite *, hmx_bitmap *);
extern void (*hmx_sprite_set_position_module)(hmx_sprite *, Sint32, Sint32);
extern sprite_bmp infoSprtBmp[];
extern void (*sPrintf)(char *, const char *, ...);
Uint32 hMesMenu;

static void DeleteCharSprt(Sint32 nMenuNum, Sint32 colom) {
    if (hSprMenu[nMenuNum][colom] != 0) {
        sprDelete(hSprMenu[nMenuNum][colom]);
        s_ctx->spr_level[hSprMenu[nMenuNum][colom]] = 0;
        hmx_sprite_set_bitmap_module(s_ctx->sprites[hSprMenu[nMenuNum][colom]],
                                     0);
        hmx_sprite_set_flags_module(s_ctx->sprites[hSprMenu[nMenuNum][colom]],
                                    0);
        hSprMenu[nMenuNum][colom] = 0;
    }
}

static Uint32 CreateCharSprt(Sint32 nMenuNum, Sint32 colom, char code,
                             Sint32 color) {
    RECTL SprRect = {0, 0, 0, 0};
    Uint32 mask[4] = {0, 0, 0, 0};
    Sint32 indx;
    POINT point;

    if (hSprMenu[nMenuNum][colom] != 0) {
        DeleteCharSprt(nMenuNum, colom);
    }
    if (color == 1) {
        if (code == 46)
            indx = 82;
        else if (code < 65)
            indx = code - 3;
        else
            indx = code - 10;
    } else {
        if (code == 46)
            indx = 81;
        else if (code < 65)
            indx = code - 39;
        else
            indx = code - 46;
    }

    sprCreate(&hSprMenu[nMenuNum][colom]);

    s_ctx->spr_level[hSprMenu[nMenuNum][colom]] = infoSprtBmp[indx].order;
    point.x = nMenuPosiX[nMenuNum] + colom * 8;
    point.y = 80;
    hmx_sprite_set_position_module(s_ctx->sprites[hSprMenu[nMenuNum][colom]],
                                   point.x, point.y);

    hmx_sprite_set_bitmap_module(s_ctx->sprites[hSprMenu[nMenuNum][colom]],
                                 s_ctx->spr_bitmaps[indx]);
    hmx_sprite_set_flags_module(s_ctx->sprites[hSprMenu[nMenuNum][colom]], 0);
    return 1;
}

void DeleteAllCharSprt(void) {
    Sint32 n, m;

    for (n = 0; n < 3; ++n) {
        for (m = 0; m < 10; ++m) {
            DeleteCharSprt(n, m);
        }
    }
}

void StrPCM(Sint32 color) {
    CreateCharSprt(0, 0, 80, color);
    CreateCharSprt(0, 1, 67, color);
    CreateCharSprt(0, 2, 77, color);
    CreateCharSprt(0, 4, 78, color);
    CreateCharSprt(0, 5, 79, color);
    CreateCharSprt(0, 6, 46, color);
}

void StrDA(Sint32 color) {
    CreateCharSprt(1, 0, 68, color);
    CreateCharSprt(1, 1, 65, color);
    CreateCharSprt(1, 3, 78, color);
    CreateCharSprt(1, 4, 79, color);
    CreateCharSprt(1, 5, 46, color);
}

void StrPCMNO(Sint32 number) {
    char buf[12];

    sPrintf(buf, "%02d", number);
    CreateCharSprt(0, 7, buf[0], 1);
    CreateCharSprt(0, 8, buf[1], 1);
}

void StrDANO(Sint32 number) {
    char buf[12];

    sPrintf(buf, "%02d", number);
    CreateCharSprt(1, 6, buf[0], 1);
    CreateCharSprt(1, 7, buf[1], 1);
}

void StrWelcome(void) {
    Sint32 indx;
    POINT point;

    indx = 99;

    sprCreate(&hMesMenu);

    s_ctx->spr_level[hMesMenu] = infoSprtBmp[indx].order;
    point.x = nMenuPosiX[0] - 8;
    point.y = 160;

    hmx_sprite_set_position_module(s_ctx->sprites[hMesMenu], point.x, point.y);

    hmx_sprite_set_bitmap_module(s_ctx->sprites[hMesMenu],
                                 s_ctx->spr_bitmaps[indx]);
    hmx_sprite_set_flags_module(s_ctx->sprites[hMesMenu], 0);
}

void DeleteCharMes(void) {
    if (hMesMenu != 0) {
        sprDelete(hMesMenu);
        hmx_sprite_set_bitmap_module(s_ctx->sprites[hMesMenu], 0);
        hmx_sprite_set_flags_module(s_ctx->sprites[hMesMenu], 0);
        s_ctx->spr_level[hMesMenu] = 0;
        hMesMenu = 0;
    }
}
