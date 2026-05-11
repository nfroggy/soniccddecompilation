#include "../equ.h"
#include "denden.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../loader2.h"
#include "../playsub.h"
#include "../suicide.h"

static void m_init(sprite_status* pActwk);
static void m_fall(sprite_status* pActwk);
static void m_move(sprite_status* pActwk);
static Sint16 m_check(sprite_status* pActwk);
static void sub(sprite_status* pActwk);

static sprite_pattern pat00 =
{
  1,
  { { -16, -24, 0, 456 } }
};
static sprite_pattern pat01 =
{
  2,
  {
    { -16, -8, 0, 457 },
    { -16, -24, 0, 458 }
  }
};
static sprite_pattern pat02 =
{
  2,
  {
    { -16, -24, 0, 459 },
    { -16, -8, 0, 460 }
  }
};
static sprite_pattern pat03 =
{
  3,
  {
    { -16, -24, 0, 461 },
    { -16, -8, 0, 462 },
    { -16, -8, 0, 463 }
  }
};
static sprite_pattern pat04 =
{
  1,
  { { -16, -16, 0, 464 } }
};
static sprite_pattern pat05 =
{
  2,
  {
    { -16, -8, 0, 465 },
    { -16, -16, 0, 466 }
  }
};
static sprite_pattern pat06 =
{
  1,
  { { -16, -20, 0, 467 } }
};
sprite_pattern* pat_denden_e[4] =
{
  &pat00,
  &pat01,
  &pat02,
  &pat03
};
sprite_pattern* pat_denden_b[2] =
{
  &pat04,
  &pat05
};
sprite_pattern* pat_none[1] = { &pat06 };
static Uint8 p00[4] = { 29, 0, 1, 255 };
static Uint8 p01[4] = { 29, 2, 3, 255 };
static Uint8* pchg[2] =
{
  p00,
  p01
};

void denden(sprite_status* pActwk) {
  void(*tbl[3])(sprite_status*) =
  {
    &m_init,
    &m_fall,
    &m_move
  };

  if (pActwk->userflag.b.h & 128)
  {
    sub(pActwk);
    return;
  }

  if (enemy_suicide(pActwk)) return;
  tbl[pActwk->r_no0 / 2](pActwk);
  actionsub(pActwk);
  frameout_s00(pActwk, ((Sint16*)pActwk)[23]);

}

static void m_init(sprite_status* pActwk) {
  ((Sint16*)pActwk)[23] = pActwk->xposi.w.h;
  pActwk->actflg |= 4;
  pActwk->sprpri = 3;
  pActwk->sproffset = 9104;
  pActwk->sprhs = 16;
  pActwk->sprhsize = 16;
  pActwk->sprvsize = 15;
  pActwk->colino = 47;
  ((Sint32*)pActwk)[12] = -16384;
  pActwk->r_no0 += 2;

  if (pActwk->userflag.b.h) pActwk->patbase = pat_denden_b;
  else pActwk->patbase = pat_denden_e;

  m_fall(pActwk);
}

static void m_fall(sprite_status* pActwk) {
  Sint16 d1;

  pActwk->yposi.l += 65536;
  d1 = emycol_d(pActwk);
  if (d1 < 0)
  {
    pActwk->yposi.w.h += d1;
    pActwk->r_no0 += 2;
  }
}

static void m_move(sprite_status* pActwk) {
  sprite_status* pPlayerwk;
  sprite_status* pNewActwk;
  Sint16 d0, d1;

  pActwk->xposi.l += ((Sint32*)pActwk)[12];
  d0 = pActwk->xposi.w.h;
  d0 -= ((Sint16*)pActwk)[23];
  if (d0 < 0)
  {
    d0 *= -1;
  }
  if (d0 >= 80) goto label3;
  d1 = emycol_d(pActwk);
  d0 = d1;
  d0 += 7;
  if ((Uint16)d0 >= 14) goto label3;
  pActwk->yposi.w.h += d1;

  if (pActwk->userflag.b.h) goto label2;
  pPlayerwk = &actwk[0];
  if (m_check(pActwk) == 0) goto label1;
  pActwk->mstno.b.h = 1;
  if (((Sint16*)pActwk)[33]) goto label2;
  if (actwkchk(&pNewActwk) != 0) goto label2;
  pNewActwk->actno = pActwk->actno;
  pNewActwk->userflag.b.h = -1;
  pNewActwk->userflag.b.l = pActwk->userflag.b.l;
  pNewActwk->xposi.w.h = pActwk->xposi.w.h;
  pNewActwk->yposi.w.h = pActwk->yposi.w.h;
  pNewActwk->yposi.w.h -= 20;
  pNewActwk->sproffset = pActwk->sproffset;
  pNewActwk->patbase = pat_none;
  pNewActwk->colino = 176;
  ((Sint16*)pNewActwk)[33] = (Uint16)(pActwk - actwk);
  ((Sint16*)pActwk)[33] = (Uint16)(pNewActwk - actwk);
  if (pActwk->actflg & 128)
  {
    soundset(183);
  }
  goto label2;
label1:
  pActwk->mstno.b.h = 0;
  if (((Sint16*)pActwk)[33] == 0) goto label2;
  frameout(&actwk[((Sint16*)pActwk)[33]]);
  ((Sint16*)pActwk)[33] = 0;
label2:
  patchg(pActwk, pchg);
  return;
label3:
  ((Sint32*)pActwk)[12] *= -1;
  pActwk->actflg ^= 1;
  pActwk->cddat ^= 1;

}

static Sint16 m_check(sprite_status* pActwk) {
  sprite_status* pPlayerwk;
  Sint16 d0;
  Sint16 carry_flag;

  pPlayerwk = &actwk[0];
  d0 = pPlayerwk->yposi.w.h;
  d0 -= pActwk->yposi.w.h;
  d0 += 120;
  if ((Uint16)d0 < 240)
  {
    carry_flag = 1;
    d0 = pPlayerwk->xposi.w.h;
    d0 -= pActwk->xposi.w.h;
    d0 += 120;
    if ((Uint16)d0 < 240)
    {
      carry_flag = 1;
    }
    else
    {
      carry_flag = 0;
    }
  }
  else
  {
    carry_flag = 0;
  }
  return carry_flag;
}

static void sub(sprite_status* pActwk) {
  sprite_status* pMainwk;

  pMainwk = &actwk[((Sint16*)pActwk)[33]];
  if (pMainwk->actno != 32)
  {
    frameout(pActwk);
    return;
  }
  if (pActwk->userflag.b.l != pMainwk->userflag.b.l)
  {
    frameout(pActwk);
    return;
  }
  pActwk->xposi.w.h = pMainwk->xposi.w.h;
  pActwk->yposi.w.h = pMainwk->yposi.w.h;
  pActwk->yposi.w.h -= 20;
  actionsub(pActwk);
}
