#include "../../types.h"
#include "lpl_types.h"
#include "spm_equ.h"
#include "sps_equ.h"
#include "pls.h"

void(*player_acttbl[3])(void) =
{
  &play0,
  &play1,
  &play2
};
static Uint8 keyBuf[256];
extern pad_status*(*sPeriPadGet)(Uint32);

void player(void) {
  player_acttbl[playwk.ACT_NO]();
}

void play0(void) {
  playwk.x_posi.w.h = -40;
  playwk.y_posi.w.h = 24;
  playwk.z_posi.w.h = 64;
  playwk.k_kaku = 0;

  zoomwk.xscale = 2048;
  zoomwk.yscale = 1024;
  playwk.k_speed.w.h = 0;

  rotflg = 1;
  zoomflg = 1;
  rotspeed.w.h = 2;
  ++playwk.ACT_NO;
}

void play1(void) {
  Sint16 iD0;
  Sint16 iD5;
  Sint16 iD6;
  pad_status* padStatus;

padStatus = sPeriPadGet(1);
  if (padStatus->On & 2)
    keyBuf[67] = 1;
  else
    keyBuf[67] = 0;
  if (padStatus->On & 4)
    keyBuf[88] = 1;
  else
    keyBuf[88] = 0;

if (swdata1.b.h & 16)
  {
    comflag_s |= 64;
    playwk.ACT_NO = 0;
    return;
  }

  if (comflag_m & 16)
    playwk.ACT_NO = 2;

check_Bbtn();

  check_Cbtn();

  iD5 = playwk.z_posi.w.h;
  iD5 >>= 6;
  iD6 = -iD5;

  if (swdata1.b.h & 4)
  {
    playwk.x_posi.w.h += 8;
    iD5 += playwk.x_posi.w.h;
  }

  if (swdata1.b.h & 8)
  {
    playwk.x_posi.w.h -= 8;
    iD6 += playwk.x_posi.w.h;
  }

  if (swdata1.b.h & 1)
  {
    playwk.y_posi.w.h += 8;
    iD5 += playwk.y_posi.w.h;
  }

  if (swdata1.b.h & 2)
  {
    playwk.y_posi.w.h -= 8;
    iD6 += playwk.y_posi.w.h;
  }

iD0 = playwk.z_posi.w.h + 336;
  if (iD0 <= playwk.x_posi.w.h)
    playwk.x_posi.w.h = iD0;

  iD0 = -playwk.z_posi.w.h;
  iD0 -= 336;
  if (iD0 >= playwk.x_posi.w.h)
    playwk.x_posi.w.h = iD0;

  iD0 = playwk.z_posi.w.h + 336;
  if (iD0 <= playwk.y_posi.w.h)
    playwk.y_posi.w.h = iD0;

  iD0 = -playwk.z_posi.w.h - 336;
  if (iD0 >= playwk.y_posi.w.h)
    playwk.y_posi.w.h = iD0;

  playwk.k_kaku += rotspeed.w.h;
  playwk.k_kaku &= 511;
}

void check_Bbtn(void) {
  if (keyBuf[88] == 0)
  {
    push_flg_s &= 239;
    return;
  }

  if (rotspeed.l != 0)
  {
    if (push_flg_s & 16) goto label1;

    rotspeed.l = 0;
    rotflg = -rotflg;
    push_flg_s |= 16;
    return;
  }

if (push_flg_s & 16)
  {
    push_flg_s |= 16;
    return;
  }

label1:
  if ((Sint16)rotflg >= 0)
    rotspeed.l += 32767 + 1;
  else
    rotspeed.l -= 32768;

  push_flg_s |= 16;
}

void check_Cbtn(void) {
  if (keyBuf[67] == 0)
  {
    push_flg_s &= 223;
    return;
  }

  if (zoomflg <= 0)
  {
    if (push_flg_s & 32) goto label1;

  }
  else
  {

    if (!(push_flg_s & 32)) goto label1;
  }

zoomflg = 1;
  if (playwk.z_posi.w.h <= 1792)
    playwk.z_posi.w.h += 8;
  push_flg_s |= 32;
  return;

label1:
  zoomflg = -1;
  if (playwk.z_posi.w.h >= -112)
    playwk.z_posi.w.h -= 8;

  push_flg_s |= 32;
}

void play2(void) {
  if (swdata1.b.h & 16)
  {
    comflag_s |= 64;
    playwk.ACT_NO = 0;
  }

  if (!(comflag_m & 16))
    playwk.ACT_NO = 1;
  playwk.k_kaku += rotspeed.w.h;
  playwk.k_kaku &= 511;
}
