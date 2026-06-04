#include "../equ.h"
#include "batta.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../suicide.h"

#if defined(R61B) || defined(R62B)
#define SPRITE_BATTA_BASE 477
#else
#define SPRITE_BATTA_BASE 483
#endif

void (*batta_tbl[4])(sprite_status *) = {&batta_init, &batta_down, &batta_wait,
                                         &batta_up};
sprite_pattern ep_batta0 = {1, {{-16, -18, 0, SPRITE_BATTA_BASE}}};
sprite_pattern ep_batta1 = {1, {{-16, -29, 0, SPRITE_BATTA_BASE + 1}}};
sprite_pattern *pat_batta_e[2] = {&ep_batta0, &ep_batta1};
sprite_pattern bp_batta0 = {1, {{-16, -18, 0, SPRITE_BATTA_BASE + 2}}};
sprite_pattern bp_batta1 = {1, {{-16, -29, 0, SPRITE_BATTA_BASE + 3}}};
sprite_pattern *pat_batta_b[2] = {&bp_batta0, &bp_batta1};

typedef struct {
    Sint32 x_velocity;
    Sint32 y_velocity;
    Sint8 hop_count;
    Uint8 hop_count_reset;
    Sint16 wall_probe_offset;
    Uint8 facing_flag;
    Sint16 wait_timer;
    Sint16 collision_delta;
} batta_work;

static batta_work *batta_work_get(sprite_status *pActwk) {
    return (batta_work *)pActwk->actfree;
}

void batta(sprite_status *pActwk) {
    if (enemy_suicide(pActwk))
        return;
    batta_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s(pActwk);
}

void batta_init(sprite_status *pActwk) {
    batta_work *work = batta_work_get(pActwk);

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->sproffset = 9272;
    pActwk->sprhs = 16;
    pActwk->sprhsize = 16;
    work->y_velocity = 458752;
    work->facing_flag = 255;
    work->wall_probe_offset = -16;
    pActwk->patno = 1;

    batta_patexg(pActwk);
    if (pActwk->userflag.b.h == 0) {
        pActwk->patbase = pat_batta_e;
        work->x_velocity = -40960;
        work->hop_count = 7;
        work->hop_count_reset = 7;
    } else {
        pActwk->patbase = pat_batta_b;
        work->x_velocity = -32768;
        work->hop_count = 3;
        work->hop_count_reset = 3;
    }

    ++work->hop_count;
}

Sint16 emylr_jump(sprite_status *pActwk, Uint8 byD3) {
    batta_work *work = batta_work_get(pActwk);

    if (work->facing_flag == 255)
        return emycol_r(pActwk, byD3);
    return emycol_l(pActwk, byD3);
}

void batta_down(sprite_status *pActwk) {
    batta_work *work = batta_work_get(pActwk);
    Sint16 iD1, iD3;

    pActwk->xposi.l += work->x_velocity;
    pActwk->yposi.l += work->y_velocity;

    iD1 = emycol_d(pActwk);
    work->collision_delta = iD1;
    iD3 = work->wall_probe_offset;
    iD1 = emylr_jump(pActwk, iD3);
    if (iD1 < 0) {
        if (work->collision_delta >= 0) {
            batta_wall(pActwk);
            return;
        }
        if (iD1 <= work->collision_delta)
            batta_wall(pActwk);
        else
            batta_floor(pActwk);
        return;
    }

    if (work->collision_delta < 0) {
        batta_floor(pActwk);
        return;
    }
    work->y_velocity += 8192;
    if (work->y_velocity >= 458752)
        work->y_velocity = 458752;
}

void batta_floor(sprite_status *pActwk) {
    batta_work *work = batta_work_get(pActwk);

    pActwk->r_no0 += 2;
    pActwk->yposi.w.h += work->collision_delta;
    if (pActwk->userflag.b.h == 0)
        work->wait_timer = 1;
    else
        work->wait_timer = 20;
}

void batta_wait(sprite_status *pActwk) {
    batta_work *work = batta_work_get(pActwk);
    Sint16 iD6;

    if (pActwk->userflag.b.h != 0) {
        iD6 = 7;
        switch (work->wait_timer) {
        case 11:
        case 18:
            iD6 = -iD6;
        case 6:
        case 15:
            pActwk->yposi.w.h += iD6;
            batta_patexg(pActwk);
            break;
        }
    }

    --work->wait_timer;
    if (work->wait_timer == 0) {
        pActwk->r_no0 += 2;
        pActwk->yposi.w.h -= 7;
        batta_patexg(pActwk);
        if (pActwk->userflag.b.h == 0)
            work->y_velocity = -393216;
        else
            work->y_velocity = -327680;
        --work->hop_count;
        if (work->hop_count < 0)
            batta_wall(pActwk);
    }
}

void batta_up(sprite_status *pActwk) {
    batta_work *work = batta_work_get(pActwk);
    Sint16 iD1, iD3;

    pActwk->xposi.l += work->x_velocity;
    pActwk->yposi.l += work->y_velocity;
    iD1 = emycol_u(pActwk);
    work->collision_delta = iD1;
    iD3 = work->wall_probe_offset;
    iD1 = emylr_jump(pActwk, iD3);
    if (iD1 < 0) {
        if (work->collision_delta >= 0 || iD1 <= work->collision_delta) {

            batta_wall(pActwk);
        } else
            batta_ceiling(pActwk);
        return;
    }

    if (work->collision_delta < 0) {
        batta_ceiling(pActwk);
        return;
    }
    work->y_velocity += 8192;
    if (work->y_velocity >= 0)
        batta_fall(pActwk);
}

void batta_ceiling(sprite_status *pActwk) {
    batta_work *work = batta_work_get(pActwk);

    pActwk->yposi.w.h -= work->collision_delta;
    work->y_velocity = 0;
    batta_fall(pActwk);
}

void batta_fall(sprite_status *pActwk) {
    pActwk->r_no0 -= 4;
    pActwk->yposi.w.h -= 11;
    batta_patexg(pActwk);
}

void batta_wall(sprite_status *pActwk) {
    batta_work *work = batta_work_get(pActwk);

    work->hop_count = work->hop_count_reset;
    pActwk->actflg ^= 1;
    pActwk->cddat ^= 1;
    work->x_velocity = -work->x_velocity;
    work->wall_probe_offset = -work->wall_probe_offset;
    if (work->facing_flag == 255)
        work->facing_flag = 1;
    else
        work->facing_flag = 255;
}

void batta_patexg(sprite_status *pActwk) {
    if (pActwk->patno != 0) {
        pActwk->patno = 0;
        pActwk->sprvsize = 19;
        pActwk->colino = 47;
    } else {
        pActwk->patno = 1;
        pActwk->sprvsize = 28;
        pActwk->colino = 48;
    }
}
