#include "../equ.h"
#include "dai_rd5.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../etc.h"
#include "../player_work.h"
#include "../ridechk.h"
#include "../spring.h"

typedef struct {
    union {
        Sint16 motion_position;
        struct {
            Uint8 motion_low;
            Uint8 motion_high;
        };
    };
    Sint16 motion_speed;
    Sint16 motion_delta;
    Sint16 origin_y;
    Sint16 origin_x;
    Uint8 wait_timer;
    Uint8 moving_backward;
    Uint8 state;
    Uint8 bob_angle;
    Uint8 flags;
} dai_rd5_work;

static inline dai_rd5_work *dai_rd5_work_get(sprite_status *pActwk) {
    return (dai_rd5_work *)pActwk->actfree;
}

#if defined(R53C) || defined(R53D)
#define SPRITE_DAIRD5_BASE 413
#else
#define SPRITE_DAIRD5_BASE 468
#endif

static void dair5_init(sprite_status *pActwk);
static void dair5_move(sprite_status *pActwk);
static void type01(sprite_status *pActwk);
static void type02(sprite_status *pActwk);
static void type03(sprite_status *pActwk);
static void type04(sprite_status *pActwk);
static void type05(sprite_status *pActwk);
static void type06(sprite_status *pActwk);
static void type06_00(sprite_status *pActwk);
static void type06_01(sprite_status *pActwk);
static void type06_02(sprite_status *pActwk);
static void dodai_sub(sprite_status *pActwk, Uint8 d4);
static void dai5sub(sprite_status *pActwk);
static void banesetsub(sprite_status *pActwk, char d2);
static void belttask(sprite_status *pActwk);

static sprite_pattern pat00 = {1, {{-16, -16, 0, SPRITE_DAIRD5_BASE}}};
static sprite_pattern pat01 = {2,
                               {{-16, -16, 0, SPRITE_DAIRD5_BASE + 1},
                                {-16, -16, 0, SPRITE_DAIRD5_BASE + 2}}};
static sprite_pattern pat02 = {1, {{-32, -16, 0, SPRITE_DAIRD5_BASE + 3}}};
static sprite_pattern pat03 = {2,
                               {{-32, -16, 0, SPRITE_DAIRD5_BASE + 4},
                                {-32, -16, 0, SPRITE_DAIRD5_BASE + 5}}};
static sprite_pattern pat04 = {1, {{-48, -16, 0, SPRITE_DAIRD5_BASE + 6}}};
static sprite_pattern pat05 = {2,
                               {{-48, -16, 0, SPRITE_DAIRD5_BASE + 7},
                                {-48, -16, 0, SPRITE_DAIRD5_BASE + 8}}};
sprite_pattern *dair5pat[6] = {&pat00, &pat01, &pat02, &pat03, &pat04, &pat05};

void dair5(sprite_status *pActwk) {
    void (*tbl[2])(sprite_status *) = {&dair5_init, &dair5_move};
    dai_rd5_work *work = dai_rd5_work_get(pActwk);

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s00(pActwk, work->origin_x);
}

static void dair5_init(sprite_status *pActwk) {
    Uint8 *a2;
    Sint16 d0;
    Uint8 bd0;
    Uint8 bd1;
    Uint8 ini_tbl[50] = {0,  16, 1,  16, 0,  16, 0,  16, 1,  16, 0,  16, 0,
                         16, 2,  32, 2,  32, 3,  32, 2,  32, 3,  32, 2,  32,
                         3,  32, 2,  32, 2,  32, 2,  32, 4,  48, 5,  48, 4,
                         48, 4,  48, 5,  48, 4,  48, 4,  48, 4,  48};

    Uint8 belt_flag[50] = {
        0,  1,   0,   0,   1,   0,   0, 0, 0, 1,   0,   1,   0,   1,   0, 0, 0,
        0,  1,   0,   0,   1,   0,   0, 0, 0, 192, 192, 192, 192, 0,   0, 0, 64,
        64, 192, 192, 192, 192, 192, 0, 0, 0, 0,   192, 192, 192, 192, 0, 0};

    Uint8 bane_flag[25] = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 2, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0};
    dai_rd5_work *work = dai_rd5_work_get(pActwk);

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;

    d0 = 17156;
    if (stageno.b.l == 2) {
        d0 = 17272;
    }
    pActwk->sproffset = d0;

    pActwk->patbase = dair5pat;
    work->origin_x = pActwk->xposi.w.h;
    work->origin_y = pActwk->yposi.w.h;
    pActwk->sprvsize = 16;
    bd0 = 0;
    bd1 = 0;

    bd0 = pActwk->userflag.b.h;
    bd0 &= 31;
    bd1 = bd0;
    bd1 = bd1 + bd1;
    a2 = &ini_tbl[bd1];
    pActwk->patno = *a2++;
    pActwk->sprhsize = *a2++;

    a2 = &belt_flag[bd0];
    work->flags = *a2;

    a2 = &bane_flag[bd0];
    bd1 = *a2;
    switch (bd1) {

    case 0:
        break;

    case 1:
        banesetsub(pActwk, 0);
        break;

    case 2:
        banesetsub(pActwk, -16);
        banesetsub(pActwk, 16);
        break;

    case 3:
        banesetsub(pActwk, -32);
        banesetsub(pActwk, 0);
        banesetsub(pActwk, 32);
        break;
    }

    dair5_move(pActwk);
}

static void dair5_move(sprite_status *pActwk) {
    void (*tbl[25])(sprite_status *) = {
        &type01, &type02, &type02, &type03, &type03, &type06, &type05,
        &type01, &type04, &type04, &type03, &type03, &type02, &type02,
        &type02, &type06, &type05, &type01, &type01, &type03, &type02,
        &type02, &type02, &type06, &type05,
    };

    tbl[pActwk->userflag.b.h & 31](pActwk);
    dai5sub(pActwk);
    belttask(pActwk);
    ridechk(pActwk, &actwk[0]);
}

static void type01(sprite_status *pActwk) {
    dai_rd5_work_get(pActwk)->flags |= 128;
}

static void type02(sprite_status *pActwk) {
    Sint16 d0;
    dai_rd5_work *work = dai_rd5_work_get(pActwk);

    work->flags |= 128;
    work->motion_delta = 4;
    dodai_sub(pActwk, 48);

    d0 = work->motion_speed;
    if (pActwk->userflag.b.h & 128) {
        d0 *= -1;
    }
    pActwk->xspeed.w = d0;
    pActwk->xposi.l += d0 << 8;
}

static void type03(sprite_status *pActwk) {
    Sint32 ld0;
    Sint16 d0;
    dai_rd5_work *work = dai_rd5_work_get(pActwk);

    work->motion_delta = 4;
    dodai_sub(pActwk, 48);
    d0 = work->motion_speed;
    if (pActwk->userflag.b.h & 128) {
        d0 *= -1;
    }
    ld0 = d0;
    ld0 <<= 8;
    pActwk->yposi.l += ld0;
}

static void type04(sprite_status *pActwk) {
    Sint16 d0;
    dai_rd5_work *work = dai_rd5_work_get(pActwk);

    work->flags |= 128;
    work->motion_delta = 4;
    dodai_sub(pActwk, 32);

    d0 = work->motion_speed;
    if (pActwk->userflag.b.h & 128) {
        d0 *= -1;
    }
    pActwk->xspeed.w = d0;
    pActwk->xposi.l += d0 << 8;
}

static void type05(sprite_status *pActwk) {
    Sint32 ld0;
    Sint16 d0;
    dai_rd5_work *work = dai_rd5_work_get(pActwk);

    if (work->state == 0) {
        work->flags |= 128;
        if (pActwk->cddat & 8) {
            work->wait_timer = 30;
            work->state = 1;
        }
    } else {
        if (work->wait_timer) {
            --work->wait_timer;
            if (work->wait_timer)
                return;
            work->flags &= 127;
        }
        pActwk->sprvsize = 16;
        if (pActwk->yspeed.w >= 512) {
            pActwk->sprvsize = 18;
            if (pActwk->yspeed.w >= 768) {
                pActwk->sprvsize = 20;
            }
        }
        d0 = pActwk->yspeed.w;
        if (d0 < 1024) {
            pActwk->yspeed.w += 32;
        }
        ld0 = pActwk->yspeed.w;
        ld0 <<= 8;
        pActwk->yposi.l += ld0;
        d0 = scra_v_posit.w.h;
        d0 += 224;
        if (d0 < pActwk->yposi.w.h) {
            ride_on_clr(pActwk, &actwk[0]);
            frameout(pActwk);
        }
    }
}

static void type06(sprite_status *pActwk) {
    void (*tbl[3])(sprite_status *) = {&type06_00, &type06_01, &type06_02};
    dai_rd5_work *work = dai_rd5_work_get(pActwk);

    tbl[work->state / 2](pActwk);
}

static void type06_00(sprite_status *pActwk) {
    dai_rd5_work *work = dai_rd5_work_get(pActwk);

    work->flags |= 128;
    if (pActwk->cddat & 8) {
        work->state += 2;
        work->wait_timer = 30;
        type06_01(pActwk);
    }
}

static void type06_01(sprite_status *pActwk) {
    Sint32 stk;
    Sint16 d0, d1;
    dai_rd5_work *work = dai_rd5_work_get(pActwk);

    if (work->wait_timer) {
        --work->wait_timer;
        if (work->wait_timer)
            return;
        work->flags &= 127;
        pActwk->sprvsize = 14;
    }
    d0 = pActwk->yspeed.w;
    d0 -= 8;
    if (d0 < -768) {
        d0 = -768;
    }
    pActwk->yspeed.w = d0;
    speedset2(pActwk);
    stk = pActwk->yposi.l;
    pActwk->yposi.w.h -= 48;
    d1 = emycol_u(pActwk);
    pActwk->yposi.l = stk;
    if (d1 < 0) {
        pActwk->sprvsize = 16;
        pActwk->yspeed.w = 0;
        work->origin_y = pActwk->yposi.w.h;
        work->flags |= 128;
        work->state += 2;
    }
}

static void type06_02(sprite_status *pActwk) {}

static void dodai_sub(sprite_status *pActwk, Uint8 d4) {
    Sint16 d0, d2;
    dai_rd5_work *work = dai_rd5_work_get(pActwk);

    d2 = work->motion_delta;
    if (work->moving_backward == 0) {
        d0 = work->motion_speed;
        d0 += d2;
        work->motion_speed = d0;
        work->motion_position += d0;
        if (d4 <= work->motion_high) {
            work->moving_backward = 255;
        }
    } else {
        d0 = work->motion_speed;
        d0 -= d2;
        work->motion_speed = d0;
        work->motion_position += d0;
        if (d4 > work->motion_high) {
            work->moving_backward = 0;
        }
    }
}

static void dai5sub(sprite_status *pActwk) {
    int_union ld0;
    Sint16 d0, d1;
    dai_rd5_work *work = dai_rd5_work_get(pActwk);

    if (work->flags & 128) {
        if (!(pActwk->cddat & 8)) {
            if (work->bob_angle == 0)
                return;

            work->bob_angle -= 8;
        } else {
            if (work->bob_angle == 64)
                return;

            work->bob_angle += 8;
        }
        sinset(work->bob_angle, &d0, &d1);
        d1 = 1024;
        ld0.l = d0 * d1;
        d0 = ld0.w.h;
        d0 += work->origin_y;
        pActwk->yposi.w.h = d0;
    }
}

static void banesetsub(sprite_status *pActwk, char d2) {
    sprite_status *pNewActwk;

    if (actwkchk2(pActwk, &pNewActwk) == 0) {
        spring_work *spring = spring_work_get(pNewActwk);

        pNewActwk->actno = 10;
        pNewActwk->yposi.w.h = pActwk->yposi.w.h;
        pNewActwk->xposi.w.h = pActwk->xposi.w.h;
        spring->follow_x_offset = d2;
        spring->follow_y_offset = -24;
        spring->parent_index = (Uint16)(pActwk - actwk);
        pNewActwk->userflag.b.h = 2;
    }
}

static void belttask(sprite_status *pActwk) {
    sprite_status *pPlayerwk;
    Sint32 ld0;
    Sint16 d0;
    Uint8 bd1;
    dai_rd5_work *work = dai_rd5_work_get(pActwk);

    if (!(work->flags & 1))
        return;
    if (!(pActwk->cddat & 8))
        return;

    pPlayerwk = &actwk[0];

    if (pPlayerwk->actno == 0)
        return;

    if (!(pPlayerwk->cddat & 8))
        return;
    d0 = player_work_get(pPlayerwk)->ride_actor_index;

    if (d0 != (Sint16)(pActwk - actwk))
        return;

    bd1 = time_flag;
    ld0 = 32768;
    if (bd1) {
        ld0 = 65536;
        --bd1;
        if (bd1) {
            ld0 = 0x20000;
        }
    }
    if (colrevflag) {
        ld0 = -ld0;
    }
    pPlayerwk->xposi.l += ld0;
}
