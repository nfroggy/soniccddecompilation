#include "../equ.h"
#include "rollplat.h"
#include "../action.h"
#include "../actset.h"
#include "../playsub.h"

static void act_init(sprite_status *banwk);
static void act_move(sprite_status *banwk);
static void act_check(sprite_status *banwk);

typedef struct {
    Sint16 spin_timer;
    Uint16 initial_child_index;
    Sint16 sequence_step;
    Uint16 active_child_index;
    Uint16 previous_child_index;
    Uint16 oldest_child_index;
    Uint16 parent_index;
    Uint8 touch_latch;
    Uint8 initial_trigger;
} rollplat_work;

static rollplat_work *rollplat_get_work(sprite_status *banwk) {
    return (rollplat_work *)banwk->actfree;
}

static Uint8 pchg00[6] = {3, 1, 2, 3, 0, 255};
static Uint8 pchg01[6] = {3, 3, 2, 1, 0, 255};
Uint8 *pchg[2] = {pchg00, pchg01};
static sprite_pattern kpat00 = {1, {{-16, -8, 0, 316}}};
static sprite_pattern kpat01 = {1, {{-16, -16, 0, 317}}};
static sprite_pattern kpat02 = {1, {{-16, -16, 0, 318}}};
static sprite_pattern kpat03 = {1, {{-16, -16, 0, 319}}};
sprite_pattern *kaitenban_pat[4] = {&kpat00, &kpat01, &kpat02, &kpat03};

void kaitenban(sprite_status *banwk) {
    void (*act_tbl[2])(sprite_status *) = {&act_init, &act_move};

    act_tbl[banwk->r_no0 / 2](banwk);
    actionsub(banwk);
    frameout_s(banwk);
}

static void act_init(sprite_status *banwk) {
    rollplat_work *work = rollplat_get_work(banwk);
    sprite_status *new_actwk;

    banwk->r_no0 += 2;
    banwk->actflg |= 4;
    banwk->sprpri = 3;
    banwk->sprhsize = 16;
    banwk->sprvsize = 16;

    banwk->patbase = kaitenban_pat;
    work->sequence_step = -2;
    if (banwk->userflag.b.h == -1) {
        work->spin_timer = 128;
        work->touch_latch = work->initial_trigger = 255;
    }
    if (actwkchk(&new_actwk) == 0) {
        work->initial_child_index = new_actwk - actwk;
        rollplat_get_work(new_actwk)->parent_index = banwk - actwk;
        new_actwk->actno = banwk->actno;
        new_actwk->userflag.b.h = 1;
        new_actwk->xposi.w.h = 896;
        new_actwk->yposi.w.h = 1272;
    }
    act_move(banwk);
}

static void act_move(sprite_status *banwk) {
    rollplat_work *work = rollplat_get_work(banwk);

    act_check(banwk);

    if (work->spin_timer != 0) {
        --work->spin_timer;
        patchg(banwk, pchg);
    }
}

static void act_check(sprite_status *banwk) {
    rollplat_work *work = rollplat_get_work(banwk);
    Sint16 tbl[3] = {1176, 1200, 1224};
    Sint16 cal_d0;
    sprite_status *new_actwk;

    if ((Uint16)(banwk->yposi.w.h - actwk[0].yposi.w.h + 16) >= 32) {
        work->touch_latch = 0;
        return;
    }
    if ((Uint16)(banwk->xposi.w.h - actwk[0].xposi.w.h + 16) >= 32) {
        work->touch_latch = 0;
        return;
    }
    if (work->touch_latch != 0)
        return;
    work->touch_latch = 255;
    if (actwk[0].yspeed.w >= 0)
        banwk->mstno.w = 255;
    else
        banwk->mstno.w = 511;

    cal_d0 = work->spin_timer;
    work->spin_timer = 128;
    if (cal_d0 != 0)
        return;

    if (work->sequence_step != 4) {
        work->sequence_step += 2;
        work->oldest_child_index = work->previous_child_index;

        work->previous_child_index = work->active_child_index;

        if (actwkchk(&new_actwk) == 0) {
            work->active_child_index = new_actwk - actwk;
            rollplat_get_work(new_actwk)->parent_index = banwk - actwk;
            new_actwk->actno = banwk->actno;
            new_actwk->xposi.w.h = 896;
            new_actwk->yposi.w.h = tbl[work->sequence_step / 2];
        }

        if (work->sequence_step == 4)
            actwk[work->initial_child_index].r_no0 = 4;
        return;
    }

    work->sequence_step = -2;
    actwk[work->active_child_index].r_no0 = 4;
    actwk[work->previous_child_index].r_no0 = 4;
    actwk[work->oldest_child_index].r_no0 = 4;
    actwk[work->initial_child_index].r_no0 = 6;
}
