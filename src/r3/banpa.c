#include "../equ.h"
#include "banpa.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"

static void act_init(sprite_status* bumperwk);
static void act_move(sprite_status* bumperwk);

static sprite_pattern bmp00 = {
  1,
  { { -32, -16, 0, 315 } }
};
sprite_pattern* banpa_pat[1] = { &bmp00 };

void banpa(sprite_status* bumperwk) {
  void(*act_tbl[2])(sprite_status*) = { &act_init, &act_move };

  act_tbl[bumperwk->r_no0 / 2](bumperwk);
  actionsub(bumperwk);
  frameout_s00(bumperwk, ((Sint16*)bumperwk)[33]);
}

static void act_init(sprite_status* bumperwk) {
  ((Sint16*)bumperwk)[33] = bumperwk->xposi.w.h;

  bumperwk->r_no0 += 2;
  bumperwk->patbase = banpa_pat;

  bumperwk->actflg = 4;
  bumperwk->sprhsize = 32;
  bumperwk->sprvsize = 16;
  bumperwk->sprpri = 1;
  bumperwk->colino = 231;

  ((Sint16*)bumperwk)[26] = 192;
  ((Sint16*)bumperwk)[25] = 96;
  if (!((Uint8)bumperwk->userflag.b.h & 128)) {
    ((Sint16*)bumperwk)[26] = 160;
    ((Sint16*)bumperwk)[25] = 80;
  }
  if (bumperwk->userflag.b.h & 64)
    *(Sint32*)&bumperwk->actfree[0] = 65536;
  else
    *(Sint32*)&bumperwk->actfree[0] = -65536;
}

static void act_move(sprite_status* bumperwk) {
  if (bumperwk->colicnt != 0) {
    if ((char)bumperwk->actflg < 0) soundset(181);

    bumperwk->colicnt = 0;

if ((Uint16)(actwk[0].yposi.w.h - bumperwk->yposi.w.h + 8) < 16) {
      actwk[0].xspeed.w = 0;
    }
    else {

if ((Uint16)(actwk[0].xposi.w.h - bumperwk->xposi.w.h + 24) < 48) {
        if (actwk[0].yposi.w.h >= bumperwk->yposi.w.h)
          actwk[0].yspeed.w = 1792;
        else
          actwk[0].yspeed.w = -1792;
      }
      else {

        if (actwk[0].xposi.w.h >= bumperwk->xposi.w.h) actwk[0].xspeed.w = 1267;
        else actwk[0].xspeed.w = -1267;
        if (actwk[0].yposi.w.h >= bumperwk->yposi.w.h) actwk[0].yspeed.w = 1267;
        else actwk[0].yspeed.w = -1267;
      }

      actwk[0].cddat |= 2;
      actwk[0].cddat &= 207;
    }
  }
  if (bumperwk->userflag.b.h < 0)
    bumperwk->xposi.l += *(Sint32*)&bumperwk->actfree[0];
  else
    bumperwk->yposi.l += *(Sint32*)&bumperwk->actfree[0];

  if (--((Uint16*)bumperwk)[25] == 0) {
    ((Uint16*)bumperwk)[25] = ((Uint16*)bumperwk)[26];

    *(Sint32*)&bumperwk->actfree[0] *= -1;
  }
}
