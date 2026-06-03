#include <stddef.h>

#include "../equ.h"
#include "harir4.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../player_work.h"
#include "../ridechk.h"
#include "coli4.h"

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
} harir4_work;
#pragma pack(pop)

_Static_assert(offsetof(harir4_work, origin_y) == 8,
               "harir4_work.origin_y offset");
_Static_assert(offsetof(harir4_work, ride_actor_index) == 10,
               "harir4_work.ride_actor_index offset");
_Static_assert(offsetof(harir4_work, origin_x) == 12,
               "harir4_work.origin_x offset");
_Static_assert(offsetof(harir4_work, ride_x_offset) == 14,
               "harir4_work.ride_x_offset offset");
_Static_assert(offsetof(harir4_work, ride_y_offset) == 15,
               "harir4_work.ride_y_offset offset");
_Static_assert(offsetof(harir4_work, wait_timer) == 16,
               "harir4_work.wait_timer offset");
_Static_assert(offsetof(harir4_work, move_offset) == 17,
               "harir4_work.move_offset offset");
_Static_assert(offsetof(harir4_work, moving_back) == 18,
               "harir4_work.moving_back offset");
_Static_assert(sizeof(harir4_work) <= sizeof(((sprite_status *)0)->actfree),
               "harir4_work must fit in sprite_status.actfree");

static harir4_work *harir4_get_work(sprite_status *pActwk) {
    return (harir4_work *)pActwk->actfree;
}

#if defined(R41A) || defined(R42A)
#define SPRITE_HARIR4_BASE 443
#elif defined(R41B) || defined(R41C)
#define SPRITE_HARIR4_BASE 431
#elif defined(R41D)
#define SPRITE_HARIR4_BASE 466
#else
#define SPRITE_HARIR4_BASE 429
#endif

sprite_pattern harir4_pat0 = {1, {{-16, -16, 0, SPRITE_HARIR4_BASE}}};
sprite_pattern harir4_pat1 = {1, {{-16, -16, 0, SPRITE_HARIR4_BASE + 1}}};
sprite_pattern *harir4pat[2] = {&harir4_pat0, &harir4_pat1};

void harir4(sprite_status *pActwk) {
    sprite_status *pRideactwk;
    harir4_work *work = harir4_get_work(pActwk);

    void (*harir4_acttbl[2])(sprite_status *) = {&harir4_init, &harir4_move};

    harir4_acttbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);

    if (work->ride_actor_index) {
        pRideactwk = &actwk[work->ride_actor_index];
        if (!pRideactwk->actno) {
            frameout(pActwk);
            return;
        }

        pActwk->xposi.w.h =
            pRideactwk->xposi.w.h + (Sint16)work->ride_x_offset;
        pActwk->yposi.w.h =
            pRideactwk->yposi.w.h + (Sint16)work->ride_y_offset;
    }

    frameout_s00(pActwk, work->origin_x);
}

void harir4_ridechk(sprite_status *pActwk) {
    player_work *player = player_work_get(&actwk[0]);

    if (pActwk->patno) {
        hitchk(pActwk, &actwk[0]);
        return;
    }

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

void harir4_init(sprite_status *pActwk) {
    harir4_work *work = harir4_get_work(pActwk);

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 928;
    pActwk->sprpri = 4;
    pActwk->patbase = harir4pat;
    if (!work->origin_x) {
        work->origin_x = pActwk->xposi.w.h;
    }
    work->origin_y = pActwk->yposi.w.h;
    pActwk->sprhsize = 18;
    pActwk->sprvsize = 12;
    if (pActwk->userflag.b.h) {
        pActwk->patno = 1;
        pActwk->colino = 131;
    }

    harir4_move(pActwk);
}

void harir4_move(sprite_status *pActwk) {
    harir4_work *work = harir4_get_work(pActwk);

    if (pActwk->userflag.b.h != 1) {
        harir4_sub(pActwk);
        pActwk->yposi.w.h = work->move_offset + work->origin_y;
    }

    harir4_ridechk(pActwk);
}

void harir4_sub(sprite_status *pActwk) {
    harir4_work *work = harir4_get_work(pActwk);

    if (work->wait_timer) {
        if (--work->wait_timer)
            return;
        if (pActwk->actflg & 128)
            soundset(183);
    }

    if (work->moving_back) {

        if (work->move_offset < 8) {
            work->move_offset = 0;
            work->moving_back = 0;
            work->wait_timer = 60;
        } else
            work->move_offset -= 8;
    } else {
        if ((char)(work->move_offset += 8) >= 32) {
            work->move_offset = 32;
            work->moving_back = 1;
            work->wait_timer = 60;
        }
    }
}
