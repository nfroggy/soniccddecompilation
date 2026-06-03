#include <stddef.h>

#include "../equ.h"
#include "haguruma.h"
#include "../actset.h"
#include "../player_work.h"

#pragma pack(push, 1)
typedef struct {
    Uint8 unused0[6];
    Sint16 origin_y;
    Sint16 origin_x;
    Uint8 inner_radius;
    Uint8 unused11;
    Sint16 speed_marker;
    Uint8 radius;
    Uint8 unused15;
    Uint8 engaged;
} haguruma_work;
#pragma pack(pop)

_Static_assert(offsetof(haguruma_work, origin_y) == 6,
               "haguruma_work.origin_y offset");
_Static_assert(offsetof(haguruma_work, origin_x) == 8,
               "haguruma_work.origin_x offset");
_Static_assert(offsetof(haguruma_work, inner_radius) == 10,
               "haguruma_work.inner_radius offset");
_Static_assert(offsetof(haguruma_work, speed_marker) == 12,
               "haguruma_work.speed_marker offset");
_Static_assert(offsetof(haguruma_work, radius) == 14,
               "haguruma_work.radius offset");
_Static_assert(offsetof(haguruma_work, engaged) == 16,
               "haguruma_work.engaged offset");
_Static_assert(sizeof(haguruma_work) <= sizeof(((sprite_status *)0)->actfree),
               "haguruma_work must fit in sprite_status.actfree");

static haguruma_work *haguruma_get_work(sprite_status *wheelwk) {
    return (haguruma_work *)wheelwk->actfree;
}

static void hagurumainit(sprite_status *wheelwk);
static void hagurumamove(sprite_status *wheelwk);
static void hagurumasub(sprite_status *wheelwk);

void haguruma(sprite_status *wheelwk) {
    void (*haguruma_move_tbl[2])(sprite_status *) = {&hagurumainit,
                                                     &hagurumamove};
    haguruma_move_tbl[wheelwk->r_no0 / 2](wheelwk);
}

void hagurumainit(sprite_status *wheelwk) {
    haguruma_work *work = haguruma_get_work(wheelwk);

    wheelwk->r_no0 += 2;
    wheelwk->actflg |= 4;
    work->origin_x = wheelwk->xposi.w.h;
    work->origin_y = wheelwk->yposi.w.h;
    work->inner_radius = 24;
    work->radius = 72;
    if (wheelwk->userflag.b.h & 15) {
        work->inner_radius = 16;
        work->radius = 48;
    }
    work->speed_marker = (Sint16)(wheelwk->userflag.b.h & -16) * 8;

    wheelwk->direc.b.h = wheelwk->cddat << 6 & 192;
    hagurumamove(wheelwk);
}

void hagurumamove(sprite_status *wheelwk) {
    hagurumasub(wheelwk);
    frameout_s(wheelwk);
}

void hagurumasub(sprite_status *wheelwk) {
    Uint16 cal_posi;
    haguruma_work *work = haguruma_get_work(wheelwk);
    player_work *player = player_work_get(&actwk[0]);

    cal_posi = actwk[0].xposi.w.h - work->origin_x + (Sint16)work->radius;

    if (cal_posi < (Uint16)(work->radius * 2)) {
        cal_posi = actwk[0].yposi.w.h - work->origin_y + (Sint16)work->radius;

        if (cal_posi < (Uint16)(work->radius * 2)) {
            if (!(actwk[0].cddat & 2))
                goto label1;

            work->engaged = 0;
            return;
        }
    }
    if (work->engaged)
        player->jump_lock = work->engaged = 0;
    return;

label1:
    if (!work->engaged) {
        work->engaged = 1;
        if (!(actwk[0].cddat & 4))
            actwk[0].mstno.b.h = 0;
        actwk[0].cddat &= 223;
        actwk[0].mstno.b.l = 1;
        player->jump_lock = 1;
    }
    if (work->speed_marker < 0) {
        if (actwk[0].mspeed.w > -1024)
            actwk[0].mspeed.w = -1024;
        else if (actwk[0].mspeed.w < -3840)
            actwk[0].mspeed.w = -3840;
        return;
    }
    if (actwk[0].mspeed.w < 1024)
        actwk[0].mspeed.w = 1024;
    else if (actwk[0].mspeed.w > 3840)
        actwk[0].mspeed.w = 3840;
}
