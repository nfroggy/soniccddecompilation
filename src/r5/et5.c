#include "../equ.h"
#include "et5.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../playsub.h"
#include "../ridechk.h"
#include "../score.h"

static void m_init(sprite_status *pActwk);
static void m_wait(sprite_status *pActwk);
static void m_die(sprite_status *pActwk);
static void m1wait(sprite_status *pActwk);
static void a_hover(sprite_status *pActwk);

static sprite_pattern et_pat00 = {1, {{-40, 16, 0, 422}}};
static sprite_pattern et_pat01 = {1, {{-40, 16, 0, 423}}};
static sprite_pattern et_pat02 = {1, {{-40, -32, 0, 424}}};
static sprite_pattern et_pat03 = {1, {{-40, -32, 0, 425}}};
static sprite_pattern et_pat04 = {1, {{-40, -32, 0, 426}}};
static sprite_pattern et_pat05 = {1, {{-40, -32, 0, 427}}};
static sprite_pattern et_pat06 = {1, {{-40, -32, 0, 428}}};
static sprite_pattern et_pat07;
sprite_pattern *pat_et[8] = {&et_pat00, &et_pat01, &et_pat02, &et_pat03,
                             &et_pat04, &et_pat05, &et_pat06, &et_pat07};
static char p_a[6] = {3, 3, 4, 5, 6, -1};
static char *pchg[1] = {p_a};
static char tbl0[64] = {
    1,   0,  0,   2,   -40, -20, 3,   28, 10, 4,   18,  -18, 5,   -18, -10, 6,
    8,   -8, 8,   -18, 14,  10,  -10, 10, 12, 30,  -10, 15,  0,   -18, 18,  20,
    -10, 20, -10, 18,  22,  8,   23,  25, 13, -10, 26,  23,  -22, 28,  -3,  -25,
    30,  10, 20,  32,  -10, 2,   34,  30, -8, 35,  13,  -10, 40,  -10, 10,  -1};

void et(sprite_status *pActwk) {
    void (*et_jmp_tbl[4])(sprite_status *) = {&m_init, &m_wait, &m_die,
                                              &m1wait};

    et_jmp_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);

    if (pActwk->r_no0 < 3)
        frameout_s(pActwk);
}

static void m_init(sprite_status *pActwk) {
    Uint8 patnowk;
    Uint16 tbl0sproffset[3][4] = {
        {1234, 1234, 0, 0}, {1234, 1234, 0, 0}, {0, 0, 0, 0}};

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 4;
    pActwk->sprhs = pActwk->sprhsize = 34;
    pActwk->sprvsize = 32;

    pActwk->sproffset = tbl0sproffset[stageno.b.l][time_flag];

    pActwk->patbase = pat_et;
    ((char **)pActwk)[12] = tbl0;
    ((Sint16 *)pActwk)[26] = pActwk->yposi.w.h;
    ((Sint16 *)pActwk)[23] = 4;
    ((Sint16 *)pActwk)[27] = 1;

    patnowk = 0;

    if (!generate_flag) {
        patnowk += 2;
    }

    if (!time_flag) {
        ++patnowk;
    }

    pActwk->patno = patnowk;

    if (!generate_flag && time_flag == 0) {
        pActwk->colino = 250;
        pActwk->yposi.w.h -= 16;
    }

    m_wait(pActwk);
}

static void m_wait(sprite_status *pActwk) {
    if (!generate_flag && time_flag == 0) {
        a_hover(pActwk);
        if (pActwk->colicnt) {
            pActwk->colino = pActwk->colicnt = 0;
            ((Sint16 *)pActwk)[23] = 0;
            pActwk->patno = 7;
            pActwk->r_no0 += 2;
            generate_flag = 1;
            scoreup(150);
            if (hitchk(pActwk, &actwk[0])) {
                ride_on_clr(pActwk, &actwk[0]);
            }

        } else {
            hitchk(pActwk, &actwk[0]);
            patchg(pActwk, (Uint8 **)pchg);
        }
    }
}

static void m_die(sprite_status *pActwk) {
    char *pTbl, timewk;
    Sint16 xwk, ywk;
    sprite_status *pNewact;

    pTbl = ((char **)pActwk)[12];
    timewk = *pTbl;
    ++pTbl;

    if (timewk < 0) {

        pActwk->r_no0 += 2;
        pActwk->actfree[0] = 8;
    } else {
        if (++pActwk->actfree[0] == timewk) {
            xwk = *pTbl;
            ++pTbl;
            ywk = *pTbl;
            ++pTbl;
            ((char **)pActwk)[12] = pTbl;

            if (actwkchk(&pNewact) == 0) {
                pNewact->actno = 24;
                pNewact->r_no1 = 1;
                pNewact->xposi.w.h = pActwk->xposi.w.h + xwk;
                pNewact->yposi.w.h = pActwk->yposi.w.h + ywk;
                soundset(158);
            }
        }
    }
}

static void m1wait(sprite_status *pActwk) {
    if (--pActwk->actfree[0] == 0) {
        pActwk->r_no0 -= 6;
        pActwk->yposi.w.h = ((Sint16 *)pActwk)[26];
        soundset(217);
    }
}

static void a_hover(sprite_status *pActwk) {
    Sint16 timewk;

    ++((Sint16 *)pActwk)[23];

    timewk = ((Sint16 *)pActwk)[23];
    if (!(timewk & 7))
        pActwk->yposi.w.h += ((Sint16 *)pActwk)[27];

    timewk = ((Sint16 *)pActwk)[23];
    if (!(timewk & 31))
        ((Sint16 *)pActwk)[27] *= -1;
}
