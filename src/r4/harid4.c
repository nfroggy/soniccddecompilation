#include "../equ.h"
#include "harid4.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../ridechk.h"

#if defined(R41A)
  #define SPRITE_HARID4_BASE 444
#elif defined(R41C)
  #define SPRITE_HARID4_BASE 432
#else
  #define SPRITE_HARID4_BASE 467
#endif

static void act_init(sprite_status* pActwk);
static void act_wait(sprite_status* pActwk);
static void act_slide(sprite_status* pActwk);
static void act_slide1(sprite_status* pActwk);
static void act_down(sprite_status* pActwk);
static void act_down1(sprite_status* pActwk);
static void act_stop(sprite_status* pActwk);

static sprite_pattern pat00 = {
  2,
  {
    { -16, -64, 0, SPRITE_HARID4_BASE + 1 },
    { -16, 32, 0, SPRITE_HARID4_BASE }
  }
};
static sprite_pattern pat01;
sprite_pattern* pat_harid4[2] = {
  &pat00,
  &pat01
};
static void(*hari4_act_tbl[7])(sprite_status*) = {
  &act_init,
  &act_wait,
  &act_slide,
  &act_slide1,
  &act_down,
  &act_down1,
  &act_stop
};

void harid4(sprite_status* pActwk) {
  if (pActwk->userflag.b.h & 128) {

    if (actwk[((Sint16*)pActwk)[33]].actno != 50) {
      frameout(pActwk);
    }

    actionsub(pActwk);
  }
  else {
    if (pActwk->r_no0 == 0 || pActwk->actflg & 128) {

      hari4_act_tbl[pActwk->r_no0 / 2](pActwk);
      hitchk(pActwk, &actwk[0]);
    }

actionsub(pActwk);
    frameout_s(pActwk);
  }
}

static void act_init(sprite_status* pActwk) {
  Sint32 xacwk;
  sprite_status* pNewact;

  pActwk->r_no0 += 2;
  pActwk->actflg |= 4;
  pActwk->sprpri = 3;
  pActwk->sprhs = 16;
  pActwk->sprhsize = 16;
  pActwk->sprvsize = 64;
  pActwk->sproffset = 928;
  pActwk->patbase = pat_harid4;

  if (waterposi_m >= pActwk->yposi.w.h) {
    xacwk = 12288;
  } else {
    xacwk = 4096;
  }
  ((Sint32*)pActwk)[14] = xacwk;

  if (actwkchk2(pActwk, &pNewact) == 0) {
    pNewact->actno = pActwk->actno;
    pNewact->userflag.b.h = -1;
    pNewact->xposi.w.h = pActwk->xposi.w.h;
    pNewact->yposi.w.h = pActwk->yposi.w.h + 48;
    ((Sint16*)pNewact)[33] = pActwk - actwk;
    ((Sint16*)pActwk)[32] = pNewact - actwk;
    pNewact->colino = 176;
    pNewact->actflg = pActwk->actflg;
    pNewact->sprpri = pActwk->sprpri;
    pNewact->sprhs = 16;
    pNewact->sprhsize = 16;
    pNewact->sprvsize = 16;
    pNewact->sproffset = 928;
    pNewact->patbase = pat_harid4;
    pNewact->patno = 1;
  }

}

static void act_wait(sprite_status* pActwk) {
  Sint16 wk;

  wk = actwk[0].yposi.w.h - pActwk->yposi.w.h;
  wk -= 64;
  if ((Uint16)wk >= 128) return;

wk = actwk[0].xposi.w.h - pActwk->xposi.w.h;
  wk += 16;
  if ((Uint16)wk >= 32) return;

pActwk->r_no0 += 2;
  pActwk->yspeed.w = 256;
  ((Sint16*)pActwk)[27] = 20;
}

static void act_slide(sprite_status* pActwk) {
  Sint32 spdwk;

  spdwk = *(Sint32*)&pActwk->actfree[0];
  pActwk->yposi.l += spdwk;
  actwk[((Sint16*)pActwk)[32]].yposi.l += spdwk;
  *(Sint32*)&pActwk->actfree[0] += 8192;

  if (--((Sint16*)pActwk)[27] < 0) {
    pActwk->r_no0 += 2;
    *(Sint32*)&pActwk->actfree[0] = 0;
    ((Sint16*)pActwk)[27] = 30;
  }
}

static void act_slide1(sprite_status* pActwk) {
  if (--((Sint16*)pActwk)[27] < 0) {
    pActwk->r_no0 += 2;
  }
}

static void act_down(sprite_status* pActwk) {
  Sint32 spdwk;
  Sint16 ret;

  spdwk = *(Sint32*)&pActwk->actfree[0];
  pActwk->yposi.l += spdwk;
  actwk[((Sint16*)pActwk)[32]].yposi.l += spdwk;
  *(Sint32*)&pActwk->actfree[0] += ((Sint32*)pActwk)[14];

  if ((ret = emycol_d(pActwk)) < 0) {
    pActwk->r_no0 += 2;
    pActwk->yposi.w.h += ret;
    pActwk->yspeed.w = 0;
    *(Sint32*)&pActwk->actfree[4] = 0x200000;

    frameout(&actwk[((Sint16*)pActwk)[32]]);
  }
}

static void act_down1(sprite_status* pActwk) {
  Sint32 spdwk;

  spdwk = *(Sint32*)&pActwk->actfree[0];
  pActwk->yposi.l += spdwk;

  if ((*(Sint32*)&pActwk->actfree[4] -= spdwk) < 0) {
    pActwk->r_no0 += 2;
  }
}

static void act_stop(sprite_status* pActwk) {

  pActwk = pActwk;
}
