#include "../equ.h"
#include "hari73.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../ridechk.h"
#include "boss_7.h"
#include "coli7.h"

static void hari73_init(sprite_status* pActwk);
static void hari73_move(sprite_status* pActwk);
static void msnc_hit(sprite_status* pActwk);
static void hari73_tobi(sprite_status* pActwk);
static Sint16 act_search(sprite_status** ppActwk, Uint8 bD0);

static sprite_pattern hari73_pat0 = {
  1,
  { { -16, -16, 0, 419 } }
};
static sprite_pattern hari73_pat1 = {
  1,
  { { -4, -16, 0, 420 } }
};
sprite_pattern* hari73_pat[2] = {
  &hari73_pat0,
  &hari73_pat1
};

void hari73(sprite_status* pActwk) {
  void(*hari73_acttbl[3])(sprite_status*) = {
    &hari73_init,
    &hari73_move,
    &hari73_tobi
  };

  hari73_acttbl[pActwk->r_no0 / 2](pActwk);
}

static void hari73_init(sprite_status* pActwk) {
  sprite_status* pNewactwk;

  pActwk->r_no0 += 2;
  pActwk->actflg |= 4;
  pActwk->sprpri = 3;
  pActwk->sproffset = 1276;
  pActwk->patbase = hari73_pat;
  pActwk->sprhs = 16;
  pActwk->sprhsize = 16;
  pActwk->sprvsize = 12;
  if (!act_search(&pNewactwk, 47)) {
    ((Sint16*)pActwk)[23] = pNewactwk - actwk;

if (pActwk->xposi.w.h - (Sint16)pActwk->sprhsize - pNewactwk->xposi.w.h - (Sint16)pNewactwk->sprhsize >= 0) {
      hari73_move(pActwk);
      return;
    }
  }

frameout(pActwk);
}

static void hari73_move(sprite_status* pActwk) {
  Sint16 wD0;
  sprite_status* pNewactwk;

  if (((Sint16*)pActwk)[23]) {
    pNewactwk = &actwk[((Sint16*)pActwk)[23]];
    wD0 = pActwk->xposi.w.h - pNewactwk->xposi.w.h;
    if (pActwk->xposi.w.h < pNewactwk->xposi.w.h) {
      wD0 *= -1;
    }
    if ((Sint16)pActwk->sprhsize > wD0) {
      msnc_hit(pActwk);
      return;
    }
  }

  if (hitchk(pActwk, &actwk[0])) {
    actwk[0].colino = 0;
    pActwk->actfree[11] = 0;
    if (actwk[0].cddat & 8) {
      pActwk->actfree[11] = 1;

      if (!plpower_m) {
        if (actwk[0].r_no0 != 4) {
          if (!actwk[0].actfree[6]) {
            actwk[0].yposi.l -= actwk[0].yspeed.w << 8;
            playdamageset(&actwk[0], pActwk);
            ride_on_clr(pActwk, &actwk[0]);
          }
        }
      }
    }
  }

  actionsub(pActwk);
  frameout_s(pActwk);
}

static void msnc_hit(sprite_status* pActwk) {
  Sint32 i;
  Sint16 wD4;
  sprite_status* pNewactwk;

  Sint16 spd_tbl[16] = {
    576, -512, 6, 16,
    544, -512, 0, 18,
    512, -512, 6, 20,
    480, -512, 10, 16
  };

soundset(163);

  wD4 = -12;

  for (i = 0; i < 16; i += 4) {
    if (actwkchk(&pNewactwk) != 0) break;
    pNewactwk->actno = 50;
    pNewactwk->r_no0 = 4;
    pNewactwk->patno = 1;

    pNewactwk->actflg = 4;
    pNewactwk->sprpri = 5;
    pNewactwk->sprhsize = 4;
    pNewactwk->sprvsize = 16;
    pNewactwk->sproffset = 1276;
    pNewactwk->patbase = hari73_pat;

    pNewactwk->xposi.w.h = pActwk->xposi.w.h + wD4;
    pNewactwk->yposi.w.h = pActwk->yposi.w.h;

    pNewactwk->xspeed.w = spd_tbl[i];
    pNewactwk->yspeed.w = spd_tbl[i + 1];
    ((Sint16*)pNewactwk)[26] = spd_tbl[i + 2];
    ((Sint16*)pNewactwk)[27] = spd_tbl[i + 3];
    wD4 += 8;
  }

  if (pActwk->actfree[11]) {
    ride_on_clr(pActwk, &actwk[0]);
  }
  frameout(pActwk);
}

static void hari73_tobi(sprite_status* pActwk) {
  hari_spdadd(pActwk);

  ++pActwk->actfree[10];
  pActwk->actfree[10] &= 3;
  if (pActwk->actfree[10] & 2) {
    actionsub(pActwk);
  }
  frameout_s(pActwk);
}

static Sint16 act_search(sprite_status** ppActwk, Uint8 bD0) {
  Sint32 i;

for (i = 2; i < 128; ++i) {
    if (bD0 == actwk[i].actno) {
      *ppActwk = &actwk[i];
      return 0;
    }
  }
  return -1;
}
