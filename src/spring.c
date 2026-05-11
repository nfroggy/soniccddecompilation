#include "equ.h"
#include "spring.h"
#include "action.h"
#include "actset.h"
#include "dircol.h"
#include "dummy.h"
#include "etc.h"
#include "loader2.h"
#include "playsub.h"
#include "ridechk.h"

#if defined (R11A)
  #define SPRITE_SPRING_BASE 467
#elif defined (R1)
  #define SPRITE_SPRING_BASE 450
#elif defined (R31) || defined (R32)
  #define SPRITE_SPRING_BASE 369
#elif defined (R33)
  #define SPRITE_SPRING_BASE 373
#elif defined (R6)
  #define SPRITE_SPRING_BASE 368
#elif defined (R81)
  #define SPRITE_SPRING_BASE 358
#elif defined (R82)
  #define SPRITE_SPRING_BASE 367
#elif defined (R83)
  #define SPRITE_SPRING_BASE 360
#else
  #define SPRITE_SPRING_BASE 382
#endif

static sprite_pattern spat00 = {
  1,
  { { -16, -8, 0, SPRITE_SPRING_BASE } }
};
static sprite_pattern spat01 = {
  1,
  { { -16, 0, 0, SPRITE_SPRING_BASE + 1 } }
};
static sprite_pattern spat02 = {
  1,
  { { -16, -32, 0, SPRITE_SPRING_BASE + 2 } }
};
static sprite_pattern spat03 = {
  1,
  { { -8, -16, 0, SPRITE_SPRING_BASE + 3 } }
};
static sprite_pattern spat04 = {
  1,
  { { -8, -16, 0, SPRITE_SPRING_BASE + 4 } }
};
static sprite_pattern spat05 = {
  1,
  { { -8, -16, 0, SPRITE_SPRING_BASE + 5 } }
};
static sprite_pattern spat06 = {
  1,
  { { -16, -16, 0, SPRITE_SPRING_BASE + 6 } }
};
static sprite_pattern spat07 = {
  1,
  { { -16, -16, 0, SPRITE_SPRING_BASE + 7 } }
};
static sprite_pattern spat08 = {
  1,
  { { -16, -32, 0, SPRITE_SPRING_BASE + 8 } }
};
static sprite_pattern kpat00 = {
  1,
  { { -8, -8, 0, SPRITE_SPRING_BASE + 9 } }
};
static sprite_pattern kpat01 = {
  1,
  { { -8, -8, 0, SPRITE_SPRING_BASE + 10 } }
};
static sprite_pattern ipat00 = {
  1,
  { { -20, -16, 0, SPRITE_SPRING_BASE + 11 } }
};
static sprite_pattern spat10 = {
  1,
  { { -16, -8, 0, SPRITE_SPRING_BASE + 12 } }
};
static sprite_pattern spat11 = {
  1,
  { { -16, 0, 0, SPRITE_SPRING_BASE + 13 } }
};
static sprite_pattern spat12 = {
  1,
  { { -16, -32, 0, SPRITE_SPRING_BASE + 14 } }
};
static sprite_pattern spat13 = {
  1,
  { { -8, -16, 0, SPRITE_SPRING_BASE + 15 } }
};
static sprite_pattern spat14 = {
  1,
  { { -8, -16, 0, SPRITE_SPRING_BASE + 16 } }
};
static sprite_pattern spat15 = {
  1,
  { { -8, -16, 0, SPRITE_SPRING_BASE + 17 } }
};
static sprite_pattern spat16 = {
  1,
  { { -16, -16, 0, SPRITE_SPRING_BASE + 18 } }
};
static sprite_pattern spat17 = {
  1,
  { { -16, -16, 0, SPRITE_SPRING_BASE + 19 } }
};
static sprite_pattern spat18 = {
  1,
  { { -16, -32, 0, SPRITE_SPRING_BASE + 20 } }
};
sprite_pattern* springpat[3] = {
  &spat00,
  &spat01,
  &spat02
};
sprite_pattern* spring90pat[3] = {
  &spat03,
  &spat04,
  &spat05
};
sprite_pattern* spring45pat[3] = {
  &spat06,
  &spat07,
  &spat08
};
sprite_pattern* springpat2[3] = {
  &spat10,
  &spat11,
  &spat12
};
sprite_pattern* spring90pat2[3] = {
  &spat13,
  &spat14,
  &spat15
};
sprite_pattern* spring45pat2[3] = {
  &spat16,
  &spat17,
  &spat18
};
sprite_pattern* komapat[2] = {
  &kpat00,
  &kpat01
};
sprite_pattern* iwapat[1] = { &ipat00 };
char springchg0[12] = {
   0,  0,  1,  1,
   2,  2,  2,  2,
   2,  2,  1, -4
};
char* springchg[1] = { springchg0 };
char komachg0[4] = { 8, 0, 1, -1 };
char* komachg[1] = { komachg0 };
void(*sjump_move_tbl[13])(sprite_status*) = {
  &sjumpinit,
  &sjumpmove,
  &sjumpmove2,
  &sjumpmove3,
  &sdushmove,
  &sdushmove2,
  &sdushmove3,
  &sjump2move,
  &sjump2move2,
  &sjump2move3,
  &sjump3move,
  &sjump3move2,
  &sjump3move3
};

void iwa(sprite_status* actionwk) {
  switch (actionwk->r_no0) {

    case 0:
      iwa_init(actionwk);
    case 2:
      iwa_move(actionwk);
      break;
  }
  actionsub(actionwk);
  frameout_s(actionwk);
}

void iwa_init(sprite_status* actionwk) {
  Uint8 d0;

  actionwk->r_no0 += 2;
  actionwk->actflg |= 4;
  actionwk->sprpri |= 4;
  actionwk->patbase = iwapat;
  actionwk->sprhsize = 16;
  actionwk->sprvsize = 16;
  actionwk->patno = 0;
  d0 = 11;
  pa_set();
}

void iwa_move(sprite_status* actionwk) {
  sprite_status* a1;

  if (!(actionwk->actflg & 128)) return;

  a1 = &actwk[0];
  ride_on_chk(actionwk, a1);
}

void koma(sprite_status* actionwk) {
  Uint16 d0, d1;

  switch (actionwk->r_no0) {

    case 0:
      koma_init(actionwk);
    case 2:
      koma_move(actionwk);
      break;
    case 4:
      koma_move2(actionwk); break;
      break;
  }
  d0 = ((Uint16*)actionwk)[29];
  d0 &= -128;
  d1 = (Uint16)(scra_h_posit.w.h - 128) & 65408;
  d0 -= d1;
  if (d0 > 640) frameout(actionwk);
}

void koma_init(sprite_status* actionwk) {
  sprite_status* a1;

actionwk->r_no0 += 2;
  actionwk->actflg |= 4;
  actionwk->sprpri = 4;
  actionwk->patbase = komapat;
  actionwk->sproffset = 848;
  actionwk->sprhsize = 8;
  actionwk->sprvsize = 7;
  ((Sint16*)actionwk)[29] = actionwk->xposi.w.h;
  actionwk->xspeed.w = 384;

  pa_set();
  patchg(actionwk, (Uint8**)&komachg);

  if (actwkchk(&a1) != 0) {

    frameout(actionwk);
    return;
  }
  a1->actno = 10;
  a1->xposi.w.h = actionwk->xposi.w.h;
  a1->yposi.w.h = actionwk->yposi.w.h;
  a1->yposi.w.h -= 16;
  ((char*)a1)[61] = -16;
  ((Uint16*)a1)[28] = actionwk - actwk;
  a1->userflag.w = actwk[((Uint16*)a1)[28]].userflag.w;
}

void koma_move(sprite_status* actionwk) {
  Sint16 d1;

  d1 = emycol_d(actionwk);
  if (d1 >= 0) {

    ++actionwk->yposi.w.h;
    return;
  }
  actionwk->yposi.w.h += d1;
  ((Uint16*)actionwk)[27] = actionwk->yposi.w.h;
  actionwk->r_no0 += 2;

}

void koma_move2(sprite_status* actionwk) {
  Sint16 d0, d1;

  if (!time_stop) {
    d1 = emycol_d(actionwk);
    actionwk->yposi.w.h += d1;
    d0 = ((Uint16*)actionwk)[27];
    d0 -= actionwk->yposi.w.h;
    if (d0 >= 12) actionwk->xspeed.w = -actionwk->xspeed.w;
    speedset2(actionwk);
    patchg(actionwk, (Uint8**)&komachg);
  }
  actionsub(actionwk);
}

void spring_d(sprite_status* actionwk) {
  switch (actionwk->r_no0) {

    case 0:
      spr_d_init(actionwk);
    case 2:
      spr_d_move(actionwk);
      break;
  }
}

void spr_d_init(sprite_status* actionwk) {
  if (actionwk->userflag.b.h & 2)
    actionwk->patbase = springpat;
  else
    actionwk->patbase = springpat2;

  actionwk->sproffset = 34080;
  actionwk->actflg |= 4;
  actionwk->sprhsize = 16;
  actionwk->sprvsize = 8;
  actionwk->sprpri = 4;
  actionwk->r_no0 += 2;
  spr_d_move(actionwk);
}

void spr_d_move(sprite_status* actionwk) {
  actionwk->xposi.l = actwk[0].xposi.l;
  actionwk->yposi.l = actwk[0].yposi.l;
  actionsub(actionwk);
}

void spring(sprite_status* actionwk) {
  Uint16 d1;
  sprite_status* a1;
  Uint16 d0;

if (actionwk->r_no1 == 5) {

    spring_d(actionwk);
  }
  if (actionwk->r_no0 == 0) goto label1;
  if (actionwk->actflg & 128) {
label1:
    sjump_move_tbl[actionwk->r_no0 / 2](actionwk);
  }
  actionsub(actionwk);

  d1 = ((Uint16*)actionwk)[28];
  if (d1 != 0) {
    a1 = &actwk[d1];
    actionwk->xposi.w.h = a1->xposi.w.h;
    actionwk->yposi.w.h = a1->yposi.w.h;
    actionwk->xposi.w.h += ((char*)actionwk)[60];
    actionwk->yposi.w.h += ((char*)actionwk)[61];
  }

  d0 = ((Uint16*)actionwk)[29];
  d0 &= -128;
  d1 = (Uint16)(scra_h_posit.w.h - 128) & 65408;
  d0 -= d1;
  if (d0 > 640) frameout(actionwk);
}

void sjumpinit(sprite_status* actionwk) {
  Sint16 sjumptbl[2] = { -4096, -2560 };
  Uint8 d0;

actionwk->r_no0 += 2;
  if (actionwk->userflag.b.h & 2)
    actionwk->patbase = springpat;
  else
    actionwk->patbase = springpat2;

  actionwk->sproffset = 1312;
  actionwk->actflg |= 4;
  actionwk->sprhsize = 16;
  actionwk->sprvsize = 8;
  ((Sint16*)actionwk)[29] = actionwk->xposi.w.h;
  actionwk->sprpri = 4;

  d0 = actionwk->userflag.b.h;
  if (d0 & 4) {
    actionwk->r_no0 = 8;
    actionwk->sprhsize = 8;
    actionwk->sprvsize = 16;
    if (actionwk->userflag.b.h & 2)
      actionwk->patbase = spring90pat;
    else
      actionwk->patbase = spring90pat2;
  }
  else
  if (d0 & 8) {
    actionwk->r_no0 = 20;
    actionwk->sprhsize = 24;
    actionwk->sprvsize = 12;
    if (actionwk->userflag.b.h & 2)
      actionwk->patbase = spring45pat;
    else
      actionwk->patbase = spring45pat2;
    pa_set();
  }
  else
  if (actionwk->actflg & 2) {
    actionwk->r_no0 = 14;
    actionwk->cddat |= 1;
  }
  if (d0 & 2)
    actionwk->sproffset |= 8192;

  ((Sint16*)actionwk)[26] = sjumptbl[(d0 & 2) / 2];

}

Sint32 ride_on_chk_s(sprite_status* actionwk, sprite_status* a1) {
  return ride_on_chk(actionwk, a1);
}

void sjumpmove(sprite_status* actionwk) {
  sprite_status* a1;

  if (!(actionwk->actflg & 128)) return;

  a1 = &actwk[0];
  if (ride_on_chk_s(actionwk, a1) == 0) return;

  actionwk->r_no0 = 4;
  a1->yposi.w.h += 8;
  a1->yspeed.w = ((Sint16*)actionwk)[26];
  a1->cddat |= 2;
  a1->cddat &= -9;
  a1->mstno.b.h = 16;
  actionwk->cddat &= -9;
  soundset(152);
}

void sjumpmove2(sprite_status* actionwk) {
  patchg(actionwk, (Uint8**)&springchg);
}

void sjumpmove3(sprite_status* actionwk) {
  actionwk->cddat &= 247;
  actionwk->mstno.b.l = 1;
  actionwk->r_no0 -= 4;
  actionwk->patno = 0;
}

Sint32 ride_on_chk_s1(sprite_status* actionwk, sprite_status* a1) {
  return ride_on_chk(actionwk, a1);
}

void sdushmove(sprite_status* actionwk) {
  sprite_status* a1;

  if (!(actionwk->actflg & 128)) return;

  a1 = &actwk[0];
  ride_on_chk_s1(actionwk, a1);
  if (!(actionwk->cddat & 32)) return;

  actionwk->r_no0 = 10;
  a1->xspeed.w = ((Sint16*)actionwk)[26];
  a1->xposi.w.h += 8;
  a1->cddat |= 1;
  if (!(actionwk->cddat & 1)) {
    a1->xposi.w.h -= 16;
    a1->xspeed.w = -a1->xspeed.w;
    a1->cddat &= 254;
  }
  ((Sint16*)a1)[33] = 15;
  a1->mspeed.w = a1->xspeed.w;
  if (!(a1->cddat & 4))
    a1->mstno.b.h = 0;

  a1->direc.b.h = 0;
  actionwk->cddat &= 223;
  a1->cddat &= 223;
  soundset(152);
}

void sdushmove2(sprite_status* actionwk) {
  patchg(actionwk, (Uint8**)&springchg);
}

void sdushmove3(sprite_status* actionwk) {
  actionwk->mstno.b.l = 1;
  actionwk->r_no0 -= 4;
  actionwk->patno = 0;
}

Sint32 ride_on_chk_s2(sprite_status* actionwk, sprite_status* a1) {
  return hitchk_u(actionwk, a1);
}

void sjump2move(sprite_status* actionwk) {
  sprite_status* a1;

  if (!(actionwk->actflg & 128)) return;

  a1 = &actwk[0];
  if (ride_on_chk_s2(actionwk, a1) == 0) return;

  actionwk->r_no0 = 16;
  a1->yposi.w.h -= 8;
  a1->yspeed.w = ((Sint16*)actionwk)[26];
  a1->yspeed.w = -a1->yspeed.w;
  a1->cddat |= 2;
  a1->cddat &= 247;
  actionwk->cddat &= 247;
  soundset(152);
}

void sjump2move2(sprite_status* actionwk) {
  patchg(actionwk, (Uint8**)&springchg);
}

void sjump2move3(sprite_status* actionwk) {
  actionwk->mstno.b.l = 1;
  actionwk->r_no0 -= 4;
  actionwk->patno = 0;
}

void sjump3move(sprite_status* actionwk) {
  sprite_status* a1;
  Uint16 sin, cos;
  Sint32 sinl, cosl;
  Uint8 d0;
  Sint16 d2;

  if (!(actionwk->actflg & 128)) return;

  a1 = &actwk[0];
  if (ride_on_chk_s(actionwk, a1) == 0)
    if (!(actionwk->cddat & 32)) return;

  actionwk->r_no0 = 22;
  d0 = 224;
  sinset(d0, (Sint16*)&sin, (Sint16*)&cos);
  d2 = ((Sint16*)actionwk)[26];
  d2 = -d2;
  sinl = sin * d2;
  cosl = cos * d2;
  sinl >>= 8;
  cosl >>= 8;
  a1->yspeed.w = sinl;
  a1->xspeed.w = cosl;
  a1->yposi.w.h += 8;
  if (actionwk->actflg & 2) {
    a1->yposi.w.h -= 16;
    a1->yspeed.w = -a1->yspeed.w;
  }
  a1->cddat &= 254;
  a1->xposi.w.h -= 8;
  if (actionwk->cddat & 1) {
    a1->xposi.w.h += 16;
    a1->cddat |= 1;
    a1->xspeed.w = -a1->xspeed.w;
  }
  a1->cddat |= 2;
  a1->cddat &= 247;
  a1->cddat &= 223;
  actionwk->cddat &= 247;
  actionwk->cddat &= 223;
  soundset(152);
  sjump3move2(actionwk);
}

void sjump3move2(sprite_status* actionwk) {
  patchg(actionwk, (Uint8**)&springchg);
}

void sjump3move3(sprite_status* actionwk) {
  actionwk->mstno.b.l = 1;
  actionwk->r_no0 -= 4;
  actionwk->patno = 0;
}
