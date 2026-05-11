#include "../../types.h"
#include "thanks_types.h"
#include "sprmove.h"
#include "act.h"
#include "etc_m.h"
#include "game.h"

static void sonicinit(sprite_status_thanks* pActwk);
static void s_runset(sprite_status_thanks* pActwk);
static void sonicrun(sprite_status_thanks* pActwk);
static Sint16 s_metalchk(sprite_status_thanks* pActwk);
static void s_turnset(sprite_status_thanks* pActwk);
static void sonicturn(sprite_status_thanks* pActwk);
static void s_standset(sprite_status_thanks* pActwk);
static void sonicstand(sprite_status_thanks* pActwk);
static void s_waitset(sprite_status_thanks* pActwk);
static void sonicwait(sprite_status_thanks* pActwk);
static void s_ballset(sprite_status_thanks* pActwk);
static void sonicball(sprite_status_thanks* pActwk);
static void sonicexit(sprite_status_thanks* pActwk);
static void sonicpit(sprite_status_thanks* pActwk);
static void sonicesc(sprite_status_thanks* pActwk);
static void emmy_exeset(sprite_status_thanks* pActwk, sprite_status_thanks* pSonic);
static void emmyinit(sprite_status_thanks* pActwk);
static void e_runinit(sprite_status_thanks* pActwk);
static void emmymove(sprite_status_thanks* pActwk);
static void e_kissset(sprite_status_thanks* pActwk);
static void emmykiss(sprite_status_thanks* pActwk);
static void e_jumpset(sprite_status_thanks* pActwk);
static void emmyjump(sprite_status_thanks* pActwk);
static void e_catchset(sprite_status_thanks* pActwk);
static void emmy_catch(sprite_status_thanks* pActwk);
static void e_turnset(sprite_status_thanks* pActwk);
static void emmyturn(sprite_status_thanks* pActwk);
static void e_standset(sprite_status_thanks* pActwk);
static void emmymoji(sprite_status_thanks* pActwk);
static void emmyexit(sprite_status_thanks* pActwk);
static void emmypose(sprite_status_thanks* pActwk);
static void emmyesc(sprite_status_thanks* pActwk);
static void metalsinit(sprite_status_thanks* pActwk);
static void m_flyset(sprite_status_thanks* pActwk);
static void metalsfly(sprite_status_thanks* pActwk);
static void m_brakeset(sprite_status_thanks* pActwk);
static void metalsbrake(sprite_status_thanks* pActwk);
static void m_stopset(sprite_status_thanks* pActwk);
static void metalsstop(sprite_status_thanks* pActwk);
static void m_standset(sprite_status_thanks* pActwk);
static void metalstand(sprite_status_thanks* pActwk);
static void m_startset(sprite_status_thanks* pActwk);
static void metalstartcttbl(sprite_status_thanks* pActwk);
static void m_eraset(sprite_status_thanks* pActwk);
static void metalsera(sprite_status_thanks* pActwk);
static void metalsexit(sprite_status_thanks* pActwk);
static void metalspit(sprite_status_thanks* pActwk);
static void metalsesc(sprite_status_thanks* pActwk);
static void heartinit(sprite_status_thanks* pActwk);
static void heartmove(sprite_status_thanks* pActwk);
static void pockyinit(sprite_status_thanks* pActwk);
static void peckyinit(sprite_status_thanks* pActwk);
static void rickyinit(sprite_status_thanks* pActwk);
static void sheepinit(sprite_status_thanks* pActwk);
static void animalmove(sprite_status_thanks* pActwk);
static void flickyinit(sprite_status_thanks* pActwk);
static void incoinit(sprite_status_thanks* pActwk);
static void hatoinit(sprite_status_thanks* pActwk);
static void birdmove(sprite_status_thanks* pActwk);
static Sint16 killchk(sprite_status_thanks* pActwk);
static void sinhmove(sprite_status_thanks* pActwk);
static void sinvmove(sprite_status_thanks* pActwk);
static void hbt_move(sprite_status_thanks* pActwk);
static Sint16 chk_sonicposi(sprite_status_thanks* pActwk, sprite_status_thanks* pA6, Sint16* d5, Sint16* d6);
static Sint16 myposichk(sprite_status_thanks* pActwk);
static Sint16 centerchk(sprite_status_thanks* pActwk);
static Sint16 dsplaychk(sprite_status_thanks* pActwk);
static void waitmode(sprite_status_thanks* pActwk);
static void escape_chk(sprite_status_thanks* pActwk);
static void pit_in(sprite_status_thanks* pActwk);
static void m_sndchk(sprite_status_thanks* pActwk);

extern Uint8 animal_flg;
extern sprite_patterns_title s_run_map;
extern sprite_status_thanks actwk[66];
extern sprite_patterns_title s_stop_map;
extern sprite_patterns_title s_stand1_map;
extern sprite_patterns_title s_stand2_map;
extern sprite_patterns_title s_wait_map;
extern sprite_patterns_title s_ball_map;
extern Sint16 actset_tm[8];
extern sprite_patterns_title erun_map;
extern Sint16 heart_tm;
extern sprite_patterns_title ekiss_map;
extern Uint8 poseno;
extern sprite_patterns_title ejump_map;
extern sprite_patterns_title ecatch_map;
extern sprite_patterns_title estand2_map;
extern sprite_patterns_title estand_map;
extern sprite_patterns_title m_fly1_map;
extern sprite_patterns_title m_brake_map;
extern sprite_patterns_title m_stop_map;
extern sprite_patterns_title m_stand_map;
extern sprite_patterns_title m_furi_map;
extern sprite_patterns_title m_kamae_map;
extern sprite_patterns_title m_era_map;
extern sprite_patterns_title m_era2_map;
extern sprite_patterns_title eheart_map;
extern sprite_patterns_title pocky_map;
extern sprite_patterns_title pecky_map;
extern sprite_patterns_title ricky_map;
extern sprite_patterns_title sheep_map;
extern sprite_patterns_title flick_map;
extern sprite_patterns_title inco_map;
extern sprite_patterns_title hato_map;

void sonic_ctrl(sprite_status_thanks* pActwk) {
  void(*acttbl[14])(sprite_status_thanks*) =
  {
    &sonicinit,
    &s_runset,
    &sonicrun,
    &s_turnset,
    &sonicturn,
    &s_standset,
    &sonicstand,
    &s_waitset,
    &sonicwait,
    &s_ballset,
    &sonicball,
    &sonicexit,
    &sonicpit,
    &sonicesc
  };

  if (animal_flg)
  {
    pActwk->SPR_FLG |= 4;
  }
  acttbl[pActwk->EXE_NO](pActwk);
  pActwk->XPOSI.w.h &= 511;
}

static void sonicinit(sprite_status_thanks* pActwk) {
  int_union ld0;

  ld0.l = random();
  ld0.w.l &= 1;
  ld0.w.l = 1;
  if (ld0.w.l)
  {
    pActwk->XPOSI.w.h = 328;
    pActwk->X_SPEED.l = -196608;
    pActwk->SPR_FLG |= 128;
  }
  else
  {
    pActwk->XPOSI.w.h = 496;
    pActwk->X_SPEED.l = 196608;
    pActwk->SPR_FLG &= 127;
  }
  pActwk->YPOSI.w.h = 140;
  pActwk->Y_SPEED.l = 0;

  pActwk->X_OFFSET = 0;
  pActwk->Y_OFFSET = 0;
  pActwk->X_ACCEL.l = 0;
  pActwk->Y_ACCEL.l = 0;

  pActwk->CGBASE = 25127;
  pActwk->free[0] = 1;
  pActwk->PAT_ADR = &s_run_map;
  pActwk->SPR_TIMER = s_run_map.aPatDat[0].timer;
  pActwk->PAT_NO = 0;

  ld0.l = random();
  ld0.l &= 32767;
  pActwk->TM_CNT = ld0.w.l % 128;
  pActwk->TM_CNT += 128;
  pActwk->EXE_NO = 2;
}

static void s_runset(sprite_status_thanks* pActwk) {
  sprite_status_thanks* pMetal;
  int_union ld0;

  pActwk->XPOSI.l &= 33554431;
  pActwk->X_SPEED.l = 196608;
  if (pActwk->SPR_FLG & 128)
  {
    pActwk->X_SPEED.l *= -1;
  }
  pActwk->Y_SPEED.l = 0;

  pActwk->X_OFFSET = 0;
  pActwk->Y_OFFSET = 0;
  pActwk->X_ACCEL.l = 0;
  pActwk->Y_ACCEL.l = 0;

  pActwk->PAT_ADR = &s_run_map;
  pActwk->SPR_TIMER = s_run_map.aPatDat[0].timer;
  pActwk->PAT_NO = 0;

  ld0.l = random();
  ld0.l &= 32767;
  pActwk->TM_CNT = ld0.w.l % 256;
  pActwk->TM_CNT += 64;

  pMetal = &actwk[2];
  pMetal->ACT_NO = 3;

  pActwk->EXE_NO = 2;
}

static void sonicrun(sprite_status_thanks* pActwk) {
  int_union ld0;

  pActwk->XPOSI.l += pActwk->X_SPEED.l;
  pActwk->YPOSI.l += pActwk->Y_SPEED.l;

  if (pActwk->SPR_FLG & 4)
  {
    pActwk->EXE_NO = 11;
    return;
  }
  if (pActwk->XPOSI.w.h >= 376)
  {
    if (pActwk->XPOSI.w.h <= 392)
    {
      ((Sint16*)pActwk)[31] = 128;
      pActwk->EXE_NO = 12;
      return;
    }
  }

  ld0.l = random();
  ld0.l &= 32767;
  ld0.w.l = (Sint32)ld0.w.l % 1536;

ld0.l = random();
  ld0.l &= 32767;
  ld0.w.l = (Sint32)ld0.w.l % 512;
  if (ld0.w.l == 0)
  {
    pActwk->EXE_NO = 9;
    return;
  }

  if (s_metalchk(pActwk))
  {
    pActwk->EXE_NO = 3;
  }
  --pActwk->TM_CNT;
  if (pActwk->TM_CNT <= 0)
  {
    if (centerchk(pActwk))
    {
      pActwk->EXE_NO = 3;
    }
  }
}

static Sint16 s_metalchk(sprite_status_thanks* pActwk) {
  sprite_status_thanks* pMetal;
  Sint16 d5;

  pMetal = &actwk[2];
  if (pMetal->EXE_NO < 2) return 0;
  if (pMetal->EXE_NO > 4) return 0;

  pActwk->XPOSI.w.h &= 511;
  pMetal->XPOSI.w.h &= 511;
  if (pActwk->X_SPEED.l < 0)
  {
    if (pMetal->X_SPEED.l < 0) return 0;
  }
  else
  {
    if (pMetal->X_SPEED.l >= 0) return 0;
  }
  d5 = pMetal->XPOSI.w.h;
  d5 -= pActwk->XPOSI.w.h;
  if (d5 < 0)
  {
    d5 *= -1;
  }
  if (d5 > 8) return 0;
  return -1;
}

static void s_turnset(sprite_status_thanks* pActwk) {
  pActwk->XPOSI.l &= 33554431;
  if (pActwk->X_SPEED.l < 0)
  {
    pActwk->X_OFFSET = 10240;
    pActwk->SPR_FLG &= 127;
  }
  else
  {
    pActwk->X_OFFSET = -10240;
    pActwk->SPR_FLG |= 128;
  }
  pActwk->PAT_ADR = &s_stop_map;
  pActwk->SPR_TIMER = s_stop_map.aPatDat[0].timer;
  pActwk->PAT_NO = 0;

  if (dsplaychk(pActwk) == 0)
  {
    soundset(144);
  }
  pActwk->EXE_NO = 4;
}

static void sonicturn(sprite_status_thanks* pActwk) {
  sprite_status_thanks* pMetal;
  Sint16 d5, d6;

  hbt_move(pActwk);
  if (pActwk->PAT_NO < 5) return;
  pMetal = &actwk[2];
  if (pMetal->EXE_NO == 2)
  {
    pActwk->EXE_NO = 1;
    return;
  }
  if (myposichk(pActwk))
  {
    pActwk->EXE_NO = 9;
  }
  else
  {
    chk_sonicposi(pActwk, pMetal, &d5, &d6);
    if (d5 <= 24)
    {
      pActwk->EXE_NO = 9;
    }
    else
    {
      pActwk->EXE_NO = 5;
    }
  }
}

static void s_standset(sprite_status_thanks* pActwk) {
  sprite_status_thanks* pEmmy;

  pActwk->X_SPEED.l = 0;

  pActwk->PAT_ADR = &s_stand1_map;
  pActwk->SPR_TIMER = s_stand1_map.aPatDat[0].timer;
  pActwk->PAT_NO = 0;

  pActwk->TM_CNT = 127;
  pActwk->free[3] = 0;

  pEmmy = &actwk[1];
  pEmmy->ACT_NO = 2;

  pActwk->EXE_NO = 6;
}

static void sonicstand(sprite_status_thanks* pActwk) {
  sprite_status_thanks* pMetal;
  sprite_status_thanks* pEmmy;
  Sint16 d5, d6;

  pMetal = &actwk[2];
  chk_sonicposi(pActwk, pMetal, &d5, &d6);
  --pActwk->TM_CNT;
  if (pActwk->TM_CNT >= 0) return;
  if (pActwk->free[3])
  {
    pEmmy = &actwk[1];
    pEmmy->SPR_FLG &= 251;
    pActwk->EXE_NO = 7;
  }
  pActwk->PAT_ADR = &s_stand2_map;
  pActwk->SPR_TIMER = s_stand2_map.aPatDat[0].timer;
  pActwk->TM_CNT = 96;
  pActwk->free[3] = 1;
}

static void s_waitset(sprite_status_thanks* pActwk) {
  int_union ld0;

  pActwk->XPOSI.l &= 33554431;
  pActwk->free[3] = 0;
  pActwk->PAT_ADR = &s_wait_map;
  pActwk->SPR_TIMER = s_wait_map.aPatDat[0].timer;
  pActwk->PAT_NO = 0;

  ld0.l = random();
  ld0.l &= 32767;
  pActwk->TM_CNT = ld0.w.l % 127;
  pActwk->TM_CNT += 127;

  pActwk->EXE_NO = 8;
}

static void sonicwait(sprite_status_thanks* pActwk) {
  sprite_status_thanks* pEmmy;
  sprite_status_thanks* pMetal;
  Sint16 d5, d6;

  pEmmy = &actwk[1];
  pMetal = &actwk[2];
  pMetal->XPOSI.w.h &= 511;
  if (chk_sonicposi(pActwk, pMetal, &d5, &d6)) pActwk->SPR_FLG |= 128;
  else pActwk->SPR_FLG &= 127;
  pMetal->SPR_FLG &= 251;
  if (d5 > 8)
  {
    --pActwk->TM_CNT;
    if (pActwk->TM_CNT <= 0)
    {
      pActwk->EXE_NO = 9;
    }
  }
  else
  {
    pActwk->EXE_NO = 1;
  }
}

static void s_ballset(sprite_status_thanks* pActwk) {
  pActwk->XPOSI.l &= 33554431;
  pActwk->X_SPEED.l = 0x20000;
  if (pActwk->SPR_FLG & 128)
  {
    pActwk->X_SPEED.l *= -1;
  }
  pActwk->Y_SPEED.l = -393216;

  pActwk->X_OFFSET = 0;
  pActwk->X_ACCEL.l = 0;
  pActwk->Y_OFFSET = 16384;
  pActwk->Y_ACCEL.l = 0;

  pActwk->PAT_ADR = &s_ball_map;
  pActwk->SPR_TIMER = s_ball_map.aPatDat[0].timer;
  pActwk->PAT_NO = 0;

  if (dsplaychk(pActwk) == 0)
  {
    soundset(146);
  }
  pActwk->EXE_NO = 10;
}

static void sonicball(sprite_status_thanks* pActwk) {
  hbt_move(pActwk);
  if (pActwk->YPOSI.w.h >= 140)
  {
    pActwk->Y_ACCEL.l = 0;
    pActwk->EXE_NO = 1;
  }
}

static void sonicexit(sprite_status_thanks* pActwk) {
  sprite_status_thanks* pEmmy;
  sprite_status_thanks* pMetal;

  waitmode(pActwk);
  if (animal_flg == 0) return;
  pEmmy = &actwk[1];
  pMetal = &actwk[2];

  if (pActwk->X_SPEED.l != 0) return;

  if (pEmmy->EXE_NO)
  {
    if (pEmmy->EXE_NO != 13) return;
    if (pEmmy->X_SPEED.l != 0) return;
  }
  if (pMetal->EXE_NO)
  {
    if (pMetal->EXE_NO != 13) return;
    if (pMetal->X_SPEED.l != 0) return;
  }
  pActwk->SPR_FLG &= 127;

  pActwk->PAT_ADR = &s_run_map;
  pActwk->SPR_TIMER = s_run_map.aPatDat[0].timer;
  pActwk->PAT_NO = 0;

  pActwk->free[1] = 0;
  pActwk->TM_CNT = 96;

  pActwk->EXE_NO = 13;
}

static void sonicpit(sprite_status_thanks* pActwk) {
  pit_in(pActwk);
}

static void sonicesc(sprite_status_thanks* pActwk) {
  sprite_status_thanks* pEmmy;
  sprite_status_thanks* pMetal;

  pEmmy = &actwk[1];
  pMetal = &actwk[2];
  if (pActwk->free[1] == 0)
  {
    --pActwk->TM_CNT;
    if (pActwk->TM_CNT > 0) return;
    pActwk->XPOSI.w.h = 448;

    pActwk->X_SPEED.l = 196608;
    pActwk->Y_SPEED.l = 0;

animal_flg = 2;
    actset_tm[0] = 1800;
    actset_tm[1] = 0;
    actset_tm[2] = 0;
    actset_tm[3] = 0;
    actset_tm[4] = 0;
    actset_tm[5] = 0;
    actset_tm[6] = 0;
    actset_tm[7] = 1800;

    pActwk->SPR_FLG &= 251;
    pEmmy->SPR_FLG &= 251;
    pMetal->SPR_FLG &= 251;

    pActwk->free[1] = 1;
  }
  else
  {
    escape_chk(pActwk);
  }
}

void emie_ctrl(sprite_status_thanks* pActwk) {
  sprite_status_thanks* pSonic;
  void(*acttbl[16])(sprite_status_thanks*) =
  {
    &emmyinit,
    &e_runinit,
    &emmymove,
    &e_kissset,
    &emmykiss,
    &e_jumpset,
    &emmyjump,
    &e_catchset,
    &emmy_catch,
    &e_turnset,
    &emmyturn,
    &e_standset,
    &emmymoji,
    &emmyexit,
    &emmypose,
    &emmyesc
  };

  pSonic = &actwk[0];
  if (animal_flg)
  {
    pActwk->SPR_FLG |= 4;
  }
  emmy_exeset(pActwk, pSonic);
  acttbl[pActwk->EXE_NO](pActwk);
  pActwk->XPOSI.w.h &= 511;
}

static void emmy_exeset(sprite_status_thanks* pActwk, sprite_status_thanks* pSonic) {
  int_union ld0;
  Sint16 d5, d6;

  if (pActwk->EXE_NO != 2) return;

  if (pSonic->ACT_NO == 0)
  {
    pActwk->EXE_NO = 15;
    return;
  }
  chk_sonicposi(pActwk, pSonic, &d5, &d6);
  if (d5 <= 40)
  {
    if (pSonic->EXE_NO >= 5)
    {
      if (pSonic->EXE_NO < 9)
      {
        pActwk->EXE_NO = 5;
        return;
      }
    }
  }
  ld0.l = random();
  ld0.l &= 32767;
  ld0.w.l = (Sint32)ld0.w.l % 256;
  if (ld0.w.l == 0)
  {
    if (pActwk->XPOSI.w.h == 384)
    {
      pActwk->XPOSI.w.h = 394;
    }
    chk_sonicposi(pActwk, pSonic, &d5, &d6);
    if (d5 >= 40)
    {
      if (myposichk(pActwk) == 0)
      {
        pActwk->TM_CNT = 64;
        pActwk->EXE_NO = 11;
        return;
      }
    }
  }
  ld0.w.l = 0;
  if (pActwk->X_SPEED.l < 0)
  {
    ld0.w.l = 1;
  }
  if (d6 != ld0.w.l)
  {
    pActwk->EXE_NO = 9;
  }
}

static void emmyinit(sprite_status_thanks* pActwk) {
  pActwk->XPOSI.w.h = -10;
  pActwk->YPOSI.w.h = 141;

  pActwk->X_SPEED.l = 147456;
  pActwk->Y_SPEED.l = 0;

  pActwk->CGBASE = 967;
  pActwk->free[0] = 1;

  pActwk->Y_ACCEL.l = 0;

  pActwk->PAT_ADR = &erun_map;
  pActwk->SPR_TIMER = erun_map.aPatDat[0].timer;
  pActwk->PAT_NO = 0;

  pActwk->EXE_NO = 2;
}

static void e_runinit(sprite_status_thanks* pActwk) {
  Sint16 d5, d6;

  pActwk->XPOSI.l &= 33554431;
  pActwk->X_SPEED.l = 147456;
  pActwk->Y_SPEED.l = 0;

  if (chk_sonicposi(pActwk, &actwk[0], &d5, &d6))
  {
    pActwk->X_SPEED.l *= -1;
    pActwk->SPR_FLG |= 128;
  }
  else
  {
    pActwk->SPR_FLG &= 127;
  }
  pActwk->CGBASE = 967;
  pActwk->X_OFFSET = 0;
  pActwk->X_ACCEL.l = 0;
  pActwk->Y_OFFSET = 0;
  pActwk->Y_ACCEL.l = 0;

  pActwk->PAT_ADR = &erun_map;
  pActwk->SPR_TIMER = erun_map.aPatDat[0].timer;
  pActwk->PAT_NO = 0;

  pActwk->EXE_NO = 2;
}

static void emmymove(sprite_status_thanks* pActwk) {
  hbt_move(pActwk);
  while (pActwk->XPOSI.w.h == 384)
  {
    hbt_move(pActwk);
  }
  if (pActwk->SPR_FLG & 4)
  {
    pActwk->EXE_NO = 13;
  }
}

static void e_kissset(sprite_status_thanks* pActwk) {
  sprite_status_thanks* pNewActwk;
  Sint16 d5, d6;

  pActwk->XPOSI.l &= 33554431;
  pActwk->YPOSI.w.h = 141;

  pActwk->X_SPEED.l = 0;
  pActwk->Y_SPEED.l = -196608;

  pActwk->X_OFFSET = 0;
  pActwk->X_ACCEL.l = 0;
  pActwk->Y_OFFSET = 16384;
  pActwk->Y_ACCEL.l = 0;

  pActwk->PAT_ADR = &ekiss_map;
  pActwk->PAT_NO = 0;
  pActwk->SPR_TIMER = ekiss_map.aPatDat[0].timer;

  if (chk_sonicposi(pActwk, &actwk[0], &d5, &d6)) pActwk->SPR_FLG |= 128;
  else pActwk->SPR_FLG &= 127;
  pActwk->free[4] = 12;
  if (get_actwk(&pNewActwk) == 0)
  {
    heart_tm = 1;
    pNewActwk->ACT_NO = 7;
  }
  pActwk->EXE_NO = 4;
}

static void emmykiss(sprite_status_thanks* pActwk) {
  sprite_status_thanks* pSonic;
  sprite_status_thanks* pMetal;
  sprite_status_thanks* pNewActwk;

  pSonic = &actwk[0];
  pMetal = &actwk[2];

  hbt_move(pActwk);
  if (pActwk->YPOSI.w.h >= 140)
  {
    pActwk->Y_ACCEL.l = 0;
  }
  --pActwk->TM_CNT;
  if (pActwk->TM_CNT <= 0)
  {
    if (pActwk->Y_ACCEL.l == 0)
    {
      pActwk->YPOSI.w.h = 139;
      pActwk->EXE_NO = 1;

      if (poseno == 2)
      {
        pSonic->SPR_FLG &= 251;
        pMetal->SPR_FLG &= 251;
        poseno = 0;
      }
    }
  }
  --((char*)pActwk)[62];
  if (((char*)pActwk)[62] < 0)
  {
    if (get_actwk(&pNewActwk) == 0)
    {
      pNewActwk->ACT_NO = 7;
      ((char*)pActwk)[62] = 16;
    }
  }
}

static void e_jumpset(sprite_status_thanks* pActwk) {
  sprite_status_thanks* pNewActwk;
  Sint16 d5, d6;

  pActwk->XPOSI.l &= 33554431;
  pActwk->YPOSI.w.h = 141;

  pActwk->X_SPEED.l = 98304;
  pActwk->Y_SPEED.l = -196608;

  pActwk->X_OFFSET = 0;
  pActwk->X_ACCEL.l = 0;
  pActwk->Y_OFFSET = 16384;
  pActwk->Y_ACCEL.l = 0;

  pActwk->PAT_ADR = &ejump_map;
  pActwk->PAT_NO = 0;
  pActwk->SPR_TIMER = ejump_map.aPatDat[0].timer;

  if (chk_sonicposi(pActwk, &actwk[0], &d5, &d6))
  {
    pActwk->X_SPEED.l *= -1;
    pActwk->SPR_FLG |= 128;
  }
  else
  {
    pActwk->SPR_FLG &= 127;
  }

if (pActwk->SPR_FLG & 128)
  {
    if (pActwk->SPR_FLG & 128)
    {
      pActwk->X_SPEED.l = -65536;
    }
  }
  else
  {
    if (!(pActwk->SPR_FLG & 128))
    {
      pActwk->X_SPEED.l = 65536;
    }
  }
  pActwk->TM_CNT = 20;
  pActwk->free[4] = 16;
  if (get_actwk(&pNewActwk) == 0)
  {
    heart_tm = 1;
    pNewActwk->ACT_NO = 7;
  }
  pActwk->EXE_NO = 6;
}

static void emmyjump(sprite_status_thanks* pActwk) {
  sprite_status_thanks* pNewActwk;

  hbt_move(pActwk);
  if (pActwk->YPOSI.w.h >= 140)
  {
    pActwk->Y_ACCEL.l = 0;
  }
  --pActwk->TM_CNT;
  if (pActwk->TM_CNT <= 0)
  {
    if (pActwk->Y_ACCEL.l == 0)
    {
      pActwk->YPOSI.w.h = 139;
      pActwk->EXE_NO = 7;
    }
  }
  --((char*)pActwk)[62];
  if (((char*)pActwk)[62] < 0)
  {
    if (get_actwk(&pNewActwk) == 0)
    {
      pNewActwk->ACT_NO = 7;
      ((char*)pActwk)[62] = 16;
    }
  }
}

static void e_catchset(sprite_status_thanks* pActwk) {
  sprite_status_thanks* pSonic;
  sprite_status_thanks* pNewActwk;

  pSonic = &actwk[0];

  pActwk->XPOSI.l &= 33554431;
  pActwk->YPOSI.w.h = 141;

  if (pActwk->SPR_FLG & 128)
  {
    pActwk->SPR_FLG |= 128;
    pActwk->XPOSI.l = pSonic->XPOSI.l;
    pActwk->XPOSI.w.h += 16;
    pActwk->X_SPEED.l *= -1;
  }
  else
  {
    pActwk->SPR_FLG &= 127;
    pActwk->XPOSI.l = pSonic->XPOSI.l;
    pActwk->XPOSI.w.h -= 16;
  }
  pActwk->X_SPEED.l = 0;
  pActwk->Y_SPEED.l = 0;

  pActwk->X_OFFSET = 0;
  pActwk->X_ACCEL.l = 0;
  pActwk->Y_OFFSET = 0;
  pActwk->Y_ACCEL.l = 0;

  pActwk->PAT_ADR = &ecatch_map;
  pActwk->PAT_NO = 0;
  pActwk->SPR_TIMER = ecatch_map.aPatDat[0].timer;

  pActwk->free[0] = 0;

  pActwk->free[4] = 12;
  if (get_actwk(&pNewActwk) == 0)
  {
    heart_tm = 1;
    pNewActwk->ACT_NO = 7;
  }
  pActwk->EXE_NO = 8;
}

static void emmy_catch(sprite_status_thanks* pActwk) {
  sprite_status_thanks* pSonic;
  sprite_status_thanks* pNewActwk;

  pSonic = &actwk[0];

  if (pSonic->SPR_FLG & 128)
  {
    pActwk->SPR_FLG |= 128;
    pActwk->XPOSI.l = pSonic->XPOSI.l;
    pActwk->XPOSI.w.h += 16;
    pActwk->X_SPEED.l *= -1;
  }
  else
  {
    pActwk->SPR_FLG &= 127;
    pActwk->XPOSI.l = pSonic->XPOSI.l;
    pActwk->XPOSI.w.h -= 16;
  }

  if (pSonic->EXE_NO >= 5)
  {
    if (pSonic->EXE_NO < 9)
    {
      --((char*)pActwk)[62];
      if (((char*)pActwk)[62] >= 0) return;

if (get_actwk(&pNewActwk) == 0)
      {
        pNewActwk->ACT_NO = 7;
        ((char*)pActwk)[62] = 16;
        return;
      }
    }
  }
  pActwk->TM_CNT = 40;
  pActwk->EXE_NO = 3;
  pActwk->free[0] = 1;
}

static void e_turnset(sprite_status_thanks* pActwk) {
  Sint16 d5, d6;

  pActwk->YPOSI.w.h = 141;

  pActwk->X_SPEED.l = 147456;
  pActwk->Y_SPEED.l = 0;

  pActwk->X_OFFSET = -8192;
  pActwk->X_ACCEL.l = 0;
  pActwk->Y_OFFSET = 0;
  pActwk->Y_ACCEL.l = 0;

  pActwk->PAT_ADR = &erun_map;
  pActwk->PAT_NO = 0;
  pActwk->SPR_TIMER = erun_map.aPatDat[0].timer;

  pActwk->SPR_FLG |= 128;
  if (chk_sonicposi(pActwk, &actwk[0], &d5 ,&d6) == 0)
  {
    pActwk->SPR_FLG &= 127;
    pActwk->X_OFFSET *= -1;
    pActwk->X_SPEED.l *= -1;
  }
  pActwk->EXE_NO = 10;
}

static void emmyturn(sprite_status_thanks* pActwk) {
  Sint16 d0;

  hbt_move(pActwk);
  d0 = pActwk->X_SPEED.w.h;
  d0 += pActwk->X_ACCEL.w.h;
  if (d0 == 0)
  {
    if (poseno == 2) pActwk->TM_CNT = 80;
    else pActwk->TM_CNT = 20;
    pActwk->EXE_NO = 3;
  }
}

static void e_standset(sprite_status_thanks* pActwk) {
  sprite_status_thanks* pNewActwk;
  int_union ld0;
  Sint16 d5, d6;

  pActwk->XPOSI.l &= 33554431;
  pActwk->YPOSI.w.h = 141;
  pActwk->X_SPEED.l = 0;
  pActwk->Y_SPEED.l = 0;

  pActwk->X_OFFSET = 0;
  pActwk->X_ACCEL.l = 0;
  pActwk->Y_OFFSET = 0;
  pActwk->Y_ACCEL.l = 0;

  ld0.l = random();
  ld0.l &= 32767;
  ld0.w.l = (Sint32)ld0.w.l % 2;
  if (ld0.w.l == 0)
  {
    pActwk->PAT_ADR = &estand_map;
    pActwk->PAT_NO = 0;
    pActwk->SPR_TIMER = estand_map.aPatDat[0].timer;
  }
  else
  {
    pActwk->PAT_ADR = &estand2_map;
    pActwk->PAT_NO = 0;
    pActwk->SPR_TIMER = estand2_map.aPatDat[0].timer;
  }

  if (chk_sonicposi(pActwk, &actwk[0], &d5, &d6)) pActwk->SPR_FLG |= 128;
  else pActwk->SPR_FLG &= 127;
  pActwk->free[4] = 16;
  if (get_actwk(&pNewActwk) == 0)
  {
    heart_tm = 1;
    pNewActwk->ACT_NO = 7;
  }
  pActwk->EXE_NO = 12;
}

static void emmymoji(sprite_status_thanks* pActwk) {
  sprite_status_thanks* pSonic;
  Sint16 d5, d6;

  pSonic = &actwk[0];

  pActwk->XPOSI.w.h &= 511;
  pSonic->XPOSI.w.h &= 511;
  if (chk_sonicposi(pActwk, &actwk[0], &d5, &d6)) pActwk->SPR_FLG |= 128;
  else pActwk->SPR_FLG &= 127;
  --pActwk->TM_CNT;
  if (pActwk->TM_CNT <= 0)
  {
    pActwk->EXE_NO = 1;
  }
}

static void emmyexit(sprite_status_thanks* pActwk) {
  waitmode(pActwk);
  if (animal_flg == 2)
  {
    pActwk->XPOSI.w.h = 448;
    pActwk->X_SPEED.l = 147456;
    pActwk->Y_SPEED.l = 0;
    pActwk->SPR_FLG &= 127;

    pActwk->PAT_ADR = &erun_map;
    pActwk->SPR_TIMER = erun_map.aPatDat[0].timer;
    pActwk->PAT_NO = 0;

    pActwk->EXE_NO = 15;
  }
}

static void emmypose(sprite_status_thanks* pActwk) {
  sprite_status_thanks* pSonic;
  sprite_status_thanks* pMetal;

  pSonic = &actwk[0];

  pActwk->XPOSI.l += pActwk->X_SPEED.l;
  pActwk->YPOSI.l += pActwk->Y_SPEED.l;

  if (pSonic->X_SPEED.l == 0)
  {
    pMetal = &actwk[2];
    if (pMetal->X_SPEED.l == 0)
    {
      if (centerchk(pActwk))
      {
        pActwk->EXE_NO = 9;
      }
    }
  }
}

static void emmyesc(sprite_status_thanks* pActwk) {
  escape_chk(pActwk);
}

void metals_ctrl(sprite_status_thanks* pActwk) {
  sprite_status_thanks* pSonic;
  void(*acttbl[16])(sprite_status_thanks*) =
  {
    &metalsinit,
    &m_flyset,
    &metalsfly,
    &m_brakeset,
    &metalsbrake,
    &m_stopset,
    &metalsstop,
    &m_standset,
    &metalstand,
    &m_startset,
    &metalstartcttbl,
    &m_eraset,
    &metalsera,
    &metalsexit,
    &metalspit,
    &metalsesc
  };

  pSonic = &actwk[0];
  if (animal_flg)
  {
    pActwk->SPR_FLG |= 4;
  }
  acttbl[pActwk->EXE_NO](pActwk);
  pActwk->XPOSI.w.h &= 511;
}

static void metalsinit(sprite_status_thanks* pActwk) {
  int_union ld0;

  pActwk->XPOSI.w.h = 336;
  pActwk->YPOSI.w.h = 132;

  pActwk->X_SPEED.l = -237568;
  pActwk->Y_SPEED.l = 0;

  pActwk->X_OFFSET = 0;
  pActwk->X_ACCEL.l = 0;
  pActwk->Y_OFFSET = 0;
  pActwk->Y_ACCEL.l = 0;

  pActwk->CGBASE = 25785;
  pActwk->free[0] = 3;
  pActwk->SPR_FLG |= 128;

  pActwk->PAT_ADR = &m_fly1_map;
  pActwk->SPR_TIMER = m_fly1_map.aPatDat[0].timer;
  pActwk->PAT_NO = 0;

  ld0.l = random();
  ld0.l &= 32767;
  ld0.w.l = (Sint32)ld0.w.l % 128;
  pActwk->TM_CNT = ld0.w.l;
  pActwk->TM_CNT += 128;

  pActwk->EXE_NO = 2;
}

static void m_flyset(sprite_status_thanks* pActwk) {
  int_union ld0;

  pActwk->YPOSI.w.h = 132;
  pActwk->X_SPEED.l = 237568;
  pActwk->Y_SPEED.l = 0;

  if (pActwk->SPR_FLG & 128)
  {
    pActwk->X_SPEED.l *= -1;
  }

  pActwk->X_OFFSET = 0;
  pActwk->X_ACCEL.l = 0;
  pActwk->Y_OFFSET = 0;
  pActwk->Y_ACCEL.l = 0;

  pActwk->CGBASE = 25785;
  pActwk->free[0] = 3;

  pActwk->PAT_ADR = &m_fly1_map;
  pActwk->SPR_TIMER = m_fly1_map.aPatDat[0].timer;
  pActwk->PAT_NO = 0;

  ld0.l = random();
  ld0.l &= 32767;
  ld0.w.l = (Sint32)ld0.w.l % 192;
  pActwk->TM_CNT = ld0.w.l;
  pActwk->TM_CNT += 128;

  if (dsplaychk(pActwk) == 0)
  {
    pActwk->free[2] = 1;
  }
  pActwk->EXE_NO = 2;
}

static void metalsfly(sprite_status_thanks* pActwk) {
  sprite_status_thanks* pSonic;
  Sint16 d5, d6;

  pSonic = &actwk[0];

  pActwk->XPOSI.l += pActwk->X_SPEED.l;
  pActwk->YPOSI.l += pActwk->Y_SPEED.l;

  m_sndchk(pActwk);

  if (pActwk->SPR_FLG & 4)
  {
    pActwk->free[2] = 0;
    pActwk->EXE_NO = 13;
    return;
  }
  if (pSonic->ACT_NO == 0)
  {
    pActwk->free[2] = 0;
    pActwk->EXE_NO = 15;
    return;
  }
  if (pActwk->XPOSI.w.h >= 376)
  {
    if (pActwk->XPOSI.w.h <= 392)
    {
      pActwk->free[2] = 0;
      ((Sint16*)pActwk)[31] = 128;
      pActwk->EXE_NO = 14;
      return;
    }
  }
  --pActwk->TM_CNT;
  if (pActwk->TM_CNT < 0)
  {
    pActwk->XPOSI.l &= 33554431;
    pActwk->TM_CNT = 0;

    chk_sonicposi(pActwk, &actwk[0], &d5, &d6);
    if (d5 < 40) return;

    chk_sonicposi(pActwk, &actwk[1], &d5, &d6);
    if (d5 < 40) return;

    if (myposichk(pActwk)) return;
    pActwk->free[2] = 0;
    pActwk->EXE_NO = 3;
  }
}

static void m_brakeset(sprite_status_thanks* pActwk) {
  pActwk->YPOSI.w.h = 136;

  pActwk->X_OFFSET = -14336;
  if (pActwk->SPR_FLG & 128)
  {
    pActwk->X_OFFSET *= -1;
  }

  pActwk->X_ACCEL.l = 0;
  pActwk->Y_OFFSET = 0;
  pActwk->Y_ACCEL.l = 0;

  pActwk->PAT_ADR = &m_brake_map;
  pActwk->SPR_TIMER = m_brake_map.aPatDat[0].timer;
  pActwk->PAT_NO = 0;
  pActwk->EXE_NO = 4;
}

static void metalsbrake(sprite_status_thanks* pActwk) {
  Sint16 d0;

  hbt_move(pActwk);
  d0 = pActwk->X_SPEED.w.h;
  d0 += pActwk->X_ACCEL.w.h;
  if (d0 == 0)
  {
    pActwk->EXE_NO = 5;
  }
}

static void m_stopset(sprite_status_thanks* pActwk) {
  pActwk->XPOSI.l &= 33554431;

  if (pActwk->XPOSI.w.h == 384)
  {
    pActwk->XPOSI.w.h = 394;
  }

  pActwk->YPOSI.w.h = 140;
  pActwk->X_SPEED.l = 0;
  pActwk->Y_SPEED.l = 0;

  pActwk->X_OFFSET = 0;
  pActwk->X_ACCEL.l = 0;
  pActwk->Y_OFFSET = 0;
  pActwk->Y_ACCEL.l = 0;

  pActwk->PAT_ADR = &m_stop_map;
  pActwk->SPR_TIMER = m_stop_map.aPatDat[0].timer;
  pActwk->PAT_NO = 0;

  pActwk->TM_CNT = 32;

  pActwk->EXE_NO = 6;
}

static void metalsstop(sprite_status_thanks* pActwk) {
  sprite_status_thanks* pSonic;

  pSonic = &actwk[0];

  --pActwk->TM_CNT;
  if (pActwk->TM_CNT < 0)
  {
    if (pSonic->EXE_NO == 12) goto label1;
    if (pSonic->EXE_NO < 5) goto label2;
    if (pSonic->EXE_NO >= 9) goto label2;
label1:
    pActwk->EXE_NO = 11;
    return;
label2:
    pActwk->EXE_NO = 7;
  }

}

static void m_standset(sprite_status_thanks* pActwk) {
  pActwk->XPOSI.l &= 33554431;
  pActwk->X_SPEED.l = 0;
  pActwk->Y_SPEED.l = 0;

  pActwk->X_OFFSET = 0;
  pActwk->X_ACCEL.l = 0;
  pActwk->Y_OFFSET = 0;
  pActwk->Y_ACCEL.l = 0;

  pActwk->PAT_ADR = &m_stand_map;
  pActwk->SPR_TIMER = m_stand_map.aPatDat[0].timer;
  pActwk->PAT_NO = 0;

  pActwk->TM_CNT = 64;
  pActwk->EXE_NO = 8;
}

static void metalstand(sprite_status_thanks* pActwk) {
  Sint16 d5, d6;

  --pActwk->TM_CNT;
  if (pActwk->TM_CNT < 0)
  {
    pActwk->EXE_NO = 9;
  }
  if (chk_sonicposi(pActwk, &actwk[0], &d5, &d6)) pActwk->SPR_FLG |= 128;
  else pActwk->SPR_FLG &= 127;
}

static void m_startset(sprite_status_thanks* pActwk) {
  Sint16 d5, d6;

  pActwk->XPOSI.l &= 33554431;
  pActwk->X_SPEED.l = 0;
  pActwk->Y_SPEED.l = 0;

  pActwk->X_OFFSET = 0;
  pActwk->X_ACCEL.l = 0;
  pActwk->Y_OFFSET = 0;
  pActwk->Y_ACCEL.l = 0;

  if (chk_sonicposi(pActwk, &actwk[0], &d5, &d6)) pActwk->SPR_FLG |= 128;
  else pActwk->SPR_FLG &= 127;

  pActwk->PAT_ADR = &m_furi_map;
  pActwk->SPR_TIMER = m_furi_map.aPatDat[0].timer;
  pActwk->PAT_NO = 0;

  pActwk->TM_CNT = 16;

  pActwk->free[3] = 0;

  pActwk->EXE_NO = 10;
}

static void metalstartcttbl(sprite_status_thanks* pActwk) {
  Sint16 d5, d6;

  --pActwk->TM_CNT;
  if (pActwk->TM_CNT < 0)
  {
    if (pActwk->free[3] == 0)
    {
      pActwk->YPOSI.w.h = 136;
      pActwk->PAT_ADR = &m_kamae_map;
      pActwk->SPR_TIMER = m_kamae_map.aPatDat[0].timer;
      pActwk->PAT_NO = 0;

      pActwk->TM_CNT = 48;
      pActwk->free[3] = 1;
    }
    else
    {
      pActwk->EXE_NO = 1;
    }
  }
  if (chk_sonicposi(pActwk, &actwk[0], &d5, &d6)) pActwk->SPR_FLG |= 128;
  else pActwk->SPR_FLG &= 127;
}

static void m_eraset(sprite_status_thanks* pActwk) {
  pActwk->XPOSI.l &= 33554431;
  pActwk->X_SPEED.l = 0;
  pActwk->Y_SPEED.l = 0;

  pActwk->X_OFFSET = 0;
  pActwk->X_ACCEL.l = 0;
  pActwk->Y_OFFSET = 0;
  pActwk->Y_ACCEL.l = 0;

  pActwk->PAT_ADR = &m_era_map;
  pActwk->SPR_TIMER = m_era_map.aPatDat[0].timer;
  pActwk->PAT_NO = 0;

  pActwk->TM_CNT = 32;

  pActwk->free[3] = 0;
  pActwk->EXE_NO = 12;
}

static void metalsera(sprite_status_thanks* pActwk) {
  --pActwk->TM_CNT;
  if (pActwk->TM_CNT < 0)
  {
    if (pActwk->free[3] == 0)
    {
      pActwk->PAT_ADR = &m_era2_map;
      pActwk->SPR_TIMER = m_era2_map.aPatDat[0].timer;
      pActwk->PAT_NO = 0;

      pActwk->TM_CNT = 128;
      pActwk->free[3] = 1;
    }
    else
    {
      pActwk->EXE_NO = 9;
    }
  }
}

static void metalsexit(sprite_status_thanks* pActwk) {
  waitmode(pActwk);

  if (animal_flg == 2)
  {
    pActwk->XPOSI.w.h = 448;
    pActwk->X_SPEED.l = 237568;
    pActwk->Y_SPEED.l = 0;
    pActwk->SPR_FLG &= 127;
    pActwk->PAT_ADR = &m_fly1_map;
    pActwk->SPR_TIMER = m_fly1_map.aPatDat[0].timer;
    pActwk->PAT_NO = 0;
    pActwk->EXE_NO = 15;
  }
}

static void metalspit(sprite_status_thanks* pActwk) {
  pit_in(pActwk);
}

static void metalsesc(sprite_status_thanks* pActwk) {
  escape_chk(pActwk);
}

void heart_ctrl(sprite_status_thanks* pActwk) {
  void(*acttbl[2])(sprite_status_thanks*) =
  {
    &heartinit,
    &heartmove
  };

  acttbl[pActwk->EXE_NO](pActwk);
}

static void heartinit(sprite_status_thanks* pActwk) {
  sprite_status_thanks* pEmmy;

  pEmmy = &actwk[1];

  pActwk->XPOSI.w.h = pEmmy->XPOSI.w.h;
  pActwk->YPOSI.w.h = pEmmy->YPOSI.w.h;

  pActwk->X_SPEED.l = 0;
  pActwk->Y_SPEED.l = -32768;

  pActwk->X_WIDE = 32;
  pActwk->Y_WIDE = 4;
  pActwk->X_OFFSET = 0;
  pActwk->Y_OFFSET = 0;

  pActwk->CGBASE = 967;
  pActwk->free[0] = 3;

  pActwk->PAT_ADR = &eheart_map;
  pActwk->PAT_NO = 0;

  pActwk->free[2] = 32;

  pActwk->EXE_NO = 1;
}

static void heartmove(sprite_status_thanks* pActwk) {
  sinvmove(pActwk);
  if (pActwk->PAT_NO >= 3)
  {
    pActwk->SPR_FLG |= 16;
  }
}

void pocky_ctrl(sprite_status_thanks* pActwk) {
  void(*acttbl[2])(sprite_status_thanks*) =
  {
    &pockyinit,
    &animalmove
  };

  acttbl[pActwk->EXE_NO](pActwk);
}

static void pockyinit(sprite_status_thanks* pActwk) {
  int_union ld0;

  pActwk->XPOSI.w.h = -10;
  pActwk->YPOSI.w.h = 144;

  pActwk->X_SPEED.l = 49152;
  ld0.l = random();
  ld0.l &= 32767;
  ld0.w.h = ld0.w.l % 512;
  ld0.w.l = 0;
  ld0.l >>= 8;
  pActwk->X_SPEED.l += ld0.l;

  pActwk->Y_SPEED.l = -229376;
  ld0.l = random();
  ld0.l &= 32767;
  ld0.w.h = ld0.w.l % 512;
  ld0.w.l = 0;
  ld0.l >>= 8;
  pActwk->Y_SPEED.l += ld0.l;

pActwk->Y_OFFSET = 14336;
  pActwk->Y_ACCEL.l = 0;
  pActwk->CGBASE = 25989;

  pActwk->free[0] = 3;
  pActwk->PAT_ADR = &pocky_map;
  pActwk->SPR_TIMER = pocky_map.aPatDat[0].timer;
  pActwk->PAT_NO = 0;
  pActwk->EXE_NO = 1;
}

void pecky_ctrl(sprite_status_thanks* pActwk) {
  void(*acttbl[2])(sprite_status_thanks*) =
  {
    &peckyinit,
    &animalmove
  };

  acttbl[pActwk->EXE_NO](pActwk);
}

static void peckyinit(sprite_status_thanks* pActwk) {
  int_union ld0;

  pActwk->XPOSI.w.h = -10;
  pActwk->YPOSI.w.h = 144;

  pActwk->X_SPEED.l = 49152;
  ld0.l = random();
  ld0.l &= 32767;
  ld0.w.h = ld0.w.l % 256;
  ld0.w.l = 0;
  ld0.l >>= 8;
  pActwk->X_SPEED.l += ld0.l;

  pActwk->Y_SPEED.l = -229376;
  ld0.l = random();
  ld0.l &= 32767;
  ld0.w.h = ld0.w.l % 256;
  ld0.w.l = 0;
  ld0.l >>= 8;
  pActwk->Y_SPEED.l += ld0.l;

  pActwk->Y_OFFSET = 14336;
  pActwk->Y_ACCEL.l = 0;
  pActwk->CGBASE = 25989;

  pActwk->free[0] = 3;
  pActwk->PAT_ADR = &pecky_map;
  pActwk->SPR_TIMER = pecky_map.aPatDat[0].timer;
  pActwk->PAT_NO = 0;
  pActwk->EXE_NO = 1;
}

void ricky_ctrl(sprite_status_thanks* pActwk) {
  void(*acttbl[2])(sprite_status_thanks*) =
  {
    &rickyinit,
    &animalmove
  };

  acttbl[pActwk->EXE_NO](pActwk);
}

static void rickyinit(sprite_status_thanks* pActwk) {
  int_union ld0;

  pActwk->XPOSI.w.h = -10;
  pActwk->YPOSI.w.h = 164;

  pActwk->X_SPEED.l = 49152;
  ld0.l = random();
  ld0.l &= 32767;
  ld0.w.h = ld0.w.l % 256;
  ld0.w.l = 0;
  ld0.l >>= 8;
  pActwk->X_SPEED.l += ld0.l;

  pActwk->Y_SPEED.l = -229376;
  ld0.l = random();
  ld0.l &= 32767;
  ld0.w.h = ld0.w.l % 256;
  ld0.w.l = 0;
  ld0.l >>= 8;
  pActwk->Y_SPEED.l += ld0.l;

  pActwk->Y_OFFSET = 14336;
  pActwk->Y_ACCEL.l = 0;

  pActwk->CGBASE = 25989;
  pActwk->free[0] = 3;
  pActwk->PAT_ADR = &ricky_map;
  pActwk->SPR_TIMER = ricky_map.aPatDat[0].timer;
  pActwk->PAT_NO = 0;
  pActwk->EXE_NO = 1;
}

void sheep_ctrl(sprite_status_thanks* pActwk) {
  void(*acttbl[2])(sprite_status_thanks*) =
  {
    &sheepinit,
    &animalmove
  };

  acttbl[pActwk->EXE_NO](pActwk);
}

static void sheepinit(sprite_status_thanks* pActwk) {
  int_union ld0;

  pActwk->XPOSI.w.h = -10;
  pActwk->YPOSI.w.h = 164;

  pActwk->X_SPEED.l = 49152;
  ld0.l = random();
  ld0.l &= 32767;
  ld0.w.h = ld0.w.l % 512;
  ld0.w.l = 0;
  ld0.l >>= 8;
  pActwk->X_SPEED.l += ld0.l;

  pActwk->Y_SPEED.l = -229376;
  ld0.l = random();
  ld0.l &= 32767;
  ld0.w.h = ld0.w.l % 256;
  ld0.w.l = 0;
  ld0.l >>= 8;
  pActwk->Y_SPEED.l += ld0.l;

  pActwk->Y_OFFSET = 16384;
  pActwk->Y_ACCEL.l = 0;

  pActwk->CGBASE = 25989;
  pActwk->free[0] = 3;
  pActwk->PAT_ADR = &sheep_map;
  pActwk->SPR_TIMER = sheep_map.aPatDat[0].timer;
  pActwk->PAT_NO = 0;
  pActwk->EXE_NO = 1;
}

static void animalmove(sprite_status_thanks* pActwk) {
  if (killchk(pActwk))
  {
    pActwk->SPR_FLG |= 16;
  }
  else
  {
    hbt_move(pActwk);
    if (pActwk->YPOSI.w.h >= 160)
    {
      pActwk->Y_ACCEL.l = 0;
    }
  }
}

void flick_ctrl(sprite_status_thanks* pActwk) {
  void(*acttbl[2])(sprite_status_thanks*) =
  {
    &flickyinit,
    &birdmove
  };

  acttbl[pActwk->EXE_NO](pActwk);
}

static void flickyinit(sprite_status_thanks* pActwk) {
  int_union ld0;

  pActwk->XPOSI.w.h = -10;
  ld0.l = random();
  ld0.l &= 32767;
  ld0.w.l = (Sint32)ld0.w.l % 120;
  pActwk->YPOSI.w.h = ld0.w.l;

  pActwk->X_SPEED.l = 65536;
  pActwk->Y_SPEED.l = 0;

  pActwk->X_WIDE = 48;
  pActwk->Y_WIDE = 4;
  pActwk->X_OFFSET = 0;
  pActwk->Y_OFFSET = 0;

  pActwk->CGBASE = 25989;
  pActwk->free[0] = 3;

  pActwk->PAT_ADR = &flick_map;
  pActwk->SPR_TIMER = flick_map.aPatDat[0].timer;
  pActwk->PAT_NO = 0;

  pActwk->EXE_NO = 1;
}

void inco_ctrl(sprite_status_thanks* pActwk) {
  void(*acttbl[2])(sprite_status_thanks*) =
  {
    &incoinit,
    &birdmove
  };

  acttbl[pActwk->EXE_NO](pActwk);
}

static void incoinit(sprite_status_thanks* pActwk) {
  int_union ld0;

  pActwk->XPOSI.w.h = -10;
  ld0.l = random();
  ld0.l &= 32767;
  ld0.w.l = (Sint32)ld0.w.l % 120;
  pActwk->YPOSI.w.h = ld0.w.l;

  pActwk->X_SPEED.l = 32768;
  ld0.l = random();
  ld0.l &= 32767;
  ld0.w.h = ld0.l % 256;
  ld0.w.l = 0;
  ld0.l >>= 8;
  pActwk->X_SPEED.l += ld0.l;
  pActwk->Y_SPEED.l = 0;

  pActwk->X_WIDE = 48;
  pActwk->Y_WIDE = 4;
  pActwk->X_OFFSET = 0;
  pActwk->Y_OFFSET = 0;

  pActwk->CGBASE = 25989;
  pActwk->free[0] = 3;

  pActwk->PAT_ADR = &inco_map;
  pActwk->SPR_TIMER = inco_map.aPatDat[0].timer;
  pActwk->PAT_NO = 0;

  pActwk->EXE_NO = 1;
}

void hato_ctrl(sprite_status_thanks* pActwk) {
  void(*acttbl[2])(sprite_status_thanks*) =
  {
    &hatoinit,
    &birdmove
  };

  acttbl[pActwk->EXE_NO](pActwk);
}

static void hatoinit(sprite_status_thanks* pActwk) {
  int_union ld0;

  pActwk->XPOSI.w.h = -10;
  ld0.l = random();
  ld0.l &= 32767;
  ld0.w.l = (Sint32)ld0.w.l % 120;
  pActwk->YPOSI.w.h = ld0.w.l;

  pActwk->X_SPEED.l = 32768;
  ld0.l = random();
  ld0.l &= 32767;
  ld0.w.h = ld0.w.l % 256;
  ld0.w.l = 0;
  ld0.l >>= 8;
  pActwk->X_SPEED.l += ld0.l;
  pActwk->Y_SPEED.l = 0;

  pActwk->X_WIDE = 48;
  pActwk->Y_WIDE = 4;
  pActwk->X_OFFSET = 0;
  pActwk->Y_OFFSET = 0;

  pActwk->CGBASE = 25989;
  pActwk->free[0] = 3;

  pActwk->PAT_ADR = &hato_map;
  pActwk->SPR_TIMER = hato_map.aPatDat[0].timer;
  pActwk->PAT_NO = 0;

  pActwk->EXE_NO = 1;
}

static void birdmove(sprite_status_thanks* pActwk) {
  if (killchk(pActwk))
  {
    pActwk->SPR_FLG |= 16;
  }
  else
  {
    sinhmove(pActwk);
  }
}

static Sint16 killchk(sprite_status_thanks* pActwk) {
  if (pActwk->XPOSI.w.h < -31) return -1;
  if (pActwk->XPOSI.w.h >= 336) return -1;
  if (pActwk->YPOSI.w.h < -4) return -1;
  if (pActwk->YPOSI.w.h >= 224) return -1;
  return 0;
}

static void sinhmove(sprite_status_thanks* pActwk) {
  int_union ld0, ld3;
  Sint32 tmp;

  pActwk->YPOSI.w.h -= pActwk->Y_OFFSET;

  ld3.w.h = 0;
  ld3.w.l = sign(pActwk->SIN_CNT);
  ld0.w.h = 0;
  ld0.w.l = pActwk->Y_WIDE;
  tmp = 0;
  tmp = ld3.w.l * ld0.w.l;
  ld3.l = tmp;
  ld3.l >>= 8;
  pActwk->Y_OFFSET = ld3.w.l;
  pActwk->YPOSI.w.h += ld3.w.l;

  ld0.l = random();
  ld0.l &= 32767;
  ld0.w.l = (Sint32)ld0.w.l % pActwk->X_WIDE;
  pActwk->SIN_CNT += ld0.w.l;
  if (pActwk->SIN_CNT >= 511)
  {
    pActwk->SIN_CNT -= 511;
  }
  pActwk->XPOSI.l += pActwk->X_SPEED.l;

  pActwk->YPOSI.l += pActwk->Y_SPEED.l;
}

static void sinvmove(sprite_status_thanks* pActwk) {
  int_union ld0, ld3;

  pActwk->XPOSI.w.h -= pActwk->X_OFFSET;

  ld3.l = sign(pActwk->SIN_CNT);
  ld0.l = pActwk->X_WIDE;
  ld3.l *= ld0.l;
  ld3.l >>= 8;
  pActwk->X_OFFSET = ld3.w.l;
  pActwk->XPOSI.w.h += ld3.w.l;

  ld0.l = random();
  ld0.l &= 32767;
  ld0.w.l = (Sint32)ld0.w.l % pActwk->Y_WIDE;
  pActwk->SIN_CNT += ld0.w.l;
  if (pActwk->SIN_CNT >= 511)
  {
    pActwk->SIN_CNT -= 511;
  }
  pActwk->XPOSI.l += pActwk->X_SPEED.l;

  pActwk->YPOSI.l += pActwk->Y_SPEED.l;
}

static void hbt_move(sprite_status_thanks* pActwk) {
  pActwk->XPOSI.l += pActwk->X_SPEED.l + pActwk->X_ACCEL.l;
  pActwk->YPOSI.l += pActwk->Y_SPEED.l + pActwk->Y_ACCEL.l;
  pActwk->X_ACCEL.l += pActwk->X_OFFSET;
  pActwk->Y_ACCEL.l += pActwk->Y_OFFSET;
}

static Sint16 chk_sonicposi(sprite_status_thanks* pActwk, sprite_status_thanks* pA6, Sint16* d5, Sint16* d6) {
  *d5 = pA6->XPOSI.w.h;
  *d5 -= pActwk->XPOSI.w.h;
  if (*d5 < 0)
  {
    *d5 *= -1;
    *d6 = 1;
    return -1;
  }
  else
  {
    *d6 = 0;
    return 0;
  }
}

static Sint16 myposichk(sprite_status_thanks* pActwk) {
  if (pActwk->XPOSI.w.h == 384) return -1;
  if(pActwk->X_SPEED.w.h >= 0)
  {
    if (pActwk->XPOSI.w.h < 264) return 0;
    if (pActwk->XPOSI.w.h < 304) return -1;
    if (pActwk->XPOSI.w.h < 448) return 0;
    if (pActwk->XPOSI.w.h < 496) return -1;
    return 0;
  }
  else
  {
    if (pActwk->XPOSI.w.h < 40) return -1;
    if (pActwk->XPOSI.w.h < 304) return 0;
    if (pActwk->XPOSI.w.h < 352) return -1;
    if (pActwk->XPOSI.w.h > 496) return -1;
    return 0;
  }
}

static Sint16 centerchk(sprite_status_thanks* pActwk) {
  pActwk->XPOSI.l &= 33554431;

  if (!(pActwk->SPR_FLG & 128))
  {
    if (pActwk->XPOSI.w.h < 112) return 0;
    if (pActwk->XPOSI.w.h > 120) return 0;
    return -1;
  }
  else
  {
    if (pActwk->XPOSI.w.h < 168) return 0;
    if (pActwk->XPOSI.w.h > 176) return 0;
    return -1;
  }
}

static Sint16 dsplaychk(sprite_status_thanks* pActwk) {
  pActwk->XPOSI.l &= 33554431;
  if (pActwk->XPOSI.w.h <= 304) return 0;
  return -1;
}

static void waitmode(sprite_status_thanks* pActwk) {
  hbt_move(pActwk);
  pActwk->XPOSI.w.h &= 511;
  if (pActwk->XPOSI.w.h >= 336)
  {
    if (pActwk->XPOSI.w.h <= 352)
    {
      pActwk->X_SPEED.l = 0;
      pActwk->Y_SPEED.l = 0;
      pActwk->X_OFFSET = 0;
      pActwk->X_ACCEL.l = 0;
      pActwk->Y_OFFSET = 0;
      pActwk->Y_ACCEL.l = 0;
    }
  }
  if (!(pActwk->SPR_FLG & 4))
  {
    pActwk->SPR_FLG &= 251;
    pActwk->EXE_NO = 1;
  }
}

static void escape_chk(sprite_status_thanks* pActwk) {
  pActwk->XPOSI.w.h &= 511;
  if (pActwk->X_SPEED.l >= 0)
  {
    if (pActwk->XPOSI.w.h >= 304)
    {
      if (pActwk->XPOSI.w.h <= 368)
      {
        pActwk->SPR_FLG |= 16;
        return;
      }
    }
  }
  else
  {
    if (pActwk->XPOSI.w.h >= 448)
    {
      if (pActwk->XPOSI.w.h <= 480)
      {
        pActwk->SPR_FLG |= 16;
        return;
      }
    }
  }
  pActwk->XPOSI.l += pActwk->X_SPEED.l;
  pActwk->YPOSI.l += pActwk->Y_SPEED.l;
}

static void pit_in(sprite_status_thanks* pActwk) {
  pActwk->XPOSI.w.h = 390;
  if (((Sint16*)pActwk)[31] < 2)
  {
    --((Sint16*)pActwk)[31];
    if (pActwk->X_SPEED.l >= 0)
    {
      pActwk->XPOSI.w.h = 400;
    }
    else
    {
      pActwk->XPOSI.w.h = 368;
    }
    pActwk->EXE_NO = 2;
  }
  else
  {
    --((Sint16*)pActwk)[31];
  }
}

static void m_sndchk(sprite_status_thanks* pActwk) {
  if (pActwk->free[2]) return;
  if (pActwk->X_SPEED.l >= 0)
  {
    if (pActwk->XPOSI.w.h >= 6) return;
    pActwk->free[2] = 1;
  }
  else
  {
    if (pActwk->XPOSI.w.h < 304) return;
    if (pActwk->XPOSI.w.h > 312) return;
    pActwk->free[2] = 1;
  }
}
