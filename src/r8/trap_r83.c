#include "../equ.h"
#include "trap_r83.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../ridechk.h"
#include "coli8.h"

static void tr8_fout(sprite_status* trapwk);
static void togeita_ridechk(sprite_status* trapwk); static void togeita_init(sprite_status* trapwk); static void togeita_dataset(sprite_status* trapwk, sprite_status* new_actwk, Uint8* togeita_data); static void setitano(sprite_status* trapwk); static void togeita_move1(sprite_status* trapwk); static Uint8 togeita_plchk_1(sprite_status* trapwk, Sint16 cal_x); static void togeita_move2(sprite_status* trapwk); static void itaset_chk(sprite_status* trapwk); static void togeita_move3(sprite_status* trapwk); static void togeitax(sprite_status* trapwk);
static void dair8_init(sprite_status* trapwk); static void dair8_move(sprite_status* trapwk);
static void harir8_init(sprite_status* trapwk); static void harir8_move(sprite_status* trapwk);

static void tr8_fout(sprite_status* trapwk) {
  Uint16 status;

  if ((status = trapwk->cdsts) == 0) { frameout(trapwk); return; }
  status = (Uint16)time_flag + status * 3;
  flagwork[status] &= 127;
  frameout(trapwk);
}

static Uint8 togeita_data0[12] = { 1, 204, 28, 8, 4, 188, 12, 8, 5, 220, 44, 8 };
static Uint8 togeita_data1[12] = { 3, 0, 24, 8, 3, 224, 24, 8, 3, 32, 24, 8 };
static Uint8 togeita_data2[12] = { 2, 52, 28, 8, 5, 36, 44, 8, 4, 68, 12, 8 };
static sprite_pattern tog00 = {
  6,
  {
    { -80, -12, 0, 289 },
    { -56, -12, 0, 288 },
    { -24, -12, 0, 290 },
    { 0, -12, 0, 290 },
    { 24, -12, 0, 288 },
    { 56, -12, 0, 288 }
  }
};
static sprite_pattern tog01 = {
  2,
  {
    { -28, -8, 0, 289 },
    { -4, -8, 0, 288 }
  }
};
static sprite_pattern tog02 = {
  2,
  {
    { -28, -8, 0, 288 },
    { 4, -8, 0, 289 }
  }
};
static sprite_pattern tog03 = {
  2,
  {
    { -24, -8, 0, 290 },
    { 0, -8, 0, 290 }
  }
};
static sprite_pattern tog04 = { 1, { { -12, -8, 0, 289 } } };
static sprite_pattern tog05 = {
  3,
  {
    { -44, -8, 0, 288 },
    { -12, -8, 0, 288 },
    { 20, -8, 0, 289 }
  }
};
static sprite_pattern tog06 = { 1, { { -16, -20, 0, 0 } } };
sprite_pattern* togeitapat[7] = {
  &tog00,
  &tog01,
  &tog02,
  &tog03,
  &tog04,
  &tog05,
  &tog06
};

void togeita(sprite_status* trapwk) {
  void(*tbl[4])(sprite_status*) = { &togeita_init, &togeita_move1, &togeita_move2, &togeita_move3 };

if (trapwk->userflag.b.h < 0) { togeitax(trapwk); return; }
  tbl[trapwk->r_no0 / 2](trapwk);
  togeita_ridechk(trapwk);
  actionsub(trapwk);
  frameout_s00(trapwk, ((Sint16*)trapwk)[29]);
}

static void togeita_ridechk(sprite_status* trapwk) {
  Sint16 cal_position;

  if (trapwk->patno == 3) return;
  if (!(trapwk->actflg & 128)) return;
  if ((cal_position = actwk[0].yposi.w.h - trapwk->yposi.w.h) < 0)
    cal_position = -cal_position;
  if (cal_position < 32)
    ride_on_chk(trapwk, &actwk[0]);
}

static void togeita_init(sprite_status* trapwk) {
  Uint8 patchg_point;
  sprite_status* new_actwk;

  trapwk->r_no0 += 2;
  trapwk->actflg |= 4;
  trapwk->sprpri = 3;

  trapwk->patbase = togeitapat;

  setitano(trapwk);

  if (trapwk->actfree[19] == 0) {

    patchg_point = trapwk->actfree[18] * 4;
    trapwk->patno = togeita_data0[patchg_point];
    trapwk->sprhsize = togeita_data0[patchg_point + 2];
    trapwk->sprvsize = togeita_data0[patchg_point + 3];
    ((Sint16*)trapwk)[29] = trapwk->xposi.w.h;
    ((Sint16*)trapwk)[27] = trapwk->yposi.w.h;
    trapwk->xposi.w.h += (char)togeita_data0[patchg_point + 1];
    trapwk->actfree[17] = 1;

    if (actwkchk(&new_actwk) != 0) { tr8_fout(trapwk); return; }
    new_actwk->actno = 51;
    new_actwk->actfree[19] = 1;
    togeita_dataset(trapwk, new_actwk, togeita_data1);

    if (actwkchk(&new_actwk) != 0) { tr8_fout(trapwk); return; }
    new_actwk->actno = 51;
    new_actwk->actfree[19] = 2;
    togeita_dataset(trapwk, new_actwk, togeita_data2);
  }

  if (trapwk->patno == 3) return;
  if (actwkchk(&new_actwk) != 0) return;
  new_actwk->actno = 51;
  new_actwk->userflag.b.h = -1;
  new_actwk->xposi.w.h = trapwk->xposi.w.h;
  new_actwk->actfree[18] = trapwk->patno;
  ((Uint16*)new_actwk)[28] = trapwk - actwk;
  ((Uint16*)trapwk)[28] = new_actwk - actwk;
}

static void togeita_dataset(sprite_status* trapwk, sprite_status* new_actwk, Uint8* togeita_data) {
  Uint8 patchg_point;

  patchg_point = trapwk->actfree[18] * 4;
  new_actwk->patno = togeita_data[patchg_point];
  new_actwk->sprhsize = togeita_data[patchg_point + 2];
  new_actwk->sprvsize = togeita_data[patchg_point + 3];
  ((Sint16*)new_actwk)[29] = ((Sint16*)trapwk)[29];

  new_actwk->yposi.w.h = trapwk->yposi.w.h;
  new_actwk->xposi.w.h = ((Sint16*)trapwk)[29] + (Sint16)(char)togeita_data[patchg_point + 1];

  new_actwk->userflag.b.h = trapwk->userflag.b.h;
}

static void setitano(sprite_status* trapwk) {
  Uint8 tbl[56] = {
    0, 1, 0, 2, 0, 0, 0, 0,
    1, 2, 1, 2, 1, 2, 1, 2,
    0, 2, 1, 2, 0, 1, 2, 1,
    1, 2, 1, 2, 2, 1, 2, 2,
    1, 2, 1, 1, 0, 1, 2, 0,
    1, 2, 2, 1, 0, 1, 2, 1,
    1, 0, 2, 1, 0, 1, 2, 2
};

  trapwk->actfree[18] = tbl[trapwk->userflag.b.h];
}

static void togeita_move1(sprite_status* trapwk) {
  Sint16 position_check, cal_x;

  if ((position_check = actwk[0].yposi.w.h - trapwk->yposi.w.h) < 0) return;
  if (position_check >= 256) return;

  if (trapwk->userflag.b.h == 0) cal_x = 48;
  else cal_x = 80;
  if (togeita_plchk_1(trapwk, cal_x)) trapwk->r_no0 += 2;
}

static Uint8 togeita_plchk_1(sprite_status* trapwk, Sint16 cal_x) {
  Sint16 position_check;

  if ((position_check = actwk[0].xposi.w.h - ((Sint16*)trapwk)[29] + cal_x) < 0)
    return 0;
  if (position_check >= cal_x * 2) return 0;
  return 1;
}

static void togeita_move2(sprite_status* trapwk) {
  Sint16 col_data;
  Sint32 stack_yposi;
  Sint16 tbl[6] = { 8, 32, 56, 80, 104, 128 };

if (trapwk->userflag.b.h == 0 && trapwk->actfree[19] == 0)
    itaset_chk(trapwk);
  if (trapwk->actfree[21] != 0) return;
  stack_yposi = trapwk->yposi.l;
  trapwk->yposi.w.h += tbl[trapwk->userflag.b.h];
  col_data = emycol_d(trapwk);
  trapwk->yposi.l = stack_yposi;
  if (col_data >= 0) {
    trapwk->yposi.l += 32768;
    trapwk->yspeed.w = 128;
  } else {
    trapwk->yspeed.w = 0;
    trapwk->actfree[21] = 255;
  }
}

static void itaset_chk(sprite_status* trapwk) {
  Sint16 cal_position;
  sprite_status* new_actwk;

  if ((cal_position = actwk[0].yposi.w.h - trapwk->yposi.w.h) < 0) {
    cal_position = -cal_position;
    if (cal_position >= 128) return;
  }
  if (togeita_plchk_1(trapwk, 160) == 0) return;
  switch (++((Sint16*)trapwk)[26]) {

    case 300:

    case 600:

    case 840:

    case 1080:
     break;

    default:
      return;
  }

  if (trapwk->actfree[17] < 5) {
    if (actwkchk(&new_actwk) != 0) return;

    new_actwk->actno = 51;
    new_actwk->xposi.w.h = ((Sint16*)trapwk)[29];
    new_actwk->yposi.w.h = ((Sint16*)trapwk)[27] - (Sint16)((char)trapwk->actfree[17] * 16);

    new_actwk->userflag.b.h = trapwk->actfree[17];
    ++trapwk->actfree[17];
  }
}

static void togeita_move3(sprite_status* trapwk) {
  trapwk->actno = trapwk->actno;
}

static void togeitax(sprite_status* trapwk) {
  Uint16 ride_number;
  Uint8 colitbl[6] = {
    0,
    156,
    156,
    0,
    155,
    157
  };
  trapwk->actflg |= 4;
  trapwk->patbase = togeitapat;

  trapwk->patno = 6;

ride_number = ((Uint16*)trapwk)[28];
  if (actwk[ride_number].actno != 51)
  { frameout(trapwk); return; }
  trapwk->yspeed.w = actwk[ride_number].yspeed.w;
  trapwk->yposi.w.h = actwk[ride_number].yposi.w.h + 12;

  trapwk->colino = colitbl[trapwk->actfree[18]];
  actionsub(trapwk);
}

static sprite_pattern dai00 = { 1, { { -16, -16, 0, 291 } } };
sprite_pattern* togedair8pat[1] = { &dai00 };

void togedair8(sprite_status* trapwk) {
  void(*tbl[2])(sprite_status*) = { &dair8_init, &dair8_move };

  if (trapwk->userflag.b.h < 0) { harir8(trapwk); return; }
  tbl[trapwk->r_no0 / 2](trapwk);
  actionsub(trapwk);
  frameout_s00(trapwk, ((Sint16*)trapwk)[29]);
}

static void dair8_init(sprite_status* trapwk) {
  sprite_status* new_actwk;

  trapwk->actflg |= 4;

  trapwk->sprpri = 3;
  trapwk->patbase = togedair8pat;
  ((Sint16*)trapwk)[29] = trapwk->xposi.w.h;
  ((Sint16*)trapwk)[27] = trapwk->yposi.w.h;
  trapwk->sprhsize = trapwk->sprvsize = 16;
  if (actwkchk(&new_actwk) != 0) { dair8_move(trapwk); return; }

  trapwk->r_no0 += 2;
  if (trapwk->userflag.b.h != 1) {
    new_actwk->actno = 10;
    new_actwk->xposi.w.h = trapwk->xposi.w.h;
    new_actwk->yposi.w.h = trapwk->yposi.w.h;
    new_actwk->actfree[15] = 232;
    ((Uint16*)new_actwk)[28] = trapwk - actwk;
    new_actwk->userflag.b.h = trapwk->userflag.b.h;
    dair8_move(trapwk);
  } else {
    new_actwk->actno = 54;
    new_actwk->userflag.b.h = -1;
    new_actwk->xposi.w.h = trapwk->xposi.w.h;
    new_actwk->yposi.w.h = trapwk->yposi.w.h;
    new_actwk->actfree[15] = 232;
    ((Uint16*)new_actwk)[28] = trapwk - actwk;
    dair8_move(trapwk);
  }
}

static void dair8_move(sprite_status* trapwk) {
  Sint16 cal_position;

  if (!(trapwk->actflg & 128)) return;

  if ((cal_position = actwk[0].yposi.w.h - trapwk->yposi.w.h) < 0)
    cal_position = -cal_position;
  if (cal_position >= 32) return;

hitchk(trapwk, &actwk[0]);
}

static sprite_pattern har00 = { 1, { { -16, -16, 0, 292 } } };
static sprite_pattern har01 = { 1, { { -16, -16, 16, 292 } } };
sprite_pattern* harir8pat[2] = { &har00, &har01 };

void harir8(sprite_status* trapwk) {
  Uint16 master;
  void(*tbl[2])(sprite_status*) = { &harir8_init, &harir8_move };

  tbl[trapwk->r_no0 / 2](trapwk);
  actionsub(trapwk);
  if ((master = ((Uint16*)trapwk)[28]) != 0) {

    if (actwk[master].actno == 0) { frameout(trapwk); return; }
    trapwk->xposi.w.h = actwk[master].xposi.w.h + (Sint16)(char)trapwk->actfree[14];

    trapwk->yposi.w.h = actwk[master].yposi.w.h + (Sint16)(char)trapwk->actfree[15];
  }

frameout_s00(trapwk, ((Sint16*)trapwk)[29]);
}

static void harir8_init(sprite_status* trapwk) {
  trapwk->r_no0 += 2;
  trapwk->actflg |= 4;

  trapwk->sprpri = 3;
  trapwk->patbase = harir8pat;
  ((Sint16*)trapwk)[29] = trapwk->xposi.w.h;
  ((Sint16*)trapwk)[27] = trapwk->yposi.w.h;

  trapwk->sprhsize = 16;
  trapwk->sprvsize = 8;
  if (trapwk->userflag.b.h != -1) {
    trapwk->patno = 1;
    trapwk->colino = 131;
    trapwk->sprhsize = 18;
  }
  harir8_move(trapwk);
}

static void harir8_move(sprite_status* trapwk) {
  Sint16 cal_position;

  if (trapwk->patno == 0)
  {
    if ((cal_position = actwk[0].yposi.w.h - trapwk->yposi.w.h) < 0)
      cal_position = -cal_position;
    if (cal_position >= 32) return;

    if (hitchk(trapwk, &actwk[0]) == 0) return;
    if (!(trapwk->cddat & 8)) return;
    if (plpower_a || plpower_m) return;

    if (actwk[0].r_no0 >= 4) return;
    if (((Sint16*)&actwk[0])[26] != 0) return;
    actwk[0].yposi.l -= actwk[0].yspeed.w << 8;
    playdamageset(&actwk[0], trapwk);
    return;
  }

if (!(trapwk->actflg & 128)) return;

  if ((cal_position = actwk[0].yposi.w.h - trapwk->yposi.w.h) < 0)
    cal_position = -cal_position;
  if (cal_position >= 32) return;

hitchk(trapwk, &actwk[0]);
}
