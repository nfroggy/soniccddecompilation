#include "../equ.h"
#include "bobin.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"
#include "../loader2.h"
#include "../playsub.h"
#include "../ridechk.h"
#include "../score.h"

#if defined(R31A)
  #define SPRITE_BOBIN_BASE 550
#elif defined(R31B) || defined(R32A)
  #define SPRITE_BOBIN_BASE 544
#elif defined(R33C) || defined(R33D)
  #define SPRITE_BOBIN_BASE 548
#elif defined(R83)
  #define SPRITE_BOBIN_BASE 489
#else
  #define SPRITE_BOBIN_BASE 497
#endif

static sprite_pattern bobinsp0 = {
  1,
  { { -16, -16, 0, SPRITE_BOBIN_BASE } }
};
static sprite_pattern bobinsp1 = {
  1,
  { { -12, -12, 0, SPRITE_BOBIN_BASE + 1 } }
};
static sprite_pattern bobinsp2 = {
  1,
  { { -24, -20, 0, SPRITE_BOBIN_BASE + 2 } }
};
static sprite_pattern fripsp0 = {
  1,
  { { -32, -12, 0, SPRITE_BOBIN_BASE + 3 } }
};
static sprite_pattern fripsp1 = {
  1,
  { { -32, -12, 0, SPRITE_BOBIN_BASE + 4 } }
};
static sprite_pattern fripsp2 = {
  1,
  { { -32, -20, 0, SPRITE_BOBIN_BASE + 5 } }
};
sprite_pattern* bobinpat[3] = { &bobinsp0, &bobinsp1, &bobinsp2 };
sprite_pattern* frippat[3] = { &fripsp0, &fripsp1, &fripsp2 };
static char bobinchg0[3] = { 15, 0, -1 };
static char bobinchg1[7] = { 4, 1, 2, 1, 2, -3, 0 };
static char* bobinchg[2] = { bobinchg0, bobinchg1 };
static char fripchg0[3] = { 15, 0, -1 };
static char fripchg1[6] = { 1, 1, 2, 1, -3, 0 };
static char* fripchg[2] = { fripchg0, fripchg1 };
char frip_posi[64] = {
   14,  13,  12,  11,
   11,  10,  10,   9,
    9,   8,   8,   7,
    7,   6,   6,   5,
    5,   4,   4,   3,
    3,   2,   2,   1,
    1,   0,   0,  -1,
   -1,  -2,  -2,  -3,
   -3,  -4,  -4,  -5,
   -5,  -6,  -6,  -7,
   -7,  -8,  -8,  -9,
   -9, -10, -10, -11,
  -11, -11, -12, -12,
  -12, -12, -12, -12,
  -11, -11, -10, -10,
   -9,  -8,  -6,  -4
};
char frip_posi_r[64] = {
  18, 19, 20, 20,
  20, 20, 20, 20,
  20, 19, 19, 19,
  19, 19, 19, 19,
  18, 18, 18, 18,
  18, 18, 17, 17,
  17, 17, 17, 17,
  16, 16, 16, 16,
  16, 16, 16, 15,
  15, 15, 15, 15,
  14, 14, 14, 14,
  14, 14, 13, 13,
  13, 13, 13, 13,
  12, 12, 12, 12,
  11, 11, 10, 10,
   9,  8,  6,  4
};

void bobin(sprite_status* actionwk) {
  switch (actionwk->r_no0) {

    case 0:
      bobininit(actionwk);
    case 2:
      bobinmove(actionwk);
      break;
  }
}

void bobininit(sprite_status* actionwk) {
  Sint16 d0;
  char d6;

  actionwk->r_no0 += 2;
  actionwk->patbase = bobinpat;
  actionwk->sproffset = 1230;

if (actionwk->xposi.w.h == 10624 && actionwk->yposi.w.h == 1024) {

    actionwk->sproffset |= 32768;
  }

  actionwk->actflg |= 4;
  actionwk->sprhs = 16;
  actionwk->sprhsize = 16;
  actionwk->sprvsize = 16;
  actionwk->sprpri = 1;
  actionwk->colino = 215;

  ((Sint16*)actionwk)[27] = actionwk->xposi.w.h;
  ((Sint16*)actionwk)[28] = 5;
  d0 = 0;
  d6 = actionwk->userflag.b.h;
  if (d6 & 2) d0 += 64;
  if (d6 & 1) d0 += 32;
  ((Sint16*)actionwk)[26] = d0;
  d0 >>= 1;
  ((Sint16*)actionwk)[25] = d0;
  if (d0 > 0) {

    *(Sint32*)&actionwk->actfree[0] = 65536;
    if (!(d6 & 64)) {

      *(Sint32*)&actionwk->actfree[0] = -*(Sint32*)&actionwk->actfree[0];
    }
  }

}

void bobinmove(sprite_status* actionwk) {
  Sint32 *a, d0l, d1l;
  sprite_status* a1;
  Uint8 d;
  Sint16 d0, d1, d2;

  if (*(Sint32*)&actionwk->actfree[0] != 0) {
    a = &actionwk->xposi.l;
    if (actionwk->userflag.w >= 0) {
      a = &actionwk->yposi.l;
    }
    *a += *(Sint32*)&actionwk->actfree[0];
    if (!--((Sint16*)actionwk)[25]) {
      ((Sint16*)actionwk)[25] = ((Sint16*)actionwk)[26];

      *(Sint32*)&actionwk->actfree[0] = -*(Sint32*)&actionwk->actfree[0];
    }
  }

if (actionwk->colicnt != 0) {
    if (((Sint16*)actionwk)[28]) {

      --((Sint16*)actionwk)[28];
      scoreup(10);
      tensuu0(actionwk, 0);
    }

    if ((char)actionwk->actflg < 0) {

      d0 = 127;
      sub_sync(d0);
    }
    a1 = &actwk[0];
    actionwk->colicnt = 0;
    d1 = actionwk->xposi.w.h - a1->xposi.w.h;
    d2 = actionwk->yposi.w.h - a1->yposi.w.h;
    d = atan_sonic(d1, d2);
    sinset(d, &d0, &d1);
    d1l = d1 * -1792;
    a1->xspeed.w = d1l >> 8;
    d0l = d0 * -1792;
    a1->yspeed.w = d0l >> 8;
    a1->cddat |= 2;
    a1->cddat &= 239;
    a1->cddat &= 223;
    a1->actfree[18] = 0;
    actionwk->mstno.b.h = 1;

    if (actionwk->cdsts)
      if (flagwork[actionwk->cdsts] < 138)
        ++flagwork[actionwk->cdsts];
  }
  patchg(actionwk, (Uint8**)bobinchg);
  actionsub(actionwk);
  frameout_s00(actionwk, ((Sint16*)actionwk)[27]);
}

void frip(sprite_status* actionwk) {
  switch (actionwk->r_no0) {

    case 0:
      frip_init(actionwk);
    case 2:
      frip_move(actionwk);
      break;
  }
}

void frip_init(sprite_status* actionwk) {
  actionwk->r_no0 += 2;
  actionwk->patbase = frippat;
  actionwk->sproffset = 1181;
  actionwk->actflg |= 4;
  actionwk->sprpri = 1;
  actionwk->sprhsize = 32;
  actionwk->sprvsize = 16;
}

void frip_move(sprite_status* actionwk) {
  if (actionwk->mstno.b.h == 0) {

    frip_chk(actionwk);
  }

  patchg(actionwk, (Uint8**)fripchg);
  actionsub(actionwk);
  frameout_s(actionwk);
}

void frip_chk(sprite_status* actionwk) {
  sprite_status* a1;
  Sint16 d0, d1, d2, d3;

  a1 = &actwk[0];
  if (a1->mstno.b.h == 43) goto label1;

  d0 = a1->xposi.w.h - actionwk->xposi.w.h;
  d1 = actionwk->sprhsize;
  d0 += d1;
  if (d0 < 0) goto label1;
  d1 += d1;
  if (d0 >= d1) goto label1;

  d0 = a1->xposi.w.h;
  d3 = actionwk->sprhsize;
  d2 = actionwk->xposi.w.h - d3;
  d0 -= d2;
  if (actionwk->cddat & 1) {

    d2 = 64 - d0;
    d0 = d2;
  }
  d2 = d0;

  d0 = frip_posi[d2];
  if (a1->yspeed.w < 0) d0 = frip_posi_r[d2];
  d0 += actionwk->yposi.w.h;
  d1 = d0;
  d3 = a1->sprvsize;
  if (a1->yspeed.w < 0) d3 = -d3;

  d1 -= d3;
  d3 += a1->yposi.w.h;
  d3 -= d0;
  d3 += 8;
  if (d3 < 0 || d3 >= 16) goto label1;

  a1->yposi.w.h = d1;
  if (a1->yspeed.w >= 0) {

    ride_on_set(actionwk, a1);
  }
  else {

    a1->yspeed.w = 0;
    a1->mspeed.w = a1->xspeed.w;
  }

  return;
label1:
  ride_on_clr(actionwk, a1);

}
