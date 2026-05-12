#include "../equ.h"
#include "warp.h"
#include "../action.h"
#include "../io.h"

static Uint16 clchg_cnt[2] = {65535, 0};
static PALETTEENTRY colortbl[32] = {
    {0, 0, 0, 1},       {0, 224, 0, 1},     {32, 32, 192, 1},
    {64, 64, 224, 1},   {96, 96, 224, 1},   {128, 128, 224, 1},
    {224, 224, 224, 1}, {160, 160, 160, 1}, {128, 128, 128, 1},
    {64, 64, 64, 1},    {224, 160, 128, 1}, {160, 96, 64, 1},
    {224, 0, 0, 1},     {128, 0, 0, 1},     {64, 0, 0, 1},
    {0, 0, 0, 1},       {0, 0, 0, 1},       {0, 224, 0, 1},
    {32, 224, 32, 1},   {64, 224, 64, 1},   {96, 224, 96, 1},
    {128, 224, 128, 1}, {160, 224, 160, 1}, {192, 224, 192, 1},
    {224, 224, 224, 1}, {192, 224, 192, 1}, {160, 224, 160, 1},
    {128, 224, 128, 1}, {96, 224, 96, 1},   {64, 224, 64, 1},
    {32, 224, 32, 1},   {0, 0, 0, 1}};
static PALETTEENTRY clchg_tbl[28] = {
    {0, 224, 0, 1},     {32, 224, 32, 1},   {64, 224, 64, 1},
    {96, 224, 96, 1},   {128, 224, 128, 1}, {160, 224, 160, 1},
    {192, 224, 192, 1}, {224, 224, 224, 1}, {192, 224, 192, 1},
    {160, 224, 160, 1}, {128, 224, 128, 1}, {96, 224, 96, 1},
    {64, 224, 64, 1},   {32, 224, 32, 1},   {0, 224, 0, 1},
    {32, 224, 32, 1},   {64, 224, 64, 1},   {96, 224, 96, 1},
    {128, 224, 128, 1}, {160, 224, 160, 1}, {192, 224, 192, 1},
    {224, 224, 224, 1}, {192, 224, 192, 1}, {160, 224, 160, 1},
    {128, 224, 128, 1}, {96, 224, 96, 1},   {64, 224, 64, 1},
    {32, 224, 32, 1}};
void (*act_tbl[2])(sprite_status *) = {&sonic_act, &atom};
extern bmp_info SprBmp[700];
static Uint16 setcnt = 0;
static PALETTEENTRY colorsave[64] = {0};
static Uint8 clfflg = 0;
static Uint8 byMskwk = 0;
static Uint16 wColwk = 0;
static Sint32 FadeCount = 0;
static Uint16 gmtime1 = 0;
static Uint16 gmtime0 = 0;
void (*ChangeTileBmp)(Sint32, Sint32) = 0;
void (*CDPlay)(Sint16) = 0;
void (*WaveRequest)(Sint16) = 0;
void (*ClrSpriteDebug)(void) = 0;
void (*EAsprset)(Sint16, Sint16, Uint16, Uint16, Uint16) = 0;
Sint32 (*SetGrid)(Sint32, Sint32, Sint32, Sint32) = 0;
void (*sCloseFile)(Sint32) = 0;
Sint32 (*sGetFileSize)(Sint32) = 0;
Sint32 (*sReadFile)(Sint32, void *, Sint32) = 0;
Sint32 (*sOpenFile)(char *) = 0;
void (*sMemCpy)(void *, void *, Sint32) = 0;
void (*sMemSet)(void *, Uint8, Sint32) = 0;
Sint32 (*sRandom)(void) = 0;
void (*sPrintf)(char *, const char *, ...) = 0;
void (*sOutputDebugString)(char *) = 0;

void DLL_meminit(char ***pBufTbl, void **pFuncTbl) {

    SetGrid = *pFuncTbl++;
    EAsprset = *pFuncTbl++;
    ClrSpriteDebug = *pFuncTbl++;
    WaveRequest = *pFuncTbl++;
    CDPlay = *pFuncTbl++;
    ++pFuncTbl;
    ChangeTileBmp = *pFuncTbl++;
    ++pFuncTbl;
    ++pFuncTbl;
    ++pFuncTbl;
    ++pFuncTbl;

    ++pFuncTbl;
    ++pFuncTbl;
    sMemSet = *pFuncTbl++;
    sMemCpy = *pFuncTbl++;
    ++pFuncTbl;
    sRandom = *pFuncTbl++;
    ++pFuncTbl;
    ++pFuncTbl;
    ++pFuncTbl;
    sPrintf = *pFuncTbl++;
    sOutputDebugString = *pFuncTbl++;
    sOpenFile = *pFuncTbl++;
    sReadFile = *pFuncTbl++;
    sCloseFile = *pFuncTbl++;

    pmapwk = (Uint16 *)**pBufTbl++;
    lpcolorwk = (PALETTEENTRY *)**pBufTbl++;
    lpcolorwk2 = (PALETTEENTRY *)**pBufTbl++;
    lpcolorwk3 = (PALETTEENTRY *)**pBufTbl++;
    lpcolorwk4 = (PALETTEENTRY *)**pBufTbl++;
    lphscrollbuff = (int_union *)**pBufTbl++;
    lpFadeFlag = (Sint32 *)*pBufTbl++;
    lpKeepWork = (game_info *)*pBufTbl++;

    lpKeepWork->pSprBmp = SprBmp;
}

void DLL_memfree(void) {}

Sint32 Get_vscroll(void) { return vscroll.l; }

Sint32 Get_scra_h_posiw(void) { return scra_h_posiw.l; }

Sint32 Get_scrb_h_posiw(void) { return scrb_h_posiw.l; }

void SetDebugFlag(Uint32 NewVal) {
    if (NewVal == 1)
        debugflag.w |= 256;
    else
        debugflag.w &= -257;
}

void game_init(void) {
    Sint32 i;

    sMemSet(actwk, 0, sizeof(actwk));

    scra_h_posit.l = 0;
    scra_v_posit.l = 0;
    scrb_h_posit.l = 0;
    scrb_v_posit.l = 0;
    scrc_h_posit.l = 0;
    scrc_v_posit.l = 0;
    scrz_h_posit.l = 0;
    scrz_v_posit.l = 0;

    for (i = 0; i < 32; ++i) {
        lpcolorwk[i] = colortbl[i];
    }
    soundset(216);
    clwin_init();
    gmtime0 = 420;
    clwin();
}

sprite_status *atomwk_search(void) {
    Sint32 i;

    for (i = 4; i < 32; ++i) {
        if (!actwk[i].actno)
            return &actwk[i];
    }
    return &actwk[4];
}

Sint32 game(void) {
    sprite_status *pActwk;
    Uint16 wD0;

    if (!gmtime0)
        return 1;

    clchg();
    wD0 = gmtime1 & 3;
    if (!wD0) {
        wD0 = setcnt++;
        setcnt &= 3;
        actwk[wD0].actno = 1;
    }

    wD0 = gmtime1 & 3;
    if (!wD0) {
        pActwk = atomwk_search();
        pActwk->actno = 2;
    }

    action();
    patset();

    --gmtime0;
    ++gmtime1;
    vscroll.w.h -= 4;

    if (!gmtime0 || swdata.b.l & 128) {
        gmtime0 = 0;
        clwout();
    }
    return 0;
}

static sprite_pattern snc0 = {1, {{-16, -24, 0, 0}}};
static sprite_pattern snc1 = {1, {{-16, -24, 0, 1}}};
static sprite_pattern snc2 = {1, {{-16, -24, 0, 2}}};
static sprite_pattern snc3 = {1, {{-16, -24, 0, 3}}};
static sprite_pattern snc4 = {1, {{-16, -24, 0, 4}}};
static sprite_pattern *ptsonic[5] = {&snc0, &snc1, &snc2, &snc3, &snc4};

void sonic0(sprite_status *pActwk) {
    pActwk->sproffset = 32768;
    pActwk->patbase = ptsonic;
    pActwk->xposi.w.h = 288;
    pActwk->yposi.w.h = 416;
    ++pActwk->r_no0;
}

void sonic1(sprite_status *pActwk) {
    pActwk->yposi.w.h -= 8;
    if (pActwk->yposi.w.h == 248) {
        ++pActwk->r_no0;
        pActwk->actfree[8] = 240;
    }
}

void sonic2(sprite_status *pActwk) {
    --pActwk->actfree[8];
    if (pActwk->actfree[8] == 0) {
        ++pActwk->r_no0;
    }
}

void sonic3(sprite_status *pActwk) {
    pActwk->yposi.w.h -= 8;
    if (pActwk->yposi.w.h == 96) {
        ++pActwk->r_no0;
    }
}

void sonic_act(sprite_status *pActwk) {
    if (gmtime1 & 1) {
        ++pActwk->patno;
        if (pActwk->patno > 4)
            pActwk->patno = 0;
    }
    pActwk->patno = actwk[0].patno;

    switch (pActwk->r_no0) {
    case 0:
        sonic0(pActwk);
        break;
    case 1:
        sonic1(pActwk);
        break;
    case 2:
        sonic2(pActwk);
        break;
    case 3:
        sonic3(pActwk);
    }

    actionsub(pActwk);
}

static sprite_pattern atm0 = {1, {{-16, -16, 0, 5}}};
static sprite_pattern atm1 = {1, {{-16, -16, 0, 6}}};
static sprite_pattern atm2 = {1, {{-24, -24, 0, 7}}};
static sprite_pattern atm3 = {1, {{-16, -16, 0, 8}}};
static sprite_pattern atm4 = {1, {{-16, -24, 0, 9}}};
static sprite_pattern *spr_atom[5] = {&atm0, &atm1, &atm2, &atm3, &atm4};

void atom1(sprite_status *pActwk) {
    Sint32 lD0;

    lD0 = *(Sint32 *)&pActwk->actfree[0];
    pActwk->xposi.l += lD0;

    lD0 = *(Sint32 *)&pActwk->actfree[4];
    pActwk->yposi.l += lD0;

    --pActwk->actfree[8];
    if (pActwk->actfree[8]) {
        actionsub(pActwk);
    } else {
        frameout(pActwk);
    }
}

void atom0(sprite_status *pActwk) {
    int_union lD0, lD1;
    pActwk->sproffset = 32768;
    pActwk->patbase = spr_atom;
    ++pActwk->r_no0;

    lD0.l = lD1.l = sRandom();
    lD0.l &= 262143;
    *(Sint32 *)&pActwk->actfree[4] = lD0.l;
    lD1.w.l &= 31;
    pActwk->xposi.w.h = actwk[0].xposi.w.h - 16 + lD1.w.l;
    pActwk->yposi.w.h = actwk[0].yposi.w.h - 24;
    pActwk->actfree[8] = 45;
    atom1(pActwk);
}

void atom(sprite_status *pActwk) {
    if (gmtime1 & 1) {
        ++pActwk->patno;
        if (pActwk->patno > 4)
            pActwk->patno = 0;
    }
    switch (pActwk->r_no0) {
    case 0:
        atom0(pActwk);
        break;
    case 1:
        atom1(pActwk);
    }
}

void clchg(void) {
    Uint16 wD0;
    Sint32 i;

    ++clchg_cnt[0];
    if ((clchg_cnt[0] & 1) == 0) {

        wD0 = clchg_cnt[1];
        ++clchg_cnt[1];
        if (clchg_cnt[1] >= 14) {
            clchg_cnt[1] = 0;
        }
        for (i = 17; i < 31; ++i) {
            lpcolorwk[i] = clchg_tbl[wD0++];
        }
    }
}

static Uint16 code_tbl[3] = {1, 5, 9};

void clwout(void) {
    clfflg = 1;
    *lpFadeFlag = 2;
    FadeCount = 8;
    wColwk = 0;
}

void clwout_new(void) {
    if (wColwk >= 3) {

        *lpFadeFlag = 0;
        return;
    }
    if (--FadeCount >= 0) {

        woutcolor(code_tbl[wColwk]);
        return;
    }

    FadeCount = 8;
    ++wColwk;
}

void woutcolor(Uint16 wD1) {
    Sint16 i;
    PALETTEENTRY *lpColwk;

    lpColwk = lpcolorwk;
    for (i = 0; i < 64; ++i, ++lpColwk) {

        switch (wD1) {

        case 1:
            if (lpColwk->peRed < 224) {
                lpColwk->peRed += 32;
            }
            break;
        case 5:
            if (lpColwk->peGreen < 224) {
                lpColwk->peGreen += 32;
            }
            break;
        case 9:
            if (lpColwk->peBlue < 224) {
                lpColwk->peBlue += 32;
            }
            break;
        }
    }
}

void clwin_init(void) {
    Sint16 i;
    PALETTEENTRY *lpColwk;
    PALETTEENTRY *lpSavewk;
    PALETTEENTRY tmp = {224, 224, 224, 1};

    lpColwk = lpcolorwk;
    lpSavewk = colorsave;
    for (i = 0; i < 64; ++i) {

        *lpSavewk++ = *lpColwk;
        *lpColwk++ = tmp;
    }
}

void clwin(void) {
    *lpFadeFlag = 1;
    FadeCount = 8;
    wColwk = 0;
    byMskwk = 224;
}

void clwin_new(void) {
    if (wColwk >= 3) {

        *lpFadeFlag = 0;
        return;
    }
    if (--FadeCount >= 0) {

        wincolor(byMskwk, code_tbl[wColwk]);
        byMskwk -= 32;
        return;
    }

    FadeCount = 8;
    byMskwk = 224;
    ++wColwk;
}

void wincolor(Uint8 byD0, Uint16 wD1) {
    Sint16 i;
    Uint8 byColwk;
    PALETTEENTRY *lpColwk, *lpSavewk;

    lpColwk = lpcolorwk;
    lpSavewk = colorsave;
    for (i = 0; i < 64; ++i) {

        switch (wD1) {

        case 1:
            byColwk = lpSavewk->peRed;
            if (byColwk < byD0)
                byColwk = byD0;
            lpColwk->peRed = byColwk;
            break;
        case 5:
            byColwk = lpSavewk->peGreen;
            if (byColwk < byD0)
                byColwk = byD0;
            lpColwk->peGreen = byColwk;
            break;
        case 9:
            byColwk = lpSavewk->peBlue;
            if (byColwk < byD0)
                byColwk = byD0;
            lpColwk->peBlue = byColwk;
            break;
        }
        ++lpSavewk;
        ++lpColwk;
    }
}

Sint32 FadeProc(void) {
    switch (*lpFadeFlag) {

    case 1:
        clwin_new();
        break;
    case 2:
        clwout_new();
        break;
    }
    return 0;
}

static Uint8 SeToWavTbl[80] = {
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 20, 21, 22, 22, 23, 23, 24, 25, 26, 27,
    28, 29, 30, 31, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54,
    55, 56, 57, 58, 58, 59, 60, 61, 62, 63, 64, 65, 67, 68, 69, 70,
    71, 71, 72, 73, 74, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42};

void soundset(Sint16 ReqNo) {
    if (ReqNo == 171)
        return;

    ReqNo -= 144;
    ReqNo = (Uint16)SeToWavTbl[ReqNo];

    WaveRequest(ReqNo);
}

dlink_export ExportedFunctions = {&game_init,
                                  (void (*)(void)) & game,
                                  &DLL_meminit,
                                  &DLL_memfree,
                                  (void (*)(Sint16, Sint16)) & SWdataSet,
                                  &Get_vscroll,
                                  &Get_scra_h_posiw,
                                  &Get_scrb_h_posiw,
                                  &FadeProc,
                                  &SetDebugFlag,
                                  0,
                                  0};
