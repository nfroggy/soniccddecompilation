#include "../equ.h"
#include "emie7.h"
#include "../actset.h"
#include "../dircol.h"
#include "../etc.h"
#include "../goal.h"
#include "../loader2.h"

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);

static void emie7_init(sprite_status *pActwk);
static void emie7_move0(sprite_status *pActwk);
static void emie7_move1(sprite_status *pActwk);
static void emie7_tobii(sprite_status *pActwk);
static void emie7_tobim(sprite_status *pActwk);
static void emie7_daki2(sprite_status *pActwk);
static void emie7_end(sprite_status *pActwk);
static void setdirect(sprite_status *pEmiewk, sprite_status *pSonicwk);
static void speedset(sprite_status *pActwk);
static void speedsetx(sprite_status *pActwk);
static void speedsety(sprite_status *pActwk);
static void dakicheck(sprite_status *pActwk);
static void empatchg(sprite_status *pActwk, char **pPattbl);
static void emie7clrsetx(PALETTEENTRY *pPalet);
static void heartset(sprite_status *pActwk);
static void heart1_init(sprite_status *pActwk);
static void heart1_move(sprite_status *pActwk);

static void (*em7_tbl[9])(sprite_status *) = {
    &emie7_init,  &emie7_move0, &emie7_move1, &emie7_move1, &emie7_tobim,
    &emie7_daki2, &emie7_end,   &emie7_tobii, &emie7_tobim};
extern PALETTEENTRY gamecolor[];
extern char *em_pchg[];
extern sprite_pattern *emie7pat[];
static void (*ht1_tbl[2])(sprite_status *) = {&heart1_init, &heart1_move};

void emie7(sprite_status *pActwk) {
    em7_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s(pActwk);

    if (pActwk->actno != 51) {
        emie7clrsetx(&gamecolor[16]);
    }
}

static void emie7_init(sprite_status *pActwk) {
    sprite_status *pNewact;

    pActwk->actflg |= 4;
    pActwk->sproffset = 41931;
    pActwk->sprpri = 1;
    pActwk->patbase = emie7pat;

    pActwk->sprhsize = 12;
    pActwk->sprvsize = 16;
    ((Sint16 *)pActwk)[29] = pActwk->xposi.w.h;
    pActwk->colino = 245;

    if (!generate_flag) {
        pActwk->xposi.w.h = 16195;
        pActwk->yposi.w.h = 427;
    }

    pActwk->mstno.b.h = 6;
    if (pActwk->colicnt) {
        if (actwkchk(&pNewact) == 0) {
            pNewact->actno = 24;
            pNewact->r_no1 = 1;
            pNewact->xposi.w.h = pActwk->xposi.w.h;
            pNewact->yposi.w.h = pActwk->yposi.w.h;
            soundset(158);
        }

        actwk[0].yspeed.w *= -1;
        pActwk->r_no0 += 2;
        ((Sint16 *)pActwk)[26] = 600;
    } else {
        empatchg(pActwk, em_pchg);
    }
}

static void emie7_move0(sprite_status *pActwk) {
    if (((char *)pActwk)[64] >= 0) {

        pActwk->yspeed.w += 16;
        speedsety(pActwk);

        if (pActwk->yposi.w.h >= 464) {
            pActwk->yposi.w.h = 464;
            pActwk->yspeed.w = 0;
            ((char *)pActwk)[64] = -1;
        }
    } else {
        setdirect(pActwk, &actwk[0]);
        pActwk->actfree[21] = 60;
        pActwk->r_no0 += 2;
        empatchg(pActwk, em_pchg);
    }
}

static void emie7_move1(sprite_status *pActwk) {
    Sint16 lenwk, maxspd, spdwk, scdwk;

    heartset(pActwk);
    setdirect(pActwk, &actwk[0]);

    if (((Sint16 *)pActwk)[26]) {
        if (!(--((Sint16 *)pActwk)[26])) {

            ((char *)pActwk)[60] = -1;
            pltime_f = 0;
            pActwk->actfree[0] = 1;
            goal_move2(pActwk);
            return;
        }
    }

    if (!pActwk->actfree[21] || !(--pActwk->actfree[21])) {

        dakicheck(pActwk);

        if ((lenwk = actwk[0].xposi.w.h - pActwk->xposi.w.h) < 0) {
            lenwk *= -1;
        }

        if (((pActwk->actfree[20] & 4) && lenwk < 32) ||
            (!(actwk[0].xspeed.w) && lenwk < 10)) {
            pActwk->actfree[20] |= 4;
            pActwk->xspeed.w = 0;

            pActwk->mstno.b.h = 1;
            empatchg(pActwk, em_pchg);
            return;
        }
    }

    spdwk = -32;
    if (!(pActwk->cddat & 1)) {
        spdwk *= -1;
    }

    spdwk += pActwk->xspeed.w;
    lenwk = spdwk;
    maxspd = 640;

    if (lenwk < 0) {
        lenwk *= -1;
        maxspd *= -1;
    }

    if (lenwk >= 640) {
        spdwk = maxspd;
    }

    pActwk->xspeed.w = spdwk;

    if (pActwk->xspeed.w < 0) {

        lenwk = ((Sint16 *)pActwk)[29];

        if (generate_flag) {
            lenwk -= 96;
        } else {
            lenwk -= 128;
        }

        if (lenwk >= pActwk->xposi.w.h) {

            pActwk->xspeed.w = 0;

            pActwk->mstno.b.h = 1;
            empatchg(pActwk, em_pchg);
            return;
        }
    }

    scdwk = emycol_d(pActwk);
    if (scdwk < 7 && scdwk >= -7) {
        pActwk->yposi.w.h += scdwk;
    }

    speedsetx(pActwk);

    pActwk->mstno.b.h = 2;
    empatchg(pActwk, em_pchg);
}

static void emie7_tobii(sprite_status *pActwk) {
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

    emie7_tobim(pActwk);
}

static void emie7_tobim(sprite_status *pActwk) {
    speedset(pActwk);
    if ((pActwk->yspeed.w += 64) >= 0) {
        pActwk->patno = 7;
    }

    if (pActwk->yposi.w.h >= 464) {

        pActwk->yposi.w.h = 464;
        pActwk->xspeed.w = 0;
        pActwk->yspeed.w = 0;

        if ((Sint16)pActwk->actfree[16] + 16 < 256) {
            pActwk->actfree[16] += 16;
            return;
        }

        pActwk->actfree[16] += 16;
        pActwk->r_no0 = 4;
    }
}

static void emie7_daki2(sprite_status *pActwk) {
    heartset(pActwk);

    plautoflag |= 1;
    swdata.w = 0;
    actwk[0].mstno.b.h = 5;
    setdirect(pActwk, &actwk[0]);

    if (actwk[0].cddat & 1) {
        pActwk->xposi.w.h = actwk[0].xposi.w.h + 12;
    } else {
        pActwk->xposi.w.h = actwk[0].xposi.w.h - 12;
    }

    pActwk->yposi.w.h = actwk[0].yposi.w.h;
    pActwk->patno = 14;
    if (!pActwk->actfree[14]) {
        pltime_f = 0;
        pActwk->actfree[0] = 1;
        goal_move2(pActwk);
    }
}

static void emie7_end(sprite_status *pActwk) {
    heartset(pActwk);
    setdirect(pActwk, &actwk[0]);

    if (actwk[0].cddat & 1) {
        pActwk->xposi.w.h = actwk[0].xposi.w.h + 12;
    } else {
        pActwk->xposi.w.h = actwk[0].xposi.w.h - 12;
    }

    pActwk->patno = 14;
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

static void speedset(sprite_status *pActwk) {
    speedsetx(pActwk);
    speedsety(pActwk);
}

static void speedsetx(sprite_status *pActwk) {
    pActwk->xposi.l += pActwk->xspeed.w << 8;
}

static void speedsety(sprite_status *pActwk) {
    pActwk->yposi.l += pActwk->yspeed.w << 8;
}

static void dakicheck(sprite_status *pActwk) {
    Sint16 spdwk, poswk1, poswk2;

    if (editmode.b.h)
        return;

    if (actwk[0].cddat & 1) {

        poswk1 = actwk[0].xposi.w.h;
        poswk2 = pActwk->xposi.w.h;
    } else {
        poswk1 = pActwk->xposi.w.h;
        poswk2 = actwk[0].xposi.w.h;
    }

    if (poswk1 < poswk2)
        return;

    if ((poswk1 -= poswk2) < 8 || poswk1 >= 28) {
        return;
    }

    poswk1 = actwk[0].yposi.w.h - pActwk->yposi.w.h;
    if ((poswk1 += 8) < 0 || poswk1 >= 16) {
        return;
    }

    if ((spdwk = actwk[0].xspeed.w) < 0) {
        spdwk *= -1;
    }

    if ((actwk[0].cddat & 2) || (actwk[0].cddat & 4)) {

        pActwk->r_no0 = 14;
    } else {
        pActwk->actfree[20] |= 129;
        pActwk->xspeed.w = pActwk->yspeed.w = 0;
        pActwk->patno = 7;
        pActwk->r_no0 = 10;

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
        pActwk->actflg |= (pActwk->cddat ^ (patnowk << 3 | patnowk >> 5)) & 3;

        pActwk->pattim = pPatdat[pActwk->patcnt + 1];

        pActwk->patcnt += 2;
    }
}

static void emie7clrsetx(PALETTEENTRY *pPalet) {
    PALETTEENTRY *pColorwk;
    Sint16 i;

    pColorwk = &lpcolorwk[16];

    for (i = 0; i < 16; ++i) {
        *pColorwk = *pPalet;
        ++pColorwk;
        ++pPalet;
    }
}

static void heartset(sprite_status *pActwk) {
    sprite_status *pHeartact;
    Sint16 wk;

    if (pActwk->actfree[17] + 6 > 255) {
        if (actwkchk(&pHeartact) == 0) {
            pHeartact->actno = 52;
            wk = 8;

            if (pActwk->cddat & 1) {
                wk = -10;
            }

            if (pActwk->actfree[20] & 1) {
                wk *= -1;
            }

            pHeartact->xposi.w.h = pActwk->xposi.w.h + wk;
            pHeartact->yposi.w.h = pActwk->yposi.w.h - 12;
        }
    }

    pActwk->actfree[17] += 6;
}

void heart7(sprite_status *pActwk) {
    ht1_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s(pActwk);
}

static void heart1_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 971;
    pActwk->patbase = emie7pat;
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

    speedset(pActwk);
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
