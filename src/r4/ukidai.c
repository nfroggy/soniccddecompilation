#include "../equ.h"
#include "ukidai.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../ridechk.h"
#include <stddef.h>

static void a_init(sprite_status *pActwk);
static void a_moving(sprite_status *pActwk);
static void a_wait(sprite_status *pActwk);
static void a_move(sprite_status *pActwk);
static void a_move1(sprite_status *pActwk);

#pragma pack(push, 1)
typedef struct {
    Sint32 collision_side;
    Sint16 step_direction;
    Sint16 probe_x_offset;
    Sint16 remaining_delta;
    Sint16 probe_y;
    Sint16 origin_x;
} ukidai_work;
#pragma pack(pop)

_Static_assert(offsetof(ukidai_work, collision_side) == 0,
               "ukidai_work.collision_side offset");
_Static_assert(offsetof(ukidai_work, step_direction) == 4,
               "ukidai_work.step_direction offset");
_Static_assert(offsetof(ukidai_work, probe_x_offset) == 6,
               "ukidai_work.probe_x_offset offset");
_Static_assert(offsetof(ukidai_work, remaining_delta) == 8,
               "ukidai_work.remaining_delta offset");
_Static_assert(offsetof(ukidai_work, probe_y) == 10,
               "ukidai_work.probe_y offset");
_Static_assert(offsetof(ukidai_work, origin_x) == 12,
               "ukidai_work.origin_x offset");
_Static_assert(sizeof(ukidai_work) <= sizeof(((sprite_status *)0)->actfree),
               "ukidai_work fits in actfree");

static ukidai_work *ukidai_work_get(sprite_status *pActwk) {
    return (ukidai_work *)pActwk->actfree;
}

static sprite_pattern pat00 = {1, {{-16, -8, 0, 530}}};
sprite_pattern *pat_ukidai[1] = {&pat00};

void ukidai(sprite_status *pActwk) {
    ukidai_work *work = ukidai_work_get(pActwk);
    void (*tbl[5])(sprite_status *) = {&a_init, &a_moving, &a_wait, &a_move,
                                       &a_move1};

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s00(pActwk, work->origin_x);
}

static void a_init(sprite_status *pActwk) {
    ukidai_work *work = ukidai_work_get(pActwk);

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->sprhs = 16;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 8;
    pActwk->sproffset = 848;
    pActwk->patbase = pat_ukidai;
    work->origin_x = pActwk->xposi.w.h;

    if (watermoveposi != 1920) {
        if (watermoveposi != 1504) {
            frameout(pActwk);
            return;
        }
    }
    a_moving(pActwk);
}

static void a_moving(sprite_status *pActwk) {
    Sint16 d0;

    pActwk->yposi.w.h = waterposi;

    d0 = waterposi_m;
    if (watermoveposi == d0) {
        if (d0 == 1920) {
            pActwk->r_no0 = 4;
            a_wait(pActwk);
            return;
        }
        if (d0 == 1504) {
            pActwk->r_no0 = 6;
            a_move(pActwk);
            return;
        }
        frameout(pActwk);
    }
}

static void a_wait(sprite_status *pActwk) {
    pActwk->yposi.w.h = waterposi;

    if (ridechk(pActwk, &actwk[0])) {
        pActwk->r_no0 += 2;
        pActwk->xposi.w.h = actwk[0].xposi.w.h;
        watermoveposi = 1504;
    }
}

static void a_move(sprite_status *pActwk) {
    ushort_union d0;

    pActwk->r_no0 += 2;
    d0.b.h = waterspeed;
    d0.b.l = water_flag;
    pActwk->yspeed.w = -(Sint16)d0.w;

    a_move1(pActwk);
}

static void a_move1(sprite_status *pActwk) {
    ukidai_work *work = ukidai_work_get(pActwk);
    sprite_status *pPlayerwk;
    Sint32 a6;
    Sint16 d1;
    Sint16 d2;
    Sint16 d3;
    Sint16 d4, d5, d6;

    pActwk->yposi.w.h = waterposi;

    pPlayerwk = &actwk[0];
    d4 = pPlayerwk->xspeed.b.h;
    d4 += pPlayerwk->xposi.w.h;
    d4 -= pActwk->xposi.w.h;
    if (pActwk->xposi.w.h) {
        a6 = 2;
        d6 = 1;
        d5 = 0;
        d5 = (Uint16)pActwk->sprhs;
        if (d4 < 0) {
            a6 = 1;
            d6 *= -1;
            d5 *= -1;
            d4 *= -1;
        }
        work->collision_side = a6;
        work->step_direction = d6;
        work->probe_x_offset = d5;
        work->remaining_delta = d4;
        work->probe_y = pActwk->yposi.w.h;
        work->probe_y += 7;
        do {
            a6 = work->collision_side;
            d3 = pActwk->xposi.w.h;
            d3 += work->probe_x_offset;
            if (a6 != 2) {
                d3 ^= 15;
            }
            d2 = work->probe_y;

            if (a6 == 1)
                d1 = emycol_l3(pActwk, d3, d2);
            else
                d1 = emycol_r3(pActwk, d3, d2);
            if (d1 < 0)
                break;
            if (work->remaining_delta < 2) {
                --work->remaining_delta;
                break;
            }
            --work->remaining_delta;
            pActwk->xposi.w.h += work->step_direction;
        } while (1);
    }
    if (ridechk(pActwk, pPlayerwk)) {
        pPlayerwk->xposi.w.h = pActwk->xposi.w.h;
    }
}
