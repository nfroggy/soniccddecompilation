#include "../equ.h"
#include "tonbo.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"
#include "../suicide.h"
#include "playsub4.h"

#if defined(R41A)
  #define SPRITE_TONBO_BASE 473
#elif defined(R41B)
  #define SPRITE_TONBO_BASE 434
#elif defined(R41C)
  #define SPRITE_TONBO_BASE 451
#elif defined(R42A)
  #define SPRITE_TONBO_BASE 479
#elif defined(R42B)
  #define SPRITE_TONBO_BASE 432
#elif defined(R43C) || defined(R43D)
  #define SPRITE_TONBO_BASE 429
#else
  #define SPRITE_TONBO_BASE 436
#endif

static void act_init(sprite_status* pActwk);
static void act_lr(sprite_status* pActwk);

static Uint8 pchg0[4] = { 2, 0, 1, 255 };
static Uint8 pchg1[4] = { 4, 0, 2, 255 };
static Uint8* pchg[2] =
{
  pchg0,
  pchg1
};
static sprite_pattern spr_tonbo_e_00 =
{
  1,
  { { -29, -17, 0, SPRITE_TONBO_BASE + 3 } }
};
static sprite_pattern spr_tonbo_e_01 =
{
  1,
  { { -29, -13, 0, SPRITE_TONBO_BASE + 4 } }
};
static sprite_pattern spr_tonbo_e_02 =
{
  1,
  { { -29, -17, 0, SPRITE_TONBO_BASE + 5 } }
};
static sprite_pattern spr_tonbo_b_00 =
{
  1,
  { { -29, -17, 0, SPRITE_TONBO_BASE } }
};
static sprite_pattern spr_tonbo_b_01 =
{
  1,
  { { -29, -13, 0, SPRITE_TONBO_BASE + 1 } }
};
static sprite_pattern spr_tonbo_b_02 =
{
  1,
  { { -29, -17, 0, SPRITE_TONBO_BASE + 2 } }
};
sprite_pattern* pat_tonbo_e[3] =
{
  &spr_tonbo_e_00,
  &spr_tonbo_e_01,
  &spr_tonbo_e_02
};
sprite_pattern* pat_tonbo_b[3] =
{
  &spr_tonbo_b_00,
  &spr_tonbo_b_01,
  &spr_tonbo_b_02
};

void tonbo(sprite_status* pActwk) {
  Sint16 temp;
  static void(*act_tbl[2])(sprite_status*) =
  {
    &act_init,
    &act_lr
  };

  if (enemy_suicide(pActwk)) return;
  act_tbl[pActwk->r_no0 / 2](pActwk);
  actionsub(pActwk);
  temp = ((Sint16*)pActwk)[33];
  frameout_s00(pActwk, temp);
}

static void act_init(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->actflg = 4;
  pActwk->colino = 44;
  pActwk->sprpri = 1;
  pActwk->sprhs = 28;
  pActwk->sprhsize = 28;
  pActwk->sprvsize = 16;
  pActwk->sproffset = 9238;
  ((Sint16*)pActwk)[33] = pActwk->xposi.w.h;
  *(Sint32*)&pActwk->actfree[0] = pActwk->yposi.l;

  if (!pActwk->userflag.b.h)
  {
    pActwk->patbase = pat_tonbo_e;
    ((Sint32*)pActwk)[13] = -65536;
    ((Sint16*)pActwk)[28] = 4;
    ((Sint16*)pActwk)[30] = 256;
  }
  else
  {
    pActwk->patbase = pat_tonbo_b;
    ((Sint32*)pActwk)[13] = -32768;
    ((Sint16*)pActwk)[28] = 1;
    ((Sint16*)pActwk)[30] = 512;
  }
  ((Sint16*)pActwk)[29] = ((Sint16*)pActwk)[30];
  ((Sint16*)pActwk)[29] >>= 1;
}

static void act_lr(sprite_status* pActwk) {
  Sint16 sin;
  Sint16 cos;
  int_union sinl;

pActwk->xposi.l += ((Sint32*)pActwk)[13];
  sinset(pActwk->actfree[4], &sin, &cos);
  sinl.l = 0;
  sinl.w.h = sin;

sinl.l >>= 4;
  pActwk->yposi.l = *(Sint32*)&pActwk->actfree[0] + sinl.l;

  ((Sint16*)pActwk)[25] += ((Sint16*)pActwk)[28];

  --((Sint16*)pActwk)[29];
  if (!((Sint16*)pActwk)[29])
  {
    ((Sint16*)pActwk)[29] = ((Sint16*)pActwk)[30];
    ((Sint32*)pActwk)[13] *= -1;
    pActwk->actflg ^= 1;
    pActwk->cddat ^= 1;
  }

  patchg(pActwk, pchg);
}
