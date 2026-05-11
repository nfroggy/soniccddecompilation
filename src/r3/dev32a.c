#include "../equ.h"
#include "dev32a.h"
#include "../impfuncs.h"

map_init_data mapinittbl = { 0, 0, 0, 3, 2, 0, 0, 5, 5, 129 };
static Sint32 drum_re0cg[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
static Sint32 drum_re1cg[6] = { 10, 11, 12, 13, 14, 15 };
static Sint32 drum_no0cg[10] = { 16, 17, 18, 19, 20, 21, 22, 23, 24, 25 };
static Sint32 drum_no1cg[6] = { 26, 27, 28, 29, 30, 31 };
static tile_changes tbl_drum00 = {
  2,
  0,
  {
    { 10, 0 },
    { 10, 1 }
  },
  {
    drum_no0cg,
    drum_re0cg
  },
  436
};
static tile_changes tbl_drum01 = {
  2,
  0,
  {
    { 10, 0 },
    { 10, 1 }
  },
  {
    drum_no1cg,
    drum_re1cg
  },
  462
};

void cg_change(void) {
  Uint8 *pcgchgtim, *pcgchgcnt;
  Sint32 i, playsubdma[16], top;

  pcgchgtim = cgchgtim;
  pcgchgcnt = cgchgcnt;

  if (cg_chg1(&tbl_drum00, 9, &pcgchgtim, &pcgchgcnt, playsubdma, &top) != 0) {

    for (i = 0; i < 10; ++i, ++top) {
      ChangeTileBmp(top, playsubdma[i]);
    }
  }

  if (cg_chg1(&tbl_drum01, 5, &pcgchgtim, &pcgchgcnt, playsubdma, &top) != 0) {

    for (i = 0; i < 6; ++i, ++top) {
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
  if (++tblidx >= pTbl->chgcnt)
    tblidx = 0;

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
