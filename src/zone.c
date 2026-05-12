#include "equ.h"
#include "zone.h"
#include "action.h"
#include "actset.h"
#include "dummy.h"
#include "impfuncs.h"
#include "loader2.h"
#include "score.h"

static void over_init(sprite_status *pAct);
static void over_move(sprite_status *pAct);
static void title_init(sprite_status *pAct);
static void title_move0(sprite_status *pAct);
static void title_move1(sprite_status *pAct);
static void title_back0(sprite_status *pAct);
static void title_back1(sprite_status *pAct);
static void title_wait(sprite_status *pAct);
static void clear_init0(sprite_status *pAct);
static void clear_init(sprite_status *pAct);
static void clear_move0(sprite_status *pAct);
static void clear_move1(sprite_status *pAct);
static void clear_move2(sprite_status *pAct);
static void clear_wait(sprite_status *pAct);

#if defined(R31) || defined(R32)
#define SPRITE_ZONE_BASE 390
#elif defined(R33)
#define SPRITE_ZONE_BASE 394
#elif defined(R6)
#define SPRITE_ZONE_BASE 389
#elif defined(R81)
#define SPRITE_ZONE_BASE 379
#elif defined(R82)
#define SPRITE_ZONE_BASE 388
#elif defined(R83)
#define SPRITE_ZONE_BASE 381
#else
#define SPRITE_ZONE_BASE 348
#endif

static sprite_pattern game0 = {1, {{-72, -8, 0, SPRITE_ZONE_BASE}}};
static sprite_pattern game1 = {1, {{8, -8, 0, SPRITE_ZONE_BASE + 1}}};
sprite_pattern *gamepat[2] = {&game0, &game1};
static sprite_pattern time0 = {1, {{-68, -8, 0, SPRITE_ZONE_BASE + 2}}};
static sprite_pattern time1 = {1, {{4, -8, 0, SPRITE_ZONE_BASE + 3}}};
sprite_pattern *timepat[2] = {&time0, &time1};
static sprite_pattern gotpat0 = {1, {{-68, -20, 0, SPRITE_ZONE_BASE + 4}}};
static sprite_pattern gotpat1 = {1, {{-100, 4, 0, SPRITE_ZONE_BASE + 5}}};
static sprite_pattern gotpat2 = {1, {{-100, 4, 0, SPRITE_ZONE_BASE + 6}}};
static sprite_pattern gotpat3 = {1, {{-100, 4, 0, SPRITE_ZONE_BASE + 7}}};
static sprite_pattern madepat0 = {1, {{-128, -20, 0, SPRITE_ZONE_BASE + 8}}};
static sprite_pattern madepat1 = {1, {{-112, 4, 0, SPRITE_ZONE_BASE + 9}}};
static sprite_pattern madepat2 = {1, {{-112, 4, 0, SPRITE_ZONE_BASE + 10}}};
static sprite_pattern madepat3 = {1, {{-112, 4, 0, SPRITE_ZONE_BASE + 11}}};
sprite_pattern bonuspat = {18,
                           {{-44, -32, 0, SPRITE_ZONE_BASE + 12},
                            {72, -32, 0, 0},
                            {80, -32, 0, 0},
                            {88, -32, 0, 0},
                            {96, -32, 0, 0},
                            {104, -32, 0, 0},
                            {112, -32, 0, 0},
                            {120, -32, 0, 0},
                            {88, -8, 0, 0},
                            {96, -8, 0, 0},
                            {104, -8, 0, 0},
                            {112, -8, 0, 0},
                            {120, -8, 0, 0},
                            {88, 16, 0, 0},
                            {96, 16, 0, 0},
                            {104, 16, 0, 0},
                            {112, 16, 0, 0},
                            {120, 16, 0, 0}}};
sprite_pattern bonuspat0 = {18,
                            {{-44, -32, 0, SPRITE_ZONE_BASE + 13},
                             {72, -32, 0, 0},
                             {80, -32, 0, 0},
                             {88, -32, 0, 0},
                             {96, -32, 0, 0},
                             {104, -32, 0, 0},
                             {112, -32, 0, 0},
                             {120, -32, 0, 0},
                             {88, -8, 0, 0},
                             {96, -8, 0, 0},
                             {104, -8, 0, 0},
                             {112, -8, 0, 0},
                             {120, -8, 0, 0},
                             {88, 16, 0, 0},
                             {96, 16, 0, 0},
                             {104, 16, 0, 0},
                             {112, 16, 0, 0},
                             {120, 16, 0, 0}}};
sprite_pattern *gotpat[5] = {&gotpat0, &bonuspat, &gotpat1, &gotpat2, &gotpat3};
sprite_pattern *gotpat_0[5] = {&gotpat0, &bonuspat0, &gotpat1, &gotpat2,
                               &gotpat3};
sprite_pattern *madepat[5] = {&madepat0, &bonuspat, &madepat1, &madepat2,
                              &madepat3};
sprite_pattern *madepat_0[5] = {&madepat0, &bonuspat0, &madepat1, &madepat2,
                                &madepat3};
extern sprite_pattern *title_pat[];
extern Uint16 title_tbl[];
static Sint16 WaitCount;
static Uint16 ClearSountWait;
static Uint16 cleartbl[12] = {204, 0, 288, 0, 272, 512, 240, 1, 204, 0, 288, 2};

void over(sprite_status *pAct) {
    switch (pAct->r_no0) {
    case 0:
        over_init(pAct);
        break;
    case 2:
        over_move(pAct);
        break;
    }
}

static void over_init(sprite_status *pAct) {
    sprite_status *pTmpAct;

    sub_sync(130);
    pAct->r_no0 = 2;
    pAct->yposi.w.h = 224;
    pAct->xposi.w.h = 128;
    ((Sint16 *)pAct)[23] = 288;
    pAct->sproffset = 32768;
    pAct->patbase = gamepat;
    plsubchg_flag = 8;
    if (pltimeover_f & 1) {
        pltimeover_f &= 254;
        if (pl_suu) {
            pAct->patbase = timepat;
            plsubchg_flag = 2;
        }
    } else {

        pltimeover_f &= 254;
        if (pl_suu) {

            frameout(pAct);
            return;
        }
    }

    plsubchg_flag |= 128;
    if (actwkchk(&pTmpAct) != 0) {

        frameout(pAct);
        return;
    }

    pTmpAct->actno = 59;
    pTmpAct->r_no0 = pAct->r_no0;
    pTmpAct->sproffset = pAct->sproffset;
    pTmpAct->patbase = pAct->patbase;
    pTmpAct->patno = 1;
    pTmpAct->yposi.w.h = 224;
    pTmpAct->xposi.w.h = 448;
    ((Sint16 *)pTmpAct)[23] = 288;

    if (pl_suu) {
        over_move(pAct);
    } else {
        sub_sync(110);
    }
}

static void over_move(sprite_status *pAct) {
    if (pAct->xposi.w.h < (Sint16)((Uint16 *)pAct)[23]) {
        pAct->xposi.w.h += 8;
    } else if (pAct->xposi.w.h > (Sint16)((Uint16 *)pAct)[23]) {
        pAct->xposi.w.h -= 8;
    }
    actionsub(pAct);
}

void title(sprite_status *pAct) {
    switch (pAct->r_no0) {
    case 0:
        title_init(pAct);
        break;
    case 2:
        title_move0(pAct);
        break;
    case 4:
        title_move1(pAct);
        break;
    case 6:
        title_back0(pAct);
        break;
    case 8:
        title_back1(pAct);
        break;
    case 10:
        title_wait(pAct);
        break;
    }
}

static void title_init(sprite_status *pAct) {
    sprite_status *pTmpAct;
    Uint16 *wp;
    Sint32 i;

    pAct->r_no0 = 2;
    pAct->xposi.w.h = 280;
    pAct->yposi.w.h = 48;
    ((Sint16 *)pAct)[26] = 48;
    ((Sint16 *)pAct)[25] = 240;
    pAct->pattim = 90;
    pAct->sproffset = 32768;
    pAct->patbase = title_pat;
    pAct->sprpri = 4;

    wp = title_tbl;
    for (i = 0; i < 8; ++i) {
        actwkchk(&pTmpAct);
        pTmpAct->actno = 60;
        pTmpAct->r_no0 = 4;
        pTmpAct->sproffset = 32768;
        pTmpAct->patbase = title_pat;

        pTmpAct->yposi.w.h = *wp++;
        pTmpAct->xposi.w.h = *wp;
        ((Sint16 *)pTmpAct)[24] = *wp++;
        ((Sint16 *)pTmpAct)[23] = *wp++;
        pTmpAct->patno = (*wp & 65280) >> 8;
        if (i == 5) {
            pTmpAct->patno = pTmpAct->patno + (Uint8)stageno.b.l;
        }
        pTmpAct->pattim = *wp++ & 255;
    }
}

static void title_move0(sprite_status *pAct) {
    if (pAct->yposi.w.h == (Sint16)((Uint16 *)pAct)[25]) {
        pAct->r_no0 += 4;
    } else if (pAct->yposi.w.h < (Sint16)((Uint16 *)pAct)[25]) {
        pAct->yposi.w.h += 8;
    } else {
        pAct->yposi.w.h -= 8;
    }
    actionsub(pAct);
}

static void title_move1(sprite_status *pAct) {
    if (pAct->xposi.w.h == (Sint16)((Uint16 *)pAct)[23]) {
        pAct->r_no0 += 4;
    } else if (pAct->xposi.w.h < (Sint16)((Uint16 *)pAct)[23]) {
        pAct->xposi.w.h += 8;
    } else {
        pAct->xposi.w.h -= 8;
    }
    actionsub(pAct);
}

static void title_back0(sprite_status *pAct) {
    if (pAct->pattim) {
        --pAct->pattim;
    } else {
        if (pAct->yposi.w.h == (Sint16)((Uint16 *)pAct)[26]) {
            pAct->r_no0 += 4;
            scroll_start.b.h = 1;
            return;
        }
        if (pAct->yposi.w.h < (Sint16)((Uint16 *)pAct)[26]) {
            pAct->yposi.w.h += 16;
        } else {
            pAct->yposi.w.h -= 16;
        }
    }
    actionsub(pAct);
}

static void title_back1(sprite_status *pAct) {
    if (pAct->pattim) {
        --pAct->pattim;
    } else {
        if (pAct->xposi.w.h == (Sint16)((Uint16 *)pAct)[24]) {
            frameout(pAct);
            return;
        }
        if (pAct->xposi.w.h < (Sint16)((Uint16 *)pAct)[24]) {
            pAct->xposi.w.h += 16;
        } else {
            pAct->xposi.w.h -= 16;
        }
    }
    actionsub(pAct);
}

static void title_wait(sprite_status *pAct) {
    scroll_start.b.h = 0;
    plautoflag = 0;
    frameout(pAct);
}

void clear(sprite_status *pAct) {
    switch (pAct->r_no0) {
    case 0:
        clear_init0(pAct);
        ClearSountWait = systemtimer.w.l;
        break;
    case 2:
        clear_init(pAct);
        ClearSountWait = systemtimer.w.l;
        break;
    case 4:
        clear_move0(pAct);
        break;
    case 6:
        clear_move1(pAct);
        break;

    case 8:
        if ((Uint16)((Uint16)systemtimer.w.l - ClearSountWait) >= 720) {
            pAct->r_no0 += 2;
        }
        actionsub(pAct);
        break;
    case 10:
        clear_move2(pAct);
        break;
    case 12:
        clear_wait(pAct);
        break;
    }
}

static void clear_init0(sprite_status *pAct) {
    --pAct->actfree[8];
    if (!pAct->actfree[8]) {
        pAct->r_no0 = 2;
        clear_init(pAct);
    }

    WaitCount = 1;
    pAct->r_no0 = 12;
}

static void clear_init(sprite_status *pAct) {
    sprite_status *pTmpAct;
    Uint16 *wp;
    Sint32 i;

    if (stageno.w == 1282)
        goto label1;
    if (actwk[0].xposi.w.h > (Sint16)(scra_h_posit.w.h + 336)) {

    label1:
        pTmpAct = pAct;
        wp = cleartbl;
        for (i = 0; i < 3; ++i) {
            ((Sint16 *)pTmpAct)[27] = 360;
            pTmpAct->actno = 58;
            pTmpAct->r_no0 = 4;
            pTmpAct->sproffset = 32768;
            if (stageno.w == 1282) {
                pTmpAct->sproffset = 32768;
                pTmpAct->patbase = gotpat_0;
                if (generate_flag) {
                    pTmpAct->patbase = madepat_0;
                }
            } else {

                pTmpAct->patbase = gotpat;
                if (generate_flag) {
                    pTmpAct->patbase = madepat;
                }
            }

            pTmpAct->yposi.w.h = *wp++;
            pTmpAct->xposi.w.h = *wp++;
            ((Sint16 *)pTmpAct)[23] = *wp++;
            pTmpAct->patno = *wp++ & 255;

            if (i == 2) {
                pTmpAct->patno = pTmpAct->patno + stageno.b.l;
            }
            actwkchk(&pTmpAct);
        }
    }
}

static void clear_move0(sprite_status *pAct) {
    if (((Uint16 *)pAct)[27]) {
        --((Uint16 *)pAct)[27];
    }

    if (pAct->xposi.w.h == (Sint16)((Uint16 *)pAct)[23]) {

        if (!pAct->patno) {
            pAct->r_no0 += 2;
        }
    } else if (pAct->xposi.w.h > (Sint16)((Uint16 *)pAct)[23]) {
        pAct->xposi.w.h -= 8;
    } else {
        pAct->xposi.w.h += 8;
    }

    if (((Uint16 *)pAct)[27] < 352) {
        actionsub(pAct);
    }
}

static void clear_move1(sprite_status *pAct) {
    int_union lD0;

    lD0.l = 0;
    bonus_f = 1;
    if (!timebonus) {
        if (!ringbonus) {
            --((Uint16 *)pAct)[27];
            if (((Sint16 *)pAct)[27] < 0) {
                pAct->r_no0 += 2;
                if (systemtimer.w.l - ClearSountWait >= 540) {
                    ClearSountWait = systemtimer.w.l - 540;
                }
            }

            if (((Sint16 *)pAct)[27] == 30) {
                if (special_flag) {
                    soundset(200);
                }
            }

            actionsub(pAct);
            return;
        }
    }

    if (timebonus) {
        lD0.w.l += 10;
        timebonus -= 100;
    }
    if (ringbonus) {

        lD0.w.l += 10;
        ringbonus -= 100;
    }

    if (!timebonus && !ringbonus) {
        WaveAllStop();
        soundset(154);
        if (((Uint16 *)pAct)[27] >= 45) {
            ((Uint16 *)pAct)[27] = 45;
        }
    } else {

        if (((Uint16 *)pAct)[27]) {
            --((Uint16 *)pAct)[27];
        }

        if (pAct->actfree[8] % 2 == 0) {
            soundset(189);
        }
    }

    scoreup(lD0.l);
    actionsub(pAct);
}

static void clear_move2(sprite_status *pAct) {
    short_union wD0;

    gameflag.w = 2;
    plflag = 0;
    enecgflg = 0;

    flowercnt[0] = 0;
    flowercnt[1] = 0;
    flowercnt[2] = 0;
    tv_flag = 0;

    enkeino = 0;
    projector_flag = 0;
    markerno = 0;

    if (ta_flag) {
        play_start &= 254;
    }

    play_start &= 253;
    time_flag = 1;
    wD0.w = stageno.w;
    ++wD0.b.l;
    if (wD0.b.l == 2) {
        time_flag = 2;
    }
    if (wD0.b.l == 3) {
        ++wD0.b.h;
        wD0.b.l = 0;
    }
    stageno.w = wD0.w;

    flagwkclr();
    fadeout_s();
    actionsub(pAct);
    if (stageno.b.l == 0) {
        gf_flag = 0;
        return;
    }

    if (!ta_flag) {
        if (clrspflg_save != 127) {
            if (!generate_flag)
                return;
            generate_flag = 0;

            gf_flag |= 1 << stageno.b.l - 1;
            if (gf_flag != 3)
                return;
        }

        generate_flag = 1;
    }
}

static void clear_wait(sprite_status *pAct) {
    if (!--WaitCount)
        pAct->r_no0 = 2;
}
