#include "../../types.h"
#include "../common/hmx_types.h"
#include "svdbmp.h"
#include "svdsprt.h"
#include "../common/hmx_oeeactl.h"

static Sint32 GetMarkfontIndx(char c, Sint32 kind);
static void MovSonicCursol(POINT point);
static Sint32 isqrt(Sint32 xx);

static Sint32 SONICMOVEM_MAG = 4;
extern mark_info markTable[];
extern draw_context *s_ctx;
extern Uint32 hSprFile[6][56];
extern Sint32 PRIO_MAX;
extern sprite_bmp infoSprtBmp[];
extern void (*hmx_sprite_set_flags_module)(hmx_sprite *, Sint32);
extern void (*hmx_sprite_set_bitmap_module)(hmx_sprite *, hmx_bitmap *);
extern void (*hmx_sprite_set_position_module)(hmx_sprite *, Sint32, Sint32);
extern POINT nPosiXSprFile[];
extern Uint32 hSprMes[];
extern Sint32 nPosiXSprMes[];
static Sint32 cursolPosiIndxX;
extern Sint32 nFileNumber;
extern Sint32 PLAYERCHAR_W;
static Sint32 nSonicKind;
extern Uint32 bMenuChosed;
extern Sint32 nTimerCunt;
static POINT posiSonicCursol;
static POINT posiTarget;
static Sint32 nSonicMode;
static Sint32 fSonicIncX;
static Sint32 fSonicIncY;
static Sint32 nStandStartTimerCunt;
static int_union posiStartXOld;
static int_union posiStartYOld;
static Uint32 hFxh[120];

static Sint32 GetMarkfontIndx(char c, Sint32 kind) {
    Sint32 n = 0;

    while (markTable[n].code) {
        if (c == markTable[n].code) {
            switch (kind) {
            case 0:
                return markTable[n].white88;
            case 1:
                return markTable[n].yello88;
            case 2:
                return markTable[n].yello816;
            case 3:
                return markTable[n].whitex16;
            default:
                return -1;
            }
        }
        ++n;
    }
    return -1;
}

Uint32 CreateCharSprt(char c, Sint32 kind, Sint32 nBlockNo, Sint32 nSprNum) {
    RECTL SprRect = {0, 0, 0, 0};
    Uint32 mask[4] = {0, 0, 0, 0};
    POINT point;
    Sint32 indx;

    switch (kind) {
    case 0:
        if (c >= 48 && c < 58)
            indx = c - 37;
        else if (c >= 65 && c < 91)
            indx = c - 44;
        else {
            indx = GetMarkfontIndx(c, kind);
            if (indx == -1)
                return 0;
        }
        break;
    case 1:
        if (c >= 48 && c < 58)
            indx = c - 1;
        else if (c >= 65 && c < 91)
            indx = c - 8;
        else {
            indx = GetMarkfontIndx(c, kind);
            if (indx == -1)
                return 0;
        }
        break;
    case 2:
        if (c >= 48 && c < 58)
            indx = c + 35;
        else if (c >= 65 && c < 91)
            indx = c + 28;
        else {
            indx = GetMarkfontIndx(c, kind);
            if (indx == -1)
                return 0;
        }
        break;

    case 3:
        if (c >= 48 && c < 58)
            indx = c + 71;
        else if (c >= 65 && c < 91)
            indx = c + 64;
        else {
            indx = GetMarkfontIndx(c, kind);
            if (indx == -1)
                return 0;
        }
        break;
    default:
        return 0;
    }
    if (nBlockNo == 1) {
        if (hSprMes[nSprNum] != 0)
            return 0;

        sprCreate(&hSprMes[nSprNum]);

        point.x = nPosiXSprMes[nSprNum];
        point.y = 10;

        hmx_sprite_set_position_module(s_ctx->sprites[hSprMes[nSprNum]],
                                       point.x, point.y);
        hmx_sprite_set_bitmap_module(s_ctx->sprites[hSprMes[nSprNum]],
                                     s_ctx->spr_bitmaps[indx]);
        hmx_sprite_set_flags_module(s_ctx->sprites[hSprMes[nSprNum]], 0);
        s_ctx->spr_level[hSprMes[nSprNum]] = PRIO_MAX - infoSprtBmp[indx].order;
        return 1;
    }
    if (nBlockNo >= 2 || nBlockNo < 9) {

        if (hSprFile[nBlockNo - 2][nSprNum] != 0)
            return 0;

        sprCreate(&hSprFile[nBlockNo - 2][nSprNum]);

        point.x = nPosiXSprFile[nSprNum].x;
        point.y = (nBlockNo - 2) * 32 + 32 + nPosiXSprFile[nSprNum].y;

        hmx_sprite_set_position_module(
            s_ctx->sprites[hSprFile[nBlockNo - 2][nSprNum]], point.x, point.y);
        hmx_sprite_set_bitmap_module(
            s_ctx->sprites[hSprFile[nBlockNo - 2][nSprNum]],
            s_ctx->spr_bitmaps[indx]);
        hmx_sprite_set_flags_module(
            s_ctx->sprites[hSprFile[nBlockNo - 2][nSprNum]], 0);
        s_ctx->spr_level[hSprFile[nBlockNo - 2][nSprNum]] =
            PRIO_MAX - infoSprtBmp[indx].order;
        return 1;
    }
    return 0;
}

Uint32 CreateStringSprt(char *str, Sint32 kind, Sint32 nBlockNo,
                        Sint32 nStartSprNum) {
    Sint32 n = nStartSprNum;

    while (*str) {
        if (*str != 32)
            CreateCharSprt(*str, kind, nBlockNo, n);
        ++str;
        ++n;
    }
    return 1;
}

void DeleteCharSprt(Sint32 nBlockNo, Sint32 nSprNum) {
    if (nBlockNo == 1) {
        if (hSprMes[nSprNum] == 0)
            return;

        hmx_sprite_set_bitmap_module(s_ctx->sprites[hSprMes[nSprNum]], 0);
        s_ctx->spr_level[hSprMes[nSprNum]] = 0;
        sprDelete(hSprMes[nSprNum]);
        hSprMes[nSprNum] = 0;

    } else if (nBlockNo >= 2 || nBlockNo < 9) {

        if (hSprFile[nBlockNo - 2][nSprNum] != 0) {

            hmx_sprite_set_bitmap_module(
                s_ctx->sprites[hSprFile[nBlockNo - 2][nSprNum]], 0);
            s_ctx->spr_level[hSprFile[nBlockNo - 2][nSprNum]] = 0;
            sprDelete(hSprFile[nBlockNo - 2][nSprNum]);
            hSprFile[nBlockNo - 2][nSprNum] = 0;
        }
    }
}

void DeleteStringSprt(Sint32 strLength, Sint32 nBlockNo, Sint32 nStartSprNum) {
    Sint32 i;
    Sint32 n = nStartSprNum;

    for (i = 0; i < strLength; ++i) {
        DeleteCharSprt(nBlockNo, n++);
    }
}

void DeleteAllCharSprt(void) {
    Sint32 n;

    DeleteStringSprt(27, 1, 0);
    for (n = 0; n < 6; ++n)
        DeleteStringSprt(56, n + 2, 0);
}

Sint32 GetCursolPositon(void) { return cursolPosiIndxX; }

void MovCursol(Sint32 x) {
    POINT point;

    cursolPosiIndxX = x;
    point.x = nPosiXSprFile[0].x + PLAYERCHAR_W * x;
    point.y = nFileNumber * 32 + 32;

    hmx_sprite_set_position_module(s_ctx->sprites[*infoSprtBmp[191].lphSpr],
                                   point.x, point.y);
}

void DispCursol(void) {
    OESprCreateIndx(191);
    MovCursol(cursolPosiIndxX);
}

void HideCursol(void) { OESprDeleteIndx(191); }

void HideSonicCursol(void) { OESprDeleteIndx(nSonicKind); }

void DispSonicCursol(POINT point) {
    nSonicKind = 203;
    OESprCreateIndx(nSonicKind);
    posiSonicCursol = point;

    hmx_sprite_set_position_module(
        s_ctx->sprites[*infoSprtBmp[nSonicKind].lphSpr], posiSonicCursol.x,
        posiSonicCursol.y);

    nSonicMode = 1;
    nStandStartTimerCunt = nTimerCunt;

    if (bMenuChosed == 0) {
        hmx_sprite_set_flags_module(
            s_ctx->sprites[*infoSprtBmp[nSonicKind].lphSpr], 1);

    } else {
        hmx_sprite_set_flags_module(
            s_ctx->sprites[*infoSprtBmp[nSonicKind].lphSpr], 0);
    }
}

static void MovSonicCursol(POINT point) {
    HideSonicCursol();
    if (nSonicKind == 203 || nSonicKind == 204 || nSonicKind == 205)
        nSonicKind = 206;
    else if (nSonicKind == 208)
        nSonicKind = 206;
    else
        ++nSonicKind;
    OESprCreateIndx(nSonicKind);
    posiSonicCursol = point;

    hmx_sprite_set_position_module(
        s_ctx->sprites[*infoSprtBmp[nSonicKind].lphSpr], posiSonicCursol.x,
        posiSonicCursol.y);

    if (fSonicIncX < 0 || bMenuChosed == 0) {
        hmx_sprite_set_flags_module(
            s_ctx->sprites[*infoSprtBmp[nSonicKind].lphSpr], 1);

    } else {
        hmx_sprite_set_flags_module(
            s_ctx->sprites[*infoSprtBmp[nSonicKind].lphSpr], 0);
    }
}

void RunSonicCursol(POINT point) {
    int_union sx, sy;

    posiTarget = point;
    sx.w.h = posiTarget.x - posiSonicCursol.x, sx.w.l = 0;
    sy.w.h = posiTarget.y - posiSonicCursol.y, sy.w.l = 0;
    if (sx.w.h == 0)
        fSonicIncX = 0;
    else
        fSonicIncX =
            SONICMOVEM_MAG *
            (sx.l / isqrt((sx.w.h * sx.w.h + sy.w.h * sy.w.h) * 65536));
    if (sy.w.h == 0)
        fSonicIncY = 0;
    else
        fSonicIncY =
            SONICMOVEM_MAG *
            (sy.l / isqrt((sx.w.h * sx.w.h + sy.w.h * sy.w.h) * 65536));
    posiStartXOld.w.h = posiSonicCursol.x;
    posiStartYOld.w.h = posiSonicCursol.y;
    nSonicMode = 2;
}

static Sint32 isqrt(Sint32 xx) {
    Sint32 s, t;

    if (xx == 0)
        return 0;
    s = 1, t = xx;
    while (s < t)
        s <<= 1, t >>= 1;
label1:
    t = s;
    s = (s + xx / s) >> 1;
    if (s < t)
        goto label1;
    return t;
}

void UpdateSonicCursol(void) {
    if (nSonicMode == 1) {
        if (nTimerCunt % 20 != 0)
            return;

        if (nTimerCunt - nStandStartTimerCunt <= 60)
            return;

        HideSonicCursol();
        if (nSonicKind == 203)
            nSonicKind = 204;
        else if (nSonicKind == 204)
            nSonicKind = 205;
        else if (nSonicKind == 205)
            nSonicKind = 204;
        else
            nSonicKind = 203;
        OESprCreateIndx(nSonicKind);

        if (bMenuChosed == 0) {
            hmx_sprite_set_flags_module(
                s_ctx->sprites[*infoSprtBmp[nSonicKind].lphSpr], 1);

        } else {
            hmx_sprite_set_flags_module(
                s_ctx->sprites[*infoSprtBmp[nSonicKind].lphSpr], 0);
        }

        hmx_sprite_set_position_module(
            s_ctx->sprites[*infoSprtBmp[nSonicKind].lphSpr], posiSonicCursol.x,
            posiSonicCursol.y);

    } else if (nSonicMode == 2) {

        posiStartXOld.l += fSonicIncX * 256;
        posiStartYOld.l += fSonicIncY * 256;

        posiSonicCursol.x = posiStartXOld.w.h;
        posiSonicCursol.y = posiStartYOld.w.h;

        if ((fSonicIncX >= 0 && posiSonicCursol.x >= posiTarget.x) ||
            fSonicIncX < 0 && posiSonicCursol.x <= posiTarget.x)
            posiSonicCursol.x = posiTarget.x;
        if ((fSonicIncY >= 0 && posiSonicCursol.y >= posiTarget.y) ||
            fSonicIncY < 0 && posiSonicCursol.y <= posiTarget.y)
            posiSonicCursol.y = posiTarget.y;
        if (posiSonicCursol.x == posiTarget.x &&
            posiSonicCursol.y == posiTarget.y) {
            HideSonicCursol();
            DispSonicCursol(posiSonicCursol);
        } else
            MovSonicCursol(posiSonicCursol);
    }
}
