#include "../equ.h"
#include "dev81a.h"
#include "../impfuncs.h"

static Uint32 cg_chg1(tile_changes *pTbl, Sint32 iNum, Uint8 **ppChgTim,
                      Uint8 **ppChgCnt, Sint32 *BmpNo, Sint32 *TileStart);

map_init_data mapinittbl = {0, 0, 0, 3, 2, 0, 0, 5, 5, 129};
static Sint32 pan1cg[4] = {0, 1, 2, 3};
static Sint32 pan2cg[4] = {4, 5, 6, 7};
static Sint32 pan3cg[4] = {8, 9, 10, 11};
static Sint32 pan4cg[4] = {12, 13, 14, 15};
static Sint32 pan5cg[4] = {16, 17, 18, 19};
static Sint32 pan6cg[4] = {20, 21, 22, 23};
static tile_changes tbl_pan = {6,
                               0,
                               {{4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 4}, {4, 5}},
                               {pan1cg, pan2cg, pan3cg, pan4cg, pan5cg, pan6cg},
                               444};

void cg_change(void) {
    Uint8 *pcgchgtim, *pcgchgcnt;
    Sint32 i, playsubdma[16], top;

    pcgchgtim = cgchgtim;
    pcgchgcnt = cgchgcnt;

    if (cg_chg1(&tbl_pan, 3, &pcgchgtim, &pcgchgcnt, playsubdma, &top) != 0) {

        for (i = 0; i < 4; ++i, ++top) {
            ChangeTileBmp(top, playsubdma[i]);
        }
    }
}

static Uint32 cg_chg1(tile_changes *pTbl, Sint32 iNum, Uint8 **ppChgTim,
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
