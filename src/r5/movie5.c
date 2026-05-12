#include "../equ.h"
#include "movie5.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../playsub.h"
#include "../ridechk.h"

static void m_init(sprite_status *pActwk);
static void m_wait(sprite_status *pActwk);
static void m_die(sprite_status *pActwk);
static void m_baku(sprite_status *pActwk);
static void m1wait(sprite_status *pActwk);
static void s_init(sprite_status *pActwk);
static void s_move(sprite_status *pActwk);

static char tbl0[37] = {1,   0,   0,   5,   -18, -10, 10, -10, 10,  15,
                        0,   -18, 20,  -10, 18,  22,  8,  23,  25,  13,
                        -10, 28,  -3,  -25, 30,  10,  20, 32,  -10, 2,
                        35,  13,  -10, 40,  -10, 10,  -1};
static sprite_pattern movie_pat00 = {1, {{-12, -12, 0, 445}}};
static sprite_pattern movie_pat01;
static sprite_pattern movie_pat02 = {1, {{-8, -4, 0, 446}}};
static sprite_pattern movie_pat03 = {1, {{-8, -4, 0, 447}}};
static sprite_pattern movie_pat04 = {1, {{-20, -24, 0, 448}}};
static sprite_pattern movie_pat05 = {1, {{-20, -24, 0, 449}}};
sprite_pattern *pat_movie[6] = {&movie_pat00, &movie_pat01, &movie_pat02,
                                &movie_pat03, &movie_pat04, &movie_pat05};
static void (*movie_act_tbl[5])(sprite_status *) = {&m_init, &m_wait, &m_die,
                                                    &m_baku, &m1wait};
static Uint8 pchg00[6] = {0, 2, 1, 3, 1, 255};
static Uint8 pchg01[58] = {0, 4, 4, 1, 4, 4, 1, 4, 4, 1, 4, 4, 1,  4, 4,
                           1, 1, 4, 4, 1, 1, 4, 4, 1, 1, 4, 4, 1,  1, 5,
                           5, 1, 5, 5, 1, 5, 5, 1, 5, 5, 1, 5, 5,  1, 1,
                           5, 5, 1, 1, 5, 5, 1, 1, 5, 5, 1, 1, 255};
static Uint8 *pchg[2] = {pchg00, pchg01};

void movie(sprite_status *pActwk) {
    if (pActwk->userflag.b.h) {
        sprite_status *pMainwk;

        pMainwk = &actwk[((Sint16 *)pActwk)[33]];
        if (pMainwk->actno != 44 || pMainwk->actfree[21]) {
            frameout(pActwk);
            return;
        }

        if (pActwk->r_no0) {
            s_move(pActwk);
        } else {
            s_init(pActwk);
        }
    } else {
        movie_act_tbl[pActwk->r_no0 / 2](pActwk);
        actionsub(pActwk);
        if (pActwk->r_no0 < 3) {
            frameout_s(pActwk);
        }
    }
}

static void m_init(sprite_status *pActwk) {
    sprite_status *pNewact;

    if (projector_flag) {
        frameout(pActwk);
        return;
    }

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprhs = 12;
    pActwk->sprhsize = 12;
    pActwk->sprvsize = 12;
    pActwk->colino = 251;
    pActwk->sproffset = 950;
    pActwk->patbase = pat_movie;
    ((char **)pActwk)[12] = tbl0;

    if (actwkchk(&pNewact) != 0) {
        frameout(pActwk);
        return;
    }

    pNewact->actno = pActwk->actno;
    pNewact->xposi.w.h = pActwk->xposi.w.h;
    pNewact->yposi.w.h = pActwk->yposi.w.h;
    pNewact->xposi.w.h -= 21;
    pNewact->yposi.w.h -= 7;
    pNewact->userflag.b.h = -1;
    ((Sint16 *)pNewact)[33] = pActwk - actwk;

    if (actwkchk(&pNewact) != 0) {
        frameout(pActwk);
        return;
    }

    pNewact->actno = pActwk->actno;
    pNewact->xposi.w.h = pActwk->xposi.w.h;
    pNewact->yposi.w.h = pActwk->yposi.w.h;
    pNewact->xposi.w.h -= 88;
    pNewact->yposi.w.h -= 4;
    pNewact->userflag.b.h = 1;
    ((Sint16 *)pNewact)[33] = pActwk - actwk;

    if (actwkchk(&pNewact) != 0) {
        frameout(pActwk);
        return;
    }

    pNewact->actno = 45;
    pNewact->xposi.w.h = pActwk->xposi.w.h;
    pNewact->yposi.w.h = pActwk->yposi.w.h;
    pNewact->xposi.w.h -= 88;
    pNewact->yposi.w.h -= 24;
    pNewact->userflag.b.h = -128;
    ((Sint16 *)pNewact)[33] = pActwk - actwk;

    if (actwkchk(&pNewact) != 0) {
        frameout(pActwk);
        return;
    }

    pNewact->actno = 45;
    pNewact->xposi.w.h = pActwk->xposi.w.h;
    pNewact->yposi.w.h = pActwk->yposi.w.h;
    pNewact->xposi.w.h -= 100;
    pNewact->yposi.w.h += 4;
    pNewact->userflag.b.h = -127;
    ((Sint16 *)pNewact)[33] = pActwk - actwk;

    m_wait(pActwk);
}

static void m_wait(sprite_status *pActwk) {
    if (pActwk->colicnt) {
        pActwk->colino = 0;
        pActwk->colicnt = 0;
        pActwk->r_no0 += 2;
    }

    hitchk(pActwk, &actwk[0]);
}

static void m_die(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->patno = 1;
    pActwk->actfree[21] = 1;

    if (hitchk(pActwk, &actwk[0])) {
        ;
        ride_on_clr(pActwk, &actwk[0]);
    }

    m_baku(pActwk);
}

static void m_baku(sprite_status *pActwk) {
    char *pTbl, timewk;
    Sint16 xwk, ywk;
    sprite_status *pNewact;

    pTbl = ((char **)pActwk)[12];
    if ((timewk = *pTbl++) >= 0) {
        if (++pActwk->actfree[0] == timewk) {
            xwk = *pTbl++;
            ywk = *pTbl++;
            ((char **)pActwk)[12] = pTbl;
            if (actwkchk(&pNewact) == 0) {
                pNewact->actno = 24;
                pNewact->r_no1 = 1;
                pNewact->xposi.w.h = pActwk->xposi.w.h;
                pNewact->yposi.w.h = pActwk->yposi.w.h;
                pNewact->xposi.w.h += xwk;
                pNewact->yposi.w.h += ywk;
                soundset(158);
            }
        }

    } else {
        pActwk->r_no0 += 2;
        ((Sint16 *)pActwk)[23] = 60;
    }
}

static void m1wait(sprite_status *pActwk) {
    if (!(--((Sint16 *)pActwk)[23])) {
        projector_flag = 1;
        frameout(pActwk);
    }
}

static void s_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 4;
    pActwk->sproffset = 950;
    pActwk->patbase = pat_movie;

    if (pActwk->userflag.b.h & 128) {

        pActwk->sprhs = pActwk->sprhsize = 8;
        pActwk->sprvsize = 4;
        pActwk->mstno.b.h = 0;
    } else {
        pActwk->sprhs = pActwk->sprhsize = 20;
        pActwk->sprvsize = 24;
        pActwk->mstno.b.h = 1;
    }

    s_move(pActwk);
}

static void s_move(sprite_status *pActwk) {
    patchg(pActwk, pchg);
    actionsub(pActwk);
}
