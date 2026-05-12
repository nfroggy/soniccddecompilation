#include "../equ.h"
#include "dev81a.h"
#include "../impfuncs.h"

static Uint32 cg_chg1(tile_changes *pTbl, Sint32 iNum, Uint8 **ppChgTim,
                      Uint8 **ppChgCnt, Sint32 *BmpNo, Sint32 *TileStart);

map_init_data mapinittbl = {0, 0, 0, 3, 2, 0, 0, 4, 4, 129};
static Sint32 pis1cg[8] = {0, 1, 2, 3, 4, 5, 6, 7};
static Sint32 pis2cg[8] = {8, 9, 10, 11, 12, 13, 14, 15};
static Sint32 pis3cg[8] = {16, 17, 18, 19, 20, 21, 22, 23};
static Sint32 com1cg[8] = {24, 25, 26, 27, 28, 29, 30, 31};
static Sint32 com2cg[8] = {32, 33, 34, 35, 36, 37, 38, 39};
static Sint32 com3cg[8] = {40, 41, 42, 43, 44, 45, 46, 47};
static Sint32 com4cg[8] = {48, 49, 50, 51, 52, 53, 54, 55};
static Sint32 mot1cg[16] = {56, 57, 58, 59, 60, 61, 62, 63,
                            64, 65, 66, 67, 68, 69, 70, 71};
static Sint32 mot2cg[16] = {72, 73, 74, 75, 76, 77, 78, 79,
                            80, 81, 82, 83, 84, 85, 86, 87};
static Sint32 mot3cg[16] = {88, 89, 90, 91, 92,  93,  94,  95,
                            96, 97, 98, 99, 100, 101, 102, 103};
static Sint32 mot4cg[16] = {104, 105, 106, 107, 108, 109, 110, 111,
                            112, 113, 114, 115, 116, 117, 118, 119};
static Sint32 spo1cg[4] = {120, 121, 122, 123};
static Sint32 spo2cg[4] = {124, 125, 126, 127};
static Sint32 spo3cg[4] = {128, 129, 130, 131};
static Sint32 spo4cg[4] = {132, 133, 134, 135};
static Sint32 spo5cg[4] = {136, 137, 138, 139};
static Sint32 spo6cg[4] = {140, 141, 142, 143};
static Sint32 spo7cg[4] = {144, 145, 146, 147};
static Sint32 bal1cg[16] = {148, 149, 150, 151, 152, 153, 154, 155,
                            156, 157, 158, 159, 160, 161, 162, 163};
static Sint32 bal2cg[16] = {164, 165, 166, 167, 168, 169, 170, 171,
                            172, 173, 174, 175, 176, 177, 178, 179};
static Sint32 bal3cg[16] = {180, 181, 182, 183, 184, 185, 186, 187,
                            188, 189, 190, 191, 192, 193, 194, 195};
static Sint32 bal4cg[16] = {196, 197, 198, 199, 200, 201, 202, 203,
                            204, 205, 206, 207, 208, 209, 210, 211};
static tile_changes tbl_pis = {4,
                               0,
                               {{6, 0}, {6, 1}, {6, 2}, {6, 3}},
                               {pis1cg, pis2cg, pis3cg, pis2cg},
                               333};
static tile_changes tbl_com = {4,
                               0,
                               {{3, 0}, {3, 1}, {3, 2}, {3, 3}},
                               {com1cg, com2cg, com3cg, com4cg},
                               341};
static tile_changes tbl_mot = {4,
                               0,
                               {{4, 0}, {4, 1}, {4, 2}, {4, 3}},
                               {mot1cg, mot2cg, mot3cg, mot4cg},
                               349};
static tile_changes tbl_spo = {
    7,
    0,
    {{7, 0}, {7, 1}, {7, 2}, {7, 3}, {7, 4}, {7, 5}, {7, 6}},
    {spo1cg, spo2cg, spo3cg, spo4cg, spo5cg, spo6cg, spo7cg},
    365};
static tile_changes tbl_bal = {16,
                               0,
                               {{3, 0},
                                {3, 1},
                                {12, 2},
                                {3, 3},
                                {3, 0},
                                {3, 1},
                                {12, 2},
                                {3, 3},
                                {3, 0},
                                {3, 1},
                                {12, 2},
                                {3, 3},
                                {50, 0},
                                {3, 1},
                                {12, 2},
                                {3, 3}},
                               {bal1cg, bal2cg, bal3cg, bal4cg},
                               529};

void cg_change(void) {
    Uint8 *pcgchgtim, *pcgchgcnt;
    Sint32 i, playsubdma[16], top;

    pcgchgtim = cgchgtim;
    pcgchgcnt = cgchgcnt;

    if (cg_chg1(&tbl_pis, 7, &pcgchgtim, &pcgchgcnt, playsubdma, &top) != 0) {

        for (i = 0; i < 8; ++i, ++top) {
            ChangeTileBmp(top, playsubdma[i]);
        }
    }

    if (cg_chg1(&tbl_com, 7, &pcgchgtim, &pcgchgcnt, playsubdma, &top) != 0) {

        for (i = 0; i < 8; ++i, ++top) {
            ChangeTileBmp(top, playsubdma[i]);
        }
    }

    if (cg_chg1(&tbl_mot, 15, &pcgchgtim, &pcgchgcnt, playsubdma, &top) != 0) {

        for (i = 0; i < 16; ++i, ++top) {
            ChangeTileBmp(top, playsubdma[i]);
        }
    }

    if (cg_chg1(&tbl_bal, 15, &pcgchgtim, &pcgchgcnt, playsubdma, &top) != 0) {

        for (i = 0; i < 16; ++i, ++top) {
            ChangeTileBmp(top, playsubdma[i]);
        }
    }

    if (cg_chg1(&tbl_spo, 3, &pcgchgtim, &pcgchgcnt, playsubdma, &top) != 0) {

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
