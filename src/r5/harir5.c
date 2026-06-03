#include <stddef.h>

#include "../equ.h"
#include "harir5.h"
#include "../action.h"
#include "../player_work.h"
#include "../ridechk.h"
#include "coli5.h"

#pragma pack(push, 1)
typedef struct {
    Uint8 unused0[8];
    Sint16 origin_y;
    Sint16 ride_actor_index;
    Sint16 origin_x;
    Uint8 ride_x_offset;
    Uint8 ride_y_offset;
    Uint8 wait_timer;
    Uint8 move_offset;
    Uint8 moving_back;
} harir5_work;
#pragma pack(pop)

_Static_assert(offsetof(harir5_work, origin_y) == 8,
               "harir5_work.origin_y offset");
_Static_assert(offsetof(harir5_work, ride_actor_index) == 10,
               "harir5_work.ride_actor_index offset");
_Static_assert(offsetof(harir5_work, origin_x) == 12,
               "harir5_work.origin_x offset");
_Static_assert(offsetof(harir5_work, ride_x_offset) == 14,
               "harir5_work.ride_x_offset offset");
_Static_assert(offsetof(harir5_work, ride_y_offset) == 15,
               "harir5_work.ride_y_offset offset");
_Static_assert(offsetof(harir5_work, wait_timer) == 16,
               "harir5_work.wait_timer offset");
_Static_assert(offsetof(harir5_work, move_offset) == 17,
               "harir5_work.move_offset offset");
_Static_assert(offsetof(harir5_work, moving_back) == 18,
               "harir5_work.moving_back offset");
_Static_assert(sizeof(harir5_work) <= sizeof(((sprite_status *)0)->actfree),
               "harir5_work must fit in sprite_status.actfree");

static harir5_work *harir5_get_work(sprite_status *pActwk) {
    return (harir5_work *)pActwk->actfree;
}

static void harir3_init(sprite_status *pActwk);
static void harir3_move(sprite_status *pActwk);
static void harir3_kind(sprite_status *pActwk);
static void harir3_ymv1(sprite_status *pActwk);
static void harir3_ymv2(sprite_status *pActwk);
static void harir3_xmv1(sprite_status *pActwk);
static void harir3_xmv2(sprite_status *pActwk);
static void harir3_sub(sprite_status *pActwk);

static sprite_pattern harir5_pat1 = {1, {{-16, -16, 0, 417}}};
static sprite_pattern harir5_pat2 = {1, {{-16, -16, 0, 418}}};
static sprite_pattern harir5_pat3 = {1, {{-16, -16, 0, 419}}};
static sprite_pattern harir5_pat4 = {1, {{-16, -16, 0, 420}}};
sprite_pattern *harir5pat[4] = {&harir5_pat1, &harir5_pat2, &harir5_pat3,
                                &harir5_pat4};

void harir5(sprite_status *pActwk) {
    sprite_status *pRideactwk;
    harir5_work *work = harir5_get_work(pActwk);

    void (*harir3_acttbl[2])(sprite_status *) = {&harir3_init, &harir3_move};

    harir3_acttbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);

    if (work->ride_actor_index) {
        pRideactwk = &actwk[work->ride_actor_index];
        pActwk->xposi.w.h =
            pRideactwk->xposi.w.h + (Sint16)(char)work->ride_x_offset;
        pActwk->yposi.w.h =
            pRideactwk->yposi.w.h + (Sint16)(char)work->ride_y_offset;
    }

    if ((Uint16)((work->origin_x & 65408) -
                 ((Uint16)(scra_h_posit.w.h - 128) & 65408)) > 640) {
        if (pActwk->cdsts) {
            flagwork[time_flag + pActwk->cdsts * 3] &= 127;
        }
        frameout(pActwk);
    }
}

static void harir3_init(sprite_status *pActwk) {
    Uint8 bD0;
    harir5_work *work = harir5_get_work(pActwk);

    Uint8 harir3sztbl[12] = {18, 12, 0, 18, 12, 131, 10, 14, 130, 10, 14, 130};

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 1099;
    pActwk->sprpri = 3;
    pActwk->patbase = harir5pat;

    work->origin_x = pActwk->xposi.w.h;
    work->origin_y = pActwk->yposi.w.h;

    bD0 = pActwk->userflag.b.h & 3;
    pActwk->patno = bD0;
    pActwk->sprhsize = harir3sztbl[bD0 * 3];
    pActwk->sprvsize = harir3sztbl[bD0 * 3 + 1];
    pActwk->colino = harir3sztbl[bD0 * 3 + 2];

    harir3_move(pActwk);
}

static void harir3_move(sprite_status *pActwk) {
    player_work *player = player_work_get(&actwk[0]);

    harir3_kind(pActwk);

    if (pActwk->patno) {
        hitchk(pActwk, &actwk[0]);
    } else {

        if (hitchk(pActwk, &actwk[0])) {
            if (pActwk->cddat & 8) {
                if (!plpower_a) {
                    if (!plpower_m) {
                        if (actwk[0].r_no0 < 4) {
                            if (!player->damage_invulnerability_timer) {
                                actwk[0].yposi.l -= actwk[0].yspeed.w << 8;
                                playdamageset(&actwk[0], pActwk);
                            }
                        }
                    }
                }
            }
        }
    }
}

static void harir3_kind(sprite_status *pActwk) {
    void (*harir3_kndtbl[4])(sprite_status *) = {&harir3_ymv1, &harir3_ymv2,
                                                 &harir3_xmv1, &harir3_xmv2};

    if (pActwk->userflag.b.h & 4)
        harir3_kndtbl[pActwk->userflag.b.h & 3](pActwk);
}

static void harir3_ymv1(sprite_status *pActwk) {
    harir5_work *work = harir5_get_work(pActwk);

    harir3_sub(pActwk);
    pActwk->yposi.w.h = (Sint16)(Uint16)work->move_offset + work->origin_y;
}

static void harir3_ymv2(sprite_status *pActwk) {
    harir5_work *work = harir5_get_work(pActwk);

    harir3_sub(pActwk);
    pActwk->yposi.w.h = -(Sint16)(Uint16)work->move_offset + work->origin_y;
}

static void harir3_xmv1(sprite_status *pActwk) {
    harir5_work *work = harir5_get_work(pActwk);

    harir3_sub(pActwk);
    pActwk->xposi.w.h = -(Sint16)(Uint16)work->move_offset + work->origin_x;
}

static void harir3_xmv2(sprite_status *pActwk) {
    harir5_work *work = harir5_get_work(pActwk);

    harir3_sub(pActwk);
    pActwk->xposi.w.h = (Sint16)(Uint16)work->move_offset + work->origin_x;
}

static void harir3_sub(sprite_status *pActwk) {
    harir5_work *work = harir5_get_work(pActwk);

    if (work->wait_timer) {
        if (--work->wait_timer)
            return;
    }

    if (work->moving_back) {
        if (work->move_offset >= 8) {
            work->move_offset -= 8;
            return;
        }
        work->move_offset = 0;
        work->moving_back = 0;
        work->wait_timer = 60;
    } else {

        work->move_offset += 8;
        if (work->move_offset >= 32) {
            work->move_offset = 32;
            work->moving_back = 1;
            work->wait_timer = 60;
        }
    }
}
