#include "../equ.h"
#include "kuzure4.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../ridechk.h"

#if defined(R41A)
  #define SPRITE_KUZURE4_BASE 515
#elif defined(R42A)
  #define SPRITE_KUZURE4_BASE 519
#elif defined(R42B)
  #define SPRITE_KUZURE4_BASE 480
#else
  #define SPRITE_KUZURE4_BASE 475
#endif

static void main_ini(sprite_status* pActwk);
static void main_check(sprite_status* pActwk);
static void main_wait(sprite_status* pActwk);
static void main_break(sprite_status* pActwk);
static void parts(sprite_status* pActwk);
static void parts_ini(sprite_status* pActwk);
static void parts_wait(sprite_status* pActwk);
static void parts_fall(sprite_status* pActwk);

static sprite_pattern pat_kuzure_00 = {
  1,
  { { -8, -16, 0, SPRITE_KUZURE4_BASE } }
};
static sprite_pattern pat_kuzure_01 = {
  1,
  { { -16, -16, 0, SPRITE_KUZURE4_BASE + 1 } }
};
static sprite_pattern pat_kuzure_02 = {
  1,
  { { -24, -16, 0, SPRITE_KUZURE4_BASE + 2 } }
};
static sprite_pattern pat_kuzure_03 = {
  1,
  { { -32, -16, 0, SPRITE_KUZURE4_BASE + 3 } }
};
static sprite_pattern pat_kuzure_04 = {
  1,
  { { -40, -16, 0, SPRITE_KUZURE4_BASE + 4 } }
};
static sprite_pattern pat_kuzure_05 = {
  1,
  { { -48, -16, 0, SPRITE_KUZURE4_BASE + 5 } }
};
static sprite_pattern pat_kuzure_06 = {
  1,
  { { -56, -16, 0, SPRITE_KUZURE4_BASE + 6 } }
};
static sprite_pattern pat_kuzure_07 = {
  1,
  { { -64, -16, 0, SPRITE_KUZURE4_BASE + 7 } }
};
static sprite_pattern pat_kuzure_08 = {
  1,
  { { -8, -8, 0, SPRITE_KUZURE4_BASE + 8 } }
};
sprite_pattern* pat_kuzure_a[9] = {
  &pat_kuzure_00,
  &pat_kuzure_01,
  &pat_kuzure_02,
  &pat_kuzure_03,
  &pat_kuzure_04,
  &pat_kuzure_05,
  &pat_kuzure_06,
  &pat_kuzure_07,
  &pat_kuzure_08
};
static void(*kuzure4_act_tbl[4])(sprite_status*) = {
  &main_ini,
  &main_check,
  &main_wait,
  &main_break
};
static void(*parts_act_tbl[3])(sprite_status*) = {
  &parts_ini,
  &parts_wait,
  parts_fall
};

void kuzure4(sprite_status* pActwk) {
  if (pActwk->userflag.b.l) {
    parts(pActwk);
  } else {
    kuzure4_act_tbl[pActwk->r_no0 / 2](pActwk);
  }
}

static void main_ini(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->actflg = 4;
  pActwk->sproffset = 17252;
  pActwk->patbase = pat_kuzure_a;
  pActwk->sprpri = 3;
  pActwk->sprvsize = 17;

  pActwk->patno = pActwk->userflag.b.h - 1;
  pActwk->sprhs = pActwk->sprhsize = pActwk->userflag.b.h * 8;
  main_check(pActwk);
}

static void main_check(sprite_status* pActwk) {
  if (ridechk(pActwk, &actwk[0])) {
    pActwk->r_no0 += 2;
    pActwk->cdsts = 0;
    ((Sint16*)pActwk)[23] = 8;

    if (actwk[0].xspeed.w < 0) {
      ((Sint16*)pActwk)[24] = -8;
    } else {
      ((Sint16*)pActwk)[24] = 8;
    }

    soundset(163);
  }

actionsub(pActwk);
  frameout_s(pActwk);
}

static void main_wait(sprite_status* pActwk) {
  ridechk(pActwk, &actwk[0]);

  if (--((Sint16*)pActwk)[23] < 0) {
    pActwk->r_no0 += 2;
  }

  actionsub(pActwk);
}

static void main_break(sprite_status* pActwk) {
  Uint8 flagwk;
  Sint16 xwk;
  Sint16 ywk;
  sprite_status* pNewact;

flagwk = 255;
  ywk = 8;
  xwk = ((Sint16)pActwk->userflag.b.h - 1) * 8;

  if (((Sint16*)pActwk)[24] >= 0) {
    xwk *= -1;
  }

if (actwkchk(&pNewact) == 0) {

pNewact->actno = pActwk->actno;
    pNewact->actflg = pActwk->actflg;
    pNewact->sproffset = pActwk->sproffset;
    pNewact->patbase = pActwk->patbase;
    pNewact->xposi.l = pActwk->xposi.l;
    pNewact->yposi.l = pActwk->yposi.l;

pNewact->userflag.b.l = flagwk;
    pNewact->yposi.w.h -= ywk;
    pNewact->xposi.w.h += xwk;
    ((Sint16*)pNewact)[23] = 4;

    if (actwkchk(&pNewact) == 0) {

pNewact->actno = pActwk->actno;
      pNewact->actflg = pActwk->actflg;
      pNewact->sproffset = pActwk->sproffset;
      pNewact->patbase = pActwk->patbase;
      pNewact->xposi.l = pActwk->xposi.l;
      pNewact->yposi.l = pActwk->yposi.l;

pNewact->userflag.b.l = flagwk;
      pNewact->yposi.w.h += ywk;
      pNewact->xposi.w.h += xwk;
    }
  }

if (ridechk(pActwk, &actwk[0])) {
    ride_on_clr(pActwk, &actwk[0]);
  }

if (--pActwk->userflag.b.h <= 0) {
    frameout(pActwk);
  } else {
    pActwk->r_no0 -= 2;
    --pActwk->patno;
    pActwk->sprhs -= 8;
    pActwk->sprhsize -= 8;
    ((Sint16*)pActwk)[23] = 7;
    pActwk->xposi.w.h += ((Sint16*)pActwk)[24];
    ridechk(pActwk, &actwk[0]);
    actionsub(pActwk);
  }
}

static void parts(sprite_status* pActwk) {
  parts_act_tbl[pActwk->r_no0 / 2](pActwk);
}

static void parts_ini(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->patno = 8;
  pActwk->sprpri = 3;
  pActwk->sprhs = pActwk->sprhsize = 8;
  pActwk->sprvsize = 8;
  actionsub(pActwk);
}

static void parts_wait(sprite_status* pActwk) {
  if (--((Sint16*)pActwk)[23] < 0) {
    pActwk->r_no0 += 2;
  }

actionsub(pActwk);
}

static void parts_fall(sprite_status* pActwk) {
  Sint32 spdwk;

  if (!(pActwk->actflg & 128)) {
    frameout(pActwk);
  } else {
    spdwk = ((Sint32*)pActwk)[12] + 16384;
    if (spdwk > 1441792) {
      spdwk = 1441792;
    }
    ((Sint32*)pActwk)[12] = spdwk;
    pActwk->yposi.l += spdwk;
    actionsub(pActwk);
  }
}
