#include "../equ.h"
#include "tekkyu7.h"
#include "../action.h"
#include "../actset.h"

#if defined(R71) || defined(R72)
  #define SPRITE_TEKKYU7_BASE 427
#elif defined(R73C) || defined(R73D)
  #define SPRITE_TEKKYU7_BASE 410
#else
  #define SPRITE_TEKKYU7_BASE 462
#endif

static void tekkyu7_init(sprite_status* pActwk);
static void tekkyu7_move(sprite_status* pActwk);
static void tekkyu7_stop(sprite_status* pActwk);

static sprite_pattern tekkyu7_pat0 = {
  1,
  { { -16, -16, 0, SPRITE_TEKKYU7_BASE } }
};
sprite_pattern* pat_tekkyu7[1] = { &tekkyu7_pat0 };

void tekkyu7(sprite_status* pActwk) {
  void(*tekkyu7_acttbl[3])(sprite_status*) = {
    &tekkyu7_init,
    &tekkyu7_move,
    &tekkyu7_stop
  };

  tekkyu7_acttbl[pActwk->r_no0 / 2](pActwk);
  actionsub(pActwk);
  frameout_s00(pActwk, ((Sint16*)pActwk)[23]);
}

void tekkyu7_init(sprite_status* pActwk) {
  Sint16 tekkyu7_tbl0[24] = {
    1024,    0, 24, 12,
       0, 1024, 16,  8,
     512,    0, 48, 24,
       0,  512, 32, 16,
     256,    0, 96, 48,
       0,  256, 64, 32
  };

  Sint16 tekkyu7_tbl1[3] = {
    60,
    40,
    20
  };

  pActwk->r_no0 += 2;
  pActwk->actflg |= 4;
  pActwk->sprpri = 3;
  pActwk->colino = 181;
  pActwk->sprhs = 16;
  pActwk->sprhsize = 16;
  pActwk->sprvsize = 16;
  pActwk->sproffset = 902;
  pActwk->patbase = pat_tekkyu7;
  ((Sint16*)pActwk)[23] = pActwk->xposi.w.h;
  ((Sint16*)pActwk)[25] = tekkyu7_tbl1[time_flag];

  *(Sint32*)&pActwk->actfree[8] = tekkyu7_tbl0[time_flag * 8 + pActwk->userflag.b.h * 4] << 8;
  *(Sint32*)&pActwk->actfree[12] = tekkyu7_tbl0[time_flag * 8 + pActwk->userflag.b.h * 4 + 1] << 8;
  ((Sint16*)pActwk)[24] = tekkyu7_tbl0[time_flag * 8 + pActwk->userflag.b.h * 4 + 2];
  ((Sint16*)pActwk)[26] = tekkyu7_tbl0[time_flag * 8 + pActwk->userflag.b.h * 4 + 3];

  tekkyu7_move(pActwk);
}

void tekkyu7_move(sprite_status* pActwk) {
  pActwk->xposi.l += *(Sint32*)&pActwk->actfree[8];
  pActwk->yposi.l += *(Sint32*)&pActwk->actfree[12];
  if (!(--((Sint16*)pActwk)[26])) {
    pActwk->r_no0 += 2;
    ((Sint16*)pActwk)[26] = ((Sint16*)pActwk)[25];
    *(Sint32*)&pActwk->actfree[8] *= -1;
    *(Sint32*)&pActwk->actfree[12] *= -1;
  }
}

void tekkyu7_stop(sprite_status* pActwk) {
  if (!(--((Sint16*)pActwk)[26])) {
    pActwk->r_no0 -= 2;
    ((Sint16*)pActwk)[26] = ((Sint16*)pActwk)[24];
  }
}
