#include "../equ.h"
#include "boss_3.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../etc.h"
#include "../goal.h"
#include "../impfuncs.h"
#include "../loader2.h"
#include "../playsub.h"
#include "../score.h"

extern void colorset2(Sint32 ColorNo);

static sprite_pattern segg3_01 =
{
  1,
  { { -24, -56, 0, 571 } }
};
static sprite_pattern segg3_02 =
{
  1,
  { { -24, -56, 0, 572 } }
};
static sprite_pattern segg3_03 =
{
  1,
  { { -24, -56, 0, 573 } }
};
static sprite_pattern segg3_04 =
{
  1,
  { { -24, -56, 0, 574 } }
};
static sprite_pattern segg3_05 =
{
  1,
  { { -32, -64, 0, 575 } }
};
static sprite_pattern segg3_06 =
{
  1,
  { { -24, -64, 0, 576 } }
};
static sprite_pattern segg3_07 =
{
  1,
  { { -24, -56, 0, 577 } }
};
static sprite_pattern segg3_08 =
{
  1,
  { { -24, -56, 0, 578 } }
};
static sprite_pattern segg3_09 =
{
  1,
  { { 0, -56, 0, 579 } }
};
static sprite_pattern segg3_10 =
{
  1,
  { { 0, -56, 0, 580 } }
};
static sprite_pattern segg3_11 =
{
  1,
  { { -48, -16, 0, 581 } }
};
static sprite_pattern segg3_12 =
{
  1,
  { { -48, -16, 0, 582 } }
};
static sprite_pattern segg3_13 =
{
  1,
  { { -64, -48, 0, 583 } }
};
static sprite_pattern segg3_14 =
{
  1,
  { { -64, -48, 0, 584 } }
};
static sprite_pattern segg3_15 =
{
  1,
  { { -64, -32, 0, 585 } }
};
static sprite_pattern segg3_16 =
{
  1,
  { { -64, -32, 0, 586 } }
};
static sprite_pattern segg3_17 =
{
  1,
  { { -64, -32, 0, 587 } }
};
static sprite_pattern segg3_18 =
{
  1,
  { { -64, -32, 0, 588 } }
};
static sprite_pattern segg3_19 =
{
  1,
  { { -64, -16, 0, 589 } }
};
static sprite_pattern segg3_20 =
{
  1,
  { { -16, -16, 0, 590 } }
};
static sprite_pattern segg3_21 =
{
  1,
  { { -16, -16, 0, 591 } }
};
static sprite_pattern segg3_22 =
{
  1,
  { { -16, -16, 0, 592 } }
};
static sprite_pattern segg3_23 =
{
  2,
  {
    { -24, -16, 0, 613 },
    { -24, -16, 0, 614 }
  }
};
static sprite_pattern segg3_24 =
{
  2,
  {
    { -24, -16, 0, 613 },
    { -24, -16, 0, 615 }
  }
};
static sprite_pattern segg3_25 =
{
  2,
  {
    { -24, -16, 0, 613 },
    { -24, -24, 0, 616 }
  }
};
static sprite_pattern segg3_26 =
{
  2,
  {
    { -24, -16, 0, 613 },
    { -24, -16, 0, 617 }
  }
};
static sprite_pattern segg3_27 =
{
  2,
  {
    { -24, -16, 0, 613 },
    { -24, -24, 0, 618 }
  }
};
static sprite_pattern segg3_28 =
{
  1,
  { { -8, -8, 0, 598 } }
};
static sprite_pattern segg3_29 =
{
  1,
  { { -12, -12, 0, 599 } }
};
static sprite_pattern segg3_B1 =
{
  2,
  {
    { -64, -48, 0, 601 },
    { -48, -32, 0, 600 }
  }
};
static sprite_pattern segg3_B2 =
{
  2,
  {
    { -64, -48, 0, 603 },
    { -48, -32, 0, 602 }
  }
};
static sprite_pattern segg3_B3 =
{
  2,
  {
    { -64, -16, 0, 605 },
    { -48, -32, 0, 604 }
  }
};
static sprite_pattern segg3_B4 =
{
  2,
  {
    { -64, -16, 0, 607 },
    { -48, -32, 0, 606 }
  }
};
static sprite_pattern segg3_B5 =
{
  2,
  {
    { -64, -16, 0, 609 },
    { -48, -32, 0, 608 }
  }
};
static sprite_pattern segg3_B6 =
{
  2,
  {
    { -64, -16, 0, 611 },
    { -48, -32, 0, 610 }
  }
};
static sprite_pattern segg3_B7 =
{
  1,
  { { -64, -16, 0, 612 } }
};
sprite_pattern* egg3_pat[10] =
{
  &segg3_01,
  &segg3_02,
  &segg3_03,
  &segg3_04,
  &segg3_05,
  &segg3_06,
  &segg3_07,
  &segg3_08,
  &segg3_09,
  &segg3_10,
};
sprite_pattern* egg3dai_s_pat[2] =
{
  &segg3_11,
  &segg3_12
};
sprite_pattern* egg3dai_l_pat[7] =
{
  &segg3_B1,
  &segg3_B2,
  &segg3_B3,
  &segg3_B4,
  &segg3_B5,
  &segg3_B6,
  &segg3_B7
};
sprite_pattern* egg3haguruma_pat[3] =
{
  &segg3_20,
  &segg3_21,
  &segg3_22
};
sprite_pattern* egg3cannon_pat[5] =
{
  &segg3_23,
  &segg3_24,
  &segg3_25,
  &segg3_26,
  &segg3_27
};
sprite_pattern* egg3punchbom_pat[2] =
{
  &segg3_28,
  &segg3_29
};
static char pchg0[8] = { 9, 0, 1, 0, 3, 2, 3, -1 };
static char pchg1[4] = { 7, 4, 5, -1 };
static char pchg2[3] = { 127, 6, -1 };
static char pchg3[3] = { 127, 7, -1 };
static char pchg4[4] = { 7, 8, 9, -1 };
char* egg3_pchg[5] =
{
  pchg0,
  pchg1,
  pchg2,
  pchg3,
  pchg4
};
static char schg0[3] = { 127, 0, -1 };
static char schg1[4] = { 0, 0, 1, -1 };
char* egg3dai_s_pchg[2] =
{
  schg0,
  schg1
};
static char hchg0[5] = { 2, 0, 2, 1, -1 };
static char hchg1[5] = { 2, 0, 1, 2, -1 };
char* egg3haguruma_pchg[2] =
{
  hchg0,
  hchg1
};
char bomtbl_dai[22] =
{
    4,  10,
  -48, -16,
   48,  16,
  -16, -16,
   16,  16,
  -32,   0,
   48, -16,
  -48,  16,
  -16,  16,
   16, -16,
   32,   0
};
char bomtbl_cannon[8] =
{
    1,   3,
    0,   0,
  -32,   0,
   32,   0
};
char bomtbl_ctrl[8] =
{
    1,   3,
    0, -56,
  -32, -56,
   32, -56
};
char bomtbl_ctrl2[6] =
{
    1,   2,
  -16, -56,
   16, -56
};
extern sprite_pattern* bakupat[];
extern char* bakuchg[];

void egg3dai_s(sprite_status* actionwk) {
  switch (actionwk->r_no0)
  {
    case 0:
      egg3dai_s_ini(actionwk);
    case 2:
      egg3dai_s_chk(actionwk);
      break;
    case 4:
      egg3dai_s_up(actionwk);
      break;
    case 6:
      egg3dai_s_bom(actionwk);
      break;
  }
}

void egg3dai_s_ini(sprite_status* actionwk) {
  sprite_status* a1;

  actionwk->r_no0 = 2;
  actionwk->actflg = 4;
  actionwk->sprpri = 4;
  actionwk->sprhsize = 48;
  actionwk->sprvsize = 16;
  actionwk->sproffset = 798;
  actionwk->patbase = egg3dai_s_pat;
  ((Sint32*)actionwk)[13] = -163840;
  ((Sint16*)actionwk)[25] = actionwk->yposi.w.h - 48;
  actionwk->colino = 62;
  actionwk->colicnt = 2;

  if (actwkchk(&a1) != 0) return;
  ((Uint16*)a1)[28] = actionwk - actwk;
  a1->actno = 50;
  a1->xposi.w.h = actionwk->xposi.w.h - 48;
  a1->yposi.w.h = actionwk->yposi.w.h;
  ((Uint16*)actionwk)[28] = a1 - actwk;
}

void egg3dai_s_chk(sprite_status* actionwk) {
  if (!actionwk->colino)
  {
    soundset(172);
    actionwk->r_no0 = 4;
    actionwk->actfree[2] |= 4;
    actionwk->mstno.b.h = 1;
    actionwk->colicnt = 0;
  }
  actionsub(actionwk);
}

void egg3dai_s_up(sprite_status* actionwk) {
  jaba_s_chk(actionwk);
  ++actionwk->actfree[0];
  if (actionwk->actfree[0] >= 16)
  {
    actionwk->mstno.b.h = 0;
    patchg(actionwk, (Uint8**)egg3dai_s_pchg);
  }

  actionwk->yposi.l += ((Sint32*)actionwk)[13];
  if (actionwk->yposi.w.h <= ((Sint16*)actionwk)[25])
  {
    actionwk->actfree[2] &= 251;
    actionwk->yposi.w.h = ((Sint16*)actionwk)[25];
    actionwk->r_no0 = 6;
  }
  actionsub(actionwk);
}

void egg3dai_s_bom(sprite_status* actionwk) {
  jaba_s_chk(actionwk);
  if (((Sint16*)actionwk)[33] < 121)
  {
    bom_set_dai(actionwk);
    actionsub(actionwk);
  }

  if (actionwk->actfree[2] & 2)
  {
    frameout(&actwk[((Uint16*)actionwk)[28]]);
    frameout(actionwk);
  }
}

void jaba_s_chk(sprite_status* actionwk) {
  sprite_status* a1;

  if (actionwk->actfree[2] & 2) return;

  a1 = &actwk[0];
  if (a1->xposi.w.h < 1152 || a1->yposi.w.h >= 1308) return;
  actionwk->actfree[2] |= 2;
  sub_sync(103);

  if (actwkchk(&a1) != 0) return;
  a1->actno = 63;
  a1->r_no0 = 6;
  a1->xposi.w.h = 1056;
  a1->yposi.w.h = 1344;
}

void egg3flash(sprite_status* actionwk) {
  Sint32 d0;

  if (actionwk->actfree[1] == 0) return;

if (actionwk->actfree[1] >> 1 & 1)
  {
    d0 = 5;
    actionwk->patno |= 1;
  }
  else
  {
    d0 = 6;
    actionwk->patno &= 254;
  }

  --actionwk->actfree[1];
  colorset2(d0);
}

void egg3dai_l(sprite_status* actionwk) {
  if (actionwk->actfree[17])
  {

    if ((Uint8)scra_vline < 176)
    {
      scra_vline += 6;
      if ((Uint8)scra_vline < 178) goto label1;
    }
    scra_vline = 178;
  }

label1:
  switch (actionwk->r_no0)
  {
    case 0:
      egg3dai_l_ini(actionwk);
      break;
    case 2:
      egg3dai_l_demo(actionwk);
      break;
    case 4:
      egg3dai_l_chk(actionwk);
      break;
    case 6:
      egg3dai_l_up(actionwk);
      break;
    case 8:
      egg3dai_l_bom(actionwk);
      break;
    case 10:
      egg3dai_l_end(actionwk);
      break;
  }
  if (actionwk->actfree[2] & 1) return;
  egg3flash(actionwk);
  actionsub(actionwk);
}

void egg3dai_l_ini(sprite_status* actionwk) {
  sprite_status* a1;

  colorset2(6);
  actionwk->r_no0 = 2;
  actionwk->actflg = 4;
  actionwk->sprpri = 4;
  actionwk->sprhsize = 80;
  actionwk->sprvsize = 16;
  actionwk->sproffset = 798;
  actionwk->patbase = egg3dai_l_pat;

if (actwkchk2(actionwk, &a1) != 0) return;
  ((Uint16*)actionwk)[29] = a1 - actwk;
  ((Uint16*)a1)[28] = actionwk - actwk;
  a1->actno = 51;
  a1->xposi.w.h = actionwk->xposi.w.h;
  a1->yposi.w.h = actionwk->yposi.w.h - 48;

  if (actwkchk2(actionwk, &a1) != 0) return;
  ((Uint16*)actionwk)[28] = a1 - actwk;
  ((Uint16*)a1)[28] = actionwk - actwk;
  a1->actno = 50;
  a1->xposi.w.h = actionwk->xposi.w.h - 64;
  a1->yposi.w.h = actionwk->yposi.w.h;

  if (actwkchk2(actionwk, &a1) != 0) return;
  ((Uint16*)a1)[28] = actionwk - actwk;
  a1->actno = 61;
  a1->xposi.w.h = actionwk->xposi.w.h;
}

void egg3dai_l_demo(sprite_status* actionwk) {
  sprite_status* a1;
  Sint16 d0, d1, d2;

  a1 = &actwk[((Uint16*)actionwk)[28]];
  a1->mstno.b.h = 1;

  a1 = &actwk[0];
  d1 = 448;
  d2 = 220;
  d0 = a1->yposi.w.h;
  if (d0 >= d1) goto label2;
  if (d0 >= d2)
  {
label1:
    if (((Sint16*)actionwk)[25] == d0)
      actionwk->actfree[2] &= 251;
    else
      actionwk->actfree[2] |= 4;
    ((Sint16*)actionwk)[25] = d0;
    actionwk->yposi.w.h = d0;
    return;
  }

  d0 = d2;
  goto label1;
label2:
  actionwk->yposi.w.h = d1;
  actionwk->actfree[2] &= 251;
  actionwk->r_no0 = 4;
  ((Sint32*)actionwk)[13] = -163840;
  ((Sint16*)actionwk)[25] = actionwk->yposi.w.h - 72;
  actionwk->colino = 63;
  actionwk->colicnt = 4;

  a1 = &actwk[((Uint16*)actionwk)[28]];
  a1->mstno.b.h = 0;
}

Sint16 jabaopen_chk(sprite_status* actionwk) {
  Sint16 jabaopen_tbl[4] = { 496, 424, 352, 272 };
  Sint16 d0, d1;

  if (actionwk->actfree[16] >= 4) return 1;
  d0 = actionwk->actfree[16];

  d0 = jabaopen_tbl[d0];

  d1 = actwk[0].yposi.w.h;
  if (d0 < d1) return 1;
  if (d1 < 128) return 1;

  d1 = actwk[0].xposi.w.h;
  if (d1 < 832) return 1;
  if (d1 > 960) return 1;

  return 0;
}

void egg3dai_l_chk(sprite_status* actionwk) {
  if (jabaopen_chk(actionwk) == 0) jaba_open(actionwk);

  if (actionwk->colino == 0) egg3dai_l_hit(actionwk);
}

void jaba_open(sprite_status* actionwk) {
  sprite_status* a1;
  Sint16 jabapos_tbl[4] = { 528, 456, 384, 304 };

if (actwkchk(&a1) != 0) return;
  a1->actno = 63;
  a1->xposi.w.h = 896;
  a1->yposi.w.h = jabapos_tbl[actionwk->actfree[16]++];
}

void egg3dai_l_hit(sprite_status* actionwk) {
  switch (actionwk->actfree[17])
  {
    case 0:

    case 1:
      hitact2(actionwk);
      break;
    case 2:
      hitact3(actionwk);
  }

  ++actionwk->actfree[17];
}

void hitact2(sprite_status* actionwk) {
  sprite_status* a1;

a1 = &actwk[((Uint16*)actionwk)[29]];
  a1->r_no0 = 4;
  a1->mstno.b.h = 1;
  ((Sint32*)a1)[13] = -360448;
  egg3dai_l_hit2(actionwk);
}

void hitact3(sprite_status* actionwk) {
  sprite_status* a1;

  actionwk->patno = 4;

  a1 = &actwk[((Uint16*)actionwk)[29]];
  a1->r_no0 = 6;
  a1->actfree[0] = 0;
  ((Sint16*)a1)[25] = actionwk->yposi.w.h - 64;

  actionwk->patno = 2;
  a1->sproffset = 1020;
  a1->mstno.w = 768;
  egg3dai_l_hit2(actionwk);
}

void egg3dai_l_hit2(sprite_status* actionwk) {
  soundset(172);
  ((Sint16*)actionwk)[33] = 0;
  actionwk->r_no0 = 6;
  actionwk->actfree[1] = 16;
  if (actionwk->colicnt != 1)
  {
    egg3dai_l_up(actionwk);
    return;
  }
  scoreup(100);
  actionwk->colino = 0;
  ((Sint16*)actionwk)[25] = actionwk->yposi.w.h - 112;
  if (!generate_flag) sub_sync(20);
  else sub_sync(19);

  egg3dai_l_up(actionwk);
}

void jaba_last(sprite_status* actionwk) {
  if (actionwk->actfree[16] != 3) return;
  if (jabaopen_chk(actionwk)) return;
  bossflag = 3;
  jaba_open(actionwk);
}

void egg3dai_l_up(sprite_status* actionwk) {
  switch (actionwk->actfree[17] - 1)
  {
    case 0:
      bom_set_cannon(actionwk);
      break;
    case 1:
      bom_set_ctrl(actionwk);
      break;
    case 2:
      bom_set_ctrl2(actionwk);
      break;
  }

  actionwk->actfree[2] |= 4;
  jaba_last(actionwk);
  actionwk->yposi.l += ((Sint32*)actionwk)[13];
  if (actionwk->yposi.w.h > ((Sint16*)actionwk)[25]) return;

  actionwk->actfree[2] &= 251;
  actionwk->yposi.w.h = ((Sint16*)actionwk)[25];
  if (actionwk->colicnt != 1)
  {
    actionwk->actfree[0] = 0;
    actionwk->colino = 63;
    actionwk->r_no0 = 4;
    ((Sint16*)actionwk)[25] = actionwk->yposi.w.h - 72;
  }
  else
  {
    genecolor();
    actionwk->actfree[0] = 0;
    actionwk->r_no0 = 8;
    ((Sint16*)actionwk)[25] = actionwk->yposi.w.h - 16;
  }
}

void egg3dai_l_bom(sprite_status* actionwk) {
  bom_set_dai(actionwk);
  jaba_last(actionwk);
  ++actionwk->actfree[0];
  if (actionwk->actfree[0] == 4)
  {
    actionwk->patno = 6;
  }

  actionwk->yposi.l += ((Sint32*)actionwk)[13];
  if (actionwk->yposi.w.h > ((Sint16*)actionwk)[25]) return;
  actionwk->r_no0 = 10;
}

void egg3dai_l_end(sprite_status* actionwk) {
  sprite_status* a1;

  bom_set_dai(actionwk);
  jaba_last(actionwk);
  if (!(actionwk->actfree[2] & 1)) return;
  if (actionwk->actfree[16] == 4)
  {
    a1 = &actwk[((Uint16*)actionwk)[28]];
    frameout(a1);
    frameout(actionwk);
  }
}

void bom_set_dai(sprite_status* actionwk) {
  sprite_status* a1;

  ++((Sint16*)actionwk)[33];
  if (((Sint16*)actionwk)[33] != 120)
  {
    if (((Sint16*)actionwk)[33] < 120)
      bom_set(actionwk, bomtbl_dai);
  }
  else
  {
    actionwk->actfree[2] |= 1;
    a1 = &actwk[((Uint16*)actionwk)[28]];
    a1->actfree[2] |= 1;
  }
}

void bom_set_cannon(sprite_status* actionwk) {
  ++((Sint16*)actionwk)[33];
  if (((Sint16*)actionwk)[33] < 4)
    bom_set(actionwk, bomtbl_cannon);

}

void bom_set_ctrl(sprite_status* actionwk) {
  ++((Sint16*)actionwk)[33];
  if (((Sint16*)actionwk)[33] < 4)
    bom_set(actionwk, bomtbl_ctrl);

}

void bom_set_ctrl2(sprite_status* actionwk) {
  ++((Sint16*)actionwk)[33];
  if (((Sint16*)actionwk)[33] > 2) return;
  bom_set(actionwk, bomtbl_ctrl2);
}

void bom_set(sprite_status* actionwk, char* tbl) {
  Sint16 d0, d2;
  char* a2;
  sprite_status* a1;

  a2 = tbl;
  d2 = ((Sint16*)actionwk)[33];
  d0 = a2[0];
  if (d2 % d0 != 0) return;
  d2 /= d0;
  d2 %= a2[1];
  d2 += d2;
  if (actwkchk(&a1) != 0) return;
  a1->xposi.w.h = actionwk->xposi.w.h + (Sint16)a2[d2 + 2];
  a1->yposi.w.h = actionwk->yposi.w.h + (Sint16)a2[d2 + 3];

a1->actno = 24;
  a1->r_no1 = 255;
  soundset(158);
}

void egg3(sprite_status* actionwk) {
  switch (actionwk->r_no0)
  {
    case 0:
      egg3_ini(actionwk);
    case 2:
      egg3_norm(actionwk);
      break;
    case 4:
      egg3_tobi(actionwk);
      break;
    case 6:
      egg3_esc1(actionwk);
      break;
    case 8:
      egg3_esc2(actionwk);
      break;
  }
  if ((char)actionwk->actflg <= 0)
    patchg(actionwk, (Uint8**)egg3_pchg);
  actionsub(actionwk);
}

void egg3_ini(sprite_status* actionwk) {
  actionwk->r_no0 = 2;
  actionwk->actflg = 4;
  actionwk->sprpri = 6;
  actionwk->sprhsize = 16;
  actionwk->sprvsize = 16;
  actionwk->sproffset = 798;
  actionwk->patbase = egg3_pat;
  ((Sint16*)actionwk)[25] = actionwk->yposi.w.h - 16;
}

void egg3_norm(sprite_status* actionwk) {
  sprite_status* a1;

  a1 = &actwk[((Uint16*)actionwk)[28]];
  actionwk->yposi.w = a1->yposi.w;
}

void egg3_tobi(sprite_status* actionwk) {
  sprite_status* a1;

  actionwk->yposi.l += ((Sint32*)actionwk)[13];
  ((Sint32*)actionwk)[13] += 12288;
  a1 = &actwk[((Uint16*)actionwk)[28]];
  if (actionwk->yposi.w.h < a1->yposi.w.h) return;

  actionwk->yposi.w.h = a1->yposi.w.h;
  actionwk->r_no0 = 2;
  if (a1->actfree[17] != 2)
  {
    actionwk->mstno.w = 1;
  }
  else
  {
    a1->patno = 2;
    actionwk->sproffset = 1020;
    actionwk->mstno.w = 768;
  }
}

void egg3_esc1(sprite_status* actionwk) {
  actionwk->yposi.l += -0x40000;
  if (actionwk->yposi.w.h <= ((Sint16*)actionwk)[25])
  {
    actionwk->mstno.w = 1024;
    actionwk->sprpri = 3;
    actionwk->yposi.w.h = ((Sint16*)actionwk)[25];
    actionwk->r_no0 = 8;
    ((Sint16*)actionwk)[25] = actionwk->xposi.w.h + 560;
  }
}

void egg3_esc2(sprite_status* actionwk) {
  actionwk->xposi.l += 0x40000;
  if (actionwk->xposi.w.h < ((Sint16*)actionwk)[25]) return;

  frameout(actionwk);
}

void egg3haguruma(sprite_status* actionwk) {
  sprite_status* a1;

  if (actionwk->r_no0 == 0)
  {

    actionwk->r_no0 += 2;
    actionwk->actflg = 4;
    actionwk->sprpri = 3;
    actionwk->sprhsize = 16;
    actionwk->sprvsize = 16;
    actionwk->sproffset = 798;
    actionwk->patbase = egg3haguruma_pat;
  }

  if (actionwk->actfree[2] & 1) return;
  a1 = &actwk[((Uint16*)actionwk)[28]];
  actionwk->yposi.w.h = a1->yposi.w.h;
  if (a1->actfree[2] & 4)
  {
    patchg(actionwk, (Uint8**)egg3haguruma_pchg);
  }
  actionsub(actionwk);
}

void egg3cannon(sprite_status* actionwk) {
  sprite_status* a2;

  a2 = &actwk[((Uint16*)actionwk)[28]];
  actionwk->yposi.w.h = a2->yposi.w.h + 32;
  if (a2->actfree[17])
  {
    frameout(actionwk);
    return;
  }

  switch (actionwk->r_no0)
  {
    case 0:
      egg3cannon_ini(actionwk);
      break;
    case 2:
      egg3cannon_01(actionwk);
      break;
    case 4:
      egg3cannon_fire(actionwk);
      break;
  }
  actionsub(actionwk);
}

void egg3cannon_ini(sprite_status* actionwk) {
  actionwk->r_no0 += 2;
  actionwk->actflg = 4;
  actionwk->sprpri = 4;
  actionwk->sprhsize = 24;
  actionwk->sprvsize = 16;
  actionwk->sproffset = 1148;
  actionwk->patbase = egg3cannon_pat;
}

void egg3cannon_01(sprite_status* actionwk) {
  sprite_status *a1, *a2;
  Sint32 d0;
  Sint16 d1;

  a2 = &actwk[((Uint16*)actionwk)[28]];
  if (actwk[0].yposi.w.h >= 1024) return;
  if (a2->actfree[16]) return;
  ++actionwk->actfree[0];
  if (actionwk->actfree[0] >= 120)
  {

    actionwk->actfree[0] = 0;
    actionwk->r_no0 = 4;

    if (actwkchk(&a1) != 0) return;
    a1->actno = 62;
    a1->xposi.w.h = actionwk->xposi.w.h;
    a1->yposi.w.h = actionwk->yposi.w.h + 8;
    d0 = sRandom() & 0x7FFFF;
    d0 %= 1024;
    d1 = 12;
    if (actionwk->patcnt == 0)
    {
      d0 = -d0;
      d1 = -d1;
    }
    a1->xposi.w.h += d1;
    a1->xspeed.w = d0;
    a1->yspeed.w = 768;

    a1->actfree[0] = 0;
    a1->pattim = 0;
  }
}

void egg3cannon_fire(sprite_status* actionwk) {
  Uint8 cannon_chg_tbl[8] = { 0, 1, 0, 2, 0, 3, 0, 4 };
  Uint8 d0;

(char)actionwk->pattim--;
  if ((char)actionwk->pattim > 0) return;
  d0 = actionwk->patcnt;
  ++d0;
  if (d0 == 4)
  {
    actionwk->r_no0 = 2;
  }
  else
    if (d0 == 8)
    {
      d0 = 0;
      actionwk->r_no0 = 2;
    }
    else
      actionwk->pattim = 2;

  actionwk->patcnt = d0;
  actionwk->patno = cannon_chg_tbl[d0];
}

void punchbom(sprite_status* actionwk) {
  switch (actionwk->r_no0)
  {
    case 0:
      punchbom_ini(actionwk);
    case 2:
      punchbom_mov(actionwk);
      break;
    case 4:
      punchbom_kem(actionwk);
      break;
    case 6:
      punchbom_del(actionwk);
      break;
    case 8:
      punchbom_kemini(actionwk);
      break;
    case 10:
      punchbom_fireini(actionwk);
      break;
  }
}

void punchbom_ini(sprite_status* actionwk) {
  actionwk->r_no0 = 2;
  actionwk->actflg = 4;
  actionwk->sprpri = 5;
  actionwk->sprhsize = 8;
  actionwk->sprvsize = 8;
  actionwk->sproffset = 1148;
  actionwk->patbase = egg3punchbom_pat;
  actionwk->colino = 215;

  ((Sint16*)actionwk)[30] = 0;
  ((Sint16*)actionwk)[31] = 32;
  ((Sint16*)actionwk)[32] = 1536;
}

void punchbom_mov(sprite_status* actionwk) {
  Sint16 d1;

  if (actionwk->colicnt)
  {
    punchbom_hit(actionwk);
    return;
  }
  ++actionwk->actfree[0];
  if (actionwk->actfree[0] >= 240)
  {
    punchbom_kemini(actionwk);
    return;
  }
  ++actionwk->pattim;

  if (actionwk->pattim >> 1 & 1) actionwk->patno = 1;
  else actionwk->patno = 0;

  add_spd3(actionwk);

  if (actionwk->actfree[1] >> 1 & 1)
  {

    if (actionwk->yspeed.w >= 0)
    {
      d1 = emycol_d(actionwk);
      if (d1 <= 0)
      {
        actionwk->yposi.w.h += d1;
        actionwk->yspeed.w = -actionwk->yspeed.w;
      }
    }
    else
    {
      d1 = emycol_u(actionwk);
      if (d1 <= 0)
      {
        actionwk->yposi.w.h -= d1;
        actionwk->yspeed.w = -actionwk->yspeed.w;
      }
    }
  }
  if (actionwk->xspeed.w > 0)
  {
    d1 = emycol_r(actionwk, actionwk->sprhs);
    if (d1 <= 0)
    {
      actionwk->xposi.w.h += d1;
      actionwk->xspeed.w = -actionwk->xspeed.w;
    }
  }
  else
  {
    d1 = emycol_l(actionwk, actionwk->sprhs);
    if (d1 <= 0)
    {
      actionwk->xposi.w.h -= d1;
      actionwk->xspeed.w = -actionwk->xspeed.w;
    }
  }
  actionsub(actionwk);
}

void punchbom_hit(sprite_status* actionwk) {
  sprite_status* a1;
  Uint8 d;
  Sint32 d0l, d1l;
  Sint16 d0, d1, d2;

  a1 = &actwk[0];
  actionwk->colicnt = 0;
  d1 = actionwk->xposi.w.h - a1->xposi.w.h;
  d2 = actionwk->yposi.w.h - a1->yposi.w.h;
  d = atan_sonic(d1, d2);
  sinset(d, &d0, &d1);
  d1l = d1 * -1792;
  a1->xspeed.w = d1l >> 8;
  d0l = d0 * -1792;
  a1->yspeed.w = d0l >> 8;
  a1->cddat |= 2;
  a1->cddat &= 239;
  a1->cddat &= 223;
  a1->actfree[18] = 0;
  actionwk->mstno.b.h = 1;
  if (actionwk->cdsts)
    if (flagwork[actionwk->cdsts] < 138)
      ++flagwork[actionwk->cdsts];

  a1->cddat &= 247;
  punchbom_kemini(actionwk);
}

void punchbom_kemini(sprite_status* actionwk) {
  soundset(158);
  actionwk->mstno.w = 256;
  actionwk->r_no0 = 4;
  punchbom_bomset(actionwk);
}

void punchbom_fireini(sprite_status* actionwk) {
  actionwk->mstno.w = 1;
  actionwk->r_no0 = 4;
  punchbom_bomset(actionwk);
}

void punchbom_bomset(sprite_status* actionwk) {
  actionwk->colino = 0;
  actionwk->actflg = 4;
  actionwk->sprpri = 1;
  actionwk->sprhsize = 16;
  actionwk->sprvsize = 16;
  actionwk->sproffset = 1664;
  actionwk->patbase = bakupat;
  punchbom_kem(actionwk);
}

void punchbom_kem(sprite_status* actionwk) {
  patchg(actionwk, (Uint8**)bakuchg);
  actionsub(actionwk);
}

void punchbom_del(sprite_status* actionwk) {
  frameout(actionwk);
}

void add_spd3(sprite_status* actionwk) {
  Sint16 d0;

  actionwk->yspeed.w += ((Sint16*)actionwk)[31];
  actionwk->xspeed.w += ((Sint16*)actionwk)[30];
  if (((Sint16*)actionwk)[31] == 0)
  {
    add_spd(actionwk);
    return;
  }
  if (((Sint16*)actionwk)[31] > 0)
  {
    d0 = ((Sint16*)actionwk)[32];
    if (actionwk->yspeed.w < d0)
    {
      add_spd(actionwk);
    }
    else
    {
      actionwk->yspeed.w = d0;
      add_spd(actionwk);
    }
  }
  else
  {
    d0 = ((Sint16*)actionwk)[32];
    if (actionwk->yspeed.w > d0)
    {
      add_spd(actionwk);
    }
    else
    {
      actionwk->yspeed.w = d0;
      add_spd(actionwk);
    }
  }
}

void add_spd2(sprite_status* actionwk) {
  actionwk->yspeed.w += ((Sint16*)actionwk)[31];
  actionwk->xspeed.w += ((Sint16*)actionwk)[30];
  add_spd(actionwk);
}

void add_spd(sprite_status* actionwk) {
  Sint32 d0;

  d0 = actionwk->xspeed.w;
  actionwk->xposi.l += d0 << 8;
  d0 = actionwk->yspeed.w;
  actionwk->yposi.l += d0 << 8;
}
