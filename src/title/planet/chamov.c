#include "../../types.h"
#include "lpl_types.h"
#include "spm_equ.h"
#include "chamov.h"
#include "etc.h"

void(*flicky_acttbl[4])(sprite_status_lpl*) =
{
  &flickyinit,
  &flickymove,
  &flickyslow,
  &flickydown
};
extern sprite_patterns_title flicky_tbl1;
extern sprite_patterns_title flicky_tbl2;
extern sprite_patterns_title flicky_tbl3;
void(*star_acttbl[2])(sprite_status_lpl*) =
{
  &starinit,
  &starmove
};
extern sprite_patterns_title star_tbl;
void(*ufo_acttbl[4])(sprite_status_lpl*) =
{
  &ufoinit,
  &ufomode,
  &ufomove1,
  &ufomove2
};
extern sprite_patterns_title ufo_tbl;
extern sprite_patterns_title ufo_tbl1;
extern sprite_patterns_title ufo_tbl2;
void(*eggm_acttbl[2])(sprite_status_lpl*) =
{
  &eggminit,
  &eggmmove
};
extern sprite_patterns_title eggm_tbl;
extern sprite_patterns_title eggm_tbl2;
void(*msnc_acttbl[5])(sprite_status_lpl*) =
{
  &msncinit,
  &msncstart,
  &msncfloat,
  &msncdush,
  &msncexit
};
extern sprite_patterns_title msnc_tbl;
extern sprite_patterns_title msnc_tbl2;
void(*tails_acttbl[4])(sprite_status_lpl*) =
{
  &tailsinit,
  &tailsmove,
  &tailsmove1,
  &ufomove2
};
extern sprite_patterns_title tails_tbl;
extern sprite_patterns_title tails_up_tbl;
extern sprite_patterns_title tails_down_tbl;

void spr_kill(sprite_status_lpl* pActwk) {
  actset_flg ^= (Uint8)pActwk->ACT_FLG;
  pActwk->SPR_FLG |= 16;
}

void spr_kills(sprite_status_lpl* pActwk) {
  Sint16 i;
  Uint16 wD0;

  wD0 = pActwk->ACT_NO;
  pActwk->ACT_NO = 0;
  pActwk->SPR_FLG |= 16;
  for (i = 0; i < 16; ++i)
  {
    if (actwk[i].ACT_NO == wD0) return;

  }
  actset_flg ^= (Uint8)pActwk->ACT_FLG;
}

Sint16 killchk(sprite_status_lpl* pActwk) {
  if (pActwk->XPOSI.w.h <= -80)
    return 1;
  if (pActwk->XPOSI.w.h >= 400)
    return 1;
  if (pActwk->YPOSI.w.h < -4)
    return 1;
  if (pActwk->YPOSI.w.h >= 224)
    return 1;

  return 0;
}

Sint16 chk_another(Uint16 wD0, sprite_status_lpl* pActwk, sprite_status_lpl** ppActwk) {
  Sint16 i;
  sprite_status_lpl* pActfind;

  pActfind = actwk;
  for (i = 0; i < 16; ++i, ++pActfind)
  {
    if (pActfind->ACT_NO == wD0)
      if (pActfind - actwk != pActwk - actwk)
      {
        *ppActwk = pActfind;
        return 1;
      }
  }
  return 0;
}

void sinmove(sprite_status_lpl* pActwk) {
  Sint16 iD0, iD3;
  int_union lD0, lD3;

  if (killchk(pActwk) != 0)
  {
    spr_kills(pActwk);
    return;
  }

  pActwk->YPOSI.w.h -= pActwk->Y_OFFSET;

  iD3 = sin_planet(pActwk->SIN_CNT);
  iD0 = pActwk->Y_WIDE;
  lD3.l = iD0 * iD3;
  lD3.l >>= 8;
  pActwk->Y_OFFSET = lD3.w.l;
  pActwk->YPOSI.w.h += lD3.w.l;

  lD0.l = random() & 32767;
  lD0.w.l = lD0.l % pActwk->X_WIDE;
  pActwk->SIN_CNT += lD0.w.l;
  if (pActwk->SIN_CNT >= 511)
    pActwk->SIN_CNT -= 511;

  lD0.l = pActwk->X_SPEED.l;
  if (music_selflg != 0)
    lD0.l <<= 3;
  pActwk->XPOSI.l += lD0.l;

  pActwk->YPOSI.l += pActwk->Y_SPEED.l;
}

void flicky_ctrl(sprite_status_lpl* pActwk) {
  flicky_acttbl[pActwk->EXE_NO](pActwk);
}

void flickyinit(sprite_status_lpl* pActwk) {
  pActwk->CGBASE += 16384;
  pActwk->PAT_NO = 0;

  pActwk->X_OFFSET = 0;
  pActwk->Y_OFFSET = 0;

  if (pActwk->SPR_FLG & 2)
  {
    pActwk->EXE_NO = 3;
    return;
  }

  if (pActwk->SPR_FLG & 1)
  {
    pActwk->EXE_NO = 2;
  }
  else
    pActwk->EXE_NO = 1;
}

void flickymove(sprite_status_lpl* pActwk) {
  if (killchk(pActwk) != 0)
  {
    spr_kills(pActwk);
  }
  else
    fnormalmove(pActwk);
}

void flickyslow(sprite_status_lpl* pActwk) {
  if (killchk(pActwk) != 0)
  {
    spr_kills(pActwk);
    return;
  }
  fnormalmove(pActwk);
  fslowmove(pActwk);
}

void flickydown(sprite_status_lpl* pActwk) {
  if (killchk(pActwk) != 0)
  {
    spr_kills(pActwk);
    return;
  }
  fnormalmove(pActwk);
  fdownmove(pActwk);
}

void fnormalmove(sprite_status_lpl* pActwk) {
  int_union lD0, lD3;

  if (pActwk->SPR_FLG & 2)
  {
    if (pActwk->Y_SPEED.l > 0) goto label1;
  }

pActwk->YPOSI.w.h -= pActwk->Y_OFFSET;

  lD3.w.l = sin_planet(pActwk->SIN_CNT);
  lD3.l = lD3.w.l << 2;
  lD3.l >>= 8;
  pActwk->Y_OFFSET = lD3.w.l;
  pActwk->YPOSI.w.h += lD3.w.l;

  lD0.l = random() & 32767;
  lD0.w.l = lD0.l % 48;
  pActwk->SIN_CNT += lD0.w.l;
  if (pActwk->SIN_CNT >= 511)
    pActwk->SIN_CNT -= 511;
label1:
  lD0.l = pActwk->X_SPEED.l;
  if (music_selflg != 0)
    lD0.l <<= 3;

  pActwk->XPOSI.l += lD0.l;
  lD0.l = pActwk->Y_SPEED.l;
  pActwk->YPOSI.l += lD0.l;
}

void fslowmove(sprite_status_lpl* pActwk) {
  Sint16 iD0;
  sprite_status_lpl* pActflicky;

if (chk_another(pActwk->ACT_NO, pActwk, &pActflicky) == 0) goto label1;

iD0 = pActwk->XPOSI.w.h - pActflicky->XPOSI.w.h;
  if (iD0 < 0)
    iD0 = -iD0;

  if (iD0 >= 72)
  {
label1:
    pActwk->PAT_ADR = &flicky_tbl2;
    pActwk->PAT_NO = 0;
    if (pActwk->X_SPEED.l < 0)
      pActwk->X_SPEED.l = -196608;
    else
      pActwk->X_SPEED.l = 196608;
  }
  else
    if (iD0 <= 32)
    {
      pActwk->PAT_ADR = &flicky_tbl1;
      pActwk->PAT_NO = 0;
      if (pActwk->X_SPEED.l < 0)
        pActwk->X_SPEED.l = -40960;
      else
        pActwk->X_SPEED.l = 40960;
    }
}

void fdownmove(sprite_status_lpl* pActwk) {
  Sint16 iD0;
  sprite_status_lpl* pActflicky;

if (chk_another(pActwk->ACT_NO, pActwk, &pActflicky) == 0) goto label1;

iD0 = pActwk->YPOSI.w.h - pActflicky->YPOSI.w.h;
  if (iD0 <= 0)
    iD0 = 0;

  if (iD0 >= 58)
  {
label1:
    pActwk->PAT_ADR = &flicky_tbl2;
    pActwk->PAT_NO = 0;
    pActwk->Y_SPEED.l = -45056;
    return;
  }
  if (iD0 <= 8)
  {
    pActwk->PAT_ADR = &flicky_tbl3;
    pActwk->PAT_NO = 0;
    pActwk->Y_SPEED.l = 57344;
  }
}

void star_ctrl(sprite_status_lpl* pActwk) {
  star_acttbl[pActwk->EXE_NO](pActwk);
}

void starinit(sprite_status_lpl* pActwk) {
  pActwk->CGBASE += 16384;
  pActwk->PAT_NO = 0;
  pActwk->PAT_ADR = &star_tbl;
  pActwk->SPR_TIMER = star_tbl.aPatDat[0].timer;

  pActwk->X_OFFSET = 0;
  pActwk->Y_OFFSET = 0;
  pActwk->EXE_NO = 1;
}

void starmove(sprite_status_lpl* pActwk) {
  int_union lD0;

  if (killchk(pActwk) != 0)
  {
    spr_kills(pActwk);
    return;
  }

  lD0.l = random() & 32767;
  lD0.w.l = lD0.l % 40;
  pActwk->SIN_CNT += lD0.w.l;
  if (pActwk->SIN_CNT >= 511)
    pActwk->SIN_CNT -= 511;
  pActwk->XPOSI.l += pActwk->X_SPEED.l;
  pActwk->YPOSI.l += pActwk->Y_SPEED.l;
}

void ufo_ctrl(sprite_status_lpl* pActwk) {
  ufo_acttbl[pActwk->EXE_NO](pActwk);
}

void ufoinit(sprite_status_lpl* pActwk) {
  int_union lD0, lD1;

  pActwk->CGBASE += 16384;
  pActwk->PAT_NO = 0;
  pActwk->PAT_ADR = &ufo_tbl;
  pActwk->SPR_TIMER = ufo_tbl.aPatDat[0].timer;

  pActwk->X_OFFSET = 0;
  pActwk->Y_OFFSET = 0;
  lD0.l = random() & 32767;
  lD1.l = lD0.l;
  lD0.w.l = lD0.l % 5 + 1;
  pActwk->timer1 = lD0.w.l;

lD1.w.l = lD1.l % 64;
  pActwk->timer2 = lD1.w.l;

  pActwk->EXE_NO = 2;
}

void ufomode(sprite_status_lpl* pActwk) {
  if (pActwk->timer1 == 3)
  {
    umset1(pActwk);
  }
  else
  {
    uendset(pActwk);
    umset2(pActwk);
  }
  if ((char)pActwk->timer1 >= 0)
    --pActwk->timer1;
}

void uendset(sprite_status_lpl* pActwk) {
  Sint16 iD1;
  int_union lD0;

  lD0.l = random() & 32767;
  lD0.w.l = lD0.l % 32 + 64;

  if (pActwk->SPR_FLG & 128)
    iD1 = pActwk->XPOSI.w.h - lD0.w.l;
  else
    iD1 = pActwk->XPOSI.w.h + lD0.w.l;
  pActwk->END_XPOSI = iD1;
}

void umset1(sprite_status_lpl* pActwk) {
  pActwk->EXE_NO = 2;
  pActwk->X_SPEED.l = 0;
  pActwk->Y_SPEED.l = 0;
  pActwk->PAT_ADR = &ufo_tbl;
  pActwk->PAT_NO = 0;
  pActwk->timer2 = 24;
}

void umset2(sprite_status_lpl* pActwk) {
  int_union lD0, lD1, lD7;

  pActwk->EXE_NO = 3;
  lD0.l = random() & 32767;
  lD1.l = lD0.l;
  lD0.w.h = lD0.l % 64;
  lD0.w.l = 0;
  lD0.l >>=4;
  lD0.l += 0x20000;
  pActwk->X_SPEED.l = lD0.l;

  if (!(pActwk->SPR_FLG & 8))
    pActwk->X_SPEED.l = -pActwk->X_SPEED.l;

  lD7.l = pActwk->Y_SPEED.l;
  lD1.w.h = lD1.l % 96;
  lD1.w.l = 0;
  lD1.l >>= 4;
  pActwk->Y_SPEED.l = lD1.l;
  pActwk->Y_SPEED.w.h += 5;

  lD0.l = random() & 32767;
  lD0.w.l = lD0.l % 56;

  if (lD7.l > 0)
  {

    pActwk->Y_SPEED.l = -pActwk->Y_SPEED.l;
    pActwk->END_YPOSI = lD0.w.l + 32;

    pActwk->PAT_ADR = &ufo_tbl2;
    pActwk->PAT_NO = 0;
  }
  else
  {

    pActwk->END_YPOSI = lD0.w.l + 160;

    pActwk->PAT_ADR = &ufo_tbl1;
    pActwk->PAT_NO = 0;
  }
}

void ufomove2(sprite_status_lpl* pActwk) {
  int_union lD0, lD1;

  if (killchk(pActwk) != 0)
  {
    spr_kills(pActwk);
    return;
  }
  lD0.l = pActwk->X_SPEED.l;
  if (music_selflg != 0)
    lD0.l <<= 3;

  pActwk->XPOSI.l += lD0.l;
  lD0.w.l = pActwk->XPOSI.w.h;

  if (pActwk->X_SPEED.l < 0)
  {
    if (lD0.w.l <= pActwk->END_XPOSI && (char)pActwk->timer1 >= 0)
      pActwk->EXE_NO = 1;
  }
  else
  {
    if (lD0.w.l >= pActwk->END_XPOSI && (char)pActwk->timer1 >= 0)
      pActwk->EXE_NO = 1;
  }

pActwk->YPOSI.l += pActwk->Y_SPEED.l;

lD1.w.l = pActwk->YPOSI.w.h;

  if (pActwk->Y_SPEED.l < 0)
  {
    if (lD1.w.l <= pActwk->END_YPOSI && (char)pActwk->timer1 >= 0)
      pActwk->EXE_NO = 1;
  }
  else
  {
    if (lD1.w.l >= pActwk->END_YPOSI && (char)pActwk->timer1 >= 0)
      pActwk->EXE_NO = 1;
  }
}

void ufomove1(sprite_status_lpl* pActwk) {
  if (pActwk->X_SPEED.l == 0)
    pActwk->X_WIDE = 72;
  else
    pActwk->X_WIDE = 40;

  sinmove(pActwk);

  --pActwk->timer2;
  if (pActwk->timer2 <= 0)
    pActwk->EXE_NO = 1;
}

void eggm_ctrl(sprite_status_lpl* pActwk) {
  eggm_acttbl[pActwk->EXE_NO](pActwk);
}

void eggminit(sprite_status_lpl* pActwk) {
  pActwk->CGBASE += 16384;
  pActwk->PAT_NO = 0;
  pActwk->PAT_ADR = &eggm_tbl;
  pActwk->SPR_TIMER = eggm_tbl.aPatDat[0].timer;

  pActwk->X_OFFSET = 0;
  pActwk->Y_OFFSET = 0;
  pActwk->timer1 = 3;
  pActwk->EXE_NO = 1;
}

void eggmmove(sprite_status_lpl* pActwk) {
  int_union lD0, lD1;

  if (pActwk->timer1 > 0)
  {
    if (--pActwk->timer1 > 0)
    {
      sinmove(pActwk);
      return;
    }
    pActwk->PAT_ADR = &eggm_tbl;
  }

  lD0.l = random() & 32767;
  lD1.l = lD0.l;
  lD0.w.l = lD0.l % 16;
  if (lD0.w.l == 0)
  {
    lD1.w.l = lD1.l % 16 + 32;
    pActwk->timer1 = lD1.w.l;
    pActwk->PAT_ADR = &eggm_tbl2;
  }

  sinmove(pActwk);
}

void msnc_ctrl(sprite_status_lpl* pActwk) {
  msnc_acttbl[pActwk->EXE_NO](pActwk);
}

void msncinit(sprite_status_lpl* pActwk) {
  pActwk->CGBASE += 16384;
  pActwk->PAT_NO = 0;
  pActwk->PAT_ADR = &msnc_tbl;
  pActwk->SPR_TIMER = msnc_tbl.aPatDat[0].timer;
  pActwk->EXE_NO = 1;
}

void msncstart(sprite_status_lpl* pActwk) {
  int_union lD0, lD1;

  if (killchk(pActwk) != 0)
  {
    spr_kill(pActwk);
    return;
  }
  lD0.l = pActwk->X_SPEED.l;
  lD1.l = pActwk->Y_SPEED.l;
  if (music_selflg != 0)
  {
    pActwk->X_SPEED.l = 1572864;
    pActwk->Y_SPEED.l = 0;
    if (pActwk->SPR_FLG & 128)
      pActwk->X_SPEED.l = -pActwk->X_SPEED.l;
    pActwk->EXE_NO = 4;
    return;
  }

  pActwk->XPOSI.l += lD0.l;
  pActwk->YPOSI.l += lD1.l;
  if (pActwk->SPR_FLG & 128)
  {
    if (pActwk->XPOSI.w.h <= 128)
    {
      pActwk->XPOSI.w.h = 128;
      pActwk->X_SPEED.l = 0;
    }
  }
  else
  {

    if (pActwk->XPOSI.w.h >= 128)
    {
      pActwk->XPOSI.w.h = 128;
      pActwk->X_SPEED.l = 0;
    }
  }

  if (!(pActwk->SPR_FLG & 8))
  {
    if (pActwk->YPOSI.w.h <= 100)
    {
      pActwk->YPOSI.w.h = 100;
      pActwk->Y_SPEED.l = 0;
    }
  }
  else
  {
    if (pActwk->YPOSI.w.h >= 100)
    {
      pActwk->YPOSI.w.h = 100;
      pActwk->Y_SPEED.l = 0;
    }
  }

  if (pActwk->XPOSI.w.h != 128) return;
  if (pActwk->YPOSI.w.h != 100) return;
  pActwk->timer1 = 64;
  pActwk->timer2 = 8;
  pActwk->EXE_NO = 2;
}

void msncfloat(sprite_status_lpl* pActwk) {
  int_union lD0;

  if (music_selflg == 0)
  {
    if ((char)pActwk->timer1 < 0)
    {
      sinmove(pActwk);
      return;
    }
    if ((char)pActwk->timer2 == 0)
    {
      pActwk->timer2 = 24;
      lD0.l = (random() & 1);
      if (lD0.l == 0) pActwk->SPR_FLG &= 127;
      else pActwk->SPR_FLG |= 128;
    }
    --pActwk->timer2;
    if ((char)--pActwk->timer1 >= 0)
    {
      sinmove(pActwk);
      return;
    }
  }

  pActwk->timer1 = 48;
  pActwk->X_WIDE = 256;
  pActwk->Y_WIDE = 2;
  pActwk->PAT_ADR = &msnc_tbl2;
  pActwk->PAT_NO = 0;
  pActwk->X_SPEED.l = -65536;
  if (pActwk->SPR_FLG & 128)
    pActwk->X_SPEED.l = -pActwk->X_SPEED.l;

  pActwk->EXE_NO = 3;
}

void msncdush(sprite_status_lpl* pActwk) {
  if (music_selflg == 0)
  {
    if ((char)--pActwk->timer1 > 0)
    {
      sinmove(pActwk);
      return;
    }
  }

  pActwk->X_WIDE = 0;
  pActwk->Y_WIDE = 0;
  pActwk->X_SPEED.l = 1572864;
  if (pActwk->SPR_FLG & 128)
    pActwk->X_SPEED.l = -pActwk->X_SPEED.l;
  pActwk->EXE_NO = 4;
}

void msncexit(sprite_status_lpl* pActwk) {
  int_union lD0, lD1;

  if (killchk(pActwk) != 0)
  {
    spr_kill(pActwk);
    return;
  }
  lD0.l = pActwk->X_SPEED.l;
  lD1.l = pActwk->Y_SPEED.l;
  if (music_selflg != 0)
  {
    lD0.l *= 2;
    lD1.l *= 2;
  }
  pActwk->XPOSI.l += lD0.l;
  pActwk->YPOSI.l += lD1.l;
}

void tails_ctrl(sprite_status_lpl* pActwk) {
  tails_acttbl[pActwk->EXE_NO](pActwk);
}

void tailsinit(sprite_status_lpl* pActwk) {
  int_union lD0, lD1;

  pActwk->CGBASE += 16384;
  pActwk->PAT_NO = 0;
  pActwk->PAT_ADR = &tails_tbl;
  pActwk->SPR_TIMER = tails_tbl.aPatDat[0].timer;

  lD0.l = random() & 32767;
  lD1.l = lD0.l;
  lD0.w.l = lD0.l % 5 + 1;
  pActwk->timer1 = lD0.w.l;
  lD1.w.l = lD1.l % 48;
  pActwk->timer2 = lD1.w.l;
  pActwk->EXE_NO = 2;
}

void tailsmove(sprite_status_lpl* pActwk) {
  uendset(pActwk);
  if (pActwk->timer1 == 3)
    tsset1(pActwk);
  else
    tsset2(pActwk);
  if ((char)pActwk->timer1 >= 0)
    --pActwk->timer1;
}

void tsset1(sprite_status_lpl* pActwk) {
  pActwk->X_SPEED.l = 0x20000;
  pActwk->Y_SPEED.l = 0;
  pActwk->PAT_NO = 0;
  pActwk->PAT_ADR = &tails_tbl;
  if (pActwk->SPR_FLG & 128)
  {
    pActwk->X_SPEED.l = -pActwk->X_SPEED.l;
    pActwk->Y_SPEED.l = -pActwk->Y_SPEED.l;
  }

  pActwk->timer2 = 24;
  pActwk->EXE_NO = 2;
}

void tsset2(sprite_status_lpl* pActwk) {
  int_union lD0, lD1, lD7;

  pActwk->EXE_NO = 3;
  lD0.l = random() & 32767;
  lD1.l = lD0.l;
  lD0.w.h = lD0.l % 16;
  lD0.w.l = 0;
  lD0.l >>= 4;
  lD0.l += 0x20000;
  pActwk->X_SPEED.l = lD0.l;
  if (pActwk->SPR_FLG & 128)
    pActwk->X_SPEED.l = -pActwk->X_SPEED.l;

  lD7.l = pActwk->Y_SPEED.l;
  lD1.w.h = lD1.l % 40;
  lD1.w.l = 0;
  lD1.l >>= 4;
  pActwk->Y_SPEED.l = lD1.l;
  ++pActwk->Y_SPEED.w.h;

  lD0.l = random() & 32767;
  lD0.w.l = lD0.l % 56;

  if (lD7.l == 0)
  {
    if (pActwk->YPOSI.w.h > 100) goto label1; else goto label2;
  }

  if (lD7.l > 0)
  {
label1:
    pActwk->Y_SPEED.l = -pActwk->Y_SPEED.l;
    pActwk->END_YPOSI = lD0.w.l + 32;
    pActwk->PAT_ADR = &tails_down_tbl;
    pActwk->PAT_NO = 0;
    return;
  }

label2:
  pActwk->END_YPOSI = lD0.w.l + 160;
  pActwk->PAT_ADR = &tails_up_tbl;
  pActwk->PAT_NO = 0;

}

void tailsmove1(sprite_status_lpl* pActwk) {
  sinmove(pActwk);
  if ((char)--pActwk->timer2 <= 0)
    pActwk->EXE_NO = 1;
}
