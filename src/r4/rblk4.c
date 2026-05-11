#include "../equ.h"
#include "rblk4.h"
#include "../action.h"
#include "../actset.h"
#include "../fcol.h"
#include "../loader2.h"
#include "../ridechk.h"

#if defined(R42B)
  #define SPRITE_RBLK4_BASE 494
#else
  #define SPRITE_RBLK4_BASE 506
#endif

static void rblk4_init(sprite_status* pActwk);
static void rblk4_wait(sprite_status* pActwk);
static void rblk4_move(sprite_status* pActwk);
static void rblk4_push(sprite_status* pActwk);
static void rblk4_ract_init(sprite_status* pActwk);
static void rblk4_ract_move(sprite_status* pActwk);

static sprite_pattern pat00 =
{
  2,
  {
    { -8, -8, 0, SPRITE_RBLK4_BASE },
    { -48, -48, 0, SPRITE_RBLK4_BASE + 1 }
  }
};
static sprite_pattern pat01 =
{
  2,
  {
    { -8, -8, 0, SPRITE_RBLK4_BASE + 2 },
    { -48, -16, 0, SPRITE_RBLK4_BASE + 3 }
  }
};
static sprite_pattern pat02 =
{
  2,
  {
    { -8, -8, 0, SPRITE_RBLK4_BASE + 4 },
    { -16, -16, 0, SPRITE_RBLK4_BASE + 5 }
  }
};
static sprite_pattern pat03 =
{
  2,
  {
    { -8, -8, 0, SPRITE_RBLK4_BASE + 6 },
    { -16, -48, 0, SPRITE_RBLK4_BASE + 7 }
  }
};
static sprite_pattern pat04 =
{
  2,
  {
    { -8, -8, 0, SPRITE_RBLK4_BASE + 8 },
    { -48, -48, 0, SPRITE_RBLK4_BASE + 9 }
  }
};
static sprite_pattern pat05 =
{
  2,
  {
    { -8, -8, 0, SPRITE_RBLK4_BASE + 10 },
    { -48, -24, 0, SPRITE_RBLK4_BASE + 11 }
  }
};
static sprite_pattern pat06 =
{
  2,
  {
    { -8, -8, 0, SPRITE_RBLK4_BASE + 12 },
    { -24, -48, 0, SPRITE_RBLK4_BASE + 13 }
  }
};
static sprite_pattern pat07 =
{
  2,
  {
    { -8, -8, 0, SPRITE_RBLK4_BASE + 14 },
    { -48, -48, 0, SPRITE_RBLK4_BASE + 15 }
  }
};
static sprite_pattern pat08 =
{
  1,
  { { -32, -32, 0, SPRITE_RBLK4_BASE + 16 } }
};
sprite_pattern* rblk4pat[9] =
{
  &pat00,
  &pat01,
  &pat02,
  &pat03,
  &pat04,
  &pat05,
  &pat06,
  &pat07,
  &pat08
};

void rblk4(sprite_status* pActwk) {
  void(*tbl[4])(sprite_status*) =
  {
    &rblk4_init,
    &rblk4_wait,
    &rblk4_move,
    &rblk4_push
  };

  if (pActwk->actfree[18] == 255)
  {
    rblk4_ract(pActwk);
  }
  else
  {
    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s(pActwk);
  }
}

static void rblk4_init(sprite_status* pActwk) {
  sprite_status* pNewActwk;

  pActwk->r_no0 += 2;
  pActwk->actflg |= 4;
  pActwk->sprpri = 3;
  pActwk->patbase = rblk4pat;
  pActwk->sproffset = 17152;
  pActwk->sprvsize = 64;
  pActwk->sprhsize = 64;

  pActwk->patno = pActwk->userflag.b.h & 3;
  pActwk->actfree[19] = pActwk->patno;

  if (actwkchk(&pNewActwk) != 0)
  {
    frameout_s0(pActwk);
    return;
  }
  pNewActwk->actno = 61;
  ((Sint16*)pNewActwk)[29] = pActwk->xposi.w.h;
  ((Sint16*)pNewActwk)[28] = (Uint16)(pActwk - actwk);
  pNewActwk->actfree[18] = 255;
  pNewActwk->actfree[19] = 0;
  ((Sint16*)pActwk)[26] = (Uint16)(pNewActwk - actwk);

  if (actwkchk(&pNewActwk) != 0)
  {
    frameout_s0(pActwk);
    return;
  }
  pNewActwk->actno = 61;
  ((Sint16*)pNewActwk)[29] = pActwk->xposi.w.h;
  ((Sint16*)pNewActwk)[28] = (Uint16)(pActwk - actwk);
  pNewActwk->actfree[18] = 255;
  pNewActwk->actfree[19] = 1;
  ((Sint16*)pActwk)[28] = (Uint16)(pNewActwk - actwk);
}

static void rblk4_wait(sprite_status* pActwk) {
  sprite_status* pActwk_y;
  sprite_status* pPlayerwk;
  char* a4;
  Sint16 d0;
  Sint16 d1;
  Uint8 d3;

  pActwk_y = &actwk[((Sint16*)pActwk)[26]];
  if (!(pActwk_y->actfree[20] & 128)) return;

  pPlayerwk = &actwk[0];
  if (pActwk->actfree[19] < 2)
  {
    d0 = pActwk->xposi.w.h;
    d1 = pPlayerwk->xposi.w.h + (Sint16)(pPlayerwk->sprhsize + 2);
  }
  else
  {
    d1 = pActwk->xposi.w.h;
    d0 = pPlayerwk->xposi.w.h + (Sint16)-((char)pPlayerwk->sprhsize + 2);
  }
  if (d0 < d1) return;
  d0 -= d1;

a4 = dirstk;
  *a4 = 0;
  scdchk(pActwk, pActwk->yposi.w.h + d0, pActwk->xposi.w.h, 16, 0, 13, a4);

  d3 = dirstk[0];
  if (d3 & 1) d3 = 0;

  if (d1 >= 0)
  {
    if (d1 >= 16)
    {
      pActwk->r_no0 += 2;
      pActwk->actfree[16] = 16;
      pActwk->actfree[17] = 0;
      return;
    }
  }
  pPlayerwk = &actwk[((Sint16*)pActwk)[28]];
  if (pPlayerwk->actfree[20])
  {
    pActwk->r_no0 = 6;
    pActwk->actfree[16] = 16;
    pActwk->actfree[17] = 0;
  }
}

static void rblk4_move(sprite_status* pActwk) {
  Uint8 d0;
  Uint8 rkpchg0[16] =
  {
    0, 4, 1, 255,
    1, 5, 2, 255,
    2, 5, 1, 255,
    3, 6, 2, 255
  };

  d0 = pActwk->actfree[19];
  d0 *= 4;
  d0 = d0 + pActwk->actfree[17];
  d0 = rkpchg0[d0];
  if (d0 & 128)
  {
    pActwk->actfree[19] = pActwk->patno;
    pActwk->r_no0 -= 2;
    return;
  }
  pActwk->patno = d0;
  --pActwk->actfree[16];
  if (pActwk->actfree[16]) return;

soundset(191);
  pActwk->actfree[16] = 4;
  ++pActwk->actfree[17];
}

static void rblk4_push(sprite_status* pActwk) {
  sprite_status* pPlayerwk;
  Sint16 sd0, sd1;
  Uint8 d0;
  Uint8 pspchg0[16] =
  {
    0, 7, 3, 255,
    1, 4, 2, 255,
    2, 4, 1, 255,
    3, 7, 0, 255
  };

  d0 = pActwk->actfree[19];
  d0 *= 4;
  d0 = d0 + pActwk->actfree[17];
  d0 = pspchg0[d0];
  if (d0 & 128)
  {
    pActwk->actfree[19] = pActwk->patno;
    pActwk->r_no0 = 2;
    return;
  }
  pActwk->patno = d0;

  pPlayerwk = &actwk[0];
  sd0 = pActwk->yposi.w.h;
  sd1 = pActwk->sprvsize;
  sd0 -= sd1;
  sd1 = pPlayerwk->sprvsize;
  sd0 -= sd1;
  pPlayerwk->yposi.w.h = sd0;

  --pActwk->actfree[16];
  if (pActwk->actfree[16]) return;

pActwk->actfree[16] = 8;
  ++pActwk->actfree[17];
  if (pActwk->actfree[17] == 1)
  {
    pPlayerwk->xposi.w.h = pActwk->xposi.w.h;
    return;
  }
  if (pActwk->actfree[17] == 2)
  {
    sd0 = 16;
    if (pActwk->actfree[19] & 2)
    {
      sd0 *= -1;
    }
    sd0 += pActwk->xposi.w.h;
    pPlayerwk->xposi.w.h = sd0;
  }
}

void rblk4_ract(sprite_status* pActwk) {
  void(*tbl[2])(sprite_status*) =
  {
    &rblk4_ract_init,
    &rblk4_ract_move
  };

  tbl[pActwk->r_no0 / 2](pActwk);
  actionsub(pActwk);
  frameout_s00(pActwk, ((Sint16*)pActwk)[29]);
}

static void rblk4_ract_init(sprite_status* pActwk) {
  Uint8 d0, d1;

  pActwk->r_no0 += 2;
  pActwk->actflg |= 4;
  pActwk->sprpri = 3;
  pActwk->patbase = rblk4pat;
  pActwk->sproffset = 17152;
  pActwk->patno = 8;

  d0 = 32;
  d1 = 16;
  if (pActwk->actfree[19])
  {
    d0 = 16;
    d1 = 16;
  }
  pActwk->sprhsize = d0;
  pActwk->sprvsize = d1;

  rblk4_ract_move(pActwk);
}

static void rblk4_ract_move(sprite_status* pActwk) {
  sprite_status* pRideact;
  char* a2;
  Uint8 d0;
  char xyofset_tbl1[8] =
  {
    -16, 0,
    -16, 0,
     16, 0,
     16, 0
  };
  char xyofset_tbl2[8] =
  {
    0, -32,
    0,  32,
    0,  32,
    0, -32
  };

  pRideact = &actwk[((Sint16*)pActwk)[28]];
  d0 = pRideact->patno;
  if (d0 >= 4)
  {
    ride_on_clr(pActwk, &actwk[0]);
    return;
  }
  d0 = d0 + d0;
  a2 = &xyofset_tbl1[d0];
  if (pActwk->actfree[19])
  {
    a2 = &xyofset_tbl2[d0];
  }
  pActwk->xposi.w.h = pRideact->xposi.w.h + (Sint16)*a2++;
  pActwk->yposi.w.h = pRideact->yposi.w.h + (Sint16)*a2++;

  hitchk(pActwk, &actwk[0]);
  if (pActwk->actfree[19] == 0)
  {
    if (pActwk->cddat & 8) pActwk->actfree[20] = 255;
    else pActwk->actfree[20] = 0;
  }
  else
  {
    if (!(pActwk->cddat & 32))
    {
      pActwk->actfree[20] = 0;
      pActwk->actfree[21] = 0;
      return;
    }
    if (pActwk->actfree[21] == 0)
    {
      pActwk->actfree[20] = 255;
      pActwk->actfree[21] = 255;
      pActwk->cddat &= 223;
    }
  }
}
