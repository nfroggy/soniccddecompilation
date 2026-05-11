#include "../equ.h"
#include "chgban.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"
#include "../loader2.h"
#include "../playsub.h"

#if defined(R73C) || defined(R73D)
  #define SPRITE_CHGBAN_BASE 453
#else
  #define SPRITE_CHGBAN_BASE 442
#endif

static void ami_init(sprite_status* pActwk);
static void ami_move(sprite_status* pActwk);
static void chgban_init(sprite_status* pActwk);
static void chgban_move0(sprite_status* pActwk);
static void chgban_move1(sprite_status* pActwk);
static void chgban_move2(sprite_status* pActwk);
static void spr_r_init(sprite_status* pActwk);
static void spr_r_move0(sprite_status* pActwk);
static void spr_r_move1(sprite_status* pActwk);
static void spr_r_move2(sprite_status* pActwk);
static void spr_r_move3(sprite_status* pActwk);
static void spr_r_move4(sprite_status* pActwk);
static void spr_r_move5(sprite_status* pActwk);
static void spr_r_move6(sprite_status* pActwk);
static void gun7_init(sprite_status* pActwk);
static void gun7_move0(sprite_status* pActwk);
static void gun7_move1(sprite_status* pActwk);
static void gun7_move2(sprite_status* pActwk);
static void hibana_init(sprite_status* pActwk);
static void hibana_move0(sprite_status* pActwk);
static void hibana_move1(sprite_status* pActwk);
static void spr_r_set(sprite_status* pActwk);
static void coli_spr(sprite_status* pActwk);
static Uint32 coli0(sprite_status* pActwk, sprite_status* pPlayerwk, Sint16 xOffs, Sint16 yOffs);
static void playset(sprite_status* pActwk, sprite_status* pPlayerwk);
static Sint16 pposiset(sprite_status* pActwk, sprite_status* pMstwk, sprite_status* pPlayerwk, char* pPostbl);
static void plspdset(sprite_status* pActwk, sprite_status* pPlayerwk, Sint16 patcnt);
static void g_move0(sprite_status* pActwk, Uint8* pPatTbl, Uint8* pRevTbl, Uint8* pGposiTbl, Uint8* pPposiTbl);
static void playset_g(sprite_status* pActwk, sprite_status* pMstadr, sprite_status* pPlayerwk, Uint8* pPposiTbl);
static void gspdset(sprite_status* pMstadr, sprite_status* pGawaadr);
static void gun7_coli(sprite_status* pActwk);
static void gun7set(sprite_status* pActwk);

static char srevtbl0[9] = { 0, 0, 0, 0, 1, 1, 1, 0, 0 };
static char srevtbl1[9] = { 2, 0, 0, 0, 1, 1, 3, 2, 2 };
static char srevtbl2[9] = { 1, 1, 3, 2, 2, 0, 0, 0, 1 };
static Sint16 spositbl0[18] = {
   8, -8,
   8,  0,
   8, -8,
   0, -8,
  -8, -8,
  -8,  0,
  -8, -8,
   0, -8,
   8, -8
};
static Sint16 spositbl1[18] = {
   8,  8,
   8,  0,
   8, -8,
   0, -8,
  -8, -8,
  -8,  0,
  -8,  8,
   0,  8,
   8,  8
};
static Sint16 spositbl2[18] = {
  -8, -8,
  -8,  0,
  -8,  8,
   0,  8,
   8,  8,
   8,  0,
   8, -8,
   0, -8,
  -8, -8
};
static char ppositbl0[9] = { 1, 0, 1, 2, 3, 4, 3, 2, 1 };
static char ppositbl1[9] = { 7, 0, 1, 2, 3, 4, 5, 6, 7 };
static char ppositblg_0[9] = { 1, 0, 1, 2, 3, 4, 5, 6, 7 };
static char ppositblg_1[9] = { 3, 4, 5, 6, 7, 0, 1, 2, 3 };
static Sint16 ppositbl[16] = {
   24,   0,
   24, -24,
    0, -24,
  -24, -24,
  -24,   0,
  -24,  24,
    0,  24,
   24,  24
};
static Uint8 gpattbl[9] = { 4, 2, 4, 3, 4, 2, 4, 3, 4 };
static Uint8 grevtbl0_0[9] = { 0, 0, 0, 0, 2, 2, 2, 0, 0 };
static Uint8 grevtbl0_1[9] = { 3, 2, 3, 1, 1, 0, 1, 1, 3 };
static Uint8 grevtbl1_0[9] = { 1, 0, 0, 0, 2, 2, 3, 1, 1 };
static Uint8 grevtbl1_1[9] = { 2, 2, 3, 1, 1, 0, 0, 0, 2 };
static Uint8 gpositbl0_0[9] = { 1, 0, 1, 2, 3, 4, 3, 2, 1 };
static Uint8 gpositbl0_1[9] = { 9, 8, 9, 10, 11, 12, 11, 10, 9 };
static Uint8 gpositbl1_0[9] = { 7, 0, 1, 2, 3, 4, 5, 6, 7 };
static Uint8 gpositbl1_1[9] = { 3, 4, 5, 6, 7, 0, 1, 2, 3 };
static Uint8 gpositbl2_0[9] = { 11, 12, 13, 14, 15, 8, 9, 10, 11 };
static Uint8 gpositbl2_1[9] = { 15, 8, 9, 10, 11, 12, 13, 14, 15 };
static Sint16 gpositbl[32] = {
   24, -16,
    4, -28,
  -16, -24,
  -28,  -4,
  -24,  16,
   -4,  28,
   16,  24,
   28,   4,
   24,  16,
   28,  -4,
   16, -24,
   -4, -28,
  -24, -16,
  -28,   4,
  -16,  24,
    4,  28
};
static Sint16 gspdtbl0[18] = {
  -1, -1,
  -1,  0,
  -1,  1,
   0,  1,
   1,  1,
   1,  0,
   1, -1,
   0, -1,
  -1, -1
};
static Sint16 gspdtbl1[18] = {
   1,  1,
   1,  0,
   1, -1,
   0, -1,
  -1, -1,
  -1,  0,
  -1,  1,
   0,  1,
   1,  1
};
sprite_pattern amipat0 = {
  1,
  { { -16, -16, 0, SPRITE_CHGBAN_BASE } }
};
sprite_pattern* amipat[1] = { &amipat0 };
static void(*ami_act_tbl[2])(sprite_status*) = {
  &ami_init,
  &ami_move
};
static Uint8 chgbanchg0[11] = { 4, 0, 1, 2, 3, 4, 3, 2, 1, 0, 252 };
static Uint8* chgbanchg[1] = { chgbanchg0 };
sprite_pattern chgbanpat0 = {
  1,
  { { -24, -16, 0, SPRITE_CHGBAN_BASE + 1 } }
};
sprite_pattern chgbanpat1 = {
  1,
  { { -24, -12, 0, SPRITE_CHGBAN_BASE + 2 } }
};
sprite_pattern chgbanpat2 = {
  1,
  { { -24, -8, 0, SPRITE_CHGBAN_BASE + 3 } }
};
sprite_pattern chgbanpat3 = {
  1,
  { { -24, -16, 0, SPRITE_CHGBAN_BASE + 4 } }
};
sprite_pattern chgbanpat4 = {
  1,
  { { -24, -12, 0, SPRITE_CHGBAN_BASE + 5 } }
};
sprite_pattern* chgbanpat[5] = {
  &chgbanpat0,
  &chgbanpat1,
  &chgbanpat2,
  &chgbanpat4,
  &chgbanpat3
};
static void(*chgban_act_tbl[4])(sprite_status*) = {
  &chgban_init,
  &chgban_move0,
  &chgban_move1,
  &chgban_move2
};
Uint8 spr_rchg0[10] = { 5, 1, 1, 1, 1, 1, 1, 1, 1, 255 };
Uint8* spr_rchg[1] = { spr_rchg0 };
sprite_pattern spr_rpat0 = {
  1,
  { { -8, -4, 0, SPRITE_CHGBAN_BASE + 6 } }
};
sprite_pattern spr_rpat1 = {
  1,
  { { -8, -8, 0, SPRITE_CHGBAN_BASE + 7 } }
};
sprite_pattern spr_rpat2 = {
  1,
  { { -32, -12, 0, SPRITE_CHGBAN_BASE + 8 } }
};
sprite_pattern spr_rpat3 = {
  1,
  { { -12, -32, 0, SPRITE_CHGBAN_BASE + 9 } }
};
sprite_pattern spr_rpat4 = {
  1,
  { { -28, -28, 0, SPRITE_CHGBAN_BASE + 10 } }
};
sprite_pattern* spr_rpat[5] = {
  &spr_rpat0,
  &spr_rpat1,
  &spr_rpat2,
  &spr_rpat3,
  &spr_rpat4
};
static void(*spring_r_act_tbl[8])(sprite_status*) = {
  &spr_r_init,
  &spr_r_move0,
  &spr_r_move1,
  &spr_r_move2,
  &spr_r_move3,
  &spr_r_move4,
  &spr_r_move5,
  &spr_r_move6
};
extern sprite_pattern* springpat2[];
extern sprite_pattern* spring90pat2[];
extern sprite_pattern* spring45pat2[];
extern Uint8* springchg[];
static Uint8 prio_sav;
static void(*gun7_act_tbl[4])(sprite_status*) = {
  &gun7_init,
  &gun7_move0,
  &gun7_move1,
  &gun7_move2
};
static Uint8 hibanachg0[6] = { 3, 0, 1, 0, 1, 252 };
static Uint8* hibanachg[1] = { hibanachg0 };
sprite_pattern hibanapat0 = {
  1,
  { { -32, -32, 0, SPRITE_CHGBAN_BASE + 11 } }
};
sprite_pattern hibanapat1 = {
  1,
  { { -24, -16, 0, SPRITE_CHGBAN_BASE + 12 } }
};
sprite_pattern* hibanapat[2] = {
  &hibanapat0,
  &hibanapat1
};
static void(*hibana_act_tbl[3])(sprite_status*) = {
  &hibana_init,
  &hibana_move0,
  &hibana_move1
};

void ami(sprite_status* pActwk) {
  ami_act_tbl[pActwk->r_no0 / 2](pActwk);
  actionsub(pActwk);
  frameout_s(pActwk);
}

static void ami_init(sprite_status* pActwk) {
  pActwk->actflg |= 4;
  pActwk->sproffset = 17144;
  pActwk->patbase = amipat;
  pActwk->r_no0 += 2;

  ami_move(pActwk);
}

static void ami_move(sprite_status* pActwk) {
  if (prio_flag) {
    pActwk->sprpri = 0;
  } else {
    pActwk->sprpri = 4;
  }
}

void chgban(sprite_status* pActwk) {
  chgban_act_tbl[pActwk->r_no0 / 2](pActwk);
  actionsub(pActwk);
  frameout_s(pActwk);
}

static void chgban_init(sprite_status* pActwk) {
  if (pActwk->userflag.b.h) {
    pActwk->actflg |= 2;
  }

pActwk->r_no0 += 2;
  pActwk->actflg |= 4;
  pActwk->sproffset = 845;

  if (!prio_flag) {
    pActwk->sproffset |= 32768;
  }

pActwk->patbase = chgbanpat;
  pActwk->sprhs = pActwk->sprhsize = pActwk->sprvsize = 24;

  chgban_move0(pActwk);
}

static void chgban_move0(sprite_status* pActwk) {
  Sint16 xsz, xdst, ysz, ydst;

  if (pActwk->actfree[0]) {
    --pActwk->actfree[0];
    return;
  }

xsz = (Sint16)(char)pActwk->sprhs + (Sint16)(char)actwk[0].sprhs;
  xdst = pActwk->xposi.w.h - actwk[0].xposi.w.h;

  ysz = (Sint16)(char)pActwk->sprvsize + (Sint16)(char)actwk[0].sprvsize;

  ydst = pActwk->yposi.w.h - actwk[0].yposi.w.h;

  if ((xdst += xsz) < 0 || xdst >= xsz * 2
      ||
      (ydst += ysz) < 0 || ydst >= ysz * 2) {
    if (((pActwk->sproffset & 32768) && prio_flag)
        ||
        (!(pActwk->sproffset & 32768) && !prio_flag)) {
      pActwk->actfree[0] = 60;
      pActwk->r_no0 += 2;
    }
  } else {
    pActwk->actfree[0] = 60;
    pActwk->r_no0 += 2;
    prio_flag ^= 1;
  }
}

static void chgban_move1(sprite_status* pActwk) {
  patchg(pActwk, chgbanchg);
}

static void chgban_move2(sprite_status* pActwk) {
  if (!prio_flag) {
    pActwk->sproffset |= 32768;
    pActwk->sprpri = 4;
  } else {
    pActwk->sproffset &= 32767;
    pActwk->sprpri = 0;
  }

pActwk->patcnt = 0;
  pActwk->r_no0 -= 4;
}

void spring_r(sprite_status* pActwk) {
  Sint32 mstwkno, gawa0wkno, gawa1wkno;

  if (pActwk->r_no0 >= 4 && pActwk->actfree[20]) {
    frameout(pActwk);
    return;
  }

if (!pActwk->r_no0 || actwk[0].r_no0 < 6) {

    spring_r_act_tbl[pActwk->r_no0 / 2](pActwk);
    if (pActwk->r_no0 != 4 || !pActwk->userflag.b.h) {

      actionsub(pActwk);
    }
  }
  else {
    actionsub(pActwk);
  }

if (pActwk->r_no0 != 2) return;

mstwkno = *(Sint32*)&pActwk->actfree[0];
  gawa0wkno = *(Sint32*)&pActwk->actfree[12];
  gawa1wkno = *(Sint32*)&pActwk->actfree[16];

  if (frameout_s00(pActwk, ((Sint16*)pActwk)[28]) != 0) {

    actwk[mstwkno].actfree[20] = 1;
    actwk[gawa0wkno].actfree[20] = 1;
    actwk[gawa1wkno].actfree[20] = 1;
  }
}

static void spr_r_init(sprite_status* pActwk) {
  sprite_status *pNewact, *pTstwk;
  Sint32 i;
  Uint8 r_nowk;

  pActwk->r_no0 += 2;
  ((Sint16*)pActwk)[28] = pActwk->xposi.w.h;
  pActwk->actflg |= 4;
  pActwk->sproffset = 768;
  pActwk->patbase = spr_rpat;
  pActwk->sprhs = 8;
  pActwk->sprvsize = 8;
  pActwk->patno = 1;

  if (pActwk->userflag.b.h >= 2) {
    pActwk->userflag.b.l = pActwk->patcnt = pActwk->userflag.b.h - 1;
    pActwk->userflag.b.h = 0;
  }

pNewact = pActwk;
  for (i = 0; i < 3; ++i) {

    if (actwkchk2(pNewact, &pTstwk) != 0) {
      frameout(pActwk);
      return;
    }

    pNewact = pTstwk;
  }

  r_nowk = 4;
  for (i = 2; i >= 0; --i) {

    actwkchk2(pActwk, &pNewact);

    pNewact->actno = 30;
    pNewact->actflg |= 4;
    pNewact->xposi.w.h = pActwk->xposi.w.h;
    ((Sint16*)pNewact)[28] = pActwk->xposi.w.h;
    pNewact->yposi.w.h = pActwk->yposi.w.h;

    if (i == 2) {
      pNewact->sproffset = 1312;
      pNewact->patbase = spring90pat2;
      pNewact->sprpri = 5;
      *(Sint32*)&pActwk->actfree[0] = pNewact - actwk;
    }
    else {
      pNewact->sproffset = 768;
      pNewact->patbase = spr_rpat;
      pNewact->sprhs = 32;
      pNewact->sprvsize = 32;

      if (i == 1) {
        *(Sint32*)&pActwk->actfree[12] = pNewact - actwk;
      }
      else {
        *(Sint32*)&pActwk->actfree[16] = pNewact - actwk;
      }
    }

pNewact->userflag.b.h = pActwk->userflag.b.h;
    *(Sint32*)&pNewact->actfree[0] = pActwk - actwk;
    pNewact->r_no0 = r_nowk;

    if (i == 2) {
      r_nowk += 4;
    }

    r_nowk += 2;
  }

  spr_r_move0(pActwk);
}

static void spr_r_move0(sprite_status* pActwk) {
  sprite_status* pMstwk;

  if (!pActwk->userflag.b.l) {
    pMstwk = &actwk[*(Sint32*)&pActwk->actfree[0]];

    if (pMstwk->r_no0 < 6 || pMstwk->userflag.b.l == 2) {

      patchg(pActwk, spr_rchg);
    }
  }
}

static void spr_r_move1(sprite_status* pActwk) {
  if (!pActwk->userflag.b.h) {
    spr_r_set(pActwk);
    coli_spr(pActwk);
  }
}

static void spr_r_move2(sprite_status* pActwk) {
  sprite_status *pMstadr, *pPlayerwk;

  pPlayerwk = &actwk[*(Sint32*)&pActwk->actfree[4]];

  if (pActwk->userflag.b.h) {

    if (pActwk->userflag.b.l == 4) {

      patchg(pActwk, springchg);

spr_r_set(pActwk);
    } else if (pActwk->userflag.b.l != 2 || !(swdata.b.l & 112)) {

      spr_r_set(pActwk);
    } else {
      pActwk->userflag.b.l = 4;
      pMstadr = &actwk[*(Sint32*)&pActwk->actfree[0]];
      plspdset(pActwk, pPlayerwk, pMstadr->patcnt);

actwk[0].actfree[2] &= 254;
      soundset(206);

patchg(pActwk, springchg);

spr_r_set(pActwk);
    }
  }
  else {
    if (!pActwk->actfree[8]) {

      patchg(pActwk, springchg);

spr_r_set(pActwk);
    } else if (--pActwk->actfree[8] != 0) {

      spr_r_set(pActwk);
    }
    else {
      actwk[0].actfree[2] &= 254;
      soundset(206);

patchg(pActwk, springchg);

spr_r_set(pActwk);
    }
  }
}

static void spr_r_move3(sprite_status* pActwk) {
  sprite_status *pMstwk, *pGawawk;

  pActwk->r_no0 -= 4;
  pActwk->patcnt = 0;
  pActwk->patno = 0;
  pActwk->userflag.b.l = 0;

  if (pActwk->userflag.b.h) {

    pMstwk = &actwk[*(Sint32*)&pActwk->actfree[0]];
    pGawawk = &actwk[*(Sint32*)&pMstwk->actfree[12]];
    pGawawk->userflag.b.l = 0;
    pGawawk = &actwk[*(Sint32*)&pMstwk->actfree[16]];
    pGawawk->userflag.b.l = 0;
    pActwk->userflag.b.l = 0;
  }

}

static void spr_r_move4(sprite_status* pActwk) {
  Uint8 *pPattbl, *pRevtbl, *pPositbl;

  pPattbl = gpattbl;

  if (!pActwk->userflag.b.h) {
    pRevtbl = grevtbl0_0;
    pPositbl = gpositbl0_0;
  }
  else {
    pRevtbl = grevtbl1_0;

    if (!pActwk->userflag.b.l) {
      pPositbl = gpositbl1_0;
    } else {
      pPositbl = gpositbl2_0;
    }
  }

g_move0(pActwk, pPattbl, pRevtbl, pPositbl, (Uint8*)ppositblg_0);
}

static void spr_r_move5(sprite_status* pActwk) {
  Uint8 *pPattbl, *pRevtbl, *pPositbl;

  pPattbl = gpattbl;

  if (!pActwk->userflag.b.h) {
    pRevtbl = grevtbl0_1;
    pPositbl = gpositbl0_1;
  }
  else {
    pRevtbl = grevtbl1_1;

    if (!pActwk->userflag.b.l) {
      pPositbl = gpositbl1_1;
    } else {
      pPositbl = gpositbl2_1;
    }
  }

g_move0(pActwk, pPattbl, pRevtbl, pPositbl, (Uint8*)ppositblg_1);
}

static void spr_r_move6(sprite_status* pActwk) {
  sprite_status* pMstwk;

  if (!(--pActwk->actfree[8])) {
    pMstwk = &actwk[*(Sint32*)&pActwk->actfree[0]];
    pMstwk = &actwk[*(Sint32*)&pMstwk->actfree[0]];
    pMstwk->userflag.b.l = 2;
    pActwk->r_no0 = pActwk->r_no1;
    soundset(192);

    actwk[0].sproffset |= prio_sav << 8;
    prio_sav = 0;
  }

pActwk->xposi.w.h += pActwk->xspeed.w;
  pActwk->yposi.w.h += pActwk->yspeed.w;
}

void gun7(sprite_status* pActwk) {
  gun7_act_tbl[pActwk->r_no0 / 2](pActwk);
  actionsub(pActwk);
  frameout_s(pActwk);
}

static void gun7_init(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->actflg |= 4;
  pActwk->sproffset = 768;
  pActwk->patbase = spr_rpat;
  pActwk->sprhsize = 32;
  pActwk->sprvsize = 12;

  pActwk->patno = 2;
  if (!pActwk->userflag.b.h) {
    gun7_move0(pActwk);
  } else {
    pActwk->patno = 0;
    pActwk->yspeed.w = -2048;
    pActwk->r_no0 = 6;
    gun7_move2(pActwk);
  }
}

static void gun7_move0(sprite_status* pActwk) {
  gun7_coli(pActwk);
}

static void gun7_move1(sprite_status* pActwk) {
  if (pActwk->actfree[0]) {
    speedset2(pActwk);

    if (--pActwk->actfree[0]) return;
    pActwk->xspeed.w *= -1;

  }
  else {
    speedset2(pActwk);

    if (pActwk->xposi.w.h == ((Sint16*)pActwk)[24]) {
      pActwk->r_no0 -= 2;
    }
  }
}

static void gun7_move2(sprite_status* pActwk) {
  pActwk->yspeed.w += 64;
  speedset2(pActwk);
  if (scra_v_posit.w.h + 232 < pActwk->yposi.w.h) {
    frameout(pActwk);
  }
}

void hibana(sprite_status* pActwk) {
  hibana_act_tbl[pActwk->r_no0 / 2](pActwk);
  actionsub(pActwk);
}

static void hibana_init(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->actflg |= 4;
  pActwk->sproffset = 33592;
  pActwk->patbase = hibanapat;
}

static void hibana_move0(sprite_status* pActwk) {
  patchg(pActwk, hibanachg);
}

static void hibana_move1(sprite_status* pActwk) {
  frameout(pActwk);
}

static void spr_r_set(sprite_status* pActwk) {
  char* pRevtbl;
  Sint16* pPostbl;
  Sint16 patcntwk;
  sprite_status* pMstwk;
  static char tbl0[9] = { 2, 0, 2, 1, 2, 0, 2, 1, 2 };
  static sprite_pattern** tbl1[3] = {
    spring90pat2, springpat2, spring45pat2
  };

switch (pActwk->userflag.b.h) {
    case 0:
      pRevtbl = srevtbl0;
      pPostbl = spositbl0;
      break;

    case 1:
      pRevtbl = srevtbl1;
      pPostbl = spositbl1;
      break;

    default:
      pRevtbl = srevtbl2;
      pPostbl = spositbl2;
      break;
  }

pActwk->sproffset = 1312;
  pMstwk = &actwk[*(Sint32*)&pActwk->actfree[0]];
  patcntwk = pMstwk->patcnt;

  if (!(patcntwk & 1)) {
    pActwk->sproffset = 1266;
  }

pActwk->patbase = tbl1[tbl0[patcntwk]];
  pActwk->actflg &= 252;
  pActwk->actflg |= pRevtbl[patcntwk];
  pActwk->xposi.w.h = pMstwk->xposi.w.h + pPostbl[patcntwk * 2];
  pActwk->yposi.w.h = pMstwk->yposi.w.h + pPostbl[patcntwk * 2 + 1];
}

static void coli_spr(sprite_status* pActwk) {
  if (editmode.b.h) return;

if (coli0(pActwk, &actwk[0], 32, 32) != 0) {
    playset(pActwk, &actwk[0]);
  }
}

static Uint32 coli0(sprite_status* pActwk, sprite_status* pPlayerwk, Sint16 xOffs, Sint16 yOffs) {
  Sint16 sizwk, poswk;

  if (pPlayerwk->r_no0 == 4) {
    pPlayerwk->r_no0 -= 2;
    ((Sint16*)pPlayerwk)[26] = 120;
  }

if (pPlayerwk->mstno.b.h == 43 || pPlayerwk->r_no0 >= 6) {
    return 0;
  }

  sizwk = (Sint16)(char)pPlayerwk->sprhs + xOffs;
  poswk = pActwk->xposi.w.h - pPlayerwk->xposi.w.h;

  if ((poswk += sizwk) < 0 || poswk >= sizwk * 2) {
    return 0;
  }

  sizwk = (Sint16)(char)pPlayerwk->sprvsize + yOffs;
  poswk = pActwk->yposi.w.h - pPlayerwk->yposi.w.h;

  if ((poswk += sizwk) < 0 || poswk >= sizwk * 2) {
    return 0;
  }

  return 1;
}

static void playset(sprite_status* pActwk, sprite_status* pPlayerwk) {
  sprite_status* pMstwk;
  Sint16 cntwk;

  pPlayerwk->actfree[18] = 0;
  pPlayerwk->actfree[2] |= 1;
  pPlayerwk->cddat |= 4;
  pPlayerwk->sprvsize = 14;
  pPlayerwk->sprhs = 7;
  pPlayerwk->mstno.b.h = 2;

prio_sav = (pPlayerwk->sproffset & 32768) >> 8;
  pPlayerwk->sproffset &= 32767;

  *(Sint32*)&pActwk->actfree[4] = pPlayerwk - actwk;
  pActwk->actfree[8] = 60;
  pActwk->r_no0 += 2;

  pMstwk = &actwk[*(Sint32*)&pActwk->actfree[0]];
  cntwk = pposiset(pActwk, pMstwk, pPlayerwk, ppositbl0);

  plspdset(pActwk, pPlayerwk, cntwk);
}

static Sint16 pposiset(sprite_status* pActwk, sprite_status* pMstwk, sprite_status* pPlayerwk, char* pPostbl) {
  Sint16 poswk, cntsv;

  cntsv = pMstwk->patcnt;
  poswk = pPostbl[cntsv];
  poswk *= 2;

  pPlayerwk->xposi.w.h = pActwk->xposi.w.h + ppositbl[poswk];
  pPlayerwk->yposi.w.h = pActwk->yposi.w.h + ppositbl[poswk + 1];

  return cntsv;
}

static void plspdset(sprite_status* pActwk, sprite_status* pPlayerwk, Sint16 patcnt) {
  int_union sinwk, coswk;

  if (!(patcnt & 1)) {

    sinset(224, &sinwk.w.l, &coswk.w.l);
    sinwk.l <<= 12;
    coswk.l <<= 12;
    sinwk.l = (Uint32)sinwk.l >> 8;
    coswk.l = (Uint32)coswk.l >> 8;
    pPlayerwk->yspeed.w = sinwk.w.l;
    pPlayerwk->xspeed.w = coswk.w.l;

    if (pActwk->actflg & 2) {
      pPlayerwk->yspeed.w *= -1;
    }

pPlayerwk->cddat &= 254;

    if (pActwk->actflg & 1) {
      pPlayerwk->cddat |= 1;
      pPlayerwk->xspeed.w *= -1;
    }
  }
  else {
    if (!(patcnt & 2)) {

      pPlayerwk->yspeed.w = 0;
      pPlayerwk->xspeed.w = 4096;

      if (pActwk->actflg & 1) {
        pPlayerwk->xspeed.w *= -1;
      }
    }
    else {
      pPlayerwk->xspeed.w = 0;
      pPlayerwk->yspeed.w = -4096;

      if (pActwk->actflg & 2) {
        pPlayerwk->yspeed.w *= -1;
      }
    }
  }

pPlayerwk->cddat |= 2;
  pPlayerwk->cddat &= 247;
}

static void g_move0(sprite_status* pActwk, Uint8* pPatTbl, Uint8* pRevTbl, Uint8* pGposiTbl, Uint8* pPposiTbl) {
  sprite_status *pMstwk, *pMstwk2, *pPlayerwk;
  Uint8 cntwk;

  pMstwk = &actwk[*(Sint32*)&pActwk->actfree[0]];
  cntwk = pMstwk->patcnt;

  pActwk->patno = pPatTbl[cntwk];
  pActwk->actflg &= 252;
  pActwk->actflg |= pRevTbl[cntwk];
  cntwk = pGposiTbl[cntwk];
  cntwk *= 2;
  pActwk->xposi.w.h = pMstwk->xposi.w.h + gpositbl[cntwk];
  pActwk->yposi.w.h = pMstwk->yposi.w.h + gpositbl[cntwk + 1];

if (pActwk->userflag.b.h) {

pMstwk2 = &actwk[*(Sint32*)&pActwk->actfree[0]];
    pMstwk2 = &actwk[*(Sint32*)&pMstwk2->actfree[0]];

    if (pMstwk2->userflag.b.l) {
      if (pMstwk2->userflag.b.l == 2 && !pActwk->userflag.b.l) {
        pPlayerwk = &actwk[*(Sint32*)&pMstwk2->actfree[4]];
        pposiset(pMstwk2, pMstwk, pPlayerwk, (char*)pPposiTbl);
      }
    }
    else {
      if (!editmode.b.h) {
        if (coli0(pActwk, &actwk[0], 24, 24) != 0) {
          playset_g(pActwk, pMstwk, &actwk[0], pPposiTbl);
        } else {
          pActwk->userflag.b.l = 0;
        }
      }
    }
  }

}

static void playset_g(sprite_status* pActwk, sprite_status* pMstadr, sprite_status* pPlayerwk, Uint8* pPposiTbl) {
  sprite_status *pMstwk, *pGawaadr, *pMstsv;

  pPlayerwk->actfree[18] = 0;
  pPlayerwk->actfree[2] |= 1;
  pPlayerwk->cddat |= 4;
  pPlayerwk->sprvsize = 14;
  pPlayerwk->sprhs = 7;
  pPlayerwk->mstno.b.h = 2;

  pMstwk = &actwk[*(Sint32*)&pActwk->actfree[0]];
  pMstwk = &actwk[*(Sint32*)&pMstwk->actfree[0]];
  pMstwk->r_no0 += 2;
  pMstwk->userflag.b.l = 1;
  *(Sint32*)&pMstwk->actfree[4] = pPlayerwk - actwk;
  pposiset(pMstwk, pMstadr, pPlayerwk, (char*)pPposiTbl);

  pMstwk = &actwk[*(Sint32*)&pActwk->actfree[0]];

  if (pActwk->r_no0 == 10) {
    pGawaadr = &actwk[*(Sint32*)&pMstwk->actfree[16]];
  } else {
    pGawaadr = &actwk[*(Sint32*)&pMstwk->actfree[12]];
  }

  pGawaadr->r_no1 = pGawaadr->r_no0;
  pMstsv = pMstadr;
  pMstadr = &actwk[*(Sint32*)&pActwk->actfree[0]];
  pMstadr = &actwk[*(Sint32*)&pMstadr->actfree[0]];

  if (pGawaadr->r_no0 == 10) {
    pMstadr->userflag.b.h = 2;
    spr_r_move4(pGawaadr);
  }
  else {
    pMstadr->userflag.b.h = 1;
    spr_r_move5(pGawaadr);
  }

pGawaadr->r_no0 = 14;
  pGawaadr->userflag.b.l = 1;
  gspdset(pMstsv, pGawaadr);
}

static void gspdset(sprite_status* pMstadr, sprite_status* pGawaadr) {
  Sint16* pGspdTbl;
  Uint8 r_nowk, cntwk;

  pGspdTbl = gspdtbl0;
  r_nowk = pGawaadr->r_no1;

  if ((r_nowk -= 10) != 0) {
    pGspdTbl = gspdtbl1;
  }

cntwk = pMstadr->patcnt;
  cntwk *= 2;
  pGawaadr->xspeed.w = pGspdTbl[cntwk];
  pGawaadr->yspeed.w = pGspdTbl[cntwk + 1];
  pGawaadr->actfree[8] = 26;
}

static void gun7_coli(sprite_status* pActwk) {
  if (!editmode.b.h) {
    if (!(actwk[0].cddat & 2)) {
      if (coli0(pActwk, &actwk[0], 32, 16) != 0) {
        gun7set(pActwk);
      }
    }
  }

}

static void gun7set(sprite_status* pActwk) {
  sprite_status *pNewact, *pGunact;

  actwk[0].actfree[2] = 0;
  pNewact = pGunact = 0;

  if (actwkchk(&pNewact) == 0) {
    pNewact->actno = 8;
    pNewact->xposi.w.h = pActwk->xposi.w.h;
    pNewact->yposi.w.h = pActwk->yposi.w.h;
    pNewact->userflag.b.h = 1;
    pGunact = pNewact;

    if (actwkchk(&pNewact) == 0) {
      pNewact->actno = 9;
      pNewact->xposi.w.h = pActwk->xposi.w.h;
      pNewact->yposi.w.h = pActwk->yposi.w.h - 6;
    }
  }

soundset(206);

  pActwk->r_no0 += 2;
  ((Sint16*)pActwk)[24] = pActwk->xposi.w.h;
  pActwk->actfree[0] = 6;

  if (pActwk->xposi.w.h < actwk[0].xposi.w.h) {

    if (pGunact != 0) {
      pGunact->xspeed.w = 512;
    }

    if (pNewact != 0) {
      pNewact->cddat |= 1;
      pNewact->xposi.w.h += 16;
    }
    actwk[0].xspeed.w = -4096;
    actwk[0].mspeed.w = actwk[0].xspeed.w;
    actwk[0].cddat |= 1;

    pActwk->xspeed.w = 2048;
  }
  else {
    if (pGunact != 0) {
      pGunact->xspeed.w = -512;
    }

    if (pNewact != 0) {
      pNewact->cddat &= 254;
      pNewact->xposi.w.h -= 16;
    }

    actwk[0].xspeed.w = 4096;
    actwk[0].mspeed.w = actwk[0].xspeed.w;
    actwk[0].cddat &= 254;

    pActwk->xspeed.w = -2048;
  }
}
