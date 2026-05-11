#include "../equ.h"
#include "playsp3.h"
#include "../actset.h"
#include "../etc.h"
#include "../fcol.h"
#include "../loader2.h"
#include "scr31a.h"

static Sint16 fcol3a[17] = { 15, 316, 326, 411, 430, 131, 132, 137, 138, 119, 118, 128, 127, 126, 125, 124, 130 };
static Sint16 fcol3b[3] = { 1, 325, 326 };
static Sint16 fcol3c[17] = { 15, 316, 326, 0, 0, 131, 132, 137, 138, 119, 118, 128, 127, 126, 125, 124, 130 };
static Sint16 fcol3d[17] = { 15, 316, 326, 357, 358, 131, 132, 137, 138, 119, 118, 128, 127, 126, 125, 124, 130 };
static Sint16* fcol_tbl[4] = { fcol3b, fcol3a, fcol3d, fcol3c };
static Uint8 bump_sound_off;

void playsp(void) {
  if (actwk[0].mstno.b.h != 43) boyo();
}

void boyo(void) {
  Uint8 direc_speed;

  direc_speed = (char)atan_sonic(actwk[0].xspeed.w, actwk[0].yspeed.w);

  direc_speed = direc_speed - 32 & 192;
  switch (direc_speed) {

    case 0:
      if (!bycol_sub0(actwk[0].xposi.w.h - (Sint16)actwk[0].sprhs, actwk[0].yposi.w.h + (Sint16)actwk[0].sprvsize)) {

        bycol_sub0(actwk[0].xposi.w.h + (Sint16)actwk[0].sprhs, actwk[0].yposi.w.h + (Sint16)actwk[0].sprvsize);
      }
      break;

    case 128:
      if (!bycol_sub0(actwk[0].xposi.w.h - (Sint16)actwk[0].sprhs, actwk[0].yposi.w.h - (Sint16)actwk[0].sprvsize)) {

        bycol_sub0(actwk[0].xposi.w.h + (Sint16)actwk[0].sprhs, actwk[0].yposi.w.h - (Sint16)actwk[0].sprvsize);
      }
      break;

    case 192:
      if (!bycol_sub0(actwk[0].xposi.w.h + (Sint16)actwk[0].sprhs, actwk[0].yposi.w.h - (Sint16)(actwk[0].sprvsize - 6))) {

bycol_sub0(actwk[0].xposi.w.h + (Sint16)actwk[0].sprhs, actwk[0].yposi.w.h + (Sint16)actwk[0].sprvsize);
      }
      break;

    case 64:
      if (!bycol_sub0(actwk[0].xposi.w.h - (Sint16)actwk[0].sprhs, actwk[0].yposi.w.h - (Sint16)(actwk[0].sprvsize - 6))) {

bycol_sub0(actwk[0].xposi.w.h - (Sint16)actwk[0].sprhs, actwk[0].yposi.w.h + (Sint16)actwk[0].sprvsize);
      }
      break;
  }

}

char bycol_sub0(Sint16 cal_x, Sint16 cal_y) {
  void(*jump_no[16])(Sint16, Sint16, Sint16, Sint16) = {
    &bob, &bob, &bob_s, &bob_s, &bob5, &bob0, &bob1, &bob1,
    &bob4, &bob3, &bob3, &bob3, &bob2, &bob0, &bob0, &bob_s0
  };
  Uint8 now_time;
  Sint16 block_no, block_back, *fcol3, i;

  block_no = block_back = scramapad(&actwk[0], cal_x, cal_y);
  if ((block_no &= 2047) == 0) { bump_sound_off = 1; return 0; }
  if ((now_time = time_flag & 127) == 2)
    now_time = now_time + generate_flag;
  fcol3 = fcol_tbl[now_time];
  for (i = 0; i <= *fcol3; ++i) if (block_no == fcol3[i + 1]) goto label1;
  bump_sound_off = 1;
  return 0;

label1:
  if (actwk[0].r_no0 == 4) {
    actwk[0].r_no0 -= 2;
    ((Sint16*)&actwk[0])[26] = 120;
  }
  col_y.b.h = 0;
  jump_no[i](i, cal_x, cal_y, block_back);
  if (i >= 2) {
    if (bump_sound_off) {
      if (i == 2 || i == 3) soundset(190);
      else soundset(181);
    }
    bump_sound_off = 0;
  }
  if (col_y.b.h != 0) return 0;
  return 1;
}

void bob(Sint16 i, Sint16 cal_x, Sint16 cal_y, Sint16 block_back) {
  Sint16 tmp_sin, tmp_cos, tmp_x, tmp_y;
  Sint32 cal_sin, cal_cos;
  sprite_status* new_actwk;

  cal_y = (Uint16)cal_y & 65520;
  if (i == 0) cal_y += 16;
  cal_x = (Uint16)cal_x & 65520;
  if (!(block_back & 2048)) cal_x += 16;
  tmp_x = cal_x, tmp_y = cal_y;
  sinset((char)atan_sonic(cal_x - actwk[0].xposi.w.h, cal_y - actwk[0].yposi.w.h), &tmp_sin, &tmp_cos);

  cal_sin = tmp_sin * -1792 >> 8;
  cal_cos = tmp_cos * -1792 >> 8;
  actwk[0].xspeed.w = cal_cos;
  actwk[0].yspeed.w = cal_sin;
  actwk[0].cddat |= 2;
  actwk[0].cddat &= 207;
  actwk[0].actfree[18] = 0;
  if (actwkchk(&new_actwk) == 0) {
    new_actwk->actno = 41;
    new_actwk->xposi.w.h = tmp_x;
    new_actwk->yposi.w.h = tmp_y;
  }

  block_wrt(0, tmp_x, tmp_y);
  block_wrt(0, tmp_x -= 16, tmp_y);
  block_wrt(0, tmp_x, tmp_y -= 16);
  block_wrt(0, tmp_x += 16, tmp_y);
  sub_sync(146);
}

void bob_s(Sint16 i, Sint16 cal_x, Sint16 cal_y, Sint16 block_back) {
  Sint16 tmp_sin, tmp_cos;
  Sint32 cal_sin, cal_cos;

  cal_y = (Uint16)cal_y & 65528;
  cal_x = (Uint16)cal_x & 65528;
  sinset((char)atan_sonic(cal_x - actwk[0].xposi.w.h, cal_y - actwk[0].yposi.w.h), &tmp_sin, &tmp_cos);

  cal_sin = tmp_sin * -1792 >> 9;
  cal_cos = tmp_cos * -1792 >> 9;
  actwk[0].xspeed.w = cal_cos;
  actwk[0].yspeed.w = cal_sin;
  bob_j();
  block_back = block_back, i = i;
}

void bob_j(void) {
  actwk[0].cddat |= 2;
  actwk[0].cddat &= 207;
  actwk[0].actfree[18] = 0;
}

void bob0(Sint16 i, Sint16 cal_x, Sint16 cal_y, Sint16 block_back) {
  if (actwk[0].yspeed.w >= 0) actwk[0].yspeed.w = -1792;
  else actwk[0].yspeed.w = 1792;
  bob_j();
  i = i, cal_x = cal_x, cal_y = cal_y, block_back = block_back;
}

void bob1(Sint16 i, Sint16 cal_x, Sint16 cal_y, Sint16 block_back) {
  if (actwk[0].xspeed.w >= 0) actwk[0].xspeed.w = -1792;
  else actwk[0].xspeed.w = 1792;
  bob_j();
  i = i, cal_x = cal_x, cal_y = cal_y, block_back = block_back;
}

void bob_s0(Sint16 i, Sint16 cal_x, Sint16 cal_y, Sint16 block_back) {
  Sint16 tmp_sin, tmp_cos;
  Sint32 cal_sin, cal_cos;

  cal_y = (Uint16)cal_y & 65528;
  cal_x = (Uint16)cal_x & 65528;
  sinset((char)atan_sonic(cal_x - actwk[0].xposi.w.h, cal_y - actwk[0].yposi.w.h), &tmp_sin, &tmp_cos);

  cal_sin = tmp_sin * -1792 >> 8;
  cal_cos = tmp_cos * -1792 >> 8;
  actwk[0].xspeed.w = cal_cos;
  actwk[0].yspeed.w = cal_sin;
  bob_j();
  block_back = block_back, i = i;
}

void bob2(Sint16 i, Sint16 cal_x, Sint16 cal_y, Sint16 block_back) {
  if (((Uint16)cal_x & 15) < 8) {
    if (block_back & 2048) bob0(i, cal_x, cal_y, block_back);
    else bob1(i, cal_x, cal_y, block_back);
  } else {
    if (block_back & 2048) bob1(i, cal_x, cal_y, block_back);
    else bob0(i, cal_x, cal_y, block_back);
  }
}

void bob3(Sint16 i, Sint16 cal_x, Sint16 cal_y, Sint16 block_back) {
  static Uint8 bydcol0[16] = {
    1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6
  };
  static Uint8 bydcol1[16] = {
    6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11
  };
  static Uint8 bydcol2[16] = {
    11, 12, 12, 12, 13, 13, 13, 14, 14, 14, 15, 15, 15, 16, 16, 16
  };
  Uint8* tbl[3] = { bydcol0, bydcol1, bydcol2 };
  Uint8* bydcol;
  Uint8 cal_direc;
  Sint16 tmp_sin, tmp_cos;
  Sint32 cal_sin, cal_cos;

  if ((i -= 9) >= 0) {
    bydcol = tbl[i];
    cal_y = (Uint16)cal_y & 15;
    cal_x = (Uint16)cal_x & 15;
    if (!(block_back & 2048)) cal_x = 15 - cal_x;
    if (cal_y < (Sint16)bydcol[cal_x]) { col_y.b.h = 1; return; }
  }

  cal_direc = atan_sonic(actwk[0].xspeed.w, actwk[0].yspeed.w);
  cal_direc = -(char)(cal_direc + 128);
  cal_direc -= 32;
  if (block_back & 2048) cal_direc += 64;
  sinset(cal_direc, &tmp_sin, &tmp_cos);
  cal_sin = tmp_sin * -1792 >> 8;
  cal_cos = tmp_cos * -1792 >> 8;
  actwk[0].xspeed.w = cal_cos;
  actwk[0].yspeed.w = cal_sin;
  bob_j();
}

void bob4(Sint16 i, Sint16 cal_x, Sint16 cal_y, Sint16 block_back) {
  if (((Uint16)cal_x & 15) < 8) {
    if (block_back & 2048) bob1(i, cal_x, cal_y, block_back);
    else bob3(i, cal_x, cal_y, block_back);
  } else {
    if (block_back & 2048) bob0(i, cal_x, cal_y, block_back);
    else bob3(i, cal_x, cal_y, block_back);
  }
}

void bob5(Sint16 i, Sint16 cal_x, Sint16 cal_y, Sint16 block_back) {
  if (((Uint16)cal_x & 15) < 8) {
    if (block_back & 2048) bob0(i, cal_x, cal_y, block_back);
    else bob_s0(i, cal_x, cal_y, block_back);
  } else {
    if (block_back & 2048) bob_s0(i, cal_x, cal_y, block_back);
    else bob0(i, cal_x, cal_y, block_back);
  }
}

void bob6(Sint16 i, Sint16 cal_x, Sint16 cal_y, Sint16 block_back) {
  if (((Uint16)cal_y & 15) < 8) {
    if (block_back & 2048) bob1(i, cal_x, cal_y, block_back);
    else bob_s0(i, cal_x, cal_y, block_back);
  } else {
    if (block_back & 2048) bob_s0(i, cal_x, cal_y, block_back);
    else bob1(i, cal_x, cal_y, block_back);
  }
}
