#include "../equ.h"
#include "pipe8.h"
#include "../action.h"
#include "../actset.h"
#include "../playsub.h"
#include "../ridechk.h"
#include <stddef.h>

#pragma pack(push, 1)
typedef struct {
    union {
        Sint16 origin_x;
        Sint16 animation_timer;
    };
    Uint8 reserved0[20 - 2];
    Sint16 parent_actor;
} pipe8_work;
#pragma pack(pop)

_Static_assert(offsetof(pipe8_work, origin_x) == 0,
               "pipe8_work.origin_x offset");
_Static_assert(offsetof(pipe8_work, animation_timer) == 0,
               "pipe8_work.animation_timer offset");
_Static_assert(offsetof(pipe8_work, parent_actor) == 20,
               "pipe8_work.parent_actor offset");
_Static_assert(sizeof(pipe8_work) <= sizeof(((sprite_status *)0)->actfree),
               "pipe8_work fits in actfree");

static pipe8_work *pipe8_work_get(sprite_status *actionwk) {
    return (pipe8_work *)actionwk->actfree;
}

static char p00[5] = {0, 1, 3, 5, -1};
static char p01[6] = {0, 4, 3, 2, 1, -1};
static char *pchg[2] = {p00, p01};
static sprite_pattern pat00 = {1, {{-8, -4, 0, 477}}};
static sprite_pattern pat01 = {1, {{-16, -4, 0, 478}}};
static sprite_pattern pat02 = {1, {{-20, -4, 0, 479}}};
static sprite_pattern pat03 = {1, {{-24, -4, 0, 480}}};
static sprite_pattern pat04 = {1, {{-28, -4, 0, 481}}};
static sprite_pattern pat05 = {1, {{0, 0, 0, 0}}};
sprite_pattern *pat_pipe8[6] = {&pat00, &pat01, &pat02, &pat03, &pat04, &pat05};

void pipe8(sprite_status *actionwk) {
    Sint16 ano;

    if (actionwk->userflag.b.l < 0) {
        shatter(actionwk);
        return;
    }

    if (actionwk->userflag.b.h < 0) {
        ano = pipe8_work_get(actionwk)->parent_actor;
        if (actwk[ano].actno != 32) {
            frameout(actionwk);
            return;
        }
    }

    switch (actionwk->r_no0) {
    case 0:
        act0wait(actionwk);
    case 2:
        act0wait1(actionwk);
        break;
    case 4:
        act0init(actionwk);
    case 6:
        act0move(actionwk);
        break;
    }
    actionsub(actionwk);
    if (actionwk->userflag.b.h < 0)
        return;
    frameout_s00(actionwk, pipe8_work_get(actionwk)->origin_x);
}

void act0wait(sprite_status *actionwk) {
    pipe8_work_get(actionwk)->origin_x = actionwk->xposi.w.h;
    act0init_sub(actionwk, actionwk);
    actionwk->r_no0 = 2;
    actionwk->patno = 5;
}

void act0wait1(sprite_status *actionwk) {
    Sint16 d0;

    d0 = actwk[0].xposi.w.h - actionwk->xposi.w.h + 320;
    if ((Uint16)d0 >= 640)
        return;
    actionwk->r_no0 += 2;
    actionwk->patno = 0;
}

void act0init(sprite_status *actionwk) {
    sprite_status *a1;

    if (actwkchk(&a1) != 0) {
        frameout(actionwk);
        return;
    }
    a1->actno = actionwk->actno;
    pipe8_work_get(a1)->parent_actor = (Sint16)(actionwk - actwk);
    a1->userflag.b.h = -1;
    a1->xposi.w.h = actionwk->xposi.w.h + 48;
    a1->actflg ^= 1;
    a1->cddat ^= 1;

    act0init_sub(actionwk, actionwk);
    act0init_sub(actionwk, a1);

    if (actwkchk(&a1) != 0) {
        frameout(actionwk);
        return;
    }
    a1->actno = actionwk->actno;
    a1->userflag.b.h = actionwk->userflag.b.h;
    a1->userflag.b.l = -1;
    a1->xposi.w.h = actionwk->xposi.w.h + 24;
    a1->yposi.w.h = actionwk->yposi.w.h;
    pipe8_work_get(a1)->parent_actor = (Sint16)(actionwk - actwk);
}

void act0init_sub(sprite_status *actionwk, sprite_status *a6) {
    a6->r_no0 = 6;
    a6->actflg |= 4;
    a6->sprpri = 3;
    a6->sproffset = 828;
    a6->sprhs = 8;
    a6->sprhsize = 8;
    a6->sprvsize = 4;
    a6->patbase = pat_pipe8;
    a6->yposi.w.h = actionwk->yposi.w.h;
}

void act0move(sprite_status *actionwk) { ridechk(actionwk, &actwk[0]); }

void shatter(sprite_status *actionwk) {
    Sint16 ano;

    ano = pipe8_work_get(actionwk)->parent_actor;
    if (actwk[ano].actno != 32) {
        frameout(actionwk);
        return;
    }
    switch (actionwk->r_no0) {
    case 0:
        act1init(actionwk);
        break;
    case 2:
        act1closed(actionwk);
        break;
    case 4:
        act1closed1(actionwk);
        break;
    case 6:
        act1open_a(actionwk);
        break;
    case 8:
        act1opend_a(actionwk);
        break;
    case 10:
        act1opend_b(actionwk);
        break;
    case 12:
        act1close_b(actionwk);
        break;
    }
    actionsub(actionwk);
}

void act1init(sprite_status *actionwk) {
    actionwk->r_no0 += 2;
    actionwk->actflg |= 4;
    actionwk->sprpri = 4;
    actionwk->sproffset = 828;
    actionwk->sprhs = 16;
    actionwk->sprhsize = 16;
    actionwk->sprvsize = 4;
    actionwk->patbase = pat_pipe8;
}

void act1closed(sprite_status *actionwk) {
    actionwk->r_no0 += 2;
    actionwk->patno = 1;
    switchflag[actionwk->userflag.b.h] &= 191;
}

void act1closed1(sprite_status *actionwk) {
    ridechk(actionwk, &actwk[0]);
    if ((char)switchflag[actionwk->userflag.b.h] < 0) {
        to_act1open_a(actionwk);
        return;
    }

    if (!act1check2(actionwk, &actwk[0], -8, 16, -3, 123))
        return;

    to_act1opend_b(actionwk);
}

void to_act1open_a(sprite_status *actionwk) {
    actionwk->r_no0 += 2;
    pipe8_work_get(actionwk)->animation_timer = 5;
}

void to_act1opend_b(sprite_status *actionwk) {
    actionwk->r_no0 += 6;
    actionwk->patno = 5;
}

void act1open_a(sprite_status *actionwk) {
    Sint16 t;
    pipe8_work *work = pipe8_work_get(actionwk);

    --work->animation_timer;
    t = work->animation_timer;
    if (t < 0) {
        actionwk->r_no0 += 2;
        actionwk->patno = 5;
        return;
    }

    patchg(actionwk, (Uint8 **)pchg);
}

void act1opend_a(sprite_status *actionwk) {
    if (act1check1(actionwk, &actwk[0], -256, 512, -160, 320) != 0)
        return;

    to_act1closed_a(actionwk);
}

void to_act1closed_a(sprite_status *actionwk) { actionwk->r_no0 -= 6; }

void act1opend_b(sprite_status *actionwk) {
    if (act1check2(actionwk, &actwk[0], -8, 16, -120, 116)) {
        to_act1close_b(actionwk);
    }
}

void to_act1close_b(sprite_status *actionwk) {
    actionwk->r_no0 += 2;
    actionwk->mstno.b.h = 1;
    pipe8_work_get(actionwk)->animation_timer = 7;
}

void act1close_b(sprite_status *actionwk) {
    Sint16 t;
    pipe8_work *work = pipe8_work_get(actionwk);

    --work->animation_timer;
    t = work->animation_timer;
    if (t < 0) {
        actionwk->r_no0 -= 10;
        actionwk->mstno.b.h = 0;
        actionwk->patno = 1;
    } else {
        patchg(actionwk, (Uint8 **)pchg);
    }
}

Sint16 act1check2(sprite_status *actionwk, sprite_status *a6, Sint16 d0,
                  Sint16 d1, Sint16 d2, Sint16 d3) {
    if (a6->yspeed.w >= 0)
        return 0;
    else
        return act1check1(actionwk, a6, d0, d1, d2, d3);
}

Sint16 act1check1(sprite_status *actionwk, sprite_status *a6, Sint16 d0,
                  Sint16 d1, Sint16 d2, Sint16 d3) {
    Sint16 d4;

    d4 = a6->yposi.w.h - actionwk->yposi.w.h;
    d4 -= d2;
    if ((Uint16)d3 <= (Uint16)d4)
        return 0;
    d4 = a6->xposi.w.h - actionwk->xposi.w.h;
    d4 -= d0;
    if ((Uint16)d1 <= (Uint16)d4)
        return 0;
    return 1;
}
