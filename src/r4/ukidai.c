#include "../equ.h"
#include "ukidai.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../ridechk.h"

static void a_init(sprite_status* pActwk);
static void a_moving(sprite_status* pActwk);
static void a_wait(sprite_status* pActwk);
static void a_move(sprite_status* pActwk);
static void a_move1(sprite_status* pActwk);

static sprite_pattern pat00 =
{
  1,
  { { -16, -8, 0, 530 } }
};
sprite_pattern* pat_ukidai[1] = { &pat00 };

void ukidai(sprite_status* pActwk) {
  void(*tbl[5])(sprite_status*) =
  {
    &a_init,
    &a_moving,
    &a_wait,
    &a_move,
    &a_move1
  };

  tbl[pActwk->r_no0 / 2](pActwk);
  actionsub(pActwk);
  frameout_s00(pActwk, ((Sint16*)pActwk)[29]);
}

static void a_init(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->actflg |= 4;
  pActwk->sprpri = 3;
  pActwk->sprhs = 16;
  pActwk->sprhsize = 16;
  pActwk->sprvsize = 8;
  pActwk->sproffset = 848;
  pActwk->patbase = pat_ukidai;
  ((Sint16*)pActwk)[29] = pActwk->xposi.w.h;

  if (watermoveposi != 1920)
  {
    if (watermoveposi != 1504)
    {
      frameout(pActwk);
      return;
    }
  }
  a_moving(pActwk);
}

static void a_moving(sprite_status* pActwk) {
  Sint16 d0;

  pActwk->yposi.w.h = waterposi;

  d0 = waterposi_m;
  if (watermoveposi == d0)
  {
    if (d0 == 1920)
    {
      pActwk->r_no0 = 4;
      a_wait(pActwk);
      return;
    }
    if (d0 == 1504)
    {
      pActwk->r_no0 = 6;
      a_move(pActwk);
      return;
    }
    frameout(pActwk);
  }
}

static void a_wait(sprite_status* pActwk) {
  pActwk->yposi.w.h = waterposi;

  if (ridechk(pActwk, &actwk[0]))
  {
    pActwk->r_no0 += 2;
    pActwk->xposi.w.h = actwk[0].xposi.w.h;
    watermoveposi = 1504;
  }
}

static void a_move(sprite_status* pActwk) {
  ushort_union d0;

  pActwk->r_no0 += 2;
  d0.b.h = waterspeed;
  d0.b.l = water_flag;
  pActwk->yspeed.w = -(Sint16)d0.w;

  a_move1(pActwk);
}

static void a_move1(sprite_status* pActwk) {
  sprite_status* pPlayerwk;
  Sint32 a6;
  Sint16 d1;
  Sint16 d2;
  Sint16 d3;
  Sint16 d4, d5, d6;

  pActwk->yposi.w.h = waterposi;

  pPlayerwk = &actwk[0];
  d4 = pPlayerwk->xspeed.b.h;
  d4 += pPlayerwk->xposi.w.h;
  d4 -= pActwk->xposi.w.h;
  if (pActwk->xposi.w.h)
  {
    a6 = 2;
    d6 = 1;
    d5 = 0;
    d5 = (Uint16)pActwk->sprhs;
    if (d4 < 0)
    {
      a6 = 1;
      d6 *= -1;
      d5 *= -1;
      d4 *= -1;
    }
    *(Sint32*)&pActwk->actfree[0] = a6;
    ((Sint16*)pActwk)[25] = d6;
    ((Sint16*)pActwk)[26] = d5;
    ((Sint16*)pActwk)[27] = d4;
    ((Sint16*)pActwk)[28] = pActwk->yposi.w.h;
    ((Sint16*)pActwk)[28] += 7;
    do
    {
      a6 = *(Sint32*)&pActwk->actfree[0];
      d3 = pActwk->xposi.w.h;
      d3 += ((Sint16*)pActwk)[26];
      if (a6 != 2)
      {
        d3 ^= 15;
      }
      d2 = ((Sint16*)pActwk)[28];

      if (a6 == 1) d1 = emycol_l3(pActwk, d3, d2);
      else d1 = emycol_r3(pActwk, d3, d2);
      if (d1 < 0) break;
      if (((Sint16*)pActwk)[27] < 2)
      {
        --((Sint16*)pActwk)[27];
        break;
      }
      --((Sint16*)pActwk)[27];
      pActwk->xposi.w.h += ((Sint16*)pActwk)[25];
    } while (1);
  }
  if (ridechk(pActwk, pPlayerwk))
  {
    pPlayerwk->xposi.w.h = pActwk->xposi.w.h;
  }
}
