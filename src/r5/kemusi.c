#include <stddef.h>

#include "../equ.h"
#include "kemusi.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../suicide.h"

#pragma pack(push, 1)
typedef struct {
    Sint16 step_delta;
    Sint16 move_duration;
    Sint16 move_timer;
    Sint16 step_reset;
    Sint16 step_counter;
    Sint16 origin_x;
    Sint16 landed_count;
    Uint8 unused14[2];
    Sint16 link_indices[3];
} kemusi_work;
#pragma pack(pop)

_Static_assert(offsetof(kemusi_work, step_delta) == 0,
               "kemusi_work.step_delta offset");
_Static_assert(offsetof(kemusi_work, move_duration) == 2,
               "kemusi_work.move_duration offset");
_Static_assert(offsetof(kemusi_work, move_timer) == 4,
               "kemusi_work.move_timer offset");
_Static_assert(offsetof(kemusi_work, step_reset) == 6,
               "kemusi_work.step_reset offset");
_Static_assert(offsetof(kemusi_work, step_counter) == 8,
               "kemusi_work.step_counter offset");
_Static_assert(offsetof(kemusi_work, origin_x) == 10,
               "kemusi_work.origin_x offset");
_Static_assert(offsetof(kemusi_work, landed_count) == 12,
               "kemusi_work.landed_count offset");
_Static_assert(offsetof(kemusi_work, link_indices) == 16,
               "kemusi_work.link_indices offset");
_Static_assert(sizeof(kemusi_work) <= sizeof(((sprite_status *)0)->actfree),
               "kemusi_work fits in actfree");

static kemusi_work *kemusi_get_work(sprite_status *pActwk) {
    return (kemusi_work *)pActwk->actfree;
}

static void kemusi_init(sprite_status *pActwk);
static void kemusi_com(sprite_status *pActwk, sprite_status *pNewactwk);
static void kemusi_fall(sprite_status *pActwk);
static void kemusi_stop(sprite_status *pActwk);
static void kemusi_move(sprite_status *pActwk);
static void kemusi_move1(sprite_status *pActwk);
static void kemusi_move1_rev(sprite_status *pActwk);
static void kemusi_move1_move(sprite_status *pActwk);

static sprite_pattern kemusi_pat0 = {1, {{-8, -12, 0, 431}}};
static sprite_pattern kemusi_pat1 = {1, {{-8, -12, 0, 432}}};
static sprite_pattern kemusi_pat2 = {1, {{-8, -12, 0, 433}}};
static sprite_pattern kemusi_pat3 = {1, {{-8, -8, 0, 434}}};
sprite_pattern *pat_kemusi[4] = {&kemusi_pat0, &kemusi_pat1, &kemusi_pat2,
                                 &kemusi_pat3};

void kemusi(sprite_status *pActwk) {
    kemusi_work *work = kemusi_get_work(pActwk);
    sprite_status *pActwk1, *pActwk2, *pActwk3;

    void (*kemusi_move_tbl[5])(sprite_status *) = {
        &kemusi_init, &kemusi_fall, &kemusi_stop, &kemusi_move, &kemusi_move1};

    if (!pActwk->userflag.b.l) {
        if (enemy_suicide(pActwk))
            return;
    }

    if (pActwk->r_no0) {
        pActwk1 = &actwk[work->link_indices[0]];
        pActwk2 = &actwk[work->link_indices[1]];
        pActwk3 = &actwk[work->link_indices[2]];
        if (pActwk1->actno != 34 || pActwk2->actno != 34 ||
            pActwk3->actno != 34) {
            frameout(pActwk);
            return;
        }
    }

    kemusi_move_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    if (!pActwk->userflag.b.l) {
        frameout_s00(pActwk, work->origin_x);
    }
}

static void kemusi_init(sprite_status *pActwk) {
    kemusi_work *work = kemusi_get_work(pActwk);
    sprite_status *pNewactwk;
    sprite_status *pActwk1, *pActwk2, *pActwk3;
    Uint8 bD2, bD3, bD4;
    Sint16 wD0, wD1, wD5, wD6;
    Sint16 i;

    work->step_delta = 3;
    pActwk->patno = 1;
    pActwk->sprvsize = 12;
    pActwk->colino = 51;

    pNewactwk = pActwk;
    kemusi_com(pActwk, pNewactwk);
    if (!pActwk->userflag.b.h) {
        wD0 = 36;
        wD1 = 6;
    } else {

        wD0 = 36;
        wD1 = 12;
    }

    work->move_duration = wD0;
    work->step_reset = wD1;

    i = 16;
    wD6 = 2;
    wD5 = pActwk->xposi.w.h;
    if (!pActwk->userflag.b.h) {
        bD4 = 2;
        bD3 = 11;
        bD2 = 180;
    } else {

        bD4 = 3;
        bD3 = 7;
        bD2 = 51;
    }

    for (; wD6 >= 0; --wD6, i += 2) {
        if (actwkchk2(pActwk, &pNewactwk) != 0) {
            frameout(pActwk);
            return;
        }
        work->link_indices[(i - 16) / 2] = pNewactwk - actwk;
        kemusi_get_work(pNewactwk)->step_delta = wD6;
        wD5 += 12;
        pNewactwk->xposi.w.h = wD5;
        pNewactwk->patno = bD4;
        pNewactwk->sprvsize = bD3;
        pNewactwk->colino = bD2;
        pNewactwk->actno = pActwk->actno;
        pNewactwk->userflag.b.h = pActwk->userflag.b.h;
        pNewactwk->yposi.w.h = pActwk->yposi.w.h;
        kemusi_get_work(pNewactwk)->move_duration = work->move_duration;
        kemusi_get_work(pNewactwk)->step_reset = work->step_reset;
        pNewactwk->userflag.b.l = -1;
        kemusi_com(pActwk, pNewactwk);
    }
    pActwk1 = &actwk[work->link_indices[0]];
    pActwk2 = &actwk[work->link_indices[1]];
    pActwk3 = &actwk[work->link_indices[2]];

    kemusi_get_work(pActwk1)->link_indices[0] = pActwk - actwk;
    kemusi_get_work(pActwk1)->link_indices[1] = pActwk2 - actwk;
    kemusi_get_work(pActwk1)->link_indices[2] = pActwk3 - actwk;

    kemusi_get_work(pActwk2)->link_indices[0] = pActwk - actwk;
    kemusi_get_work(pActwk2)->link_indices[1] = pActwk1 - actwk;
    kemusi_get_work(pActwk2)->link_indices[2] = pActwk3 - actwk;

    kemusi_get_work(pActwk3)->link_indices[0] = pActwk - actwk;
    kemusi_get_work(pActwk3)->link_indices[1] = pActwk1 - actwk;
    kemusi_get_work(pActwk3)->link_indices[2] = pActwk2 - actwk;
}

static void kemusi_com(sprite_status *pActwk, sprite_status *pNewactwk) {
    pNewactwk->r_no0 = 2;
    pNewactwk->actflg |= 4;
    pNewactwk->sprpri = 3;
    pNewactwk->sprhs = 8;
    pNewactwk->sprhsize = 8;
    pNewactwk->sproffset = 9214;
    pNewactwk->patbase = pat_kemusi;
    kemusi_get_work(pNewactwk)->origin_x = pActwk->xposi.w.h;
}

static void kemusi_fall(sprite_status *pActwk) {
    Sint16 wD1;
    sprite_status *pSubactwk;

    ++pActwk->yposi.w.h;
    if ((wD1 = emycol_d(pActwk)) < 0) {
        pActwk->yposi.w.h += wD1;
        pActwk->r_no0 += 2;
        pSubactwk = pActwk;
        if (pActwk->userflag.b.l) {
            pSubactwk = &actwk[kemusi_get_work(pActwk)->link_indices[0]];
        }
        ++kemusi_get_work(pSubactwk)->landed_count;
    }
}

static void kemusi_stop(sprite_status *pActwk) {
    kemusi_work *work = kemusi_get_work(pActwk);

    if (!pActwk->userflag.b.l) {
        if (work->landed_count == 4) {
            pActwk->r_no0 += 2;
            actwk[work->link_indices[0]].r_no0 += 2;
            actwk[work->link_indices[1]].r_no0 += 2;
            actwk[work->link_indices[2]].r_no0 += 2;
            kemusi_move(pActwk);
        }
    }
}

static void kemusi_move(sprite_status *pActwk) {
    kemusi_work *work = kemusi_get_work(pActwk);

    pActwk->r_no0 += 2;
    work->step_delta ^= 3;
    work->move_timer = work->move_duration;
    work->step_counter = work->step_reset;
    if (!pActwk->userflag.b.l) {
        pActwk->patno ^= 1;
    }
    kemusi_move1(pActwk);
}

static void kemusi_move1(sprite_status *pActwk) {
    kemusi_work *work = kemusi_get_work(pActwk);
    Sint16 wD0;

    if (!(work->step_counter -= work->step_delta)) {
        work->step_counter = work->step_reset;
        kemusi_move1_move(pActwk);
        wD0 = work->origin_x - pActwk->xposi.w.h;
        if (wD0 < 0)
            wD0 *= -1;
        if (wD0 >= 80) {
            kemusi_move1_rev(pActwk);
            return;
        }
        wD0 = emycol_d(pActwk);

        if (wD0 + 7 >= 14) {
            kemusi_move1_rev(pActwk);
            return;
        }
        pActwk->yposi.w.h += wD0;
    }

    if (!(--work->move_timer)) {
        pActwk->r_no0 -= 2;
    }
}

static void kemusi_move1_rev(sprite_status *pActwk) {
    pActwk->actflg ^= 1;
    pActwk->cddat ^= 1;
    kemusi_move1_move(pActwk);
    kemusi_move1(pActwk);
}

static void kemusi_move1_move(sprite_status *pActwk) {
    if (!(pActwk->actflg & 1))
        --pActwk->xposi.w.h;
    else
        ++pActwk->xposi.w.h;
}
