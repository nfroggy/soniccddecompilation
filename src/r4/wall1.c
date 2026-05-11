#include "../equ.h"
#include "wall1.h"
#include "../action.h"
#include "../actset.h"
#include "../ridechk.h"

#if defined(R41A) || defined(R42A)
  #define SPRITE_WALL1_BASE 450
#elif defined(R41B)
  #define SPRITE_WALL1_BASE 470
#elif defined(R41C)
  #define SPRITE_WALL1_BASE 434
#elif defined(R41D)
  #define SPRITE_WALL1_BASE 469
#elif defined(R42B)
  #define SPRITE_WALL1_BASE 466
#elif defined(R42C) || defined(R42D)
  #define SPRITE_WALL1_BASE 465
#else
  #define SPRITE_WALL1_BASE 481
#endif

static void act_init(sprite_status* pActwk);
static void act_move(sprite_status* pActwk);

static sprite_pattern pat00 = {
  1,
  { { -16, -16, 0, SPRITE_WALL1_BASE } }
};
static sprite_pattern pat01 = {
  1,
  { { -16, -16, 0, SPRITE_WALL1_BASE + 1 } }
};
static sprite_pattern pat02 = {
  1,
  { { -32, -16, 0, SPRITE_WALL1_BASE + 2 } }
};
static sprite_pattern pat03 = {
  1,
  { { -16, -32, 0, SPRITE_WALL1_BASE + 3 } }
};
static sprite_pattern pat04 = {
  1,
  { { -32, -32, 0, SPRITE_WALL1_BASE + 4 } }
};
static sprite_pattern pat05 = {
  1,
  { { -64, -16, 0, SPRITE_WALL1_BASE + 5 } }
};
static sprite_pattern pat06 = {
  1,
  { { -16, -64, 0, SPRITE_WALL1_BASE + 6 } }
};
sprite_pattern* pat_wall1[7] = {
  &pat00,
  &pat01,
  &pat02,
  &pat03,
  &pat04,
  &pat05,
  &pat06
};
static Uint8 tbl0[14] = {
  16, 16,
  16, 16,
  32, 16,
  16, 32,
  32, 32,
  64, 16,
  16, 64
};

void wall1(sprite_status* pActwk) {
  if (pActwk->r_no0) {
    act_move(pActwk);
  } else {
    act_init(pActwk);
  }
}

static void act_init(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->actflg |= 4;
  pActwk->sprpri = 1;
  pActwk->sproffset = 17514;
  pActwk->patbase = pat_wall1;
  pActwk->patno = pActwk->userflag.b.h;
  pActwk->sprhs = pActwk->sprhsize = tbl0[pActwk->userflag.b.h * 2];
  pActwk->sprvsize = tbl0[pActwk->userflag.b.h * 2 + 1];

  act_move(pActwk);
}

static void act_move(sprite_status* pActwk) {
  hitchk(pActwk, &actwk[0]);
  actionsub(pActwk);
  frameout_s(pActwk);
}
