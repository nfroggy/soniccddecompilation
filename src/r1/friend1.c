#include "../equ.h"
#include "friend1.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../etc.h"
#include "../playsub.h"
#include "../suicide.h"

static void t_roll(Sint32 cos_shift, Sint32 sin_shift, sprite_status *pActwk);
static void rev_h(sprite_status *pActwk);
static void set_sproffset(sprite_status *pActwk);
static void t_init(sprite_status *pActwk);
static void t_move(sprite_status *pActwk);
static void t_movie(sprite_status *pActwk);
static void p_init(sprite_status *pActwk);
static void p_move(sprite_status *pActwk);
static void p_movie(sprite_status *pActwk);

#if defined(R11B)
#define SPRITE_FRIEND_BASE 521
#elif defined(R11C) || defined(R11D)
#define SPRITE_FRIEND_BASE 502
#elif defined(R12A) || defined(R12B)
#define SPRITE_FRIEND_BASE 512
#elif defined(R12C) || defined(R12D)
#define SPRITE_FRIEND_BASE 503
#elif defined(R13C) || defined(R13D)
#define SPRITE_FRIEND_BASE 491
#else
#define SPRITE_FRIEND_BASE 536
#endif

static Uint8 pchg00[4] = {19, 0, 1, 255};
static Uint8 pchg01[30] = {0, 0, 0, 2, 0, 0, 2, 1, 1, 2, 2, 1, 1, 2, 2,
                           0, 0, 2, 0, 0, 2, 1, 1, 2, 2, 1, 1, 2, 2, 255};
static Uint8 pchg10[58] = {0, 3, 3, 2, 3, 3, 2, 3, 3, 2, 3, 3, 2,  3, 3,
                           2, 2, 3, 3, 2, 2, 3, 3, 2, 2, 3, 3, 2,  2, 5,
                           5, 2, 5, 5, 2, 5, 5, 2, 5, 5, 2, 5, 5,  2, 2,
                           5, 5, 2, 2, 5, 5, 2, 2, 5, 5, 2, 2, 255};
static Uint8 *pchg0[2] = {pchg00, pchg01};
static Uint8 *pchg1[1] = {pchg10};
static sprite_pattern spr_flicky1 = {1, {{-8, -8, 0, SPRITE_FRIEND_BASE}}};
static sprite_pattern spr_flicky2 = {1, {{-8, -8, 0, SPRITE_FRIEND_BASE + 1}}};
static sprite_pattern spr_ricky1 = {1, {{-12, -8, 0, SPRITE_FRIEND_BASE + 2}}};
static sprite_pattern spr_ricky2 = {1, {{-12, -8, 0, SPRITE_FRIEND_BASE + 3}}};
static sprite_pattern spr_ricky3 = {1, {{-12, -8, 0, SPRITE_FRIEND_BASE + 4}}};
static sprite_pattern spr_ricky4 = {1, {{-12, -8, 0, SPRITE_FRIEND_BASE + 5}}};
static sprite_pattern spr_friend0 = {0, {{-12, -8, 0, SPRITE_FRIEND_BASE + 5}}};
static sprite_pattern spr_ricky5 = {1, {{-12, -8, 0, SPRITE_FRIEND_BASE + 6}}};
sprite_pattern *pat_friend0[3] = {&spr_flicky1, &spr_flicky2, &spr_friend0};
sprite_pattern *pat_friend1[6] = {&spr_ricky1, &spr_ricky2, &spr_friend0,
                                  &spr_ricky3, &spr_ricky4, &spr_ricky5};
static Uint16 tbl0sproffset[11] = {1271, 904, 1123, 0, 1271, 911,
                                   1121, 0,   0,    0, 975};

typedef struct {
    Sint16 origin_x;
    union {
        struct {
            Sint16 origin_y;
            Uint8 orbit_angle;
            Sint8 orbit_delta;
        };
        Sint32 x_velocity;
    };
    Sint32 y_velocity;
    union {
        Uint16 parent_actor;
        struct {
            Uint8 parent_actor_low;
            Uint8 parent_destroyed;
        };
    };
} friend1_work;

static friend1_work *friend1_work_get(sprite_status *pActwk) {
    return (friend1_work *)pActwk->actfree;
}

static void t_roll(Sint32 cos_shift, Sint32 sin_shift, sprite_status *pActwk) {
    friend1_work *work = friend1_work_get(pActwk);
    Uint16 sin, cos;

    sinset(work->orbit_angle, (Sint16 *)&sin, (Sint16 *)&cos);
    cos = (Sint16)cos >> cos_shift;
    sin = (Sint16)sin >> sin_shift;
    cos += work->origin_x;
    sin += work->origin_y;
    pActwk->xposi.w.h = cos;
    pActwk->yposi.w.h = sin;
}

static void rev_h(sprite_status *pActwk) {
    pActwk->actflg ^= 1;
    pActwk->cddat ^= 1;
}

static void set_sproffset(sprite_status *pActwk) {
    Sint32 temp;

    temp = stageno.b.l;
    temp *= 4;
    temp += time_flag;
    temp &= 255;
    pActwk->sproffset = tbl0sproffset[temp];
}

void friend(sprite_status *pActwk) {
    void (*tbl_f[3])(sprite_status *) = {&t_init, &t_move, &t_movie};
    void (*tbl_r[3])(sprite_status *) = {&p_init, &p_move, &p_movie};

    if (friend_suicide(pActwk))
        return;
    if ((pActwk->userflag.b.h & 127) == 0)
        tbl_f[pActwk->r_no0 / 2](pActwk);
    else
        tbl_r[pActwk->r_no0 / 2](pActwk);
}

static void t_init(sprite_status *pActwk) {
    friend1_work *work = friend1_work_get(pActwk);

    pActwk->r_no0 += 2;
    pActwk->actflg = 4;
    pActwk->sprvsize = 8;
    pActwk->sprhs = 8;
    pActwk->sprpri = 4;
    pActwk->sprhsize = 8;
    pActwk->patbase = pat_friend0;
    work->origin_x = pActwk->xposi.w.h;
    work->origin_y = pActwk->yposi.w.h;
    rev_h(pActwk);
    set_sproffset(pActwk);

    if (pActwk->userflag.b.h >= 0) {
        pActwk->sprpri = 4;
        pActwk->sproffset |= 32768;
        work->orbit_angle = 1;
        work->orbit_delta = 1;
    } else {
        pActwk->r_no0 += 2;
        pActwk->mstno.b.h = 1;
        pActwk->sprpri = 3;
    }
}

static void t_move(sprite_status *pActwk) {
    friend1_work *work = friend1_work_get(pActwk);
    Uint8 temp;

    t_roll(1, 1, pActwk);
    temp = work->orbit_angle + (Uint8)work->orbit_delta;
    if ((Uint8)(temp - 1) < 127)
        goto label1;
    temp = work->orbit_angle;
    work->orbit_delta = -work->orbit_delta;
    rev_h(pActwk);
label1:
    work->orbit_angle = temp;
    patchg(pActwk, pchg0);
    actionsub(pActwk);
    frameout_s00(pActwk, work->origin_x);
}

static void t_movie(sprite_status *pActwk) {
    friend1_work *work = friend1_work_get(pActwk);
    sprite_status *tempact;

    tempact = &actwk[work->parent_actor];
    if (tempact->actno != 46) {
        frameout(pActwk);
        return;
    }
    if (friend1_work_get(tempact)->parent_destroyed) {
        frameout(pActwk);
        return;
    }

    t_roll(3, 4, pActwk);

    work->orbit_angle += 4;
    if ((work->orbit_angle & 127) == 0)
        rev_h(pActwk);
    else {
        patchg(pActwk, pchg0);
        actionsub(pActwk);
    }
}

static void p_init(sprite_status *pActwk) {
    friend1_work *work = friend1_work_get(pActwk);

    pActwk->r_no0 += 2;
    pActwk->actflg = 4;
    pActwk->sprvsize = 8;
    pActwk->sprhs = 8;
    pActwk->sprpri = 4;
    pActwk->sprhsize = 8;
    pActwk->patbase = pat_friend1;
    work->origin_x = pActwk->xposi.w.h;
    set_sproffset(pActwk);

    if (pActwk->userflag.b.h >= 0) {
        work->x_velocity = 65536;
        work->y_velocity = -0x40000;
    } else {
        pActwk->r_no0 = 4;
        rev_h(pActwk);
    }
}

static void p_move(sprite_status *pActwk) {
    friend1_work *work = friend1_work_get(pActwk);
    Sint16 temp;

    pActwk->xposi.l += work->x_velocity;
    pActwk->yposi.l += work->y_velocity;

    if ((work->y_velocity += 8192) < 0)
        pActwk->patno = 0;
    else
        pActwk->patno = 1;

    if ((temp = emycol_d(pActwk)) <= 0) {
        pActwk->yposi.w.h += temp;
        work->y_velocity = -0x40000;
        work->x_velocity *= -1;
        rev_h(pActwk);
    }
    actionsub(pActwk);
    frameout_s(pActwk);
}

static void p_movie(sprite_status *pActwk) {
    friend1_work *work = friend1_work_get(pActwk);
    sprite_status *tempact;

    tempact = &actwk[work->parent_actor];
    if (tempact->actno != 46) {
        frameout(pActwk);
        return;
    }
    if (friend1_work_get(tempact)->parent_destroyed) {
        frameout(pActwk);
        return;
    }
    patchg(pActwk, pchg1);
    actionsub(pActwk);
}
