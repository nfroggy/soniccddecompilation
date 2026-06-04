#include "../equ.h"
#include "swblk4.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../ridechk.h"

typedef struct {
    Sint16 switch_index;
    Sint16 origin_y;
    Sint16 linked_block_index;
    Sint16 origin_x;
    Uint8 follow_dx;
    Uint8 follow_dy;
    Uint8 is_secondary;
} swblk4_work;

static swblk4_work *swblk4_get_work(sprite_status *pActwk) {
    return (swblk4_work *)pActwk->actfree;
}

#if defined(R41A) || defined(R42A)
#define SPRITE_SWBLK4_BASE 438
#else
#define SPRITE_SWBLK4_BASE 472
#endif

static void switchr4_init(sprite_status *pActwk);
static void switchr4_move(sprite_status *pActwk);
static Uint32 switchr4_colichk(sprite_status *pActwk, sprite_status *pPlayerwk);
static void swblkr4_init(sprite_status *pActwk);
static void swblkr4_move(sprite_status *pActwk);

static sprite_pattern switchr4_pat0 = {1, {{-16, -12, 0, SPRITE_SWBLK4_BASE}}};
static sprite_pattern switchr4_pat1 = {1,
                                       {{-16, -4, 0, SPRITE_SWBLK4_BASE + 1}}};
sprite_pattern *switchr4pat[2] = {&switchr4_pat0, &switchr4_pat1};
static void (*switchr4_jmp_tbl[2])(sprite_status *) = {&switchr4_init,
                                                       &switchr4_move};
static sprite_pattern swblkr4_pat0 = {1,
                                      {{-32, -64, 0, SPRITE_SWBLK4_BASE + 2}}};
static sprite_pattern swblkr4_pat1 = {1,
                                      {{-16, -64, 0, SPRITE_SWBLK4_BASE + 3}}};
static sprite_pattern swblkr4_pat2 = {1,
                                      {{-16, -32, 0, SPRITE_SWBLK4_BASE + 4}}};
sprite_pattern *swblkr4pat[3] = {&swblkr4_pat0, &swblkr4_pat1, &swblkr4_pat2};
static void (*swblkr4_jmp_tbl[2])(sprite_status *) = {&swblkr4_init,
                                                      &swblkr4_move};

void switchr4(sprite_status *pActwk) {
    swblk4_work *work = swblk4_get_work(pActwk);
    sprite_status *pRideact;
    Sint16 actidx;

    switchr4_jmp_tbl[pActwk->r_no0 / 2](pActwk);
    ride_on_chk(pActwk, &actwk[0]);
    if ((actidx = work->linked_block_index) != 0) {
        pRideact = &actwk[actidx];
        pActwk->xposi.w.h =
            (Sint16)(char)work->follow_dx + pRideact->xposi.w.h;
        pActwk->yposi.w.h =
            (Sint16)(char)work->follow_dy + pRideact->yposi.w.h;
    }

    actionsub(pActwk);
    frameout_s00(pActwk, work->origin_x);
}

static void switchr4_init(sprite_status *pActwk) {
    swblk4_work *work = swblk4_get_work(pActwk);

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 1290;
    pActwk->sprpri = 3;
    pActwk->patbase = switchr4pat;

    if (!work->origin_x) {
        work->origin_x = pActwk->xposi.w.h;
    }

    pActwk->sprhsize = 14;
    pActwk->sprvsize = 5;

    switchr4_move(pActwk);
}

static void switchr4_move(sprite_status *pActwk) {
    if (switchr4_colichk(pActwk, &actwk[0]) != 0) {

        pActwk->userflag.b.h |= 1;
    } else {
        pActwk->userflag.b.h &= -2;
    }

    if (pActwk->userflag.b.h) {
        pActwk->patno = 1;
    } else {
        pActwk->patno = 0;
    }
}

static Uint32 switchr4_colichk(sprite_status *pActwk,
                               sprite_status *pPlayerwk) {
    Sint16 szwk;
    Sint16 poswk;

    szwk = (Sint16)(char)pActwk->sprhsize + 8;
    poswk = szwk + (pPlayerwk->xposi.w.h - pActwk->xposi.w.h);
    if (poswk < 0 || poswk >= szwk * 2) {
        return 0;
    }

    poswk = (Sint16)(char)pPlayerwk->sprvsize + pPlayerwk->yposi.w.h;
    szwk = pActwk->yposi.w.h - 16 - poswk;
    if (szwk > 0 || szwk < -16) {
        return 0;
    }

    return 1;
}

void swblkr4(sprite_status *pActwk) {
    swblk4_work *work = swblk4_get_work(pActwk);
    Sint16 actidx, xwk, hwk;
    sprite_status *pRideact;

    swblkr4_jmp_tbl[pActwk->r_no0 / 2](pActwk);
    ride_on_chk(pActwk, &actwk[0]);

    if (work->is_secondary) {
        if ((actidx = work->linked_block_index) != 0) {
            pRideact = &actwk[actidx];
            if (pRideact->actno != 48) {

                frameout(pActwk);
                return;
            }

            pActwk->xposi.w.h = pRideact->xposi.w.h - 32;
            pActwk->yposi.w.h = pRideact->yposi.w.h + 32;
        }
    }

    actionsub(pActwk);

    xwk = work->origin_x & -128;
    hwk = scra_h_posit.w.h - 128 & -128;
    if ((Uint16)(xwk - hwk) > 640) {
        frameout(pActwk);
    }
}

static void swblkr4_init(sprite_status *pActwk) {
    swblk4_work *work = swblk4_get_work(pActwk);
    sprite_status *pNewact;

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 17514;
    pActwk->sprpri = 3;
    pActwk->patbase = swblkr4pat;

    if (!work->is_secondary) {
        work->origin_x = pActwk->xposi.w.h;
        work->origin_y = pActwk->yposi.w.h;
        pActwk->xposi.w.h += 16;
        pActwk->sprhsize = 16;
        pActwk->sprvsize = 64;
        pActwk->patno = 1;

        if (actwkchk2(pActwk, &pNewact) != 0) {
            frameout(pActwk);
            return;
        }

        pNewact->actno = pActwk->actno;
        swblk4_get_work(pNewact)->origin_x = work->origin_x;

        swblk4_get_work(pNewact)->origin_y = work->origin_y;

        pNewact->sprhsize = 16;
        pNewact->sprvsize = 32;
        swblk4_get_work(pNewact)->is_secondary = 1;
        pNewact->yposi.w.h = pActwk->yposi.w.h + 32;
        pNewact->xposi.w.h = pActwk->xposi.w.h - 32;
        swblk4_get_work(pNewact)->linked_block_index =
            (Sint16)(pActwk - actwk);
        work->linked_block_index = (Sint16)(pNewact - actwk);
        pNewact->patno = 2;

        if (actwkchk2(pActwk, &pNewact) != 0) {
            frameout(pActwk);
            return;
        }

        pNewact->actno = 49;
        swblk4_get_work(pNewact)->linked_block_index =
            (Sint16)(pActwk - actwk);
        work->switch_index = (Sint16)(pNewact - actwk);
        swblk4_get_work(pNewact)->follow_dy = 188;
        swblk4_get_work(pNewact)->origin_x = work->origin_x;

        swblk4_get_work(pNewact)->origin_y = work->origin_y;
    }

    swblkr4_move(pActwk);
}

static void swblkr4_move(sprite_status *pActwk) {
    swblk4_work *work = swblk4_get_work(pActwk);
    Sint32 ysv;
    sprite_status *pRideact;

    if (work->is_secondary)
        return;

    ysv = pActwk->yposi.l;

    if (pActwk->yspeed.w < 0) {

        pActwk->yposi.w.h -= 64;
        if (emycol_u(pActwk) < 0) {
            pActwk->yspeed.w = 0;
        }
    } else if (emycol_d(pActwk) < 0) {
        pActwk->yspeed.w = 0;
    }

    pActwk->yposi.l = ysv;
    pActwk->yposi.l += pActwk->yspeed.w << 8;
    pRideact = &actwk[work->linked_block_index];

    if (pRideact->actno == 48) {
        pRideact->yspeed.w = pActwk->yspeed.w;
    }

    pRideact = &actwk[work->switch_index];

    if (pRideact->actno != 49)
        return;

    if (pRideact->userflag.b.h & 3) {

        if ((pActwk->yspeed.w -= 8) < -256) {
            pActwk->yspeed.w = -256;
        }
    } else {
        if ((pActwk->yspeed.w += 8) > 256) {
            pActwk->yspeed.w = 256;
        }
    }
}
