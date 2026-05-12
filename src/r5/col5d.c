#include "../equ.h"
#include "col5a.h"

typedef struct {
    Uint8 adr;
    Uint8 tate;
    Uint8 yoko;
    Uint8 time;
    PALETTEENTRY colortbl[64];
} color_change_data;

static void colchgsub(Uint8 **ppChgTime, Uint8 **ppChgCnt,
                      color_change_data *pColortbl);
static void fadein1(void);
static void fadein2(PALETTEENTRY *lpPeDest, PALETTEENTRY *lpPeSrc);
static Sint32 fadeout_new(void);
static void fadeout1(void);
static void fadeout2(PALETTEENTRY *lpPeDest);
static Sint32 flashin_new(void);
static void flashin1(void);
static void flashin2(PALETTEENTRY *lpPeDest, PALETTEENTRY *lpPeSrc);
static Sint32 flashout_new(void);
static void flashout1(void);
static void flashout2(PALETTEENTRY *lpPeDest);
static void colorset00(Sint32 ColorNo, Sint32 WorkOffs);

PALETTEENTRY advacolor[64] = {
    {0, 32, 160, 1},    {0, 0, 96, 1},      {0, 0, 192, 1},
    {64, 64, 224, 1},   {96, 96, 224, 1},   {128, 128, 224, 1},
    {224, 224, 224, 1}, {224, 160, 0, 1},   {160, 96, 0, 1},
    {96, 32, 0, 1},     {224, 224, 0, 1},   {160, 160, 224, 1},
    {192, 0, 0, 1},     {96, 0, 0, 1},      {32, 0, 0, 1},
    {0, 0, 0, 1},       {0, 0, 0, 1},       {0, 0, 192, 1},
    {32, 32, 224, 1},   {64, 64, 224, 1},   {96, 96, 224, 1},
    {128, 128, 224, 1}, {224, 224, 224, 1}, {160, 160, 160, 1},
    {128, 128, 128, 1}, {96, 96, 96, 1},    {64, 64, 64, 1},
    {128, 64, 32, 1},   {224, 160, 128, 1}, {192, 128, 96, 1},
    {0, 0, 0, 1},       {224, 0, 0, 1},     {0, 0, 128, 1},
    {32, 0, 0, 1},      {224, 224, 224, 1}, {96, 32, 0, 1},
    {128, 64, 0, 1},    {192, 96, 0, 1},    {224, 128, 0, 1},
    {224, 192, 0, 1},   {32, 64, 192, 1},   {96, 128, 224, 1},
    {160, 192, 224, 1}, {192, 224, 224, 1}, {0, 64, 0, 1},
    {0, 96, 0, 1},      {64, 160, 0, 1},    {128, 224, 0, 1},
    {32, 128, 192, 1},  {32, 0, 160, 1},    {32, 64, 192, 1},
    {96, 128, 224, 1},  {160, 192, 224, 1}, {192, 224, 224, 1},
    {224, 224, 224, 1}, {192, 160, 224, 1}, {160, 128, 224, 1},
    {128, 96, 224, 1},  {128, 224, 0, 1},   {64, 160, 0, 1},
    {32, 0, 0, 1},      {96, 32, 0, 1},     {192, 96, 0, 1},
    {224, 192, 0, 1}};
PALETTEENTRY advacolor2[64] = {
    {0, 0, 0, 1},    {0, 0, 0, 1},       {32, 0, 0, 1},      {32, 0, 0, 1},
    {64, 32, 32, 1}, {64, 32, 32, 1},    {96, 64, 64, 1},    {96, 64, 64, 1},
    {64, 32, 32, 1}, {64, 32, 32, 1},    {96, 64, 64, 1},    {128, 96, 96, 1},
    {64, 32, 32, 1}, {32, 0, 0, 1},      {0, 0, 0, 1},       {0, 0, 0, 1},
    {0, 0, 0, 1},    {0, 0, 0, 1},       {32, 0, 0, 1},      {64, 32, 32, 1},
    {64, 32, 32, 1}, {96, 64, 64, 1},    {128, 96, 96, 1},   {64, 32, 32, 1},
    {96, 64, 64, 1}, {64, 32, 32, 1},    {32, 0, 0, 1},      {64, 32, 32, 1},
    {96, 64, 64, 1}, {64, 32, 32, 1},    {0, 0, 0, 1},       {64, 32, 32, 1},
    {0, 0, 0, 1},    {224, 224, 0, 1},   {0, 0, 0, 1},       {0, 0, 0, 1},
    {0, 0, 0, 1},    {0, 0, 0, 1},       {224, 224, 0, 1},   {0, 0, 0, 1},
    {0, 0, 0, 1},    {0, 0, 0, 1},       {0, 0, 0, 1},       {0, 0, 0, 1},
    {0, 0, 0, 1},    {0, 0, 0, 1},       {0, 0, 0, 1},       {0, 0, 0, 1},
    {0, 0, 0, 1},    {192, 224, 224, 1}, {0, 0, 0, 1},       {0, 0, 0, 1},
    {0, 0, 0, 1},    {0, 0, 0, 1},       {192, 224, 224, 1}, {0, 0, 0, 1},
    {0, 0, 0, 1},    {0, 0, 0, 1},       {0, 0, 0, 1},       {0, 0, 0, 1},
    {0, 0, 0, 1},    {0, 0, 0, 1},       {0, 0, 0, 1},       {0, 0, 0, 1}};
PALETTEENTRY gamecolor[16] = {
    {0, 0, 0, 1},       {0, 0, 0, 1},       {32, 32, 128, 1},
    {64, 64, 160, 1},   {96, 96, 192, 1},   {128, 128, 224, 1},
    {224, 224, 224, 1}, {160, 160, 160, 1}, {128, 128, 128, 1},
    {64, 64, 64, 1},    {224, 160, 128, 1}, {160, 96, 64, 1},
    {224, 0, 0, 1},     {128, 0, 0, 1},     {64, 0, 0, 1},
    {224, 224, 0, 1}};
PALETTEENTRY zone5color0[48] = {
    {0, 0, 0, 1},       {0, 0, 0, 1},       {32, 32, 192, 1},
    {64, 64, 224, 1},   {128, 128, 224, 1}, {160, 0, 96, 1},
    {224, 224, 224, 1}, {160, 160, 160, 1}, {128, 128, 128, 1},
    {64, 64, 64, 1},    {224, 64, 128, 1},  {224, 128, 160, 1},
    {224, 224, 0, 1},   {128, 128, 0, 1},   {64, 64, 0, 1},
    {224, 32, 0, 1},    {0, 0, 0, 1},       {96, 160, 0, 1},
    {64, 96, 0, 1},     {32, 32, 0, 1},     {32, 0, 0, 1},
    {192, 192, 0, 1},   {192, 224, 32, 1},  {128, 128, 0, 1},
    {64, 64, 0, 1},     {128, 192, 32, 1},  {224, 224, 192, 1},
    {96, 96, 64, 1},    {0, 0, 0, 1},       {32, 0, 0, 1},
    {160, 32, 32, 1},   {0, 0, 0, 1},       {0, 0, 0, 1},
    {0, 0, 32, 1},      {0, 32, 32, 1},     {0, 64, 32, 1},
    {32, 96, 64, 1},    {32, 128, 96, 1},   {32, 160, 160, 1},
    {224, 224, 224, 1}, {224, 0, 224, 1},   {96, 0, 96, 1},
    {32, 0, 32, 1},     {0, 128, 0, 1},     {32, 0, 0, 1},
    {128, 0, 0, 1},     {224, 64, 0, 1},    {224, 160, 0, 1}};
PALETTEENTRY zone5color1[48] = {
    {0, 0, 0, 1},       {0, 0, 0, 1},       {32, 32, 192, 1},
    {64, 64, 224, 1},   {128, 128, 224, 1}, {160, 0, 96, 1},
    {224, 224, 224, 1}, {160, 160, 160, 1}, {128, 128, 128, 1},
    {64, 64, 64, 1},    {224, 64, 128, 1},  {224, 128, 160, 1},
    {224, 224, 0, 1},   {128, 128, 0, 1},   {64, 64, 0, 1},
    {224, 32, 0, 1},    {0, 0, 0, 1},       {96, 160, 0, 1},
    {64, 96, 0, 1},     {32, 32, 0, 1},     {32, 0, 0, 1},
    {192, 192, 0, 1},   {192, 224, 32, 1},  {128, 128, 0, 1},
    {64, 64, 0, 1},     {128, 192, 32, 1},  {224, 224, 192, 1},
    {96, 96, 64, 1},    {0, 0, 0, 1},       {32, 0, 0, 1},
    {160, 32, 32, 1},   {0, 0, 0, 1},       {0, 0, 0, 1},
    {32, 0, 0, 1},      {64, 32, 0, 1},     {96, 64, 0, 1},
    {128, 96, 0, 1},    {160, 160, 32, 1},  {128, 32, 64, 1},
    {160, 32, 64, 1},   {192, 64, 64, 1},   {192, 96, 64, 1},
    {240, 160, 96, 1},  {224, 224, 224, 1}, {32, 32, 0, 1},
    {64, 64, 0, 1},     {128, 128, 0, 1},   {224, 224, 0, 1}};
PALETTEENTRY zone5colboss0[16] = {
    {0, 0, 0, 1},     {0, 0, 0, 1},     {0, 160, 160, 1},   {0, 32, 32, 1},
    {0, 64, 64, 1},   {0, 96, 96, 1},   {224, 224, 224, 1}, {0, 224, 224, 1},
    {64, 32, 32, 1},  {128, 32, 32, 1}, {224, 96, 96, 1},   {32, 0, 32, 1},
    {224, 224, 0, 1}, {128, 128, 0, 1}, {64, 64, 0, 1},     {224, 0, 0, 1}};
palette_part colortbl[7] = {{advacolor, 0, 64},     {advacolor, 0, 64},
                            {advacolor2, 0, 64},    {gamecolor, 0, 16},
                            {zone5color0, 16, 48},  {zone5color1, 16, 48},
                            {zone5colboss0, 16, 16}};
static Sint32 FadeCount;

void clchgctr(void) {
    Uint8 *pChgTime, *pChgCnt;
    color_change_data colchg50 = {42,
                                  3,
                                  3,
                                  4,
                                  {{224, 224, 192, 1},
                                   {96, 96, 64, 1},
                                   {0, 0, 0, 1},
                                   {0, 0, 0, 1},
                                   {224, 224, 192, 1},
                                   {96, 96, 64, 1},
                                   {96, 96, 64, 1},
                                   {0, 0, 0, 1},
                                   {224, 224, 192, 1}}};
    color_change_data colchg51 = {54,
                                  3,
                                  4,
                                  12,
                                  {{128, 32, 64, 1},
                                   {160, 32, 64, 1},
                                   {192, 64, 64, 1},
                                   {192, 96, 64, 1},
                                   {160, 32, 64, 1},
                                   {192, 32, 64, 1},
                                   {224, 64, 64, 1},
                                   {224, 96, 64, 1},
                                   {224, 64, 64, 1},
                                   {224, 32, 64, 1},
                                   {224, 64, 64, 1},
                                   {224, 96, 64, 1}}};
    color_change_data colchg52 = {60,
                                  4,
                                  4,
                                  5,
                                  {{224, 224, 0, 1},
                                   {128, 128, 0, 1},
                                   {64, 64, 0, 1},
                                   {32, 32, 0, 1},
                                   {128, 128, 0, 1},
                                   {64, 64, 0, 1},
                                   {32, 32, 0, 1},
                                   {224, 224, 0, 1},
                                   {64, 64, 0, 1},
                                   {32, 32, 0, 1},
                                   {224, 224, 0, 1},
                                   {128, 128, 0, 1},
                                   {32, 32, 0, 1},
                                   {224, 224, 0, 1},
                                   {128, 128, 0, 1},
                                   {64, 64, 0, 1}}};
    color_change_data colchg53 = {42,
                                  3,
                                  3,
                                  4,
                                  {{224, 0, 160, 1},
                                   {160, 0, 128, 1},
                                   {64, 0, 96, 1},
                                   {160, 0, 128, 1},
                                   {64, 0, 96, 1},
                                   {224, 0, 160, 1},
                                   {64, 0, 96, 1},
                                   {224, 0, 160, 1},
                                   {160, 0, 128, 1}}};

    pChgTime = clchgtim;
    pChgCnt = clchgcnt;

    if (bossflag) {

        ++pChgTime;
        ++pChgCnt;
    } else {

        if (!colrevflag) {
            colchgsub(&pChgTime, &pChgCnt, &colchg50);
        } else {
            colchgsub(&pChgTime, &pChgCnt, &colchg53);
        }
    }

    if (enkeino & 1) {
        colchgsub(&pChgTime, &pChgCnt, &colchg51);
        colchgsub(&pChgTime, &pChgCnt, &colchg52);
    }
}

static void colchgsub(Uint8 **ppChgTime, Uint8 **ppChgCnt,
                      color_change_data *pColortbl) {
    Sint16 cnt;
    PALETTEENTRY *pPalet, *pColtbl;
    Sint32 i;

    if ((char)--**ppChgTime < 0) {
        **ppChgTime = pColortbl->time;
        if ((cnt = **ppChgCnt + 1) >= pColortbl->tate) {
            cnt = 0;
        }

        **ppChgCnt = cnt;

        cnt *= pColortbl->yoko;
        pPalet = &lpcolorwk[pColortbl->adr];
        pColtbl = &pColortbl->colortbl[cnt];

        for (i = 0; i < pColortbl->yoko; ++i) {
            *pPalet++ = *pColtbl++;
        }
    }

    ++*ppChgTime;
    ++*ppChgCnt;
}

void fadein0(void) {
    PALETTEENTRY *lpPe;
    Sint32 i;

    lpPe = &lpcolorwk[startcolor / 2];

    for (i = 0; colorcnt >= i; ++i) {
        lpPe->peRed = 0;
        lpPe->peGreen = 0;
        lpPe->peBlue = 0;
        lpPe->peFlags = 1;
        ++lpPe;
    }
    FadeCount = 0;
    *lpFadeFlag = 1;
}

Sint32 fadein0_new(void) {

    int_flg = 18;

    fadein1();

    if (++FadeCount > 21) {
        FadeCount = 0;
        return 1;
    }
    return 0;
}

static void fadein1(void) {
    PALETTEENTRY *lpPe, *lpPe2;
    Sint32 i;

    lpPe = &lpcolorwk[startcolor / 2];
    lpPe2 = &lpcolorwk2[startcolor / 2];

    for (i = 0; colorcnt >= i; ++i) {
        fadein2(lpPe, lpPe2);
        ++lpPe;
        ++lpPe2;
    }
    if (stageno.b.h == 1)
        return;

    lpPe = &lpcolorwk3[startcolor / 2];
    lpPe2 = &lpcolorwk4[startcolor / 2];

    for (i = 0; colorcnt >= i; ++i) {
        fadein2(lpPe, lpPe2);
        ++lpPe;
        ++lpPe2;
    }
}

static void fadein2(PALETTEENTRY *lpPeDest, PALETTEENTRY *lpPeSrc) {
    if (lpPeSrc->peBlue != lpPeDest->peBlue ||
        lpPeSrc->peGreen != lpPeDest->peGreen ||
        lpPeSrc->peRed != lpPeDest->peRed) {
        if (lpPeSrc->peBlue > lpPeDest->peBlue) {
            lpPeDest->peBlue += 32;
        } else if (lpPeSrc->peGreen > lpPeDest->peGreen) {
            lpPeDest->peGreen += 32;
        } else if (lpPeSrc->peRed > lpPeDest->peRed) {
            lpPeDest->peRed += 32;
        }
    }
}

void fadeout(void) {
    startcolor = 0;
    colorcnt = 63;

    FadeCount = 0;
    *lpFadeFlag = 2;
}

static Sint32 fadeout_new(void) {

    int_flg = 18;

    fadeout1();

    if (++FadeCount > 21) {
        FadeCount = 0;
        return 1;
    }
    return 0;
}

static void fadeout1(void) {
    PALETTEENTRY *lpPe;
    Sint32 i;

    lpPe = &lpcolorwk[startcolor / 2];

    for (i = 0; colorcnt >= i; ++i) {
        fadeout2(lpPe);
        ++lpPe;
    }
    lpPe = &lpcolorwk3[startcolor / 2];

    for (i = 0; colorcnt >= i; ++i) {
        fadeout2(lpPe);
        ++lpPe;
    }
}

static void fadeout2(PALETTEENTRY *lpPeDest) {
    if (lpPeDest->peRed) {
        lpPeDest->peRed -= 32;
    } else if (lpPeDest->peGreen) {
        lpPeDest->peGreen -= 32;
    } else if (lpPeDest->peBlue) {
        lpPeDest->peBlue -= 32;
    }
}

void flashin(void) {
    PALETTEENTRY *lpPe;
    Sint32 i;

    startcolor = 0;
    colorcnt = 63;

    lpPe = &lpcolorwk[startcolor / 2];

    for (i = 0; colorcnt >= i; ++i) {
        lpPe->peRed = 224;
        lpPe->peGreen = 224;
        lpPe->peBlue = 224;
        lpPe->peFlags = 1;
        ++lpPe;
    }
    FadeCount = 0;
    *lpFadeFlag = 3;
}

static Sint32 flashin_new(void) {

    int_flg = 18;

    flashin1();

    if (++FadeCount > 21) {
        FadeCount = 0;
        return 1;
    }
    return 0;
}

static void flashin1(void) {
    PALETTEENTRY *lpPe, *lpPe2;
    Sint32 i;

    lpPe = &lpcolorwk[startcolor / 2];
    lpPe2 = &lpcolorwk2[startcolor / 2];

    for (i = 0; colorcnt >= i; ++i) {
        flashin2(lpPe, lpPe2);
        ++lpPe;
        ++lpPe2;
    }
    if (stageno.b.h == 1)
        return;

    lpPe = &lpcolorwk3[startcolor / 2];
    lpPe2 = &lpcolorwk4[startcolor / 2];

    for (i = 0; colorcnt >= i; ++i) {
        flashin2(lpPe, lpPe2);
        ++lpPe;
        ++lpPe2;
    }
}

static void flashin2(PALETTEENTRY *lpPeDest, PALETTEENTRY *lpPeSrc) {
    if (lpPeSrc->peBlue != lpPeDest->peBlue ||
        lpPeSrc->peGreen != lpPeDest->peGreen ||
        lpPeSrc->peRed != lpPeDest->peRed) {
        if (lpPeSrc->peBlue < lpPeDest->peBlue) {
            lpPeDest->peBlue -= 32;
        } else if (lpPeSrc->peGreen < lpPeDest->peGreen) {
            lpPeDest->peGreen -= 32;
        } else if (lpPeSrc->peRed < lpPeDest->peRed) {
            lpPeDest->peRed -= 32;
        }
    }
}

void flashout(void) {
    startcolor = 0;
    colorcnt = 63;

    FadeCount = 0;
    *lpFadeFlag = 4;
}

static Sint32 flashout_new(void) {

    int_flg = 18;

    flashout1();

    if (++FadeCount > 21) {
        FadeCount = 0;
        return 1;
    }
    return 0;
}

static void flashout1(void) {
    PALETTEENTRY *lpPe;
    Sint32 i;

    lpPe = &lpcolorwk[startcolor / 2];

    for (i = 0; colorcnt >= i; ++i) {
        flashout2(lpPe);
        ++lpPe;
    }
    lpPe = &lpcolorwk3[startcolor / 2];

    for (i = 0; colorcnt >= i; ++i) {
        flashout2(lpPe);
        ++lpPe;
    }
}

static void flashout2(PALETTEENTRY *lpPeDest) {
    if (lpPeDest->peRed != 224) {
        lpPeDest->peRed += 32;
    } else if (lpPeDest->peGreen != 224) {
        lpPeDest->peGreen += 32;
    } else if (lpPeDest->peBlue != 224) {
        lpPeDest->peBlue += 32;
    }
}

void colorset(Sint32 ColorNo) { colorset00(ColorNo, 1); }

void colorset2(Sint32 ColorNo) { colorset00(ColorNo, 0); }

void colorset3(Sint32 ColorNo) { colorset00(ColorNo, 1); }

void colorset4(Sint32 ColorNo) { colorset00(ColorNo, 2); }

static void colorset00(Sint32 ColorNo, Sint32 WorkOffs) {
    PALETTEENTRY *lppe;
    PALETTEENTRY *lpPeSrc, *lpPeDest;
    Sint32 i, n;

    switch (WorkOffs + colortbl[ColorNo].offset / 64) {
    case 0:
        lppe = lpcolorwk;
        break;
    case 1:
        lppe = lpcolorwk2;
        break;
    case 2:
        lppe = lpcolorwk3;
        break;
    default:
        lppe = lpcolorwk4;
        break;
    }

    lpPeSrc = colortbl[ColorNo].PeSrc;
    lpPeDest = &lppe[colortbl[ColorNo].offset];
    n = colortbl[ColorNo].cnt;
    for (i = 0; i < n; ++i) {
        *lpPeDest++ = *lpPeSrc++;
    }
}

Sint32 FadeProc(void) {
    switch (*lpFadeFlag) {
    case 1:
        return fadein0_new();
    case 2:
        return fadeout_new();
    case 3:
        return flashin_new();
    case 4:
        return flashout_new();
    }
    return 1;
}
