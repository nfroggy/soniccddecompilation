#include "../equ.h"
#include "dev61a.h"
#include "../impfuncs.h"

static Uint32 cg_chg1(tile_changes *pTbl, Sint32 iNum, Uint8 **ppChgTim,
                      Uint8 **ppChgCnt, Sint32 *BmpNo, Sint32 *TileStart);

map_init_data mapinittbl = {0, 0, 0, 3, 2, 0, 0, 4, 4, 129};
static Sint32 nullcg[8];
static Sint32 sp1cg[4] = {1, 2, 3, 4};
static Sint32 sp2cg[4] = {5, 6, 7, 8};
static Sint32 t1cg[8] = {9, 10, 11, 12, 13, 14, 15, 16};
static Sint32 t2cg[8] = {17, 18, 19, 20, 21, 22, 23, 24};
static Sint32 t3cg[8] = {25, 26, 27, 28, 29, 30, 31, 32};
static tile_changes tbl_thund = {
    3, 0, {{4, 0}, {4, 1}, {4, 2}}, {t1cg, t2cg, t3cg}, 347};
static tile_changes tbl_null = {
    3, 0, {{4, 0}, {4, 1}, {4, 2}}, {nullcg, nullcg, nullcg}, 347};
static tile_changes tbl_sp = {2, 0, {{3, 0}, {3, 1}}, {sp1cg, sp2cg}, 343};

void cg_change(void) {
    Uint8 *pcgchgtim, *pcgchgcnt;
    Sint32 i, playsubdma[16], top;

    pcgchgtim = cgchgtim;
    pcgchgcnt = cgchgcnt;

    if (cg_chg1(&tbl_sp, 3, &pcgchgtim, &pcgchgcnt, playsubdma, &top) != 0) {

        for (i = 0; i < 4; ++i, ++top) {
            ChangeTileBmp(top, playsubdma[i]);
        }
    }

    --cgchgtim[2];
    if ((char)cgchgtim[2] < 0) {

        cgchgtim[2] ^= 1;
        if (cgchgtim[2] == 0)
            cgchgtim[2] = 90;
        else
            cgchgtim[2] = 30;
    }

    if (cgchgtim[2] != 0) {

        if (cg_chg1(&tbl_thund, 7, &pcgchgtim, &pcgchgcnt, playsubdma, &top) !=
            0) {

            for (i = 0; i < 8; ++i, ++top) {
                ChangeTileBmp(top, playsubdma[i]);
            }
        }
        return;
    }

    if (cg_chg1(&tbl_null, 7, &pcgchgtim, &pcgchgcnt, playsubdma, &top) != 0) {

        for (i = 0; i < 8; ++i, ++top) {
            ChangeTileBmp(top, playsubdma[i]);
        }
    }
}

Uint32 cg_chg1(tile_changes *pTbl, Sint32 iNum, Uint8 **ppChgTim,
               Uint8 **ppChgCnt, Sint32 *BmpNo, Sint32 *TileStart) {
    Sint32 tblidx, *pNoTbl;
    char Timwk;

    Timwk = --**ppChgTim;
    if (Timwk >= 0) {

        ++*ppChgTim;
        ++*ppChgCnt;
        return 0;
    }

    tblidx = **ppChgCnt;
    if (++tblidx >= pTbl->chgcnt) {
        tblidx = 0;
    }

    **ppChgCnt = tblidx;
    **ppChgTim = pTbl->dattbl[tblidx].tim;

    tblidx = (Sint16)pTbl->dattbl[tblidx].no;
    pNoTbl = pTbl->TileNoTbl[tblidx];

    for (; iNum >= 0; --iNum) {
        *BmpNo++ = *pNoTbl++;
    }

    ++*ppChgTim;
    ++*ppChgCnt;
    *TileStart = pTbl->TileStart;
    return 1;
}
