#include <stddef.h>

#include "../equ.h"
#include "dai8.h"
#include "../action.h"
#include "../actset.h"
#include "../playsub.h"
#include "../ridechk.h"

#if defined(R81)
#define SPRITE_DAI8_BASE 475
#elif defined(R82)
#define SPRITE_DAI8_BASE 437
#else
#define SPRITE_DAI8_BASE 433
#endif

static void act_init(sprite_status *actionwk);

#pragma pack(push, 1)
typedef struct {
    Sint16 timer;
    Sint16 parent_index;
    Sint16 origin_x;
    Sint16 origin_y;
    Uint8 unused8[13];
    Uint8 ride_pressed;
} dai8_work;
#pragma pack(pop)

_Static_assert(offsetof(dai8_work, timer) == 0,
               "dai8_work.timer offset");
_Static_assert(offsetof(dai8_work, parent_index) == 2,
               "dai8_work.parent_index offset");
_Static_assert(offsetof(dai8_work, origin_x) == 4,
               "dai8_work.origin_x offset");
_Static_assert(offsetof(dai8_work, origin_y) == 6,
               "dai8_work.origin_y offset");
_Static_assert(offsetof(dai8_work, ride_pressed) == 21,
               "dai8_work.ride_pressed offset");
_Static_assert(sizeof(dai8_work) <= sizeof(((sprite_status *)0)->actfree),
               "dai8_work fits in actfree");

static dai8_work *dai8_get_work(sprite_status *actionwk) {
    return (dai8_work *)actionwk->actfree;
}

static char p00[47] = {0, 5, 5, 5, 5, 5, 4, 4, 4, 5, 5, 5, 5, 5, 4, 4,
                       4, 3, 3, 3, 3, 3, 2, 2, 2, 3, 3, 3, 3, 3, 2, 2,
                       2, 1, 1, 1, 1, 1, 7, 7, 7, 1, 1, 1, 1, 1, 0};
static char p01[152] = {
    0, 1, 1, 1, 1, 1, 7, 7, 7, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 3,
    2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 5, 5, 5, 5, 5, 4, 4, 4, 5, 5, 5,
    5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5,
    5, 4, 4, 4, 5, 5, 5, 5, 5, 4, 4, 4, 3, 3, 3, 3, 3, 2, 2, 2, 3, 3,
    3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 7, 7, 7, 1, 1, 1, 1, 1, 0};
static char *pchg[2] = {p00, p01};
static sprite_pattern pat00 = {1, {{-16, -16, 0, SPRITE_DAI8_BASE}}};
static sprite_pattern pat01 = {1, {{-16, -16, 0, SPRITE_DAI8_BASE + 1}}};
static sprite_pattern pat02 = {1, {{-8, -16, 0, SPRITE_DAI8_BASE + 2}}};
static sprite_pattern pat03 = {1, {{-8, -16, 0, SPRITE_DAI8_BASE + 3}}};
static sprite_pattern pat04 = {1, {{-4, -16, 0, SPRITE_DAI8_BASE + 4}}};
static sprite_pattern pat05 = {1, {{-4, -16, 0, SPRITE_DAI8_BASE + 5}}};
static sprite_pattern patnull = {1, {{0, 0, 0, 0}}};
sprite_pattern *pat_dai8[8] = {&pat00, &pat01, &pat02,   &pat03,
                               &pat04, &pat05, &patnull, &pat00};

void dai8(sprite_status *actionwk) {
    dai8_work *work = dai8_get_work(actionwk);
    Sint16 ano;

    if (actionwk->userflag.b.h < 0) {
        ano = work->parent_index;
        dai8_work *parent_work = dai8_get_work(&actwk[ano]);
        if (actwk[ano].actno != 44) {
            frameout(actionwk);
            return;
        }
        if (work->origin_x != parent_work->origin_x) {

            frameout(actionwk);
            return;
        }
        if (work->origin_y != parent_work->origin_y) {

            frameout(actionwk);
            return;
        }
    }

    switch (actionwk->r_no0) {
    case 0:
        act_init(actionwk);
        break;
    case 2:
        act_wait(actionwk);
        break;
    case 4:
        act_appear(actionwk);
        break;
    case 6:
        act_on(actionwk);
        break;
    case 8:
        act_off(actionwk);
        break;
    }
    actionsub(actionwk);
    if (actionwk->userflag.b.h >= 0) {
        frameout_s00(actionwk, work->origin_x);
    }
}

static void act_init(sprite_status *actionwk) {
    Sint16 tbl00[2] = {60, 0};
    Sint16 tbl01[5] = {60, 1, 60, 32, 0};
    Sint16 tbl02[8] = {90, 2, 180, 32, 16, 270, 64, 32};
    sprite_status *a1;
    Sint16 *a5tbl, i, d6;

    switch (actionwk->userflag.b.h) {
    case 0:
        a5tbl = tbl00;
        break;
    case 1:
        a5tbl = tbl01;
        break;
    case 2:
        a5tbl = tbl02;
        break;
    }
    dai8_get_work(actionwk)->timer = *a5tbl++;
    act_init_sub(actionwk, actionwk);

    d6 = *a5tbl++;
    for (i = 0; i < d6; ++i) {
        if (actwkchk(&a1) != 0) {
            frameout(actionwk);
            break;
        }
        a1->actno = actionwk->actno;
        dai8_get_work(a1)->parent_index = (Uint16)(actionwk - actwk);
        a1->userflag.b.h = -1;
        dai8_get_work(a1)->timer = *a5tbl++;
        a1->xposi.w.h = actionwk->xposi.w.h + *a5tbl++;
        a1->yposi.w.h = actionwk->yposi.w.h + *a5tbl++;
        act_init_sub(actionwk, a1);
    }
}

void act_init_sub(sprite_status *actionwk, sprite_status *a6) {
    a6->actflg |= 4;
    a6->sprpri = 3;
    a6->sprhs = 16;
    a6->sprhsize = 16;
    a6->sprvsize = 16;
    a6->sproffset = 920;
    a6->patbase = pat_dai8;
    a6->patno = 6;
    a6->r_no0 = 2;
    dai8_get_work(a6)->origin_x = actionwk->xposi.w.h;
    dai8_get_work(a6)->origin_y = actionwk->yposi.w.h;
}

void act_wait(sprite_status *actionwk) {
    dai8_work *work = dai8_get_work(actionwk);

    if (--work->timer != 0)
        return;
    actionwk->r_no0 += 2;
}

void act_appear(sprite_status *actionwk) {
    dai8_work *work = dai8_get_work(actionwk);

    patchg(actionwk, (Uint8 **)pchg);
    if (actionwk->patno == 0) {
        work->timer = 120;
        actionwk->r_no0 += 2;
    }
}

void act_on(sprite_status *actionwk) {
    dai8_work *work = dai8_get_work(actionwk);

    if (ridechk(actionwk, &actwk[0]) != 0)
        work->ride_pressed = 255;
    else
        work->ride_pressed = 0;

    if (--work->timer != 0)
        return;
    actionwk->mstno.w = 511;
    actionwk->r_no0 += 2;
    if (work->ride_pressed != 0) {
        ride_on_clr(actionwk, &actwk[0]);
    }
}

void act_off(sprite_status *actionwk) {
    dai8_work *work = dai8_get_work(actionwk);

    patchg(actionwk, (Uint8 **)pchg);

    if (actionwk->patno == 0) {
        work->timer = 120;
        actionwk->r_no0 -= 2;
    }
}
