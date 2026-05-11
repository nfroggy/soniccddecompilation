#include "../equ.h"
#include "beltsw5.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"

static void beltsw5_init(sprite_status* pActwk);
static void beltsw5_move1(sprite_status* pActwk);
static void beltsw5_move2(sprite_status* pActwk);
static void beltsw5_move3(sprite_status* pActwk);

static sprite_pattern beltsw5_pat0 = {
  1,
  { { -4, -16, 0, 406 } }
};
static sprite_pattern beltsw5_pat1 = {
  1,
  { { -8, -36, 0, 407 } }
};
static sprite_pattern beltsw5_pat2 = {
  1,
  { { -28, -16, 0, 408 } }
};
sprite_pattern* beltsw5pat[3] = {
  &beltsw5_pat0,
  &beltsw5_pat1,
  &beltsw5_pat2
};

void beltsw5(sprite_status* pActwk) {
  void(*beltsw5_move_tbl[4])(sprite_status*) = {
    &beltsw5_init,
    &beltsw5_move1,
    &beltsw5_move2,
    &beltsw5_move3
  };

  beltsw5_move_tbl[pActwk->r_no0 / 2](pActwk);
  actionsub(pActwk);
  frameout_s(pActwk);
}

static void beltsw5_init(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->actflg |= 4;
  pActwk->sprpri = 3;
  pActwk->sprhsize = 24;
  pActwk->sprvsize = 24;
  pActwk->sproffset = 18327;
  pActwk->patbase = beltsw5pat;

  if (colrevflag & 1) pActwk->patno = 2;
  pActwk->actfree[18] = colrevflag;
  beltsw5_move1(pActwk);
}

static void beltsw5_move1(sprite_status* pActwk) {
  Sint16 wD0;

  if (!editmode.b.h) {
    wD0 = actwk[0].yposi.w.h - pActwk->yposi.w.h;
    if ((Uint16)actwk[0].yposi.w.h < (Uint16)pActwk->yposi.w.h) wD0 *= -1;
    if ((Uint16)wD0 < 16) {
      wD0 = actwk[0].xposi.w.h - pActwk->xposi.w.h;
      if ((Uint16)actwk[0].xposi.w.h < (Uint16)pActwk->xposi.w.h) wD0 *= -1;
      if ((Uint16)wD0 < 8) {
        colrevflag ^= 1;
        pActwk->r_no0 += 2;

        if (pActwk->actflg & 128) soundset(191);
        return;
      }
    }
  }

  if (colrevflag != pActwk->actfree[18]) {
    pActwk->actfree[19] = 255;
    pActwk->r_no0 += 2;
  }

  pActwk->actfree[18] = colrevflag;
}

static void beltsw5_move2(sprite_status* pActwk) {
  ++pActwk->actfree[16];
  if (pActwk->actfree[16] == 7) {
    pActwk->patno = 1;
  }
  else {

    if (pActwk->actfree[16] == 14) {
      pActwk->actfree[16] = 0;
      pActwk->patno = 0;
      if (colrevflag & 1) pActwk->patno = 2;

      pActwk->r_no0 += 2;
      pActwk->actfree[19] = 0;
    }
  }
}

static void beltsw5_move3(sprite_status* pActwk) {
  Sint16 wD0;

  wD0 = actwk[0].yposi.w.h - pActwk->yposi.w.h;
  if ((Uint16)actwk[0].yposi.w.h < (Uint16)pActwk->yposi.w.h) wD0 *= -1;
  if ((Uint16)wD0 < 32) {
    wD0 = actwk[0].xposi.w.h - pActwk->xposi.w.h;
    if ((Uint16)actwk[0].xposi.w.h < (Uint16)pActwk->xposi.w.h) wD0 *= -1;
    if ((Uint16)wD0 < 16) return;
  }

pActwk->r_no0 = 2;
  pActwk->actfree[18] = colrevflag;
}
