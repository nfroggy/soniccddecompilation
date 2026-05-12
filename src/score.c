#include "equ.h"
#include "score.h"
#include "action.h"
#include "impfuncs.h"
#include "loader2.h"

static void tensuu(sprite_status *pAct);
static void ten_init(sprite_status *pAct);
static void ten_move(sprite_status *pAct);
static void score_init(sprite_status *pAct);
static void score_move(sprite_status *pAct);
static void ringinit(void);
static void scoreinit(void);
static void posiwrt(void);
static void bonuswrt(sprite_data *pSprdat, Uint32 lDispVal);
static void ringwrt(sprite_data *pSprdat, Uint32 lDispVal);
static void scorewrt(sprite_data *pSprdat, Uint32 lDispVal);
static void scorewrt2(sprite_data *pSprdat, Uint32 lDispVal, Uint32 *subval,
                      Sint16 sD6, Sint16 zero_disp);
static void posiwrt0(sprite_data *pSprdat, Uint32 lDispVal);
static void playsuuwrt(sprite_data *pSprdat);
static void timewrt1(sprite_data *pSprdat, Uint32 lDispVal);
static void timewrt(sprite_data *pSprdat, Uint32 lDispVal);
static void timewrt0(sprite_data *pSprdat, Uint32 lDispVal, Uint32 *subval,
                     Sint16 sD6);

extern Sint16 playdieset(sprite_status *pActwk);

#if defined(R31) || defined(R32)
#define SPRITE_LIFEICON_PAST 359
#define SPRITE_LIFEICON_PRESENT 358
#define SPRITE_LIFEICON_FUTURE 357
#define SPRITE_RINGS_NORMAL 355
#define SPRITE_RINGS_DANGER 356
#define SPRITE_DIGIT_0 329
#define SPRITE_TEN_BASE 345
#define SPRITE_SCORE_BASE 350
#elif defined(R33)
#define SPRITE_LIFEICON_PAST 363
#define SPRITE_LIFEICON_PRESENT 362
#define SPRITE_LIFEICON_FUTURE 361
#define SPRITE_RINGS_NORMAL 359
#define SPRITE_RINGS_DANGER 360
#define SPRITE_DIGIT_0 333
#define SPRITE_TEN_BASE 349
#define SPRITE_SCORE_BASE 354
#elif defined(R6)
#define SPRITE_LIFEICON_PAST 358
#define SPRITE_LIFEICON_PRESENT 357
#define SPRITE_LIFEICON_FUTURE 356
#define SPRITE_RINGS_NORMAL 354
#define SPRITE_RINGS_DANGER 355
#define SPRITE_DIGIT_0 328
#define SPRITE_TEN_BASE 344
#define SPRITE_SCORE_BASE 349
#elif defined(R81)
#define SPRITE_LIFEICON_PAST 348
#define SPRITE_LIFEICON_PRESENT 347
#define SPRITE_LIFEICON_FUTURE 346
#define SPRITE_RINGS_NORMAL 344
#define SPRITE_RINGS_DANGER 345
#define SPRITE_DIGIT_0 318
#define SPRITE_TEN_BASE 334
#define SPRITE_SCORE_BASE 339
#elif defined(R82)
#define SPRITE_LIFEICON_PAST 357
#define SPRITE_LIFEICON_PRESENT 356
#define SPRITE_LIFEICON_FUTURE 355
#define SPRITE_RINGS_NORMAL 353
#define SPRITE_RINGS_DANGER 354
#define SPRITE_DIGIT_0 327
#define SPRITE_TEN_BASE 343
#define SPRITE_SCORE_BASE 348
#elif defined(R83)
#define SPRITE_LIFEICON_PAST 350
#define SPRITE_LIFEICON_PRESENT 349
#define SPRITE_LIFEICON_FUTURE 348
#define SPRITE_RINGS_NORMAL 346
#define SPRITE_RINGS_DANGER 347
#define SPRITE_DIGIT_0 320
#define SPRITE_TEN_BASE 336
#define SPRITE_SCORE_BASE 341
#else
#define SPRITE_LIFEICON_PAST 338
#define SPRITE_LIFEICON_PRESENT 337
#define SPRITE_LIFEICON_FUTURE 336
#define SPRITE_RINGS_NORMAL 334
#define SPRITE_RINGS_DANGER 335
#define SPRITE_DIGIT_0 308
#define SPRITE_TEN_BASE 324
#define SPRITE_SCORE_BASE 329
#endif

static Uint32 subtbl[6] = {100000, 10000, 1000, 100, 10, 1};
static Uint32 subtblh4[4] = {4096, 256, 16, 1};
static sprite_pattern tenpat0 = {
    2, {{-8, 0, 0, SPRITE_TEN_BASE}, {0, 0, 0, SPRITE_TEN_BASE + 3}}};
static sprite_pattern tenpat1 = {
    2, {{-8, 0, 0, SPRITE_TEN_BASE + 1}, {0, 0, 0, SPRITE_TEN_BASE + 3}}};
static sprite_pattern tenpat2 = {
    2, {{-8, 0, 0, SPRITE_TEN_BASE + 2}, {0, 0, 0, SPRITE_TEN_BASE + 3}}};
static sprite_pattern tenpat3 = {
    2, {{-8, 0, 0, SPRITE_TEN_BASE + 4}, {0, 0, 0, SPRITE_TEN_BASE + 3}}};
static sprite_pattern tenpat4 = {1, {{-4, 0, 0, SPRITE_TEN_BASE + 4}}};
static sprite_pattern tenpat5 = {3,
                                 {{-12, 0, 0, SPRITE_TEN_BASE},
                                  {-4, 0, 0, SPRITE_TEN_BASE + 3},
                                  {4, 0, 0, SPRITE_TEN_BASE + 3}}};
static sprite_pattern *tenpat[6] = {&tenpat0, &tenpat1, &tenpat2,
                                    &tenpat3, &tenpat4, &tenpat5};
sprite_pattern scorepat0 = {19,
                            {{0, 0, 0, SPRITE_SCORE_BASE},
                             {40, 0, 0, 0},
                             {48, 0, 0, 0},
                             {56, 0, 0, 0},
                             {64, 0, 0, 0},
                             {72, 0, 0, 0},
                             {80, 0, 0, 0},
                             {88, 0, 0, SPRITE_DIGIT_0},
                             {0, 16, 0, SPRITE_SCORE_BASE + 1},
                             {40, 16, 0, SPRITE_DIGIT_0},
                             {48, 16, 0, SPRITE_SCORE_BASE + 2},
                             {56, 16, 0, SPRITE_DIGIT_0},
                             {64, 16, 0, SPRITE_DIGIT_0},
                             {72, 16, 0, SPRITE_SCORE_BASE + 3},
                             {80, 16, 0, SPRITE_DIGIT_0},
                             {88, 16, 0, SPRITE_DIGIT_0},
                             {48, 32, 0, 0},
                             {56, 32, 0, 0},
                             {64, 32, 0, SPRITE_DIGIT_0}}};
sprite_pattern scorepat1 = {3,
                            {{0, 0, 0, SPRITE_SCORE_BASE + 8},
                             {16, 8, 0, SPRITE_SCORE_BASE + 4},
                             {24, 4, 0, SPRITE_DIGIT_0}}};
sprite_pattern scorepat2 = {15,
                            {{0, 0, 0, SPRITE_SCORE_BASE},
                             {40, 0, 0, 0},
                             {48, 0, 0, 0},
                             {56, 0, 0, 0},
                             {64, 0, 0, 0},
                             {72, 0, 0, 0},
                             {80, 0, 0, SPRITE_DIGIT_0},
                             {0, 16, 0, SPRITE_SCORE_BASE + 1},
                             {56, 16, 0, 0},
                             {64, 16, 0, 0},
                             {72, 16, 0, 0},
                             {80, 16, 0, SPRITE_DIGIT_0},
                             {48, 32, 0, 0},
                             {56, 32, 0, 0},
                             {64, 32, 0, SPRITE_DIGIT_0}}};
sprite_pattern scorepat3 = {1, {{0, 32, 0, SPRITE_SCORE_BASE + 5}}};
static sprite_pattern *scorepat[4] = {&scorepat0, &scorepat1, &scorepat2,
                                      &scorepat3};
extern sprite_pattern bonuspat;
extern sprite_pattern bonuspat0;
Uint8 scoreinittbl[7] = {255, 255, 255, 255, 255, 255, 0};
Uint8 ringinittbl[3] = {255, 255, 0};

static void tensuu(sprite_status *pAct) {
    if (!pAct->r_no0)
        ten_init(pAct);
    ten_move(pAct);
    actionsub(pAct);
}

static void ten_init(sprite_status *pAct) {
    pAct->r_no0 = 2;
    pAct->actflg = 4;
    pAct->patbase = tenpat;
    pAct->patno = pAct->userflag.b.h & 127;
    pAct->actfree[0] = 24;
}

static void ten_move(sprite_status *pAct) {
    --pAct->actfree[0];

    if (!pAct->actfree[0])
        frameout(pAct);
    pAct->yposi.w.h -= 2;
}

void score(sprite_status *pAct) {
    if ((Uint8)pAct->userflag.b.h & 128) {
        tensuu(pAct);
        return;
    }
    if (!pAct->r_no0)
        score_init(pAct);
    score_move(pAct);
}

static void score_init(sprite_status *pAct) {
    pAct->r_no0 = 2;
    pAct->patbase = scorepat;
    pAct->sproffset = 32768;
    pAct->xposi.w.h = 144;
    pAct->yposi.w.h = 136;
    if (pAct->userflag.b.l) {
        pAct->patno = 3;
        return;
    }

    if (debugflag.w) {
        pAct->patno = 2;
    }

    if (pAct->userflag.b.h) {
        pAct->yposi.w.h = 328;
        pAct->patno = 1;

        switch (time_flag & 3) {
        case 0:
            scorepat1.spra[0].index = SPRITE_LIFEICON_PAST;
            break;
        case 1:
            scorepat1.spra[0].index = SPRITE_LIFEICON_PRESENT;
            break;
        default:
            scorepat1.spra[0].index = SPRITE_LIFEICON_FUTURE;
            break;
        }
    }
}

static void score_move(sprite_status *pAct) {
    if (!pAct->userflag.b.h) {
        if (pAct->userflag.b.l) {
            if (plring) {
                pAct->sproffset = 32768;
                scorepat3.spra[0].index = SPRITE_RINGS_NORMAL;

            } else {

                if (!(systemtimer.b.b4 & 15)) {
                    if (scorepat3.spra[0].index == SPRITE_RINGS_NORMAL)
                        scorepat3.spra[0].index = SPRITE_RINGS_DANGER;
                    else
                        scorepat3.spra[0].index = SPRITE_RINGS_NORMAL;
                }
            }
        } else {

            pAct->patno = 0;
            if (debugflag.w) {
                pAct->patno = 2;
            }
        }
    }

    actionsub(pAct);
}

void scoreup(Uint32 updata) {
    plscore_f = 1;

    plscore += updata;
    if ((Uint32)plscore > 999999) {

        plscore = 999999;
    }
    if (extrascore > (Uint32)plscore)
        return;

    extrascore += 5000;
    ++pl_suu;
    ++pl_suu_f;
    WaveAllStop();
    sub_sync(122);
}

void scoreset(void) {
    sprite_data *pSprdat;
    sprite_pattern *pSprpat;

    pSprdat = &scorepat2.spra[3];
    if (debugflag.w) {
        posiwrt();

        pSprdat = &scorepat2.spra[12];
        timewrt0(pSprdat, blkno & 2047, &subtbl[3], 2);
    } else {

        if (plscore_f) {
            if (plscore_f & 128) {
                scoreinit();
            }

            plscore_f = 0;
            pSprdat = &scorepat0.spra[1];
            scorewrt(pSprdat, plscore);
        }

        if (plring_f) {
            if (plring_f & 128) {
                ringinit();
            }

            plring_f = 0;
            if (plring > 999)
                plring = 999;

            pSprdat = &scorepat0.spra[16];
            ringwrt(pSprdat, plring);
        }
    }

    if (!debugflag.w && pltime_f && !pauseflag.b.h) {
        if (pltime.l == 604987) {

            if (time_flag & 128)
                return;

            pltime_f = 0;
            pltime.l = 0;
            playdieset(&actwk[0]);
            pltimeover_f = 1;

            return;
        }
        if (pltime.l != 604987) {
            if (!plautoflag) {

                ++pltime.b.b4;
                if ((Uint8)pltime.b.b4 >= 60) {
                    pltime.b.b4 = 0;
                    ++pltime.b.b3;
                    if ((Uint8)pltime.b.b3 >= 60) {
                        pltime.b.b3 = 0;
                        ++pltime.b.b2;
                        if ((Uint8)pltime.b.b2 >= 9) {
                            pltime.b.b2 = 9;
                        }
                    }
                }
            }
        }

        pSprdat = &scorepat0.spra[9];
        timewrt1(pSprdat, pltime.b.b2);

        pSprdat = &scorepat0.spra[11];
        timewrt(pSprdat, pltime.b.b3);

        pSprdat = &scorepat0.spra[14];
        if (pltime.l == 604987) {
            timewrt(pSprdat, 99);
        } else {
            timewrt(pSprdat, (pltime.b.b4 * 100) / 60);
        }
    }

    if (pl_suu_f) {
        pl_suu_f = 0;
        pSprdat = &scorepat1.spra[2];
        playsuuwrt(pSprdat);
    }

    if (bonus_f) {
        bonus_f = 0;
        if (stageno.w == 1282)
            pSprpat = &bonuspat0;
        else
            pSprpat = &bonuspat;
        bonuswrt(&pSprpat->spra[8], (Uint16)ringbonus);
        bonuswrt(&pSprpat->spra[13], (Uint16)timebonus);
        scorewrt(&pSprpat->spra[1], plscore);
        pSprpat->spra[7].index = SPRITE_DIGIT_0;
    }
}

static void ringinit(void) {}

static void scoreinit(void) {
    sprite_data *pSprdat;

    pSprdat = &scorepat1.spra[2];
    playsuuwrt(pSprdat);
}

static void posiwrt(void) {
    sprite_data *pSprdat;
    int_union lwk;

    lwk.l = 0;
    lwk.w.l = actwk[0].xposi.w.h;
    pSprdat = &scorepat2.spra[3];
    posiwrt0(pSprdat, lwk.l);

    lwk.w.l = actwk[0].yposi.w.h;
    pSprdat = &scorepat2.spra[8];
    posiwrt0(pSprdat, lwk.l);
}

static void bonuswrt(sprite_data *pSprdat, Uint32 lDispVal) {
    scorewrt2(pSprdat, lDispVal, &subtbl[1], 4, 1);
}

static void ringwrt(sprite_data *pSprdat, Uint32 lDispVal) {
    scorewrt2(pSprdat, lDispVal, &subtbl[3], 2, 1);
}

static void scorewrt(sprite_data *pSprdat, Uint32 lDispVal) {
    scorewrt2(pSprdat, lDispVal, &subtbl[0], 5, 0);
}

static void scorewrt2(sprite_data *pSprdat, Uint32 lDispVal, Uint32 *subval,
                      Sint16 sD6, Sint16 zero_disp) {
    Uint32 lDisp1;
    Sint32 wrt = 0;

    do {
        lDisp1 = lDispVal / *subval;
        lDispVal -= lDisp1 * *subval;
        ++subval;
        if (lDisp1 != 0) {
            wrt = 1;
        }
        if (wrt != 0 || (sD6 == 0 && zero_disp)) {
            pSprdat->index = (Uint16)lDisp1 + SPRITE_DIGIT_0;
        } else {
            pSprdat->index = 0;
        }
        ++pSprdat;
    } while (--sD6 >= 0);
}

static void posiwrt0(sprite_data *pSprdat, Uint32 lDispVal) {
    timewrt0(pSprdat, lDispVal, &subtblh4[0], 3);
}

static void playsuuwrt(sprite_data *pSprdat) {
    Uint32 lDispVal;

    if (pl_suu > 9)
        lDispVal = 9;
    else
        lDispVal = pl_suu;

    timewrt0(pSprdat, lDispVal, &subtbl[5], 0);
}

static void timewrt1(sprite_data *pSprdat, Uint32 lDispVal) {
    timewrt0(pSprdat, lDispVal, &subtbl[5], 0);
}

static void timewrt(sprite_data *pSprdat, Uint32 lDispVal) {
    timewrt0(pSprdat, lDispVal, &subtbl[4], 1);
}

static void timewrt0(sprite_data *pSprdat, Uint32 lDispVal, Uint32 *subval,
                     Sint16 sD6) {
    Uint16 lDisp1;

    do {
        lDisp1 = lDispVal / *subval;
        lDispVal -= lDisp1 * *subval;
        ++subval;
        pSprdat->index = lDisp1 + SPRITE_DIGIT_0;
        ++pSprdat;
    } while (--sD6 >= 0);
}
