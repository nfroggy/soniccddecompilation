#include "../equ.h"
#include "beam.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../loader2.h"
#include "../playsub.h"

static void beam_init(sprite_status* beamwk);
static void beam_mast(sprite_status* beamwk);
static void beam_move0(sprite_status* beamwk);
static void beam_move1(sprite_status* beamwk);
static void beam_move2(sprite_status* beamwk);
static void beam_coli0(sprite_status* beamwk);
static void beam_coli(sprite_status* beamwk);

static sprite_pattern bea00 = {
  2,
  {
    { -8, -12, 0, 294 },
    { 0, -12, 8, 294 }
  }
};
static sprite_pattern bea01 = {
  2,
  {
    { -8, -12, 0, 295 },
    { 0, -12, 8, 295 }
  }
};
static sprite_pattern bea02 = {
  2,
  {
    { -8, -12, 0, 295 },
    { 0, -12, 8, 295 }
  }
};
static sprite_pattern bea03 = {
  2,
  {
    { -8, -4, 0, 300 },
    { 0, -4, 8, 300 }
  }
};
static sprite_pattern bea04 = {
  2,
  {
    { -8, -4, 0, 301 },
    { 0, -4, 8, 301 }
  }
};
static sprite_pattern bea05 = {
  2,
  {
    { -4, -4, 0, 297 },
    { -4, -12, 0, 297 }
  }
};
static sprite_pattern bea06 = {
  4,
  {
    { -4, -4, 0, 297 },
    { -4, -12, 0, 297 },
    { -4, -20, 0, 297 },
    { -4, -28, 0, 297 }
  }
};
static sprite_pattern bea07 = {
  6,
  {
    { -4, -4, 0, 297 },
    { -4, -12, 0, 297 },
    { -4, -20, 0, 297 },
    { -4, -28, 0, 297 },
    { -4, -36, 0, 297 },
    { -4, -44, 0, 297 }
  }
};
static sprite_pattern bea08 = {
  8,
  {
    { -4, -4, 0, 297 },
    { -4, -12, 0, 297 },
    { -4, -20, 0, 297 },
    { -4, -28, 0, 297 },
    { -4, -36, 0, 297 },
    { -4, -44, 0, 297 },
    { -4, -52, 0, 297 },
    { -4, -60, 0, 297 }
  }
};
static sprite_pattern bea09 = {
  10,
  {
    { -4, -4, 0, 297 },
    { -4, -12, 0, 297 },
    { -4, -20, 0, 297 },
    { -4, -28, 0, 297 },
    { -4, -36, 0, 297 },
    { -4, -44, 0, 297 },
    { -4, -52, 0, 297 },
    { -4, -60, 0, 297 },
    { -4, -68, 0, 297 },
    { -4, -76, 0, 297 }
  }
};
static sprite_pattern bea0a = {
  12,
  {
    { -4, -4, 0, 297 },
    { -4, -12, 0, 297 },
    { -4, -20, 0, 297 },
    { -4, -28, 0, 297 },
    { -4, -36, 0, 297 },
    { -4, -44, 0, 297 },
    { -4, -52, 0, 297 },
    { -4, -60, 0, 297 },
    { -4, -68, 0, 297 },
    { -4, -76, 0, 297 },
    { -4, -84, 0, 297 },
    { -4, -92, 0, 297 }
  }
};
static sprite_pattern bea0b = {
  14,
  {
    { -4, -4, 0, 297 },
    { -4, -12, 0, 297 },
    { -4, -20, 0, 297 },
    { -4, -28, 0, 297 },
    { -4, -36, 0, 297 },
    { -4, -44, 0, 297 },
    { -4, -52, 0, 297 },
    { -4, -60, 0, 297 },
    { -4, -68, 0, 297 },
    { -4, -76, 0, 297 },
    { -4, -84, 0, 297 },
    { -4, -92, 0, 297 },
    { -4, -100, 0, 297 },
    { -4, -108, 0, 297 }
  }
};
static sprite_pattern bea0c = {
  16,
  {
    { -4, -4, 0, 297 },
    { -4, -12, 0, 297 },
    { -4, -20, 0, 297 },
    { -4, -28, 0, 297 },
    { -4, -36, 0, 297 },
    { -4, -44, 0, 297 },
    { -4, -52, 0, 297 },
    { -4, -60, 0, 297 },
    { -4, -68, 0, 297 },
    { -4, -76, 0, 297 },
    { -4, -84, 0, 297 },
    { -4, -92, 0, 297 },
    { -4, -100, 0, 297 },
    { -4, -108, 0, 297 },
    { -4, -116, 0, 297 },
    { -4, -124, 0, 297 }
  }
};
static sprite_pattern bea0d = {
  2,
  {
    { -4, -4, 0, 298 },
    { -4, -12, 0, 298 }
  }
};
static sprite_pattern bea0e = {
  4,
  {
    { -4, -4, 0, 298 },
    { -4, -12, 0, 298 },
    { -4, -20, 0, 298 },
    { -4, -28, 0, 298 }
  }
};
static sprite_pattern bea0f = {
  6,
  {
    { -4, -4, 0, 298 },
    { -4, -12, 0, 298 },
    { -4, -20, 0, 298 },
    { -4, -28, 0, 298 },
    { -4, -36, 0, 298 },
    { -4, -44, 0, 298 }
  }
};
static sprite_pattern bea10 = {
  8,
  {
    { -4, -4, 0, 298 },
    { -4, -12, 0, 298 },
    { -4, -20, 0, 298 },
    { -4, -28, 0, 298 },
    { -4, -36, 0, 298 },
    { -4, -44, 0, 298 },
    { -4, -52, 0, 298 },
    { -4, -60, 0, 298 }
  }
};
static sprite_pattern bea11 = {
  10,
  {
    { -4, -4, 0, 298 },
    { -4, -12, 0, 298 },
    { -4, -20, 0, 298 },
    { -4, -28, 0, 298 },
    { -4, -36, 0, 298 },
    { -4, -44, 0, 298 },
    { -4, -52, 0, 298 },
    { -4, -60, 0, 298 },
    { -4, -68, 0, 298 },
    { -4, -76, 0, 298 }
  }
};
static sprite_pattern bea12 = {
  12,
  {
    { -4, -4, 0, 298 },
    { -4, -12, 0, 298 },
    { -4, -20, 0, 298 },
    { -4, -28, 0, 298 },
    { -4, -36, 0, 298 },
    { -4, -44, 0, 298 },
    { -4, -52, 0, 298 },
    { -4, -60, 0, 298 },
    { -4, -68, 0, 298 },
    { -4, -76, 0, 298 },
    { -4, -84, 0, 298 },
    { -4, -92, 0, 298 }
  }
};
static sprite_pattern bea13 = {
  14,
  {
    { -4, -4, 0, 298 },
    { -4, -12, 0, 298 },
    { -4, -20, 0, 298 },
    { -4, -28, 0, 298 },
    { -4, -36, 0, 298 },
    { -4, -44, 0, 298 },
    { -4, -52, 0, 298 },
    { -4, -60, 0, 298 },
    { -4, -68, 0, 298 },
    { -4, -76, 0, 298 },
    { -4, -84, 0, 298 },
    { -4, -92, 0, 298 },
    { -4, -100, 0, 298 },
    { -4, -108, 0, 298 }
  }
};
static sprite_pattern bea14 = {
  16,
  {
    { -4, -4, 0, 298 },
    { -4, -12, 0, 298 },
    { -4, -20, 0, 298 },
    { -4, -28, 0, 298 },
    { -4, -36, 0, 298 },
    { -4, -44, 0, 298 },
    { -4, -52, 0, 298 },
    { -4, -60, 0, 298 },
    { -4, -68, 0, 298 },
    { -4, -76, 0, 298 },
    { -4, -84, 0, 298 },
    { -4, -92, 0, 298 },
    { -4, -100, 0, 298 },
    { -4, -108, 0, 298 },
    { -4, -116, 0, 298 },
    { -4, -124, 0, 298 }
  }
};
static sprite_pattern bea15 = {
  2,
  {
    { -4, -4, 0, 299 },
    { -4, -12, 0, 299 }
  }
};
static sprite_pattern bea16 = {
  4,
  {
    { -4, -4, 0, 299 },
    { -4, -12, 0, 299 },
    { -4, -20, 0, 299 },
    { -4, -28, 0, 299 }
  }
};
static sprite_pattern bea17 = {
  6,
  {
    { -4, -4, 0, 299 },
    { -4, -12, 0, 299 },
    { -4, -20, 0, 299 },
    { -4, -28, 0, 299 },
    { -4, -36, 0, 299 },
    { -4, -44, 0, 299 }
  }
};
static sprite_pattern bea18 = {
  8,
  {
    { -4, -4, 0, 299 },
    { -4, -12, 0, 299 },
    { -4, -20, 0, 299 },
    { -4, -28, 0, 299 },
    { -4, -36, 0, 299 },
    { -4, -44, 0, 299 },
    { -4, -52, 0, 299 },
    { -4, -60, 0, 299 }
  }
};
static sprite_pattern bea19 = {
  10,
  {
    { -4, -4, 0, 299 },
    { -4, -12, 0, 299 },
    { -4, -20, 0, 299 },
    { -4, -28, 0, 299 },
    { -4, -36, 0, 299 },
    { -4, -44, 0, 299 },
    { -4, -52, 0, 299 },
    { -4, -60, 0, 299 },
    { -4, -68, 0, 299 },
    { -4, -76, 0, 299 }
  }
};
static sprite_pattern bea1a = {
  12,
  {
    { -4, -4, 0, 299 },
    { -4, -12, 0, 299 },
    { -4, -20, 0, 299 },
    { -4, -28, 0, 299 },
    { -4, -36, 0, 299 },
    { -4, -44, 0, 299 },
    { -4, -52, 0, 299 },
    { -4, -60, 0, 299 },
    { -4, -68, 0, 299 },
    { -4, -76, 0, 299 },
    { -4, -84, 0, 299 },
    { -4, -92, 0, 299 }
  }
};
static sprite_pattern bea1b = {
  14,
  {
    { -4, -4, 0, 299 },
    { -4, -12, 0, 299 },
    { -4, -20, 0, 299 },
    { -4, -28, 0, 299 },
    { -4, -36, 0, 299 },
    { -4, -44, 0, 299 },
    { -4, -52, 0, 299 },
    { -4, -60, 0, 299 },
    { -4, -68, 0, 299 },
    { -4, -76, 0, 299 },
    { -4, -84, 0, 299 },
    { -4, -92, 0, 299 },
    { -4, -100, 0, 299 },
    { -4, -108, 0, 299 }
  }
};
static sprite_pattern bea1c = {
  16,
  {
    { -4, -4, 0, 299 },
    { -4, -12, 0, 299 },
    { -4, -20, 0, 299 },
    { -4, -28, 0, 299 },
    { -4, -36, 0, 299 },
    { -4, -44, 0, 299 },
    { -4, -52, 0, 299 },
    { -4, -60, 0, 299 },
    { -4, -68, 0, 299 },
    { -4, -76, 0, 299 },
    { -4, -84, 0, 299 },
    { -4, -92, 0, 299 },
    { -4, -100, 0, 299 },
    { -4, -108, 0, 299 },
    { -4, -116, 0, 299 },
    { -4, -124, 0, 299 }
  }
};
static sprite_pattern bea1d = {
  2,
  {
    { -8, -4, 0, 302 },
    { 0, -4, 0, 303 }
  }
};
sprite_pattern* beampat[21] = { &bea00, &bea01, &bea02, &bea03, &bea04, &bea05, &bea06, &bea07, &bea08, &bea09, &bea0a, &bea0b, &bea0c, &bea0d, &bea0e, &bea0f, &bea10, &bea11, &bea12, &bea13, &bea14 };
sprite_pattern* beampat0[21] = { &bea00, &bea01, &bea02, &bea03, &bea1d, &bea05, &bea06, &bea07, &bea08, &bea09, &bea0a, &bea0b, &bea0c, &bea15, &bea16, &bea17, &bea18, &bea19, &bea1a, &bea1b, &bea1c };
static Uint8 beamchg0[4] = { 2, 5, 13, 255 };
static Uint8 beamchg1[4] = { 2, 14, 6, 255 };
static Uint8 beamchg2[4] = { 2, 7, 15, 255 };
static Uint8 beamchg3[4] = { 2, 16, 8, 255 };
static Uint8 beamchg4[4] = { 2, 9, 17, 255 };
static Uint8 beamchg5[4] = { 2, 18, 10, 255 };
static Uint8 beamchg6[4] = { 2, 11, 19, 255 };
static Uint8 beamchg7[4] = { 2, 20, 12, 255 };
static Uint8 beamchg8[4] = { 2, 3, 4, 255 };
static Uint8* beamchg[9] = { beamchg0, beamchg1, beamchg2, beamchg3, beamchg4, beamchg5, beamchg6, beamchg7, beamchg8 };

void beam(sprite_status* beamwk) {
  void(*tbl[5])(sprite_status*) = { &beam_init, &beam_mast, &beam_move0, &beam_move1, &beam_move2 };

tbl[beamwk->r_no0 / 2](beamwk);
  actionsub(beamwk);
  frameout_s(beamwk);
}

static void beam_init(sprite_status* beamwk) {
  beamwk->r_no0 += 2;
  beamwk->actflg |= 4;

  beamwk->patbase = beampat;
  beamwk->patno = 1;
  if (beamwk->userflag.b.h) {
    beamwk->patno = 2;
    beamwk->patbase= beampat0;
  }
  beamwk->actfree[0] = 1;
  beam_mast(beamwk);
}

static void beam_mast(sprite_status* beamwk) {
  sprite_status* new_actwk;

  if (--beamwk->actfree[0]) return;

  if (actwkchk2(beamwk, &new_actwk) != 0) {
    if (actwkchk(&new_actwk) != 0) return;
  }
  new_actwk->actno = 29;
  new_actwk->r_no0 = 4;

  new_actwk->patbase = beamwk->patbase;
  new_actwk->actflg |= 4;
  new_actwk->sprpri = 2;
  new_actwk->xposi.w.h = beamwk->xposi.w.h;
  new_actwk->yposi.w.h = beamwk->yposi.w.h & 65520;
  new_actwk->userflag.b.h = beamwk->userflag.b.h;

  new_actwk->sprvsize = 4;

  beamwk->actfree[0] = 30;
  if ((char)beamwk->actflg < 0) {
    if (beamwk->userflag.b.h == 0) soundset(197);
    else soundset(188);
  }
}

static void beam_move0(sprite_status* beamwk) {
  sprite_status *new_actwk, **parent;

  beamwk->yposi.w.h += 4;
  if (!(beamwk->yposi.w.h & 15)) {
    if (++beamwk->mstno.b.h >= 7) beamwk->mstno.b.h = 7;
  }
  patchg(beamwk, beamchg);

  if (emycol_d(beamwk) >= 0) goto label1;
  beamwk->r_no0 += 2;
  if (actwkchk2(beamwk, &new_actwk) != 0) {
    beamwk->actfree[21] = 255;
    goto label1;
  }
  new_actwk->actno = 29;

  new_actwk->patbase = beamwk->patbase;
  new_actwk->actflg |= 4;
  new_actwk->xposi.w.h = beamwk->xposi.w.h;
  new_actwk->yposi.w.h = beamwk->yposi.w.h;
  new_actwk->r_no0 = 8;
  new_actwk->mstno.b.h = 8;
  parent = &((sprite_status**)beamwk)[12];
  *parent = new_actwk;
label1:
  beam_coli0(beamwk);
}

static void beam_move1(sprite_status* beamwk) {
  Sint16 cal_yposi;
  sprite_status **parent, *new_actwk;

  cal_yposi = beamwk->yposi.w.h + 4;
  if (!(cal_yposi & 15)) {
    cal_yposi -= 16;
    if (beamwk->mstno.b.h - 1 < 0) goto label1;
    --beamwk->mstno.b.h;
  }
  beamwk->yposi.w.h = cal_yposi;
  patchg(beamwk, beamchg);
  beam_coli0(beamwk);
  return;
label1:
  if (beamwk->actfree[21] == 0) {
    parent = &((sprite_status**)beamwk)[12];
    new_actwk = *parent;
    new_actwk->actfree[0] = 1;
  }
  frameout(beamwk);
}

static void beam_move2(sprite_status* beamwk) {
  if (beamwk->actfree[0]) { frameout(beamwk); return; }
  patchg(beamwk, beamchg);
}

static void beam_coli0(sprite_status* beamwk) {
  if (actwk[0].mstno.b.h != 43) {
    if (beamwk->userflag.b.h == 0) {
      if (chibi_flag == 0) beam_coli(beamwk);
    }
    else if (chibi_flag != 0) beam_coli(beamwk);
  }
}

static void beam_coli(sprite_status* beamwk) {
  Sint16 cal_size, cal_posi, cal_tbl;
  char tbl[8] = { -12, -28, -44, -60, -76, -92, -108, -124 };

  if (editmode.b.h) return;
  cal_size = (char)actwk[0].sprhs + 4;
  cal_posi = beamwk->xposi.w.h - actwk[0].xposi.w.h + cal_size;
  if (cal_posi < 0 || cal_posi >= cal_size * 2) return;
  cal_size = (char)actwk[0].sprvsize;
  cal_posi = actwk[0].yposi.w.h + cal_size;
  cal_tbl = tbl[beamwk->mstno.b.h];
  cal_tbl += beamwk->yposi.w.h;
  if (cal_tbl >= cal_posi) return;
  cal_posi = actwk[0].yposi.w.h - cal_size;
  cal_tbl = beamwk->yposi.w.h + 4;
  if (cal_posi >= cal_tbl) return;

  actwk[0].r_no0 = 4;
  actwk[0].cddat |= 2;
  actwk[0].yspeed.w = -1024;
  actwk[0].xspeed.w = -512;
  if (actwk[0].xposi.w.h >= beamwk->xposi.w.h)
    actwk[0].xspeed.w = -actwk[0].xspeed.w;
  actwk[0].mspeed.w = 0;
  actwk[0].mstno.b.h = 56;
  actwk[0].sprvsize = 10;
  actwk[0].sprhs = 5;
  chibi_flag ^= 1;
  if (chibi_flag == 0) {
    actwk[0].sprvsize = 19;
    actwk[0].sprhs = 9;
  }
}
