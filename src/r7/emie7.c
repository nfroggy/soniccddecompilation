#include <stddef.h>

#include "../equ.h"
#include "emie7.h"
#include "../actset.h"
#include "../dircol.h"
#include "../etc.h"
#include "../goal.h"
#include "../loader2.h"

void actionsub(sprite_status *pActwk);
void frameout(sprite_status *pActwk);

#pragma pack(push, 1)
typedef struct {
    Uint8 goal_started;
    Uint8 unused1[5];
    Sint16 timer;
    Uint8 unused8[4];
    Sint16 home_x;
    Uint8 goal_flag;
    Uint8 unused15;
    Uint8 motion_counter;
    Uint8 heart_counter;
    Uint8 stop_flag;
    Uint8 unused19;
    Uint8 daki_flags;
    Uint8 daki_delay;
} emie7_work;
#pragma pack(pop)

_Static_assert(offsetof(emie7_work, goal_started) == 0,
               "emie7_work.goal_started offset");
_Static_assert(offsetof(emie7_work, timer) == 6,
               "emie7_work.timer offset");
_Static_assert(offsetof(emie7_work, home_x) == 12,
               "emie7_work.home_x offset");
_Static_assert(offsetof(emie7_work, goal_flag) == 14,
               "emie7_work.goal_flag offset");
_Static_assert(offsetof(emie7_work, motion_counter) == 16,
               "emie7_work.motion_counter offset");
_Static_assert(offsetof(emie7_work, heart_counter) == 17,
               "emie7_work.heart_counter offset");
_Static_assert(offsetof(emie7_work, stop_flag) == 18,
               "emie7_work.stop_flag offset");
_Static_assert(offsetof(emie7_work, daki_flags) == 20,
               "emie7_work.daki_flags offset");
_Static_assert(offsetof(emie7_work, daki_delay) == 21,
               "emie7_work.daki_delay offset");
_Static_assert(sizeof(emie7_work) <= sizeof(((sprite_status *)0)->actfree),
               "emie7_work fits in actfree");

static emie7_work *emie7_get_work(sprite_status *pActwk) {
    return (emie7_work *)pActwk->actfree;
}

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
    emie7_get_work(pActwk)->home_x = pActwk->xposi.w.h;
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
        emie7_get_work(pActwk)->timer = 600;
    } else {
        empatchg(pActwk, em_pchg);
    }
}

static void emie7_move0(sprite_status *pActwk) {
    emie7_work *work = emie7_get_work(pActwk);

    if ((Sint8)work->stop_flag >= 0) {

        pActwk->yspeed.w += 16;
        speedsety(pActwk);

        if (pActwk->yposi.w.h >= 464) {
            pActwk->yposi.w.h = 464;
            pActwk->yspeed.w = 0;
            work->stop_flag = (Uint8)-1;
        }
    } else {
        setdirect(pActwk, &actwk[0]);
        work->daki_delay = 60;
        pActwk->r_no0 += 2;
        empatchg(pActwk, em_pchg);
    }
}

static void emie7_move1(sprite_status *pActwk) {
    emie7_work *work = emie7_get_work(pActwk);
    Sint16 lenwk, maxspd, spdwk, scdwk;

    heartset(pActwk);
    setdirect(pActwk, &actwk[0]);

    if (work->timer) {
        if (!(--work->timer)) {

            work->goal_flag = (Uint8)-1;
            pltime_f = 0;
            work->goal_started = 1;
            goal_move2(pActwk);
            return;
        }
    }

    if (!work->daki_delay || !(--work->daki_delay)) {

        dakicheck(pActwk);

        if ((lenwk = actwk[0].xposi.w.h - pActwk->xposi.w.h) < 0) {
            lenwk *= -1;
        }

        if (((work->daki_flags & 4) && lenwk < 32) ||
            (!(actwk[0].xspeed.w) && lenwk < 10)) {
            work->daki_flags |= 4;
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

        lenwk = work->home_x;

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
    emie7_work *work = emie7_get_work(pActwk);
    Sint16 lenwk;

    pActwk->patno = 6;

    if ((lenwk = pActwk->xposi.w.h - work->home_x) < 0) {

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
    emie7_work *work = emie7_get_work(pActwk);

    speedset(pActwk);
    if ((pActwk->yspeed.w += 64) >= 0) {
        pActwk->patno = 7;
    }

    if (pActwk->yposi.w.h >= 464) {

        pActwk->yposi.w.h = 464;
        pActwk->xspeed.w = 0;
        pActwk->yspeed.w = 0;

        if ((Sint16)work->motion_counter + 16 < 256) {
            work->motion_counter += 16;
            return;
        }

        work->motion_counter += 16;
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
    if (!emie7_get_work(pActwk)->goal_flag) {
        pltime_f = 0;
        emie7_get_work(pActwk)->goal_started = 1;
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
        emie7_get_work(pActwk)->daki_flags |= 129;
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
    emie7_work *work = emie7_get_work(pActwk);
    sprite_status *pHeartact;
    Sint16 wk;

    if (work->heart_counter + 6 > 255) {
        if (actwkchk(&pHeartact) == 0) {
            pHeartact->actno = 52;
            wk = 8;

            if (pActwk->cddat & 1) {
                wk = -10;
            }

            if (work->daki_flags & 1) {
                wk *= -1;
            }

            pHeartact->xposi.w.h = pActwk->xposi.w.h + wk;
            pHeartact->yposi.w.h = pActwk->yposi.w.h - 12;
        }
    }

    work->heart_counter += 6;
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
    emie7_work *work = emie7_get_work(pActwk);
    Sint16 sinwk, coswk;

    if (!work->stop_flag) {
        sinset(work->motion_counter * 3, &sinwk, &coswk);
        pActwk->xspeed.w = sinwk >> 2;
    }

    speedset(pActwk);
    if (++work->motion_counter == 20) {
        ++pActwk->patno;
    } else if (work->motion_counter == 110) {
        ++pActwk->patno;
        pActwk->xspeed.w = pActwk->yspeed.w = 0;
        work->stop_flag = 1;
    } else if (work->motion_counter == 120) {
        frameout(pActwk);
    }
}
