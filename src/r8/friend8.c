#include "../equ.h"
#include "friend8.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../etc.h"
#include "../playsub.h"
#include "../suicide.h"

#if defined(R82)
  #define SPRITE_FRIEND8_BASE 459
#elif defined(R83)
  #define SPRITE_FRIEND8_BASE 439
#else
  #define SPRITE_FRIEND8_BASE 497
#endif

static void t_init(sprite_status* actionwk);
static void t_move(sprite_status* actionwk);
static void t_roll(sprite_status* actionwk, Sint16 d2, Sint16 d3);
static void p_init(sprite_status* actionwk);
static void p_move(sprite_status* actionwk);
static void set_sproffset(sprite_status* actionwk);

static sprite_pattern pat00 =
{
  1,
  { { -8, -8, 0, SPRITE_FRIEND8_BASE } }
};
static sprite_pattern pat01 =
{
  1,
  { { -8, -8, 0, SPRITE_FRIEND8_BASE + 1 } }
};
static sprite_pattern pat02 =
{
  1,
  { { -12, -8, 0, SPRITE_FRIEND8_BASE + 2 } }
};
static sprite_pattern pat03 =
{
  1,
  { { -12, -8, 0, SPRITE_FRIEND8_BASE + 3 } }
};
sprite_pattern* pat_friend0[2] =
{
  &pat00,
  &pat01
};
sprite_pattern* pat_friend1[2] =
{
  &pat02,
  &pat03
};
static char p00[4] = { 3, 0, 1, -1 };
Uint8 p01[30] =
{
  0, 0, 0, 2, 0, 0, 2, 1, 1, 2,
  2, 1, 1, 2, 2, 0, 0, 2, 0, 0,
  2, 1, 1, 2, 2, 1, 1, 2, 2, 255
};
static char p10[58] =
{
  0, 3, 3, 2, 3, 3, 2, 3, 3, 2,
  3, 3, 2, 3, 3, 2, 2, 3, 3, 2,
  2, 3, 3, 2, 2, 3, 3, 2, 2, 4,
  4, 2, 4, 4, 2, 4, 4, 2, 4, 4,
  2, 4, 4, 2, 2, 4, 4, 2, 2, 4,
  4, 2, 2, 4, 4, 2, 2, -1
};
static char* pchg0[2] =
{
  p00,
  (char*)p01
};
static Uint16 tbl0sproffset[11] =
{
  1943, 986, 986, 0,
  1943, 986, 986, 0,
     0,   0, 986
};

void friend(sprite_status* actionwk) {

  if (friend_suicide(actionwk) == -1) return;

if (actionwk->userflag.b.h & 127)
    sheep(actionwk);
  else
    hato(actionwk);
}

void hato(sprite_status* actionwk) {
  switch (actionwk->r_no0)
  {
    case 0:
      t_init(actionwk);
      break;
    case 2:
      t_move(actionwk);
      break;
  }
}

static void t_init(sprite_status* actionwk) {
  actionwk->r_no0 += 2;
  actionwk->actflg = 4;

  actionwk->sprvsize = 8;
  actionwk->sprhs = 8;
  actionwk->sprpri = 4;
  actionwk->sprhsize = 8;
  actionwk->patbase = pat_friend0;
  ((Sint16*)actionwk)[23] = actionwk->xposi.w.h;
  ((Sint16*)actionwk)[24] = actionwk->yposi.w.h;
  actionwk->actflg ^= 1, actionwk->cddat ^= 1;
  set_sproffset(actionwk);

actionwk->actfree[4] = 1;
  actionwk->actfree[5] = 1;
}

static void t_move(sprite_status* actionwk) {
  char d0, d1;

  t_roll(actionwk, 1, 1);
  d1 = d0 = (char)actionwk->actfree[4] + (char)actionwk->actfree[5];
  d1 += 126;
  if (d1 >= 0)
  {
    d0 = actionwk->actfree[4];
    actionwk->actfree[5] = -actionwk->actfree[5];
    actionwk->actflg ^= 1, actionwk->cddat ^= 1;
  }

  actionwk->actfree[4] = d0;
  patchg(actionwk, (Uint8**)pchg0);
  actionsub(actionwk);
  frameout_s00(actionwk, ((Sint16*)actionwk)[23]);
}

static void t_roll(sprite_status* actionwk, Sint16 d2, Sint16 d3) {
  Sint16 sin, cos;

  sinset(actionwk->actfree[4], &sin, &cos);
  cos >>= d2;
  sin >>= d3;
  actionwk->xposi.w.h = cos + ((Sint16*)actionwk)[23];
  actionwk->yposi.w.h = sin + ((Sint16*)actionwk)[24];
}

void sheep(sprite_status* actionwk) {
  switch (actionwk->r_no0)
  {
    case 0:
      p_init(actionwk);
      break;
    case 2:
      p_move(actionwk);
      break;
  }
}

static void p_init(sprite_status* actionwk) {
  actionwk->r_no0 += 2;
  actionwk->actflg |= 4;
  actionwk->sprvsize = 7;
  actionwk->sprhs = 8;
  actionwk->sprpri = 4;
  actionwk->sprhsize = 8;
  actionwk->patbase = pat_friend1;

  set_sproffset(actionwk);

  ((Sint32*)actionwk)[12] = 65536;
  ((Sint32*)actionwk)[13] = -0x40000;
}

static void p_move(sprite_status* actionwk) {
  Sint16 d1;
  Sint32 spd;

  actionwk->xposi.l += ((Sint32*)actionwk)[12];
  actionwk->yposi.l += ((Sint32*)actionwk)[13];

  ((Sint32*)actionwk)[13] += 8192;
  spd = ((Sint32*)actionwk)[13];
  if (spd < 0)
    actionwk->patno = 0;
  else
    actionwk->patno = 1;
  d1 = emycol_d(actionwk);
  if (d1 < 0)
  {

    actionwk->yposi.w.h += d1;
    ((Sint32*)actionwk)[13] = -0x40000;
    ((Sint32*)actionwk)[12] = -((Sint32*)actionwk)[12];

    actionwk->actflg ^= 1, actionwk->cddat ^= 1;
  }
  p_end(actionwk);
}

void p_end(sprite_status* actionwk) {
  actionsub(actionwk);
  frameout_s(actionwk);
}

static void set_sproffset(sprite_status* actionwk) {
  Uint16* a1;
  Uint16 d0;

  a1 = tbl0sproffset;
  d0 = ((stageno.w + 1) << 2) + (Uint16)time_flag;
  actionwk->sproffset = a1[d0];
}
