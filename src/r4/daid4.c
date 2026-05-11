#include "../equ.h"
#include "daid4.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../ridechk.h"

static void a_ini(sprite_status* pActwk);
static void a_born(sprite_status* pActwk);
static void a_off(sprite_status* pActwk);
static void a_off1(sprite_status* pActwk);
static void a_on(sprite_status* pActwk);
static void a_on1(sprite_status* pActwk);

static sprite_pattern pat0 =
{
  2,
  {
    { -32, -32, 0, 501 },
    { -32, 16, 0, 503 }
  }
};
static sprite_pattern pat1 =
{
  2,
  {
    { -32, -34, 0, 502 },
    { -32, 14, 0, 504 }
  }
};
sprite_pattern* pat_daid4[2] =
{
  &pat0,
  &pat1
};

void daid4(sprite_status* pActwk) {
  void(*tbl[6])(sprite_status*) =
  {
    &a_ini,
    &a_born,
    &a_off,
    &a_off1,
    &a_on,
    &a_on1
  };

  tbl[pActwk->r_no0 / 2](pActwk);
}

static void a_ini(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->actflg |= 4;
  pActwk->sprpri = 3;
  pActwk->sprhs = 32;
  pActwk->sprhsize = 32;
  pActwk->sproffset = 17168;
  pActwk->patbase = pat_daid4;
  ((Sint16*)pActwk)[23] = pActwk->yposi.w.h;
  pActwk->sprvsize = 33;
}

static void a_born(sprite_status* pActwk) {
  Sint16 d0;

  d0 = actwk[0].yposi.w.h;
  d0 -= 224;
  if (d0 > ((Sint16*)pActwk)[23])
  {
    pActwk->r_no0 += 2;
    pActwk->yposi.w.h = d0;
  }
  frameout_s(pActwk);
}

static void a_off(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->sprvsize = 33;
  pActwk->patno &= 254;
  ((Sint32*)pActwk)[12] = 0;
  pActwk->yspeed.w = 0;

  a_off1(pActwk);
}

static void a_off1(sprite_status* pActwk) {
  Sint16 d0;

  pActwk->yposi.l += ((Sint32*)pActwk)[12];
  ((Sint32*)pActwk)[12] += 1024;
  if ((long int)((Sint32*)pActwk)[12] > 65536)
  {
    ((Sint32*)pActwk)[12] = 65536;
  }
  if (!(pActwk->actflg & 128))
  {
    d0 = pActwk->yposi.w.h;
    d0 -= actwk[0].yposi.w.h;
    if (d0 < 0)
    {
      d0 *= -1;
    }
    if (d0 > 224)
    {
      pActwk->r_no0 = 2;
      goto label1;
    }
  }
  if (ridechk(pActwk, &actwk[0]))
  {
    pActwk->r_no0 = 8;
    ride_on_clr(pActwk, &actwk[0]);
    pActwk->sprvsize = 31;
    pActwk->yposi.w.h += 2;
    actwk[0].yposi.w.h += 4;
    soundset(191);
  }
label1:
  actionsub(pActwk);
  frameout_s(pActwk);
}

static void a_on(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->patno |= 1;
  pActwk->sprvsize = 31;
  pActwk->yspeed.w = -512;

  a_on1(pActwk);
}

static void a_on1(sprite_status* pActwk) {
  Sint16 d0;

  d0 = pActwk->yposi.w.h;
  if (d0 <= ((Sint16*)pActwk)[23])
  {
    pActwk->r_no0 = 2;
  } else
  {
    if (ridechk(pActwk, &actwk[0]) == 0)
    {
      pActwk->r_no0 = 4;
      pActwk->yposi.w.h -= 2;
      actwk[0].yposi.w.h -= 4;
    }
  }
  actionsub(pActwk);
  pActwk->yposi.l -= 0x20000;
  frameout_s(pActwk);
}
