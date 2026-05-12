#include "equ.h"
#include "emie1.h"
#include "action.h"
#include "actset.h"
#include "dircol.h"
#include "etc.h"
#include "loader2.h"

static void emie1_init(sprite_status *pActwk);
static void emie1_matu(sprite_status *pActwk);
static void emie1_dakii(sprite_status *pActwk);
static void emie1_tobii(sprite_status *pActwk);
static void emie1_tobim(sprite_status *pActwk);
static void emie_play(sprite_status *pEmiewk, sprite_status *pSonicwk);
static void setdirect(sprite_status *pEmiewk, sprite_status *pSonicwk);
static void emie1_speedset(sprite_status *pActwk);
static void emie1_speedsetx(sprite_status *pActwk);
static void emie1_speedsety(sprite_status *pActwk);
static void jumpchk_d(void);
static void pljumpset(void);
static void dakicheck(sprite_status *pActwk);
static void empatchg(sprite_status *pActwk, char **pPattbl);
static void emie1clrset(void);
static void emie1clrsetx(PALETTEENTRY *pPalet);
static void heartset(sprite_status *pActwk);
static void heart1_init(sprite_status *pActwk);
static void heart1_move(sprite_status *pActwk);

static void (*em1_tbl[5])(sprite_status *) = {
    &emie1_init, &emie1_matu, &emie1_dakii, &emie1_tobii, &emie1_tobim};
extern PALETTEENTRY zone1colora[];
extern char *em_pchg[];
extern sprite_pattern *emie1pat[];
static PALETTEENTRY emie1_clr[16] = {
    {0, 0, 0, 1},       {0, 0, 0, 1},       {128, 32, 96, 1},
    {160, 64, 128, 1},  {224, 96, 224, 1},  {224, 160, 224, 1},
    {224, 224, 224, 1}, {160, 160, 160, 1}, {128, 128, 128, 1},
    {64, 64, 64, 1},    {224, 160, 128, 1}, {192, 96, 0, 1},
    {32, 192, 0, 1},    {0, 128, 0, 1},     {96, 0, 128, 1},
    {224, 0, 0, 1}};
static void (*ht1_tbl[2])(sprite_status *) = {&heart1_init, &heart1_move};

void emie1(sprite_status *pActwk) {
    if (ta_flag) {
        emie1clrsetx(zone1colora);
    } else {
        em1_tbl[pActwk->r_no0 / 2](pActwk);
        heartset(pActwk);

        actionsub(pActwk);
        frameout_s(pActwk);

        if (pActwk->actno != 47) {
            emie1clrsetx(zone1colora);
        }
    }
}

static void emie1_init(sprite_status *pActwk) {
    Sint16 iScd, xwk;

    pActwk->actflg |= 4;
    pActwk->sproffset = 9072;
    pActwk->sprpri = 1;
    pActwk->patbase = emie1pat;

    pActwk->sprhsize = 12;
    pActwk->sprvsize = 16;
    ((Sint16 *)pActwk)[29] = pActwk->xposi.w.h;

    emie1clrset();

    while ((iScd = emycol_d(pActwk)) != 0) {
        pActwk->yposi.w.h += iScd;
    }

    setdirect(pActwk, &actwk[0]);

    xwk = actwk[0].xposi.w.h;
    if ((xwk -= pActwk->xposi.w.h) < 0) {
        xwk = -xwk;
    }

    if (xwk < 112) {
        pActwk->r_no0 += 2;
    }

    pActwk->mstno.b.h = 5;
    empatchg(pActwk, em_pchg);
}

static void emie1_matu(sprite_status *pActwk) {
    Sint16 lenwk;
    Uint8 dakiflgwk;

    setdirect(pActwk, &actwk[0]);

    if ((lenwk = actwk[0].xposi.w.h - pActwk->xposi.w.h) < 0) {
        lenwk = -lenwk;
    }

    dakiflgwk = pActwk->actfree[20];

    if ((dakiflgwk & 4) || actwk[0].xspeed.w || lenwk >= 16) {

        if (dakiflgwk & 4) {

            if (lenwk < 32) {

                pActwk->xspeed.w = 0;
                pActwk->yposi.w.h += emycol_d(pActwk);
                pActwk->mstno.b.h = 1;
                empatchg(pActwk, em_pchg);
                return;
            }

            pActwk->actfree[20] &= 251;
        }

        if (pActwk->cddat & 1) {
            pActwk->xspeed.w -= 16;
        } else
            pActwk->xspeed.w += 16;

        if (pActwk->xspeed.w > 512) {
            pActwk->xspeed.w = 512;
        } else if (pActwk->xspeed.w < -512) {
            pActwk->xspeed.w = -512;
        }

        if (pActwk->xspeed.w >= 0) {

            if (((Sint16 *)pActwk)[29] + 144 > pActwk->xposi.w.h) {

                lenwk = emycol_d(pActwk);

                if (lenwk < 7 && lenwk >= -7) {
                    pActwk->yposi.w.h += lenwk;
                    emie1_speedsetx(pActwk);
                    dakicheck(pActwk);

                    pActwk->mstno.b.h = 2;
                    empatchg(pActwk, em_pchg);
                    return;
                }
            }
        } else {
            if ((((Sint16 *)pActwk)[29] - 304) < pActwk->xposi.w.h) {

                lenwk = emycol_d(pActwk);

                if (lenwk < 7 && lenwk >= -7) {
                    pActwk->yposi.w.h += lenwk;
                    emie1_speedsetx(pActwk);
                    dakicheck(pActwk);

                    pActwk->mstno.b.h = 2;
                    empatchg(pActwk, em_pchg);
                    return;
                }
            }
        }
    } else if (!(dakiflgwk & 64)) {

        pActwk->actfree[20] |= 4;
        pActwk->xspeed.w = 0;

        pActwk->yposi.w.h += emycol_d(pActwk);
        pActwk->mstno.b.h = 1;
        empatchg(pActwk, em_pchg);
        return;
    }

    pActwk->xspeed.w = 0;

    if (dakiflgwk & 128) {

        if (!(dakiflgwk & 64)) {
            if (pActwk->actfree[21] >= 3) {

                if (((Sint16)pActwk->actfree[16] + 4) > 255) {
                    pActwk->actfree[21] = 0;
                }

                pActwk->actfree[16] += 4;
                pActwk->mstno.b.h = 4;
                empatchg(pActwk, em_pchg);
                return;
            }

            pActwk->yspeed.w = -768;
            pActwk->actfree[20] |= 64;
        }

        emie1_speedsety(pActwk);
        pActwk->yspeed.w += 64;

        if (pActwk->yspeed.w < 0) {
            pActwk->patno = 6;
        } else
            pActwk->patno = 4;

        if (emycol_d(pActwk) < 0) {

            pActwk->yspeed.w = 0;
            pActwk->actfree[20] &= 191;
            ++pActwk->actfree[21];
        }
    } else {
        pActwk->yposi.w.h += emycol_d(pActwk);
        pActwk->mstno.b.h = 1;
        empatchg(pActwk, em_pchg);
    }
}

static void emie1_dakii(sprite_status *pActwk) {

    actwk[0].actfree[2] |= 1;

    actwk[0].mstno.b.h = 5;
    emie_play(pActwk, &actwk[0]);

    setdirect(pActwk, &actwk[0]);

    if (actwk[0].cddat & 1) {
        pActwk->xposi.w.h = actwk[0].xposi.w.h + 12;
    } else {
        pActwk->xposi.w.h = actwk[0].xposi.w.h - 12;
    }

    pActwk->yposi.w.h = actwk[0].yposi.w.h;

    swdata = swdata1;
    jumpchk_d();
    if (!(actwk[0].actfree[2] & 1)) {

        pActwk->actfree[20] &= 254;
        pActwk->r_no0 = 6;
    } else if (pltime.l >= 602624) {

        pljumpset();
        pActwk->actfree[20] &= 254;
        pActwk->r_no0 = 2;
    } else {
        pActwk->mstno.b.h = 3;
        empatchg(pActwk, em_pchg);
    }
}

static void emie1_tobii(sprite_status *pActwk) {
    Sint16 lenwk;

    pActwk->patno = 6;

    if ((lenwk = pActwk->xposi.w.h - ((Sint16 *)pActwk)[29]) < 0) {

        lenwk = -lenwk;
    }

    if (lenwk >= 128) {
        pActwk->xspeed.w = 0;
    } else {
        if (pActwk->cddat & 1) {

            pActwk->xspeed.w = 128;
        } else {
            pActwk->xspeed.w = -128;
        }
    }

    pActwk->yspeed.w = -768;
    pActwk->r_no0 += 2;

    emie1_tobim(pActwk);
}

static void emie1_tobim(sprite_status *pActwk) {
    emie1_speedset(pActwk);
    if ((pActwk->yspeed.w += 64) >= 0) {
        pActwk->patno = 7;
    }

    if (emycol_d(pActwk) >= 0)
        return;

    pActwk->xspeed.w = 0;
    pActwk->yspeed.w = 0;

    if ((Sint16)pActwk->actfree[16] + 16 < 256) {
        pActwk->actfree[16] += 16;
    } else {

        pActwk->actfree[16] += 16;
        pActwk->r_no0 = 2;
    }
}

static void emie_play(sprite_status *pEmiewk, sprite_status *pSonicwk) {
    if (pEmiewk->xspeed.w) {
        emie1_speedsetx(pSonicwk);
        pSonicwk->yposi.w.h += emycol_d(pSonicwk);

        if (pSonicwk->xspeed.w < 0) {

            if ((pSonicwk->xspeed.w + 64) < 0) {
                pSonicwk->xspeed.w += 64;
            } else {
                pSonicwk->xspeed.w = 0;
            }
        } else {
            if ((pSonicwk->xspeed.w - 64) >= 0) {
                pSonicwk->xspeed.w -= 64;
            } else {
                pSonicwk->xspeed.w = 0;
            }
        }
    }
}

static void setdirect(sprite_status *pEmiewk, sprite_status *pSonicwk) {
    if (pEmiewk->xposi.w.h < pSonicwk->xposi.w.h) {

        pEmiewk->cddat &= 254;
        pEmiewk->actflg &= 254;
    } else {
        pEmiewk->cddat |= 1;
        pEmiewk->actflg |= 1;
    }
}

static void emie1_speedset(sprite_status *pActwk) {
    emie1_speedsetx(pActwk);
    emie1_speedsety(pActwk);
}

static void emie1_speedsetx(sprite_status *pActwk) {
    pActwk->xposi.l += pActwk->xspeed.w << 8;
}

static void emie1_speedsety(sprite_status *pActwk) {
    pActwk->yposi.l += pActwk->yspeed.w << 8;
}

static void jumpchk_d(void) {

    if (!(swdata.b.h & 112))
        return;

    pljumpset();
}

static void pljumpset(void) {
    Sint16 sinwk, coswk;

    actwk[0].actfree[2] = 0;
    sinset(actwk[0].direc.b.h - 64, &sinwk, &coswk);
    coswk = (coswk * 1664) >> 8;
    actwk[0].xspeed.w += coswk;
    sinwk = (sinwk * 1664) >> 8;
    actwk[0].yspeed.w += sinwk;

    actwk[0].cddat |= 2;
    actwk[0].cddat &= 223;
    actwk[0].actfree[18] = 1;
    actwk[0].actfree[14] = 0;

    if (actwk[0].cddat & 4) {

        actwk[0].sprvsize = 19;
        actwk[0].sprhs = 9;
        actwk[0].cddat |= 16;
    } else {
        actwk[0].sprvsize = 14;
        actwk[0].sprhs = 7;
        actwk[0].yposi.w.h += 5;
        actwk[0].cddat |= 4;
        actwk[0].mstno.b.h = 2;
    }
}

static void dakicheck(sprite_status *pActwk) {
    Sint16 lenwk;

    if (pActwk->xspeed.w >= 0) {

        if (((Sint16 *)pActwk)[29] + 144 <= pActwk->xposi.w.h)
            return;

    } else {
        if (((Sint16 *)pActwk)[29] - 304 >= pActwk->xposi.w.h)
            return;
    }

    if (pltime.l >= 602624)
        return;

    if (editmode.b.h)
        return;

    if (actwk[0].cddat & 1) {

        if ((lenwk = pActwk->xposi.w.h - actwk[0].xposi.w.h) < 0)
            return;

    } else if ((lenwk = actwk[0].xposi.w.h - pActwk->xposi.w.h) < 0)
        return;

    if (lenwk < 12 || lenwk >= 24) {
        return;
    }

    lenwk = actwk[0].yposi.w.h - pActwk->yposi.w.h + 8;
    if (lenwk < 0 || lenwk >= 16) {

        return;
    }

    if ((lenwk = actwk[0].xspeed.w) < 0) {
        lenwk = -lenwk;
    }

    if (actwk[0].cddat & 6 || actwk[0].actfree[6] || lenwk >= 1664 ||
        plpower_b || plpower_a || plpower_m) {
        pActwk->r_no0 = 6;
    } else {

        pActwk->actfree[20] |= 129;
        pActwk->xspeed.w = pActwk->yspeed.w = 0;
        pActwk->patno = 7;
        pActwk->r_no0 = 4;

        sub_sync(124);
    }
}

static void empatchg(sprite_status *pActwk, char **pPattbl) {
    char *pPatdat, patnowk;

    if (pActwk->mstno.b.h != pActwk->mstno.b.l) {

        pActwk->mstno.b.l = pActwk->mstno.b.h;
        pActwk->patcnt = 0;
        pActwk->pattim = 0;
    }

    if ((char)--pActwk->pattim <= 0) {
        pPatdat = pPattbl[pActwk->mstno.b.l];
        if ((patnowk = pPatdat[pActwk->patcnt]) < 0) {

            pActwk->patcnt = 0;
            patnowk = pPatdat[pActwk->patcnt];
        }

        pActwk->patno = patnowk & 31;

        pActwk->actflg &= 252;
        pActwk->actflg |=
            (pActwk->cddat ^ ((patnowk << 3) | (patnowk >> 5))) & 3;

        pActwk->pattim = pPatdat[pActwk->patcnt + 1];

        pActwk->patcnt += 2;
    }
}

static void emie1clrset(void) { emie1clrsetx(emie1_clr); }

static void emie1clrsetx(PALETTEENTRY *pPalet) {
    PALETTEENTRY *pColorwk;
    Sint16 i;

    pColorwk = lpcolorwk + 16;

    for (i = 0; i < 16; ++i) {
        *pColorwk = *pPalet;
        ++pColorwk;
        ++pPalet;
    }
}

static void heartset(sprite_status *pActwk) {
    sprite_status *pHeartact;
    Sint16 wk;

    if (pActwk->actfree[20] & 1) {

        wk = (Sint16)pActwk->actfree[17] + 16;
        pActwk->actfree[17] += 16;
        if (wk <= 255)
            return;

    } else {
        wk = (Sint16)pActwk->actfree[17] + 6;
        pActwk->actfree[17] += 6;
        if (wk <= 255)
            return;
    }

    if (actwkchk(&pHeartact) != 0)
        return;

    pHeartact->actno = 48;
    if (pActwk->cddat & 1) {
        wk = -10;
    } else {
        wk = 8;
    }

    if (pActwk->actfree[20] & 1) {
        wk = -wk;
    }

    pHeartact->xposi.w.h = pActwk->xposi.w.h + wk;
    pHeartact->yposi.w.h = pActwk->yposi.w.h - 12;
}

void heart1(sprite_status *pActwk) {
    ht1_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s(pActwk);
}

static void heart1_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 880;
    pActwk->patbase = emie1pat;
    pActwk->patno = 8;
    pActwk->yspeed.w = -96;
    pActwk->sprpri = 3;

    heart1_move(pActwk);
}

static void heart1_move(sprite_status *pActwk) {
    Sint16 sinwk, coswk;

    if (!pActwk->actfree[18]) {
        sinset(pActwk->actfree[16] * 3, &sinwk, &coswk);
        pActwk->xspeed.w = sinwk >> 2;
    }

    emie1_speedset(pActwk);
    if (++pActwk->actfree[16] == 20) {
        ++pActwk->patno;
    } else if (pActwk->actfree[16] == 110) {
        ++pActwk->patno;
        pActwk->xspeed.w = pActwk->yspeed.w = 0;
        pActwk->actfree[18] = 1;
    } else if (pActwk->actfree[16] == 120) {
        frameout(pActwk);
    }
}
