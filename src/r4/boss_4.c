#include "../equ.h"
#include "boss_4.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"
#include "../loader2.h"
#include "col4a.h"
#include "playsub4.h"

typedef struct {
  char E4A_VEC;
  char E4A_ETC;
  Sint16 E4A_X;
  Sint16 E4A_Y;
}
E4A;

static Uint32 egg4air_ini(sprite_status* pActwk);
static void make_airhead(sprite_status* pActwk);
static Uint32 egg4air_01(sprite_status* pActwk);
static void egg4air_01_next(sprite_status* pActwk, E4A* pEscRoot);
static void egg4air_yure(sprite_status* pActwk);
static void egg4air_yurenai(sprite_status* pActwk);
static void egg4air_hitchk(sprite_status* pActwk);
static Uint32 egg4air_02(sprite_status* pActwk);
static Uint32 egg4air_03(sprite_status* pActwk);
static void egg4air_bom(sprite_status* pActwk);
static void e4a_03_far(sprite_status* pActwk);
static void e4a_03_most_near(sprite_status* pActwk);
static void e4a_03_near(sprite_status* pActwk);
static void e4a_03_normal(sprite_status* pActwk);
static void egg4_make_bom(sprite_status* pActwk);
static Uint32 egg4air_04(sprite_status* pActwk);
static void egg4airhead_ini(sprite_status* pActwk);
static void egg4airhead_01(sprite_status* pActwk);
static void egg4airhead_02(sprite_status* pActwk);
static void make_gate(sprite_status* pActwk);

static Uint8 air_pchg0[4] = { 3, 0, 1, 255 };
static Uint8 air_pchg1[4] = { 3, 2, 3, 255 };
static Uint8 air_pchg2[4] = { 3, 4, 5, 255 };
static Uint8 air_pchg3[4] = { 0, 6, 1, 255 };
static Uint8 air_pchg4[4] = { 0, 7, 3, 255 };
static Uint8 air_pchg5[4] = { 0, 8, 5, 255 };
static Uint8* egg4air_pchg[6] = {
  air_pchg0,
  air_pchg1,
  air_pchg2,
  air_pchg3,
  air_pchg4,
  air_pchg5
};
static sprite_pattern air_pat00 = {
  1,
  { { -36, -28, 0, 439 } }
};
static sprite_pattern air_pat01 = {
  1,
  { { -36, -28, 0, 440 } }
};
static sprite_pattern air_pat02 = {
  1,
  { { -36, -28, 0, 441 } }
};
static sprite_pattern air_pat03 = {
  1,
  { { -36, -28, 0, 442 } }
};
static sprite_pattern air_pat04 = {
  1,
  { { -36, -28, 0, 443 } }
};
static sprite_pattern air_pat05 = {
  1,
  { { -36, -28, 0, 444 } }
};
static sprite_pattern air_pat06 = {
  1,
  { { -36, -28, 0, 445 } }
};
static sprite_pattern air_pat07 = {
  1,
  { { -36, -28, 0, 446 } }
};
static sprite_pattern air_pat08 = {
  1,
  { { -36, -28, 0, 447 } }
};
sprite_pattern* egg4air_pat[9] = {
  &air_pat00,
  &air_pat01,
  &air_pat02,
  &air_pat03,
  &air_pat04,
  &air_pat05,
  &air_pat06,
  &air_pat07,
  &air_pat08
};
static Uint32(*e4a_act_tbl[5])(sprite_status*) = {
  &egg4air_ini,
  &egg4air_01,
  &egg4air_02,
  &egg4air_03,
  &egg4air_04
};
static E4A egg4_air_tbl1[19] = {
  { 5, 0, 1232, 1056 },
  { 7, 0, 1248, 1072 },
  { 5, 0, 1248, 1120 },
  { 0, 2, 1596, 1120 },
  { 5, 0, 1596, 1312 },
  { 1, 0, 1376, 1312 },
  { 5, 0, 1376, 1496 },
  { 7, 0, 1396, 1516 },
  { 5, 0, 1396, 2176 },
  { 5, 0, 1396,  296 },
  { 7, 0, 1528,  428 },
  { 0, 0, 1652,  428 },
  { 7, 0, 1732,  508 },
  { 5, 0, 1732,  624 },
  { 7, 0, 1764,  656 },
  { 5, 1, 1764,  688 },
  { 5, 0, 1764,  748 },
  { 6, 0, 1696,  816 },
  { 1, 0, 1232,  816 }
};
static E4A egg4_air_tbl2[3] = {
  { 0, 0, 1952, 688 },
  { 7, 0, 2000, 736 },
  { 0, 3, 2168, 736 }
};
static E4A egg4_air_tbl3[4] = {
  { 2, 0, 1596, 1088 },
  { 0, 0, 1856, 1088 },
  { 4, 0, 1956,  988 },
  { 0, 3, 2168,  988 }
};
static Uint8 head_pchg0[3] = { 255, 0, 255 };
static Uint8 head_pchg1[6] = { 3, 2, 1, 3, 1, 255 };
static Uint8* egg4airhead_pchg[2] = {
  head_pchg0,
  head_pchg1
};
static sprite_pattern head_pat0 = {
  1,
  { { -12, -28, 0, 448 } }
};
static sprite_pattern head_pat1 = {
  1,
  { { -12, -28, 0, 449 } }
};
static sprite_pattern head_pat2 = {
  1,
  { { -12, -44, 0, 450 } }
};
static sprite_pattern head_pat3 = {
  1,
  { { -12, -44, 0, 451 } }
};
sprite_pattern* egg4airhead_pat[4] = {
  &head_pat0,
  &head_pat1,
  &head_pat2,
  &head_pat3
};
static void(*e4ahead_act_tbl[3])(sprite_status*) = {
  &egg4airhead_ini,
  &egg4airhead_01,
  &egg4airhead_02
};

void egg4air(sprite_status* pActwk) {
  if (e4a_act_tbl[pActwk->r_no0 / 2 ](pActwk) != 0) {
    if (pActwk->actfree[3]) {

      --pActwk->actfree[3];
      pActwk->mstno.b.h += 3;
      patchg(pActwk, egg4air_pchg);
      pActwk->mstno.b.h -= 3;
    }
    else patchg(pActwk, egg4air_pchg);

actionsub(pActwk);
  }
}

static Uint32 egg4air_ini(sprite_status* pActwk) {
  pActwk->colino = 61;
  pActwk->colicnt = 5;

  pActwk->cddat = 1;
  pActwk->r_no0 = 8;
  pActwk->actflg = 4;
  pActwk->sprpri = 6;
  pActwk->sprhsize = 36;
  pActwk->sprvsize = 56;
  pActwk->sproffset = 798;
  pActwk->patbase = egg4air_pat;

  ((Sint32*)pActwk)[16] = 114688;
  ((Sint32*)pActwk)[4] = 81000;

  colorset2(5);
  make_airhead(pActwk);

  return 1;
}

static void make_airhead(sprite_status* pActwk) {
  sprite_status* pNewact;

  if (actwkchk(&pNewact) == 0) {
    ((Sint16*)pActwk)[26] = pNewact - actwk;
    ((Sint16*)pNewact)[26] = pActwk - actwk;
    pNewact->actno = 75;
    pNewact->xposi.w.h = pActwk->xposi.w.h;
    pNewact->yposi.w.h = pActwk->yposi.w.h;
  }
}

static Uint32 egg4air_01(sprite_status* pActwk) {
  E4A* pEscRoot;
  Sint32 flg;
  Sint32 spdwk;

  pActwk->yposi.w.h -= ((Sint16*)pActwk)[30];

  egg4air_hitchk(pActwk);

  if (pActwk->r_no1 & 128) {
    pEscRoot = egg4_air_tbl2;
  } else if (pActwk->r_no1 & 64) {
    pEscRoot = egg4_air_tbl3;
  } else {
    pEscRoot = egg4_air_tbl1;
  }

pEscRoot += pActwk->r_no1 & 63;

  switch (pEscRoot->E4A_VEC) {

    case 0:
      pActwk->mstno.b.h = 1;
      pActwk->cddat &= 254;
      pActwk->xposi.l += ((Sint32*)pActwk)[16];

      if (pActwk->xposi.w.h < pEscRoot->E4A_X) {
        egg4air_yure(pActwk);
      } else {
        egg4air_01_next(pActwk, pEscRoot);
      }

      break;

case 1:
      pActwk->mstno.b.h = 1;
      pActwk->cddat |= 1;
      pActwk->xposi.l -= ((Sint32*)pActwk)[16];

      if (pActwk->xposi.w.h > pEscRoot->E4A_X) {
        egg4air_yure(pActwk);
      } else {
        egg4air_01_next(pActwk, pEscRoot);
      }

      break;

case 5:
      pActwk->mstno.b.h = 0;
      pActwk->yposi.l += ((Sint32*)pActwk)[16];

      if (pActwk->yposi.w.h < pEscRoot->E4A_Y) {
        egg4air_yurenai(pActwk);
        break;
      } else if (pEscRoot->E4A_Y != 2176) {
        pActwk->yposi.w.h = pEscRoot->E4A_Y;
      } else {
        pActwk->yposi.w.h = 128;
      }

      egg4air_01_next(pActwk, pEscRoot);

break;

case 6:
      pActwk->mstno.b.h = 2;
      pActwk->cddat |= 1;

      flg = 0;

      spdwk = ((Sint32*)pActwk)[4];
      pActwk->yposi.l += spdwk;
      if (pActwk->yposi.w.h >= pEscRoot->E4A_Y) {
        pActwk->yposi.w.h = pEscRoot->E4A_Y;
        ++flg;
      }

pActwk->xposi.l -= spdwk;
      if (pActwk->xposi.w.h <= pEscRoot->E4A_X) {
        pActwk->xposi.w.h = pEscRoot->E4A_X;
        ++flg;
      }

if (flg == 2) {
        egg4air_01_next(pActwk, pEscRoot);
      } else {
        egg4air_yurenai(pActwk);
      }

      break;

case 7:
      pActwk->mstno.b.h = 2;
      pActwk->cddat &= 254;

      flg = 0;

      spdwk = ((Sint32*)pActwk)[4];
      pActwk->yposi.l += spdwk;
      if (pActwk->yposi.w.h >= pEscRoot->E4A_Y) {
        pActwk->yposi.w.h = pEscRoot->E4A_Y;
        ++flg;
      }

pActwk->xposi.l += spdwk;
      if (pActwk->xposi.w.h >= pEscRoot->E4A_X) {
        pActwk->xposi.w.h = pEscRoot->E4A_X;
        ++flg;
      }

if (flg == 2) {
        egg4air_01_next(pActwk, pEscRoot);
      } else {
        egg4air_yurenai(pActwk);
      }

      break;

case 2:
      pActwk->mstno.b.h = 0;
      pActwk->yposi.l -= ((Sint32*)pActwk)[16];

      if (pActwk->yposi.w.h > pEscRoot->E4A_Y) {
        egg4air_yurenai(pActwk);
        break;
      } else if (pEscRoot->E4A_Y != 65344) {
        pActwk->yposi.w.h = pEscRoot->E4A_Y;
      } else {
        pActwk->yposi.w.h = 1856;
      }

      egg4air_01_next(pActwk, pEscRoot);

break;

case 3:
      pActwk->mstno.b.h = 2;
      pActwk->cddat |= 1;

      flg = 0;

      spdwk = ((Sint32*)pActwk)[16];
      pActwk->yposi.l -= spdwk;
      if (pActwk->yposi.w.h <= pEscRoot->E4A_Y) {
        pActwk->yposi.w.h = pEscRoot->E4A_Y;
        ++flg;
      }

pActwk->xposi.l -= spdwk;
      if (pActwk->xposi.w.h <= pEscRoot->E4A_X) {
        pActwk->xposi.w.h = pEscRoot->E4A_X;
        ++flg;
      }

if (flg == 2) {
        egg4air_01_next(pActwk, pEscRoot);
      } else {
        egg4air_yurenai(pActwk);
      }

      break;

case 4:
      flg = 0;

      spdwk = ((Sint32*)pActwk)[16];
      pActwk->yposi.l -= spdwk;
      if (pActwk->yposi.w.h <= pEscRoot->E4A_Y) {
        pActwk->yposi.w.h = pEscRoot->E4A_Y;
        ++flg;
      }

pActwk->xposi.l += spdwk;
      if (pActwk->xposi.w.h >= pEscRoot->E4A_X) {
        pActwk->xposi.w.h = pEscRoot->E4A_X;
        ++flg;
      }

pActwk->mstno.b.h = 2;
      pActwk->cddat &= 254;
      if (flg == 2) {
        egg4air_01_next(pActwk, pEscRoot);
      } else {
        egg4air_yurenai(pActwk);
      }

      break;
  }

return 1;
}

static void egg4air_01_next(sprite_status* pActwk, E4A* pEscRoot) {
  Sint16 actidx;

  if (!pEscRoot->E4A_ETC || pActwk->colicnt) {

    ++pActwk->r_no1;
    if ((pActwk->r_no1 & 63) >= 19) {
      pActwk->r_no1 &= 192;
    }
  }
  else {
    if (pEscRoot->E4A_ETC == 1) {

      pActwk->r_no1 = 128;

      if ((actidx = ((Sint16*)pActwk)[26]) != 0) {
        if ((actidx = ((Sint16*)&actwk[actidx])[27]) != 0) {
          actwk[actidx].actfree[21] = 1;
        }
      }
    } else if (pEscRoot->E4A_ETC == 2) {

      pActwk->r_no1 = 64;

      if ((actidx = ((Sint16*)pActwk)[26]) != 0) {
        if ((actidx = ((Sint16*)&actwk[actidx])[28]) != 0) {
          actwk[actidx].actfree[21] = 1;
        }
      }
    } else {

      ((Sint32*)pActwk)[16] = 229376;
      pActwk->r_no0 = 4;
      pActwk->mstno.b.h = 0;
    }
  }
}

static void egg4air_yure(sprite_status* pActwk) {
  Sint16 sinwk, coswk;

  pActwk->actfree[5] += 2;
  sinset(pActwk->actfree[5], &sinwk, &coswk);
  sinwk >>= 5;
  ((Sint16*)pActwk)[30] = sinwk;
  pActwk->yposi.w.h += sinwk;
}

static void egg4air_yurenai(sprite_status* pActwk) {
  pActwk->yposi.w.h += ((Sint16*)pActwk)[30];
}

static void egg4air_hitchk(sprite_status* pActwk) {
  Sint16 actidx;

  if (pActwk->actfree[2] && !(--pActwk->actfree[2])) {
    ((Sint32*)pActwk)[16] = 114688;
    ((Sint32*)pActwk)[4] = 81000;
  }

if (!pActwk->colicnt) return;

if (pActwk->actfree[1]) {

    if (--pActwk->actfree[1] == 0) {
      pActwk->colino = 61;
    }

    return;
  }

  if (!pActwk->colino) {

soundset(172);

if ((actidx = ((Sint16*)pActwk)[26]) != 0) {
      actwk[actidx].actfree[0] = 40;
      pActwk->actfree[2] = 70;
      ((Sint32*)pActwk)[16] = 294912;
      ((Sint32*)pActwk)[4] = 208000;

      pActwk->actfree[3] = 16;
      if (pActwk->colicnt == 1) {

        pActwk->r_no0 = 6;
        pActwk->colicnt = 0;
      }
      else pActwk->actfree[1] = 30;
    }
  }
}

static Uint32 egg4air_02(sprite_status* pActwk) {
  Sint16 actidx;

  pActwk->yposi.l += ((Sint32*)pActwk)[16];
  ((Sint32*)pActwk)[16] += 12288;

  if (pActwk->yposi.w.h >= 1408) {
    if ((actidx = ((Sint16*)pActwk)[26]) != 0) {
      actwk[actidx].r_no0 += 2;
    }

frameout(pActwk);
    return 0;
  }

  return 1;
}

static Uint32 egg4air_03(sprite_status* pActwk) {
  int_union xwk, ywk;

  egg4air_bom(pActwk);

  xwk.l = ywk.l = 0;
  xwk.w.l = actwk[0].xposi.w.h - pActwk->xposi.w.h;
  if (xwk.w.l < 0) {
    xwk.w.l = -xwk.w.l;
  }

ywk.w.l = actwk[0].yposi.w.h - pActwk->yposi.w.h;
  if (ywk.w.l < 0) {
    ywk.w.l = -ywk.w.l;
  }

xwk.l *= xwk.l;
  ywk.l *= ywk.l;
  xwk.l += ywk.l;

  if (pActwk->actfree[4] & 32) {

    if (xwk.l >= 20736) {
      e4a_03_far(pActwk);
      return 1;
    }
    pActwk->actfree[4] &= 223;
  }

if (xwk.l >= 69696) {
    e4a_03_far(pActwk);
  } else if (xwk.l < 7744) {
    e4a_03_most_near(pActwk);
  } else if (xwk.l < 18496) {
    e4a_03_near(pActwk);
  } else {
    e4a_03_normal(pActwk);
  }

  return 1;
}

static void egg4air_bom(sprite_status* pActwk) {
  Sint16 actidx;

  if ((actidx = ((Sint16*)pActwk)[26]) != 0) {
    actwk[actidx].actfree[0] = 30;
  }

  if (!pActwk->actfree[0]) {
    pActwk->actfree[0] = 32;
    egg4_make_bom(pActwk);
  }

--pActwk->actfree[0];
}

static void e4a_03_far(sprite_status* pActwk) {

  pActwk->actfree[4] |= 32;

  pActwk->yposi.w.h -= ((Sint16*)pActwk)[30];
  pActwk->mstno.b.h = 0;
  egg4air_yure(pActwk);
}

static void e4a_03_most_near(sprite_status* pActwk) {
  if ((Sint32)pActwk->yspeed.w > 2048 || (Sint32)pActwk->xspeed.w > 2048) {

((Sint32*)pActwk)[16] = 0x100000;
    ((Sint32*)pActwk)[4] = 741000;
  } else {
    ((Sint32*)pActwk)[16] = 0x80000;
    ((Sint32*)pActwk)[4] = 370000;
  }

egg4air_01(pActwk);
}

static void e4a_03_near(sprite_status* pActwk) {
  ((Sint32*)pActwk)[16] = 196608;
  ((Sint32*)pActwk)[4] = 139000;

egg4air_01(pActwk);
}

static void e4a_03_normal(sprite_status* pActwk) {
  ((Sint32*)pActwk)[16] = 114688;
  ((Sint32*)pActwk)[4] = 81000;

egg4air_01(pActwk);
}

static void egg4_make_bom(sprite_status* pActwk) {
  sprite_status* pNewact;

  if (actwkchk(&pNewact) == 0) {
    pNewact->r_no1 = 1;
    pNewact->actno = 24;
    pNewact->xposi.w.h = pActwk->xposi.w.h;
    pNewact->yposi.w.h = pActwk->yposi.w.h;
    soundset(158);
  }
}

static Uint32 egg4air_04(sprite_status* pActwk) {
  if (actwk[0].xposi.w.h >= 1144) {
    pActwk->r_no0 = 2;

    bossstart = 4;
    ((Sint32*)pActwk)[4] = 208000;
  }

return 1;
}

void egg4airhead(sprite_status* pActwk) {
  e4ahead_act_tbl[pActwk->r_no0 / 2](pActwk);
}

static void egg4airhead_ini(sprite_status* pActwk) {
  pActwk->r_no0 += 2;
  pActwk->actflg = 4;
  pActwk->sprpri = 6;
  pActwk->sprhsize = 36;
  pActwk->sprvsize = 56;
  pActwk->sproffset = 798;
  pActwk->patbase = egg4airhead_pat;
  make_gate(pActwk);
}

static void egg4airhead_01(sprite_status* pActwk) {
  Sint16 actidx;

  if (pActwk->actfree[0]) {
    if (!(--pActwk->actfree[0])) {

      pActwk->mstno.b.h = 0;
      pActwk->patno = 0;
      pActwk->patcnt = 0;
      pActwk->pattim = 0;
      pActwk->pattimm = 0;
    }
    else pActwk->mstno.b.h = 1;
  }

if ((actidx = ((Sint16*)pActwk)[26]) != 0) {
    pActwk->xposi.w.h = actwk[actidx].xposi.w.h;
    pActwk->yposi.w.h = actwk[actidx].yposi.w.h;
    pActwk->cddat = actwk[actidx].cddat;
  }

  patchg(pActwk, egg4airhead_pchg);
  actionsub(pActwk);
}

static void egg4airhead_02(sprite_status* pActwk) {
  Sint16 actidx;

  if (actwk[0].xposi.w.h >= 2112 && actwk[0].yposi.w.h >= 1360) {
    bossflag = 4;

    if ((actidx = ((Sint16*)pActwk)[27]) != 0) {
      if (actwk[actidx].actno) {
        frameout(&actwk[actidx]);
      }
    }

if ((actidx = ((Sint16*)pActwk)[28]) != 0) {
      if (actwk[actidx].actno) {
        frameout(&actwk[actidx]);
      }
    }

frameout(pActwk);
  }
}

static void make_gate(sprite_status* pActwk) {
  sprite_status* pNewact;

  if (actwkchk(&pNewact) == 0) {
    ((Sint16*)pActwk)[27] = pNewact - actwk;
    pNewact->actno = 71;
    pNewact->userflag.b.h = 1;
    pNewact->xposi.w.h = 1888;
    pNewact->yposi.w.h = 704;

    if (actwkchk(&pNewact) == 0) {
      ((Sint16*)pActwk)[28] = pNewact - actwk;
      pNewact->actno = 71;
      pNewact->userflag.b.h = 1;
      pNewact->xposi.w.h = 1728;
      pNewact->yposi.w.h = 1120;
    }
  }

}
