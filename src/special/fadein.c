#include "../types.h"
#include "common.h"
#include "spm_equ.h"
#include "fadein.h"

static Uint16 code_tbl[3] = {1, 5, 9};
static Uint8 byMskwk = 0;
static Uint16 wColwk = 0;
static Sint32 FadeCount = 0;
extern Sint32 *lpFadeFlag;
extern PALETTEENTRY *lpcolorwk;

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
    } else {
        FadeCount = 8;
        ++wColwk;
    }
}

void woutcolor(Uint16 wD1) {
    Sint16 i;
    PALETTEENTRY *lpColwk;

    lpColwk = lpcolorwk;
    for (i = 0; i < 64; ++i, ++lpColwk) {
        switch (wD1) {
        case 1:
            if (lpColwk->peRed < 224)
                lpColwk->peRed += 32;

            break;
        case 5:
            if (lpColwk->peGreen < 224)
                lpColwk->peGreen += 32;

            break;
        case 9:
            if (lpColwk->peBlue < 224)
                lpColwk->peBlue += 32;

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
    } else {
        FadeCount = 8;
        byMskwk = 224;
        ++wColwk;
    }
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
        }

        ++lpSavewk;
        ++lpColwk;
    }
    intflg |= 2;
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
