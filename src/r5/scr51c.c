#include "../equ.h"
#include "scr51a.h"
#include "../impfuncs.h"
#include "../save.h"
#include "../scrchk.h"
#include "col5a.h"

static void playposiset(void);
static void scrbinit(Sint16 sXpos, Sint16 sYpos);
static void z51cline(Sint16** ppHscw);
static void zonescrsetsub0(Sint16 yoffs, Sint16 yline, int_union** pHscrbuf, Sint16* pHscrwk);
static void scrollb_v(Sint16 yPos);
static void scrollb_h(Sint32 xoffs, Sint32 flgbit);
static void scrollc_h(Sint32 xoffs, Sint32 flgbit);
static void scrollz_h(Sint32 xoffs, Sint32 flgbit);
static void scroll_h(void);
static void scrh_move(void);
static void scroll_v(void);
static void sv_move_main(Sint16 yPos);
static void sv_move_main2(Sint16 yPos);
static void sv_move_sub(Sint16 yPos);
static void sv_move_sub2(void);
static void sv_move_minus(Sint16 speed);
static void scrv_up_ch(int_union lSpd);
static void sv_move_plus(Sint16 sSpd);
static void scrv_down_ch(int_union lSpd);
static void scrv_move(int_union lSpd);
static void mapset2(Uint8* pMapDst, Sint32 idx);

static Uint8 z81awrttbl[65];
static int_union* vblockwrtbtbl[8] = {
  &scrb_h_posiw,
  &scrb_v_posiw,
  &scrb_h_posiw,
  &scrb_v_posiw,
  &scrc_h_posiw,
  &scrc_v_posiw,
  &scrz_h_posiw,
  &scrz_v_posiw
};
static int_union* mapwrt_tbl[8] = {
  &scrb_h_posiw,
  &scrb_v_posiw,
  &scrb_h_posiw,
  &scrb_v_posiw,
  &scrc_h_posiw,
  &scrc_v_posiw,
  &scrz_h_posiw,
  &scrz_v_posiw
};
extern Uint8 mapwkb[8][64];
extern Uint8 mapwka[8][64];
extern map_init_data mapinittbl;
extern Uint8* zonemaptbl[];

void enkeichg(void) {
  Sint16 tbl[9] = {
    3584, 8960, -1,
    3584, 8960, -1,
    3584, 8960, -1
  };
  char tbl0[3] = {
    2, 4, 2
  };
  Sint32 EnkeiNo, TileNo, BmpNo, i;
  Sint16 *pTbl, poswk;

  if (scrflaga.b.h) return;

EnkeiNo = 0;
  pTbl = tbl;

  while ((poswk = *pTbl++) >= 0) {
    if (actwk[0].xposi.w.h <= poswk) break;

++EnkeiNo;
  }

if (enkeino == EnkeiNo) return;

enkeino = EnkeiNo;

TileNo = 592;
  if (EnkeiNo == 1) {
    i = 99;
    BmpNo = 95;
  } else {
    i = 95;
    BmpNo = 0;
  }
  for ( ; i > 0; --i) {
    ChangeTileBmp(TileNo++, BmpNo++);
  }

  colorset2((tbl0[EnkeiNo] >> 2) + 4);
  colorset((tbl0[EnkeiNo] >> 2) + 4);

  mapset2((Uint8*)mapwkb, tbl0[EnkeiNo] / 2);
  scrbinit(scra_h_posit.w.h, scra_v_posit.w.h);
  mapwrt_cnt = 1;
}

Sint16 scr_dir_tbl[6] = { 4, 0, 10903, 0, 784, 96 };
Sint16 playpositbl[1][2] = { { 64, 620 } };
Uint8 playmapnotbl[4] = { 127, 127, 98, 24 };

sprite_status* main_chk(void) {
  if (main_play == 0)
    return &actwk[0];
  else
    return &actwk[1];

}

void scr_set(void) {
  Sint16* pScrTbl;

  scrh_flag = scrv_flag = 0;
  scr_die.b.h = zone_flag.b.h = 0;
  scr_timer.b.h = 0;

  pScrTbl = scr_dir_tbl;
  scrar_no = *pScrTbl;
  ++pScrTbl;
  scralim_left = scralim_n_left = *pScrTbl;
  ++pScrTbl;
  scralim_right = scralim_n_right = *pScrTbl;
  ++pScrTbl;
  scralim_up = scralim_n_up = *pScrTbl;
  ++pScrTbl;
  scralim_down = scralim_n_down = *pScrTbl;
  scra_h_keep = *pScrTbl + 576;
  ++pScrTbl;
  scra_h_count = 16;
  scra_v_count = 16;

  scra_vline = *pScrTbl;
  scra_hline = 160;

  playposiset();
}

static void playposiset(void) {
  Sint16 xwk, ywk, *pPositbl;

  if (plflag) {
    playload();
    xwk = actwk[0].xposi.w.h;

    if (actwk[0].yposi.w.h > 0) {
      ywk = actwk[0].yposi.w.h;
    } else {
      ywk = 0;
    }
  }
  else {
    pPositbl = playpositbl[0];

actwk[0].xposi.w.h = xwk = pPositbl[0];
    actwk[0].yposi.w.h = ywk = pPositbl[1];
  }

if (xwk > 160) {
    xwk -= 160;
  } else {
    xwk = 0;
  }

if (xwk > scralim_right) {
    xwk = scralim_right;
  }

scra_h_posit.w.h = xwk;

  if (ywk > 96) {
    ywk -= 96;
  } else {
    ywk = 0;
  }

if (ywk > scralim_down) {
    ywk = scralim_down;
  }

scra_v_posit.w.h = ywk;

  scrbinit(xwk, ywk);

loopmapno = playmapnotbl[0];
  loopmapno2 = playmapnotbl[1];
  ballmapno = playmapnotbl[2];
  ballmapno2 = playmapnotbl[3];
}

static void scrbinit(Sint16 sXpos, Sint16 sYpos) {
  int_union lYwk;

  lYwk.w.h = sYpos;
  lYwk.w.l = 0;

  lYwk.l = (Uint32)lYwk.l >> 2;

  scrb_v_posit.l = lYwk.l;
  scrc_v_posit.w.h = lYwk.w.h;
  scrz_v_posit.w.h = lYwk.w.h;

  sXpos = (Uint16)sXpos >> 1;
  scrb_h_posit.w.h = sXpos;

  sXpos = (Uint16)sXpos >> 1;
  scrc_h_posit.w.h = sXpos;

  sXpos >>= 2;
  scrz_h_posit.w.h = sXpos * 3;
}

void scroll(void) {
  int_union ldwk;
  int_union* pHscrbuf;
  Sint32 i;
  Sint32 lXwk;
  Sint32 lYwk;
  Sint16 sYnum;
  Sint16* psHscr;
  Sint16* psHscw;
  static Sint32 scaddtbl[5] = { 65536, 57344, 49152, 40960, 32768 };
  static Uint8 z12c_cnttbl[5] = { 1, 3, 3, 3, 1 };

  if (scroll_start.b.h) return;

scrflaga.w = scrflagb.w = scrflagc.w = scrflagz.w = 0;

  scrchk();

  scroll_h();
  scroll_v();

  vscroll.w.h = scra_v_posit.w.h;
  vscroll.w.l = scrb_v_posit.w.h;

lXwk = scra_hz * 16 * 3;
  scrollz_h(lXwk, 6);

  lXwk = scra_hz * 64;
  scrollc_h(lXwk, 4);

  lXwk = scra_hz * 128;
  scrollb_h(lXwk, 2);

  lYwk = (Uint16)scra_v_posit.w.h >> 2;

scrollb_v(lYwk);

  scrc_v_posit.w.h = scrz_v_posit.w.h = vscroll.w.l = scrb_v_posit.w.h;
  scrflagb.b.h = scrflagb.b.h | scrflagz.b.h | scrflagc.b.h;
  scrflagz.b.h = scrflagc.b.h = 0;

  psHscw = hscrollwork;

  if (enkeino & 1) {

    ldwk.w.l = -scrz_h_posit.w.h;
    for (i = 0; i < 14; ++i) {

      *psHscw++ = ldwk.w.l;
    }
    z51cline(&psHscw);
  }
  else {
    ldwk.w.l = -scrb_h_posit.w.h;
    for (i = 0; i < 10; ++i) {

      *psHscw++ = ldwk.w.l;
    }

    ldwk.w.l = -scrz_h_posit.w.h;
    for (i = 0; i < 22; ++i) {

      *psHscw++ = ldwk.w.l;
    }

    ldwk.w.l = -scrc_h_posit.w.h;
    for (i = 0; i < 4; ++i) {

      *psHscw++ = ldwk.w.l;
    }

    ldwk.w.l = -scrb_h_posit.w.h;
    for (i = 0; i < 12; ++i) {

      *psHscw++ = ldwk.w.l;
    }
  }

pHscrbuf = lphscrollbuff;
  sYnum = (Uint16)(scrb_v_posit.w.h & 504) >> 2;
  psHscr = &hscrollwork[sYnum / 2];
  zonescrsetsub0(scrb_v_posit.w.h, 28, &pHscrbuf, psHscr);
}

static void z51cline(Sint16** ppHscw) {
  Sint32 lHwk;
  int_union ldHposwk;
  Sint32 i, j;
  Sint16 wk;
  char z51cscrtbl[3] = {
    39, 3, 9
  };

  lHwk = scra_h_posit.w.h - scrc_h_posit.w.h;
  lHwk = (Uint32)lHwk * 128 / 12;
  lHwk *= 512;

  ldHposwk.l = 0;
  ldHposwk.w.l = scrc_h_posit.w.h;

  for (i = 2; i >= 0; --i) {

    wk = -ldHposwk.w.l;

    for (j = z51cscrtbl[i]; j >= 0; --j) {

      **ppHscw = wk;
      ++*ppHscw;
    }

    ldHposwk.l = (Uint32)ldHposwk.l >> 16 & 65535 | ldHposwk.l << 16 & -65536;
    ldHposwk.l += lHwk;
    ldHposwk.l = (Uint32)ldHposwk.l >> 16 & 65535 | ldHposwk.l << 16 & -65536;
  }
}

static void zonescrsetsub0(Sint16 yoffs, Sint16 yline, int_union** pHscrbuf, Sint16* pHscrwk) {
  Sint32 i, j;
  int_union ldwk;

  ldwk.l = 0;

  for (i = 0, j = yoffs & 7 ; yline >= i; ++i, j = 0) {
    ldwk.w.l = *pHscrwk;
    ++pHscrwk;

    for ( ; j < 8; ++j, ++*pHscrbuf) {
      (*pHscrbuf)->l = ldwk.l;
    }
  }
}

static void scrollb_v(Sint16 yPos) {
  Sint16 ysv, ywk;

  ysv = scrb_v_posit.w.h;
  scrb_v_posit.w.h = yPos;
  ywk = (Uint8)(yPos & 16) ^ scrb_v_count;

  if (!ywk) {
    scrb_v_count ^= 16;
    if (yPos - ysv < 0) {
      scrflagb.b.h |= 1;
    }
    else {
      scrflagb.b.h |= 2;
    }
  }

}

static void scrollb_h(Sint32 xoffs, Sint32 flgbit) {
  Sint32 lXwk, lXsv;
  int_union ldXwk;

  lXsv = scrb_h_posit.l;
  lXwk = lXsv + xoffs;
  scrb_h_posit.l = lXwk;
  ldXwk.l = lXwk;
  ldXwk.l = (Uint32)ldXwk.l >> 16 & 65535 | ldXwk.l << 16 & -65536;
  ldXwk.w.l &= 16;
  ldXwk.b.b4 ^= scrb_h_count;

  if (!ldXwk.b.b4) {
    scrb_h_count ^= 16;
    if (lXwk - lXsv < 0) {
      scrflagb.b.h |= 1 << flgbit;
    }
    else {
      scrflagb.b.h |= 1 << flgbit + 1;
    }
  }

}

static void scrollc_h(Sint32 xoffs, Sint32 flgbit) {
  Sint32 lXwk, lXsv;
  int_union ldXwk;

  lXsv = scrc_h_posit.l;
  lXwk = lXsv + xoffs;
  scrc_h_posit.l = lXwk;
  ldXwk.l = lXwk;
  ldXwk.l = (Uint32)ldXwk.l >> 16 & 65535 | ldXwk.l << 16 & -65536;
  ldXwk.w.l &= 16;
  ldXwk.b.b4 ^= scrc_h_count;

  if (!ldXwk.b.b4) {
    scrc_h_count ^= 16;
    if (lXwk - lXsv < 0) {
      scrflagc.b.h |= 1 << flgbit;
    }
    else {
      scrflagc.b.h |= 1 << flgbit + 1;
    }
  }

}

static void scrollz_h(Sint32 xoffs, Sint32 flgbit) {
  Sint32 lXwk, lXsv;
  int_union ldXwk;

  lXsv = scrz_h_posit.l;
  lXwk = lXsv + xoffs;
  scrz_h_posit.l = lXwk;
  ldXwk.l = lXwk;
  ldXwk.l = (Uint32)ldXwk.l >> 16 & 65535 | ldXwk.l << 16 & -65536;
  ldXwk.w.l &= 16;
  ldXwk.b.b4 ^= scrz_h_count;

  if (!ldXwk.b.b4) {
    scrz_h_count ^= 16;
    if (lXwk - lXsv < 0) {
      scrflagz.b.h |= 1 << flgbit;
    }
    else {
      scrflagz.b.h |= 1 << flgbit + 1;
    }
  }

}

static void scroll_h(void) {
  Sint16 xwk;
  Uint8 bXwk;

  xwk = scra_h_posit.w.h;

  scrh_move();
  bXwk = (Uint8)(scra_h_posit.w.h & 16) ^ scra_h_count;
  if (!bXwk) {
    scra_h_count ^= 16;
    if (scra_h_posit.w.h - xwk < 0) {
      scrflaga.b.h |= 4;
    } else {
      scrflaga.b.h |= 8;
    }
  }

}

static void scrh_move(void) {
  Sint16 xwk;

  xwk = actwk[0].xposi.w.h - scra_h_posit.w.h - scra_hline;
  if (xwk == 0) {

    scra_hz = 0;
    return;
  } else if (xwk < 0) {

    if (xwk < -16) {
      xwk = -16;
    }

xwk += scra_h_posit.w.h;
    if (xwk < scralim_left) {
      xwk = scralim_left;
    }
  }
  else {
    if (xwk > 16) {
      xwk = 16;
    }

xwk += scra_h_posit.w.h;
    if (xwk > scralim_right) {
      xwk = scralim_right;
    }
  }

scra_hz = (xwk - scra_h_posit.w.h) << 8;
  scra_h_posit.w.h = xwk;
}

static void scroll_v(void) {
  Sint16 ywk;

  ywk = actwk[0].yposi.w.h - scra_v_posit.w.h;
  if (actwk[0].cddat & 4) {
    ywk -= 5;
  }

if (actwk[0].cddat & 2) {
    ywk += 32;
    if (ywk < scra_vline) {
      ywk -= scra_vline;
      sv_move_main2(ywk);
      return;
    } else if (ywk >= scra_vline + 64) {
      ywk -= scra_vline + 64;
      sv_move_main2(ywk);
      return;
    }
    ywk -= scra_vline + 64;
    if (!limmoveflag) goto label1;
    sv_move_sub2();
    return;
  }

if ((ywk -= scra_vline) != 0) {
    sv_move_main(ywk);
    return;
  } else if (!limmoveflag) goto label1;
  sv_move_sub2();
  return;

label1:
  scra_vz = 0;
}

static void sv_move_main(Sint16 yPos) {
  Sint16 spdwk;

  if (scra_vline == 96) {
    if (actwk[0].mspeed.w < 0) {
      spdwk = -actwk[0].mspeed.w;
    } else {
      spdwk = actwk[0].mspeed.w;
    }

if (spdwk >= 2048) {
      sv_move_main2(yPos);
    }
    else {

      if (yPos > 6) {
        sv_move_plus(1536);
      } else if (yPos < -6) {
        sv_move_minus(1536);
      }
      else sv_move_sub(yPos);
    }
  }
  else {
    if (yPos > 2) {
      sv_move_plus(512);
    } else if (yPos < -2) {
      sv_move_minus(512);
    }
    else sv_move_sub(yPos);
  }

}

static void sv_move_main2(Sint16 yPos) {
  Sint16 spdwk;

  spdwk = 4096;

  if (yPos > 16) {
    sv_move_plus(spdwk);
  } else if (yPos < -16) {
    sv_move_minus(spdwk);
  } else {
    sv_move_sub(yPos);
  }
}

static void sv_move_sub(Sint16 yPos) {
  int_union lSpd;

  lSpd.l = 0;
  lSpd.w.l = yPos + scra_v_posit.w.h;
  if (yPos < 0) {
    scrv_up_ch(lSpd);
  } else {
    scrv_down_ch(lSpd);
  }
}

static void sv_move_sub2(void) {
  limmoveflag = 0;

  sv_move_sub(0);
}

static void sv_move_minus(Sint16 speed) {
  int_union lSpd;

  lSpd.l = (-speed << 8) + scra_v_posit.l;
  lSpd.l = (Uint32)lSpd.l >> 16 & 65535 | lSpd.l << 16 & -65536;
  scrv_up_ch(lSpd);
}

static void scrv_up_ch(int_union lSpd) {
  if (lSpd.w.l <= scralim_up) {
    if (lSpd.w.l > -256) {

      lSpd.w.l = scralim_up;
    } else {
      lSpd.w.l &= 2047;
      actwk[0].yposi.w.h &= 2047;
      scra_v_posit.w.h &= 2047;
      scrb_v_posit.w.h &= 1023;
    }
  }

  scrv_move(lSpd);
}

static void sv_move_plus(Sint16 sSpd) {
  int_union lSpd;

  lSpd.l = (sSpd << 8) + scra_v_posit.l;
  lSpd.l = (Uint32)lSpd.l >> 16 & 65535 | lSpd.l << 16 & -65536;
  scrv_down_ch(lSpd);
}

static void scrv_down_ch(int_union lSpd) {
  if (lSpd.w.l >= scralim_down) {
    if (lSpd.w.l < 2048) {

      lSpd.w.l = scralim_down;
    } else {
      lSpd.w.l -= 2048;
      actwk[0].yposi.w.h &= 2047;
      scra_v_posit.w.h -= 2048;
      scrb_v_posit.w.h &= 1023;
    }
  }

  scrv_move(lSpd);
}

static void scrv_move(int_union lSpd) {
  Sint16 ywk, ysv;
  uint_union spdwk;

  ysv = scra_v_posit.w.h;
  lSpd.l = (Uint32)lSpd.l >> 16 & 65535 | lSpd.l << 16 & -65536;
  spdwk.l = lSpd.l - scra_v_posit.l;
  spdwk.l = spdwk.l >> 8 | spdwk.l << 24;
  scra_vz = spdwk.w.l;

  scra_v_posit.l = lSpd.l;

  ywk = (Uint8)(scra_v_posit.w.h & 16) ^ scra_v_count;
  if (!ywk) {
    scra_v_count ^= 16;
    ywk = scra_v_posit.w.h - ysv;
    if (ywk < 0) {
      scrflaga.b.h |= 1;
    } else {
      scrflaga.b.h |= 2;
    }
  }

}

void scrollwrt(void) {
  Uint16 wH_posiw, wV_posiw;
  Sint32 VramBase;
  POINT TilePoint;
  Uint8* pScrFlag;
  Uint8* pMapWk;
  int_union lD4;
  Uint16 wD5;

  VramBase = 24576;
  wH_posiw = scrb_h_posiw.w.h;
  wV_posiw = scrb_v_posiw.w.h;
  pMapWk = (Uint8*)mapwkb;
  pScrFlag = &scrflagbw.b.h;
  scrollwrtb(pScrFlag, pMapWk, VramBase);

  VramBase = 16384;
  wH_posiw = scra_h_posiw.w.h;
  wV_posiw = scra_v_posiw.w.h;
  pMapWk = (Uint8*)mapwka;
  pScrFlag = &scrflagaw.b.h;

  if (*pScrFlag) {
    if (*pScrFlag & 1) {
      *pScrFlag &= 254;

      lD4.l = -16;
      wD5 = 65520;

      vramadrset(wH_posiw, wV_posiw, wD5, lD4.w.l, &TilePoint);

      hblockwrt(&TilePoint, VramBase, wH_posiw, wV_posiw, wD5, lD4.w.l, pMapWk, 21);
    }

if (*pScrFlag & 2) {
      *pScrFlag &= 253;

      lD4.l = 224;
      wD5 = 65520;
      vramadrset(wH_posiw, wV_posiw, wD5, lD4.w.l, &TilePoint);

      hblockwrt(&TilePoint, VramBase, wH_posiw, wV_posiw, wD5, lD4.w.l, pMapWk, 21);
    }

if (*pScrFlag & 4) {
      *pScrFlag &= 251;

      lD4.l = -16;
      wD5 = 65520;
      vramadrset(wH_posiw, wV_posiw, wD5, lD4.w.l, &TilePoint);

      vblockwrt(&TilePoint, VramBase, wH_posiw, wV_posiw, wD5, lD4.w.l, pMapWk, 15);
    }

if (*pScrFlag & 8) {
      *pScrFlag &= 247;

      lD4.l = -16;
      wD5 = 320;
      vramadrset(wH_posiw, wV_posiw, wD5, lD4.w.l, &TilePoint);

      vblockwrt(&TilePoint, VramBase, wH_posiw, wV_posiw, wD5, lD4.w.l, pMapWk, 15);
    }
  }

}

void scrollwrtb(Uint8* pScrFlag, Uint8* pMapWk, Sint32 VramBase) {
  int_union lD4;
  Sint32 WrtTblCnt;
  Uint16 wD0;
  Uint16 wD5;
  Uint16 wH_posiw;
  Uint16 wV_posiw;
  POINT TilePoint;
  Sint32 BlkIndex;
  Sint32 i;

  lD4.l = -16;

  if (*pScrFlag & 1) {
    *pScrFlag &= 254;
  }
  else {
    *pScrFlag &= 254;

    if (*pScrFlag & 2) {
      *pScrFlag &= 253;
      lD4.w.l = 224;
    }
    else {
      *pScrFlag &= 253;
      goto label1;
    }
  }

wD0 = (Sint16)(scrb_v_posit.w.h + lD4.w.l) / 16;
  wD0 &= 127;
  wD0 = z81awrttbl[wD0 + 1];

wH_posiw = vblockwrtbtbl[wD0]->w.h;
  wV_posiw = vblockwrtbtbl[wD0 + 1]->w.h;
  if (wD0 != 0) {
    wD5 = 65520;
    vramadrset(wH_posiw, wV_posiw, wD5, lD4.w.l, &TilePoint);

    hblockwrt(&TilePoint, VramBase, wH_posiw, wV_posiw, wD5, lD4.w.l, pMapWk, 21);

}
  else {

    wD5 = 0;
    vramadrset1(wH_posiw, wV_posiw, wD5, lD4.w.l, &TilePoint);

hblockwrt1(&TilePoint, VramBase, wH_posiw, wV_posiw, wD5, lD4.w.l, pMapWk, 31);
  }

label1:
  if (*pScrFlag != 0) {

lD4.l = -16;
    wD5 = 65520;
    if (*pScrFlag & 168) {
      *pScrFlag = (*pScrFlag & 168) >> 1;
      wD5 = 320;
    }

WrtTblCnt = (Uint16)(scrb_v_posit.w.h / 16);

    if (WrtTblCnt < 0) WrtTblCnt = 0;
    if (WrtTblCnt > 113) WrtTblCnt = 113;

for (i = 0; i < 16; ++i) {
      wD0 = z81awrttbl[WrtTblCnt++];

      if (wD0 != 0 && *pScrFlag & (Uint8)(1 << wD0)) {
        wH_posiw = vblockwrtbtbl[wD0]->w.h;
        wV_posiw = vblockwrtbtbl[wD0 + 1]->w.h;

        mapadrset(wH_posiw, wV_posiw, wD5, lD4.w.l, pMapWk, &BlkIndex);

vramadrset(wH_posiw, wV_posiw, wD5, lD4.w.l, &TilePoint);

        blockwrt(VramBase, &TilePoint, BlkIndex);
      }
      lD4.w.l += 16;
    }
    *pScrFlag = 0;
  }
}

void scrollwrtc(void) {}

void scrollwrtz(void) {}

void hblockwrt(POINT* pTilePoint, Sint32 VramBase, Uint16 wH_posiw, Uint16 wV_posiw, Uint16 xOffs, Uint16 yOffs, Uint8* pMapWk, Sint32 lpcnt) {
  Sint32 BlkIndex;

do {
    mapadrset(wH_posiw, wV_posiw, xOffs, yOffs, pMapWk, &BlkIndex);

blockwrt(VramBase, pTilePoint, BlkIndex);
    pTilePoint->x += 2;
    if (pTilePoint->x >= 64) {
      pTilePoint->x -= 64;
    }
    xOffs += 16;
  } while (--lpcnt >= 0);
}

void hblockwrt1(POINT* pTilePoint, Sint32 VramBase, Uint16 wH_posiw, Uint16 wV_posiw, Uint16 xOffs, Uint16 yOffs, Uint8* pMapWk, Sint32 lpcnt) {
  Sint32 BlkIndex;

  do {
    mapadrset1(wH_posiw, wV_posiw, xOffs, yOffs, pMapWk, &BlkIndex);

blockwrt(VramBase, pTilePoint, BlkIndex);
    pTilePoint->x += 2;
    if (pTilePoint->x >= 64) {
      pTilePoint->x -= 64;
    }
    xOffs += 16;
  } while (--lpcnt >= 0);
}

void vblockwrt(POINT* pTilePoint, Sint32 VramBase, Uint16 wH_posiw, Uint16 wV_posiw, Uint16 xOffs, Uint16 yOffs, Uint8* pMapWk, Sint32 lpcnt) {
  Sint32 BlkIndex;

  do {
    mapadrset(wH_posiw, wV_posiw, xOffs, yOffs, pMapWk, &BlkIndex);

blockwrt(VramBase, pTilePoint, BlkIndex);
    pTilePoint->y += 2;
    if (pTilePoint->y >= 32) {
      pTilePoint->y -= 32;
    }
    yOffs += 16;
  } while (--lpcnt >= 0);
}

void blockwrt(Sint32 VramBase, POINT* pTilePoint, Sint32 BlkIndex) {
  Sint32 base;
  Sint32 frip;
  Sint32 p0, p1, p2, p3;
  Sint32 x, y;

  if (VramBase == 24576) {

    base = 1;
  }
  else if (VramBase == 16384) {

    base = 0;
  }
  x = pTilePoint->x;
  y = pTilePoint->y;

frip = BlkIndex & 6144;
  BlkIndex &= 1023;
  if (frip == 6144) {

    p0 = 3, p1 = 2;
    p2 = 1, p3 = 0;
  }
  else if (frip & 4096) {

    p0 = 2, p1 = 3;
    p2 = 0, p3 = 1;
  }
  else if (frip & 2048) {

    p0 = 1, p1 = 0;
    p2 = 3, p3 = 2;
  }
  else {

    p0 = 0, p1 = 1;
    p2 = 2, p3 = 3;
  }

  SetGrid(base, x, y, blockwk[BlkIndex][p0], frip);
  SetGrid(base, x + 1, y, blockwk[BlkIndex][p1], frip);
  SetGrid(base, x, y + 1, blockwk[BlkIndex][p2], frip);
  SetGrid(base, x + 1, y + 1, blockwk[BlkIndex][p3], frip);

}

Sint32 mapadrset(Uint16 wH_posiw, Uint16 wV_posiw, Uint16 xOffs, Uint16 yOffs, Uint8* pMapWk, Sint32* pIndex) {
  return mapadrset99(wH_posiw, wV_posiw, xOffs, yOffs, pMapWk, pIndex);
}

Sint32 mapadrset1(Uint16 wH_posiw, Uint16 wV_posiw, Uint16 xOffs, Uint16 yOffs, Uint8* pMapWk, Sint32* pIndex) {
  wH_posiw = 0;
  return mapadrset99(wH_posiw, wV_posiw, xOffs, yOffs, pMapWk, pIndex);
}

Sint32 mapadrset99(Uint16 wH_posiw, Uint16 wV_posiw, Uint16 xOffs, Uint16 yOffs, Uint8* pMapWk, Sint32* pIndex) {
  Sint32 xBlk, yBlk;
  Sint32 i;
  Uint8 ScreenNo;
  Uint16* lpw;

  xOffs += wH_posiw;
  yOffs += wV_posiw;

if ((Sint16)xOffs < 0)
    xOffs = 0;
  if ((Sint16)yOffs < 0)
    yOffs = 0;
  if ((Sint16)xOffs >= 16384)
    xOffs = 16383;
  if ((Sint16)yOffs >= 2048)
    yOffs = 2047;
  i = xOffs / 256 + yOffs / 256 * 64;
  if (i < 0) i = 0;

  ScreenNo = pMapWk[i] & 127;

  if (ScreenNo) {

xOffs &= 32767;
    yOffs &= 32767;

xBlk = xOffs;
    xBlk %= 256;
    xBlk /= 16;
    yBlk = yOffs;
    yBlk %= 256;
    yBlk /= 16;

    lpw = pmapwk;
    lpw += xBlk;
    lpw += yBlk * 16;
    lpw += (ScreenNo - 1) * 16 * 16;
    *pIndex = *lpw;

    return 1;
  }

  *pIndex = 0;
  return 0;
}

Sint32 mapadrset2(Uint16 xOffs, Uint16 yOffs, Uint8* pMapWk, Sint32* pIndex, Uint16** ppBlockNo) {
  Sint32 xBlk;
  Sint32 yBlk;
  Uint8 ScreenNo;
  Uint16* lpw;

ScreenNo = pMapWk[xOffs / 256 + yOffs / 256 * 64] & 127;
  if (ScreenNo) {

xBlk = xOffs;
    xBlk %= 256;
    xBlk /= 16;
    yBlk = yOffs;
    yBlk %= 256;
    yBlk /= 16;

lpw = pmapwk;
    lpw += xBlk;
    lpw += yBlk * 16;
    lpw += (ScreenNo - 1) * 16 * 16;
    *ppBlockNo = lpw;
    *pIndex = *lpw;

return 1;
  }

  *pIndex = 0;
  return 0;
}

void block_wrt(Uint16 BlockNo, Uint16 xOffs, Uint16 yOffs) {
  Sint32 VramBase;
  POINT TilePoint;
  Uint8* pMapWk;
  Uint16* pBlockIndex;
  Sint32 Index;

  VramBase = 16384;
  pMapWk = (Uint8*)mapwka;
  if (mapadrset2(xOffs, yOffs, pMapWk, &Index, &pBlockIndex) != 0) {
    *pBlockIndex = BlockNo;
    if (block_chk(xOffs, yOffs) == 0) {
      vramadrset0(xOffs, yOffs, &TilePoint);
      blockwrt(VramBase, &TilePoint, BlockNo);
    }
  }

}

Sint32 block_chk(Uint16 xOffs, Uint16 yOffs) {
  if ((scra_v_posit.w.h & 65520) - 16 > yOffs) {
    if ((Sint16)(scra_v_posit.w.h + 240 & 65520) - 16 <= (Sint16)yOffs) {
      if ((scra_h_posit.w.h & 65520) - 16 > xOffs) {
        if ((Sint16)(scra_v_posit.w.h + 336 & 65520) - 16 <= (Sint16)xOffs) {

          return 0;
        }
      }
    }
  }

  return 1;
}

void vramadrset(Uint16 wH_posiw, Uint16 wV_posiw, Uint16 xOffs, Uint16 yOffs, POINT* lpTilePoint) {
  vramadrset99(wH_posiw, wV_posiw, xOffs, yOffs, lpTilePoint);

}

void vramadrset1(Uint16 wH_posiw, Uint16 wV_posiw, Uint16 xOffs, Uint16 yOffs, POINT* lpTilePoint) {
  wH_posiw = 0;
  vramadrset99(wH_posiw, wV_posiw, xOffs, yOffs, lpTilePoint);

}

void vramadrset0(Uint16 xOffs, Uint16 yOffs, POINT* lpTilePoint) {
  vramadrset99(0, 0, xOffs, yOffs, lpTilePoint);

}

void vramadrset2(Uint16 wH_posiw, Uint16 wV_posiw, Uint16 xOffs, Uint16 yOffs, POINT* lpTilePoint) {
  vramadrset99(wH_posiw, wV_posiw, xOffs, yOffs, lpTilePoint);

}

void vramadrset99(Uint16 wH_posiw, Uint16 wV_posiw, Uint16 xOffs, Uint16 yOffs, POINT* lpTilePoint) {
  xOffs += wH_posiw;
  yOffs += wV_posiw;
  yOffs &= 240;
  xOffs &= 496;

  lpTilePoint->x = xOffs / 16 * 2;
  lpTilePoint->y = yOffs / 16 * 2;

}

void mapwrt(void) {
  Sint32 VramBase;
  Uint16 wH_posiw, wV_posiw;
  Uint8* pMapWk;

  wH_posiw = scra_h_posiw.w.h;
  wV_posiw = scra_v_posiw.w.h;
  pMapWk = (Uint8*)mapwka;
  VramBase = 16384;
  mapwrt2(wH_posiw, wV_posiw, pMapWk, VramBase);
  mapwrtb();
}

void mapwrtb(void) {
  Sint32 VramBase;
  Uint16 wH_posiw, wV_posiw;
  Uint8* pMapWk;

  wH_posiw = scrb_h_posiw.w.h;
  wV_posiw = scrb_v_posiw.w.h;
  pMapWk = (Uint8*)mapwkb;
  VramBase = 24576;
  mapwrt_z81a(pMapWk, VramBase);

}

void mapwrt2(Uint16 wH_posiw, Uint16 wV_posiw, Uint8* pMapWk, Sint32 VramBase) {
  mapwrt3(wH_posiw, wV_posiw, pMapWk, VramBase, 65520, 15);
}

void mapwrt3(Uint16 wH_posiw, Uint16 wV_posiw, Uint8* pMapWk, Sint32 VramBase, Uint16 wD4, Uint16 wD6) {
  POINT TilePoint;

  do {
    vramadrset(wH_posiw, wV_posiw, 0, wD4, &TilePoint);

hblockwrt(&TilePoint, VramBase, wH_posiw, wV_posiw, 0, wD4, pMapWk, 31);

wD4 += 16;
  } while ((Sint16)--wD6 >= 0);

}

void mapwrt_z81a(Uint8* pMapWk, Sint32 VramBase) {
  Uint16 wD0, wD4, wD6;

  wD4 = 65520;
  wD6 = 15;

  do {
    wD0 = scrb_v_posit.w.h + wD4 & 1008;

    mapwrt_sub(&z81awrttbl[1], wD0, wD4, pMapWk, VramBase);
    wD4 += 16;
  } while ((Sint16)--wD6 >= 0);
}

void mapwrt_sub(Uint8* pWrttbl, Uint16 wD0, Uint16 wD4, Uint8* pMapWk, Sint32 VramBase) {
  POINT TilePoint;
  Uint16 wH_posiw, wV_posiw;
  Uint16 wD5;

  wD0 /= 16;
  wD0 = pWrttbl[wD0];

  wH_posiw = mapwrt_tbl[wD0]->w.h;
  wV_posiw = mapwrt_tbl[wD0 + 1]->w.h;
  if (wD0) {
    wD5 = 65520;
    vramadrset(wH_posiw, wV_posiw, wD5, wD4, &TilePoint);

hblockwrt(&TilePoint, VramBase, wH_posiw, wV_posiw, wD5, wD4, pMapWk, 21);

}
  else {

    wD5 = 0;
    vramadrset1(wH_posiw, wV_posiw, wD5, wD4, &TilePoint);

hblockwrt1(&TilePoint, VramBase, wH_posiw, wV_posiw, wD5, wD4, pMapWk, 31);
  }

}

void mapinit(void) {
  Sint16 tbl[9] = {
    3584, 8960, -1,
    3584, 8960, -1,
    3584, 8960, -1
  };
  char tbl0[3] = {
    2, 4, 2
  };
  Sint32 EnkeiNo, TileNo, BmpNo, i;
  Sint16 *pTbl, poswk;

  EnkeiNo = 0;
  pTbl = tbl;

  while ((poswk = *pTbl++) >= 0) {
    if (actwk[0].xposi.w.h <= poswk) break;

++EnkeiNo;
  }

TileNo = 592;
  if (EnkeiNo == 0) {
    i = 95;
    BmpNo = 0;

    for ( ; i > 0; --i) {
      ChangeTileBmp(TileNo++, BmpNo++);
    }
  }

colorset2(mapinittbl.colorno2);
  colorset(mapinittbl.colorno2);

if (play_start & 2) divdevset();
}

void mapset(void) {}

void divdevset() {}

void enecginit(void) {}

static void mapset2(Uint8* pMapDst, Sint32 idx) {
  Sint32 h, v, hwk;
  Uint8 *pZonetbl, *pMapwk;

  pZonetbl = zonemaptbl[idx];
  h = *pZonetbl++;
  v = *pZonetbl++;
  for ( ;v >= 0; --v) {
    pMapwk = pMapDst;

    for (hwk = h; hwk >= 0; --hwk) {
      *pMapwk++ = *pZonetbl++;
    }

    pMapDst += 64;
  }
}
