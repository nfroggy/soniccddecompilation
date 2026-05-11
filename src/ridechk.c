#include "equ.h"
#include "ridechk.h"
#include "dircol.h"
#include "loader2.h"

extern Sint16 playdieset(sprite_status* pActwk);

Sint16 ride_on_clr(sprite_status* pActwk, sprite_status* pPlayerwk) {
  Uint16 wActwkNo;
  Uint8 byPlwk;

  if (!(pActwk->cddat & 8))
    return -1;
  if (!(pPlayerwk->cddat & 8))
    return -1;

  wActwkNo = pPlayerwk->actfree[19];

  if ((Uint16)(pActwk - actwk) != wActwkNo)
    return -1;

  if (pPlayerwk->actfree[0] != 0)
    soundset(171);

  pPlayerwk->actfree[14] = 0;
  pPlayerwk->cddat |= 2;
  pPlayerwk->cddat &= 247;
  pActwk->cddat &= 247;
  byPlwk = pPlayerwk->actfree[2];
  if (!(byPlwk & 64))
    if (pPlayerwk->mstno.b.h != 23)
      byPlwk &= 254;

  pPlayerwk->actfree[2] = byPlwk;
  pPlayerwk->actfree[19] = 0;
  if (pPlayerwk->mstno.b.h == 43)
    pPlayerwk->cddat &= 253;
  return 0;
}

Sint16 ride_on_set(sprite_status* pActwk, sprite_status* pPlayerwk) {
  Uint8 byCCR;
  Uint16 wActwkNo;
  sprite_status* pRideAct;

  if (pPlayerwk->r_no0 == 4)
  {
    pPlayerwk->r_no0 -= 2;
    ((Sint16*)pPlayerwk)[26] = 120;
  }

  pActwk->r_no1 = 0;
  pPlayerwk->actfree[18] = 0;

  if (!(pActwk->cddat & 8)) byCCR = 0;
  else byCCR = 1;
  pActwk->cddat |= 8;
  if (byCCR == 0)
  {
    if (pPlayerwk->mstno.b.h == 43)
    {
      pActwk->cddat &= 247;
      return ride_on_clr(pActwk, pPlayerwk);
    }

pPlayerwk->cddat &= 239;
    if (!(pPlayerwk->cddat & 4)) byCCR = 0;
    else byCCR = 1;
    pPlayerwk->cddat &= 251;
    if (byCCR == 1)
    {
      if (chibi_flag != 0)
      {
        pPlayerwk->sprvsize = 10;
        pPlayerwk->sprhs = 5;
        pPlayerwk->yposi.w.h -= 2;
      }
      else
      {

        pPlayerwk->sprvsize = 19;
        pPlayerwk->sprhs = 9;
        pPlayerwk->yposi.w.h -= 5;
      }

      pPlayerwk->mstno.b.h = 0;
    }
  }

if (!(pPlayerwk->cddat & 8)) byCCR = 0;
  else byCCR = 1;
  pPlayerwk->cddat |= 8;
  if (byCCR == 1)
  {
    wActwkNo = pPlayerwk->actfree[19];
    if ((Uint16)(pActwk - actwk) == wActwkNo)
      return -1;
    pRideAct = &actwk[wActwkNo];
    pRideAct->cddat &= 247;
  }

  pPlayerwk->actfree[19] = pActwk - actwk;
  pPlayerwk->direc.w = 0;
  pPlayerwk->yspeed.w = 0;

  if (pActwk->actno != 10 || pActwk->r_no0 != 2)
    pPlayerwk->mspeed.w = pPlayerwk->xspeed.w;

  pPlayerwk->cddat &= 253;
  return 0;
}

Sint16 hitchk_u(sprite_status* pActwk, sprite_status* pPlayerwk) {
  pActwk->r_no1 = 2;
  return ride_on_chk(pActwk, pPlayerwk);
}

Sint16 ridechk(sprite_status* pActwk, sprite_status* pPlayerwk) {
  pActwk->r_no1 = 1;
  return ride_on_chk(pActwk, pPlayerwk);
}

Sint16 ride_on_chk(sprite_status* pActwk, sprite_status* pPlayerwk) {
  return hitchk(pActwk, pPlayerwk);
}

Sint16 hitchk(sprite_status* pActwk, sprite_status* pPlayerwk) {
  Sint16 iD0, iD1, iD2;
  Uint8 byPlflg;

  if (pPlayerwk->mstno.b.h == 23)
    return hit_e(pActwk, pPlayerwk);
  byPlflg = pPlayerwk->actfree[2];
  if (byPlflg & 64)
    return hit_e(pActwk, pPlayerwk);
  if (pPlayerwk->r_no0 >= 6)
    return hit_e(pActwk, pPlayerwk);
  if (pPlayerwk->actno == 0)
    return hit_e(pActwk, pPlayerwk);
  if (!(pActwk->actflg & 128))
    return hit_e(pActwk, pPlayerwk);
  if (editmode.b.h != 0)
    return hit_e(pActwk, pPlayerwk);

  iD1 = (Sint16)pActwk->sprhsize + 10;
  iD0 = pPlayerwk->xposi.w.h - pActwk->xposi.w.h;
  iD0 += iD1;
  if (iD0 < 0)
    return hit_e(pActwk, pPlayerwk);

  iD2 = iD1;
  iD2 += iD2;
  if ((Uint16)iD0 >= (Uint16)iD2)
    return hit_e(pActwk, pPlayerwk);

  if (pPlayerwk->mstno.b.h == 43)
  {
    if (pActwk->cddat & 8)
      return hit_v(pActwk, pPlayerwk, iD0, iD1);
    else
      return hit_e(pActwk, pPlayerwk);
  }

  if (pActwk->r_no1 != 1)
    return hit_v(pActwk, pPlayerwk, iD0, iD1);
  if (pPlayerwk->yspeed.w == 0)
    return hit_v(pActwk, pPlayerwk, iD0, iD1);
  if (pPlayerwk->yspeed.w < 0)
    return hit_e(pActwk, pPlayerwk);

  return hit_v(pActwk, pPlayerwk, iD0, iD1);
}

Sint16 hit_v(sprite_status* pActwk, sprite_status* pPlayerwk, Sint16 iHitXs, Sint16 iHitHs) {
  Sint16 iHitYs;
  Sint16 iHitVs;
  Sint16 iD4, iD5;
  Sint16 iwk;

  if (pActwk->actno == 25 && actwk[0].cddat & 4)
    iwk = -2;
  else
    iwk = 2;

iHitVs = (Sint16)pActwk->sprvsize + (Sint16)pPlayerwk->sprvsize + iwk;

iHitYs = pPlayerwk->yposi.w.h - pActwk->yposi.w.h;
  iHitYs += iHitVs;
  if (iHitYs < 0)
    return hit_e(pActwk, pPlayerwk);
  iD4 = iHitVs + iHitVs;
  if (iHitYs >= iD4)
    return hit_e(pActwk, pPlayerwk);

  iD4 = iHitXs;
  if (iHitHs < iHitXs)
  {
    iHitHs += iHitHs;
    iHitXs -= iHitHs;
    iD4 = -iHitXs;
  }

iD5 = iHitYs;
  if (iHitVs < iHitYs)
  {
    iHitVs += iHitVs;
    iHitYs -= iHitVs;
    iD5 = -iHitYs;
  }

  if (iD5 < iD4)
    return hit_y(pActwk, pPlayerwk, iHitYs);

  return hit_x(pActwk, pPlayerwk, iHitXs, iD5);
}

Sint16 hit_x(sprite_status* pActwk, sprite_status* pPlayerwk, Sint16 iHitXs, Sint16 iD5) {
  if (pActwk->r_no1 == 1)
    return hit_e(pActwk, pPlayerwk);
  if (pActwk->actno == 10)
    if (pPlayerwk->cddat & 2)
    return hit_e(pActwk, pPlayerwk);

  if ((Uint16)iD5 < 5)
    return hit_e(pActwk, pPlayerwk);
  hit_set(pActwk, pPlayerwk);
  ride_on_clr(pActwk, pPlayerwk);
  pActwk->r_no1 = 0;
  pPlayerwk->xposi.w.h -= iHitXs;
  if (iHitXs >= 0)
  {

    if (pPlayerwk->xspeed.w > 0)
      return push_set(pActwk, pPlayerwk);
    else
      return push_clr(pActwk, pPlayerwk);
  }

if (pPlayerwk->xspeed.w >= 0)
    return push_clr(pActwk, pPlayerwk);
  else
    return push_set(pActwk, pPlayerwk);

}

Sint16 push_set(sprite_status* pActwk, sprite_status* pPlayerwk) {
  side_coli(pActwk, pPlayerwk);
  if (!(pPlayerwk->cddat & 2))
  {
    pActwk->cddat |= 32;
    pPlayerwk->cddat |= 32;
    pPlayerwk->mspeed.w = 0;
  }

  pPlayerwk->xspeed.w = 0;
  return 0;
}

Sint16 push_clr(sprite_status* pActwk, sprite_status* pPlayerwk) {
  hit_clr(pActwk, pPlayerwk);
  side_coli(pActwk, pPlayerwk);
  pActwk->cddat &= 223;
  pPlayerwk->cddat &= 223;
  return 0;
}

Sint16 hit_y(sprite_status* pActwk, sprite_status* pPlayerwk, Sint16 iHitYs) {
  Sint16 iSprVchk;
  Sint16 iDirCol;
  Sint16 iD0, iD1, iD3;
  char cDirec;
  Sint32 lXspeed, lYspeed;

  if (pActwk->actno == 25 && pPlayerwk->cddat & 4)
    return hit_e(pActwk, pPlayerwk);

  iSprVchk = pActwk->sprvsize;
  iSprVchk += pPlayerwk->sprvsize;

  if (iHitYs != 0)
    if (iHitYs < 0)
      return hit_yu(pActwk, pPlayerwk, iHitYs);

if (pPlayerwk->mstno.b.h != 43)
    if (pPlayerwk->yspeed.w != 0)
      if (pPlayerwk->yspeed.w < 0) return hit_e(pActwk, pPlayerwk);

  pPlayerwk->yposi.w.h = pActwk->yposi.w.h;
  pPlayerwk->yposi.w.h -= iSprVchk;
  lXspeed = pActwk->xspeed.w;
  lXspeed = lXspeed << 8;
  pPlayerwk->xposi.l += lXspeed;

  cDirec = -64;
  if (pActwk->xspeed.w < 0)
    cDirec = -cDirec;
  if (pActwk->xspeed.w != 0)
  {

iDirCol = dircolm(pPlayerwk, &cDirec);
    if (iDirCol < 0)
    {
      if (pActwk->xspeed.w < 0)
        iDirCol *= -1;
      pPlayerwk->xposi.w.h += iDirCol;
    }
  }

lYspeed = pActwk->yspeed.w << 8;
  pPlayerwk->yposi.l += lYspeed;

  if (pActwk->actno != 10)
  {
    if (pActwk->yspeed.w >= 0)
    {
      iDirCol = dircol_d(pPlayerwk, &iD0, &iD1, &iD3);
      if (iDirCol < 0)
      {
        pPlayerwk->yposi.w.h += iDirCol;
        return hit_e(pActwk, pPlayerwk);
      }
    }

    if (pActwk->yspeed.w < 0)
    {
      iDirCol = dircol_u2(pPlayerwk);
      if (iDirCol < 0)
      {
        playdieset(pPlayerwk);
        return hit_e(pActwk, pPlayerwk);
      }
    }
  }

  ride_on_set(pActwk, pPlayerwk);
  return 1;
}

Sint16 hit_yu(sprite_status* pActwk, sprite_status* pPlayerwk, Sint16 iHitYs) {
  if (pActwk->r_no1 == 1)
    return hit_e(pActwk, pPlayerwk);
  if (pActwk->actno == 9)
    return hit_e(pActwk, pPlayerwk);
  if (pActwk->actno != 10)
    return hit_yu1(pActwk, pPlayerwk, iHitYs);
  if (pActwk->r_no1 == 2)
    return hit_yu0(pActwk, pPlayerwk, iHitYs);
  if (pActwk->actflg & 2)
    return hit_yu0(pActwk, pPlayerwk, iHitYs);

  return hit_e(pActwk, pPlayerwk);
}

Sint16 hit_yu1(sprite_status* pActwk, sprite_status* pPlayerwk, Sint16 iHitYs) {
  if (pPlayerwk->cddat & 2)
    return hit_yu0(pActwk, pPlayerwk, iHitYs);
  if (pActwk->yspeed.w <= 0)
    return hit_yu0(pActwk, pPlayerwk, iHitYs);
  playdieset(pPlayerwk);
  return hit_yu0(pActwk, pPlayerwk, iHitYs);
}

Sint16 hit_yu0(sprite_status* pActwk, sprite_status* pPlayerwk, Sint16 iHitYs) {
  pPlayerwk->yposi.w.h -= iHitYs;
  pPlayerwk->yspeed.w = 0;
  hit_clr(pActwk, pPlayerwk);
  ride_on_clr(pActwk, pPlayerwk);
  pActwk->r_no1 = 0;
  return 1;
}

Sint16 hit_e(sprite_status* pActwk, sprite_status* pPlayerwk) {
  hit_clr(pActwk, pPlayerwk);
  ride_on_clr(pActwk, pPlayerwk);
  pActwk->r_no1 = 0;
  return 0;
}

void side_coli(sprite_status* pActwk, sprite_status* pPlayerwk) {
  char cDirect = -64;
  Sint16 iDirCol;

  if (pActwk->xspeed.w == 0) return;

  if (pActwk->actno == 10) return;

  if (pActwk->xspeed.w < 0)
    cDirect = -cDirect;

  iDirCol = dircol(pPlayerwk, &cDirect);
  if (iDirCol >= 0) return;

  playdieset(pPlayerwk);
}

void hit_set(sprite_status* pActwk, sprite_status* pPlayerwk) {
  Sint16 iHitActno;
  sprite_status* pHitAct;

  if (pActwk->actno == 10)
  {
    pPlayerwk->colino = 0;
    return;
  }

  iHitActno = (Uint16)pPlayerwk->colino;
  if (iHitActno != 0)
  {
    pHitAct = &actwk[iHitActno];
    if (pHitAct == pActwk) return;

    if (pActwk->xspeed.w == 0 && pHitAct->xspeed.w == 0) return;

if (pPlayerwk->xposi.w.h >= pActwk->xposi.w.h && pPlayerwk->xposi.w.h >= pHitAct->xposi.w.h) return;

if (pActwk->actno == 21) return;

#if defined(R41A)
    if (pActwk->actno == 40 && pHitAct->actno == 39) return;
#endif

    if (stageno.w == 768 && pActwk->actno == 39 && pHitAct->actno == 39) return;

playdieset(pPlayerwk);
    return;
  }

  pPlayerwk->colino = pActwk - actwk;
}

void hit_clr(sprite_status* pActwk, sprite_status* pPlayerwk) {
  Uint16 iHitAct;

  iHitAct = pPlayerwk->colino;
  if (pActwk == &actwk[iHitAct]) return;

  pPlayerwk->colino = 0;
}
