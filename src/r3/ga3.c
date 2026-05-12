#include "../equ.h"
#include "ga3.h"
#include "../action.h"
#include "../actset.h"
#include "../playsub.h"
#include "../suicide.h"

#if defined(R31A) || defined(R31B) || defined(R32A)
#define SPRITE_GA3_BASE 529
#elif defined(R33C) || defined(R33D)
#define SPRITE_GA3_BASE 533
#else
#define SPRITE_GA3_BASE 482
#endif

static void m_init(sprite_status *actionwk);
static void m_wait(sprite_status *actionwk);
static void s_init(sprite_status *actionwk);

static sprite_pattern spat_b3 = {1, {{-16, -10, 0, SPRITE_GA3_BASE}}};
static sprite_pattern spat_b4 = {1, {{-16, -8, 0, SPRITE_GA3_BASE + 1}}};
static sprite_pattern spat_b5 = {1, {{-16, -10, 0, SPRITE_GA3_BASE + 2}}};
static sprite_pattern spat_e3 = {1, {{-16, -12, 0, SPRITE_GA3_BASE + 3}}};
static sprite_pattern spat_e4 = {1, {{-16, -8, 0, SPRITE_GA3_BASE + 4}}};
static sprite_pattern spat_e5 = {1, {{-16, -8, 0, SPRITE_GA3_BASE + 5}}};
static sprite_pattern spat_0 = {1, {{-12, -12, 0, SPRITE_GA3_BASE + 6}}};
static sprite_pattern spat_1 = {1, {{-12, -12, 0, SPRITE_GA3_BASE + 7}}};
static sprite_pattern spat_2 = {1, {{-12, -16, 0, SPRITE_GA3_BASE + 8}}};
sprite_pattern *pat_b_ga[6] = {&spat_0,  &spat_1,  &spat_2,
                               &spat_b3, &spat_b4, &spat_b5};
sprite_pattern *pat_e_ga[6] = {&spat_0,  &spat_1,  &spat_2,
                               &spat_e3, &spat_e4, &spat_e5};
static char pchg0[4] = {29, 0, 1, 1};
static char pchg1[3] = {49, 2, 2};
static char pchg2[6] = {4, 3, 4, 5, 4, -1};
static char *pchg[3] = {pchg0, pchg1, pchg2};
static char ringchg0[6] = {5, 4, 5, 6, 7, -1};
static char *ringchg[1] = {ringchg0};
extern sprite_pattern *ringpat[];

void ga(sprite_status *actionwk) {
    if (actionwk->userflag.b.h - 1 < 0)
        eb_ga0(actionwk);
    else
        eb_ga1(actionwk);
}

void eb_ga0(sprite_status *actionwk) {

    if (enemy_suicide(actionwk) == -1)
        return;

    switch (actionwk->r_no0) {

    case 0:
        m_init(actionwk);
    case 2:
        m_wait(actionwk);
        break;
    case 4:
        m_appear0(actionwk);
        break;
    case 6:
        m_appear1(actionwk);
        break;
    case 8:
        m_move(actionwk);
        break;
    }
    actionsub(actionwk);
    frameout_s(actionwk);
}

static void m_init(sprite_status *actionwk) {
    actionwk->r_no0 += 2;
    actionwk->actflg |= 4;
    actionwk->patno = 0;
    actionwk->sprpri = 1;
    actionwk->sprhsize = 16;
    actionwk->sprvsize = 8;
    actionwk->sproffset = 42017;
    ((Sint16 *)actionwk)[23] = 61;
    if (actionwk->userflag.b.h & 128)
        actionwk->patbase = pat_b_ga;
    else {

        actionwk->patbase = pat_e_ga;
        actionwk->actfree[21] = 255;
    }
}

static void m_wait(sprite_status *actionwk) {
    if (--((Sint16 *)actionwk)[23] == 0) {

        actionwk->r_no0 += 2;
        ((Sint16 *)actionwk)[23] = 61;
    }
}

void m_appear0(sprite_status *actionwk) {
    if (--((Sint16 *)actionwk)[23] == 0) {

        actionwk->colino = 37;
        actionwk->r_no0 += 2;
        ++actionwk->mstno.b.h;
        ((Sint16 *)actionwk)[23] = 51;
    }
    m_disp(actionwk);
}

void m_disp(sprite_status *actionwk) {
    patchg(actionwk, (Uint8 **)pchg);
    actionsub(actionwk);
}

void m_appear1(sprite_status *actionwk) {
    if (--((Sint16 *)actionwk)[23] == 0) {

        actionwk->colino = 38;
        actionwk->r_no0 += 2;
        ++actionwk->mstno.b.h;
        ((Sint32 *)actionwk)[13] = 32768;
        ((Sint16 *)actionwk)[23] = 32;
        ((Sint32 *)actionwk)[12] = -32768;
        if (actionwk->userflag.b.h < 0) {
            ((Sint16 *)actionwk)[23] = 48;
            ((Sint32 *)actionwk)[12] = -16384;
        }
        if (actionwk->xposi.w.h - actwk[0].xposi.w.h < 0) {

            ((Sint32 *)actionwk)[12] = -((Sint32 *)actionwk)[12];

            actionwk->actflg |= 1;
            actionwk->cddat |= 1;
        }
    }
    m_disp(actionwk);
}

void m_move(sprite_status *actionwk) {
    sprite_status *a1;

    actionwk->xposi.l += ((Sint32 *)actionwk)[12];
    actionwk->yposi.l += ((Sint32 *)actionwk)[13];
    if (--((Sint16 *)actionwk)[23] == 0) {

        ((Sint16 *)actionwk)[23] = 65;
        if (actionwk->userflag.b.h < 0) {

            ((Sint16 *)actionwk)[23] = 97;
        }
        ((Sint32 *)actionwk)[13] = -((Sint32 *)actionwk)[13];

        if (actionwk->actfree[21] != 0) {

            if (actwkchk(&a1) == 0) {
                a1->actno = actionwk->actno;
                a1->xposi.l = actionwk->xposi.l;
                a1->yposi.l = actionwk->yposi.l;
                a1->userflag.b.h = 1;
            }
        }
    }
    m_disp(actionwk);
}

void eb_ga1(sprite_status *actionwk) {
    switch (actionwk->r_no0) {

    case 0:
        s_init(actionwk);
    case 2:
        s_fall(actionwk);
        break;
    case 4:
        s_die(actionwk);
        return;
    }
    actionsub(actionwk);
}

static void s_init(sprite_status *actionwk) {
    actionwk->r_no0 += 2;
    actionwk->actflg |= 4;
    actionwk->sprpri = 1;

    actionwk->sprhs = 6;
    actionwk->sprhsize = 6;
    actionwk->sprvsize = 6;
    actionwk->sproffset = 42926;
    actionwk->patbase = ringpat;
    actionwk->sprhsize = 6;
    ((Sint16 *)actionwk)[23] = 180;
}

void s_fall(sprite_status *actionwk) {
    if (--((Sint16 *)actionwk)[23] == 0) {

        s_die(actionwk);
        return;
    }

    actionwk->yposi.l += 65536;
    patchg(actionwk, (Uint8 **)ringchg);
}

void s_die(sprite_status *actionwk) { frameout(actionwk); }
