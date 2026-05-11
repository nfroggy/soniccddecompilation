#include "equ.h"
#include "dai_rd1.h"
#include "action.h"
#include "actset.h"
#include "dircol.h"
#include "dummy.h"
#include "etc.h"
#include "loader2.h"
#include "ridechk.h"

static void dodai_init(sprite_status* pActwk);
static void dodai_move(sprite_status* pActwk);
static void dodai_ud(sprite_status* pActwk);
static void dodai_lr(sprite_status* pActwk);
static Uint8 dodai_ride1(sprite_status* pActwk, Sint32 xpos);
static Uint8 dodai_ride2(sprite_status* pActwk);
static void dodai_nA(sprite_status* pActwk);
static void dodai_nB(sprite_status* pActwk);
static Uint8 dodai_fix(sprite_status* pActwk);
static void dodai_fal(sprite_status* pActwk);
static void dodai_up(sprite_status* pActwk);
static void dodai_upx(sprite_status* pActwk);
static void dodai_rm(sprite_status* pActwk);
static void dodai_lm(sprite_status* pActwk);
static Sint16 dodai_sub(sprite_status* pActwk);
static void dai_fout(sprite_status* pActwk);
static void vfuta_init(sprite_status* pActwk);
static void vfuta_move1(sprite_status* pActwk);
static void vfuta_move2(sprite_status* pActwk);

#if defined(R11B)
  #define SPRITE_DAI11A_BASE 518
  #define SPRITE_VFUTA_BASE 533
#elif defined(R11C) || defined(R11D)
  #define SPRITE_DAI11A_BASE 499
  #define SPRITE_VFUTA_BASE 514
#elif defined(R12A) || defined(R12B)
  #define SPRITE_DAI11A_BASE 509
  #define SPRITE_VFUTA_BASE 527
#elif defined(R12C) || defined(R12D)
  #define SPRITE_DAI11A_BASE 500
  #define SPRITE_VFUTA_BASE 518
#elif defined(R13C) || defined(R13D)
  #define SPRITE_DAI11A_BASE 488
  #define SPRITE_VFUTA_BASE 524
#else
  #define SPRITE_DAI11A_BASE 533
  #define SPRITE_VFUTA_BASE 548
#endif

static sprite_pattern dai11a_pat1 = {
  1,
  { { -16, -16, 0, SPRITE_DAI11A_BASE } }
};
static sprite_pattern dai11a_pat2 = {
  1,
  { { -32, -16, 0, SPRITE_DAI11A_BASE + 1 } }
};
static sprite_pattern dai11a_pat3 = {
  1,
  { { -48, -16, 0, SPRITE_DAI11A_BASE + 2 } }
};
sprite_pattern* dai11a_pattbl[3] = {
  &dai11a_pat1,
  &dai11a_pat2,
  &dai11a_pat3
};
static void(*dodai_acttbl[2])(sprite_status*) = {
  &dodai_init,
  &dodai_move
};
static Uint8 dai_hstbl[3] = { 16, 32, 48 };
static Uint8 dai_mcnttbl[4] = { 2, 3, 4, 6 };
static void(*dodai_mtype[10])(sprite_status*) = {
  &dodai_ud,
  &dodai_lr,
  &dodai_nA,
  &dodai_nB,
  (void(*)(sprite_status*))&dodai_fix,
  &dodai_fal,
  &dodai_up,
  &dodai_upx,
  &dodai_rm,
  &dodai_lm
};
static sprite_pattern vfuta_pat0 = {
  1,
  { { -4, -24, 0, SPRITE_VFUTA_BASE } }
};
static sprite_pattern vfuta_pat1 = {
  1,
  { { -4, -24, 0, SPRITE_VFUTA_BASE + 1 } }
};
sprite_pattern* vfutapat[2] = {
  &vfuta_pat0,
  &vfuta_pat1
};
static void(*vfuta_tbl[3])(sprite_status*) = {
  &vfuta_init,
  &vfuta_move1,
  &vfuta_move2
};

void dodai(sprite_status* pActwk) {
  dodai_acttbl[pActwk->r_no0 / 2](pActwk);
  actionsub(pActwk);
}

static void dodai_init(sprite_status* pActwk) {
  char wk;
  sprite_status* pNewact;

pActwk->actflg |= 4;
  pActwk->sproffset = 17598;
  pActwk->sprpri = 2;

((Sint16*)pActwk)[30] = pActwk->xposi.w.h;
  ((Sint16*)pActwk)[31] = pActwk->yposi.w.h;
  ((Sint16*)pActwk)[29] = pActwk->yposi.w.h;

  pActwk->patbase = dai11a_pattbl;
  wk = pActwk->userflag.b.h & 3;
  pActwk->patno = wk;

pActwk->sprhsize = dai_hstbl[wk];
  pActwk->sprvsize = 8;

  wk = (pActwk->userflag.b.h >> 2) & 3;
  pActwk->actfree[3] = dai_mcnttbl[wk];

pActwk->r_no0 += 2;
  if ((wk = pActwk->userflag.b.l) != 0) {

    if (actwkchk(&pNewact) != 0) {

      dai_fout(pActwk);
    }
    else {
      pNewact->actno = 10;
      pNewact->xposi.w.h = pActwk->xposi.w.h;
      pNewact->yposi.w.h = pActwk->yposi.w.h - 16;
      ((char*)(pNewact))[61] = -16;

((Uint16*)pNewact)[28] = pActwk - actwk;

pNewact->userflag.b.h = pActwk->userflag.b.l & 2;
      wk = (Uint8)pActwk->userflag.b.l & 248;
      pNewact->actfree[14] = wk;
      pNewact->xposi.w.h += (Uint16)wk;
    }
  }
}

static void dodai_move(sprite_status* pActwk) {
  Uint8 mtype;
  Uint16 xlen;

  if (time_stop) {
    ridechk(pActwk, &actwk[0]);
    return;
  }

mtype = (pActwk->userflag.b.h >> 4) & 15;
  dodai_mtype[mtype](pActwk);

  xlen = ((Uint16*)pActwk)[30] & 65408;
  xlen -= (scra_h_posit.w.h - 128) & -128;
  if (xlen > 640) {
    ride_on_clr(pActwk, &actwk[0]);
    dai_fout(pActwk);
  }
}

static void dodai_ud(sprite_status* pActwk) {
  ++pActwk->actfree[0];
  pActwk->yposi.w.h = ((Sint16*)pActwk)[31] + dodai_sub(pActwk);

  ridechk(pActwk, &actwk[0]);
}

static void dodai_lr(sprite_status* pActwk) {
  Sint32 xsav;

  xsav = pActwk->xposi.l;
  pActwk->xposi.w.h = ((Sint16*)pActwk)[30] + dodai_sub(pActwk);

  ++pActwk->actfree[0];

  pActwk->yposi.w.h = ((char)(((Uint8*)pActwk)[48]) >> 1) + ((Sint16*)pActwk)[31];

dodai_ride1(pActwk, xsav);
}

static Uint8 dodai_ride1(sprite_status* pActwk, Sint32 xpos) {
  pActwk->xspeed.w = (pActwk->xposi.l - xpos) >> 8;
  return dodai_ride2(pActwk);
}

static Uint8 dodai_ride2(sprite_status* pActwk) {
  if (!ridechk(pActwk, &actwk[0])) {

    if (pActwk->actfree[2]) {
      --pActwk->actfree[2];
    }
    return 0;
  }

  if (pActwk->actfree[2] < 8) {
    ++pActwk->actfree[2];
  }

  return 1;
}

static void dodai_nA(sprite_status* pActwk) {
  Sint32 xsav;

  xsav = pActwk->xposi.l;
  ++pActwk->actfree[0];
  pActwk->yposi.w.h = ((Sint16*)pActwk)[31] + dodai_sub(pActwk);

  pActwk->xposi.w.h = ((Sint16*)pActwk)[30] + dodai_sub(pActwk);

  dodai_ride1(pActwk, xsav);
}

static void dodai_nB(sprite_status* pActwk) {
  Sint32 xsav;

  xsav = pActwk->xposi.l;
  ++pActwk->actfree[0];
  pActwk->yposi.w.h = ((Sint16*)pActwk)[31] + dodai_sub(pActwk);

  pActwk->xposi.w.h = ((Sint16*)pActwk)[30] - dodai_sub(pActwk);

  dodai_ride1(pActwk, xsav);
}

static Uint8 dodai_fix(sprite_status* pActwk) {
  pActwk->yposi.w.h = ((char)pActwk->actfree[2] >> 1) + ((Sint16*)pActwk)[31];

  return dodai_ride2(pActwk);
}

static void dodai_fal(sprite_status* pActwk) {
  if (!pActwk->actfree[1]) {
    if (!dodai_fix(pActwk)) return;

pActwk->actfree[4] = 30;
    pActwk->actfree[1] += 2;
  }

if (pActwk->actfree[4]) {
    --pActwk->actfree[4];
    dodai_fix(pActwk);
    return;
  }

ridechk(pActwk, &actwk[0]);
  pActwk->yposi.l += pActwk->yspeed.w << 8;

  if (pActwk->yspeed.w < 1024) {
    pActwk->yspeed.w += 64;
  }

if ((scra_v_posit.w.h + 256) < pActwk->yposi.w.h) {

    ride_on_clr(pActwk, &actwk[0]);
    frameout(pActwk);
  }
}

static void dodai_up(sprite_status* pActwk) {
  switch (pActwk->actfree[1]) {
    case 0:
      if (!dodai_fix(pActwk)) break;

pActwk->actfree[1] += 2;
    case 2:
      if (pActwk->actfree[0] < 64) {
        pActwk->yposi.w.h = ((Sint16*)pActwk)[31] - dodai_sub(pActwk);

        pActwk->actfree[0] += 2;
        ridechk(pActwk, &actwk[0]);
        break;
      }

      ((Sint16*)pActwk)[31] = pActwk->yposi.w.h;
      pActwk->actfree[1] += 2;
    case 4:
      dodai_fix(pActwk);
      break;
  }

}

static void dodai_upx(sprite_status* pActwk) {
  Sint16 coli;

  switch (pActwk->actfree[1]) {
    case 0:
      if (!dodai_fix(pActwk)) break;

pActwk->actfree[1] += 2;
      pActwk->actfree[4] = 60;
    case 2:
      if (pActwk->actfree[4]) {
        --pActwk->actfree[4];
        dodai_fix(pActwk);
        break;
      }

      speedset2(pActwk);
      pActwk->yspeed.w -= 8;

      if ((coli = emycol_u(pActwk)) >= 0) {
        dodai_ride2(pActwk);
        break;
      }

      pActwk->yposi.w.h -= coli;
      ((Sint16*)pActwk)[31] = pActwk->yposi.w.h;
      pActwk->actfree[1] += 2;
    case 4:
      dodai_fix(pActwk);
      break;
  }

}

static void dodai_rm(sprite_status* pActwk) {
  Sint32 xsav;

  switch (pActwk->actfree[1]) {
    case 0:
      if (!dodai_fix(pActwk)) break;

pActwk->actfree[1] += 2;
      pActwk->actfree[4] = 60;
    case 2:
      if (pActwk->actfree[4]) {
        --pActwk->actfree[4];
        dodai_fix(pActwk);
        break;
      }

      if (pActwk->actfree[0] < 64) {
        xsav = pActwk->xposi.l;
        pActwk->xposi.w.h = ((Sint16*)pActwk)[30] + dodai_sub(pActwk);

        ++pActwk->actfree[0];
        pActwk->yposi.w.h = ((char)pActwk->actfree[2] >> 1) + ((Sint16*)pActwk)[31];

        dodai_ride1(pActwk, xsav);
        break;
      }

      ((Sint16*)pActwk)[30] = pActwk->xposi.w.h;
      pActwk->actfree[1] += 2;
    case 4:
      dodai_fix(pActwk);
      break;
  }

}

static void dodai_lm(sprite_status* pActwk) {
  Sint32 xsav;

  switch (pActwk->actfree[1]) {
    case 0:
      if (!dodai_fix(pActwk)) break;

pActwk->actfree[1] += 2;
      pActwk->actfree[4] = 60;
    case 2:
      if (pActwk->actfree[4]) {
        --pActwk->actfree[4];
        dodai_fix(pActwk);
        break;
      }

      if (pActwk->actfree[0] < 64) {
        xsav = pActwk->xposi.l;
        pActwk->xposi.w.h = ((Sint16*)pActwk)[30] - dodai_sub(pActwk);

        ++pActwk->actfree[0];
        pActwk->yposi.w.h = ((char)pActwk->actfree[2] >> 1) + ((Sint16*)pActwk)[31];

        dodai_ride1(pActwk, xsav);
        break;
      }

      ((Sint16*)pActwk)[30] = pActwk->xposi.w.h;
      pActwk->actfree[1] += 2;
    case 4:
      dodai_fix(pActwk);
      break;
  }

}

static Sint16 dodai_sub(sprite_status* pActwk) {
  Sint16 sinwk, coswk;

  sinset((Uint8)(char)pActwk->actfree[0], &sinwk, &coswk);
  return (sinwk * (Sint16)((char*)pActwk)[49]) >> 4;
}

static void dai_fout(sprite_status* pActwk) {
  Sint16 s;

  if (pActwk->cdsts) {
    s = time_flag + pActwk->cdsts * 3;
    flagwork[s] &= 127;
  }

  frameout(pActwk);
}

void vfuta(sprite_status* pActwk) {
  vfuta_tbl[pActwk->r_no0 / 2](pActwk);
  actionsub(pActwk);
  frameout_s(pActwk);
}

static void vfuta_init(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->patbase = vfutapat;
  pActwk->sprpri = 1;
  pActwk->actflg |= 4;
  pActwk->sprhsize = 4;
  pActwk->sprvsize = 24;

  pa_set();
  vfuta_move1(pActwk);
}

static void vfuta_move1(sprite_status* pActwk) {
  Sint16 wk;

  if ((wk = pActwk->yposi.w.h - actwk[0].yposi.w.h) < 0) {;
    wk = -wk;
  }

  if (wk < 32) {
    wk = pActwk->xposi.w.h - actwk[0].xposi.w.h;

    if (0 <= wk && wk < 49) {
      pActwk->actfree[16] = pActwk->actfree[17] = 0;
      pActwk->r_no0 = 4;

      if (pActwk->actflg & 128) {
        soundset(164);
        pActwk->patno = 1;
      }
    }
  }

  ride_on_chk(pActwk, &actwk[0]);
}

static void vfuta_move2(sprite_status* pActwk) {
  Sint16 wk;

  wk = pActwk->actfree[16];
  wk += 8;
  pActwk->actfree[16] += 8;

  if (wk > 255) {
    pActwk->r_no0 -= 2;
    pActwk->patno = 0;
  }
}
