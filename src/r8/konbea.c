#include "../equ.h"
#include "konbea.h"
#include "../action.h"
#include "../actset.h"
#include "../ridechk.h"
#include "../playsub.h"

typedef struct {
    union {
        struct {
            Sint16 parent_index;
            Sint16 timer;
            Sint16 origin_x;
            Sint16 origin_y;
            Uint8 riding_flag;
        } straight;
        struct {
            Sint16 parent_index;
            Sint16 origin_x;
            Sint16 origin_y;
            union {
                Sint32 rotation_accumulator;
                struct {
                    Uint16 rotation_fraction;
                    Sint16 angle_index;
                };
            };
            Sint16 roll_timer;
            Uint8 riding_flag;
        } moving;
    };
} konbea_work;

static konbea_work *konbea_get_work(sprite_status *pActwk) {
    return (konbea_work *)pActwk->actfree;
}

static void a_init(sprite_status *pActwk);
static void a_init_sub(sprite_status *pActwk, sprite_status *pNewact);
static void a_stop(sprite_status *pActwk);
static void a_stop1(sprite_status *pActwk);
static void a_move(sprite_status *pActwk);
static void move_version(sprite_status *pActwk);
static void b_init(sprite_status *pActwk);
static void b_init_sub(sprite_status *pActwk, sprite_status *pNewact);
static void b_stop(sprite_status *pActwk);
static void b_stop1(sprite_status *pActwk);
static void b_roll(sprite_status *pActwk);
static void b_move(sprite_status *pActwk);

char konbea8tbl[892] = {
    0,    26,  -1,   25,  -2,   25,  -3,   24,  -4,   24,  -5,   23,  -6,   23,
    -7,   22,  -8,   22,  -9,   21,  -10,  21,  -11,  20,  -12,  20,  -13,  19,
    -14,  19,  -15,  18,  -16,  18,  -17,  17,  -18,  17,  -19,  16,  -20,  16,
    -21,  15,  -22,  15,  -23,  14,  -24,  14,  -25,  13,  -26,  13,  -27,  12,
    -28,  12,  -29,  11,  -30,  11,  -31,  10,  -32,  10,  -33,  9,   -34,  9,
    -35,  8,   -36,  8,   -37,  7,   -38,  7,   -39,  6,   -40,  6,   -41,  5,
    -42,  5,   -43,  4,   -44,  4,   -45,  3,   -46,  3,   -47,  2,   -48,  2,
    -49,  1,   -50,  1,   -51,  0,   -52,  0,   -53,  -1,  -54,  -1,  -55,  -2,
    -56,  -2,  -57,  -3,  -58,  -3,  -59,  -4,  -60,  -4,  -61,  -5,  -62,  -5,
    -63,  -6,  -64,  -6,  -65,  -7,  -66,  -7,  -67,  -8,  -68,  -8,  -69,  -9,
    -70,  -9,  -71,  -10, -72,  -10, -73,  -11, -74,  -11, -75,  -12, -76,  -12,
    -77,  -13, -78,  -13, -79,  -14, -80,  -14, -81,  -15, -82,  -15, -83,  -16,
    -84,  -16, -85,  -17, -86,  -17, -87,  -18, -88,  -18, -89,  -19, -90,  -19,
    -91,  -20, -92,  -21, -93,  -22, -94,  -23, -95,  -24, -96,  -25, -97,  -26,
    -98,  -27, -99,  -28, -99,  -29, -100, -30, -100, -31, -100, -32, -101, -33,
    -101, -34, -101, -35, -102, -36, -102, -37, -102, -38, -102, -39, -102, -40,
    -102, -41, -102, -42, -102, -43, -102, -44, -102, -45, -101, -46, -101, -47,
    -101, -48, -100, -49, -100, -50, -100, -51, -99,  -52, -98,  -53, -98,  -54,
    -97,  -55, -96,  -56, -95,  -57, -94,  -58, -93,  -59, -92,  -60, -91,  -60,
    -90,  -61, -89,  -61, -88,  -61, -87,  -62, -86,  -62, -85,  -62, -84,  -63,
    -83,  -63, -82,  -63, -81,  -63, -80,  -63, -79,  -63, -78,  -63, -77,  -63,
    -76,  -63, -75,  -63, -74,  -63, -73,  -63, -72,  -62, -71,  -62, -70,  -62,
    -69,  -61, -68,  -61, -67,  -60, -66,  -60, -65,  -59, -64,  -59, -63,  -58,
    -62,  -58, -61,  -57, -60,  -57, -59,  -56, -58,  -56, -57,  -55, -56,  -55,
    -55,  -54, -54,  -54, -53,  -53, -52,  -53, -51,  -52, -50,  -52, -49,  -51,
    -48,  -51, -47,  -50, -46,  -50, -45,  -49, -44,  -49, -43,  -48, -42,  -48,
    -41,  -47, -40,  -47, -39,  -46, -38,  -46, -37,  -45, -36,  -45, -35,  -44,
    -34,  -44, -33,  -43, -32,  -43, -31,  -42, -30,  -42, -29,  -41, -28,  -41,
    -27,  -40, -26,  -40, -25,  -39, -24,  -39, -23,  -38, -22,  -38, -21,  -37,
    -20,  -37, -19,  -36, -18,  -36, -17,  -35, -16,  -35, -15,  -34, -14,  -34,
    -13,  -33, -12,  -33, -11,  -32, -10,  -32, -9,   -31, -8,   -31, -7,   -30,
    -6,   -30, -5,   -29, -4,   -29, -3,   -28, -2,   -28, -1,   -27, 0,    -26,
    1,    -25, 2,    -25, 3,    -24, 4,    -24, 5,    -23, 6,    -23, 7,    -22,
    8,    -22, 9,    -21, 10,   -21, 11,   -20, 12,   -20, 13,   -19, 14,   -19,
    15,   -18, 16,   -18, 17,   -17, 18,   -17, 19,   -16, 20,   -16, 21,   -15,
    22,   -15, 23,   -14, 24,   -14, 25,   -13, 26,   -13, 27,   -12, 28,   -12,
    29,   -11, 30,   -11, 31,   -10, 32,   -10, 33,   -9,  34,   -9,  35,   -8,
    36,   -8,  37,   -7,  38,   -7,  39,   -6,  40,   -6,  41,   -5,  42,   -5,
    43,   -4,  44,   -4,  45,   -3,  46,   -3,  47,   -2,  48,   -2,  49,   -1,
    50,   -1,  51,   0,   52,   0,   53,   1,   54,   1,   55,   2,   56,   2,
    57,   3,   58,   3,   59,   4,   60,   4,   61,   5,   62,   5,   63,   6,
    64,   6,   65,   7,   66,   7,   67,   8,   68,   8,   69,   9,   70,   9,
    71,   10,  72,   10,  73,   11,  74,   11,  75,   12,  76,   12,  77,   13,
    78,   13,  79,   14,  80,   14,  81,   15,  82,   15,  83,   16,  84,   16,
    85,   17,  86,   17,  87,   18,  88,   18,  89,   19,  90,   19,  91,   20,
    92,   21,  93,   22,  94,   23,  95,   24,  96,   25,  97,   26,  98,   27,
    99,   28,  99,   29,  100,  30,  100,  31,  100,  32,  101,  33,  101,  34,
    101,  35,  102,  36,  102,  37,  102,  38,  102,  39,  102,  40,  102,  41,
    102,  42,  102,  43,  102,  44,  102,  45,  101,  46,  101,  47,  101,  48,
    100,  49,  100,  50,  100,  51,  99,   52,  98,   53,  98,   54,  97,   55,
    96,   56,  95,   57,  94,   58,  93,   59,  92,   60,  91,   60,  90,   61,
    89,   61,  88,   61,  87,   62,  86,   62,  85,   62,  84,   63,  83,   63,
    82,   63,  81,   63,  80,   63,  79,   63,  78,   63,  77,   63,  76,   63,
    75,   63,  74,   63,  73,   63,  72,   62,  71,   62,  70,   62,  69,   61,
    68,   61,  67,   60,  66,   60,  65,   59,  64,   59,  63,   58,  62,   58,
    61,   57,  60,   57,  59,   56,  58,   56,  57,   55,  56,   55,  55,   54,
    54,   54,  53,   53,  52,   53,  51,   52,  50,   52,  49,   51,  48,   51,
    47,   50,  46,   50,  45,   49,  44,   49,  43,   48,  42,   48,  41,   47,
    40,   47,  39,   46,  38,   46,  37,   45,  36,   45,  35,   44,  34,   44,
    33,   43,  32,   43,  31,   42,  30,   42,  29,   41,  28,   41,  27,   40,
    26,   40,  25,   39,  24,   39,  23,   38,  22,   38,  21,   37,  20,   37,
    19,   36,  18,   36,  17,   35,  16,   35,  15,   34,  14,   34,  13,   33,
    12,   33,  11,   32,  10,   32,  9,    31,  8,    31,  7,    30,  6,    30,
    5,    29,  4,    29,  3,    28,  2,    28,  1,    27};
static Sint16 a_tbl_00[2] = {60, 0};
static Sint16 a_tbl_01[5] = {60, 1, 60, 32, 0};
static Sint16 *a_tbl0[2] = {a_tbl_00, a_tbl_01};
static Uint8 pchg_00[14] = {1, 1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 0, 255};
static Uint8 *pchg[1] = {pchg_00};
static sprite_pattern pat_00 = {1, {{-16, -8, 0, 408}}};
static sprite_pattern pat_01 = {1, {{-16, -12, 0, 409}}};
static sprite_pattern pat_02 = {1, {{-12, -16, 0, 410}}};
static sprite_pattern pat_03 = {1, {{-8, -16, 0, 411}}};
static sprite_pattern pat_04 = {1, {{-12, -16, 0, 412}}};
static sprite_pattern pat_05 = {1, {{-16, -12, 0, 413}}};
sprite_pattern *pat_konbea[7] = {&pat_00, &pat_01, &pat_02, &pat_03,
                                 &pat_04, &pat_05, &pat_00};
static void (*a_act_tbl[4])(sprite_status *) = {&a_init, &a_stop, &a_stop1,
                                                &a_move};
static void (*b_act_tbl[4])(sprite_status *) = {&b_init, &b_stop, &b_stop1,
                                                &b_roll};

void konbea(sprite_status *pActwk) {
    sprite_status *pMainwk;
    konbea_work *pWork = konbea_get_work(pActwk);

    if (!pActwk->userflag.b.h) {

        if (pActwk->userflag.b.l) {
            pMainwk = &actwk[pWork->straight.parent_index];

            if (pMainwk->actno != 42) {
                frameout(pActwk);
                return;
            }

            if (pWork->straight.origin_x !=
                konbea_get_work(pMainwk)->straight.origin_x) {

                frameout(pActwk);
                return;
            }

            if (pWork->straight.origin_y !=
                konbea_get_work(pMainwk)->straight.origin_y) {

                frameout(pActwk);
                return;
            }
        }

        a_act_tbl[pActwk->r_no0 / 2](pActwk);
        actionsub(pActwk);

        if (!(pActwk->userflag.b.l & 128)) {
            frameout_s00(pActwk, pWork->straight.origin_x);
        }

    } else {
        move_version(pActwk);
    }
}

static void a_init(sprite_status *pActwk) {
    Sint16 *pTbl, i;
    sprite_status *pNewact;

    pTbl = a_tbl0[pActwk->userflag.b.l];

    konbea_get_work(pActwk)->straight.timer = *pTbl++;
    pNewact = pActwk;
    a_init_sub(pActwk, pNewact);

    for (i = *pTbl++ - 1; i >= 0; --i) {

        if (actwkchk(&pNewact) != 0) {
            frameout(pActwk);
            break;
        }

        pNewact->actno = pActwk->actno;
        konbea_get_work(pNewact)->straight.parent_index = pActwk - actwk;
        pNewact->userflag.b.h = pActwk->userflag.b.h;
        pNewact->userflag.b.l = -1;
        konbea_get_work(pNewact)->straight.timer = *pTbl++;
        pNewact->xposi.w.h = pActwk->xposi.w.h + *pTbl++;
        pNewact->yposi.w.h = pActwk->yposi.w.h + *pTbl++;
        a_init_sub(pActwk, pNewact);
    }
}

static void a_init_sub(sprite_status *pActwk, sprite_status *pNewact) {
    pNewact->r_no0 = 2;
    pNewact->actflg |= 4;
    pNewact->sprpri = 3;
    pNewact->sprhs = pNewact->sprhsize = 16;
    pNewact->sprvsize = 5;
    pNewact->sproffset = 844;
    pNewact->patbase = pat_konbea;
    konbea_get_work(pNewact)->straight.origin_x = pActwk->xposi.w.h;
    konbea_get_work(pNewact)->straight.origin_y = pActwk->yposi.w.h;
}

static void a_stop(sprite_status *pActwk) {
    pActwk->sprvsize = 5;
    konbea_get_work(pActwk)->straight.timer = 120;
    pActwk->r_no0 += 2;
    a_stop1(pActwk);
}

static void a_stop1(sprite_status *pActwk) {
    konbea_work *pWork = konbea_get_work(pActwk);

    if (ridechk(pActwk, &actwk[0])) {
        pWork->straight.riding_flag = 255;
    } else {
        pWork->straight.riding_flag = 0;
    }

    if (!(--pWork->straight.timer)) {
        pActwk->sprvsize = 16;
        pActwk->mstno.w = 255;
        pActwk->r_no0 += 2;

        if (pWork->straight.riding_flag) {
            ride_on_clr(pActwk, &actwk[0]);
        }
    }
}

static void a_move(sprite_status *pActwk) {
    patchg(pActwk, pchg);

    if (!pActwk->patno) {
        pActwk->r_no0 = 2;
    }
}

static void move_version(sprite_status *pActwk) {
    sprite_status *pMainwk;
    konbea_work *pWork = konbea_get_work(pActwk);

    if (pActwk->userflag.b.l) {
        pMainwk = &actwk[pWork->moving.parent_index];

        if (pMainwk->actno != 42) {
            frameout(pActwk);
            return;
        }

        if (pWork->moving.origin_x !=
            konbea_get_work(pMainwk)->moving.origin_x) {

            frameout(pActwk);
            return;
        }

        if (pWork->moving.origin_y !=
            konbea_get_work(pMainwk)->moving.origin_y) {

            frameout(pActwk);
            return;
        }
    }

    b_act_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);

    if (!(pActwk->userflag.b.l & 128)) {
        frameout_s00(pActwk, pWork->moving.origin_x);
    }
}

static void b_init(sprite_status *pActwk) {
    sprite_status *pNewact;
    Sint32 lp;
    Sint16 wk1, wk2;

    pNewact = pActwk;
    b_init_sub(pActwk, pNewact);

    wk1 = wk2 = 89;
    for (lp = 3; lp >= 0; --lp) {

        if (actwkchk(&pNewact) != 0) {
            frameout_s(pActwk);
            return;
        }

        pNewact->actno = pActwk->actno;
        konbea_get_work(pNewact)->moving.parent_index = pActwk - actwk;
        pNewact->userflag.b.h = pActwk->userflag.b.h;
        pNewact->userflag.b.l = -1;
        pNewact->xposi.w.h = pActwk->xposi.w.h;
        pNewact->yposi.w.h = pActwk->yposi.w.h;
        konbea_get_work(pNewact)->moving.angle_index = wk2;
        wk2 += wk1;
        b_init_sub(pActwk, pNewact);
    }
}

static void b_init_sub(sprite_status *pActwk, sprite_status *pNewact) {
    pNewact->r_no0 = 2;
    pNewact->actflg |= 4;
    pNewact->sprpri = 3;
    pNewact->sprhs = pNewact->sprhsize = 16;
    pNewact->sprvsize = 5;
    pNewact->sproffset = 844;
    pNewact->patbase = pat_konbea;
    konbea_get_work(pNewact)->moving.origin_x = pActwk->xposi.w.h;
    konbea_get_work(pNewact)->moving.origin_y = pActwk->yposi.w.h;
}

static void b_stop(sprite_status *pActwk) {
    pActwk->sprvsize = 5;
    pActwk->r_no0 += 2;
    b_stop1(pActwk);
}

static void b_stop1(sprite_status *pActwk) {
    konbea_work *pWork = konbea_get_work(pActwk);

    b_move(pActwk);

    if (ridechk(pActwk, &actwk[0])) {
        pWork->moving.riding_flag = 255;
    } else {
        pWork->moving.riding_flag = 0;
    }

    if (pWork->moving.angle_index == 344) {
        pActwk->r_no0 += 2;
        pActwk->sprvsize = 16;
        pWork->moving.roll_timer = 216;

        if (pWork->moving.riding_flag) {
            ride_on_clr(pActwk, &actwk[0]);
        }
    }
}

static void b_roll(sprite_status *pActwk) {
    short_union patnowk;
    konbea_work *pWork = konbea_get_work(pActwk);

    b_move(pActwk);

    if (--pWork->moving.roll_timer < 0) {
        pActwk->patno = 0;
        pActwk->r_no0 = 2;
    } else {
        patnowk.b.h = pActwk->patno;
        patnowk.b.l = pActwk->patcnt;

        if ((patnowk.w += 128) > 1280) {
            patnowk.w = 0;
        }

        pActwk->patno = patnowk.b.h;
        pActwk->patcnt = patnowk.b.l;
    }
}

static void b_move(sprite_status *pActwk) {
    Sint16 xsav, idx, xwk, ywk;
    char *pKonbeaTbl;
    konbea_work *pWork = konbea_get_work(pActwk);

    xsav = pActwk->xposi.w.h;
    idx = pWork->moving.angle_index;
    pKonbeaTbl = &konbea8tbl[idx * 2];

    xwk = pWork->moving.origin_x + (Sint16)*pKonbeaTbl++;
    ywk = pWork->moving.origin_y + (Sint16)*pKonbeaTbl++;
    pActwk->xposi.w.h = xwk;
    pActwk->yposi.w.h = ywk;
    pActwk->xspeed.w = xwk - xsav << 8;

    if ((pWork->moving.rotation_accumulator += 65536) >= 29229056) {
        pWork->moving.rotation_accumulator = 0;
    }
}
