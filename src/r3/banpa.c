#include <stddef.h>

#include "../equ.h"
#include "banpa.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"

static void act_init(sprite_status *bumperwk);
static void act_move(sprite_status *bumperwk);

#pragma pack(push, 1)
typedef struct {
    Sint32 speed;
    Uint16 timer;
    Uint16 interval;
    Uint8 unused8[12];
    Sint16 origin_x;
} banpa_work;
#pragma pack(pop)

_Static_assert(offsetof(banpa_work, speed) == 0, "banpa_work.speed offset");
_Static_assert(offsetof(banpa_work, timer) == 4, "banpa_work.timer offset");
_Static_assert(offsetof(banpa_work, interval) == 6, "banpa_work.interval offset");
_Static_assert(offsetof(banpa_work, origin_x) == 20, "banpa_work.origin_x offset");
_Static_assert(sizeof(banpa_work) <= sizeof(((sprite_status *)0)->actfree),
               "banpa_work fits in actfree");

static banpa_work *banpa_get_work(sprite_status *bumperwk) {
    return (banpa_work *)bumperwk->actfree;
}

static sprite_pattern bmp00 = {1, {{-32, -16, 0, 315}}};
sprite_pattern *banpa_pat[1] = {&bmp00};

void banpa(sprite_status *bumperwk) {
    void (*act_tbl[2])(sprite_status *) = {&act_init, &act_move};

    act_tbl[bumperwk->r_no0 / 2](bumperwk);
    actionsub(bumperwk);
    frameout_s00(bumperwk, banpa_get_work(bumperwk)->origin_x);
}

static void act_init(sprite_status *bumperwk) {
    banpa_work *work = banpa_get_work(bumperwk);

    work->origin_x = bumperwk->xposi.w.h;

    bumperwk->r_no0 += 2;
    bumperwk->patbase = banpa_pat;

    bumperwk->actflg = 4;
    bumperwk->sprhsize = 32;
    bumperwk->sprvsize = 16;
    bumperwk->sprpri = 1;
    bumperwk->colino = 231;

    work->interval = 192;
    work->timer = 96;
    if (!((Uint8)bumperwk->userflag.b.h & 128)) {
        work->interval = 160;
        work->timer = 80;
    }
    if (bumperwk->userflag.b.h & 64)
        work->speed = 65536;
    else
        work->speed = -65536;
}

static void act_move(sprite_status *bumperwk) {
    banpa_work *work = banpa_get_work(bumperwk);

    if (bumperwk->colicnt != 0) {
        if ((char)bumperwk->actflg < 0)
            soundset(181);

        bumperwk->colicnt = 0;

        if ((Uint16)(actwk[0].yposi.w.h - bumperwk->yposi.w.h + 8) < 16) {
            actwk[0].xspeed.w = 0;
        } else {

            if ((Uint16)(actwk[0].xposi.w.h - bumperwk->xposi.w.h + 24) < 48) {
                if (actwk[0].yposi.w.h >= bumperwk->yposi.w.h)
                    actwk[0].yspeed.w = 1792;
                else
                    actwk[0].yspeed.w = -1792;
            } else {

                if (actwk[0].xposi.w.h >= bumperwk->xposi.w.h)
                    actwk[0].xspeed.w = 1267;
                else
                    actwk[0].xspeed.w = -1267;
                if (actwk[0].yposi.w.h >= bumperwk->yposi.w.h)
                    actwk[0].yspeed.w = 1267;
                else
                    actwk[0].yspeed.w = -1267;
            }

            actwk[0].cddat |= 2;
            actwk[0].cddat &= 207;
        }
    }
    if (bumperwk->userflag.b.h < 0)
        bumperwk->xposi.l += work->speed;
    else
        bumperwk->yposi.l += work->speed;

    if (--work->timer == 0) {
        work->timer = work->interval;

        work->speed *= -1;
    }
}
