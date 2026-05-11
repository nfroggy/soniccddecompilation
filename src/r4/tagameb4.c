#include "../equ.h"
#include "tagameb4.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../suicide.h"
#include "playsub4.h"

#if defined(R41A)
  #define SPRITE_TAGAMEB4_BASE 485
#elif defined(R41B)
  #define SPRITE_TAGAMEB4_BASE 446
#elif defined(R41C)
  #define SPRITE_TAGAMEB4_BASE 463
#elif defined(R41D)
  #define SPRITE_TAGAMEB4_BASE 453
#elif defined(R42A)
  #define SPRITE_TAGAMEB4_BASE 491
#elif defined(R42B)
  #define SPRITE_TAGAMEB4_BASE 444
#else
  #define SPRITE_TAGAMEB4_BASE 448
#endif

static void tagameb0(sprite_status* pActwk);
static void a_init(sprite_status* pActwk);
static void make_toge(sprite_status* pActwk);
static void a_init_sub(Sint16 subact, sprite_status* pActwk);
static void a_move(sprite_status* pActwk);
static Sint32 a_check(sprite_status* pActwk);
static void a_stop(sprite_status* pActwk);
static void a_stop1(sprite_status* pActwk);
static void a_dash(sprite_status* pActwk);
static void tagameb1(sprite_status* pActwk);
static void b_init(sprite_status* pActwk);
static void b_follow(sprite_status* pActwk);
static void b_move(sprite_status* pActwk);

static Uint8 pchg0[4] = { 30, 0, 1, 255 };
static Uint8* pchg[1] = { pchg0 };
static sprite_pattern spr_tagame_b_00 =
{
  1,
  { { -20, -12, 0, SPRITE_TAGAMEB4_BASE } }
};
static sprite_pattern spr_tagame_b_01 =
{
  1,
  { { -20, -12, 0, SPRITE_TAGAMEB4_BASE + 1 } }
};
static sprite_pattern spr_tagame_e_00 =
{
  1,
  { { -20, -12, 0, SPRITE_TAGAMEB4_BASE + 2 } }
};
static sprite_pattern spr_tagame_e_01 =
{
  1,
  { { -20, -12, 0, SPRITE_TAGAMEB4_BASE + 3 } }
};
static sprite_pattern spr_tagame_99 =
{
  1,
  { { 0, 0, 0, SPRITE_TAGAMEB4_BASE + 4 } }
};
sprite_pattern* pat_tagameb_b[2] =
{
  &spr_tagame_b_00,
  &spr_tagame_b_01
};
sprite_pattern* pat_tagameb_e[2] =
{
  &spr_tagame_e_00,
  &spr_tagame_e_01
};
static sprite_pattern* pat[1] = { &spr_tagame_99 };

void tagameb(sprite_status* pActwk) {
  if (pActwk->userflag.b.h >= 0)
    tagameb0(pActwk);
  else
    tagameb1(pActwk);
}

static void tagameb0(sprite_status* pActwk) {
  static void(*tbl[5])(sprite_status*) =
  {
    &a_init,
    &a_move,
    &a_stop,
    &a_stop1,
    &a_dash
  };

  if (enemy_suicide(pActwk)) return;
  tbl[pActwk->r_no0 / 2](pActwk);
  actionsub(pActwk);
  frameout_s(pActwk);
}

static void a_init(sprite_status* pActwk) {
  ((Sint16*)pActwk)[30] = pActwk->xposi.w.h;
  pActwk->actflg |= 4;
  pActwk->sprpri = 3;
  pActwk->sproffset = 9168;
  pActwk->sprhs = 20;
  pActwk->sprhsize = 20;
  pActwk->sprvsize = 8;
  pActwk->colino = 45;
  pActwk->r_no0 += 2;

  if (!pActwk->userflag.b.h)
  {
    pActwk->patbase = pat_tagameb_e;
    ((Sint32*)pActwk)[12] = -65536;
    ((Sint16*)pActwk)[29] = 200;
  }
  else
  {
    pActwk->patbase = pat_tagameb_b;
    ((Sint32*)pActwk)[12] = -32768;
    ((Sint16*)pActwk)[29] = 400;
    return;
  }
  make_toge(pActwk);
}

static void make_toge(sprite_status* pActwk) {
  sprite_status* subActwk;

  if (actwkchk(&subActwk) == 0)
  {
    subActwk->actno = pActwk->actno;
    subActwk->userflag.b.h = -1;
    subActwk->actfree[19] = pActwk->cdsts;
    ((Sint16*)subActwk)[33] = (Uint16)(Uint8)(pActwk - actwk);
    ((Sint16*)pActwk)[26] = (Uint16)(Uint8)(subActwk - actwk);
  }
  else
  {
    frameout(pActwk);
    return;
  }
  if (actwkchk(&subActwk) == 0)
  {
    subActwk->actno = pActwk->actno;
    subActwk->userflag.b.h = -1;
    subActwk->actfree[19] = pActwk->cdsts;
    ((Sint16*)subActwk)[33] = (Uint16)(Uint8)(pActwk - actwk);
    ((Sint16*)pActwk)[27] = (Uint16)(Uint8)(subActwk - actwk);
  }
  else
  {
    frameout(pActwk);
    return;
  }
  if (actwkchk(&subActwk) == 0)
  {
    subActwk->actno = pActwk->actno;
    subActwk->userflag.b.h = -1;
    subActwk->actfree[19] = pActwk->cdsts;
    ((Sint16*)subActwk)[33] = (Uint16)(Uint8)(pActwk - actwk);
    ((Sint16*)pActwk)[28] = (Uint16)(Uint8)(subActwk - actwk);
  }
  else
  {
    frameout(pActwk);
    return;
  }

  a_init_sub(((Sint16*)pActwk)[26], pActwk);
  actwk[((Sint16*)pActwk)[26]].xposi.w.h += 2;
  actwk[((Sint16*)pActwk)[26]].yposi.w.h -= 10;
  a_init_sub(((Sint16*)pActwk)[27], pActwk);
  actwk[((Sint16*)pActwk)[27]].xposi.w.h -= 3;
  actwk[((Sint16*)pActwk)[27]].yposi.w.h -= 10;
  a_init_sub(((Sint16*)pActwk)[28], pActwk);
  actwk[((Sint16*)pActwk)[28]].xposi.w.h -= 7;
  actwk[((Sint16*)pActwk)[28]].yposi.w.h -= 9;
}

static void a_init_sub(Sint16 subact, sprite_status* pActwk) {
  actwk[subact].xposi.w.h = pActwk->xposi.w.h;
  actwk[subact].yposi.w.h = pActwk->yposi.w.h;
  actwk[subact].actflg |= 4;
  actwk[subact].sproffset = 9168;
  actwk[subact].sprpri = 3;
  actwk[subact].sprhs = 2;
  actwk[subact].sprhsize = 2;
  actwk[subact].sprvsize = 2;
}

static void a_move(sprite_status* pActwk) {
  if (!pActwk->userflag.b.h)
  {
    if (a_check(pActwk) != 0)
    {

      pActwk->r_no0 += 2;
      ((Sint16*)pActwk)[23] = 60;
      return;
    }
  }

  pActwk->xposi.l += ((Sint32*)pActwk)[12];

  if (!pActwk->userflag.b.h)
  {
    actwk[((Sint16*)pActwk)[26]].xposi.l += ((Sint32*)pActwk)[12];
    actwk[((Sint16*)pActwk)[27]].xposi.l += ((Sint32*)pActwk)[12];
    actwk[((Sint16*)pActwk)[28]].xposi.l += ((Sint32*)pActwk)[12];
  }

  --((Sint16*)pActwk)[23];
  if (((Sint16*)pActwk)[23] < 0)
  {
    ((Sint16*)pActwk)[23] = ((Sint16*)pActwk)[29];
    ((Sint32*)pActwk)[12] *= -1;
    pActwk->actflg ^= 1;
    pActwk->cddat ^= 1;

    if (!pActwk->userflag.b.h)
    {
      actwk[((Sint16*)pActwk)[26]].xposi.w.h = pActwk->xposi.w.h;
      actwk[((Sint16*)pActwk)[27]].xposi.w.h = pActwk->xposi.w.h;
      actwk[((Sint16*)pActwk)[28]].xposi.w.h = pActwk->xposi.w.h;
      if (!(pActwk->actflg & 1))
      {
        actwk[((Sint16*)pActwk)[26]].xposi.w.h -= 2;
        actwk[((Sint16*)pActwk)[27]].xposi.w.h += 3;
        actwk[((Sint16*)pActwk)[28]].xposi.w.h += 7;
      }
      else
      {
        actwk[((Sint16*)pActwk)[26]].xposi.w.h -= 1;
        actwk[((Sint16*)pActwk)[27]].xposi.w.h -= 6;
        actwk[((Sint16*)pActwk)[28]].xposi.w.h -= 10;
      }
    }
  }

  patchg(pActwk, pchg);
}

static Sint32 a_check(sprite_status* pActwk) {
  Sint16 wD0;
  Sint32 ret;

ret = 0;

  wD0 = actwk[0].yposi.w.h - pActwk->yposi.w.h - -56 - 56;
  if (wD0 < 0)
  {
    wD0 = actwk[0].xposi.w.h - pActwk->xposi.w.h;
    if (!(pActwk->actflg & 1))
      wD0 *= -1;
    wD0 -= 0x28 - 0x78;
    if (wD0 >= 0)
      ret = -1;
  }
  return ret;
}

static void a_stop(sprite_status* pActwk) {
  Sint16 subact;

  --((Sint16*)pActwk)[23];
  if (((Sint16*)pActwk)[23] < 0)
  {
    if (pActwk->actflg < 0)
      soundset(179);

((Sint16*)pActwk)[23] = 60;
    pActwk->r_no0 += 2;

    if (!pActwk->userflag.b.h)
    {
      subact = ((Sint16*)pActwk)[26];
      *(Sint32*)&actwk[subact].actfree[0] = -0x20000;
      *(Sint32*)&actwk[subact].actfree[4] = -196608;
      *(Sint32*)&actwk[subact].actfree[8] = 0;
      *(Sint32*)&actwk[subact].actfree[12] = 8192;
      actwk[subact].r_no0 += 2;
      subact = ((Sint16*)pActwk)[27];
      *(Sint32*)&actwk[subact].actfree[0] = 65536;
      *(Sint32*)&actwk[subact].actfree[4] = -196608;
      *(Sint32*)&actwk[subact].actfree[8] = 0;
      *(Sint32*)&actwk[subact].actfree[12] = 8192;
      actwk[subact].r_no0 += 2;
      subact = ((Sint16*)pActwk)[28];
      *(Sint32*)&actwk[subact].actfree[0] = 0x20000;
      *(Sint32*)&actwk[subact].actfree[4] = -196608;
      *(Sint32*)&actwk[subact].actfree[8] = 0;
      *(Sint32*)&actwk[subact].actfree[12] = 8192;
      actwk[subact].r_no0 += 2;
    }
  }
}

static void a_stop1(sprite_status* pActwk) {
  --((Sint16*)pActwk)[23];
  if (((Sint16*)pActwk)[23] < 0)
  {
    pActwk->r_no0 += 2;

    if (!pActwk->userflag.b.h)
      ((Sint32*)pActwk)[12] = 0x40000;
    else
      ((Sint32*)pActwk)[12] = 98304;

    if (!(pActwk->actflg & 1))
      ((Sint32*)pActwk)[12] *= -1;
  }

}

static void a_dash(sprite_status* pActwk) {
  pActwk->xposi.l += ((Sint32*)pActwk)[12];
}

static void tagameb1(sprite_status* pActwk) {
  Sint16 subact;
  static void(*tbl[3])(sprite_status*) =
  {
    &b_init,
    &b_follow,
    &b_move
  };

  subact = ((Sint16*)pActwk)[33];
  if (actwk[subact].actno != 45)
  {
    frameout(pActwk);
    return;
  }
  if (pActwk->actfree[19] != actwk[subact].cdsts)
  {
    frameout(pActwk);
    return;
  }
  tbl[pActwk->r_no0 / 2](pActwk);
}

static void b_init(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->patbase = pat;
  b_follow(pActwk);
}

static void b_follow(sprite_status* pActwk) {
  actionsub(pActwk);
}

static void b_move(sprite_status* pActwk) {
  pActwk->xposi.l += *(Sint32*)&pActwk->actfree[0];
  pActwk->yposi.l += *(Sint32*)&pActwk->actfree[4];
  *(Sint32*)&pActwk->actfree[0] += *(Sint32*)&pActwk->actfree[8];
  *(Sint32*)&pActwk->actfree[4] += *(Sint32*)&pActwk->actfree[12];

  if (pActwk->yposi.w.h - actwk[0].yposi.w.h > 224)
  {
    frameout(pActwk);
    return;
  }
  actionsub(pActwk);
}
