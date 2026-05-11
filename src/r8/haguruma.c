#include "../equ.h"
#include "haguruma.h"
#include "../actset.h"

static void hagurumainit(sprite_status* wheelwk);
static void hagurumamove(sprite_status* wheelwk);
static void hagurumasub(sprite_status* wheelwk);

void haguruma(sprite_status* wheelwk) {
  void(*haguruma_move_tbl[2])(sprite_status*) = { &hagurumainit, &hagurumamove };
  haguruma_move_tbl[wheelwk->r_no0 / 2](wheelwk);
}

void hagurumainit(sprite_status* wheelwk) {
  wheelwk->r_no0 += 2;
  wheelwk->actflg |= 4;
  ((Sint16*)wheelwk)[27] = wheelwk->xposi.w.h;
  ((Sint16*)wheelwk)[26] = wheelwk->yposi.w.h;
  wheelwk->actfree[10] = 24;
  wheelwk->actfree[14] = 72;
  if (wheelwk->userflag.b.h & 15) {
    wheelwk->actfree[10] = 16;
    wheelwk->actfree[14] = 48;
  }
  ((Sint16*)wheelwk)[29] = (Sint16)(wheelwk->userflag.b.h & -16) * 8;

  wheelwk->direc.b.h = wheelwk->cddat << 6 & 192;
  hagurumamove(wheelwk);
}

void hagurumamove(sprite_status* wheelwk) {
  hagurumasub(wheelwk);
  frameout_s(wheelwk);
}

void hagurumasub(sprite_status* wheelwk) {
  Uint16 cal_posi;

  cal_posi = actwk[0].xposi.w.h - ((Sint16*)wheelwk)[27] + (Sint16)wheelwk->actfree[14];

  if (cal_posi < (Uint16)(wheelwk->actfree[14] * 2)) {
    cal_posi = actwk[0].yposi.w.h - ((Sint16*)wheelwk)[26] + (Sint16)wheelwk->actfree[14];

    if (cal_posi < (Uint16)(wheelwk->actfree[14] * 2)) {
      if (!(actwk[0].cddat & 2)) goto label1;

      wheelwk->actfree[16] = 0;
      return;
    }
  }
  if (wheelwk->actfree[16])
    actwk[0].actfree[14] = wheelwk->actfree[16] = 0;
  return;

label1:
  if (!wheelwk->actfree[16]) {
    wheelwk->actfree[16] = 1;
    if (!(actwk[0].cddat & 4)) actwk[0].mstno.b.h = 0;
    actwk[0].cddat &= 223;
    actwk[0].mstno.b.l = 1;
    actwk[0].actfree[14] = 1;
  }
  if (((Sint16*)wheelwk)[29] < 0) {
    if (actwk[0].mspeed.w > -1024) actwk[0].mspeed.w = -1024;
    else if (actwk[0].mspeed.w < -3840) actwk[0].mspeed.w = -3840;
    return;
  }
  if (actwk[0].mspeed.w < 1024) actwk[0].mspeed.w = 1024;
  else if (actwk[0].mspeed.w > 3840) actwk[0].mspeed.w = 3840;
}
