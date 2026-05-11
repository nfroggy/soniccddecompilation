#include "../equ.h"
#include "kemusi.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../suicide.h"

static void kemusi_init(sprite_status* pActwk);
static void kemusi_com(sprite_status* pActwk, sprite_status* pNewactwk);
static void kemusi_fall(sprite_status* pActwk);
static void kemusi_stop(sprite_status* pActwk);
static void kemusi_move(sprite_status* pActwk);
static void kemusi_move1(sprite_status* pActwk);
static void kemusi_move1_rev(sprite_status* pActwk);
static void kemusi_move1_move(sprite_status* pActwk);

static sprite_pattern kemusi_pat0 = {
  1,
  { { -8, -12, 0, 431 } }
};
static sprite_pattern kemusi_pat1 = {
  1,
  { { -8, -12, 0, 432 } }
};
static sprite_pattern kemusi_pat2 = {
  1,
  { { -8, -12, 0, 433 } }
};
static sprite_pattern kemusi_pat3 = {
  1,
  { { -8, -8, 0, 434 } }
};
sprite_pattern* pat_kemusi[4] = {
  &kemusi_pat0,
  &kemusi_pat1,
  &kemusi_pat2,
  &kemusi_pat3
};

void kemusi(sprite_status* pActwk) {
  sprite_status *pActwk1, *pActwk2, *pActwk3;

  void(*kemusi_move_tbl[5])(sprite_status*) = {
    &kemusi_init,
    &kemusi_fall,
    &kemusi_stop,
    &kemusi_move,
    &kemusi_move1
  };

  if (!pActwk->userflag.b.l) {
    if (enemy_suicide(pActwk)) return;
  }

  if (pActwk->r_no0) {
    pActwk1 = &actwk[((Sint16*)pActwk)[31]];
    pActwk2 = &actwk[((Sint16*)pActwk)[32]];
    pActwk3 = &actwk[((Sint16*)pActwk)[33]];
    if (pActwk1->actno != 34 || pActwk2->actno != 34 || pActwk3->actno != 34) {
      frameout(pActwk);
      return;
    }
  }

  kemusi_move_tbl[pActwk->r_no0 / 2](pActwk);
  actionsub(pActwk);
  if (!pActwk->userflag.b.l) {
    frameout_s00(pActwk, ((Sint16*)pActwk)[28]);
  }
}

static void kemusi_init(sprite_status* pActwk) {
  sprite_status* pNewactwk;
  sprite_status *pActwk1, *pActwk2, *pActwk3;
  Uint8 bD2, bD3, bD4;
  Sint16 wD0, wD1, wD5, wD6;
  Sint16 i;

  ((Sint16*)pActwk)[23] = 3;
  pActwk->patno = 1;
  pActwk->sprvsize = 12;
  pActwk->colino = 51;

  pNewactwk = pActwk;
  kemusi_com(pActwk, pNewactwk);
  if (!pActwk->userflag.b.h) {
    wD0 = 36;
    wD1 = 6;
  }
  else {

    wD0 = 36;
    wD1 = 12;
  }

  ((Sint16*)pActwk)[24] = wD0;
  ((Sint16*)pActwk)[26] = wD1;

  i = 16;
  wD6 = 2;
  wD5 = pActwk->xposi.w.h;
  if (!pActwk->userflag.b.h) {
    bD4 = 2;
    bD3 = 11;
    bD2 = 180;
  }
  else {

    bD4 = 3;
    bD3 = 7;
    bD2 = 51;
  }

  for ( ; wD6 >= 0; --wD6, i += 2) {
    if (actwkchk2(pActwk, &pNewactwk) != 0) {
      frameout(pActwk);
      return;
    }
    ((Sint16*)((char*)pActwk + i))[23] = pNewactwk - actwk;
    ((Sint16*)pNewactwk)[23] = wD6;
    wD5 += 12;
    pNewactwk->xposi.w.h = wD5;
    pNewactwk->patno = bD4;
    pNewactwk->sprvsize = bD3;
    pNewactwk->colino = bD2;
    pNewactwk->actno = pActwk->actno;
    pNewactwk->userflag.b.h = pActwk->userflag.b.h;
    pNewactwk->yposi.w.h = pActwk->yposi.w.h;
    ((Sint16*)pNewactwk)[24] = ((Sint16*)pActwk)[24];
    ((Sint16*)pNewactwk)[26] = ((Sint16*)pActwk)[26];
    pNewactwk->userflag.b.l = -1;
    kemusi_com(pActwk, pNewactwk);
  }
  pActwk1 = &actwk[((Sint16*)pActwk)[31]];
  pActwk2 = &actwk[((Sint16*)pActwk)[32]];
  pActwk3 = &actwk[((Sint16*)pActwk)[33]];

((Sint16*)pActwk1)[31] = pActwk - actwk;
  ((Sint16*)pActwk1)[32] = pActwk2 - actwk;
  ((Sint16*)pActwk1)[33] = pActwk3 - actwk;

((Sint16*)pActwk2)[31] = pActwk - actwk;
  ((Sint16*)pActwk2)[32] = pActwk1 - actwk;
  ((Sint16*)pActwk2)[33] = pActwk3 - actwk;

((Sint16*)pActwk3)[31] = pActwk - actwk;
  ((Sint16*)pActwk3)[32] = pActwk1 - actwk;
  ((Sint16*)pActwk3)[33] = pActwk2 - actwk;
}

static void kemusi_com(sprite_status* pActwk, sprite_status* pNewactwk) {
  pNewactwk->r_no0 = 2;
  pNewactwk->actflg |= 4;
  pNewactwk->sprpri = 3;
  pNewactwk->sprhs = 8;
  pNewactwk->sprhsize = 8;
  pNewactwk->sproffset = 9214;
  pNewactwk->patbase = pat_kemusi;
  ((Sint16*)pNewactwk)[28] = pActwk->xposi.w.h;
}

static void kemusi_fall(sprite_status* pActwk) {
  Sint16 wD1;
  sprite_status* pSubactwk;

  ++pActwk->yposi.w.h;
  if ((wD1 = emycol_d(pActwk)) < 0) {
    pActwk->yposi.w.h += wD1;
    pActwk->r_no0 += 2;
    pSubactwk = pActwk;
    if (pActwk->userflag.b.l) {
      pSubactwk = &actwk[((Sint16*)pActwk)[31]];
    }
    ++((Sint16*)pSubactwk)[29];
  }

}

static void kemusi_stop(sprite_status* pActwk) {
  if (!pActwk->userflag.b.l) {
    if (((Sint16*)pActwk)[29] == 4) {
      pActwk->r_no0 += 2;
      actwk[((Sint16*)pActwk)[31]].r_no0 += 2;
      actwk[((Sint16*)pActwk)[32]].r_no0 += 2;
      actwk[((Sint16*)pActwk)[33]].r_no0 += 2;
      kemusi_move(pActwk);
    }
  }

}

static void kemusi_move(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  ((Sint16*)pActwk)[23] ^= 3;
  ((Sint16*)pActwk)[25] = ((Sint16*)pActwk)[24];
  ((Sint16*)pActwk)[27] = ((Sint16*)pActwk)[26];
  if (!pActwk->userflag.b.l) {
    pActwk->patno ^= 1;
  }
  kemusi_move1(pActwk);
}

static void kemusi_move1(sprite_status* pActwk) {
  Sint16 wD0;

  if (!(((Sint16*)pActwk)[27] -= ((Sint16*)pActwk)[23])) {
    ((Sint16*)pActwk)[27] = ((Sint16*)pActwk)[26];
    kemusi_move1_move(pActwk);
    wD0 = ((Sint16*)pActwk)[28] - pActwk->xposi.w.h;
    if (wD0 < 0)
      wD0 *= -1;
    if (wD0 >= 80) {
      kemusi_move1_rev(pActwk);
      return;
    }
    wD0 = emycol_d(pActwk);

    if (wD0 + 7 >= 14) {
      kemusi_move1_rev(pActwk);
      return;
    }
    pActwk->yposi.w.h += wD0;
  }

  if (!(--((Sint16*)pActwk)[25])) {
    pActwk->r_no0 -= 2;
  }
}

static void kemusi_move1_rev(sprite_status* pActwk) {
  pActwk->actflg ^= 1;
  pActwk->cddat ^= 1;
  kemusi_move1_move(pActwk);
  kemusi_move1(pActwk);
}

static void kemusi_move1_move(sprite_status* pActwk) {
  if (!(pActwk->actflg & 1))
    --pActwk->xposi.w.h;
  else
    ++pActwk->xposi.w.h;
}
