#include "../equ.h"
#include "../action.h"
#include "../actset.h"
#include "../ridechk.h"
#include "baneiwa.h"
#include <stddef.h>

typedef struct {
    Uint8 a;
    char b;
    Sint16 c;
} move_data;

static void baneiwa_init(sprite_status *pActwk);
static void baneiwa_move(sprite_status *pActwk);

#pragma pack(push, 1)
typedef struct {
    Uint8 reserved0[10];
    Sint16 parent_actor;
    Uint8 reserved1[16 - 12];
    Uint8 move_timer;
    Uint8 move_index;
    Uint8 is_child;
    Uint8 reserved2[20 - 19];
    Sint16 acceleration;
} baneiwa_work;
#pragma pack(pop)

_Static_assert(offsetof(baneiwa_work, move_timer) == 16,
               "baneiwa_work.move_timer offset");
_Static_assert(offsetof(baneiwa_work, move_index) == 17,
               "baneiwa_work.move_index offset");
_Static_assert(offsetof(baneiwa_work, is_child) == 18,
               "baneiwa_work.is_child offset");
_Static_assert(offsetof(baneiwa_work, acceleration) == 20,
               "baneiwa_work.acceleration offset");
_Static_assert(offsetof(baneiwa_work, parent_actor) == 10,
               "baneiwa_work.parent_actor offset");
_Static_assert(sizeof(baneiwa_work) <= sizeof(((sprite_status *)0)->actfree),
               "baneiwa_work fits in actfree");

static baneiwa_work *baneiwa_work_get(sprite_status *pActwk) {
    return (baneiwa_work *)pActwk->actfree;
}

static sprite_pattern pat0 = {1, {{-16, -32, 0, 500}}};
sprite_pattern *baneiwapat[1] = {&pat0};

void baneiwa(sprite_status *pActwk) {
    baneiwa_work *work = baneiwa_work_get(pActwk);
    sprite_status *pRideAct;
    void (*tbl[2])(sprite_status *) = {&baneiwa_init, &baneiwa_move};

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    if (work->is_child) {
        pRideAct = &actwk[work->parent_actor];
        if (pRideAct->actno != 72) {
            frameout(pActwk);
        }
    } else {
        frameout_s(pActwk);
    }
}

static void baneiwa_init(sprite_status *pActwk) {
    baneiwa_work *work = baneiwa_work_get(pActwk);
    sprite_status *pNewActwk;
    baneiwa_work *new_work;

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 17514;
    pActwk->sprpri = 3;
    pActwk->patbase = baneiwapat;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 32;

    if (work->is_child == 0) {
        if (actwkchk(&pNewActwk) == 0) {
            new_work = baneiwa_work_get(pNewActwk);
            pNewActwk->actno = 72;
            pNewActwk->xposi.w.h = pActwk->xposi.w.h + 32;
            pNewActwk->yposi.w.h = pActwk->yposi.w.h;
            new_work->is_child = 1;

            new_work->parent_actor = (Uint16)(pActwk - actwk);
        }
    }
}

static void baneiwa_move(sprite_status *pActwk) {
    Sint16 d0;
    Sint16 d1;
    Uint8 index;

    move_data baneiwa_mvtbl[4] = {
        {128, -4, 512}, {128, -4, 0}, {128, 4, -512}, {128, 4, 0}};

    if (pActwk->yspeed.w < 0) {

        ride_on_chk(pActwk, &actwk[0]);
    }
    baneiwa_work *work = baneiwa_work_get(pActwk);

    if (work->move_timer) {
        pActwk->yposi.l += pActwk->yspeed.w << 8;
        pActwk->yspeed.w += work->acceleration;
        --work->move_timer;
        if (work->move_timer)
            goto label1;

        ++work->move_index;
        if (work->move_index != 4)
            goto label1;

        work->move_index = 0;
        goto label1;
    }

    index = work->move_index;
    work->move_timer = baneiwa_mvtbl[index].a;
    d0 = baneiwa_mvtbl[index].b;
    d1 = baneiwa_mvtbl[index].c;
    if (work->is_child) {
        d0 *= -1;
        d1 *= -1;
    }
    work->acceleration = d0;
    pActwk->yspeed.w = d1;
label1:
    if (pActwk->yspeed.w >= 0) {

        ride_on_chk(pActwk, &actwk[0]);
    }
}
