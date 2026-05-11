#include "../equ.h"
#include "sw8.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../ridechk.h"

#if defined(R82)
  #define SPRITE_SW8_BASE 315
#elif defined(R83)
  #define SPRITE_SW8_BASE 308
#else
  #define SPRITE_SW8_BASE 306
#endif

static void act_init(sprite_status* swwk);
static void act_move(sprite_status* swwk);

static sprite_pattern sw00 = {
  2,
  {
    { -16, -8, 0, SPRITE_SW8_BASE },
    { 0, -8, 8, SPRITE_SW8_BASE }
  }
};
static sprite_pattern sw01 = {
  2,
  {
    { -16, -4, 0, SPRITE_SW8_BASE + 1 },
    { 0, -4, 8, SPRITE_SW8_BASE + 1 }
  }
};
sprite_pattern* pat_sw[2] = {
  &sw00,
  &sw01
};

void sw(sprite_status* swwk) {
  if (swwk->r_no0 == 0) act_init(swwk);
  else act_move(swwk);
}

static void act_init(sprite_status* swwk) {
  swwk->r_no0 += 2;
  swwk->actflg |= 4;
  swwk->sprpri = 1;
  swwk->sprhs = swwk->sprhsize = 16;

  swwk->patbase = pat_sw;
  swwk->sprvsize = 8;

  ((Sint16*)swwk)[32] = swwk->userflag.b.h;
}

static void act_move(sprite_status* swwk) {
  Sint16 switch_adr;

  swwk->actfree[20] = swwk->actfree[21];
  switch_adr = ((Sint16*)swwk)[32];
  if (hitchk(swwk, &actwk[0]) && actwk[0].yposi.w.h <= swwk->yposi.w.h) {
    swwk->actfree[21] = 255;
    switchflag[switch_adr] |= 192;
  } else {
    swwk->actfree[21] = 0;
    switchflag[switch_adr] &= 127;
  }

if (swwk->actfree[20] == 0 && swwk->actfree[21] == 255) {

    if ((char)swwk->actflg < 0) soundset(191);

    switchflag[switch_adr] ^= 32;
    actwk[0].yposi.w.h += 8;
    swwk->yposi.w.h += 4;
    ++swwk->patno;
    swwk->sprvsize -= 4;
  }

  if (swwk->actfree[20] == 255 && swwk->actfree[21] == 0) {

    actwk[0].yposi.w.h -= 12;
    swwk->yposi.w.h -= 4;
    --swwk->patno;
    swwk->sprvsize += 4;
  }

  actionsub(swwk);
  frameout_s(swwk);
}
