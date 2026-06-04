#include "../equ.h"
#include "rblk4.h"
#include "../action.h"
#include "../actset.h"
#include "../fcol.h"
#include "../loader2.h"
#include "../ridechk.h"

#if defined(R42B)
#define SPRITE_RBLK4_BASE 494
#else
#define SPRITE_RBLK4_BASE 506
#endif

static void rblk4_init(sprite_status *pActwk);
static void rblk4_wait(sprite_status *pActwk);
static void rblk4_move(sprite_status *pActwk);
static void rblk4_push(sprite_status *pActwk);
static void rblk4_ract_init(sprite_status *pActwk);
static void rblk4_ract_move(sprite_status *pActwk);

typedef struct {
    Sint16 link_actor_a;
    Sint16 link_actor_b;
    Sint16 origin_x;
    Uint8 timer;
    Uint8 anim_index;
    Uint8 role_marker;
    Uint8 pattern_index;
    Uint8 contact_flag;
    Uint8 latch_flag;
} rblk4_work;

static rblk4_work *rblk4_work_get(sprite_status *pActwk) {
    return (rblk4_work *)pActwk->actfree;
}

static sprite_pattern pat00 = {
    2, {{-8, -8, 0, SPRITE_RBLK4_BASE}, {-48, -48, 0, SPRITE_RBLK4_BASE + 1}}};
static sprite_pattern pat01 = {
    2,
    {{-8, -8, 0, SPRITE_RBLK4_BASE + 2}, {-48, -16, 0, SPRITE_RBLK4_BASE + 3}}};
static sprite_pattern pat02 = {
    2,
    {{-8, -8, 0, SPRITE_RBLK4_BASE + 4}, {-16, -16, 0, SPRITE_RBLK4_BASE + 5}}};
static sprite_pattern pat03 = {
    2,
    {{-8, -8, 0, SPRITE_RBLK4_BASE + 6}, {-16, -48, 0, SPRITE_RBLK4_BASE + 7}}};
static sprite_pattern pat04 = {
    2,
    {{-8, -8, 0, SPRITE_RBLK4_BASE + 8}, {-48, -48, 0, SPRITE_RBLK4_BASE + 9}}};
static sprite_pattern pat05 = {2,
                               {{-8, -8, 0, SPRITE_RBLK4_BASE + 10},
                                {-48, -24, 0, SPRITE_RBLK4_BASE + 11}}};
static sprite_pattern pat06 = {2,
                               {{-8, -8, 0, SPRITE_RBLK4_BASE + 12},
                                {-24, -48, 0, SPRITE_RBLK4_BASE + 13}}};
static sprite_pattern pat07 = {2,
                               {{-8, -8, 0, SPRITE_RBLK4_BASE + 14},
                                {-48, -48, 0, SPRITE_RBLK4_BASE + 15}}};
static sprite_pattern pat08 = {1, {{-32, -32, 0, SPRITE_RBLK4_BASE + 16}}};
sprite_pattern *rblk4pat[9] = {&pat00, &pat01, &pat02, &pat03, &pat04,
                               &pat05, &pat06, &pat07, &pat08};

void rblk4(sprite_status *pActwk) {
    rblk4_work *work = rblk4_work_get(pActwk);
    void (*tbl[4])(sprite_status *) = {&rblk4_init, &rblk4_wait, &rblk4_move,
                                       &rblk4_push};

    if (work->role_marker == 255) {
        rblk4_ract(pActwk);
    } else {
        tbl[pActwk->r_no0 / 2](pActwk);
        actionsub(pActwk);
        frameout_s(pActwk);
    }
}

static void rblk4_init(sprite_status *pActwk) {
    rblk4_work *work = rblk4_work_get(pActwk);
    sprite_status *pNewActwk;

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->patbase = rblk4pat;
    pActwk->sproffset = 17152;
    pActwk->sprvsize = 64;
    pActwk->sprhsize = 64;

    pActwk->patno = pActwk->userflag.b.h & 3;
    work->pattern_index = pActwk->patno;

    if (actwkchk(&pNewActwk) != 0) {
        frameout_s0(pActwk);
        return;
    }
    pNewActwk->actno = 61;
    rblk4_work_get(pNewActwk)->origin_x = pActwk->xposi.w.h;
    rblk4_work_get(pNewActwk)->link_actor_b = (Sint16)(pActwk - actwk);
    rblk4_work_get(pNewActwk)->role_marker = 255;
    rblk4_work_get(pNewActwk)->pattern_index = 0;
    work->link_actor_a = (Sint16)(pNewActwk - actwk);

    if (actwkchk(&pNewActwk) != 0) {
        frameout_s0(pActwk);
        return;
    }
    pNewActwk->actno = 61;
    rblk4_work_get(pNewActwk)->origin_x = pActwk->xposi.w.h;
    rblk4_work_get(pNewActwk)->link_actor_b = (Sint16)(pActwk - actwk);
    rblk4_work_get(pNewActwk)->role_marker = 255;
    rblk4_work_get(pNewActwk)->pattern_index = 1;
    work->link_actor_b = (Sint16)(pNewActwk - actwk);
}

static void rblk4_wait(sprite_status *pActwk) {
    rblk4_work *work = rblk4_work_get(pActwk);
    sprite_status *pActwk_y;
    sprite_status *pPlayerwk;
    char *a4;
    Sint16 d0;
    Sint16 d1;
    Uint8 d3;

    pActwk_y = &actwk[work->link_actor_a];
    if (!(rblk4_work_get(pActwk_y)->contact_flag & 128))
        return;

    pPlayerwk = &actwk[0];
    if (work->pattern_index < 2) {
        d0 = pActwk->xposi.w.h;
        d1 = pPlayerwk->xposi.w.h + (Sint16)(pPlayerwk->sprhsize + 2);
    } else {
        d1 = pActwk->xposi.w.h;
        d0 = pPlayerwk->xposi.w.h + (Sint16) - ((char)pPlayerwk->sprhsize + 2);
    }
    if (d0 < d1)
        return;
    d0 -= d1;

    a4 = dirstk;
    *a4 = 0;
    scdchk(pActwk, pActwk->yposi.w.h + d0, pActwk->xposi.w.h, 16, 0, 13, a4);

    d3 = dirstk[0];
    if (d3 & 1)
        d3 = 0;

    if (d1 >= 0) {
        if (d1 >= 16) {
            pActwk->r_no0 += 2;
            work->timer = 16;
            work->anim_index = 0;
            return;
        }
    }
    pPlayerwk = &actwk[work->link_actor_b];
    if (rblk4_work_get(pPlayerwk)->contact_flag) {
        pActwk->r_no0 = 6;
        work->timer = 16;
        work->anim_index = 0;
    }
}

static void rblk4_move(sprite_status *pActwk) {
    rblk4_work *work = rblk4_work_get(pActwk);
    Uint8 d0;
    Uint8 rkpchg0[16] = {0, 4, 1, 255, 1, 5, 2, 255,
                         2, 5, 1, 255, 3, 6, 2, 255};

    d0 = work->pattern_index;
    d0 *= 4;
    d0 = d0 + work->anim_index;
    d0 = rkpchg0[d0];
    if (d0 & 128) {
        work->pattern_index = pActwk->patno;
        pActwk->r_no0 -= 2;
        return;
    }
    pActwk->patno = d0;
    --work->timer;
    if (work->timer)
        return;

    soundset(191);
    work->timer = 4;
    ++work->anim_index;
}

static void rblk4_push(sprite_status *pActwk) {
    rblk4_work *work = rblk4_work_get(pActwk);
    sprite_status *pPlayerwk;
    Sint16 sd0, sd1;
    Uint8 d0;
    Uint8 pspchg0[16] = {0, 7, 3, 255, 1, 4, 2, 255,
                         2, 4, 1, 255, 3, 7, 0, 255};

    d0 = work->pattern_index;
    d0 *= 4;
    d0 = d0 + work->anim_index;
    d0 = pspchg0[d0];
    if (d0 & 128) {
        work->pattern_index = pActwk->patno;
        pActwk->r_no0 = 2;
        return;
    }
    pActwk->patno = d0;

    pPlayerwk = &actwk[0];
    sd0 = pActwk->yposi.w.h;
    sd1 = pActwk->sprvsize;
    sd0 -= sd1;
    sd1 = pPlayerwk->sprvsize;
    sd0 -= sd1;
    pPlayerwk->yposi.w.h = sd0;

    --work->timer;
    if (work->timer)
        return;

    work->timer = 8;
    ++work->anim_index;
    if (work->anim_index == 1) {
        pPlayerwk->xposi.w.h = pActwk->xposi.w.h;
        return;
    }
    if (work->anim_index == 2) {
        sd0 = 16;
        if (work->pattern_index & 2) {
            sd0 *= -1;
        }
        sd0 += pActwk->xposi.w.h;
        pPlayerwk->xposi.w.h = sd0;
    }
}

void rblk4_ract(sprite_status *pActwk) {
    rblk4_work *work = rblk4_work_get(pActwk);
    void (*tbl[2])(sprite_status *) = {&rblk4_ract_init, &rblk4_ract_move};

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s00(pActwk, work->origin_x);
}

static void rblk4_ract_init(sprite_status *pActwk) {
    rblk4_work *work = rblk4_work_get(pActwk);
    Uint8 d0, d1;

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->patbase = rblk4pat;
    pActwk->sproffset = 17152;
    pActwk->patno = 8;

    d0 = 32;
    d1 = 16;
    if (work->pattern_index) {
        d0 = 16;
        d1 = 16;
    }
    pActwk->sprhsize = d0;
    pActwk->sprvsize = d1;

    rblk4_ract_move(pActwk);
}

static void rblk4_ract_move(sprite_status *pActwk) {
    rblk4_work *work = rblk4_work_get(pActwk);
    sprite_status *pRideact;
    char *a2;
    Uint8 d0;
    char xyofset_tbl1[8] = {-16, 0, -16, 0, 16, 0, 16, 0};
    char xyofset_tbl2[8] = {0, -32, 0, 32, 0, 32, 0, -32};

    pRideact = &actwk[work->link_actor_b];
    d0 = pRideact->patno;
    if (d0 >= 4) {
        ride_on_clr(pActwk, &actwk[0]);
        return;
    }
    d0 = d0 + d0;
    a2 = &xyofset_tbl1[d0];
    if (work->pattern_index) {
        a2 = &xyofset_tbl2[d0];
    }
    pActwk->xposi.w.h = pRideact->xposi.w.h + (Sint16)*a2++;
    pActwk->yposi.w.h = pRideact->yposi.w.h + (Sint16)*a2++;

    hitchk(pActwk, &actwk[0]);
    if (work->pattern_index == 0) {
        if (pActwk->cddat & 8)
            work->contact_flag = 255;
        else
            work->contact_flag = 0;
    } else {
        if (!(pActwk->cddat & 32)) {
            work->contact_flag = 0;
            work->latch_flag = 0;
            return;
        }
        if (work->latch_flag == 0) {
            work->contact_flag = 255;
            work->latch_flag = 255;
            pActwk->cddat &= 223;
        }
    }
}
