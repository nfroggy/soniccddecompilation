#include "../equ.h"
#include "scr12c.h"
#include "../impfuncs.h"
#include "../save.h"
#include "../scrchk.h"
#include "col1c.h"

static void playposiset(void);
static void scrbinit(Sint16 xWk, Sint16 yWk);
static void zonescrsetsub0(Sint16 yoffs, Sint16 yline, int_union **pHscrbuf,
                           Sint16 *pHscrwk);
static void scrollb_hv(Sint32 xOffs, Sint32 yOffs);
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

static Uint8 z12dwrttbl[33] = {0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6,
                               6, 2, 4, 4, 4, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static int_union *vblockwrtbtbl[8] = {
    &scrb_h_posiw, &scrb_v_posiw, &scrb_h_posiw, &scrb_v_posiw,
    &scrc_h_posiw, &scrc_v_posiw, &scrz_h_posiw, &scrz_v_posiw};
static int_union *mapwrt_tbl[8] = {&scrb_h_posiw, &scrb_v_posiw, &scrb_h_posiw,
                                   &scrb_v_posiw, &scrc_h_posiw, &scrc_v_posiw,
                                   &scrz_h_posiw, &scrz_v_posiw};
extern Uint8 mapwka[8][64];
extern Uint8 mapwkb[8][64];
extern map_init_data mapinittbl;

void enecginit(void) {}

void divdevset() {}

sprite_status *main_chk(void) {
    if (!main_play)
        return &actwk[0];
    return &actwk[1];
}

void scr_set(void) {
    Uint16 scr_dir_tbl[6] = {4, 0, 10391, 0, 784, 96};
    Sint32 i;

    scrh_flag = scrv_flag = scr_die.b.h = scr_timer.b.h = zone_flag.b.h = 0;

    i = 0;
    scrar_no = scr_dir_tbl[i++];
    scralim_left = scralim_n_left = scr_dir_tbl[i++];
    scralim_right = scralim_n_right = scr_dir_tbl[i++];
    scralim_up = scralim_n_up = scr_dir_tbl[i++];
    scralim_down = scralim_n_down = scr_dir_tbl[i++];

    scra_h_keep = scralim_left + 576;
    scra_h_count = 16;
    scra_v_count = 16;

    scra_vline = scr_dir_tbl[i++];
    scra_hline = 160;

    playposiset();
}

static void playposiset(void) {
    Uint16 playpositbl[2] = {80, 389};
    Uint16 endplpositbl[16] = {80,   944, 3744, 1132, 5968, 189,  2560, 1580,
                               2992, 76,  5488, 364,  432,  1836, 5120, 684};
    Uint8 playmapnotbl[4] = {145, 182, 127, 127};
    Sint16 xWk, yWk;

    if (plflag) {
        playload();
        xWk = actwk[0].xposi.w.h;
        yWk = actwk[0].yposi.w.h;
        if (yWk < 0)
            yWk = 0;
    } else {
        if (demoflag.w & -32768) {
            xWk = actwk[0].xposi.w.h = endplpositbl[(enddemono - 1) * 2];
            yWk = actwk[0].yposi.w.h = endplpositbl[(enddemono - 1) * 2 + 1];
        } else {
            xWk = actwk[0].xposi.w.h = playpositbl[demoflag.w * 2];
            yWk = actwk[0].yposi.w.h = playpositbl[demoflag.w * 2 + 1];
        }
    }

    if (xWk > 160)
        xWk -= 160;
    else
        xWk = 0;
    if (scralim_right < xWk)
        xWk = scralim_right;
    scra_h_posit.w.h = xWk;

    if (yWk > 96)
        yWk -= 96;
    else
        yWk = 0;
    if (scralim_down < yWk)
        yWk = scralim_down;
    scra_v_posit.w.h = yWk;

    scrbinit(xWk, yWk);

    loopmapno = playmapnotbl[0];
    loopmapno2 = playmapnotbl[1];
    ballmapno = playmapnotbl[2];
    ballmapno2 = playmapnotbl[3];
}

static void scrbinit(Sint16 xWk, Sint16 yWk) {
    int_union lYwk;

    lYwk.w.h = yWk;
    lYwk.w.l = 0;

    lYwk.l = (lYwk.l >> 4) * 2;
    scrb_v_posit.l = lYwk.l;
    scrc_v_posit.w.h = (Uint16)(yWk / 16) * 2;
    scrz_v_posit.w.h = (Uint16)(yWk / 16) * 2;

    scrb_h_posit.w.h = (Uint16)(xWk / 8);
    scrc_h_posit.w.h = (Uint16)(xWk / 16 * 3);
    scrz_h_posit.w.h = (Uint16)(xWk / 32 * 3);

    sMemSet(hscrollwork, 0, 16);
}

void scroll(void) {
    Sint16 *pHScrollWork;
    int_union *pHScrollBuff;
    Sint32 i;
    Sint32 j;
    Uint16 wD1;

    int_union lD0, lD2;
    Sint32 lD4, lD5;
    static Sint32 scaddtbl[4] = {65536, 49152, 32768, 16384};
    Uint8 z12d_cnttbl[4] = {0, 4, 3, 3};

    if (scroll_start.b.h)
        return;

    scrflaga.w = scrflagb.w = scrflagc.w = scrflagz.w = 0;

    scrchk();

    scroll_h();
    scroll_v();

    vscroll.w.h = scra_v_posit.w.h;
    vscroll.w.l = scrb_v_posit.w.h;

    scrollz_h((scra_hz << 3) * 3, 6);
    scrollc_h((scra_hz << 4) * 3, 4);

    lD4 = scra_hz << 5;
    lD5 = (scra_vz << 4) * 2;
    scrollb_hv(lD4, lD5);

    vscroll.w.l = scrc_v_posit.w.h = scrz_v_posit.w.h = scrb_v_posit.w.h;

    scrflagb.b.h = scrflagb.b.h | scrflagz.b.h | scrflagc.b.h;
    scrflagz.b.h = scrflagc.b.h = 0;

    for (i = 0; i < 4; ++i) {
        hscrollwork_add_pair(i * 2, scaddtbl[i]);
    }

    lD0.w.h = -scra_h_posit.w.h;
    pHScrollWork = &hscrollwork[8];
    for (i = 3; i >= 0; --i) {

        wD1 = -(hscrollwork_get_pair((3 - i) * 2).w.h +
                scrz_h_posit.w.h);
        for (j = z12d_cnttbl[i]; j >= 0; --j) {

            *pHScrollWork++ = wD1;
        }
    }

    lD0.w.l = -scrz_h_posit.w.h;
    for (i = 0; i < 8; ++i) {

        *pHScrollWork++ = lD0.w.l;
    }

    lD0.w.l = -scrb_h_posit.w.h;
    for (i = 0; i < 2; ++i) {

        *pHScrollWork++ = lD0.w.l;
    }

    lD0.w.l = -scrc_h_posit.w.h;
    for (i = 0; i < 6; ++i) {

        *pHScrollWork++ = lD0.w.l;
    }

    pHScrollBuff = lphscrollbuff;
    pHScrollWork = &hscrollwork[8];
    lD2.w.l = lD0.w.l = scrb_v_posit.w.h;
    lD0.w.l = (Uint16)(lD0.w.l & 504) >> 2;
    wD1 = 28;
    pHScrollWork += lD0.w.l / 2;
    zonescrsetsub0(lD2.w.l, wD1, &pHScrollBuff, pHScrollWork);
}

static void zonescrsetsub0(Sint16 yoffs, Sint16 yline, int_union **pHscrbuf,
                           Sint16 *pHscrwk) {
    Sint32 i, j;
    int_union ldwk;

    ldwk.l = 0;
    ldwk.w.h = -scra_h_posit.w.h;

    for (i = 0, j = yoffs & 7; yline >= i; ++i, j = 0) {
        ldwk.w.l = *pHscrwk;
        ++pHscrwk;

        for (; j < 8; ++j, ++*pHscrbuf) {
            (*pHscrbuf)->l = ldwk.l;
        }
    }
}

static void scrollb_hv(Sint32 xOffs, Sint32 yOffs) {
    Sint32 lwk, lsv;
    int_union ldwk;

    lsv = scrb_h_posit.l;
    ldwk.l = scrb_h_posit.l = lwk = lsv + xOffs;
    if (((Uint8)(ldwk.w.h & 16) ^ scrb_h_count) == 0) {
        scrb_h_count ^= 16;
        if (lwk - lsv < 0) {
            scrflagb.b.h |= 4;
        } else {
            scrflagb.b.h |= 8;
        }
    }

    lsv = scrb_v_posit.l;
    ldwk.l = scrb_v_posit.l = lwk = lsv + yOffs;
    if (((Uint8)(ldwk.w.h & 16) ^ scrb_v_count) == 0) {
        scrb_v_count ^= 16;
        if (lwk - lsv < 0) {
            scrflagb.b.h |= 1;
        } else {
            scrflagb.b.h |= 2;
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
        } else {
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
        } else {
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
        } else
            scrflaga.b.h |= 8;
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
    } else {
        if (xwk > 16) {
            xwk = 16;
        }

        xwk += scra_h_posit.w.h;
        if (xwk > scralim_right) {
            xwk = scralim_right;
        }
    }

    scra_hz = xwk - scra_h_posit.w.h << 8;
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
        if (!limmoveflag)
            goto label1;
        sv_move_sub2();
        return;
    }

    if ((ywk -= scra_vline) != 0) {
        sv_move_main(ywk);
        return;
    } else if (limmoveflag) {
        sv_move_sub2();
        return;
    }

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
        } else {

            if (yPos > 6) {
                sv_move_plus(1536);
            } else if (yPos < -6) {
                sv_move_minus(1536);
            } else
                sv_move_sub(yPos);
        }
    } else {
        if (yPos > 2) {
            sv_move_plus(512);
        } else if (yPos < -2) {
            sv_move_minus(512);
        } else {
            sv_move_sub(yPos);
        }
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
    spdwk.l = (spdwk.l >> 8) | (spdwk.l << 24);
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
    Uint8 *pScrFlag;
    Uint8 *pMapWk;
    int_union lD4;
    Uint16 wD5;

    pScrFlag = &scrflagbw.b.h;
    wH_posiw = scrb_h_posiw.w.h;
    wV_posiw = scrb_v_posiw.w.h;
    pMapWk = (Uint8 *)mapwkb;
    VramBase = 24576;
    scrollwrtb(pScrFlag, pMapWk, VramBase);

    pScrFlag = &scrflagaw.b.h;
    wH_posiw = scra_h_posiw.w.h;
    wV_posiw = scra_v_posiw.w.h;
    pMapWk = (Uint8 *)mapwka;
    VramBase = 16384;

    if (*pScrFlag) {
        if (*pScrFlag & 1) {
            *pScrFlag &= 254;

            lD4.l = -16;
            wD5 = 65520;
            vramadrset(wH_posiw, wV_posiw, wD5, lD4.w.l, &TilePoint);
            hblockwrt(&TilePoint, VramBase, wH_posiw, wV_posiw, wD5, lD4.w.l,
                      pMapWk, 21);
        }
        if (*pScrFlag & 2) {
            *pScrFlag &= 253;

            lD4.l = 224;
            wD5 = 65520;
            vramadrset(wH_posiw, wV_posiw, wD5, lD4.w.l, &TilePoint);
            hblockwrt(&TilePoint, VramBase, wH_posiw, wV_posiw, wD5, lD4.w.l,
                      pMapWk, 21);
        }
        if (*pScrFlag & 4) {
            *pScrFlag &= 251;

            lD4.l = -16;
            wD5 = 65520;
            vramadrset(wH_posiw, wV_posiw, wD5, lD4.w.l, &TilePoint);
            vblockwrt(&TilePoint, VramBase, wH_posiw, wV_posiw, wD5, lD4.w.l,
                      pMapWk, 15);
        }
        if (*pScrFlag & 8) {
            *pScrFlag &= 247;

            lD4.l = -16;
            wD5 = 320;
            vramadrset(wH_posiw, wV_posiw, wD5, lD4.w.l, &TilePoint);
            vblockwrt(&TilePoint, VramBase, wH_posiw, wV_posiw, wD5, lD4.w.l,
                      pMapWk, 15);
        }
    }
}

void scrollwrtb(Uint8 *pScrFlag, Uint8 *pMapWk, Sint32 VramBase) {
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
    } else {
        *pScrFlag &= 254;

        if (*pScrFlag & 2) {
            *pScrFlag &= 253;
            lD4.w.l = 224;
        } else {
            *pScrFlag &= 253;
            goto label1;
        }
    }

    wD0 = (Sint16)(scrb_v_posit.w.h + lD4.w.l) / 16;
    wD0 &= 127;
    wD0 = z12dwrttbl[wD0 + 1];
    wH_posiw = vblockwrtbtbl[wD0]->w.h;
    wV_posiw = vblockwrtbtbl[wD0 + 1]->w.h;
    if (wD0 != 0) {
        wD5 = 65520;
        vramadrset(wH_posiw, wV_posiw, wD5, lD4.w.l, &TilePoint);

        hblockwrt(&TilePoint, VramBase, wH_posiw, wV_posiw, wD5, lD4.w.l,
                  pMapWk, 21);

    } else {

        wD5 = 0;
        vramadrset1(wH_posiw, wV_posiw, wD5, lD4.w.l, &TilePoint);

        hblockwrt1(&TilePoint, VramBase, wH_posiw, wV_posiw, wD5, lD4.w.l,
                   pMapWk, 31);
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

        if (WrtTblCnt < 0)
            WrtTblCnt = 0;
        if (WrtTblCnt > 113)
            WrtTblCnt = 113;

        for (i = 0; i < 16; ++i) {
            wD0 = z12dwrttbl[WrtTblCnt++];
            if (*pScrFlag & (Uint8)(1 << wD0)) {
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

void hblockwrt(POINT *pTilePoint, Sint32 VramBase, Uint16 wH_posiw,
               Uint16 wV_posiw, Uint16 xOffs, Uint16 yOffs, Uint8 *pMapWk,
               Sint32 lpcnt) {
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

void hblockwrt1(POINT *pTilePoint, Sint32 VramBase, Uint16 wH_posiw,
                Uint16 wV_posiw, Uint16 xOffs, Uint16 yOffs, Uint8 *pMapWk,
                Sint32 lpcnt) {
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

void vblockwrt(POINT *pTilePoint, Sint32 VramBase, Uint16 wH_posiw,
               Uint16 wV_posiw, Uint16 xOffs, Uint16 yOffs, Uint8 *pMapWk,
               Sint32 lpcnt) {
    Sint32 BlkIndex;

    do {
        mapadrset(wH_posiw, wV_posiw, xOffs, yOffs, pMapWk, &BlkIndex);
        blockwrt(VramBase, pTilePoint, BlkIndex);
        pTilePoint->y += 2;
        if (pTilePoint->y >= 32)
            pTilePoint->y -= 32;
        yOffs += 16;
    } while (--lpcnt >= 0);
}

void blockwrt(Sint32 VramBase, POINT *pTilePoint, Sint32 BlkIndex) {
    Sint32 base;
    Sint32 frip;
    Sint32 p0, p1, p2, p3;
    Sint32 x, y;

    if (VramBase == 24576)
        base = 1;
    else if (VramBase == 16384)
        base = 0;
    x = pTilePoint->x;
    y = pTilePoint->y;

    frip = BlkIndex & 6144;
    BlkIndex &= 1023;
    if (frip == 6144) {
        p0 = 3;
        p1 = 2;
        p2 = 1;
        p3 = 0;
    } else if (frip & 4096) {
        p0 = 2;
        p1 = 3;
        p2 = 0;
        p3 = 1;
    } else if (frip & 2048) {
        p0 = 1;
        p1 = 0;
        p2 = 3;
        p3 = 2;
    } else {
        p0 = 0;
        p1 = 1;
        p2 = 2;
        p3 = 3;
    }

    SetGrid(base, x, y, blockwk[BlkIndex][p0], frip);
    SetGrid(base, x + 1, y, blockwk[BlkIndex][p1], frip);
    SetGrid(base, x, y + 1, blockwk[BlkIndex][p2], frip);
    SetGrid(base, x + 1, y + 1, blockwk[BlkIndex][p3], frip);
}

Sint32 mapadrset(Uint16 wH_posiw, Uint16 wV_posiw, Uint16 xOffs, Uint16 yOffs,
                 Uint8 *pMapWk, Sint32 *pIndex) {
    return mapadrset99(wH_posiw, wV_posiw, xOffs, yOffs, pMapWk, pIndex);
}

Sint32 mapadrset1(Uint16 wH_posiw, Uint16 wV_posiw, Uint16 xOffs, Uint16 yOffs,
                  Uint8 *pMapWk, Sint32 *pIndex) {
    wH_posiw = 0;
    return mapadrset99(wH_posiw, wV_posiw, xOffs, yOffs, pMapWk, pIndex);
}

Sint32 mapadrset99(Uint16 wH_posiw, Uint16 wV_posiw, Uint16 xOffs, Uint16 yOffs,
                   Uint8 *pMapWk, Sint32 *pIndex) {
    Sint32 xBlk, yBlk;
    Sint32 i;
    Uint8 ScreenNo;
    Uint16 *lpw;

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
    if (i < 0)
        i = 0;

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
        lpw += (ScreenNo - 1 << 4) << 4;
        *pIndex = *lpw;

        return 1;
    }
    *pIndex = 0;
    return 0;
}

Sint32 mapadrset2(Uint16 xOffs, Uint16 yOffs, Uint8 *pMapWk, Sint32 *pIndex,
                  Uint16 **ppBlockNo) {
    Sint32 xBlk, yBlk;
    Uint8 ScreenNo;
    Uint16 *lpw;

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
    Uint8 *pMapWk;
    Uint16 *pBlockIndex;
    Sint32 Index;

    VramBase = 16384;
    pMapWk = (Uint8 *)mapwka;
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
                if ((Sint16)(scra_v_posit.w.h + 336 & 65520) - 16 <=
                    (Sint16)xOffs)
                    return 0;
            }
        }
    }
    return 1;
}

void vramadrset(Uint16 wH_posiw, Uint16 wV_posiw, Uint16 xOffs, Uint16 yOffs,
                POINT *lpTilePoint) {
    vramadrset99(wH_posiw, wV_posiw, xOffs, yOffs, lpTilePoint);
}

void vramadrset1(Uint16 wH_posiw, Uint16 wV_posiw, Uint16 xOffs, Uint16 yOffs,
                 POINT *lpTilePoint) {
    wH_posiw = 0;
    vramadrset99(wH_posiw, wV_posiw, xOffs, yOffs, lpTilePoint);
}

void vramadrset0(Uint16 xOffs, Uint16 yOffs, POINT *lpTilePoint) {
    vramadrset99(0, 0, xOffs, yOffs, lpTilePoint);
}

void vramadrset2(Uint16 wH_posiw, Uint16 wV_posiw, Uint16 xOffs, Uint16 yOffs,
                 POINT *lpTilePoint) {
    vramadrset99(wH_posiw, wV_posiw, xOffs, yOffs, lpTilePoint);
}

void vramadrset99(Uint16 wH_posiw, Uint16 wV_posiw, Uint16 xOffs, Uint16 yOffs,
                  POINT *lpTilePoint) {
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
    Uint8 *pMapWk;

    wH_posiw = scra_h_posiw.w.h;
    wV_posiw = scra_v_posiw.w.h;
    pMapWk = (Uint8 *)mapwka;
    VramBase = 16384;
    mapwrt2(wH_posiw, wV_posiw, pMapWk, VramBase);

    pMapWk = (Uint8 *)mapwkb;
    VramBase = 24576;
    mapwrt_z11c(pMapWk, VramBase);
}

void mapwrt2(Uint16 wH_posiw, Uint16 wV_posiw, Uint8 *pMapWk, Sint32 VramBase) {
    mapwrt3(wH_posiw, wV_posiw, pMapWk, VramBase, 65520, 15);
}

void mapwrt3(Uint16 wH_posiw, Uint16 wV_posiw, Uint8 *pMapWk, Sint32 VramBase,
             Uint16 wD4, Uint16 wD6) {
    POINT TilePoint;

    do {
        vramadrset(wH_posiw, wV_posiw, 0, wD4, &TilePoint);
        hblockwrt(&TilePoint, VramBase, wH_posiw, wV_posiw, 0, wD4, pMapWk, 31);
        wD4 += 16;
    } while ((Sint16)--wD6 >= 0);
}

void mapwrt_z11c(Uint8 *pMapWk, Sint32 VramBase) {
    Uint16 wD0, wD4, wD6;

    wD4 = 65520;
    wD6 = 15;

    do {
        wD0 = scrb_v_posit.w.h + wD4 & 496;
        mapwrt_sub(&z12dwrttbl[1], wD0, wD4, pMapWk, VramBase);
        wD4 += 16;
    } while ((Sint16)--wD6 >= 0);
}

void mapwrt_sub(Uint8 *pWrttbl, Uint16 wD0, Uint16 wD4, Uint8 *pMapWk,
                Sint32 VramBase) {
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
        hblockwrt(&TilePoint, VramBase, wH_posiw, wV_posiw, wD5, wD4, pMapWk,
                  21);
    } else {
        wD5 = 0;
        vramadrset1(wH_posiw, wV_posiw, wD5, wD4, &TilePoint);
        hblockwrt1(&TilePoint, VramBase, wH_posiw, wV_posiw, wD5, wD4, pMapWk,
                   31);
    }
}

void mapinit(void) {

    colorset2(mapinittbl.colorno2);
    colorset(mapinittbl.colorno2);

    if (plflag)
        enecginit();
    if (play_start & 2)
        divdevset();
}
