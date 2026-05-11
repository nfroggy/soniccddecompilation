#include "../../types.h"
#include "lpl_types.h"
#include "spm_equ.h"
#include "asciiset.h"
#include "actm.h"

static void ascchk(sprite_status_lpl* pActwk);
static void set_music(sprite_status_lpl* pActwk);
static void ascset(sprite_status_lpl* pActwk);
static void ascchg(sprite_status_lpl* pActwk);
static void ascspr_init(sprite_status_lpl* pActwk, sprite_status_lpl* pAscwk);
static void ascspr_set(sprite_status_lpl* pActwk, sprite_status_lpl* pAscwk);
static void ascspr_chk(sprite_status_lpl* pActwk, sprite_status_lpl* pAscwk);
static void ascspr_exit(sprite_status_lpl* pActwk, sprite_status_lpl* pAscwk);

static Uint8 bit_SPACE;
static Uint8 space_flg;
extern Sint16 ascii_xposi_tbl[];
extern sprite_patterns_title* asciispr_tbl[];

void sprascii(void) {
  sprite_status_lpl* pAsciiWk;
  static void(*ascsprtbl[3])(sprite_status_lpl*) = {
    &ascchk,
    &ascset,
    &ascchg
  };

bit_SPACE = 0;

  pAsciiWk = &actwk[17];
  ascsprtbl[pAsciiWk->ACT_NO](pAsciiWk);
}

static void ascchk(sprite_status_lpl* pActwk) {
  if (!(swdata1.b.h & 32)) {

if (push_flg & 32) {

      if (!(music_selflg & 1)) {

        push_flg &= 223;
      }
      else if (!(music_selflg & 4)) {

        push_flg &= 223;
        set_music(pActwk);
        return;
      }
    }
  }
  else {

    push_flg |= 32;
  }

  if (!(swdata1.b.h & 16)) {

    if (push_flg & 16) {

      if (!(music_selflg & 1)) {
        push_flg &= 239;
      }
      else if (!(music_selflg & 4)) {

        push_flg &= 239;
        set_music(pActwk);
        return;
      }
    }
  }
  else {

    push_flg |= 16;
  }

  if (!(swdata1.b.h & 64)) {

if (push_flg & 64) {

      if (!(music_selflg & 1)) {

        music_selflg |= 1;
        music_selflg |= 4;
        push_flg &= 191;
      }
      else if (!(music_selflg & 4)) {

        push_flg &= 191;
        set_music(pActwk);
        return;
      }
    }
    if (music_selflg & 4) {

      if (chk_actwk() == 0) {

        comflag_m |= 16;
        music_selflg &= 251;
        pActwk->ACT_NO = 1;
      }
    }

  }
  else {

    push_flg |= 64;
  }

if (bit_SPACE == 0) {

    if (space_flg & 1) {

      if (!(music_selflg & 1)) {

        music_selflg |= 1;
        music_selflg |= 4;
        space_flg = 0;
      }
      else if (!(music_selflg & 4)) {

        space_flg = 0;
        set_music(pActwk);
        return;
      }
    }
    if (music_selflg & 4) {

      if (chk_actwk() == 0) {

        comflag_m |= 16;
        music_selflg &= 251;
        pActwk->ACT_NO = 1;
      }
    }
  }
  else {

    space_flg |= 1;
  }

}

static void set_music(sprite_status_lpl* pActwk) {
  Sint16 MusicNo, PlanetNo;

  actset_flg = 0;
  MusicNo = pActwk->EXE_NO;
  comdata_m4 = MusicNo;
  PlanetNo = ascii_xposi_tbl[MusicNo * 2];
  if (PlanetNo != 3) lplanet_no = PlanetNo;
  comdata_m5 = PlanetNo;
  comflag_m &= 239;
  comflag_m |= 32;

}

static void ascset(sprite_status_lpl* pActwk) {
  sprite_status_lpl* pFreeAct;

  if (get_actwk(&pFreeAct) == 0) {

    pActwk->XPOSI.w.h = pFreeAct - actwk;
    music_selflg |= 2;
    pActwk->ACT_NO = 2;
  }

}

static void ascchg(sprite_status_lpl* pActwk) {

}

void set_asciicg(void) {
  sprite_status_lpl* pStringwk;

  if (music_selflg & 2) {

    pStringwk = &actwk[actwk[17].XPOSI.w.h];
    pStringwk->ACT_NO = 7;
    music_selflg &= 253;
    if (!(actset_flg & 1)) {

      actset_flg |= 1;
      pStringwk->ACT_FLG = 1;
      return;
    }

    pStringwk->ACT_FLG = 2;
    actset_flg |= 2;
  }

}

void ascii_sprite(sprite_status_lpl* pActwk) {
  static void(*acttbl[4])(sprite_status_lpl*, sprite_status_lpl*) = {
    &ascspr_init,
    &ascspr_set,
    &ascspr_chk,
    &ascspr_exit
  };

  acttbl[pActwk->EXE_NO](pActwk, &actwk[17]);

}

static void ascspr_init(sprite_status_lpl* pActwk, sprite_status_lpl* pAscwk) {
  pActwk->PAT_NO = 0;
  pActwk->PAT_ADR = asciispr_tbl[pAscwk->EXE_NO];
  if (pAscwk->XPOSI.b.b3 & 1) {

    pActwk->XPOSI.w.h = -128;
    pActwk->X_SPEED.l = 0x200000;
    pActwk->SPR_FLG |= 8;
  }
  else {

    pActwk->XPOSI.w.h = 256;
    pActwk->X_SPEED.l = -0x200000;
    pActwk->SPR_FLG &= 247;
  }
  pActwk->SPR_FLG |= 32;
  pActwk->YPOSI.w.h = 208;
  pActwk->X_OFFSET = ascii_xposi_tbl[pAscwk->EXE_NO * 2 + 1];
  pActwk->CGBASE -= 8192;
  pActwk->EXE_NO = 1;
  pAscwk->ACT_NO = 0;

}

static void ascspr_set(sprite_status_lpl* pActwk, sprite_status_lpl* pAscwk) {
  pActwk->XPOSI.l += pActwk->X_SPEED.l;
  if (!(pActwk->SPR_FLG & 8) && pActwk->XPOSI.w.h < pActwk->X_OFFSET
      || (pActwk->SPR_FLG & 8) && pActwk->XPOSI.w.h >= pActwk->X_OFFSET) {

    pActwk->X_SPEED.l = 0;
    pActwk->XPOSI.w.h = pActwk->X_OFFSET;
    pActwk->EXE_NO = 2;
  }

}

static void ascspr_chk(sprite_status_lpl* pActwk, sprite_status_lpl* pAscwk) {
  if (swdata1.b.h & 8) {

    pActwk->EXE_NO = 3;
    pActwk->X_SPEED.l = 0x200000;
    pActwk->SPR_FLG |= 8;
  }
  if (swdata1.b.h & 4) {

    pActwk->X_SPEED.l = -0x200000;
    pActwk->EXE_NO = 3;
    pActwk->SPR_FLG &= 247;
  }

}

static void ascspr_exit(sprite_status_lpl* pActwk, sprite_status_lpl* pAscwk) {
  pActwk->XPOSI.l += pActwk->X_SPEED.l;
  if (!(pActwk->SPR_FLG & 8)) {

    if (pActwk->XPOSI.w.h < -127) {

      music_selflg |= 4;
      pAscwk->XPOSI.b.b3 &= -2;
      actset_flg ^= pActwk->ACT_FLG;
      pActwk->SPR_FLG |= 16;
      if (++pAscwk->EXE_NO > 31) pAscwk->EXE_NO = 0;
    }
  }
  else {

    if (pActwk->XPOSI.w.h >= 288) {

      music_selflg |= 4;
      pAscwk->XPOSI.b.b3 |= 1;
      actset_flg ^= pActwk->ACT_FLG;
      pActwk->SPR_FLG |= 16;
      if ((Sint16)--pAscwk->EXE_NO < 0) pAscwk->EXE_NO = 31;
    }
  }

}
