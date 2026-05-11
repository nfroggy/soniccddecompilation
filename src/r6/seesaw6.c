#include "../equ.h"
#include "seesaw6.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../playsub.h"
#include "../ridechk.h"

#if defined(R61B) || defined(R62B)
  #define SPRITE_SEESAW6_BASE 481
#elif defined(R63C) || defined(R63D)
  #define SPRITE_SEESAW6_BASE 450
#else
  #define SPRITE_SEESAW6_BASE 487
#endif

void(*seesaw6_tbl[3])(sprite_status*) = {
  &m_init,
  &m_move,
  &m_up
};
Uint8 seesaw_pat_00[4] = { 2, 0, 1, 255 };
Uint8 seesaw_pat_01[5] = { 2, 1, 2, 3, 255 };
Uint8 seesaw_pat_02[4] = { 2, 2, 3, 255 };
Uint8 seesaw_pat_03[4] = { 2, 4, 5, 255 };
Uint8 seesaw_pat_04[6] = { 2, 4, 5, 6, 7, 255 };
Uint8 seesaw_pat_05[4] = { 2, 6, 7, 255 };
Uint8* seesaw_pchg[6] =
{
  seesaw_pat_00,
  seesaw_pat_01,
  seesaw_pat_02,
  seesaw_pat_03,
  seesaw_pat_04,
  seesaw_pat_05
};
sprite_pattern seesawPat00 = { 1, { { -24, -24, 0, SPRITE_SEESAW6_BASE } } };
sprite_pattern seesawPat01 = { 1, { { -24, -24, 0, SPRITE_SEESAW6_BASE + 1 } } };
sprite_pattern seesawPat02 = { 1, { { -24, -24, 0, SPRITE_SEESAW6_BASE + 2 } } };
sprite_pattern seesawPat03 = { 1, { { -24, -24, 0, SPRITE_SEESAW6_BASE + 3 } } };
sprite_pattern seesawPat04 = { 1, { { -24, -24, 0, SPRITE_SEESAW6_BASE + 4 } } };
sprite_pattern seesawPat05 = { 1, { { -24, -24, 0, SPRITE_SEESAW6_BASE + 5 } } };
sprite_pattern seesawPat06 = { 1, { { -24, -24, 0, SPRITE_SEESAW6_BASE + 6 } } };
sprite_pattern seesawPat07 = { 1, { { -24, -24, 0, SPRITE_SEESAW6_BASE + 7 } } };
sprite_pattern seesawPat08 = { 1, { { -24, -8, 0, SPRITE_SEESAW6_BASE + 8 } } };
sprite_pattern seesawPat09 = { 1, { { -16, -7, 0, SPRITE_SEESAW6_BASE + 9 } } };
sprite_pattern* pat_seesaw6[10] = {
  &seesawPat00,
  &seesawPat01,
  &seesawPat02,
  &seesawPat03,
  &seesawPat04,
  &seesawPat05,
  &seesawPat06,
  &seesawPat07,
  &seesawPat08,
  &seesawPat09
};

void seesaw6(sprite_status* pActwk) {
  if (pActwk->userflag.b.h != 0)
  {
    slave(pActwk);
  }
  else
  {

    seesaw6_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s(pActwk);
  }
}

void m_init(sprite_status* pActwk) {
  sprite_status* pActfree;

  pActwk->r_no0 += 2;
  pActwk->actflg |= 4;
  pActwk->sprpri = 3;
  pActwk->sprhs = 24;
  pActwk->sprhsize = 24;
  pActwk->sprvsize = 24;
  pActwk->sproffset = 952;
  pActwk->patbase = pat_seesaw6;

  if (actwkchk(&pActfree) != 0)
  {
    frameout(pActwk);
    return;
  }
  m_ini_s(pActwk, pActfree);

  ((Uint16*)pActwk)[23] = pActfree - actwk;
  pActfree->xposi.w.h -= 40;
  pActfree->yposi.w.h -= 24;
  if (actwkchk(&pActfree) != 0)
  {
    frameout(pActwk);
    return;
  }
  m_ini_s(pActwk, pActfree);

  ((Uint16*)pActwk)[24] = pActfree - actwk;
  pActfree->xposi.w.h += 40;
  pActfree->yposi.w.h += 24;
  pActfree->actflg |= 1;
  pActfree->cddat |= 1;
}

void m_ini_s(sprite_status* pActwk, sprite_status* pActfree) {
  pActfree->actno = pActwk->actno;
  pActfree->actflg = pActwk->actflg;
  pActfree->sprpri = pActwk->sprpri;
  pActfree->sproffset = pActwk->sproffset;
  pActfree->patbase = pActwk->patbase;
  pActfree->xposi.w.h = pActwk->xposi.w.h;
  pActfree->yposi.w.h = pActwk->yposi.w.h;
  pActfree->userflag.b.h = -1;
  pActfree->sprhs = 16;
  pActfree->sprhsize = 16;
  pActfree->sprvsize = 8;
  pActfree->patno = 9;

  ((Uint16*)pActfree)[23] = pActwk - actwk;
  ((Sint16*)pActwk)[25] = 120;
}

void m_move(sprite_status* pActwk) {
  Uint16 wActwkNo;

  if (pActwk->yspeed.w == 0)
    m_stay(pActwk);
  else
    m_fall(pActwk);

  wActwkNo = ((Uint16*)pActwk)[24];
  ridechk(&actwk[wActwkNo], &actwk[0]);
  actionsub(&actwk[wActwkNo]);

  wActwkNo = ((Uint16*)pActwk)[23];
  if (ridechk(&actwk[wActwkNo], &actwk[0]) != 0)
    actwk[wActwkNo].actfree[21] = 255;
  else
    actwk[wActwkNo].actfree[21] = 0;
  actionsub(&actwk[wActwkNo]);

  if (actwk[wActwkNo].actfree[21] == 0)
  {
    patchg(pActwk, seesaw_pchg);
  }
  else
  {
    pActwk->r_no0 = 4;
    ((Sint16*)pActwk)[25] = 3;
    pActwk->patno = 8;
  }
}

void m_stay(sprite_status* pActwk) {
  Sint16 iwk;

  iwk = ((Sint16*)pActwk)[25];
  if (((Sint16*)pActwk)[25] >= 0)
  {

    --((Sint16*)pActwk)[25];
    if (((Sint16*)pActwk)[25] < 0)
    {
      pActwk->yspeed.w = 256;
    }
    else
    {
      if (((Sint16*)pActwk)[25] != 60) return;
    }

    ++pActwk->mstno.b.h;
  }
}

void m_fall(sprite_status* pActwk) {
  sprite_status* pActSu;
  sprite_status* pActSd;
  Uint16 wYspd;
  Sint16 iD0, iD1;

  pActSu = &actwk[((Uint16*)pActwk)[23]];
  pActSd = &actwk[((Uint16*)pActwk)[24]];

  wYspd = pActwk->yspeed.b.h;
  pActwk->yposi.w.h += wYspd;
  pActSu->yposi.w.h += wYspd;
  pActSd->yposi.w.h += wYspd;

iD0 = pActSd->sprhsize;
  if (pActSd->xposi.w.h >= pActwk->xposi.w.h)
    iD0 = -iD0;

  iD0 += pActwk->xposi.w.h;
  iD1 = emycol_d2(pActSd, iD0);
  if (iD1 > 0) return;

pActwk->yposi.w.h += iD1;
  pActSu->yposi.w.h += iD1;
  pActSd->yposi.w.h += iD1;
  pActwk->yspeed.w = 0;
}

void m_up(sprite_status* pActwk) {
  sprite_status *pActSd, *pActSu;
  Uint16 wwk_u, wwk_d;
  pActSd = &actwk[((Uint16*)pActwk)[24]];
  pActSd->yposi.w.h -= 24;
  pActwk->yposi.w.h -= 12;
  --((Sint16*)pActwk)[25];
  if (((Sint16*)pActwk)[25] < 0)
  {

pActwk->r_no0 = 2;
    pActwk->yspeed.w = 0;
    ((Sint16*)pActwk)[25] = 120;
    wwk_d = ((Uint16*)pActwk)[24];
    wwk_u = ((Uint16*)pActwk)[23];
    ((Uint16*)pActwk)[24] = wwk_u;
    ((Uint16*)pActwk)[23] = wwk_d;
    if (pActwk->mstno.b.h > 2) pActwk->mstno.b.h = 0;
    else pActwk->mstno.b.h = 3;
    pActwk->mstno.b.l = -1;
  }

  actionsub(pActSd);
  pActSu = &actwk[((Uint16*)pActwk)[23]];
  actionsub(pActSu);
}

void slave(sprite_status* pActwk) {
  sprite_status* pActwk2;

  pActwk2 = &actwk[((Uint16*)pActwk)[23]];
  if (pActwk2->actno != 44)
    frameout(pActwk);
}
