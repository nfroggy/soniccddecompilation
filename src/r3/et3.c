#include "../equ.h"
#include "et3.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../playsub.h"
#include "../ridechk.h"
#include "../score.h"

#if defined(R31A)
#define SPRITE_ET_BASE 562
#elif defined(R31B) || defined(R32A)
#define SPRITE_ET_BASE 556
#elif defined(R33C) || defined(R33D)
#define SPRITE_ET_BASE 560
#else
#define SPRITE_ET_BASE 509
#endif

static void m_init(sprite_status *actionwk);
static void m_wait(sprite_status *actionwk);

static sprite_pattern tpat00 = {1, {{-40, 16, 0, SPRITE_ET_BASE}}};
static sprite_pattern tpat01 = {1, {{-40, 16, 0, SPRITE_ET_BASE + 1}}};
static sprite_pattern tpat07 = {1, {{0, 0, 0, 0}}};
static sprite_pattern tpat02 = {
    2, {{-24, -32, 0, SPRITE_ET_BASE + 7}, {-40, -32, 0, SPRITE_ET_BASE + 8}}};
static sprite_pattern tpat03 = {3,
                                {{-22, -8, 0, SPRITE_ET_BASE + 9},
                                 {-24, -32, 0, SPRITE_ET_BASE + 7},
                                 {-40, -32, 0, SPRITE_ET_BASE + 8}}};
static sprite_pattern tpat04 = {4,
                                {{-22, -8, 0, SPRITE_ET_BASE + 10},
                                 {-24, -32, 0, SPRITE_ET_BASE + 13},
                                 {-24, -32, 0, SPRITE_ET_BASE + 7},
                                 {-40, -32, 0, SPRITE_ET_BASE + 8}}};
static sprite_pattern tpat05 = {4,
                                {{-22, -8, 0, SPRITE_ET_BASE + 11},
                                 {-24, -32, 0, SPRITE_ET_BASE + 14},
                                 {-24, -32, 0, SPRITE_ET_BASE + 7},
                                 {-40, -32, 0, SPRITE_ET_BASE + 8}}};
static sprite_pattern tpat06 = {4,
                                {{-22, -8, 0, SPRITE_ET_BASE + 12},
                                 {-24, -32, 0, SPRITE_ET_BASE + 15},
                                 {-24, -32, 0, SPRITE_ET_BASE + 7},
                                 {-40, -32, 0, SPRITE_ET_BASE + 8}}};
sprite_pattern *pat_et[8] = {&tpat00, &tpat01, &tpat02, &tpat03,
                             &tpat04, &tpat05, &tpat06, &tpat07};
static Uint16 tbl0sproffset[11] = {1087, 1033, 0, 0, 1108, 1024, 0, 0, 0, 0, 0};
static char tbl0[64] = {
    1,   0,  0,   2,   -40, -20, 3,   28, 10, 4,   18,  -18, 5,   -18, -10, 6,
    8,   -8, 8,   -18, 14,  10,  -10, 10, 12, 30,  -10, 15,  0,   -18, 18,  20,
    -10, 20, -10, 18,  22,  8,   23,  25, 13, -10, 26,  23,  -22, 28,  -3,  -25,
    30,  10, 20,  32,  -10, 2,   34,  30, -8, 35,  13,  -10, 40,  -10, 10,  -1};
static char pchg0[6] = {3, 3, 4, 5, 6, -1};
static char *pchg[1] = {pchg0};

void et(sprite_status *actionwk) {
    switch (actionwk->r_no0) {
    case 0:
        m_init(actionwk);
    case 2:
        m_wait(actionwk);
        break;
    case 4:
        m_die(actionwk);
        break;
    case 6:
        m1wait(actionwk);
        break;
    }
    actionsub(actionwk);
    if (actionwk->r_no0 <= 2)
        frameout_s(actionwk);
}

static void m_init(sprite_status *actionwk) {
    Uint16 *a1;
    Uint16 d0;

    actionwk->r_no0 += 2;
    actionwk->actflg |= 4;
    actionwk->sprpri = 4;
    actionwk->sprhs = 34;
    actionwk->sprhsize = 34;
    actionwk->sprvsize = 32;

    a1 = tbl0sproffset;
    d0 = (stageno.w + 1 << 2) + (Uint16)time_flag;
    actionwk->sproffset = a1[d0];

    actionwk->patbase = pat_et;

    ((Sint16 *)actionwk)[24] = 0;
    ((Sint16 *)actionwk)[26] = actionwk->yposi.w.h;
    ((Sint16 *)actionwk)[23] = 4;
    ((Sint16 *)actionwk)[27] = 1;

    d0 = 0;
    if (generate_flag == 0)
        d0 += 2;
    if (time_flag == 0)
        ++d0;
    actionwk->patno = d0;
    if (generate_flag == 0 && time_flag == 0) {
        actionwk->colino = 250;
        actionwk->yposi.w.h -= 16;
    }
}

static void m_wait(sprite_status *actionwk) {
    Sint32 d0;
    sprite_status *a1;

    if (generate_flag != 0 || time_flag != 0)
        return;
    a_hover(actionwk);

    if (actionwk->colicnt == 0)
        goto label1;
    actionwk->colino = 0;
    ((Sint16 *)actionwk)[23] = 0;
    actionwk->patno = 7;
    actionwk->r_no0 += 2;
    generate_flag = 1;
    d0 = 150;
    scoreup(d0);
    a1 = &actwk[0];
    if (hitchk(actionwk, a1))
        ride_on_clr(actionwk, a1);
    return;
label1:
    hitchk(actionwk, &actwk[0]);
    patchg(actionwk, (Uint8 **)pchg);
}

void m_die(sprite_status *actionwk) {
    sprite_status *a1;
    char d0;
    Sint16 d5, d6, a6;

    a6 = ((Sint16 *)actionwk)[24];
    d0 = tbl0[a6++];
    if (d0 < 0) {
        actionwk->r_no0 += 2;
        ((char *)actionwk)[46] = 8;
        return;
    }
    ++((char *)actionwk)[46];
    if (((char *)actionwk)[46] != d0)
        return;
    d5 = tbl0[a6++];
    d6 = tbl0[a6++];
    ((Sint16 *)actionwk)[24] = a6;
    if (actwkchk(&a1) != 0)
        return;
    a1->actno = 24;
    a1->r_no1 = 1;
    a1->xposi = actionwk->xposi;
    a1->yposi = actionwk->yposi;
    a1->xposi.w.h += d5;
    a1->yposi.w.h += d6;
    baku_init(a1);
    soundset(158);
}

void m1wait(sprite_status *actionwk) {
    if (--actionwk->actfree[0])
        return;
    actionwk->r_no0 -= 6;
    actionwk->yposi.w.h = ((Sint16 *)actionwk)[26];
    soundset(217);
}

void a_hover(sprite_status *actionwk) {
    Uint16 d0;

    ++((Uint16 *)actionwk)[23];
    d0 = ((Uint16 *)actionwk)[23];
    d0 &= 7;
    if (d0 == 0) {
        d0 = ((Uint16 *)actionwk)[27];
        actionwk->yposi.w.h += d0;
    }
    d0 = ((Uint16 *)actionwk)[23];
    d0 &= 31;
    if (d0 == 0)
        ((Sint16 *)actionwk)[27] = -((Sint16 *)actionwk)[27];
}
