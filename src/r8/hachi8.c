#include <stddef.h>

#include "../equ.h"
#include "hachi8.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"
#include "../suicide.h"

extern void patchg(sprite_status *patchgwk, Uint8 **pat_dat);

#pragma pack(push, 1)
typedef struct {
    union {
        Sint32 x_speed;
        struct {
            Sint16 ball_angle;
            Sint16 ball_angle_speed;
        };
    };
    union {
        Sint16 turn_timer_reset;
        Sint16 ball_angle_step;
    };
    union {
        Sint16 turn_timer;
        Sint16 chain_count;
    };
    union {
        struct {
            Sint16 wave_phase;
            Sint16 wave_delta;
            Sint16 wave_shift;
            Sint16 origin_x;
            Sint16 origin_y;
            Sint16 first_ball_index;
            Sint16 second_ball_index;
        };
        struct {
            Uint8 chain_link_indices[10];
            Uint8 unused18[2];
            Sint16 parent_index;
        };
    };
} hachi8_work;
#pragma pack(pop)

_Static_assert(offsetof(hachi8_work, x_speed) == 0,
               "hachi8_work.x_speed offset");
_Static_assert(offsetof(hachi8_work, ball_angle) == 0,
               "hachi8_work.ball_angle offset");
_Static_assert(offsetof(hachi8_work, ball_angle_speed) == 2,
               "hachi8_work.ball_angle_speed offset");
_Static_assert(offsetof(hachi8_work, turn_timer_reset) == 4,
               "hachi8_work.turn_timer_reset offset");
_Static_assert(offsetof(hachi8_work, turn_timer) == 6,
               "hachi8_work.turn_timer offset");
_Static_assert(offsetof(hachi8_work, chain_link_indices) == 8,
               "hachi8_work.chain_link_indices offset");
_Static_assert(offsetof(hachi8_work, wave_phase) == 8,
               "hachi8_work.wave_phase offset");
_Static_assert(offsetof(hachi8_work, wave_delta) == 10,
               "hachi8_work.wave_delta offset");
_Static_assert(offsetof(hachi8_work, wave_shift) == 12,
               "hachi8_work.wave_shift offset");
_Static_assert(offsetof(hachi8_work, origin_x) == 14,
               "hachi8_work.origin_x offset");
_Static_assert(offsetof(hachi8_work, origin_y) == 16,
               "hachi8_work.origin_y offset");
_Static_assert(offsetof(hachi8_work, first_ball_index) == 18,
               "hachi8_work.first_ball_index offset");
_Static_assert(offsetof(hachi8_work, second_ball_index) == 20,
               "hachi8_work.second_ball_index offset");
_Static_assert(offsetof(hachi8_work, parent_index) == 20,
               "hachi8_work.parent_index offset");
_Static_assert(sizeof(hachi8_work) <= sizeof(((sprite_status *)0)->actfree),
               "hachi8_work fits in actfree");

static hachi8_work *hachi8_get_work(sprite_status *actionwk) {
    return (hachi8_work *)actionwk->actfree;
}

static void ball(sprite_status *actionwk);
static void ball_move(sprite_status *actionwk);

static char p00[4] = {2, 0, 1, -1};
static char *pchg[1] = {p00};
static sprite_pattern body_pat0 = {1, {{-19, -20, 0, 469}}};
static sprite_pattern body_pat1 = {1, {{-19, -20, 0, 470}}};
static sprite_pattern kusari_00 = {1, {{-8, -8, 0, 471}}};
static sprite_pattern ball_pat0 = {1, {{-12, -12, 0, 472}}};
static sprite_pattern kusari_01 = {1, {{-8, -8, 0, 473}}};
static sprite_pattern ball_pat1 = {1, {{-12, -12, 0, 474}}};
sprite_pattern *pat_hachi8_e[4] = {&body_pat0, &body_pat1, &kusari_00,
                                   &ball_pat0};
sprite_pattern *pat_hachi8_b[4] = {&body_pat0, &body_pat1, &kusari_01,
                                   &ball_pat1};

void hachi8(sprite_status *actionwk) {
    if (actionwk->userflag.b.l == 0) {
        body(actionwk);
        return;
    }
    if (actionwk->userflag.b.l > 0) {
        kusari(actionwk);
        return;
    }
    ball(actionwk);
}

void body(sprite_status *actionwk) {
    hachi8_work *work = hachi8_get_work(actionwk);
    sprite_status *a1;

    if (enemy_suicide(actionwk) == -1)
        return;

    if (actionwk->r_no0 != 0) {
        body_move(actionwk);
        return;
    }

    actionwk->r_no0 += 2;
    actionwk->actflg |= 4;
    actionwk->sprpri = 3;
    actionwk->sprhs = 16;
    actionwk->sprhsize = 16;
    actionwk->sprvsize = 16;
    actionwk->colino = 46;
    actionwk->sproffset = 9301;
    work->origin_x = actionwk->xposi.w.h;
    work->origin_y = actionwk->yposi.w.h;
    work->turn_timer = 0;
    if (actionwk->userflag.b.h == 0) {
        actionwk->patbase = pat_hachi8_e;
        work->x_speed = -65536;
        work->turn_timer_reset = 128;
        work->wave_delta = 1024;
        work->wave_shift = 3;
    } else {
        actionwk->patbase = pat_hachi8_b;
        work->x_speed = -32768;
        work->turn_timer_reset = 256;
        work->wave_delta = 768;
        work->wave_shift = 4;
    }

    if (actwkchk2(actionwk, &a1) != 0) {
        frameout(actionwk);
        return;
    }
    a1->actno = actionwk->actno;
    work->first_ball_index = a1 - actwk;
    hachi8_get_work(a1)->parent_index = actionwk - actwk;
    a1->actflg = actionwk->actflg;
    a1->sproffset = actionwk->sproffset;
    a1->patbase = actionwk->patbase;
    a1->userflag.b.h = actionwk->userflag.b.h;
    a1->userflag.b.l = -1;
    a1->xposi.w.h = actionwk->xposi.w.h;
    a1->yposi.w.h = actionwk->yposi.w.h;

    if (actionwk->userflag.b.h != 0) {
        body_move(actionwk);
        return;
    }
    if (actwkchk2(actionwk, &a1) != 0) {
        frameout(actionwk);
        return;
    }

    a1->actno = actionwk->actno;
    work->second_ball_index = a1 - actwk;
    hachi8_get_work(a1)->parent_index = actionwk - actwk;
    a1->actflg = actionwk->actflg;
    a1->sproffset = actionwk->sproffset;
    a1->patbase = actionwk->patbase;
    a1->userflag.b.h = actionwk->userflag.b.h;
    a1->userflag.b.l = -2;
    a1->xposi.w.h = actionwk->xposi.w.h;
    a1->yposi.w.h = actionwk->yposi.w.h;
}

void body_move(sprite_status *actionwk) {
    hachi8_work *work = hachi8_get_work(actionwk);
    Uint16 d0, sin, cos;
    Sint32 d0l;
    Sint16 t;

    actionwk->xposi.l += work->x_speed;
    --work->turn_timer;
    t = work->turn_timer;
    if (t < 0) {
        work->turn_timer = work->turn_timer_reset;

        work->x_speed = -work->x_speed;

        actionwk->actflg ^= 1;
        actionwk->cddat ^= 1;
    }

    actionwk->yposi.w.h = work->origin_y;
    actionwk->yposi.w.l = 0;
    work->wave_phase += work->wave_delta;

    d0 = work->wave_phase >> 8;
    sinset(d0, (Sint16 *)&sin, (Sint16 *)&cos);
    d0l = sin << 16;
    d0l >>= work->wave_shift;
    actionwk->yposi.l += d0l;
    patchg(actionwk, (Uint8 **)pchg);
    actionsub(actionwk);
    frameout_s00(actionwk, work->origin_x);
}

static void ball(sprite_status *actionwk) {
    hachi8_work *work = hachi8_get_work(actionwk);
    sprite_status *a1;
    Sint16 i, d6;

    if (actionwk->r_no0 != 0) {
        ball_move(actionwk);
        return;
    }

    actionwk->r_no0 += 2;
    actionwk->sprhs = 12;
    actionwk->sprhsize = 12;
    actionwk->sprvsize = 12;
    actionwk->sprpri = 2;
    actionwk->patno = 3;
    actionwk->colino = 175;
    if (actionwk->userflag.b.l == -1) {
        work->ball_angle = 2048;
        work->ball_angle_step = 8;
        work->chain_count = 9;
    } else {
        work->ball_angle = 4096;
        work->ball_angle_step = 8;
        work->chain_count = 3;
    }

    d6 = work->chain_count;

    for (i = 0; i <= d6; ++i) {
        if (actwkchk2(actionwk, &a1) != 0) {

            frameout(actionwk);
            return;
        }

        a1->actno = actionwk->actno;
        work->chain_link_indices[i] = a1 - actwk;
        hachi8_get_work(a1)->parent_index = (Uint16)work->parent_index;

        a1->actflg = actionwk->actflg;
        a1->sproffset = actionwk->sproffset;
        a1->patbase = actionwk->patbase;
        a1->userflag.b.h = actionwk->userflag.b.h;
        a1->userflag.b.l = 1;
        a1->xposi.w.h = actionwk->xposi.w.h;
        a1->yposi.w.h = actionwk->yposi.w.h;
    }
    ball_move(actionwk);
}

static void ball_move(sprite_status *actionwk) {
    hachi8_work *work = hachi8_get_work(actionwk);
    int_union d2, d3;
    Sint16 d0, d6, i, ano;
    Uint16 sin, cos;
    Sint32 d4, d5;

    work->ball_angle_speed += work->ball_angle_step;

    d6 = work->ball_angle;
    work->ball_angle += work->ball_angle_speed;

    if ((work->ball_angle_step >= 0 && (Uint16)work->ball_angle_speed < 16384 &&
         (Uint16)work->ball_angle >= 16384) ||
        (work->ball_angle_step < 0 && (Uint16)work->ball_angle_speed >= 16384 &&
         (Uint16)work->ball_angle < 16384)) {
        work->ball_angle_step = -work->ball_angle_step;
    }

    d6 = work->chain_count;
    ano = (Uint16)work->parent_index;
    d5 = actwk[ano].yposi.l + 786432;
    d4 = actwk[ano].xposi.l;
    d0 = work->ball_angle >> 8;
    sinset(d0, (Sint16 *)&sin, (Sint16 *)&cos);
    d3.l = sin;
    d2.l = cos;
    d3.l = (Uint32)d3.l >> 16 & 65535 | d3.l << 16 & -65536;
    d2.l = (Uint32)d2.l >> 16 & 65535 | d2.l << 16 & -65536;
    d3.w.l = 0;
    d2.w.l = 0;
    d3.l >>= 5;
    d2.l >>= 5;

    for (i = 0; i <= d6; ++i) {
        d5 += d3.l;
        d4 += d2.l;
        ano = work->chain_link_indices[i];
        actwk[ano].yposi.l = d5;
        actwk[ano].xposi.l = d4;
    }
    d5 += d3.l;
    d4 += d2.l;
    actionwk->yposi.l = d5;
    actionwk->xposi.l = d4;
    kusari_move(actionwk);
}

void kusari(sprite_status *actionwk) {
    if (actionwk->r_no0 != 0) {
        kusari_move(actionwk);
        return;
    }

    actionwk->r_no0 += 2;
    actionwk->sprhs = 8;
    actionwk->sprhsize = 8;
    actionwk->sprvsize = 8;
    actionwk->sprpri = 3;
    actionwk->patno = 2;
    kusari_move(actionwk);
}

void kusari_move(sprite_status *actionwk) {
    Sint16 ano;

    ano = (Uint16)hachi8_get_work(actionwk)->parent_index;
    if (actwk[ano].actno != 46) {
        frameout(actionwk);
        return;
    }
    actionsub(actionwk);
}
