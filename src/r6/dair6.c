#include "../equ.h"
#include "dair6.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"
#include "../ridechk.h"

typedef struct {
    Sint16 origin_y;
    Sint16 origin_x;
    Uint8 phase_counter;
} dair6_work;

static dair6_work *dair6_get_work(sprite_status *actionwk) {
    return (dair6_work *)actionwk->actfree;
}

static sprite_pattern dair6pat0 = {1, {{-24, -16, 0, 418}}};
sprite_pattern *dair6pat[1] = {&dair6pat0};

void dair6(sprite_status *actionwk) {
    switch (actionwk->r_no0) {

    case 0:
        dair6_init(actionwk);
    case 2:
        dair6_move(actionwk);
        break;
    }
    actionsub(actionwk);
    frameout_s00(actionwk, dair6_get_work(actionwk)->origin_x);
}

void dair6_ridechk(sprite_status *actionwk) { ridechk(actionwk, &actwk[0]); }

void dair6_init(sprite_status *actionwk) {
    dair6_work *work = dair6_get_work(actionwk);

    actionwk->actflg |= 4;
    actionwk->sprpri = 1;
    actionwk->sproffset = 17384;
    actionwk->patbase = dair6pat;
    work->origin_x = actionwk->xposi.w.h;
    work->origin_y = actionwk->yposi.w.h;
    actionwk->sprvsize = 12;
    actionwk->sprhsize = 24;
    actionwk->r_no0 += 2;
}

void dair6_move(sprite_status *actionwk) {
    switch (actionwk->userflag.b.h) {

    case 0:
        mv_right(actionwk);
        break;
    case 1:
        mv_left(actionwk);
        break;
    case 2:
        mv_up(actionwk);
        break;
    case 3:
        mv_down(actionwk);
        break;
    }
}

void mv_up(sprite_status *actionwk) {
    dair6_work *work = dair6_get_work(actionwk);
    Sint16 d0;

    d0 = dair6_sub2(actionwk);
    d0 = -d0;
    actionwk->yposi.w.h = d0 + work->origin_y;
    dair6_ridechk(actionwk);
}

void mv_down(sprite_status *actionwk) {
    dair6_work *work = dair6_get_work(actionwk);
    Sint16 d0;

    d0 = dair6_sub2(actionwk);
    actionwk->yposi.w.h = d0 + work->origin_y;
    dair6_ridechk(actionwk);
}

void mv_right(sprite_status *actionwk) {
    dair6_work *work = dair6_get_work(actionwk);
    int_union wk, d0l;
    Sint16 d0;

    wk.l = actionwk->xposi.l;
    d0 = dair6_sub2(actionwk);
    actionwk->xposi.w.h = d0 + work->origin_x;
    d0l.l = actionwk->xposi.l - wk.l;
    d0l.l >>= 8;
    actionwk->xspeed.w = d0l.w.l;
    dair6_ridechk(actionwk);
}

void mv_left(sprite_status *actionwk) {
    dair6_work *work = dair6_get_work(actionwk);
    Sint16 d0;
    int_union d0l, wk;

    wk.l = actionwk->xposi.l;
    d0 = dair6_sub2(actionwk);
    d0 = -d0;
    actionwk->xposi.w.h = d0 + work->origin_x;
    d0l.l = actionwk->xposi.l - wk.l;
    d0l.l >>= 8;
    actionwk->xspeed.w = d0l.w.l;
    dair6_ridechk(actionwk);
}

Sint16 dair6_sub2(sprite_status *actionwk) {
    dair6_work *work = dair6_get_work(actionwk);
    Sint16 sin, cos;

    sinset(gametimer.w & 255, &sin, &cos);
    sin = sin + sin + sin + sin >> 4;
    ++work->phase_counter;
    return sin;
}
