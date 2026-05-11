#include "../equ.h"
#include "friend5.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../etc.h"
#include "../playsub.h"
#include "../suicide.h"

static void t_init(sprite_status* pActwk);
static void t_move(sprite_status* pActwk);
static void t_movie(sprite_status* pActwk);
static void p_init(sprite_status* pActwk);
static void p_move(sprite_status* pActwk);
static void p_movie(sprite_status* pActwk);
static void set_sproffset(sprite_status* pActwk);
static void t_roll(sprite_status* pActwk, Sint32 xsft, Sint32 ysft);

static sprite_pattern friend2_pat02;
static sprite_pattern friend1_pat02;
static sprite_pattern friend0_pat02;

static Uint8 pchg0_00[4] = { 19, 0, 1, 255 };
Uint8 pchg0_01[30] = {
  0, 0, 0, 2, 0, 0, 2, 1, 1, 2,
  2, 1, 1, 2, 2, 0, 0, 2, 0, 0,
  2, 1, 1, 2, 2, 1, 1, 2, 2, 255
};
static Uint8* pchg0[2] = {
  pchg0_00,
  pchg0_01
};
static Uint8 pchg1_00[58] = {
  0, 3, 3, 2, 3, 3, 2, 3, 3, 2,
  3, 3, 2, 3, 3, 2, 2, 3, 3, 2,
  2, 3, 3, 2, 2, 3, 3, 2, 2, 4,
  4, 2, 4, 4, 2, 4, 4, 2, 4, 4,
  2, 4, 4, 2, 2, 4, 4, 2, 2, 4,
  4, 2, 2, 4, 4, 2, 2, 255
};
static Uint8* pchg1[1] = { pchg1_00 };
static sprite_pattern friend0_pat00 = {
  1,
  { { -8, -8, 0, 435 } }
};
static sprite_pattern friend0_pat01 = {
  1,
  { { -8, -8, 0, 436 } }
};
sprite_pattern* pat_friend0[3] = {
  &friend0_pat00,
  &friend0_pat01,
  &friend0_pat02
};
static sprite_pattern friend1_pat00 = {
  1,
  { { -12, -8, 1, 437 } }
};
static sprite_pattern friend1_pat01 = {
  1,
  { { -12, -8, 1, 438 } }
};
static sprite_pattern friend1_pat03 = {
  1,
  { { -12, -8, 1, 439 } }
};
static sprite_pattern friend1_pat04 = {
  1,
  { { -12, -7, 1, 440 } }
};
sprite_pattern* pat_friend1[5] = {
  &friend1_pat00,
  &friend1_pat01,
  &friend1_pat02,
  &friend1_pat03,
  &friend1_pat04
};
static sprite_pattern friend2_pat00 = {
  1,
  { { -12, -8, 1, 441 } }
};
static sprite_pattern friend2_pat01 = {
  1,
  { { -12, -8, 1, 442 } }
};
static sprite_pattern friend2_pat03 = {
  1,
  { { -12, -8, 1, 443 } }
};
static sprite_pattern friend2_pat04 = {
  1,
  { { -12, -7, 1, 444 } }
};
static sprite_pattern* pat_friend2[5] = {
  &friend2_pat00,
  &friend2_pat01,
  &friend2_pat02,
  &friend2_pat03,
  &friend2_pat04
};
static void(*flicky_act_tbl[3])(sprite_status*) = {
  &t_init,
  &t_move,
  &t_movie
};
static void(*ricky_act_tbl[3])(sprite_status*) = {
  &p_init,
  &p_move,
  &p_movie
};

void friend(sprite_status* pActwk) {
  if (!friend_suicide(pActwk)) {

if (pActwk->userflag.b.h & 127) {

      ricky_act_tbl[pActwk->r_no0 / 2](pActwk);
      return;
    }
    flicky_act_tbl[pActwk->r_no0 / 2](pActwk);
  }
}

static void t_init(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->actflg = 4;
  pActwk->sprvsize = 8;
  pActwk->sprhs = 8;
  pActwk->sprhsize = 8;
  pActwk->sprpri = 4;
  pActwk->patbase = pat_friend0;
  ((Sint16*)pActwk)[23] = pActwk->xposi.w.h;
  ((Sint16*)pActwk)[24] = pActwk->yposi.w.h;
  pActwk->actflg ^= 1, pActwk->cddat ^= 1;
  set_sproffset(pActwk);

  if (pActwk->userflag.b.h & 128) {

    pActwk->r_no0 += 2;
    pActwk->mstno.b.h = 1;
    pActwk->sprpri = 3;
  }
  else {
    pActwk->sproffset |= 32768;
    pActwk->actfree[4] = 1;
    pActwk->actfree[5] = 1;
  }
}

static void t_move(sprite_status* pActwk) {
  char anglewk;

  t_roll(pActwk, 1, 1);

  anglewk = ((char*)pActwk)[50] + ((char*)pActwk)[51];

  if ((Uint8)(anglewk - 1) >= 127) {
    anglewk = ((char*)pActwk)[50];
    ((char*)pActwk)[51] *= -1;
    pActwk->actflg ^= 1, pActwk->cddat ^= 1;
  }

pActwk->actfree[4] = anglewk;
  patchg(pActwk, pchg0);
  actionsub(pActwk);
  frameout_s00(pActwk, ((Sint16*)pActwk)[23]);
}

static void t_movie(sprite_status* pActwk) {
  sprite_status* pMoviewk;

  pMoviewk = &actwk[((Sint16*)pActwk)[33]];
  if (pMoviewk->actno != 44 || pMoviewk->actfree[21]) {
    frameout(pActwk);
    return;
  }

  t_roll(pActwk, 3, 4);
  ((char*)pActwk)[50] += 4;
  if (!(pActwk->actfree[4] & 127)) {
    pActwk->actflg ^= 1, pActwk->cddat ^= 1;
  } else {
    patchg(pActwk, pchg0);
    actionsub(pActwk);
  }
}

static void p_init(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->actflg = 4;
  pActwk->sprvsize = 8;
  pActwk->sprhs = 8;
  pActwk->sprhsize = 8;
  pActwk->sprpri = 4;
  pActwk->patbase = pat_friend1;
  ((Sint16*)pActwk)[23] = pActwk->xposi.w.h;
  set_sproffset(pActwk);

  if (pActwk->userflag.b.h & 128) {

    pActwk->patbase = pat_friend2;
    pActwk->r_no0 += 2;
    pActwk->actflg ^= 1, pActwk->cddat ^= 1;
  }
  else {
    ((Sint32*)pActwk)[12] = 65536;
    ((Sint32*)pActwk)[13] = -0x40000;
  }
}

static void p_move(sprite_status* pActwk) {
  Sint16 colval;

  pActwk->xposi.l += ((Sint32*)pActwk)[12];
  pActwk->yposi.l += ((Sint32*)pActwk)[13];
  if ((((Sint32*)pActwk)[13] += 8192) < 0) {
    pActwk->patno = 0;
  }
  else pActwk->patno = 1;

if ((colval = emycol_d(pActwk)) < 0) {
    pActwk->yposi.w.h += colval;
    ((Sint32*)pActwk)[13] = -0x40000;
    ((Sint32*)pActwk)[12] *= -1;
    pActwk->actflg ^= 1, pActwk->cddat ^= 1;
  }

actionsub(pActwk);
  frameout_s(pActwk);
}

static void p_movie(sprite_status* pActwk) {
  sprite_status* pMoviewk;

  pMoviewk = &actwk[((Sint16*)pActwk)[33]];
  if (pMoviewk->actno != 44 || pMoviewk->actfree[21]) {
    frameout(pActwk);
    return;
  }

  patchg(pActwk, pchg1);
  actionsub(pActwk);
}

static void set_sproffset(sprite_status* pActwk) {
  Sint16 tbl0sproffset[11] = {
    1210,
    1210,
    1210,
       0,
    1210,
    1210,
    1210,
       0,
       0,
       0,
    1210
  };

  pActwk->sproffset = tbl0sproffset[time_flag + stageno.b.l * 4];
}

static void t_roll(sprite_status* pActwk, Sint32 xsft, Sint32 ysft) {
  Sint16 sinwk, coswk;

  sinset(pActwk->actfree[4], &sinwk, &coswk);
  coswk >>= xsft;
  sinwk >>= ysft;
  coswk += ((Sint16*)pActwk)[23];
  sinwk += ((Sint16*)pActwk)[24];
  pActwk->xposi.w.h = coswk;
  pActwk->yposi.w.h = sinwk;
}
