#include <stddef.h>

#include "../equ.h"
#include "piston6.h"
#include "../action.h"
#include "../actset.h"
#include "../ridechk.h"

#if defined(R61B) || defined(R62B)
#define SPRITE_PISTON6_BASE 464
#elif defined(R63C) || defined(R63D)
#define SPRITE_PISTON6_BASE 447
#else
#define SPRITE_PISTON6_BASE 470
#endif

static sprite_pattern nullpat = {1, {{0, 0, 0, 0}}};
static sprite_pattern piston6pat0 = {1, {{-32, -40, 0, SPRITE_PISTON6_BASE}}};
sprite_pattern *piston6pat[2] = {&piston6pat0, &nullpat};

#pragma pack(push, 1)
typedef struct {
    Uint8 unused0[8];
    Sint16 base_y;
    Uint16 parent_index;
    Sint16 base_x;
    Uint8 unused14[2];
    Uint8 wait_timer;
    Uint8 extension;
    Uint8 retracting;
} piston6_work;
#pragma pack(pop)

_Static_assert(offsetof(piston6_work, base_y) == 8,
               "piston6_work.base_y offset");
_Static_assert(offsetof(piston6_work, parent_index) == 10,
               "piston6_work.parent_index offset");
_Static_assert(offsetof(piston6_work, base_x) == 12,
               "piston6_work.base_x offset");
_Static_assert(offsetof(piston6_work, wait_timer) == 16,
               "piston6_work.wait_timer offset");
_Static_assert(offsetof(piston6_work, extension) == 17,
               "piston6_work.extension offset");
_Static_assert(offsetof(piston6_work, retracting) == 18,
               "piston6_work.retracting offset");
_Static_assert(sizeof(piston6_work) <= sizeof(((sprite_status *)0)->actfree),
               "piston6_work fits in actfree");

static piston6_work *piston6_get_work(sprite_status *actionwk) {
    return (piston6_work *)actionwk->actfree;
}

void piston6(sprite_status *actionwk) {
    if (actionwk->userflag.b.h < 0) {
        piston6_side(actionwk);
        return;
    }
    switch (actionwk->r_no0) {
    case 0:
        piston6_init(actionwk);
    case 2:
        piston6_move(actionwk);
        break;
    }
    actionsub(actionwk);
    frameout_s(actionwk);
}

void piston6_ridechk(sprite_status *actionwk) { ridechk(actionwk, &actwk[0]); }

void piston6_init(sprite_status *actionwk) {
    piston6_work *work = piston6_get_work(actionwk);
    sprite_status *a1;

    actionwk->r_no0 += 2;
    actionwk->actflg |= 4;

    actionwk->sproffset = 832;
    actionwk->patbase = piston6pat;
    actionwk->sprvsize = 40;
    actionwk->sprhsize = 32;
    work->base_x = actionwk->xposi.w.h;
    work->base_y = actionwk->yposi.w.h;
    if (actwkchk(&a1) == 0)
        pis6_setdata(actionwk, a1, 32);
    if (actwkchk(&a1) == 0)
        pis6_setdata(actionwk, a1, -32);
}

void piston6_move(sprite_status *actionwk) {
    piston6_work *work = piston6_get_work(actionwk);
    Sint16 d0;

    piston6_sub(actionwk);
    d0 = work->extension;
    d0 = -d0;
    d0 += work->base_y;
    actionwk->yposi.w.h = d0;

    if (work->retracting != 0) {
        if (work->wait_timer == 0)
            goto label1;
    }

    if (work->extension >= 33) {
        ride_on_clr(actionwk, &actwk[0]);
        return;
    }
label1:
    piston6_ridechk(actionwk);
}

void piston6_sub(sprite_status *actionwk) {
    piston6_work *work = piston6_get_work(actionwk);

    if (work->wait_timer != 0) {
        if (--work->wait_timer != 0)
            return;
    }

    if (work->retracting != 0) {
        --work->extension;
        if ((Sint8)work->extension > 0)
            return;
        work->extension = work->retracting = 0;

        work->wait_timer = 60;
    } else {

        work->extension += 8;
        if (work->extension < 80)
            return;
        work->extension = 80;
        work->retracting = 1;
        work->wait_timer = 60;
    }
}

void pis6_setdata(sprite_status *actionwk, sprite_status *a1, Sint16 d0) {
    a1->actno = 32;
    piston6_get_work(a1)->parent_index = actionwk - actwk;
    a1->userflag.b.h = -1;
    a1->xposi.w.h = actionwk->xposi.w.h + d0;
    a1->yposi.w.h = actionwk->yposi.w.h;
    a1->actflg |= 4;
    a1->patbase = piston6pat;
    a1->sprvsize = 40;

    a1->patno = 1;
}

void piston6_side(sprite_status *actionwk) {
    piston6_work *work = piston6_get_work(actionwk);
    sprite_status *a1;

    a1 = &actwk[work->parent_index];
    if (a1->actno != 32) {
        frameout(actionwk);
        return;
    }
    actionwk->yposi.w.h = a1->yposi.w.h;

    hitchk(actionwk, &actwk[0]);
    actionsub(actionwk);
}
