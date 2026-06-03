#include <stddef.h>

#include "../equ.h"
#include "friend5.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../etc.h"
#include "../playsub.h"
#include "../suicide.h"

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
    Uint8 unused10[10];
    union {
        Sint16 movie_parent_index;
        struct {
            Uint8 unused20;
            Uint8 movie_done;
        };
    };
} friend5_work;
#pragma pack(pop)

_Static_assert(offsetof(friend5_work, base_x) == 0,
               "friend5_work.base_x offset");
_Static_assert(offsetof(friend5_work, base_y) == 2,
               "friend5_work.base_y offset");
_Static_assert(offsetof(friend5_work, angle) == 4,
               "friend5_work.angle offset");
_Static_assert(offsetof(friend5_work, angle_delta) == 5,
               "friend5_work.angle_delta offset");
_Static_assert(offsetof(friend5_work, x_speed) == 2,
               "friend5_work.x_speed offset");
_Static_assert(offsetof(friend5_work, y_speed) == 6,
               "friend5_work.y_speed offset");
_Static_assert(offsetof(friend5_work, movie_parent_index) == 20,
               "friend5_work.movie_parent_index offset");
_Static_assert(offsetof(friend5_work, movie_done) == 21,
               "friend5_work.movie_done offset");
_Static_assert(sizeof(friend5_work) <= sizeof(((sprite_status *)0)->actfree),
               "friend5_work fits in actfree");

static friend5_work *friend5_get_work(sprite_status *pActwk) {
    return (friend5_work *)pActwk->actfree;
}

static void t_init(sprite_status *pActwk);
static void t_move(sprite_status *pActwk);
static void t_movie(sprite_status *pActwk);
static void p_init(sprite_status *pActwk);
static void p_move(sprite_status *pActwk);
static void p_movie(sprite_status *pActwk);
static void set_sproffset(sprite_status *pActwk);
static void t_roll(sprite_status *pActwk, Sint32 xsft, Sint32 ysft);

static sprite_pattern friend2_pat02;
static sprite_pattern friend1_pat02;
static sprite_pattern friend0_pat02;

static Uint8 pchg0_00[4] = {19, 0, 1, 255};
Uint8 pchg0_01[30] = {0, 0, 0, 2, 0, 0, 2, 1, 1, 2, 2, 1, 1, 2, 2,
                      0, 0, 2, 0, 0, 2, 1, 1, 2, 2, 1, 1, 2, 2, 255};
static Uint8 *pchg0[2] = {pchg0_00, pchg0_01};
static Uint8 pchg1_00[58] = {0, 3, 3, 2, 3, 3, 2, 3, 3, 2, 3, 3, 2,  3, 3,
                             2, 2, 3, 3, 2, 2, 3, 3, 2, 2, 3, 3, 2,  2, 4,
                             4, 2, 4, 4, 2, 4, 4, 2, 4, 4, 2, 4, 4,  2, 2,
                             4, 4, 2, 2, 4, 4, 2, 2, 4, 4, 2, 2, 255};
static Uint8 *pchg1[1] = {pchg1_00};
static sprite_pattern friend0_pat00 = {1, {{-8, -8, 0, 435}}};
static sprite_pattern friend0_pat01 = {1, {{-8, -8, 0, 436}}};
sprite_pattern *pat_friend0[3] = {&friend0_pat00, &friend0_pat01,
                                  &friend0_pat02};
static sprite_pattern friend1_pat00 = {1, {{-12, -8, 1, 437}}};
static sprite_pattern friend1_pat01 = {1, {{-12, -8, 1, 438}}};
static sprite_pattern friend1_pat03 = {1, {{-12, -8, 1, 439}}};
static sprite_pattern friend1_pat04 = {1, {{-12, -7, 1, 440}}};
sprite_pattern *pat_friend1[5] = {&friend1_pat00, &friend1_pat01,
                                  &friend1_pat02, &friend1_pat03,
                                  &friend1_pat04};
static sprite_pattern friend2_pat00 = {1, {{-12, -8, 1, 441}}};
static sprite_pattern friend2_pat01 = {1, {{-12, -8, 1, 442}}};
static sprite_pattern friend2_pat03 = {1, {{-12, -8, 1, 443}}};
static sprite_pattern friend2_pat04 = {1, {{-12, -7, 1, 444}}};
static sprite_pattern *pat_friend2[5] = {&friend2_pat00, &friend2_pat01,
                                         &friend2_pat02, &friend2_pat03,
                                         &friend2_pat04};
static void (*flicky_act_tbl[3])(sprite_status *) = {&t_init, &t_move,
                                                     &t_movie};
static void (*ricky_act_tbl[3])(sprite_status *) = {&p_init, &p_move, &p_movie};

void friend(sprite_status *pActwk) {
    if (!friend_suicide(pActwk)) {

        if (pActwk->userflag.b.h & 127) {

            ricky_act_tbl[pActwk->r_no0 / 2](pActwk);
            return;
        }
        flicky_act_tbl[pActwk->r_no0 / 2](pActwk);
    }
}

static void t_init(sprite_status *pActwk) {
    friend5_work *work = friend5_get_work(pActwk);

    pActwk->r_no0 += 2;
    pActwk->actflg = 4;
    pActwk->sprvsize = 8;
    pActwk->sprhs = 8;
    pActwk->sprhsize = 8;
    pActwk->sprpri = 4;
    pActwk->patbase = pat_friend0;
    work->base_x = pActwk->xposi.w.h;
    work->base_y = pActwk->yposi.w.h;
    pActwk->actflg ^= 1, pActwk->cddat ^= 1;
    set_sproffset(pActwk);

    if (pActwk->userflag.b.h & 128) {

        pActwk->r_no0 += 2;
        pActwk->mstno.b.h = 1;
        pActwk->sprpri = 3;
    } else {
        pActwk->sproffset |= 32768;
        work->angle = 1;
        work->angle_delta = 1;
    }
}

static void t_move(sprite_status *pActwk) {
    friend5_work *work = friend5_get_work(pActwk);
    char anglewk;

    t_roll(pActwk, 1, 1);

    anglewk = (char)work->angle + work->angle_delta;

    if ((Uint8)(anglewk - 1) >= 127) {
        anglewk = work->angle;
        work->angle_delta *= -1;
        pActwk->actflg ^= 1, pActwk->cddat ^= 1;
    }

    work->angle = anglewk;
    patchg(pActwk, pchg0);
    actionsub(pActwk);
    frameout_s00(pActwk, work->base_x);
}

static void t_movie(sprite_status *pActwk) {
    friend5_work *work = friend5_get_work(pActwk);
    sprite_status *pMoviewk;

    pMoviewk = &actwk[work->movie_parent_index];
    if (pMoviewk->actno != 44 || friend5_get_work(pMoviewk)->movie_done) {
        frameout(pActwk);
        return;
    }

    t_roll(pActwk, 3, 4);
    work->angle += 4;
    if (!(work->angle & 127)) {
        pActwk->actflg ^= 1, pActwk->cddat ^= 1;
    } else {
        patchg(pActwk, pchg0);
        actionsub(pActwk);
    }
}

static void p_init(sprite_status *pActwk) {
    friend5_work *work = friend5_get_work(pActwk);

    pActwk->r_no0 += 2;
    pActwk->actflg = 4;
    pActwk->sprvsize = 8;
    pActwk->sprhs = 8;
    pActwk->sprhsize = 8;
    pActwk->sprpri = 4;
    pActwk->patbase = pat_friend1;
    work->base_x = pActwk->xposi.w.h;
    set_sproffset(pActwk);

    if (pActwk->userflag.b.h & 128) {

        pActwk->patbase = pat_friend2;
        pActwk->r_no0 += 2;
        pActwk->actflg ^= 1, pActwk->cddat ^= 1;
    } else {
        work->x_speed = 65536;
        work->y_speed = -0x40000;
    }
}

static void p_move(sprite_status *pActwk) {
    friend5_work *work = friend5_get_work(pActwk);
    Sint16 colval;

    pActwk->xposi.l += work->x_speed;
    pActwk->yposi.l += work->y_speed;
    if ((work->y_speed += 8192) < 0) {
        pActwk->patno = 0;
    } else
        pActwk->patno = 1;

    if ((colval = emycol_d(pActwk)) < 0) {
        pActwk->yposi.w.h += colval;
        work->y_speed = -0x40000;
        work->x_speed *= -1;
        pActwk->actflg ^= 1, pActwk->cddat ^= 1;
    }

    actionsub(pActwk);
    frameout_s(pActwk);
}

static void p_movie(sprite_status *pActwk) {
    friend5_work *work = friend5_get_work(pActwk);
    sprite_status *pMoviewk;

    pMoviewk = &actwk[work->movie_parent_index];
    if (pMoviewk->actno != 44 || friend5_get_work(pMoviewk)->movie_done) {
        frameout(pActwk);
        return;
    }

    patchg(pActwk, pchg1);
    actionsub(pActwk);
}

static void set_sproffset(sprite_status *pActwk) {
    Sint16 tbl0sproffset[11] = {1210, 1210, 1210, 0, 1210, 1210,
                                1210, 0,    0,    0, 1210};

    pActwk->sproffset = tbl0sproffset[time_flag + stageno.b.l * 4];
}

static void t_roll(sprite_status *pActwk, Sint32 xsft, Sint32 ysft) {
    friend5_work *work = friend5_get_work(pActwk);
    Sint16 sinwk, coswk;

    sinset(work->angle, &sinwk, &coswk);
    coswk >>= xsft;
    sinwk >>= ysft;
    coswk += work->base_x;
    sinwk += work->base_y;
    pActwk->xposi.w.h = coswk;
    pActwk->yposi.w.h = sinwk;
}
