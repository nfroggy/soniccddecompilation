#include "../../types.h"
#include "lpl_types.h"
#include "spm_equ.h"
#include "actm.h"
#include "../../impfuncs.h"
#include "asciiset.h"
#include "chamov.h"
#include "plm.h"

static void actloop(sprite_status_lpl* pActwk);
static void priact(sprite_status_lpl* pActwk, Sint32 ActNo);
static void dummy(sprite_status_lpl* pActwk);
static void pattern_chg(sprite_status_lpl* pActwk, sprite_patterns_title* pPatTbl);

static void(*act_tbl[31])(sprite_status_lpl*) =
{
  &flicky_ctrl,
  &star_ctrl,
  &eggm_ctrl,
  &ufo_ctrl,
  &msnc_ctrl,
  &tails_ctrl,
  &ascii_sprite,
  &dummy, &dummy,
  &dummy, &dummy,
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
static Uint16 LinkData;

Sint32 get_actwk(sprite_status_lpl** ppFreeAct) {
  Sint32 i;
  sprite_status_lpl* pChkAct;

  i = 15;
  pChkAct = actwk;
  do
  {
    if (pChkAct->ACT_NO == 0)
    {
      *ppFreeAct = pChkAct;
      return 0;
    }
    ++pChkAct;
  } while (--i >= 0);
  return -1;
}

Sint32 chk_actwk(void) {
  Sint32 i;
  sprite_status_lpl* pChkAct;

  i = 15;
  pChkAct = actwk;
  do
  {
    if (pChkAct->ACT_NO != 0) return -1;
    ++pChkAct;
  } while (--i >= 0);
  return 0;
}

void action(void) {
  Uint16 i;

  sprinit();

  actloop(actwk);

  spr_set();
  for (i = LinkData; i < 80; ++i) EAsprset(0, 0, 0, i, 0);

}

static void actloop(sprite_status_lpl* pActwk) {
  Sint32 i, ActNo;

  i = 15;
  do
  {
    if ((ActNo = pActwk->ACT_NO) != 0) priact(pActwk, ActNo);
    ++pActwk;
  } while (--i >= 0);

}

static void priact(sprite_status_lpl* pActwk, Sint32 ActNo) {
  act_tbl[ActNo - 1](pActwk);
  if (pActwk->SPR_FLG & 16) sMemSet(pActwk, 0, sizeof(*pActwk));

}

static void dummy(sprite_status_lpl* pActwk) {
  actset_flg ^= pActwk->ACT_FLG;
  pActwk->SPR_FLG |= 16;

}

void spr_set(void) {
  Sint32 i, j;
  sprite_status_lpl* pActwk;
  sprite_patterns_title* pPatTbl;
  sprite_pattern_mapping_title* pMapDat;
  Sint16 xpos, ypos;
  Uint16 RevFlg;

  pActwk = actwk;
  LinkData = 0;
  for (i = 15; i >= 0 && LinkData < 80; --i, ++pActwk)
  {
    if (pActwk->ACT_NO == 0) continue;
    if ((pPatTbl = pActwk->PAT_ADR) == 0) continue;
    pattern_chg(pActwk, pPatTbl);
    j = pPatTbl->aPatDat[pActwk->PAT_NO].paMapTbl->mapnum;
    pMapDat = pPatTbl->aPatDat[pActwk->PAT_NO].paMapTbl->aMapDat;
    for (--j; j >= 0; --j, ++pMapDat)
    {
      RevFlg = 0;
      xpos = pActwk->XPOSI.w.h;
      if (pActwk->SPR_FLG & 128)
      {
        RevFlg |= 1;
        xpos -= pMapDat->hrevoffs;
      }
      else
      {
        xpos -= pMapDat->hoffs;
      }
      ypos = pActwk->YPOSI.w.h;
      if (pActwk->SPR_FLG & 64)
      {
        RevFlg |= 2;
        ypos -= pMapDat->vrevoffs;
      }
      else
      {
        ypos -= pMapDat->voffs;
      }
      RevFlg ^= pMapDat->etc;
      if (pActwk->SPR_FLG & 32) RevFlg |= 32768;
      EAsprset(xpos + 128, ypos + 128, pMapDat->index, LinkData, RevFlg);
      if (++LinkData >= 80) break;
    }
  }
}

static void pattern_chg(sprite_status_lpl* pActwk, sprite_patterns_title* pPatTbl) {
  Sint16 PatNoWk;

  if (--pActwk->SPR_TIMER < 0)
  {
    if ((PatNoWk = pActwk->PAT_NO + 1) >= pPatTbl->patnum) PatNoWk = 0;
    pActwk->PAT_NO = PatNoWk;
    pActwk->SPR_TIMER = pPatTbl->aPatDat[PatNoWk].timer;
  }

}
