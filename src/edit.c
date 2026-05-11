#include "equ.h"
#include "edit.h"
#include "action.h"
#include "actset.h"
#include "fcol.h"

#if defined(R42B)
Sint32 CKeyTogle;
#else
extern Sint32 CKeyTogle;
#endif
extern sprite_pattern* sncpat[];
extern edit_data edit_tbl;

void edit(sprite_status* pActwk) {
  edit_data* pEdit_tbl;
  sprite_status* pActwk2;
  Sint32 CKey;
  Sint32 edit_spd = 0x80000;
  Sint32 speed;
  Sint16 xposi;
  Sint16 yposi;
  Sint16 sprvs;
  Sint16 index;
  char en;

if (editmode.b.h == 0) return;

  if (swdata1.b.h & 15)
  {
    edit_spd += 8192;
    if (edit_spd > 0x80000) edit_spd = 0x80000;
  }
  else
  {
    edit_spd = 16384;
  }
  speed = edit_spd;
  if (swdata1.b.h & 1) pActwk->yposi.l -= speed;
  if (swdata1.b.h & 2) pActwk->yposi.l += speed;
  if (swdata1.b.h & 4) pActwk->xposi.l -= speed;
  if (swdata1.b.h & 8) pActwk->xposi.l += speed;
  yposi = pActwk->yposi.w.h;
  sprvs = pActwk->sprvsize;
  yposi += sprvs;
  xposi = pActwk->xposi.w.h;
  blkno = scramapad(pActwk, xposi, yposi);

  pEdit_tbl = &edit_tbl;

  if (swdata1.b.l & 16)
  {
    editmode.b.h = 0;
    pActwk->patbase = sncpat;
    pActwk->sproffset = 1920;
    pActwk->sprpri = 2;
    pActwk->patno = 0;
    pActwk->actflg = 4;
    actionsub(pActwk);
    return;
  }

  if (swdata1.b.l & 64)
  {
    en = editno.b.h + 1;
    if (en >= pEdit_tbl->num) en = 0;
    editno.b.h = en;
  }

index = editno.b.h;

  pActwk->patbase = pEdit_tbl->dat[index].patbase;
  pActwk->sproffset = pEdit_tbl->dat[index].offset;
  pActwk->sprpri = pEdit_tbl->dat[index].sprpri;
  pActwk->patno = pEdit_tbl->dat[index].patno;
  edit_user = pEdit_tbl->dat[index].userflag_l;
  pActwk->actflg = pEdit_tbl->dat[index].rludrv | 4;
  pActwk->mstno.b.h = 0;

  CKey = 0;
  if (CKey & 128)
  { /* Unreachable code. */
    CKey &= 1;
    if (CKeyTogle != CKey) {
      if (actwkchk(&pActwk2) == 0)
      {
        index = editno.b.h;
        pActwk2->actno = pEdit_tbl->dat[index].actno;
        pActwk2->userflag.b.h = pEdit_tbl->dat[index].userflag_h;
        pActwk2->userflag.b.l = pEdit_tbl->dat[index].userflag_l;
        pActwk2->patno = pEdit_tbl->dat[index].patno;
        pActwk2->xposi.w.h = pActwk->xposi.w.h;
        pActwk2->yposi.w.h = pActwk->yposi.w.h;
        pActwk2->actflg = pActwk->actflg & 3;
        pActwk2->cddat = pActwk->actflg & 3;
      }
    }
  }
  CKeyTogle = CKey & 1;
  actionsub(pActwk);
}
