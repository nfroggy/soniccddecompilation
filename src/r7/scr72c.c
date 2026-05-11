#include "../equ.h"
#include "scr71a.h"
#include "../impfuncs.h"
#include "../save.h"
#include "../scrchk.h"
#include "col7a.h"

static Sint32 z71aline(Sint32 hsCount);
static Sint32 z71aline0(Sint32 hsCount);
static void zonescrsetsub0(Sint16 VPosi, Uint16 offs);

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
Uint16 scr_dir_tbl[6] = { 4, 0, 11927, 0, 1296, 96 };
extern Uint8 mapwkb[8][64];
extern Uint8 mapwka[8][64];
extern map_init_data mapinittbl;

void enecginit(void) {}

void divdevset() {}

void scr_set(void) {
  Sint32 i;

  scrh_flag = scrv_flag = scr_die.b.h = scr_timer.b.h = zone_flag.b.h = 0;

  i = 0;
  scrar_no = scr_dir_tbl[i++];
  scralim_left = scr_dir_tbl[i];
  scralim_n_left = scr_dir_tbl[i++];
  scralim_right = scr_dir_tbl[i];
  scralim_n_right = scr_dir_tbl[i++];
  scralim_up = scr_dir_tbl[i];
  scralim_n_up = scr_dir_tbl[i++];
  scralim_down = scr_dir_tbl[i];
  scralim_n_down = scr_dir_tbl[i++];

  scra_h_keep = scralim_left + 576;
  scra_h_count = 16;
  scra_v_count = 16;

  scra_vline = scr_dir_tbl[i++];
  scra_hline = 160;

  playposiset();
}

void playposiset(void) {
  Uint16 playpositbl[2] = {
    64,
    652
  };
  Uint8 playmapnotbl[4] = {
    132,
    134,
    127,
    127
  };
  Sint16 xWk, yWk;
  Sint32 i;
  if (plflag != 0) {

    playload();
    xWk = actwk[0].xposi.w.h;
    yWk = actwk[0].yposi.w.h;
    if (yWk < 0) yWk = 0;
  }
  else {

i = 0;
    xWk = actwk[0].xposi.w.h = playpositbl[i++];
    yWk = actwk[0].yposi.w.h = playpositbl[i++];
  }

  if ((Uint16)xWk > 160) {
    xWk -= 160;
  }
  else {
    xWk = 0;
  }

if ((Uint16)scralim_right < (Uint16)xWk) {
    xWk = scralim_right;
  }
  scra_h_posit.w.h = xWk;

  if ((Uint16)yWk > 96) {
    yWk -= 96;
  }
  else {
    yWk = 0;
  }

  if (scralim_down < yWk) {
    yWk = scralim_down;
  }

  scra_v_posit.w.h = yWk;

  scrbinit(yWk, xWk);

  i = 0;
  loopmapno = playmapnotbl[i++];
  loopmapno2 = playmapnotbl[i++];
  ballmapno = playmapnotbl[i++];
  ballmapno2 = playmapnotbl[i++];

}

void scrbinit(Sint16 yWk, Sint16 xWk) {
  uint_union data;

data.l = 0;
  data.w.h = yWk;
  data.l = (data.l >> 2) + (data.l >> 5);
  scrb_v_posit.l = data.w.l;
  scrc_v_posit.w.h = data.w.h;
  scrz_v_posit.w.h = data.w.h;

  scrc_h_posit.w.h = (Uint16)(xWk >> 1);
  scrz_h_posit.w.h = (Uint16)((xWk >> 2) + (xWk >> 3));
  scrb_h_posit.w.h = (Uint16)((xWk >> 4) * 3);
}

void scroll(void) {
  uint_union data;
  Sint32 hsCount;
  Sint32 i;

  if (scroll_start.b.h) return;

scrflaga.w = scrflagb.w = scrflagc.w = scrflagz.w = 0;
  scrchk();
  scroll_h();
  scroll_v();

  vscroll.w.h = scra_v_posit.w.h;
  vscroll.w.l = scrb_v_posit.w.h;

scrollz_h((scra_hz << 6) + (scra_hz << 5), 64);
  scrollc_h(scra_hz << 7, 16);
  scrollb_h((long int)(scra_hz << 4) * 3, 4);

  data.l = 0;
  data.w.l = scra_v_posit.w.h;
  data.w.l = (data.w.l >> 2) + (data.w.l >> 5);
  scrollb_v(data.w.l);

  vscroll.w.l = scrb_v_posit.w.h;
  scrc_v_posit.w.h = scrb_v_posit.w.h;
  scrz_v_posit.w.h = scrb_v_posit.w.h;
  scrflagb.b.h |= scrflagz.b.h | scrflagc.b.h;
  scrflagz.b.h = 0;
  scrflagc.b.h = 0;

  hsCount = 0;

hsCount = z71aline(hsCount);

  for (i = 0; i < 4; ++i) {

    hscrollwork[hsCount] = -scrb_h_posit.w.h;
    ++hsCount;
  }

  hsCount = z71aline0(hsCount);

  for (i = 0; i < 12; ++i) {

    hscrollwork[hsCount] = -scrc_h_posit.w.h;
    ++hsCount;
  }

data.w.l = scrb_v_posit.w.h;
  data.w.l &= 1016;
  data.w.l >>= 2;
  zonescrsetsub0(scrb_v_posit.w.h, data.w.l / 2);
}

Sint32 z71aline(Sint32 hsCount) {
  Sint32 temp;
  int_union data;
  Sint32 i, j;
  Sint32 z71ascrtbl[39] = {
    1, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0
  };

  temp = scra_h_posit.w.h - scrb_h_posit.w.h;
  temp <<= 6;
  temp /= 39;
  temp <<= 10;
  data.l = 0;
  data.w.l = scrb_h_posit.w.h;
  hsCount += 46;
  for (i = 0; i < 39; ++i) {

for (j = -1; j < z71ascrtbl[38 - i]; ++j) {

--hsCount;
      hscrollwork[hsCount] = -data.w.l;
    }
    data.l = (Uint32)data.l >> 16 & 65535 | data.l << 16 & -65536;
    data.l += temp;
    data.l = (Uint32)data.l >> 16 & 65535 | data.l << 16 & -65536;
  }
  hsCount += 46;
  return hsCount;
}

Sint32 z71aline0(Sint32 hsCount) {
  Sint32 temp;
  int_union data;
  Sint32 i, j;
  Sint32 z71ascrtbl0[10] = {
    7, 3, 3, 5,
    3, 3, 0, 0,
    0, 0
  };

  temp = scrz_h_posit.w.h - scrb_h_posit.w.h;
  temp <<= 6;
  temp /= 10;
  temp <<= 10;
  data.l = 0;
  data.w.l = scrb_h_posit.w.h;
  data.l = (Uint32)data.l >> 16 & 65535 | data.l << 16 & -65536;
  data.l += temp;
  data.l = (Uint32)data.l >> 16 & 65535 | data.l << 16 & -65536;
  for (i = 0; i < 10; ++i) {

for (j = -1; j < z71ascrtbl0[9 - i]; ++j) {

hscrollwork[hsCount] = -data.w.l;
      ++hsCount;
    }
    data.l = (Uint32)data.l >> 16 & 65535 | data.l << 16 & -65536;
    data.l += temp;
    data.l = (Uint32)data.l >> 16 & 65535 | data.l << 16 & -65536;
  }
  return hsCount;
}

void zonescrsetsub0(Sint16 VPosi, Uint16 offs) {
  int_union* pHScrollBuff;
  int_union hsw;
  Sint32 i, j;

pHScrollBuff = lphscrollbuff;

VPosi &= 7;
  hsw.l = 0;
  hsw.w.l = hscrollwork[offs++];

  j = VPosi;
  while (j < 8) {

    pHScrollBuff->l = hsw.l;
    ++pHScrollBuff;
    ++j;
  }
  for (i = 0; i < 29; ++i) {

    hsw.w.l = hscrollwork[offs++];
    for (j = 0; j < 8; ++j) {

      pHScrollBuff->l = hsw.l;
      ++pHScrollBuff;
    }
  }
}

void scroll_h(void) {
  Uint16 wD4;

  wD4 = scra_h_posit.w.h;
  scrh_move();
  if (((Uint8)(scra_h_posit.w.h & 16) ^ scra_h_count) != 0) {
    scra_h_count ^= 16;
    if ((Sint16)wD4 > scra_h_posit.w.h) {
      scrflaga.b.h |= 4;
    }
    else {

      scrflaga.b.h |= 8;
    }
  }

}

void scrh_move(void) {
  Uint16 wD0;

  wD0 = actwk[0].xposi.w.h;
  wD0 -= scra_h_posit.w.h;
  wD0 -= scra_hline;
  if (wD0 == 0) {

    scra_hz = 0;
  }
  else if ((Sint16)wD0 < 0) {
    left_check(wD0);
  }
  else {
    right_check(wD0);
  }

}

void right_check(Uint16 wD0) {
  Uint16 wD1;

  if ((Sint16)wD0 > 16) {
    wD0 = 16;
  }

  wD0 += scra_h_posit.w.h;
  if (scralim_right < (Sint16)wD0) {
    wD0 = scralim_right;
  }

  wD1 = wD0;
  wD1 -= scra_h_posit.w.h;
  if (wD1 & 32768) wD1 <<= 8, wD1 |= 32768; else wD1 <<= 8;
  scra_h_posit.w.h = wD0;
  scra_hz = wD1;

}

void left_check(Uint16 wD0) {
  Uint16 wD1;

  if ((Sint16)wD0 < -16) {
    wD0 = -16;
  }

  wD0 += scra_h_posit.w.h;
  if (scralim_left > (Sint16)wD0) {
    wD0 = scralim_left;
  }

  wD1 = wD0;
  wD1 -= scra_h_posit.w.h;
  if (wD1 & 32768) wD1 <<= 8, wD1 |= 32768; else wD1 <<= 8;
  scra_h_posit.w.h = wD0;
  scra_hz = wD1;

}

void scroll_v(void) {
  Uint16 wD0;

  wD0 = actwk[0].yposi.w.h - scra_v_posit.w.h;
  if (actwk[0].cddat & 4) {
    wD0 -= 5;
  }

  if (actwk[0].cddat & 2) {
    wD0 += 32;
    if ((Uint16)scra_vline > wD0) {
      wD0 -= scra_vline;
      sv_move_main2(wD0);
      return;
    }
    wD0 -= scra_vline;
    if (wD0 >= 64) {
      wD0 -= 64;
      sv_move_main2(wD0);
      return;
    }
    wD0 -= 64;
    if (limmoveflag == 0) goto label1;
    sv_move_sub2();
    return;
  }

wD0 -= scra_vline;
  if (wD0) {
    sv_move_main(wD0);
    return;
  }
  if (limmoveflag == 0) {
    sv_move_sub2();
    return;
  }

label1:
  scra_vz = 0;

}

void sv_move_main(Uint16 wD0) {
  Uint16 wD1;

  if (scra_vline != 96) {
    sv_move_main1(wD0);
  }
  else {

    wD1 = actwk[0].mspeed.w;
    if ((Sint16)wD1 < 0) {
      wD1 = -(Sint16)wD1;
    }

    if (wD1 >= 2048) {
      sv_move_main2(wD0);
    }
    else {
      if ((Sint16)wD0 > 6) {
        sv_move_plus(1536);
      }
      else if ((Sint16)wD0 < -6) {
        sv_move_minus(1536);
      }
      else {
        sv_move_sub(wD0);
      }
    }
  }
}

void sv_move_main1(Uint16 wD0) {

  if ((Sint16)wD0 > 2) {
    sv_move_plus(512);
  }
  else if ((Sint16)wD0 < -2) {
    sv_move_minus(512);
  }
  else {
    sv_move_sub(wD0);
  }

}

void sv_move_main2(Uint16 wD0) {

  if ((Sint16)wD0 > 16) {
    sv_move_plus(4096);
  }
  else if ((Sint16)wD0 < -16) {
    sv_move_minus(4096);
  }
  else {
    sv_move_sub(wD0);
  }

}

void sv_move_sub2(void) {
  limmoveflag = 0;
  sv_move_sub(0);
}

void sv_move_sub(Uint16 wD0) {
  int_union lD1;

  lD1.w.h = 0;
  lD1.w.l = wD0;
  lD1.l += scra_v_posit.w.h;
  if ((Sint16)wD0 > 0) {
    scrv_down_ch(lD1);
  }
  else {
    scrv_up_ch(lD1);
  }

}

void sv_move_minus(Uint16 wD1) {
  int_union lD1;
  Uint16 wk;

  lD1.l = -(Sint16)wD1;
  lD1.l <<= 8;
  lD1.l += scra_v_posit.l;

  wk = lD1.w.h;
  lD1.w.h = lD1.w.l;
  lD1.w.l = wk;
  scrv_up_ch(lD1);
}

void scrv_up_ch(int_union lD1) {
  if (scralim_up >= lD1.w.l) {
    if (lD1.w.l < -255) {
      lD1.w.l &= 2047;
      actwk[0].yposi.w.h &= 2047;
      scra_v_posit.w.h &= 2047;
      scrb_v_posit.w.h &= 1023;
    }
    else {

      lD1.w.l = scralim_up;
    }
  }
  scrv_move(lD1);

}

void sv_move_plus(Uint16 wD1) {
  int_union lD1;
  Uint16 wk;

  lD1.w.h = 0;
  lD1.w.l = wD1;
  lD1.l <<= 8;
  lD1.l += scra_v_posit.l;

  wk = lD1.w.h;
  lD1.w.h = lD1.w.l;
  lD1.w.l = wk;

  scrv_down_ch(lD1);

}

void scrv_down_ch(int_union lD1) {
  if (scralim_down <= lD1.w.l) {
    lD1.w.l -= 2048;
    if (lD1.w.l > 0) {
      actwk[0].yposi.w.h &= 2047;
      scra_v_posit.w.h -= 2048;
      scrb_v_posit.w.h &= 1023;
    }
    else {
      lD1.w.l = scralim_down;
    }
  }
  scrv_move(lD1);

}

void scrv_move(int_union lD1) {
  Uint16 wD4, wk;

  wD4 = scra_v_posit.w.h;

  wk = lD1.w.h;
  lD1.w.h = lD1.w.l;
  lD1.w.l = wk;

  scra_vz = (Uint16)((lD1.l - scra_v_posit.l) >> 8);
  scra_v_posit.l = lD1.l;

if (((Uint8)(scra_v_posit.w.h & 16) ^ scra_v_count) == 0) {
    scra_v_count ^= 16;
    if (scra_v_posit.w.h >= (Sint16)wD4) {

      scrflaga.b.h |= 2;
    }
    else {
      scrflaga.b.h |= 1;
    }
  }

}

void scrollb_hv(int_union lD4, int_union lD5) {
  int_union lD0, lD2, lD3;

  lD0.l = lD2.l = scrb_h_posit.l;
  lD0.l += lD4.l;
  scrb_h_posit.l = lD0.l;

  if (((Uint8)(scrb_h_posit.w.h & 16) ^ scrb_h_count) == 0) {

    scrb_h_count ^= 16;
    lD0.l -= lD2.l;
    if ((long int)lD0.l < 0) {
      scrflagb.b.h |= 4;
    }
    else {

      scrflagb.b.h |= 8;
    }
  }

lD0.l = lD3.l = scrb_v_posit.l;
  lD0.l += lD5.l;
  if ((long int)lD0.l < 0) lD0.l = 0;
  scrb_v_posit.l = lD0.l;

  if (((Uint8)(scrb_v_posit.w.h & 16) ^ scrb_v_count) == 0) {

    scrb_v_count ^= 16;
    lD0.l -= lD3.l;
    if ((long int)lD0.l < 0) {
      scrflagb.b.h |= 1;
    }
    else {

      scrflagb.b.h |= 2;
    }
  }

}

void scrollb_v(Uint16 wD0) {
  Uint16 wD3;

  wD3 = scrb_v_posit.w.h;
  scrb_v_posit.w.h = wD0;

  if (((Uint8)(wD0 & 16) ^ scrb_v_count) == 0) {
    scrb_v_count ^= 16;
    if ((Sint16)wD3 > (Sint16)wD0) {
      scrflagb.b.h |= 1;
    }
    else {

      scrflagb.b.h |= 2;
    }
  }

}

void scrollb_h(Sint32 lD4, Sint32 flagb) {
  int_union lD0, lD1, lD2;
  Uint8 bD3;

  lD2.l = scrb_h_posit.l;
  lD0.l = lD2.l + lD4;
  scrb_h_posit.l = lD0.l;

  lD1.w.h = lD0.w.l;
  lD1.w.l = lD0.w.h;
  lD1.w.l &= 16;

  bD3 = scrb_h_count;
  lD1.b.b4 ^= bD3;
  if (!lD1.b.b4) {
    scrb_h_count ^= 16;
    lD0.l -= lD2.l;
    if ((long int)lD0.l < 0) {
      scrflagb.b.h |= flagb;
    }
    else {

      scrflagb.b.h |= flagb << 1;
    }
  }

}

void scrollc_h(Sint32 lD4, Sint32 flagc) {
  int_union lD0, lD1, lD2;
  Uint8 bD3;

  lD2.l = scrc_h_posit.l;
  lD0.l = lD2.l + lD4;
  scrc_h_posit.l = lD0.l;

  lD1.w.h = lD0.w.l;
  lD1.w.l = lD0.w.h;
  lD1.w.l &= 16;

  bD3 = scrc_h_count;
  lD1.b.b4 ^= bD3;
  if (!lD1.b.b4) {
    scrc_h_count ^= 16;
    lD0.l -= lD2.l;
    if ((long int)lD0.l < 0) {
      scrflagc.b.h |= flagc;
    }
    else {

      scrflagc.b.h |= flagc << 1;
    }
  }

}

void scrollz_h(Sint32 lD4, Sint32 flagz) {
  int_union lD0, lD1, lD2;
  Uint8 bD3;

  lD2.l = scrz_h_posit.l;
  lD0.l = lD2.l + lD4;
  scrz_h_posit.l = lD0.l;

  lD1.w.h = lD0.w.l;
  lD1.w.l = lD0.w.h;
  lD1.w.l &= 16;

  bD3 = scrz_h_count;
  lD1.b.b4 ^= bD3;
  if (!lD1.b.b4) {
    scrz_h_count ^= 16;
    lD0.l -= lD2.l;
    if ((long int)lD0.l < 0) {
      scrflagz.b.h |= flagz;
    }
    else {

      scrflagz.b.h |= flagz << 1;
    }
  }

}

void scrollwrtadva(void) {
  Uint16 wH_posiw, wV_posiw;
  Sint32 VramBase;
  Uint8* pScrFlag;
  Uint8* pMapWk;

  VramBase = 24576;
  wH_posiw = scrb_h_posit.w.h;
  wV_posiw = scrb_v_posit.w.h;
  pMapWk = (Uint8*)mapwkb;
  pScrFlag = &scrflagb.b.h;
  scrollwrtb(pScrFlag, pMapWk, VramBase);

  wH_posiw = scrc_h_posit.w.h;
  wV_posiw = scrc_v_posit.w.h;
  pScrFlag = &scrflagc.b.h;
  scrollwrtc();

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
      if (wD0 != 0) {
        if (*pScrFlag & (Uint8)(1 << wD0)) {
          wH_posiw = vblockwrtbtbl[wD0]->w.h;
          wV_posiw = vblockwrtbtbl[wD0 + 1]->w.h;

          mapadrset(wH_posiw, wV_posiw, wD5, lD4.w.l, pMapWk, &BlkIndex);

vramadrset(wH_posiw, wV_posiw, wD5, lD4.w.l, &TilePoint);

          blockwrt(VramBase, &TilePoint, BlkIndex);
        }
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
  Sint32 base = 0;
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
  int_union temp;

wD4 = 65520;
  wD6 = 15;

  do {
    temp.l = 0;
    temp.w.l = scrb_v_posit.w.h;
    wD0 = scrb_v_posit.w.h;
    wD0 += wD4 & 496;
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

  colorset2(mapinittbl.colorno2);
  colorset(mapinittbl.colorno2);

if (play_start & 2) divdevset();
}

void mapset(void) {}
