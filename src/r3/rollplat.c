#include "../equ.h"
#include "rollplat.h"
#include "../action.h"
#include "../actset.h"
#include "../playsub.h"

static void act_init(sprite_status* banwk);
static void act_move(sprite_status* banwk);
static void act_check(sprite_status* banwk);

static Uint8 pchg00[6] = { 3, 1, 2, 3, 0, 255 };
static Uint8 pchg01[6] = { 3, 3, 2, 1, 0, 255 };
Uint8* pchg[2] = { pchg00, pchg01 };
static sprite_pattern kpat00 = { 1, { { -16, -8, 0, 316 } } };
static sprite_pattern kpat01 = { 1, { { -16, -16, 0, 317 } } };
static sprite_pattern kpat02 = { 1, { { -16, -16, 0, 318 } } };
static sprite_pattern kpat03 = { 1, { { -16, -16, 0, 319 } } };
sprite_pattern* kaitenban_pat[4] = { &kpat00, &kpat01, &kpat02, &kpat03 };

void kaitenban(sprite_status* banwk) {
  void(*act_tbl[2])(sprite_status*) = { &act_init, &act_move };

  act_tbl[banwk->r_no0 / 2](banwk);
  actionsub(banwk);
  frameout_s(banwk);
}

static void act_init(sprite_status* banwk) {
  sprite_status* new_actwk;

  banwk->r_no0 += 2;
  banwk->actflg |= 4;
  banwk->sprpri = 3;
  banwk->sprhsize = 16;
  banwk->sprvsize = 16;

  banwk->patbase = kaitenban_pat;
  ((Sint16*)banwk)[25] = -2;
  if (banwk->userflag.b.h == -1) {
    ((Sint16*)banwk)[23] = 128;
    banwk->actfree[20] = banwk->actfree[21] = 255;
  }
  if (actwkchk(&new_actwk) == 0) {
    ((Uint16*)banwk)[24] = new_actwk - actwk;
    ((Uint16*)new_actwk)[29] = banwk - actwk;
    new_actwk->actno = banwk->actno;
    new_actwk->userflag.b.h = 1;
    new_actwk->xposi.w.h = 896;
    new_actwk->yposi.w.h = 1272;
  }
  act_move(banwk);
}

static void act_move(sprite_status* banwk) {
  act_check(banwk);

  if (((Sint16*)banwk)[23] != 0) {
    --((Sint16*)banwk)[23];
    patchg(banwk, pchg);
  }
}

static void act_check(sprite_status* banwk) {
  Sint16 tbl[3] = { 1176, 1200, 1224 };
  Sint16 cal_d0;
  sprite_status* new_actwk;

  if ((Uint16)(banwk->yposi.w.h - actwk[0].yposi.w.h + 16) >= 32)
  { banwk->actfree[20] = 0; return; }
  if ((Uint16)(banwk->xposi.w.h - actwk[0].xposi.w.h + 16) >= 32)
  { banwk->actfree[20] = 0; return; }
  if (banwk->actfree[20] != 0) return;
  banwk->actfree[20] = 255;
  if (actwk[0].yspeed.w >= 0) banwk->mstno.w = 255;
  else banwk->mstno.w = 511;

  cal_d0 = ((Sint16*)banwk)[23];
  ((Sint16*)banwk)[23] = 128;
  if (cal_d0 != 0) return;

if (((Sint16*)banwk)[25] != 4) {
    ((Sint16*)banwk)[25] += 2;
    ((Sint16*)banwk)[28] = ((Sint16*)banwk)[27];

    ((Sint16*)banwk)[27] = ((Sint16*)banwk)[26];

    if (actwkchk(&new_actwk) == 0) {
      ((Uint16*)banwk)[26] = new_actwk - actwk;
      ((Uint16*)new_actwk)[29] = banwk - actwk;
      new_actwk->actno = banwk->actno;
      new_actwk->xposi.w.h = 896;
      new_actwk->yposi.w.h = tbl[((Sint16*)banwk)[25] / 2];
    }

    if (((Sint16*)banwk)[25] == 4)
      actwk[((Uint16*)banwk)[24]].r_no0 = 4;
    return;
  }

  ((Sint16*)banwk)[25] = -2;
  actwk[((Uint16*)banwk)[26]].r_no0 = 4;
  actwk[((Uint16*)banwk)[27]].r_no0 = 4;
  actwk[((Uint16*)banwk)[28]].r_no0 = 4;
  actwk[((Uint16*)banwk)[24]].r_no0 = 6;
}
