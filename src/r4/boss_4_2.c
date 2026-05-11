#include "../equ.h"
#include "boss_4_2.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"
#include "../goal.h"
#include "../io.h"
#include "../loader2.h"
#include "../score.h"
#include "playsub4.h"

typedef struct {
  char xpos;
  char ypos;
  Sint32 xspd;
  Sint32 yspd;
}
tama;

static void egg4_warai_chk(sprite_status* pActwk);
static void egg4_warai(sprite_status* pActwk);
static Uint32 egg4_ini(sprite_status* pActwk);
static void make_egg4meca(sprite_status* pActwk);
static Uint32 egg4_scrset(sprite_status* pActwk);
static Uint32 egg4_scrreset(sprite_status* pActwk);
static Uint32 egg4_awademo1(sprite_status* pActwk);
static void egg4_posiini(sprite_status* pActwk);
static void make_awa(sprite_status* pActwk, sprite_status** pNewact);
static void make_awa2(sprite_status* pActwk, sprite_status** pNewact);
static Uint32 egg4_awademo2(sprite_status* pActwk);
static Uint32 egg4_awademo3(sprite_status* pActwk);
static Uint32 egg4_movel(sprite_status* pActwk);
static void egg4_hitchk(sprite_status* pActwk);
static void make_tama(sprite_status* pActwk);
static void egg4_posiset(sprite_status* pActwk);
static Uint32 egg4_mover(sprite_status* pActwk);
static Uint32 egg4_wait(sprite_status* pActwk);
static Uint32 egg4_die(sprite_status* pActwk);
static void bom_set(sprite_status* pActwk);
static Uint32 egg4_esc(sprite_status* pActwk);
static Uint32 egg4_movec(sprite_status* pActwk);
static Uint32 egg4_movec2(sprite_status* pActwk);
static Uint32 egg4_movec3(sprite_status* pActwk);
static void egg4meca_ini(sprite_status* pActwk);
static void egg4meca_01(sprite_status* pActwk);
static Uint32 egg4awa_ini(sprite_status* pActwk);
static Uint32 egg4awa_deru(sprite_status* pActwk);
static Uint32 egg4awa_tuku(sprite_status* pActwk);
static Uint32 egg4awa_hiro(sprite_status* pActwk);
static void awa_hitchk(sprite_status* pActwk);
static Uint32 egg4awa_roll(sprite_status* pActwk);
static Uint32 egg4awa_ychg(sprite_status* pActwk);
static void ychg_ret(sprite_status* pActwk, sprite_status* pEggwk);
static void ychg_rad_endproc(sprite_status* pActwk);
static Uint32 egg4awa_del(sprite_status* pActwk);
static Uint32 egg4awa_out(sprite_status* pActwk);
static Uint32 egg4awa_chi(sprite_status* pActwk);
static Uint32 egg4tama_ini(sprite_status* pActwk);
static Uint32 egg4tama_01(sprite_status* pActwk);
static Uint32 egg4tama_02(sprite_status* pActwk);
static Uint32 egg4tama_kill(sprite_status* pActwk);
static Uint32 frameout_chk(sprite_status* pActwk);

static sprite_pattern egg4_pat0 = {
  1,
  { { -32, -28, 0, 452 } }
};
static sprite_pattern egg4_pat1 = {
  1,
  { { -32, -28, 0, 453 } }
};
static sprite_pattern egg4_pat2 = {
  1,
  { { -32, -28, 0, 454 } }
};
static sprite_pattern egg4_pat3 = {
  1,
  { { -32, -28, 0, 455 } }
};
static sprite_pattern egg4_pat4 = {
  1,
  { { -32, -28, 0, 456 } }
};
static sprite_pattern egg4_pat5 = {
  1,
  { { -32, -44, 0, 457 } }
};
static sprite_pattern egg4_pat6 = {
  1,
  { { -32, -44, 0, 458 } }
};
sprite_pattern* egg4_pat[7] = {
  &egg4_pat0,
  &egg4_pat1,
  &egg4_pat2,
  &egg4_pat3,
  &egg4_pat4,
  &egg4_pat5,
  &egg4_pat6
};
static Uint8 egg4_pchg0[3] = { 255, 0, 255 };
static Uint8 egg4_pchg1[4] = { 7, 1, 2, 255 };
static Uint8 egg4_pchg2[3] = { 255, 3, 255 };
static Uint8 egg4_pchg3[6] = { 3, 5, 4, 6, 4, 255 };
static Uint8* egg4_pchg[4] = {
  egg4_pchg0,
  egg4_pchg1,
  egg4_pchg2,
  egg4_pchg3
};
static Uint32(*egg4_act_tbl[14])(sprite_status*) = {
  &egg4_ini,
  &egg4_scrset,
  &egg4_scrreset,
  &egg4_awademo1,
  &egg4_awademo2,
  &egg4_awademo3,
  &egg4_movel,
  &egg4_mover,
  &egg4_wait,
  &egg4_die,
  &egg4_esc,
  &egg4_movec,
  &egg4_movec2,
  &egg4_movec3
};
extern Uint16 scr_dir_tbl[];

void egg4(sprite_status* pActwk) {
  pActwk->actfree[2] &= 191;
  if (!pActwk->actfree[1]) {

    egg4_warai_chk(pActwk);
  } else if (--pActwk->actfree[1] == 0) {
    pActwk->mstno.b.h = 0;
    pActwk->patno = 0;
    pActwk->patcnt = 0;
    pActwk->pattim = 0;
    pActwk->pattimm = 0;
  }

if (egg4_act_tbl[pActwk->r_no0 / 2](pActwk) != 0) {
    patchg(pActwk, egg4_pchg);
    actionsub(pActwk);
  }
}

static void egg4_warai_chk(sprite_status* pActwk) {
  if (!pActwk->mstno.b.h) {

    if (((Sint16*)&actwk[0])[26] || actwk[0].r_no0 == 6) {

egg4_warai(pActwk);
    }
  }
}

static void egg4_warai(sprite_status* pActwk) {
  pActwk->actfree[1] = 120;
  pActwk->mstno.b.h = 1;
  pActwk->patno = 0;
  pActwk->patcnt = 0;
  pActwk->pattim = 0;
  pActwk->pattimm = 0;
}

static Uint32 egg4_ini(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->actflg = 4;
  pActwk->sprpri = 4;
  pActwk->sprhsize = 32;
  pActwk->sprvsize = 44;
  pActwk->sproffset = 798;
  pActwk->patbase = egg4_pat;
  pActwk->colicnt = 2;

  make_egg4meca(pActwk);

  return 1;
}

static void make_egg4meca(sprite_status* pActwk) {
  sprite_status* pNewact;

  if (actwkchk(&pNewact) == 0) {
    ((Sint16*)pActwk)[26] = pNewact - actwk;
    ((Sint16*)pNewact)[25] = pActwk - actwk;
    pNewact->actno = 77;
    pNewact->xposi.w.h = pActwk->xposi.w.h;
    pNewact->yposi.w.h = pActwk->yposi.w.h;
  }
}

static Uint32 egg4_scrset(sprite_status* pActwk) {
  Sint16 xwk;

  scralim_right = scralim_n_right = 2480;

  if ((xwk = actwk[0].xposi.w.h - 160) >= scralim_left) {
    if (actwk[0].xposi.w.h >= 2640) {

      sub_sync(103);

      pActwk->r_no0 = 6;
      scralim_right = scralim_n_right = 2480;
      xwk = 2480;
    }

scralim_left = scralim_n_left = xwk;
  }

return 1;
}

static Uint32 egg4_scrreset(sprite_status* pActwk) {
  Sint16 *pScrdir, sLeft, sRight, sUp, sDown, BgmNo;

  pScrdir = (Sint16*)&scr_dir_tbl[1];
  sLeft = *pScrdir++;
  sRight = *pScrdir++;
  sUp = *pScrdir++;
  sDown = *pScrdir++;

  scralim_right += 6;
  scralim_n_right += 6;

  if (scralim_right < sRight) {
    return 0;
  }

bossstart = 0;
  scralim_right = scralim_n_right = sRight;

  if (generate_flag) {
    BgmNo = 22;
  } else {
    BgmNo = 23;
  }

  sub_sync(BgmNo);
  genecolor();
  frameout(pActwk);

  return 0;
}

static Uint32 egg4_awademo1(sprite_status* pActwk) {
  sprite_status* pNewact;

  egg4_posiini(pActwk);
  ++pActwk->actfree[0];

  if (pActwk->actfree[0] == 10) {
    make_awa2(pActwk, &pNewact);
  }

if (pActwk->actfree[0] == 20 || pActwk->actfree[0] >= 30) {

soundset(182);

    pActwk->actfree[0] = 0;
    make_awa(pActwk, &pNewact);

    pNewact->actfree[3] = pActwk->actfree[3]++;

    if (pActwk->actfree[3] == 16) {
      pActwk->r_no0 = 8;
    }
  }

  return 1;
}

static void egg4_posiini(sprite_status* pActwk) {
  if (pActwk->yposi.w.h >= 1312) {

    pActwk->xposi.w.h = 2736;
    pActwk->xposi.w.l = 0;
    pActwk->yposi.w.h = 1312;
    pActwk->yposi.w.l = 0;
  } else {
    pActwk->yposi.l += 18000;
  }
}

static void make_awa(sprite_status* pActwk, sprite_status** pNewact) {
  sprite_status* pMakeact;

  if (actwkchk(&pMakeact) == 0) {
    *pNewact = pMakeact;
    ((Sint16*)pMakeact)[25] = pActwk - actwk;
    pMakeact->actno = 79;
    pMakeact->xposi.w.h = pActwk->xposi.w.h;
    pMakeact->yposi.w.h = 1464;
  }
}

static void make_awa2(sprite_status* pActwk, sprite_status** pNewact) {
  make_awa(pActwk, pNewact);
  (*pNewact)->r_no1 = 1;
}

static Uint32 egg4_awademo2(sprite_status* pActwk) {
  egg4_posiini(pActwk);

  if (++pActwk->actfree[0] == 150) {
    pActwk->actfree[0] = 0;
    pActwk->r_no0 = 10;
  }

  return 1;
}

static Uint32 egg4_awademo3(sprite_status* pActwk) {
  if (++pActwk->actfree[0] == 150) {
    pActwk->r_no0 = 12;
    ((Sint16*)pActwk)[33] = 96;
    pActwk->yspeed.w = 64;
    ((Sint16*)pActwk)[28] = 0;
    ((Sint16*)pActwk)[30] = 96;
    ((Sint16*)pActwk)[31] = 0;
    ((Sint16*)pActwk)[23] = 0;
  }

  return 1;
}

static Uint32 egg4_movel(sprite_status* pActwk) {
  egg4_hitchk(pActwk);

  ((Sint16*)pActwk)[28] += 256;
  if (((Sint16*)pActwk)[28] == 2048) {
    make_tama(pActwk);
  }

if (((Uint16*)pActwk)[28] == 32768) {

    pActwk->actfree[0] = 0;
    pActwk->r_no0 = 16;
    pActwk->actfree[2] |= 16;
  } else {
    pActwk->xposi.w.h -= ((Sint16*)pActwk)[30];
    pActwk->yposi.w.h -= ((Sint16*)pActwk)[31];
    egg4_posiset(pActwk);
  }

  return 1;
}

static void egg4_hitchk(sprite_status* pActwk) {
  if (pActwk->colino) return;

pActwk->colino = 60;
  if (pActwk->colicnt != 1) return;

pActwk->actfree[0] = 0;
  pActwk->r_no0 = 18;
  pActwk->actfree[2] |= 128;
  pActwk->mstno.b.h = 2;
  pActwk->patno = pActwk->patcnt = pActwk->pattim = pActwk->pattimm = 0;
  egg4_die(pActwk);
}

static void make_tama(sprite_status* pActwk) {
  Sint32 i;
  sprite_status* pNewact;
  Sint16 xposwk;
  Sint32 xspdwk;
  static tama tama_tbl[4] = {
    { -29, 23, -81000,  81000 },
    {  -6, 31,      0, 114688 },
    {  16, 28,  57344,  99319 },
    {  29, 17,  81000,  81000 }
  };

for (i = 0; i < 4; ++i) {
    if (actwkchk(&pNewact) != 0) break;

((Sint16*)pNewact)[25] = pActwk - actwk;
    pNewact->actno = 78;
    pNewact->xposi.w.h = pActwk->xposi.w.h;
    pNewact->yposi.w.h = pActwk->yposi.w.h;

    xposwk = tama_tbl[i].xpos;
    xspdwk = tama_tbl[i].xspd;

    if (pActwk->cddat & 1) {

      xposwk = -xposwk;
      xspdwk = -xspdwk;
    }

((Sint16*)pNewact)[30] = xposwk;
    ((Sint16*)pNewact)[31] = tama_tbl[i].ypos;
    ((Sint32*)pNewact)[16] = xspdwk;
    ((Sint32*)pNewact)[4] = tama_tbl[i].yspd;
  }
}

static void egg4_posiset(sprite_status* pActwk) {
  Sint16 sinwk, coswk;
  int_union xwk, ywk;

  sinset(pActwk->actfree[11], &sinwk, &coswk);
  xwk.l = ((Sint16*)pActwk)[33];
  ywk.l = pActwk->yspeed.w;
  ywk.l *= sinwk;
  xwk.l *= coswk;
  ywk.l >>= 8;
  xwk.l >>= 8;

  ((Sint16*)pActwk)[30] = xwk.w.l;
  ((Sint16*)pActwk)[31] = ywk.w.l;
  pActwk->xposi.w.h += xwk.w.l;
  pActwk->yposi.w.h += ywk.w.l;
}

static Uint32 egg4_mover(sprite_status* pActwk) {
  egg4_hitchk(pActwk);

  ((Sint16*)pActwk)[28] -= 256;
  if (((Sint16*)pActwk)[28] == 30720) {
    make_tama(pActwk);
  }

if (((Sint16*)pActwk)[28] == 0) {

    pActwk->actfree[0] = 0;
    pActwk->r_no0 = 16;
    pActwk->actfree[2] |= 16;
  } else {
    pActwk->xposi.w.h -= ((Sint16*)pActwk)[30];
    pActwk->yposi.w.h -= ((Sint16*)pActwk)[31];
    egg4_posiset(pActwk);
  }

  return 1;
}

static Uint32 egg4_wait(sprite_status* pActwk) {
  char awaposi_cnt_tbl[8] = {
    14, 12, 10, 8, 6, 4, -1, -1
  };
  char cnt;
  sprite_status* pChildact;

  pActwk->actfree[2] &= 207;
  egg4_hitchk(pActwk);
  ++pActwk->actfree[0];

  if ((Sint32)pActwk->actfree[0] == 48) {

    pChildact = &actwk[((Sint16*)pActwk)[26]];
    pChildact->actflg ^= 1;
    pChildact->cddat ^= 1;
    pActwk->actflg ^= 1;
    pActwk->cddat ^= 1;
  } else if (pActwk->actfree[0] == 96) {

    pActwk->actfree[0] = 0;
    if (pActwk->cddat & 1) {

      pActwk->actfree[2] |= 8;
      pActwk->r_no0 = 14;
    }
    else {
      pActwk->actfree[2] &= 247;
      pActwk->r_no0 = 12;
    }

while ((cnt = awaposi_cnt_tbl[pActwk->actfree[9]]) >= 0) {
      if (pActwk->actfree[3] > cnt) break;

++pActwk->actfree[9];
      pActwk->r_no0 = 22;
    }
  }

  return 1;
}

static Uint32 egg4_die(sprite_status* pActwk) {
  ++pActwk->actfree[0];

  if (pActwk->actfree[0] < 60) {
    bom_set(pActwk);
  } else if (pActwk->actfree[0] == 60) {

    pActwk->mstno.b.h = 3;
    pActwk->actfree[1] = 255;
    pActwk->patno = pActwk->patcnt = pActwk->pattim = pActwk->pattimm = 0;

    pActwk->patno = 1;
  } else if (pActwk->actfree[0] == 61) {

    pActwk->actfree[0] = 0;
    pActwk->r_no0 = 20;
    pActwk->actflg |= 1;
    pActwk->cddat |= 1;
    actwk[((Sint16*)pActwk)[26]].actflg |= 1;
    actwk[((Sint16*)pActwk)[26]].cddat |= 1;
    scoreup(100);
  }

  return 1;
}

static void bom_set(sprite_status* pActwk) {
  int_union tmwk;
  Sint32 wk;
  Sint16 bom_tbl[20] = {
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
  sprite_status* pNewact;

  tmwk.l = 0;
  tmwk.b.b4 = pActwk->actfree[0];
  wk = tmwk.l;
  tmwk.w.l = wk % 4;
  tmwk.w.h = wk / 4;
  if (tmwk.w.l == 0) {
    tmwk.l = (Uint32)tmwk.l >> 16 & 65535 | tmwk.l << 16 & -65536;
    tmwk.l %= 10;
    tmwk.l <<= 1;
    if (actwkchk(&pNewact) == 0) {
      pNewact->r_no1 = 1;
      pNewact->actno = 24;
      pNewact->xposi.w.h = pActwk->xposi.w.h;
      pNewact->yposi.w.h = pActwk->yposi.w.h;
      pNewact->xposi.w.h += bom_tbl[tmwk.l++];
      pNewact->yposi.w.h += bom_tbl[tmwk.l];
      soundset(158);
    }
  }
}

static Uint32 egg4_esc(sprite_status* pActwk) {
  if (++pActwk->actfree[0] < 60) {
    return 1;
  }

  pActwk->xposi.l += 114688;
  if (pActwk->xposi.w.h >= 2864) {

    pActwk->r_no0 = 4;
    frameout(&actwk[((Sint16*)pActwk)[26]]);
  }

  return 1;
}

static Uint32 egg4_movec(sprite_status* pActwk) {
  int_union offswk;

  ++pActwk->actfree[0];

  switch (pActwk->actfree[0]) {

    case 20:
      egg4_warai(pActwk);
      pActwk->actfree[8] = 0;
      pActwk->actfree[2] |= 32;
      break;

case 21:
      pActwk->actfree[2] &= 223;
      break;

case 45:
      make_tama(pActwk);
      break;
  }

if (pActwk->actfree[2] & 8) {

    pActwk->xposi.l += 65536;
    if (pActwk->xposi.w.h >= 2736) goto label1;
    return 1;
  }

pActwk->xposi.l -= 65536;
  if (pActwk->xposi.w.h <= 2544) goto label1;
  return 1;

label1:
  ((Sint32*)pActwk)[15] *= -1;
  ((Uint16*)pActwk)[28] += 32768;
  pActwk->xposi.w.h = 2640;
  pActwk->xposi.w.l = 0;

  offswk.w.h = ((Sint16*)pActwk)[30];
  offswk.w.l = ((Sint16*)pActwk)[31];
  pActwk->xposi.l += offswk.l;

  pActwk->yposi.w.h = 1312;
  pActwk->yposi.w.l = 0;

  pActwk->actfree[0] = 0;
  pActwk->r_no0 = 16;
  pActwk->actfree[2] |= 16;

  return 1;
}

static Uint32 egg4_movec2(sprite_status* pActwk) {
  pActwk = pActwk;
  return 1;
}

static Uint32 egg4_movec3(sprite_status* pActwk) {
  pActwk = pActwk;
  return 1;
}

static sprite_pattern egg4meca_pat0 = {
  1,
  { { -32, -28, 0, 459 } }
};
static sprite_pattern egg4meca_pat1 = {
  1,
  { { -32, -28, 0, 460 } }
};
sprite_pattern* egg4meca_pat[2] = {
  &egg4meca_pat0,
  &egg4meca_pat1
};
static void(*egg4meca_act_tbl[2])(sprite_status*) = {
  &egg4meca_ini,
  &egg4meca_01
};
static Uint8 egg4awa_pchg0[7] = { 9, 0, 1, 2, 3, 4, 255 };
static Uint8 egg4awa_pchg1[3] = { 255, 4, 255 };
static Uint8 egg4awa_pchg2[5] = { 19, 4, 5, 6, 255 };
static Uint8 egg4awa_pchg3[3] = { 255, 6, 255 };
static Uint8 egg4awa_pchg4[4] = { 19, 7, 8, 252 };
static Uint8* egg4awa_pchg[5] = {
  egg4awa_pchg0,
  egg4awa_pchg1,
  egg4awa_pchg2,
  egg4awa_pchg3,
  egg4awa_pchg4
};
static sprite_pattern egg4awa_pat0 = {
  1,
  { { -4, -4, 0, 463 } }
};
static sprite_pattern egg4awa_pat1 = {
  1,
  { { -4, -4, 0, 464 } }
};
static sprite_pattern egg4awa_pat2 = {
  1,
  { { -4, -4, 0, 465 } }
};
static sprite_pattern egg4awa_pat3 = {
  1,
  { { -8, -8, 0, 466 } }
};
static sprite_pattern egg4awa_pat4 = {
  1,
  { { -8, -8, 0, 467 } }
};
static sprite_pattern egg4awa_pat5 = {
  1,
  { { -12, -12, 0, 468 } }
};
static sprite_pattern egg4awa_pat6 = {
  1,
  { { -16, -16, 0, 469 } }
};
static sprite_pattern egg4awa_pat7 = {
  1,
  { { -16, -16, 0, 470 } }
};
static sprite_pattern egg4awa_pat8 = {
  1,
  { { -16, -16, 0, 471 } }
};
sprite_pattern* egg4awa_pat[9] = {
  &egg4awa_pat0,
  &egg4awa_pat1,
  &egg4awa_pat2,
  &egg4awa_pat3,
  &egg4awa_pat4,
  &egg4awa_pat5,
  &egg4awa_pat6,
  &egg4awa_pat7,
  &egg4awa_pat8
};
static Uint32(*egg4awa_act_tbl[9])(sprite_status*) = {
  &egg4awa_ini,
  &egg4awa_deru,
  &egg4awa_tuku,
  &egg4awa_hiro,
  &egg4awa_roll,
  &egg4awa_ychg,
  &egg4awa_del,
  &egg4awa_out,
  &egg4awa_chi
};

void egg4meca(sprite_status* pActwk) {
  egg4meca_act_tbl[pActwk->r_no0 / 2](pActwk);
  actionsub(pActwk);
}

static void egg4meca_ini(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->actflg = 4;
  pActwk->sprpri = 4;
  pActwk->sprhsize = 32;
  pActwk->sprvsize = 28;
  pActwk->sproffset = 798;
  pActwk->patbase = egg4meca_pat;

  egg4meca_01(pActwk);
}

static void egg4meca_01(sprite_status* pActwk) {
  sprite_status* pEggact;

  pEggact = &actwk[((Sint16*)pActwk)[25]];
  pActwk->xposi.w.h = pEggact->xposi.w.h;
  pActwk->yposi.w.h = pEggact->yposi.w.h;
}

void egg4awa(sprite_status* pActwk) {
  if (egg4awa_act_tbl[pActwk->r_no0 / 2](pActwk) != 0) {
    actionsub(pActwk);
  }
}

static Uint32 egg4awa_ini(sprite_status* pActwk) {
  int_union RndNum;
  uint_union uRndNum;

  pActwk->r_no0 = 2;
  pActwk->actflg = 4;
  pActwk->sprpri = 3;
  pActwk->sprhsize = 16;
  pActwk->sprvsize = 16;
  pActwk->sproffset = 798;
  pActwk->patbase = egg4awa_pat;

  RndNum.l = random();
  RndNum.l = RndNum.w.l;
  RndNum.w.l = RndNum.l % 1280;
  if (RndNum.w.l < 0) {

    RndNum.w.l -= 128;
  } else {
    RndNum.w.l += 128;
  }

((Sint16*)pActwk)[29] = RndNum.w.l;

  uRndNum.l = random();
  uRndNum.w.h = 0;
  uRndNum.w.l = (Sint16)(uRndNum.l % 16 + 10);
  ((Sint16*)pActwk)[33] = uRndNum.w.l;

  egg4awa_deru(pActwk);
}

static Uint32 egg4awa_deru(sprite_status* pActwk) {
  sprite_status* pEggact;
  Sint16 xwk;

  pActwk->yposi.l += -98304;
  if (pActwk->r_no1) {

    if (pActwk->yposi.w.h < 1136) {
      return egg4awa_out(pActwk);
    }
  }
  else {
    pEggact = &actwk[((Sint16*)pActwk)[25]];
    if (pEggact->yposi.w.h >= pActwk->yposi.w.h) {

      if ((xwk = pActwk->xposi.w.h - pEggact->xposi.w.h) >= 0) {

        ((Uint16*)pActwk)[28] = 32768;
      } else {
        ((Uint16*)pActwk)[28] = 0;
      }

((Sint16*)pActwk)[33] = xwk;
      pActwk->yspeed.w = xwk;
      ((Sint16*)pActwk)[30] = -xwk;
      ((Sint16*)pActwk)[31] = 0;
      pActwk->xposi.w.h = pEggact->xposi.w.h;
      pActwk->yposi.w.h = pEggact->yposi.w.h;
      pActwk->r_no0 = 4;
      pActwk->mstno.b.h = 2;
      return 1;
    }
  }

((Sint16*)pActwk)[28] += ((Sint16*)pActwk)[29];

pActwk->xposi.w.h -= ((Sint16*)pActwk)[30];
  egg4_posiset(pActwk);

  if (pActwk->patcnt == 4) {
    pActwk->mstno.b.h = 1;
    pActwk->patcnt = 0;
  }

patchg(pActwk, egg4awa_pchg);
  return 1;
}

static Uint32 egg4awa_tuku(sprite_status* pActwk) {
  sprite_status* pEggact;
  Sint16 RadSpd;

  pEggact = &actwk[((Sint16*)pActwk)[25]];
  pActwk->xposi.w.h = pEggact->xposi.w.h;
  pActwk->yposi.w.h = pEggact->yposi.w.h;
  egg4_posiset(pActwk);

  if ((RadSpd = ((Sint16*)pActwk)[29]) >= 0) {
    RadSpd *= -1;
  }

((Sint16*)pActwk)[28] += RadSpd;
  if (pEggact->r_no0 == 10) {
    ((Sint16*)pActwk)[30] = 0;
    ((Sint16*)pActwk)[31] = 0;
    pActwk->r_no0 = 6;
    pActwk->xposi.l = pEggact->xposi.l;
    pActwk->yposi.l = pEggact->yposi.l;

RadSpd = (char)pActwk->actfree[3];
    RadSpd = (Uint16)RadSpd << 12;
    ((Sint16*)pActwk)[28] = RadSpd;
    ((Sint16*)pActwk)[33] = 16;
    pActwk->yspeed.w = 16;
  }

  return 1;
}

static Uint32 egg4awa_hiro(sprite_status* pActwk) {
  Sint32 flg;
  sprite_status* pEggact;

  if (pActwk->actfree[2] & 2) {
    awa_hitchk(pActwk);
  }

flg = 0;

  pEggact = &actwk[((Sint16*)pActwk)[25]];
  pActwk->xposi.l = pEggact->xposi.l;
  pActwk->yposi.l = pEggact->yposi.l;

  if ((Sint32)((Sint16*)pActwk)[33] < 48) {
    ((Sint32*)pActwk)[16] += 16384;
  }
  else {
    ((Sint16*)pActwk)[33] = 48;
    ++flg;
  }

if ((Sint32)pActwk->yspeed.w < 48) {
    ((Sint32*)pActwk)[4] += 16384;
  }
  else {
    pActwk->yspeed.w = 48;
    ++flg;
  }

if (pEggact->actfree[2] & 8) {

    ((Sint16*)pActwk)[28] += 1152;
  }
  else {
    ((Sint16*)pActwk)[28] -= 1152;
  }

egg4_posiset(pActwk);

  if (pActwk->patcnt == 2) {
    pActwk->mstno.b.h = 3;
    pActwk->patcnt = 0;
  }

if (flg == 2) {
    pActwk->r_no0 = 8;
    flg = pActwk->actfree[2];

    if (!(pActwk->actfree[2] & 2)) {
      pActwk->colino = 255;
      pActwk->colicnt = 2;
    }

    pActwk->actfree[2] |= 2;
  }

patchg(pActwk, egg4awa_pchg);

  return 1;
}

static void awa_hitchk(sprite_status* pActwk) {
  sprite_status* pEggact;
  Uint8 flgwk;

  pEggact = &actwk[((Sint16*)pActwk)[25]];
  if (pEggact->r_no0 != 20) {
    if (pActwk->colino) return;

flgwk = pEggact->actfree[2];
    pEggact->actfree[2] |= 64;
    if (flgwk & 64) {

      pActwk->colino = 255;
      pActwk->colicnt = 2;
      return;
    }
  }

pActwk->r_no0 = 12;
  pActwk->mstno.b.h = 4;
  pActwk->patno = pActwk->patcnt = pActwk->pattim = pActwk->pattimm = 0;
  --pEggact->actfree[3];
  egg4awa_del(pActwk);
}

static Uint32 egg4awa_roll(sprite_status* pActwk) {
  sprite_status* pEggact;

  pEggact = &actwk[((Sint16*)pActwk)[25]];
  pActwk->xposi.l = pEggact->xposi.l;
  pActwk->yposi.l = pEggact->yposi.l;

  if (pEggact->actfree[2] & 16) {
    pActwk->r_no0 = 10;
    return egg4awa_ychg(pActwk);
  }

if (pEggact->actfree[2] & 32) {
    pActwk->r_no0 = 16;
    pActwk->sprpri = 3;
    return egg4awa_chi(pActwk);
  }

awa_hitchk(pActwk);
  if (pEggact->actfree[2] & 8) {

    ((Sint16*)pActwk)[28] += 384;
  }
  else {
    ((Sint16*)pActwk)[28] -= 384;
  }

egg4_posiset(pActwk);
  return 1;
}

static Uint32 egg4awa_ychg(sprite_status* pActwk) {
  sprite_status* pEggact;

  awa_hitchk(pActwk);

  pEggact = &actwk[((Sint16*)pActwk)[25]];
  pActwk->xposi.l = pEggact->xposi.l;
  pActwk->yposi.l = pEggact->yposi.l;

  if (pActwk->r_no1) {

    ((Sint32*)pActwk)[16] -= 65536;
    if (((Sint32*)pActwk)[16] > -3145728) {
      ychg_ret(pActwk, pEggact);
    }
    else {
      ((Sint32*)pActwk)[16] = 3145728;
      pActwk->r_no0 = 8;

ychg_rad_endproc(pActwk);
      ychg_ret(pActwk, pEggact);
      pActwk->r_no1 = 0;
    }
  }
  else {
    if ((((Sint32*)pActwk)[16] -= 65536) < 0) {

      ++pActwk->r_no1;
    }

    ychg_ret(pActwk, pEggact);
  }

  return 1;
}

static void ychg_ret(sprite_status* pActwk, sprite_status* pEggwk) {
  Sint16 spdwk;
  Uint8 radwk;

  spdwk = 1152;

  if (!(pEggwk->actfree[2]) & 8) {
    spdwk *= -1;
  }

((Sint16*)pActwk)[28] += spdwk;

  radwk = pActwk->actfree[11] - 64;
  if (pEggwk->actfree[2] & 8) {
    radwk ^= 128;
  }

if (radwk & 128) {

    pActwk->sprpri = 5;
  }
  else {
    pActwk->sprpri = 3;
  }

  egg4_posiset(pActwk);
}

static void ychg_rad_endproc(sprite_status* pActwk) {
  Uint16 radwk;

  radwk = ((Uint16*)pActwk)[28];

  if (radwk < 16384) {

    ((Uint16*)pActwk)[28] = 32768 - radwk;

}
  else {
    ((Uint16*)pActwk)[28] = 32768 - radwk;
  }
}

static Uint32 egg4awa_del(sprite_status* pActwk) {
  patchg(pActwk, egg4awa_pchg);

  return 1;
}

static Uint32 egg4awa_out(sprite_status* pActwk) {
  frameout(pActwk);

  return 0;
}

static Uint32 egg4awa_chi(sprite_status* pActwk) {
  sprite_status* pEggact;
  Sint16 cntwk, radwk;
  static Sint16 rad_tbl[17] = {
    0,
    0,
    -32768,
    21845,
    16384,
    13107,
    10922,
    9362,
    8192,
    7281,
    6553,
    5957,
    5461,
    5041,
    4681,
    4369,
    4096
  };

  awa_hitchk(pActwk);

  pEggact = &actwk[((Sint16*)pActwk)[25]];
  pActwk->xposi.l = pEggact->xposi.l;
  pActwk->yposi.l = pEggact->yposi.l;
  ((Sint32*)pActwk)[16] -= 65536;
  ((Sint32*)pActwk)[4] -= 65536;

  if (((Sint32*)pActwk)[16] <= 0x100000) {

    cntwk = pEggact->actfree[3];
    radwk = rad_tbl[cntwk];
    radwk *= pEggact->actfree[8];
    ((Sint16*)pActwk)[28] = radwk;

    ++pEggact->actfree[8];

    ((Sint32*)pActwk)[16] = 0x100000;
    ((Sint32*)pActwk)[4] = 0x100000;
    pActwk->r_no0 = 6;
  }

awa_hitchk(pActwk);
  if (pEggact->actfree[2] & 8) {

    ((Sint16*)pActwk)[28] += 1152;
  }
  else {
    ((Sint16*)pActwk)[28] -= 1152;
  }

egg4_posiset(pActwk);

  return 1;
}

static sprite_pattern egg4tama_pat0 = {
  1,
  { { -4, -4, 0, 461 } }
};
static sprite_pattern egg4tama_pat1 = {
  1,
  { { -4, -4, 0, 462 } }
};
sprite_pattern* egg4tama_pat[2] = {
  &egg4tama_pat0,
  &egg4tama_pat1
};
static Uint8 egg4tama_pchg0[4] = { 1, 0, 1, 255 };
static Uint8* egg4tama_pchg[1] = { egg4tama_pchg0 };
static Uint32(*egg4tama_act_tbl[3])(sprite_status*) = {
  &egg4tama_ini,
  &egg4tama_01,
  &egg4tama_02
};

void egg4tama(sprite_status* pActwk) {
  if (egg4tama_act_tbl[pActwk->r_no0 / 2](pActwk) != 0) {
    patchg(pActwk, egg4tama_pchg);
    actionsub(pActwk);
  }
}

static Uint32 egg4tama_ini(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->actflg = 4;
  pActwk->sprpri = 6;
  pActwk->sprhsize = 4;
  pActwk->sprvsize = 4;
  pActwk->sproffset = 798;
  pActwk->patbase = egg4tama_pat;

  egg4tama_01(pActwk);
}

static Uint32 egg4tama_01(sprite_status* pActwk) {
  sprite_status* pEggact;

  pEggact = &actwk[((Sint16*)pActwk)[25]];
  if (pEggact->actfree[2] & 128) {
    return egg4tama_kill(pActwk);
  }

  pActwk->xposi.w.h = pEggact->xposi.w.h;
  pActwk->yposi.w.h = pEggact->yposi.w.h;
  pActwk->xposi.w.h += ((Sint16*)pActwk)[30];
  pActwk->yposi.w.h += ((Sint16*)pActwk)[31];

  if (++pActwk->actfree[0] == 45) {
    pActwk->colino = 254;
    pActwk->colicnt = 2;
    pActwk->r_no0 += 2;
  }

return 1;
}

static Uint32 egg4tama_02(sprite_status* pActwk) {
  sprite_status* pEggact;

  pEggact = &actwk[((Sint16*)pActwk)[25]];
  if (pEggact->actfree[2] & 128) {
    return egg4tama_kill(pActwk);
  }

  pActwk->xposi.l += ((Sint32*)pActwk)[16];
  pActwk->yposi.l += ((Sint32*)pActwk)[4];

  if (frameout_chk(pActwk) != 0) {
    return egg4tama_kill(pActwk);
  }

  return 1;
}

static Uint32 egg4tama_kill(sprite_status* pActwk) {
  frameout(pActwk);

  return 0;
}

static Uint32 frameout_chk(sprite_status* pActwk) {
  if (pActwk->xposi.w.h < 2464
      || pActwk->xposi.w.h > 2816
      || pActwk->yposi.w.h > 1488) {
    return 1;
  }

  return 0;

}
