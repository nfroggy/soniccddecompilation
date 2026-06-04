#include "../equ.h"
#include "tekkyu7j.h"
#include "../action.h"
#include "../actset.h"

typedef struct {
    Sint16 launcher_timer;
    Uint8 axis_is_vertical;
    Sint16 origin_position;
    Sint16 phase_timer;
    Sint32 speed;
    Sint32 acceleration;
    Uint8 initial_side;
} tekkyu7j_work;

static tekkyu7j_work *tekkyu7j_get_work(sprite_status *pActwk) {
    return (tekkyu7j_work *)pActwk->actfree;
}

#if defined(R73C) || defined(R73D)
#define SPRITE_TEKKYU7J_BASE 406
#else
#define SPRITE_TEKKYU7J_BASE 423
#endif

static void tekkyu7(sprite_status *pActwk);
static void tekkyu7_init(sprite_status *pActwk);
static void tekkyu7_move(sprite_status *pActwk);
static void tekkyu7_stop(sprite_status *pActwk);
static void tekkyu7_jump(sprite_status *pActwk);

static sprite_pattern tekkyu7j_pat0 = {1,
                                       {{-16, -16, 0, SPRITE_TEKKYU7J_BASE}}};
static sprite_pattern tekkyu7j_pat1 = {1,
                                       {{-16, 0, 0, SPRITE_TEKKYU7J_BASE + 1}}};
static sprite_pattern tekkyu7j_pat2 = {
    1, {{-16, -16, 0, SPRITE_TEKKYU7J_BASE + 2}}};
static sprite_pattern tekkyu7j_pat3 = {1,
                                       {{0, -16, 0, SPRITE_TEKKYU7J_BASE + 3}}};
sprite_pattern *pat_tekkyu7j[4] = {&tekkyu7j_pat0, &tekkyu7j_pat1,
                                   &tekkyu7j_pat2, &tekkyu7j_pat3};
extern sprite_pattern *pat_tekkyu7[];

void tekkyu7j(sprite_status *pActwk) {
    tekkyu7j_work *work = tekkyu7j_get_work(pActwk);
    sprite_status *pNewactwk;

    if (pActwk->userflag.b.l) {
        tekkyu7(pActwk);
    } else {

        if (!pActwk->r_no0) {

            pActwk->r_no0 += 2;
            pActwk->actflg |= 4;
            pActwk->sprpri = 1;
            pActwk->sproffset = 910;
            pActwk->patbase = pat_tekkyu7j;
            pActwk->patno = pActwk->userflag.b.h;
            work->launcher_timer = 150;
            pActwk->sprhs = 16;
            pActwk->sprhsize = 16;
            pActwk->sprvsize = 16;
        }

        if (!(--work->launcher_timer)) {
            work->launcher_timer = 150;
            if (actwkchk(&pNewactwk) == 0) {
                pNewactwk->actno = pActwk->actno;
                pNewactwk->userflag.b.h = pActwk->userflag.b.h;
                pNewactwk->userflag.b.l = -1;
                pNewactwk->xposi.w.h = pActwk->xposi.w.h;
                pNewactwk->yposi.w.h = pActwk->yposi.w.h;
            }
        }
        actionsub(pActwk);
        frameout_s(pActwk);
    }
}

static void tekkyu7(sprite_status *pActwk) {
    void (*tekkyu7_acttbl[4])(sprite_status *) = {&tekkyu7_init, &tekkyu7_move,
                                                  &tekkyu7_stop, &tekkyu7_jump};

    tekkyu7_acttbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
}

void tekkyu7_init(sprite_status *pActwk) {
    tekkyu7j_work *work = tekkyu7j_get_work(pActwk);
    Sint16 wD0;

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 4;
    pActwk->colino = 181;
    pActwk->sprhs = 16;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 16;
    pActwk->sproffset = 902;
    pActwk->patbase = pat_tekkyu7;
    work->phase_timer = 32;

    work->speed = 32768;
    work->initial_side = 0;
    if (!(pActwk->userflag.b.h & 1)) {
        work->initial_side = 255;
        work->speed *= -1;
    }
    work->axis_is_vertical = 0;
    wD0 = pActwk->xposi.w.h;
    if (pActwk->userflag.b.h < 2) {
        work->axis_is_vertical = 1;
        wD0 = pActwk->yposi.w.h;
    }
    work->origin_position = wD0;

    tekkyu7_move(pActwk);
}

void tekkyu7_move(sprite_status *pActwk) {
    tekkyu7j_work *work = tekkyu7j_get_work(pActwk);

    if (work->axis_is_vertical)
        pActwk->yposi.l += work->speed;
    else
        pActwk->xposi.l += work->speed;
    if (!(--work->phase_timer)) {
        pActwk->r_no0 += 2;
        work->phase_timer = 30;
    }
}

void tekkyu7_stop(sprite_status *pActwk) {
    tekkyu7j_work *work = tekkyu7j_get_work(pActwk);

    if (!(--work->phase_timer)) {
        pActwk->r_no0 += 2;
        work->speed = 393216;
        work->acceleration = -16384;
        if (!(pActwk->userflag.b.h & 1)) {
            work->speed *= -1;
            work->acceleration *= -1;
        }
    }
}

void tekkyu7_jump(sprite_status *pActwk) {
    tekkyu7j_work *work = tekkyu7j_get_work(pActwk);
    Sint16 wD0;
    Uint8 bD0;

    work->speed += work->acceleration;
    if (work->axis_is_vertical) {
        pActwk->yposi.l += work->speed;
        wD0 = pActwk->yposi.w.h;
    } else {
        pActwk->xposi.l += work->speed;
        wD0 = pActwk->xposi.w.h;
    }
    bD0 = 0;
    if (wD0 < work->origin_position)
        bD0 = 255;
    if (work->initial_side != bD0)
        frameout(pActwk);
}
