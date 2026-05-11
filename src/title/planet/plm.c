#include "../../types.h"
#include "lpl_types.h"
#include "spm_equ.h"
#include "plm.h"
#include "actm.h"
#include "etc.h"

static Sint16 fpat0[8] = { 0, 0, -24, 8, -48, -8, -34, 20 };
static Sint16 fpat1[8] = { 0, 0, -16, -24, -16, 24, -32, 0 };
static Sint16 fpat2[12] = { 0, 0, -16, -22, -40, -28, -32, 16, -16, 24, -48, -5 };
static ini_pats flick_ini_tbl[4] =
{
  { 4, fpat0 },
  { 4, fpat1 },
  { 6, fpat2 },
  { 6, fpat2 }
};
static Sint16 spat0[6] = { 0, 0, 32, 0, -32, 0 };
static ini_pats star_ini_tbl[5] =
{
  { 3, spat0 },
  { 3, spat0 },
  { 2, spat0 },
  { 1, spat0 },
  { 3, spat0 }
};
static Sint16 upat0[4] = { 0, 0, 16, 48 };
static ini_pats ufo_ini_tbl[5] =
{
  { 1, upat0 },
  { 2, upat0 },
  { 1, upat0 },
  { 2, upat0 },
  { 1, upat0 }
};
void(*sprinit_tbl[6])(void) =
{
  &flick_set,
  &star_set,
  &ufo_set,
  &eggman_set,
  &metals_set,
  &tails_set
};
Sint16 actset_tbl[12] =
{
  300, 600,
  360, 660,
  420, 720,
  480, 780,
  540, 840,
  600, 900
};
extern sprite_patterns_title flicky_tbl;
extern sprite_patterns_title flicky_tbl1;
extern sprite_patterns_title flicky_tbl3;

void sprinit(void) {
  int_union lD0;
  Sint16 i, iD0, iD5;

  if (!(music_selflg & 1))
  {

    for (i = 0; i < 6; ++i)
    {
      if (--actset_tm[i] < 0)
      {
        iD5 = 5 - i;
        iD0 = iD5;
        iD0 += iD0;
        actset_tm[i] = actset_tbl[iD0];

        lD0.l = random() & 65535;
        lD0.w.l = lD0.l % actset_tbl[iD0 + 1];
        actset_tm[i] += lD0.w.l;

        sprinit_tbl[iD5]();
        break;
      }
    }
  }

}

void flick_set(void) {
  Sint16 i;
  Sint16 iD3, iD5, iD6, iD7;
  sprite_status_lpl* pActwk;
  int_union lD0;
  ini_pats* pFoffset;

  if (!(actset_flg & 1))
  {

    actset_flg |= 1;
    actset_wk = 1;
  }
  else if (!(actset_flg & 2))
  {

    actset_flg |= 2;
    actset_wk = 2;
  }
  else
    return;

  lD0.l = random();
  iD7 = lD0.w.l;

  if (iD7 < 0)
    iD5 = 328;
  else
    iD5 = -6;

  lD0.l = random() & 32767;
  lD0.w.l = lD0.l % 160;
  iD6 = lD0.w.l + 16;

lD0.l = iD7 & 32767;
  lD0.w.l = lD0.l % 4;
  pFoffset = &flick_ini_tbl[lD0.w.l];

  if (actset_wk & 1)
    iD3 = 0;
  else
    iD3 = 1;

  for (i = 0; i < flick_ini_tbl[lD0.w.l].number; ++i)
  {
    if (get_actwk(&pActwk) != 0) break;

f_actset(pActwk, pFoffset, iD5, iD6, iD7, i, flick_ini_tbl[lD0.w.l].number - 1);
  }
  actset_wk = 0;
}

void f_actset(sprite_status_lpl* pActwk, ini_pats* pFoffset, Sint16 initx, Sint16 inity, Sint16 iReverse, Sint16 iNormal, Sint16 max) {
  int_union lD0;
  Sint16 iD0;

  pActwk->CGBASE = 0;
  pActwk->ACT_FLG = actset_wk;
  pActwk->XPOSI.w.h = initx;
  pActwk->YPOSI.w.h = inity;
  pActwk->X_WIDE = 48;
  pActwk->Y_WIDE = 4;

if (iNormal != max) goto label1; // 76a8

  lD0.l = random() & 32767;
  lD0.w.l = lD0.l % 3;
  if (lD0.w.l != 0) // 76a8
  {
    if (lD0.w.l != 1) // 766c
    {

      pActwk->SPR_FLG |= 2;
      pActwk->X_SPEED.l = 81920;
      pActwk->Y_SPEED.l = 57344;
      pActwk->PAT_ADR = &flicky_tbl3;
    } // 76d0
    else
    {

      pActwk->SPR_FLG |= 1;
      pActwk->X_SPEED.l = 40960;
      pActwk->Y_SPEED.l = 0;
      pActwk->PAT_ADR = &flicky_tbl1;
    }
  } // 76d0
  else
  {
label1:
    pActwk->X_SPEED.l = 81920;
    pActwk->Y_SPEED.l = 0;
    pActwk->PAT_ADR = &flicky_tbl;
  }

  pActwk->SPR_TIMER = pActwk->PAT_ADR->aPatDat[0].timer;

  pActwk->SPR_FLG |= 8;
  pActwk->ACT_NO = 1;
  iD0 = pFoffset->inipat[iNormal * 2];
  if (iReverse < 0)
  {
    pActwk->SPR_FLG |= 128;
    pActwk->SPR_FLG &= 247;
    iD0 = -iD0;
    pActwk->X_SPEED.l = -pActwk->X_SPEED.l;
  }
  pActwk->XPOSI.w.h += iD0;
  pActwk->YPOSI.w.h += pFoffset->inipat[iNormal * 2 + 1];
}

void star_set(void) {
  Sint16 i, iD1, iD3, iD5, iD6, iD7;
  int_union lD0;
  sprite_status_lpl* pActwk;
  ini_pats* pSoffset;

  if (colorno <= 10) return;
  if (colorno >= 24) return;

if (!(actset_flg & 1))
  {

    actset_flg |= 1;
    actset_wk = 1;
  }
  else if (!(actset_flg & 2))
  {

    actset_flg |= 2;
    actset_wk = 2;
  }
  else
    return;

  lD0.l = random();
  iD7 = lD0.w.l;
  iD3 = 0;

  lD0.l = random() & 32767;
  lD0.w.l = lD0.l % 128;
  iD5 = lD0.w.l;

  if (iD5 <= 128)
    iD3 = 1;

  iD6 = 0;
  if (actset_wk & 1)
    iD1 = 0;
  else
    iD1 = 1;

  lD0.l = iD7 & 32767;
  lD0.w.l = lD0.l % 5;
  pSoffset = &star_ini_tbl[lD0.w.l];

for (i = 0; i < star_ini_tbl[lD0.w.l].number; ++i)
  {
    if (get_actwk(&pActwk) != 0) break;

    s_actset(pActwk, pSoffset, iD3, iD5, iD6, i);
  }
  actset_wk = 0;
}

void s_actset(sprite_status_lpl* pActwk, ini_pats* pSoffset, Sint16 iD3, Sint16 iD5, Sint16 iD6, Sint16 i) {
  int_union lD0;

  pActwk->ACT_NO = 2;
  pActwk->CGBASE = 0;
  pActwk->ACT_FLG = actset_wk;

  pActwk->XPOSI.w.h = iD5;
  pActwk->YPOSI.w.h = iD6;

  pActwk->X_SPEED.l = 0x20000;
  if (iD3 != 0)
    pActwk->X_SPEED.l = -0x20000;
  lD0.l = random() & 32767;
  lD0.w.h = lD0.l % 128;
  lD0.l &= 2147418112;
  lD0.l >>= 4;
  pActwk->Y_SPEED.l = lD0.l + 0x40000;

  lD0.l = random() & 32767;

lD0.w.l = lD0.l % 240;

  pActwk->XPOSI.w.h += lD0.w.l;

pActwk->YPOSI.w.h += pSoffset->inipat[i];
}

void ufo_set(void) {
  sprite_status_lpl* pActwk;
  ini_pats* pUoffset;
  int_union lD0;
  Sint16 i;
  Sint16 iD1, iD5, iD6, iD7;
  Sint16 iNum;

  if (!(actset_flg & 1))
  {

    actset_flg |= 1;
    actset_wk = 1;
  }
  else if (!(actset_flg & 2))
  {

    actset_flg |= 2;
    actset_wk = 2;
  }
  else
    return;

  lD0.l = random();
  iD7 = lD0.w.l;

if (iD7 < 0)
    iD5 = 328;
  else
    iD5 = -6;

  lD0.l = random() & 32767;
  lD0.w.l = lD0.l % 160;
  iD6 = lD0.w.l + 16;

  if (actset_wk & 1)
    iD1 = 0;
  else
    iD1 = 1;

  lD0.l = iD7 & 32767;
  lD0.w.l = lD0.l % 5;

  pUoffset = &ufo_ini_tbl[lD0.w.l];
  iNum = pUoffset->number;

  for (i = 0; i < iNum; ++i)
  {
    if (get_actwk(&pActwk) != 0) break;

    pActwk->CGBASE = 0;
    pActwk->ACT_FLG = actset_wk;
    pActwk->XPOSI.w.h = iD5;
    pActwk->YPOSI.w.h = iD6;

    pActwk->X_WIDE = 40;
    pActwk->Y_WIDE = 6;

    pActwk->X_SPEED.l = 0x20000;
    pActwk->Y_SPEED.l = 0;
    pActwk->SPR_FLG |= 8;

    pActwk->ACT_NO = 4;
    lD0.w.l = pUoffset->inipat[i * 2];
    if (iD7 < 0)
    {
      pActwk->SPR_FLG |= 128;
      pActwk->SPR_FLG &= 247;
      lD0.w.l = -lD0.w.l;
      pActwk->X_SPEED.l = -pActwk->X_SPEED.l;
      pActwk->Y_SPEED.l = -pActwk->Y_SPEED.l;
    }

    pActwk->XPOSI.w.h += lD0.w.l;
    pActwk->YPOSI.w.h += pUoffset->inipat[i * 2 + 1];
  }
  actset_wk = 0;
}

void eggman_set(void) {
  sprite_status_lpl* pActwk;
  int_union lD0;
  Sint16 iD5, iD6;
  Sint16 iD7;

  if (actset_flg & 3) return;
  actset_flg |= 3;
  actset_wk = 3;

  iD7 = random();

if (iD7 < 0)
    iD5 = -6;
  else
    iD5 = 328;
  lD0.l = random() & 32767;
  iD6 = lD0.l % 64;
  iD6 += 128;

if (get_actwk(&pActwk) == 0)
  {
    pActwk->CGBASE = 0;
    pActwk->ACT_FLG = actset_wk;
    pActwk->XPOSI.w.h = iD5;
    pActwk->YPOSI.w.h = iD6;
    pActwk->X_WIDE = 40;
    pActwk->Y_WIDE = 10;
    pActwk->X_SPEED.l = -81920;
    pActwk->Y_SPEED.l = -32768;

    pActwk->ACT_NO = 3;
    if (iD7 < 0)
    {
      pActwk->SPR_FLG |= 128;
      pActwk->X_SPEED.l *= -1;
    }
  }
  actset_wk = 0;
}

void metals_set(void) {
  sprite_status_lpl* pActwk;
  int_union lD0;
  Sint16 iD5, iD6;
  Sint16 iD7;

  if (actset_flg & 3) return;
  actset_flg |= 3;
  actset_wk = 3;

  iD7 = random();

lD0.l = random() & 32767;
  iD5 = lD0.l % 256;

  if (iD7 < 0)
    iD6 = 0;
  else
    iD6 = 192;

if (get_actwk(&pActwk) == 0)
  {
    pActwk->CGBASE = 0;
    pActwk->ACT_FLG = actset_wk;
    pActwk->XPOSI.w.h = iD5;
    pActwk->YPOSI.w.h = iD6;
    pActwk->X_WIDE = 40;
    pActwk->Y_WIDE = 4;
    pActwk->X_SPEED.l = 0x40000;
    pActwk->Y_SPEED.l = 327680;
    pActwk->SPR_FLG |= 8;

    pActwk->ACT_NO = 5;

    if (pActwk->XPOSI.w.h >= 128)
    {
      pActwk->SPR_FLG |= 128;
      pActwk->X_SPEED.l = -pActwk->X_SPEED.l;
    }

    if (pActwk->YPOSI.w.h >= 100)
    {
      pActwk->SPR_FLG &= 247;
      pActwk->Y_SPEED.l = -pActwk->Y_SPEED.l;
    }
  }
  actset_wk = 0;
}

void tails_set(void) {
  sprite_status_lpl* pActwk;
  int_union lD0;
  Sint16 iD5, iD6;
  Sint16 iD7;

  if (actset_flg & 3) return;
  actset_flg |= 3;
  actset_wk = 3;

  iD7 = random();

if (iD7 < 0)
    iD5 = 328;
  else
    iD5 = -6;

  lD0.l = random() & 32767;
  iD6 = lD0.l % 160;
  iD6 += 32;

if (get_actwk(&pActwk) == 0)
  {
    pActwk->CGBASE = 0;
    pActwk->ACT_FLG = actset_wk;
    pActwk->XPOSI.w.h = iD5;
    pActwk->YPOSI.w.h = iD6;
    pActwk->X_WIDE = 36;
    pActwk->Y_WIDE = 2;
    pActwk->X_SPEED.l = 0x20000;
    pActwk->Y_SPEED.l = 0;

    pActwk->ACT_NO = 6;
    if (iD7 < 0)
    {
      pActwk->SPR_FLG |= 128;
      pActwk->X_SPEED.l = -pActwk->X_SPEED.l;
      pActwk->Y_SPEED.l = -pActwk->Y_SPEED.l;
    }
  }
  actset_wk = 0;
}
