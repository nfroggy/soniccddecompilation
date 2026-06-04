#include "../equ.h"
#include "friend6.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../etc.h"
#include "../playsub.h"
#include "../suicide.h"

typedef struct {
    Sint16 base_x;
    Sint16 base_y;
    Uint8 angle;
    Sint8 angle_delta;
    Sint32 x_speed;
    Sint32 y_speed;
    Uint16 movie_parent_index;
    Uint8 movie_done;
} friend6_work;

static friend6_work *friend6_get_work(sprite_status *actionwk) {
    return (friend6_work *)actionwk->actfree;
}

static void t_init(sprite_status *actionwk);
static void t_move(sprite_status *actionwk);
static void t_movie(sprite_status *actionwk);
static void t_roll(sprite_status *actionwk, Sint16 d2, Sint16 d3);
static void p_init(sprite_status *actionwk);
static void p_move(sprite_status *actionwk);
static void p_movie(sprite_status *actionwk);
static void set_sproffset(sprite_status *actionwk);

static sprite_pattern patnull = {1, {{0, 0, 0, 0}}};
static sprite_pattern pat00 = {1, {{-12, -8, 0, 435}}};
static sprite_pattern pat01 = {1, {{-12, -8, 0, 436}}};
static sprite_pattern pat02 = {1, {{-8, -12, 0, 437}}};
static sprite_pattern pat03 = {1, {{-8, -12, 0, 438}}};
static sprite_pattern pat04 = {1, {{-12, -8, 0, 439}}};
static sprite_pattern pat05 = {1, {{-12, -8, 0, 440}}};
sprite_pattern *pat_friend0[3] = {&pat00, &pat01, &patnull};
sprite_pattern *pat_friend1[5] = {&pat02, &pat03, &patnull, &pat04, &pat05};
static char p00[4] = {19, 0, 1, -1};
Uint8 p01[30] = {0, 0, 0, 2, 0, 0, 2, 1, 1, 2, 2, 1, 1, 2, 2,
                 0, 0, 2, 0, 0, 2, 1, 1, 2, 2, 1, 1, 2, 2, 255};
static char p10[58] = {0, 3, 3, 2, 3, 3, 2, 3, 3, 2, 3, 3, 2, 3, 3,
                       2, 2, 3, 3, 2, 2, 3, 3, 2, 2, 3, 3, 2, 2, 4,
                       4, 2, 4, 4, 2, 4, 4, 2, 4, 4, 2, 4, 4, 2, 2,
                       4, 4, 2, 2, 4, 4, 2, 2, 4, 4, 2, 2, -1};
static char *pchg0[2] = {p00, (char *)p01};
static char *pchg1[1] = {p10};
static Uint16 tbl0sproffset[11] = {1943, 986, 986, 0, 1943, 986,
                                   986,  0,   0,   0, 986};

void friend(sprite_status *actionwk) {

    if (friend_suicide(actionwk) == -1)
        return;

    if (actionwk->userflag.b.h & 127)
        pecky(actionwk);
    else
        tsubame(actionwk);
}

void tsubame(sprite_status *actionwk) {
    switch (actionwk->r_no0) {
    case 0:
        t_init(actionwk);
        break;
    case 2:
        t_move(actionwk);
        break;
    case 4:
        t_movie(actionwk);
        break;
    }
}

static void t_init(sprite_status *actionwk) {
    friend6_work *work = friend6_get_work(actionwk);

    actionwk->r_no0 += 2;
    actionwk->actflg = 4;

    actionwk->sprvsize = 8;
    actionwk->sprhs = 8;
    actionwk->sprpri = 1;
    actionwk->sprhsize = 8;
    actionwk->patbase = pat_friend0;
    work->base_x = actionwk->xposi.w.h;
    work->base_y = (actionwk->yposi).w.h;
    actionwk->actflg ^= 1, actionwk->cddat ^= 1;
    set_sproffset(actionwk);

    if (actionwk->userflag.b.h < 0) {
        actionwk->r_no0 += 2;
        actionwk->mstno.b.h = 1;
        actionwk->sprpri = 3;
    } else {
        actionwk->sprpri = 1;
        actionwk->sproffset |= 32768;
        work->angle = 1;
        work->angle_delta = 1;
    }
}

static void t_move(sprite_status *actionwk) {
    friend6_work *work = friend6_get_work(actionwk);
    char d0, d1;

    t_roll(actionwk, 1, 1);
    d1 = d0 = (char)work->angle + work->angle_delta;
    d1 += 126;
    if (d1 >= 0) {
        d0 = work->angle;
        work->angle_delta = -work->angle_delta;
        actionwk->actflg ^= 1, actionwk->cddat ^= 1;
    }

    work->angle = d0;
    patchg(actionwk, (Uint8 **)pchg0);
    actionsub(actionwk);
    frameout_s00(actionwk, work->base_x);
}

static void t_movie(sprite_status *actionwk) {
    friend6_work *work = friend6_get_work(actionwk);
    sprite_status *a1;

    a1 = &actwk[work->movie_parent_index];
    if (a1->actno != 47 || friend6_get_work(a1)->movie_done) {
        frameout(actionwk);
        return;
    }
    t_roll(actionwk, 3, 4);
    work->angle += 4;
    if (!(work->angle & 127))
        actionwk->actflg ^= 1, actionwk->cddat ^= 1;

    patchg(actionwk, (Uint8 **)pchg0);
    actionsub(actionwk);
}

static void t_roll(sprite_status *actionwk, Sint16 d2, Sint16 d3) {
    friend6_work *work = friend6_get_work(actionwk);
    Sint16 sin, cos;

    sinset(work->angle, &sin, &cos);
    cos >>= d2;
    sin >>= d3;
    actionwk->xposi.w.h = cos + work->base_x;
    actionwk->yposi.w.h = sin + work->base_y;
}

void pecky(sprite_status *actionwk) {
    switch (actionwk->r_no0) {
    case 0:
        p_init(actionwk);
        break;
    case 2:
        p_move(actionwk);
        break;
    case 4:
        p_move(actionwk);
        break;
    case 6:
        p_rev(actionwk);
        break;
    case 8:
        p_movie(actionwk);
        break;
    }
}

static void p_init(sprite_status *actionwk) {
    actionwk->r_no0 += 2;
    actionwk->actflg |= 4;
    actionwk->sprvsize = 8;
    actionwk->sprhs = 8;
    actionwk->sprpri = 4;
    actionwk->sprhsize = 8;
    actionwk->patbase = pat_friend1;

    set_sproffset(actionwk);
    if (actionwk->userflag.b.h < 0) {
        actionwk->r_no0 = 8;

    } else {
        friend6_get_work(actionwk)->x_speed = 65536;
        friend6_get_work(actionwk)->y_speed = -0x40000;
    }
}

static void p_move(sprite_status *actionwk) {
    friend6_work *work = friend6_get_work(actionwk);
    Sint16 d1;

    actionwk->xposi.l += work->x_speed;
    actionwk->yposi.l += work->y_speed;

    work->y_speed += 8192;
    if (work->y_speed < 0)
        actionwk->patno = 0;
    else
        actionwk->patno = 1;
    d1 = emycol_d(actionwk);
    if (d1 < 0) {
        actionwk->r_no0 += 2;
        actionwk->yposi.w.h += d1;
        work->y_speed = -0x40000;
    }
    p_end(actionwk);
}

void p_end(sprite_status *actionwk) {
    actionsub(actionwk);
    frameout_s(actionwk);
}

void p_rev(sprite_status *actionwk) {
    actionwk->r_no0 = 2;
    friend6_get_work(actionwk)->x_speed = -friend6_get_work(actionwk)->x_speed;

    actionwk->actflg ^= 1, actionwk->cddat ^= 1;
    p_end(actionwk);
}

static void p_movie(sprite_status *actionwk) {
    friend6_work *work = friend6_get_work(actionwk);
    sprite_status *a1;

    a1 = &actwk[work->movie_parent_index];
    if (a1->actno != 47 || friend6_get_work(a1)->movie_done) {
        frameout(actionwk);
        return;
    }
    patchg(actionwk, (Uint8 **)pchg1);
    actionsub(actionwk);
}

static void set_sproffset(sprite_status *actionwk) {
    Uint16 *a1;
    Uint16 d0;

    a1 = tbl0sproffset;
    d0 = ((stageno.w + 1) << 2) + (Uint16)time_flag;
    actionwk->sproffset = a1[d0];
}
