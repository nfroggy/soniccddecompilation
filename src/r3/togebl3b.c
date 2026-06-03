#include <stddef.h>

#include "../equ.h"
#include "togebl3a.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"

#if defined(R32B)
#define SPRITE_TOGEBL3B_BASE 503
#else
#define SPRITE_TOGEBL3B_BASE 550
#endif

static sprite_pattern spat_chg0 = {1, {{-8, -8, 0, SPRITE_TOGEBL3B_BASE}}};
static sprite_pattern spat_chg1 = {1, {{-8, -8, 0, SPRITE_TOGEBL3B_BASE + 1}}};
sprite_pattern *pat_chg[1] = {&spat_chg0};
sprite_pattern *togeball_pat[1] = {&spat_chg1};

#pragma pack(push, 1)
typedef struct {
    union {
        struct {
            Sint16 angle;
            Sint16 angular_speed;
            Uint16 first_option_index;
            Uint16 second_option_index;
        };
        struct {
            Sint32 target_x;
            Sint32 target_y;
        };
    };
    Uint8 unused8[12];
    Uint16 parent_index;
} togeball3b_work;
#pragma pack(pop)

_Static_assert(offsetof(togeball3b_work, angle) == 0,
               "togeball3b_work.angle offset");
_Static_assert(offsetof(togeball3b_work, angular_speed) == 2,
               "togeball3b_work.angular_speed offset");
_Static_assert(offsetof(togeball3b_work, first_option_index) == 4,
               "togeball3b_work.first_option_index offset");
_Static_assert(offsetof(togeball3b_work, second_option_index) == 6,
               "togeball3b_work.second_option_index offset");
_Static_assert(offsetof(togeball3b_work, target_x) == 0,
               "togeball3b_work.target_x offset");
_Static_assert(offsetof(togeball3b_work, target_y) == 4,
               "togeball3b_work.target_y offset");
_Static_assert(offsetof(togeball3b_work, parent_index) == 20,
               "togeball3b_work.parent_index offset");
_Static_assert(sizeof(togeball3b_work) <= sizeof(((sprite_status *)0)->actfree),
               "togeball3b_work fits in actfree");

static togeball3b_work *togeball3b_get_work(sprite_status *actionwk) {
    return (togeball3b_work *)actionwk->actfree;
}

void togeball(sprite_status *actionwk) {
    if (actionwk->userflag.b.h >= 0)
        togeball_main(actionwk);
    else
        togeball_opt(actionwk);
}

void togeball_main(sprite_status *actionwk) {
    switch (actionwk->r_no0) {

    case 0:
        act_init(actionwk);
        break;
    case 2:
        act_move(actionwk);
        break;
    }
    actionsub(actionwk);
    frameout_s(actionwk);
}

void act_init(sprite_status *actionwk) {
    togeball3b_work *work = togeball3b_get_work(actionwk);
    sprite_status *a1;

    actionwk->r_no0 += 2;
    actionwk->actflg |= 4;
    actionwk->sprpri = 1;
    actionwk->sprhsize = 8;
    actionwk->sprvsize = 8;
    actionwk->patbase = pat_chg;
    actionwk->sproffset = 957;
    work->angular_speed = 256;
    if (actionwk->userflag.b.h != 0)
        work->angular_speed = -work->angular_speed;

    if (actwkchk2(actionwk, &a1) != 0) {

        frameout(actionwk);
        return;
    }
    togeball3b_get_work(a1)->parent_index = actionwk - actwk;
    work->first_option_index = a1 - actwk;
    a1->actno = actionwk->actno;
    a1->userflag.b.h = -1;
    a1->sprhsize = 8;
    a1->sprvsize = 8;

    if (actwkchk2(actionwk, &a1) != 0) {

        frameout(actionwk);
        return;
    }
    togeball3b_get_work(a1)->parent_index = actionwk - actwk;
    work->second_option_index = a1 - actwk;
    a1->actno = actionwk->actno;
    a1->userflag.b.h = -1;
    a1->sprhsize = 8;
    a1->sprvsize = 8;
}

void act_move(sprite_status *actionwk) {
    togeball3b_work *work = togeball3b_get_work(actionwk);
    Sint16 d0;
    Uint16 sin, cos;
    Sint32 d0l, d1l, d2l, d3l, d4l, d5l;
    sprite_status *a1;

    d0 = work->angular_speed;
    work->angle += d0;
    d0 = work->angle;
    d0 >>= 8;

    sinset(d0, (Sint16 *)&sin, (Sint16 *)&cos);
    d0l = d1l = 0;
    d0l = sin << 16;
    d1l = cos << 16;
    d0l >>= 2;
    d1l >>= 2;
    d4l = d0l;
    d5l = d1l;
    d0l >>= 1;
    d1l >>= 1;
    d2l = d0l;
    d3l = d1l;
    d0l >>= 1;
    d1l >>= 1;
    a1 = &actwk[work->first_option_index];
    togeball3b_get_work(a1)->target_y = actionwk->yposi.l + d0l;
    togeball3b_get_work(a1)->target_x = actionwk->xposi.l + d1l;
    a1 = &actwk[work->second_option_index];
    togeball3b_get_work(a1)->target_y = actionwk->yposi.l + d2l;
    togeball3b_get_work(a1)->target_x = actionwk->xposi.l + d3l;
}

void togeball_opt(sprite_status *actionwk) {
    togeball3b_work *work = togeball3b_get_work(actionwk);
    sprite_status *a1;

    switch (actionwk->r_no0) {

    case 0:
        act_init2(actionwk);
    case 2:
        act_move2(actionwk);
        break;
    }
    a1 = &actwk[work->parent_index];
    if (a1->actno != 44) {

        frameout(actionwk);
        return;
    }
    actionsub(actionwk);
}

void act_init2(sprite_status *actionwk) {
    actionwk->r_no0 += 2;
    actionwk->actflg |= 4;
    actionwk->sprpri = 1;
    actionwk->patbase = togeball_pat;
    actionwk->sproffset = 957;
    actionwk->colino = 139;
}

void act_move2(sprite_status *actionwk) {
    togeball3b_work *work = togeball3b_get_work(actionwk);

    actionwk->xposi.w.h = (Sint16)(work->target_x >> 16);
    actionwk->yposi.w.h = (Sint16)(work->target_y >> 16);
}
