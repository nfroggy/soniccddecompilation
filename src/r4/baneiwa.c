#include "../equ.h"
#include "../action.h"
#include "../actset.h"
#include "../ridechk.h"
#include "baneiwa.h"

typedef struct {
  Uint8 a;
  char b;
  Sint16 c;
}
move_data;

static void baneiwa_init(sprite_status* pActwk);
static void baneiwa_move(sprite_status* pActwk);

static sprite_pattern pat0 =
{
  1,
  { { -16, -32, 0, 500 } }
};
sprite_pattern* baneiwapat[1] = { &pat0 };

void baneiwa(sprite_status* pActwk) {
  sprite_status* pRideAct;
  void(*tbl[2])(sprite_status*) =
  {
    &baneiwa_init,
    &baneiwa_move
  };

  tbl[pActwk->r_no0 / 2](pActwk);
  actionsub(pActwk);
  if (pActwk->actfree[18])
  {
    pRideAct = &actwk[((Sint16*)pActwk)[28]];
    if (pRideAct->actno != 72)
    {
      frameout(pActwk);
    }
  }
  else
  {
    frameout_s(pActwk);
  }
}

static void baneiwa_init(sprite_status* pActwk) {
  sprite_status* pNewActwk;

  pActwk->r_no0 += 2;
  pActwk->actflg |= 4;
  pActwk->sproffset = 17514;
  pActwk->sprpri = 3;
  pActwk->patbase = baneiwapat;
  pActwk->sprhsize = 16;
  pActwk->sprvsize = 32;

  if (pActwk->actfree[18] == 0)
  {
    if (actwkchk(&pNewActwk) == 0)
    {
      pNewActwk->actno = 72;
      pNewActwk->xposi.w.h = pActwk->xposi.w.h + 32;
      pNewActwk->yposi.w.h = pActwk->yposi.w.h;
      pNewActwk->actfree[18] = 1;

      ((Sint16*)pNewActwk)[28] = (Uint16)(pActwk - actwk);
    }
  }
}

static void baneiwa_move(sprite_status* pActwk) {
  Sint16 d0;
  Sint16 d1;
  Uint8 index;

move_data baneiwa_mvtbl[4] =
  {
    { 128, -4,  512 },
    { 128, -4,    0 },
    { 128,  4, -512 },
    { 128,  4,    0 }
  };

if (pActwk->yspeed.w < 0)
  {

    ride_on_chk(pActwk, &actwk[0]);
  }
  if (pActwk->actfree[16])
  {
    pActwk->yposi.l += pActwk->yspeed.w << 8;
    pActwk->yspeed.w += ((Sint16*)pActwk)[33];
    --pActwk->actfree[16];
    if (pActwk->actfree[16]) goto label1;

++pActwk->actfree[17];
    if (pActwk->actfree[17] != 4) goto label1;

pActwk->actfree[17] = 0;
    goto label1;
  }

  index = pActwk->actfree[17];
  pActwk->actfree[16] = baneiwa_mvtbl[index].a;
  d0 = baneiwa_mvtbl[index].b;
  d1 = baneiwa_mvtbl[index].c;
  if (pActwk->actfree[18])
  {
    d0 *= -1;
    d1 *= -1;
  }
  ((Sint16*)pActwk)[33] = d0;
  pActwk->yspeed.w = d1;
label1:
  if (pActwk->yspeed.w >= 0)
  {

    ride_on_chk(pActwk, &actwk[0]);
  }
}
