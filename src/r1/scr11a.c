#include "../equ.h"
#include "scr11a.h"
#include "../impfuncs.h"
#include "../save.h"
#include "../scrchk.h"
#include "col1a.h"

static void scr11a_test(void);

static Uint8 z81awrttbl[129] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0
};
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
extern Uint8 mapwka[8][64];
extern Uint8 mapwkb[8][64];

void enecginit(void) {}

void divdevset() {}

sprite_status* main_chk(void) {
  if (main_play == 0) {
    return &actwk[0];
  }
  return &actwk[1];
}

void scr_set(void) {

Uint16 scr_dir_tbl[6] = {
    4, 0, 10391, 0, 1808, 96
  };
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
    1899
  };
  Uint8 playmapnotbl[4] = {
    127,
    127,
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

  scrbinit(xWk, yWk);

  i = 0;
  loopmapno = playmapnotbl[i++];
  loopmapno2 = playmapnotbl[i++];
  ballmapno = playmapnotbl[i++];
  ballmapno2 = playmapnotbl[i++];

}

void scrbinit(Sint16 xWk, Sint16 yWk) {
  Sint32 i;

if ((Uint16)actwk[0].xposi.w.h >= 2048) {
    if ((Uint16)yWk >= 480) {
      yWk = (yWk - 480) / 2 + 480;
    }
  }

  scrb_v_posit.w.h = yWk;
  scrb_v_posit.w.l = 0;
  scrc_v_posit.w.h = yWk;
  scrz_v_posit.w.h = yWk;

  scrb_h_posit.w.h = (Uint16)(xWk / 2);
  scrz_h_posit.w.h = (Uint16)(xWk / 8 * 3);
  scrc_h_posit.w.h = (Uint16)(xWk / 16 * 3);

  for (i = 0; i < 19; ++i) {
    linework[i] = 0;
  }

}

void scroll(void) {
  Sint32 LineSpdTbl[19] = {
    65536, 57344, 49152,
    40960, 32768, 24576,
    18432, 16384, 10240,
     8192,  8192, 16384,
    32768, 49152, 65536,
    49152, 32768, 16384,
    8192
  };
  Uint8 z11a_cnttbl[10] = {
    3, 5, 5, 3,
    7, 7, 7, 5,
    3, 1
  };
  Uint8 z11a_cnttbl2[9] = {
    1, 3, 9, 3, 15,
    7, 5, 3, 1
  };
  Uint16 lposi_tbl[6] = {
    640, 224, 1920, 128, 32767, 864
  };
  Sint32 HspdWk;
  Sint32* pLinework;
  Sint16* pHScrollWork;
  Sint16 wWk;
  int_union* pHScrollBuff;
  Sint16 yWk;
  Sint32 i, j;
  Sint32 Lposi_TblCnt;
  Uint16 wD1, wD4, wD5, wD6;
  int_union lD0, lD2, lD3;

  if (scroll_start.b.h != 0) return;

scrflaga.w = scrflagb.w = scrflagc.w = scrflagz.w = 0;
  scrchk();
  scroll_h();
  scroll_v();

  vscroll.w.h = scra_v_posit.w.h;
  vscroll.w.l = scrb_v_posit.w.h;

  if ((actwk[0].actfree[2] & 2) != 0 && scra_hz != 0) {
    HspdWk = actwk[0].xspeed.w << 8;
  }
  else {
    HspdWk = 0;
  }

  scrollz_h((scra_hz << 5) + HspdWk, 64);
  scrollc_h((long int)(scra_hz << 4) * 3 + (long int)HspdWk * 2, 16);
  scrollb_h((scra_hz << 7) + HspdWk, 4);

  yWk = scra_v_posit.w.h;
  if ((Uint16)actwk[0].xposi.w.h >= 2048) {
    if ((Uint16)yWk >= 480) {
      yWk = (yWk - 480) / 2 + 480;
    }
  }

  scrollb_v(yWk);
  vscroll.w.l = scrb_v_posit.w.h;
  scrc_v_posit.w.h = scrb_v_posit.w.h;
  scrz_v_posit.w.h = scrb_v_posit.w.h;

  scrflagb.b.h |= scrflagz.b.h;
  scrflagb.b.h |= scrflagc.b.h;
  scrflagz.b.h = 0;
  scrflagc.b.h = 0;

for (i = 0; i < 19; ++i) {
    linework[i] += LineSpdTbl[i];
  }

  lD0.w.h = (Uint16)-scra_h_posit.w.h;

  pHScrollWork = hscrollwork;
  pLinework = linework;

for (i = 0; i < 10; ++i) {

    wWk = (((Uint32)(*pLinework++ & -65536) >> 16) + scrz_h_posit.w.h) * -1;

for (j = 0; z11a_cnttbl[i] >= j; ++j) {
      *pHScrollWork++ = wWk;
    }
  }

for (i = 0; i < 20; ++i) {
    *pHScrollWork++ = -scrc_h_posit.w.h;
  }

for (i = 0; i < 4; ++i) {
    *pHScrollWork++ = -scrb_h_posit.w.h;
  }

for (i = 0; i < 56; ++i) {
    *pHScrollWork++ = -scrb_h_posit.w.h;
  }

for (i = 0; i < 4; ++i) {
    *pHScrollWork++ = -scrb_h_posit.w.h;
  }

for (i = 0; i < 20; ++i) {
    *pHScrollWork++ = -scrc_h_posit.w.h;
  }

for (i = 0; i < 9; ++i) {
    wWk = (((Uint32)(*pLinework++ & -65536) >> 16) + scrz_h_posit.w.h) * -1;

    for (j = 0; z11a_cnttbl2[i] >= j; ++j) {
      *pHScrollWork++ = wWk;
    }
  }

for (i = 0; i < 20; ++i) {
    *pHScrollWork++ = -scrc_h_posit.w.h;
  }

for (i = 0; i < 4; ++i) {
    *pHScrollWork++ = -scrb_h_posit.w.h;
  }

for (i = 0; i < 16; ++i) {
    *pHScrollWork++ = -scrb_h_posit.w.h;
  }

  pHScrollBuff = lphscrollbuff;
  pHScrollWork = hscrollwork;
  Lposi_TblCnt = 0;
  lD2.w.l = wD4 = scrb_v_posit.w.h;
  pHScrollWork += (scrb_v_posit.w.h & 2040) / 8;
  wD1 = 29;

if (lposi_tbl[Lposi_TblCnt] > wD4) {

label1:
    lD2.w.l &= 7;
    wD4 -= lD2.w.l;
    wD4 += 8;
    lD0.w.l = *pHScrollWork++;

    i = 8 - lD2.w.l;
    goto label3;

label2:
    do {
      if ((Sint16)wD1 < 0) break;

if (lposi_tbl[Lposi_TblCnt] <= wD4) goto label4;

wD4 += 8;
      lD0.l = *pHScrollWork++;

      i = 8;
label3:
      for ( ; i > 0; --i) {
        pHScrollBuff->l = lD0.l;
        ++pHScrollBuff;
      }
    } while ((Sint16)--wD1 >= 0);

return;
  }

label4:
  wD5 = wD4 - lposi_tbl[Lposi_TblCnt];
  wD6 = lposi_tbl[Lposi_TblCnt + 1];
  if (wD5 > wD6) {
    wD6 -= wD5;
  }
  else {
    if (wD5 == wD6) {
      wD6 -= wD5;
    }
    else {
      wD6 -= wD5;

lD2.l = (Sint16)(scra_h_posit.w.h - scrb_h_posit.w.h);
      lD2.l = lD2.l << 8;

      lD2.w.l = (Uint16)(lD2.l / 224);
      lD2.l = lD2.w.l;
      lD2.l = lD2.l << 8;

      lD3.l = 0;
      lD3.w.l = scrb_h_posit.w.h;
      if ((Sint16)--wD5 >= 0) {

do {
          lD3.l = (Uint32)lD3.l >> 16 & 65535 | lD3.l << 16 & -65536;
          lD3.l += lD2.l;
          lD3.l = (Uint32)lD3.l >> 16 & 65535 | lD3.l << 16 & -65536;
        } while ((Sint16)--wD5 >= 0);
      }

wD5 = wD6;
      wD5 >>= 3;
      if (wD5 > wD1) {

        wD1 -= wD5;
        wD5 = -(Sint16)wD1;
        wD5 <<= 3;
        wD6 -= wD5;
        if (wD6 == 0) goto label5;

}
      else {
        wD1 -= wD5;
      }

      --wD6;

      do {
        pHScrollBuff->w.h = 0;
        pHScrollBuff->w.l = (Uint16)-lD3.w.l;
        ++pHScrollBuff;
        lD3.l = (Uint32)lD3.l >> 16 & 65535 | lD3.l << 16 & -65536;
        lD3.l += lD2.l;
        lD3.l = (Uint32)lD3.l >> 16 & 65535 | lD3.l << 16 & -65536;
        ++wD4;
        if ((wD4 & 7) == 0) {
          ++pHScrollWork;
        }

      } while ((Sint16)--wD6 >= 0);
    }
label5:
    Lposi_TblCnt += 2;
    goto label2;
  }

Lposi_TblCnt += 2;
  lD2.w.l = wD4;
  goto label1;

}

void scroll_h(void) {
  Uint16 wD4;

  wD4 = scra_h_posit.w.h;
  scrh_move();
  if ((Uint8)(scra_h_posit.w.h & 16) ^ scra_h_count) {
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
    wD0 = 65520;
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
    return;
  }

  wD1 = actwk[0].mspeed.w;
  if ((Sint16)wD1 < 0) {
    wD1 = -(Sint16)wD1;
  }

  if (wD1 >= 2048) {
    sv_move_main2(wD0);
    return;
  }
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
  lD1.l = lD1.l << 8;
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
      actwk[0].yposi.w.h &= 32767;
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
  lD1.l = lD1.l << 8;
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
  scrb_h_posit.l = lD0.l;

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

      scrflagb.b.h |= flagb * 2;
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

      scrflagc.b.h |= flagc * 2;
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

      scrflagz.b.h |= flagz * 2;
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

  ScreenNo = pMapWk[i];

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
    lpw += (ScreenNo - 1 << 4) << 4;
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

ScreenNo = pMapWk[xOffs / 256 + yOffs / 256 * 64];
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
    lpw += (ScreenNo - 1 << 4) << 4;
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

static void scr11a_test(void) {
  sOutputDebugString("\x8a\xc3\x82\xa6\x82\xc4\x82\xcd\x82\xa2\x82\xaf\x82\xc8\x82\xa2\n");
}

void mapwrt(void) {
  Sint32 VramBase;
  Uint16 wH_posiw, wV_posiw;
  Uint8* pMapWk;

  wH_posiw = scra_h_posiw.w.h;
  wV_posiw = scra_v_posiw.w.h;
  pMapWk = (Uint8*)mapwka;
  VramBase = 16384;
  scr11a_test();
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

  wD4 = 65520;
  wD6 = 15;

  do {
    wD0 = scrb_v_posit.w.h + wD4 & 2032;

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

mapset();

colorset2(5);
  colorset(5);

if (plflag != 0) {
    enecginit();
  }

  if (play_start & 2) {

    divdevset();
  }

}

void mapset(void) {}
