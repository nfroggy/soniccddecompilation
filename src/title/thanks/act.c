#include "../../types.h"
#include "thanks_types.h"
#include "act.h"
#include "../../impfuncs.h"
#include "sprmove.h"

static void actloop(Sint16 id);
static void priact(sprite_status_thanks* pActwk, Sint16 ActNo);
static void dummy(sprite_status_thanks* pActwk);
static void pattern_chg(sprite_status_thanks* pActwk, sprite_patterns_title* pPatTbl);

static void(*act_tbl[31])(sprite_status_thanks*) = {
  &sonic_ctrl,
  &emie_ctrl,
  &metals_ctrl,
  &flick_ctrl,
  &pocky_ctrl,
  &ricky_ctrl,
  &heart_ctrl,
  &inco_ctrl,
  &hato_ctrl,
  &sheep_ctrl,
  &pecky_ctrl,
  &dummy, &dummy,
  &dummy, &dummy,
  &dummy, &dummy,
  &dummy, &dummy,
  &dummy, &dummy,
  &dummy, &dummy,
  &dummy, &dummy,
  &dummy, &dummy,
  &dummy, &dummy,
  &dummy, &dummy
};
extern sprite_status_thanks actwk[66];
extern sprite_status_thanks** priptr[4];
extern sprite_status_thanks* pribuf0[64];
extern sprite_status_thanks* pribuf3[64];
extern sprite_status_thanks* pribuf2[64];
extern sprite_status_thanks* pribuf1[64];
static Uint16 LinkData;

Sint16 get_actwk(sprite_status_thanks** pNewActwk) {
  sprite_status_thanks* pActwk;
  Sint16 i;

  for (i = 0; i <= 60; ++i)
  {
    pActwk = &actwk[i + 3];
    if (pActwk->ACT_NO == 0)
    {
      *pNewActwk = pActwk;
      return 0;
    }
  }
  return -1;
}

Sint16 chk_actwk(void) {
  sprite_status_thanks* pActwk;
  Sint16 i;

  for (i = 0; i <= 60; ++i)
  {
    pActwk = &actwk[i];
    if (pActwk->ACT_NO == 0)
    {
      return 0;
    }
  }
  return -1;
}

void priset(void) {
  sprite_status_thanks*** a1;
  sprite_status_thanks*** a2;
  sprite_status_thanks** a3;
  sprite_status_thanks* pActwk;
  Sint16 i;

  a1 = priptr;
  priptr[0] = pribuf0;
  priptr[1] = pribuf1;
  priptr[2] = pribuf2;
  priptr[3] = pribuf3;

  pribuf0[0] = 0;
  pribuf0[1] = 0;
  pribuf0[2] = 0;
  pribuf0[3] = 0;

  for (i = 0; i <= 63; ++i)
  {
    pActwk = &actwk[i];
    if (pActwk->ACT_NO)
    {
      a2 = &priptr[pActwk->free[0]];
      a3 = *a2;
      *a3++ = pActwk;
      *a3 = 0;
      *a2 = a3;
    }
  }
}

void action(void) {
  Sint16 i;

  actloop(0);
  priset();
  spr_set();
  for (i = LinkData; i < 80; ++i) EAsprset(0, 0, 0, i, 0);
}

static void actloop(Sint16 id) {
  sprite_status_thanks* pActwk;
  Sint16 i;

  for (i = 0; i <= 63; ++i)
  {
    pActwk = &actwk[id++];
    if (pActwk->ACT_NO)
    {
      priact(pActwk, pActwk->ACT_NO);
    }
  }
}

static void priact(sprite_status_thanks* pActwk, Sint16 ActNo) {
  act_tbl[ActNo - 1](pActwk);
  if (pActwk->SPR_FLG & 16)
  {
    sMemSet(pActwk, 0, sizeof(*pActwk));
  }
}

static void dummy(sprite_status_thanks* pActwk) {
  pActwk->ACT_NO = 0;
}

void spr_set(void) {
  sprite_status_thanks* pActwk;
  sprite_status_thanks** pPribuf;
  sprite_patterns_title* pPatTbl;
  sprite_pattern_mapping_title* pMapDat;
  Sint16 pri, i, j;
  Sint16 xpos, ypos;
  Uint16 RevFlg;

  LinkData = 0;
  priptr[0] = pribuf0;
  priptr[1] = pribuf1;
  priptr[2] = pribuf2;
  priptr[3] = pribuf3;

  for (pri = 0; pri <= 3; ++pri)
  {
    pPribuf = priptr[pri];
    for (i = 0; i < 64; ++i)
    {
      if (*pPribuf == 0) break;
      pActwk = *pPribuf++;
      if (pActwk->ACT_NO == 0) continue;

      pPatTbl = pActwk->PAT_ADR;
      pattern_chg(pActwk, pPatTbl);

      j = pPatTbl->aPatDat[pActwk->PAT_NO].paMapTbl->mapnum;
      pMapDat = pPatTbl->aPatDat[pActwk->PAT_NO].paMapTbl->aMapDat;

      for ( ; j > 0; --j, ++pMapDat)
      {
        RevFlg = 0;
        xpos = pActwk->XPOSI.w.h;
        if (pActwk->SPR_FLG & 128)
        {
          xpos -= pMapDat->hrevoffs;
          RevFlg |= 1;
        }
        else
        {
          xpos -= pMapDat->hoffs;
        }
        xpos += 128;
        ypos = pActwk->YPOSI.w.h;
        if (pActwk->SPR_FLG & 64)
        {
          ypos -= pMapDat->vrevoffs;
          RevFlg |= 2;
        }
        else
        {
          ypos -= pMapDat->voffs;
        }
        ypos += 128;
        RevFlg ^= pMapDat->etc;
        if (pActwk->SPR_FLG & 32)
        {
          RevFlg |= 32768;
        }
        EAsprset(xpos, ypos, pMapDat->index, LinkData, RevFlg);
        if (++LinkData >= 80) break;
      }
    }
  }
}

static void pattern_chg(sprite_status_thanks* pActwk, sprite_patterns_title* pPatTbl) {
  Sint16 d0;

  --pActwk->SPR_TIMER;
  if (pActwk->SPR_TIMER <= 0)
  {
    d0 = pActwk->PAT_NO;
    ++d0;
    if ((Uint16)d0 >= (Uint16)pPatTbl->patnum)
    {
      d0 = 0;
    }
    pActwk->PAT_NO = d0;
    pActwk->SPR_TIMER = pPatTbl->aPatDat[d0].timer;
  }
}

void clrwork(sprite_status_thanks* pActwk) {
  sMemSet(pActwk, 0, sizeof(*pActwk));
}

void action_all_clr(void) {
  Sint16 i;

  for (i = 0; i < 64; ++i)
  {
    sMemSet(&actwk[i], 0, sizeof(actwk[i]));
  }
}
