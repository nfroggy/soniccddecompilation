#include "../equ.h"
#include "legmeca8.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../etc.h"
#include "../ridechk.h"

typedef struct {
    Uint8 routine;
    Uint8 roll_start_timer;
    Sint16 rotation;
    Sint16 segment_indices[9];
} legmeca8_work;

static legmeca8_work *legmeca8_get_work(sprite_status *pActwk) {
    return (legmeca8_work *)pActwk->actfree;
}

static void m_init(sprite_status *pActwk);
static void m0move(sprite_status *pActwk);
static void m_move_com(sprite_status *pActwk);
static void m1reset(sprite_status *pActwk);
static void m1move(sprite_status *pActwk);
static void m0reset(sprite_status *pActwk);
static void m_init_leg(sprite_status *pActwk);
static void leg_center(sprite_status *pActwk);
static void c_wait(sprite_status *pActwk);
static void c_roll(sprite_status *pActwk);
static void c_roll1(sprite_status *pActwk);
static void leg_radius(sprite_status *pActwk);
static void r_wait(sprite_status *pActwk);
static void r_roll(sprite_status *pActwk);
static void _ridechk(sprite_status *pActwk, sprite_status *pPlayerwk);
static void slave(sprite_status *pActwk);

static sprite_pattern pat00 = {2, {{-4, -4, 0, 463}, {-24, -40, 0, 464}}};
static sprite_pattern pat01 = {1, {{-4, -4, 0, 465}}};
sprite_pattern *pat_legmeca[2] = {&pat00, &pat01};

void legmeca(sprite_status *pActwk) {
    void (*tbl[5])(sprite_status *) = {&m_init, &m0move, &m1reset, &m1move,
                                       &m0reset};

    if (pActwk->userflag.b.h) {
        slave(pActwk);
    } else {
        tbl[legmeca8_get_work(pActwk)->routine / 2](pActwk);
    }
}

static void m_init(sprite_status *pActwk) {
    sprite_status *pNewActwk;
    legmeca8_work *pWork;
    Sint16 d4;
    Sint16 d5;
    Sint16 d6;
    Sint16 i;

    pWork = legmeca8_get_work(pActwk);
    d4 = pActwk->xposi.w.h;
    d5 = 8;
    d6 = 8;
    pNewActwk = pActwk;
    for (i = 0; i <= d6; ++i) {
        if (i) {
            if (actwkchk(&pNewActwk) != 0) {
                frameout(pActwk);
                return;
            }
            pNewActwk->actno = pActwk->actno;
            pNewActwk->userflag.b.h = 1;
            pNewActwk->sprpri = 3;
            pNewActwk->patno = 1;
            pNewActwk->sprhs = 4;
            pNewActwk->sprhsize = 4;
            pNewActwk->sprvsize = 4;
            pNewActwk->yposi.w.h = pActwk->yposi.w.h;
            d4 += d5;
            pNewActwk->xposi.w.h = d4;
        }

        pWork->segment_indices[i] = (Uint16)(pNewActwk - actwk);
        legmeca8_get_work(pNewActwk)->segment_indices[0] =
            (Uint16)(pActwk - actwk);
        pNewActwk->actflg |= 4;
        pNewActwk->sproffset = 792;
        pNewActwk->patbase = pat_legmeca;
    }
    m_init_leg(pNewActwk);
    m_init_leg(pActwk);

    pWork->routine += 2;
    pWork->roll_start_timer = 12;
    pWork->rotation = -32768;

    m0move(pActwk);
}

static void m0move(sprite_status *pActwk) {
    legmeca8_work *pWork = legmeca8_get_work(pActwk);

    leg_center(&actwk[pWork->segment_indices[8]]);

    leg_radius(pActwk);

    m_move_com(pActwk);
}

static void m_move_com(sprite_status *pActwk) {
    legmeca8_work *pWork = legmeca8_get_work(pActwk);

    actionsub(&actwk[pWork->segment_indices[1]]);
    actionsub(&actwk[pWork->segment_indices[2]]);
    actionsub(&actwk[pWork->segment_indices[3]]);
    actionsub(&actwk[pWork->segment_indices[4]]);
    actionsub(&actwk[pWork->segment_indices[5]]);
    actionsub(&actwk[pWork->segment_indices[6]]);
    actionsub(&actwk[pWork->segment_indices[7]]);

    frameout_s(pActwk);
}

static void m1reset(sprite_status *pActwk) {
    legmeca8_get_work(pActwk)->routine = 6;

    m1move(pActwk);
}

static void m1move(sprite_status *pActwk) {
    legmeca8_work *pWork = legmeca8_get_work(pActwk);

    leg_center(pActwk);

    leg_radius(&actwk[pWork->segment_indices[8]]);

    m_move_com(pActwk);
}

static void m0reset(sprite_status *pActwk) {
    legmeca8_get_work(pActwk)->routine = 2;
    m0move(pActwk);
}

static void m_init_leg(sprite_status *pActwk) {
    pActwk->patno = 0;
    pActwk->sprpri = 4;
    pActwk->sprhs = 24;
    pActwk->sprhsize = 24;
    pActwk->sprvsize = 41;
}

static void leg_center(sprite_status *pActwk) {
    void (*tbl[3])(sprite_status *) = {&c_wait, &c_roll, &c_roll1};

    tbl[pActwk->r_no0 / 2](pActwk);
}

static void c_wait(sprite_status *pActwk) {
    ridechk(pActwk, &actwk[0]);
    actionsub(pActwk);
}

static void c_roll(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->xposi.w.l = -32768;
    pActwk->yposi.w.l = -32768;
}

static void c_roll1(sprite_status *pActwk) {
    sprite_status *pMasterwk;
    sprite_status *pLegwk;
    sprite_status *pKusariwk;
    legmeca8_work *pMasterWork;
    int_union ld0, ld1, ld3, ld4, ld5;
    Sint16 d0, d1, d6, i;

    pMasterwk = &actwk[legmeca8_get_work(pActwk)->segment_indices[0]];
    pMasterWork = legmeca8_get_work(pMasterwk);
    pMasterWork->rotation += 512;

    i = 1;
    d6 = 6;
    ld5.l = pActwk->xposi.l;
    ld4.l = pActwk->yposi.l;
    ld1.l = 0;
    ld0.l = 0;
    sinset((Uint8)((Uint16)pMasterWork->rotation >> 8), &d0, &d1);
    ld1.w.h = d1;
    ld0.w.h = d0;
    ld1.l >>= 5;
    ld0.l >>= 5;
    do {
        pKusariwk = &actwk[pMasterWork->segment_indices[i++]];
        ld5.l += ld1.l;
        ld4.l += ld0.l;
        pKusariwk->xposi.l = ld5.l;
        pKusariwk->yposi.l = ld4.l;
    } while (d6--);

    pLegwk = &actwk[pMasterWork->segment_indices[8]];
    if (pActwk->userflag.b.h) {
        pLegwk = &actwk[pMasterWork->segment_indices[0]];
    }
    ld3.l = pLegwk->xposi.l;
    ld5.l += ld1.l;
    ld4.l += ld0.l;
    pLegwk->xposi.l = ld5.l;
    pLegwk->yposi.l = ld4.l;
    ld5.l -= ld3.l;
    ld5.l >>= 8;
    pLegwk->xspeed.w = ld5.w.l;

    c_wait(pActwk);
}

static void leg_radius(sprite_status *pActwk) {
    void (*tbl[2])(sprite_status *) = {&r_wait, &r_roll};

    tbl[pActwk->r_no0 / 2](pActwk);
}

static void r_wait(sprite_status *pActwk) {
    sprite_status *pMasterwk;
    sprite_status *pLegwk0;
    sprite_status *pLegwk1;

    if (ridechk(pActwk, &actwk[0])) {
        legmeca8_work *pMasterWork;

        pMasterwk = &actwk[legmeca8_get_work(pActwk)->segment_indices[0]];
        pMasterWork = legmeca8_get_work(pMasterwk);
        if (pMasterWork->roll_start_timer) {
            --pMasterWork->roll_start_timer;
            pLegwk0 = &actwk[pMasterWork->segment_indices[0]];
            pLegwk1 = &actwk[pMasterWork->segment_indices[8]];
            pLegwk0->r_no0 += 2;
            pLegwk1->r_no0 += 2;
        }
    }
    actionsub(pActwk);
}

static void r_roll(sprite_status *pActwk) {
    sprite_status *pMasterwk;
    sprite_status *pLegwk0;
    sprite_status *pLegwk1;
    Sint16 d0, d1;

    _ridechk(pActwk, &actwk[0]);
    actionsub(pActwk);

    pMasterwk = &actwk[legmeca8_get_work(pActwk)->segment_indices[0]];
    d0 = legmeca8_get_work(pMasterwk)->rotation;
    d0 += 16384;
    if (d0 >= 0) {
        d1 = emycol_d(pActwk);
        if (d1 < 0) {
            legmeca8_work *pMasterWork;

            d1 += 2;
            pActwk->yposi.w.h += d1;

            pMasterwk = &actwk[legmeca8_get_work(pActwk)->segment_indices[0]];
            pMasterWork = legmeca8_get_work(pMasterwk);
            pMasterWork->routine += 2;

            pMasterWork->rotation += 32768;
            pLegwk0 = &actwk[pMasterWork->segment_indices[0]];
            pLegwk1 = &actwk[pMasterWork->segment_indices[8]];
            pLegwk0->r_no0 = 0;
            pLegwk1->r_no0 = 0;
            pLegwk0->xspeed.w = 0;
            pLegwk1->xspeed.w = 0;
            pLegwk0->yspeed.w = 0;
            pLegwk1->yspeed.w = 0;
        }
    }
}

static void _ridechk(sprite_status *pActwk, sprite_status *pPlayerwk) {
    Sint16 stack;
    Uint8 d0;

    stack = pActwk->xspeed.w;
    pActwk->xspeed.w = 0;
    pActwk->sprvsize = 43;
    if (ridechk(pActwk, pPlayerwk))
        d0 = 255;
    else
        d0 = 0;
    pActwk->xspeed.w = stack;
    pActwk->sprvsize = 41;
    if (d0) {
        ridechk(pActwk, pPlayerwk);
    }
}

static void slave(sprite_status *pActwk) {
    sprite_status *pMasterwk;

    pMasterwk = &actwk[legmeca8_get_work(pActwk)->segment_indices[0]];
    if (pMasterwk->actno != 38) {
        frameout(pActwk);
    }
}
