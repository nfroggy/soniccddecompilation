#include "../equ.h"
#include "hachi8.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"
#include "../suicide.h"

extern void patchg(sprite_status* patchgwk, Uint8** pat_dat);

static void ball(sprite_status* actionwk);
static void ball_move(sprite_status* actionwk);

static char p00[4] = { 2, 0, 1, -1 };
static char* pchg[1] = { p00 };
static sprite_pattern body_pat0 =
{
  1,
  { { -19, -20, 0, 469 } }
};
static sprite_pattern body_pat1 =
{
  1,
  { { -19, -20, 0, 470 } }
};
static sprite_pattern kusari_00 =
{
  1,
  { { -8, -8, 0, 471 } }
};
static sprite_pattern ball_pat0 =
{
  1,
  { { -12, -12, 0, 472 } }
};
static sprite_pattern kusari_01 =
{
  1,
  { { -8, -8, 0, 473 } }
};
static sprite_pattern ball_pat1 =
{
  1,
  { { -12, -12, 0, 474 } }
};
sprite_pattern* pat_hachi8_e[4] =
{
  &body_pat0,
  &body_pat1,
  &kusari_00,
  &ball_pat0
};
sprite_pattern* pat_hachi8_b[4] =
{
  &body_pat0,
  &body_pat1,
  &kusari_01,
  &ball_pat1
};

void hachi8(sprite_status* actionwk) {
  if (actionwk->userflag.b.l == 0)
  {
    body(actionwk);
    return;
  }
  if (actionwk->userflag.b.l > 0)
  {
    kusari(actionwk);
    return;
  }
  ball(actionwk);
}

void body(sprite_status* actionwk) {
  sprite_status* a1;

  if (enemy_suicide(actionwk) == -1) return;

  if (actionwk->r_no0 != 0)
  {
    body_move(actionwk);
    return;
  }

  actionwk->r_no0 += 2;
  actionwk->actflg |= 4;
  actionwk->sprpri = 3;
  actionwk->sprhs = 16;
  actionwk->sprhsize = 16;
  actionwk->sprvsize = 16;
  actionwk->colino = 46;
  actionwk->sproffset = 9301;
  ((Sint16*)actionwk)[30] = actionwk->xposi.w.h;
  ((Sint16*)actionwk)[31] = actionwk->yposi.w.h;
  ((Sint16*)actionwk)[26] = 0;
  if (actionwk->userflag.b.h == 0)
  {
    actionwk->patbase = pat_hachi8_e;
    *(Sint32*)&actionwk->actfree[0] = -65536;
    ((Sint16*)actionwk)[25] = 128;
    ((Sint16*)actionwk)[28] = 1024;
    ((Sint16*)actionwk)[29] = 3;
  }
  else
  {
    actionwk->patbase = pat_hachi8_b;
    *(Sint32*)&actionwk->actfree[0] = -32768;
    ((Sint16*)actionwk)[25] = 256;
    ((Sint16*)actionwk)[28] = 768;
    ((Sint16*)actionwk)[29] = 4;
  }

  if (actwkchk2(actionwk, &a1) != 0)
  {
    frameout(actionwk);
    return;
  }
  a1->actno = actionwk->actno;
  ((Uint16*)actionwk)[32] = a1 - actwk;
  ((Uint16*)a1)[33] = actionwk - actwk;
  a1->actflg = actionwk->actflg;
  a1->sproffset = actionwk->sproffset;
  a1->patbase = actionwk->patbase;
  a1->userflag.b.h = actionwk->userflag.b.h;
  a1->userflag.b.l = -1;
  a1->xposi.w.h = actionwk->xposi.w.h;
  a1->yposi.w.h = actionwk->yposi.w.h;

if (actionwk->userflag.b.h != 0)
  {
    body_move(actionwk);
    return;
  }
  if (actwkchk2(actionwk, &a1) != 0)
  {
    frameout(actionwk);
    return;
  }

  a1->actno = actionwk->actno;
  ((Uint16*)actionwk)[33] = a1 - actwk;
  ((Uint16*)a1)[33] = actionwk - actwk;
  a1->actflg = actionwk->actflg;
  a1->sproffset = actionwk->sproffset;
  a1->patbase = actionwk->patbase;
  a1->userflag.b.h = actionwk->userflag.b.h;
  a1->userflag.b.l = -2;
  a1->xposi.w.h = actionwk->xposi.w.h;
  a1->yposi.w.h = actionwk->yposi.w.h;
}

void body_move(sprite_status* actionwk) {
  Uint16 d0, sin, cos;
  Sint32 d0l;
  Sint16 t;

  actionwk->xposi.l += *(Sint32*)&actionwk->actfree[0];
  --((Sint16*)actionwk)[26];
  t = ((Sint16*)actionwk)[26];
  if (t < 0)
  {
    ((Sint32*)actionwk)[13] = ((Sint16*)actionwk)[25];

    *(Sint32*)&actionwk->actfree[0] = -*(Sint32*)&actionwk->actfree[0];

    actionwk->actflg ^= 1;
    actionwk->cddat ^= 1;
  }

  actionwk->yposi.w.h = ((Sint16*)actionwk)[31];
  actionwk->yposi.w.l = 0;
  ((Sint16*)actionwk)[27] += ((Sint16*)actionwk)[28];

  d0 = ((Sint16*)actionwk)[27] >> 8;
  sinset(d0, (Sint16*)&sin, (Sint16*)&cos);
  d0l = sin << 16;
  d0l >>= ((Sint16*)actionwk)[29];
  actionwk->yposi.l += d0l;
  patchg(actionwk, (Uint8**)pchg);
  actionsub(actionwk);
  frameout_s00(actionwk, ((Sint16*)actionwk)[30]);
}

static void ball(sprite_status* actionwk) {
  sprite_status* a1;
  Sint16 i, d6;

  if (actionwk->r_no0 != 0)
  {
    ball_move(actionwk);
    return;
  }

  actionwk->r_no0 += 2;
  actionwk->sprhs = 12;
  actionwk->sprhsize = 12;
  actionwk->sprvsize = 12;
  actionwk->sprpri = 2;
  actionwk->patno = 3;
  actionwk->colino = 175;
  if (actionwk->userflag.b.l == -1)
  {
    ((Sint16*)actionwk)[23] = 2048;
    ((Sint16*)actionwk)[25] = 8;
    ((Sint16*)actionwk)[26] = 9;
  }
  else
  {
    ((Sint16*)actionwk)[23] = 4096;
    ((Sint16*)actionwk)[25] = 8;
    ((Sint16*)actionwk)[26] = 3;
  }

  d6 = ((Sint16*)actionwk)[26];

for (i = 0; i <= d6; ++i)
  {
    if (actwkchk2(actionwk, &a1) != 0)
    {

      frameout(actionwk);
      return;
    }

    a1->actno = actionwk->actno;
    actionwk->actfree[i + 8] = a1 - actwk;
    ((Uint16*)a1)[33] = ((Uint16*)actionwk)[33];

    a1->actflg = actionwk->actflg;
    a1->sproffset = actionwk->sproffset;
    a1->patbase = actionwk->patbase;
    a1->userflag.b.h = actionwk->userflag.b.h;
    a1->userflag.b.l = 1;
    a1->xposi.w.h = actionwk->xposi.w.h;
    a1->yposi.w.h = actionwk->yposi.w.h;
  }
  ball_move(actionwk);
}

static void ball_move(sprite_status* actionwk) {
  int_union d2, d3;
  Sint16 d0, d6, i, ano;
  Uint16 sin, cos;
  Sint32 d4, d5;

  ((Sint16*)actionwk)[24] += ((Sint16*)actionwk)[25];

  d6 = ((Sint16*)actionwk)[23];
  ((Sint16*)actionwk)[23] += ((Sint16*)actionwk)[24];

if ((((Sint16*)actionwk)[25] >= 0
        && ((Uint16*)actionwk)[24] < 16384
        && ((Uint16*)actionwk)[23] >= 16384)
      ||
      (((Sint16*)actionwk)[25] < 0
        && ((Uint16*)actionwk)[24] >= 16384
        && ((Uint16*)actionwk)[23] < 16384))
  {
    ((Sint16*)actionwk)[25] = -((Sint16*)actionwk)[25];
  }

d6 = ((Sint16*)actionwk)[26];
  ano = ((Uint16*)actionwk)[33];
  d5 = actwk[ano].yposi.l + 786432;
  d4 = actwk[ano].xposi.l;
  d0 = ((Sint16*)actionwk)[23] >> 8;
  sinset(d0, (Sint16*)&sin, (Sint16*)&cos);
  d3.l = sin;
  d2.l = cos;
  d3.l = (Uint32)d3.l >> 16 & 65535 | d3.l << 16 & -65536;
  d2.l = (Uint32)d2.l >> 16 & 65535 | d2.l << 16 & -65536;
  d3.w.l = 0;
  d2.w.l = 0;
  d3.l >>= 5;
  d2.l >>= 5;

for (i = 0; i <= d6; ++i)
  {
    d5 += d3.l;
    d4 += d2.l;
    ano = actionwk->actfree[i + 8];
    actwk[ano].yposi.l = d5;
    actwk[ano].xposi.l = d4;
  }
  d5 += d3.l;
  d4 += d2.l;
  actionwk->yposi.l = d5;
  actionwk->xposi.l = d4;
  kusari_move(actionwk);
}

void kusari(sprite_status* actionwk) {
  if (actionwk->r_no0 != 0)
  {
    kusari_move(actionwk);
    return;
  }

  actionwk->r_no0 += 2;
  actionwk->sprhs = 8;
  actionwk->sprhsize = 8;
  actionwk->sprvsize = 8;
  actionwk->sprpri = 3;
  actionwk->patno = 2;
  kusari_move(actionwk);
}

void kusari_move(sprite_status* actionwk) {
  Sint16 ano;

  ano = ((Uint16*)actionwk)[33];
  if (actwk[ano].actno != 46)
  {
    frameout(actionwk);
    return;
  }
  actionsub(actionwk);
}
