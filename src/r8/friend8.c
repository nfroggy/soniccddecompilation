#include <stddef.h>

#include "../equ.h"
#include "friend8.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../etc.h"
#include "../playsub.h"
#include "../suicide.h"

#if defined(R82)
#define SPRITE_FRIEND8_BASE 459
#elif defined(R83)
#define SPRITE_FRIEND8_BASE 439
#else
#define SPRITE_FRIEND8_BASE 497
#endif

static void t_init(sprite_status *actionwk);
static void t_move(sprite_status *actionwk);
static void t_roll(sprite_status *actionwk, Sint16 d2, Sint16 d3);
static void p_init(sprite_status *actionwk);
static void p_move(sprite_status *actionwk);
static void set_sproffset(sprite_status *actionwk);

#pragma pack(push, 1)
typedef struct {
    union {
        struct {
            Sint16 base_x;
            Sint16 base_y;
            Uint8 angle;
            Sint8 angle_delta;
        };
        struct {
            Uint8 unused0[2];
            Sint32 x_speed;
            Sint32 y_speed;
        };
    };
} friend8_work;
#pragma pack(pop)

_Static_assert(offsetof(friend8_work, base_x) == 0,
               "friend8_work.base_x offset");
_Static_assert(offsetof(friend8_work, base_y) == 2,
               "friend8_work.base_y offset");
_Static_assert(offsetof(friend8_work, angle) == 4,
               "friend8_work.angle offset");
_Static_assert(offsetof(friend8_work, angle_delta) == 5,
               "friend8_work.angle_delta offset");
_Static_assert(offsetof(friend8_work, x_speed) == 2,
               "friend8_work.x_speed offset");
_Static_assert(offsetof(friend8_work, y_speed) == 6,
               "friend8_work.y_speed offset");
_Static_assert(sizeof(friend8_work) <= sizeof(((sprite_status *)0)->actfree),
               "friend8_work fits in actfree");

static friend8_work *friend8_get_work(sprite_status *actionwk) {
    return (friend8_work *)actionwk->actfree;
}

static sprite_pattern pat00 = {1, {{-8, -8, 0, SPRITE_FRIEND8_BASE}}};
static sprite_pattern pat01 = {1, {{-8, -8, 0, SPRITE_FRIEND8_BASE + 1}}};
static sprite_pattern pat02 = {1, {{-12, -8, 0, SPRITE_FRIEND8_BASE + 2}}};
static sprite_pattern pat03 = {1, {{-12, -8, 0, SPRITE_FRIEND8_BASE + 3}}};
sprite_pattern *pat_friend0[2] = {&pat00, &pat01};
sprite_pattern *pat_friend1[2] = {&pat02, &pat03};
static char p00[4] = {3, 0, 1, -1};
Uint8 p01[30] = {0, 0, 0, 2, 0, 0, 2, 1, 1, 2, 2, 1, 1, 2, 2,
                 0, 0, 2, 0, 0, 2, 1, 1, 2, 2, 1, 1, 2, 2, 255};
static char p10[58] = {0, 3, 3, 2, 3, 3, 2, 3, 3, 2, 3, 3, 2, 3, 3,
                       2, 2, 3, 3, 2, 2, 3, 3, 2, 2, 3, 3, 2, 2, 4,
                       4, 2, 4, 4, 2, 4, 4, 2, 4, 4, 2, 4, 4, 2, 2,
                       4, 4, 2, 2, 4, 4, 2, 2, 4, 4, 2, 2, -1};
static char *pchg0[2] = {p00, (char *)p01};
static Uint16 tbl0sproffset[11] = {1943, 986, 986, 0, 1943, 986,
                                   986,  0,   0,   0, 986};

void friend(sprite_status *actionwk) {

    if (friend_suicide(actionwk) == -1)
        return;

    if (actionwk->userflag.b.h & 127)
        sheep(actionwk);
    else
        hato(actionwk);
}

void hato(sprite_status *actionwk) {
    switch (actionwk->r_no0) {
    case 0:
        t_init(actionwk);
        break;
    case 2:
        t_move(actionwk);
        break;
    }
}

static void t_init(sprite_status *actionwk) {
    friend8_work *work = friend8_get_work(actionwk);

    actionwk->r_no0 += 2;
    actionwk->actflg = 4;

    actionwk->sprvsize = 8;
    actionwk->sprhs = 8;
    actionwk->sprpri = 4;
    actionwk->sprhsize = 8;
    actionwk->patbase = pat_friend0;
    work->base_x = actionwk->xposi.w.h;
    work->base_y = actionwk->yposi.w.h;
    actionwk->actflg ^= 1, actionwk->cddat ^= 1;
    set_sproffset(actionwk);

    work->angle = 1;
    work->angle_delta = 1;
}

static void t_move(sprite_status *actionwk) {
    friend8_work *work = friend8_get_work(actionwk);
    char d0, d1;

    t_roll(actionwk, 1, 1);
    d1 = d0 = (char)work->angle + (char)work->angle_delta;
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

static void t_roll(sprite_status *actionwk, Sint16 d2, Sint16 d3) {
    friend8_work *work = friend8_get_work(actionwk);
    Sint16 sin, cos;

    sinset(work->angle, &sin, &cos);
    cos >>= d2;
    sin >>= d3;
    actionwk->xposi.w.h = cos + work->base_x;
    actionwk->yposi.w.h = sin + work->base_y;
}

void sheep(sprite_status *actionwk) {
    switch (actionwk->r_no0) {
    case 0:
        p_init(actionwk);
        break;
    case 2:
        p_move(actionwk);
        break;
    }
}

static void p_init(sprite_status *actionwk) {
    friend8_work *work = friend8_get_work(actionwk);

    actionwk->r_no0 += 2;
    actionwk->actflg |= 4;
    actionwk->sprvsize = 7;
    actionwk->sprhs = 8;
    actionwk->sprpri = 4;
    actionwk->sprhsize = 8;
    actionwk->patbase = pat_friend1;

    set_sproffset(actionwk);

    work->x_speed = 65536;
    work->y_speed = -0x40000;
}

static void p_move(sprite_status *actionwk) {
    friend8_work *work = friend8_get_work(actionwk);
    Sint16 d1;
    Sint32 spd;

    actionwk->xposi.l += work->x_speed;
    actionwk->yposi.l += work->y_speed;

    work->y_speed += 8192;
    spd = work->y_speed;
    if (spd < 0)
        actionwk->patno = 0;
    else
        actionwk->patno = 1;
    d1 = emycol_d(actionwk);
    if (d1 < 0) {

        actionwk->yposi.w.h += d1;
        work->y_speed = -0x40000;
        work->x_speed = -work->x_speed;

        actionwk->actflg ^= 1, actionwk->cddat ^= 1;
    }
    p_end(actionwk);
}

void p_end(sprite_status *actionwk) {
    actionsub(actionwk);
    frameout_s(actionwk);
}

static void set_sproffset(sprite_status *actionwk) {
    Uint16 *a1;
    Uint16 d0;

    a1 = tbl0sproffset;
    d0 = ((stageno.w + 1) << 2) + (Uint16)time_flag;
    actionwk->sproffset = a1[d0];
}
