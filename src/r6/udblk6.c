#include "../equ.h"
#include "udblk6.h"
#include "../action.h"
#include "../actset.h"
#include "../ridechk.h"

#if defined(R61B) || defined(R62B)
  #define SPRITE_UDBLK6_BASE 475
#elif defined(R63C) || defined(R63D)
  #define SPRITE_UDBLK6_BASE 448
#else
  #define SPRITE_UDBLK6_BASE 481
#endif

static sprite_pattern udblk6pat0 =
{
  1,
  { { -48, -80, 0, SPRITE_UDBLK6_BASE } }
};
sprite_pattern* udblk6pat[1] = { &udblk6pat0 };

void udblk6(sprite_status* actionwk) {
  switch (actionwk->r_no0)
  {
    case 0:
      udblk6_init(actionwk);
    case 2:
      udblk6_chk1(actionwk);
      break;
    case 4:
      udblk6_mov1(actionwk);
      break;
    case 6:
      udblk6_chk2(actionwk);
      break;
    case 8:
      udblk6_mov2(actionwk);
      break;
  }
  actionsub(actionwk);
  frameout_s(actionwk);
}

void udblk6_ridechk(sprite_status* actionwk) {
  ride_on_chk(actionwk, &actwk[0]);
}

void udblk6_init(sprite_status* actionwk) {
  actionwk->r_no0 += 2;
  actionwk->actflg |= 4;
  actionwk->sprpri = 3;
  actionwk->sproffset = 17640;
  actionwk->patbase = udblk6pat;
  actionwk->sprvsize = 80;
  actionwk->sprhsize = 48;
  ((Sint16*)actionwk)[27] = actionwk->yposi.w.h;
}

void udblk6_chk1(sprite_status* actionwk) {
  Sint16 d0;

  d0 = actionwk->yposi.w.h - actwk[0].yposi.w.h;
  if (d0 < 0)
  {
    d0 = -d0;
  }

  if (d0 < 192)
  {
    d0 = actionwk->xposi.w.h - actwk[0].xposi.w.h;
    if (d0 >= 0)
    {
      if (d0 < 136)
      {
        actionwk->actfree[18] = 0;
        ((Sint16*)actionwk)[31] = 0;
        actionwk->r_no0 += 2;
      }
    }
  }

  udblk6_ridechk(actionwk);
}

void udblk6_mov1(sprite_status* actionwk) {
  actionwk->actfree[18] = 0;
  movecnt(actionwk);
  if (actionwk->actfree[17] == 2)
    actionwk->r_no0 += 2;

  udblk6_ridechk(actionwk);
}

void udblk6_chk2(sprite_status* actionwk) {
  Sint16 d0;

  d0 = actionwk->yposi.w.h - actwk[0].yposi.w.h;
  if (d0 < 0)
  {
    d0 = -d0;
  }

  if (d0 < 192)
  {
    d0 = actwk[0].xposi.w.h - actionwk->xposi.w.h;
    if (d0 >= 0)
    {
      if (d0 >= 136)
      {
        ((Sint16*)actionwk)[31] = 0;
        actionwk->r_no0 += 2;
      }
    }
  }

  udblk6_ridechk(actionwk);
}

void udblk6_mov2(sprite_status* actionwk) {
  actionwk->actfree[18] = 1;
  movecnt(actionwk);
  if (actionwk->actfree[17] == 2)
    actionwk->r_no0 = 2;

  udblk6_ridechk(actionwk);
}

void movecnt(sprite_status* actionwk) {

  Sint16 mvtbl1[6] =
  {
    64,  8,    0,
    64, -8,  512
  };

Sint16 mvtbl2[6] =
  {
    64, -8,    0,
    64,  8, -512
  };
  Sint16* a2;
  Sint16 d0;

a2 = mvtbl1;
  if (actionwk->actfree[18] != 0) a2 = mvtbl2;

  if (actionwk->actfree[16] == 0)
  {
    d0 = actionwk->actfree[17] * 3;
    actionwk->actfree[16] = (char)a2[d0];
    ((Sint16*)actionwk)[33] = a2[d0 + 1];
    ((Sint16*)actionwk)[26] = a2[d0 + 2];
  }
  else
  {

    actionwk->yposi.l += actionwk->yspeed.w << 8;
    actionwk->yspeed.w += ((Sint16*)actionwk)[33];

    if (--actionwk->actfree[16] == 0)
      ++actionwk->actfree[17];
  }
}
