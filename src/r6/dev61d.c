#include "../equ.h"
#include "dev61a.h"
#include "../impfuncs.h"

static Uint32 cg_chg1(tile_changes* pTbl, Sint32 iNum, Uint8** ppChgTim, Uint8** ppChgCnt, Sint32* BmpNo, Sint32* TileStart);

map_init_data mapinittbl = { 0, 0, 0, 3, 2, 0, 0, 4, 4, 129 };
static Sint32 sp1cg[4] = { 0, 1, 2, 3 };
static Sint32 sp2cg[4] = { 4, 5, 6, 7 };
static Sint32 t1cg[8] = { 8, 9, 10, 11, 12, 13, 14, 15 };
static Sint32 t2cg[8] = { 16, 17, 18, 19, 20, 21, 22, 23 };
static Sint32 t3cg[8] = { 24, 25, 26, 27, 28, 29, 30, 31 };
static Sint32 r01cg[4] = { 32, 33, 34, 35 };
static Sint32 r02cg[4] = { 36, 37, 38, 39 };
static Sint32 r03cg[4] = { 40, 41, 42, 43 };
static Sint32 r04cg[4] = { 44, 45, 46, 47 };
static tile_changes tbl_thund = {
  3,
  0,
  {
    { 4, 0 },
    { 4, 1 },
    { 4, 2 }
  },
  {
    t1cg,
    t2cg,
    t3cg
  },
  380
};
static tile_changes tbl_sp = {
  2,
  0,
  {
    { 2, 0 },
    { 2, 1 }
  },
  {
    sp1cg,
    sp2cg
  },
  376
};
static tile_changes tbl_light = {
  4,
  0,
  {
    { 4, 0 },
    { 9, 1 },
    { 4, 2 },
    { 15, 3 }
  },
  {
    r01cg,
    r02cg,
    r03cg,
    r04cg
  },
  372
};

void cg_change(void) {
  Uint8 *pcgchgtim, *pcgchgcnt;
  Sint32 i, playsubdma[16], top;

  pcgchgtim = cgchgtim;
  pcgchgcnt = cgchgcnt;

  if (cg_chg1(&tbl_thund, 7, &pcgchgtim, &pcgchgcnt, playsubdma, &top) != 0) {

    for (i = 0; i < 8; ++i, ++top) {
      ChangeTileBmp(top, playsubdma[i]);
    }
  }

if (cg_chg1(&tbl_sp, 3, &pcgchgtim, &pcgchgcnt, playsubdma, &top) != 0) {

    for (i = 0; i < 4; ++i, ++top) {
      ChangeTileBmp(top, playsubdma[i]);
    }
  }

if (cg_chg1(&tbl_light, 3, &pcgchgtim, &pcgchgcnt, playsubdma, &top) != 0) {

    for (i = 0; i < 4; ++i, ++top) {
      ChangeTileBmp(top, playsubdma[i]);
    }
  }
}

Uint32 cg_chg1(tile_changes* pTbl, Sint32 iNum, Uint8** ppChgTim, Uint8** ppChgCnt, Sint32* BmpNo, Sint32* TileStart) {
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

  for ( ; iNum >= 0; --iNum) {
    *BmpNo++ = *pNoTbl++;
  }

++*ppChgTim;
  ++*ppChgCnt;
  *TileStart = pTbl->TileStart;
  return 1;
}
