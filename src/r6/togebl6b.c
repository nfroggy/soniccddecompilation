#include "../equ.h"
#include "togebl6a.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"

typedef struct {
    Sint16 angle;
    Sint16 angular_speed;
    Uint16 child_index[4];
    union {
        Sint32 target_x_long;
        struct {
            Uint16 target_x_fraction;
            Sint16 target_x;
        };
    };
    union {
        Sint32 target_y_long;
        struct {
            Uint16 target_y_fraction;
            Sint16 target_y;
        };
    };
    Uint16 parent_index;
} togeball6b_work;

static togeball6b_work *togeball6b_get_work(sprite_status *actionwk) {
    return (togeball6b_work *)actionwk->actfree;
}

static void act_init(sprite_status *actionwk);
static void act_move(sprite_status *actionwk);

static sprite_pattern spat_chg0 = {1, {{-8, -8, 0, 491}}};
static sprite_pattern spat_chg1 = {1, {{-8, -8, 0, 492}}};
sprite_pattern *pat_chg[1] = {&spat_chg0};
sprite_pattern *pat_togeball[1] = {&spat_chg1};

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

static void act_init(sprite_status *actionwk) {
    togeball6b_work *work = togeball6b_get_work(actionwk);
    sprite_status *a1;

    actionwk->r_no0 += 2;
    actionwk->actflg |= 4;
    actionwk->sprpri = 1;
    actionwk->sprhsize = 8;
    actionwk->sprvsize = 8;
    actionwk->patbase = pat_chg;
    actionwk->sproffset = 1016;
    work->angular_speed = 256;
    if (actionwk->userflag.b.h != 0)
        work->angular_speed = -work->angular_speed;

    if (actwkchk2(actionwk, &a1) != 0) {
        frameout(actionwk);
        return;
    }
    togeball6b_get_work(a1)->parent_index = actionwk - actwk;
    work->child_index[0] = a1 - actwk;
    a1->actno = actionwk->actno;
    a1->userflag.b.h = -1;
    a1->sprhsize = 8;
    a1->sprvsize = 8;

    if (actwkchk2(actionwk, &a1) != 0) {
        frameout(actionwk);
        return;
    }
    togeball6b_get_work(a1)->parent_index = actionwk - actwk;
    work->child_index[1] = a1 - actwk;
    a1->actno = actionwk->actno;
    a1->userflag.b.h = -1;
    a1->sprhsize = 8;
    a1->sprvsize = 8;

    if (actwkchk2(actionwk, &a1) != 0) {
        frameout(actionwk);
        return;
    }
    togeball6b_get_work(a1)->parent_index = actionwk - actwk;
    work->child_index[2] = a1 - actwk;
    a1->actno = actionwk->actno;
    a1->userflag.b.h = -1;
    a1->sprhsize = 8;
    a1->sprvsize = 8;

    if (actwkchk2(actionwk, &a1) != 0) {
        frameout(actionwk);
        return;
    }
    togeball6b_get_work(a1)->parent_index = actionwk - actwk;
    work->child_index[2] = a1 - actwk;
    a1->actno = actionwk->actno;
    a1->userflag.b.h = -1;
    a1->sprhsize = 8;
    a1->sprvsize = 8;

    if (actwkchk2(actionwk, &a1) != 0) {
        frameout(actionwk);
        return;
    }
    togeball6b_get_work(a1)->parent_index = actionwk - actwk;
    work->child_index[3] = a1 - actwk;
    a1->actno = actionwk->actno;
    a1->userflag.b.h = -1;
    a1->sprhsize = 8;
    a1->sprvsize = 8;
}

static void act_move(sprite_status *actionwk) {
    togeball6b_work *work = togeball6b_get_work(actionwk);
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
    a1 = &actwk[work->child_index[0]];
    togeball6b_get_work(a1)->target_y_long = actionwk->yposi.l + d0l;
    togeball6b_get_work(a1)->target_x_long = actionwk->xposi.l + d1l;
    a1 = &actwk[work->child_index[1]];
    togeball6b_get_work(a1)->target_y_long = actionwk->yposi.l + d2l;
    togeball6b_get_work(a1)->target_x_long = actionwk->xposi.l + d3l;
    a1 = &actwk[work->child_index[2]];
    togeball6b_get_work(a1)->target_y_long = actionwk->yposi.l + d0l + d2l;
    togeball6b_get_work(a1)->target_x_long = actionwk->xposi.l + d1l + d3l;

    a1 = &actwk[work->child_index[3]];
    togeball6b_get_work(a1)->target_y_long = actionwk->yposi.l + d4l;
    togeball6b_get_work(a1)->target_x_long = actionwk->xposi.l + d5l;
}

void togeball_opt(sprite_status *actionwk) {
    sprite_status *a1;

    switch (actionwk->r_no0) {
    case 0:
        act_init2(actionwk);
    case 2:
        act_move2(actionwk);
        break;
    }
    a1 = &actwk[togeball6b_get_work(actionwk)->parent_index];
    if (a1->actno != 45) {
        frameout(actionwk);
        return;
    }
    actionsub(actionwk);
}

void act_init2(sprite_status *actionwk) {
    actionwk->r_no0 += 2;
    actionwk->actflg |= 4;
    actionwk->sprpri = 1;
    actionwk->patbase = pat_togeball;
    actionwk->sproffset = 1016;
    actionwk->colino = 185;
}

void act_move2(sprite_status *actionwk) {
    togeball6b_work *work = togeball6b_get_work(actionwk);

    actionwk->xposi.w.h = work->target_x;
    actionwk->yposi.w.h = work->target_y;
}
