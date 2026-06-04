#include "../equ.h"
#include "udblk6.h"
#include "../action.h"
#include "../actset.h"
#include "../ridechk.h"

#if defined(R61B) || defined(R62B)
#define SPRITE_UDBLK6_BASE 475
#elif defined(R63C) || defined(R63D)
#define SPRITE_UDBLK6_BASE 448
#else
#define SPRITE_UDBLK6_BASE 481
#endif

static sprite_pattern udblk6pat0 = {1, {{-48, -80, 0, SPRITE_UDBLK6_BASE}}};
sprite_pattern *udblk6pat[1] = {&udblk6pat0};

typedef struct {
    Sint16 speed_reset;
    Sint16 origin_y;
    union {
        Sint16 reset_word;
        struct {
            Uint8 phase_timer;
            Uint8 phase_index;
        };
    };
    Uint8 direction;
    Sint16 acceleration;
} udblk6_work;

static udblk6_work *udblk6_get_work(sprite_status *actionwk) {
    return (udblk6_work *)actionwk->actfree;
}

void udblk6(sprite_status *actionwk) {
    switch (actionwk->r_no0) {
    case 0:
        udblk6_init(actionwk);
    case 2:
        udblk6_chk1(actionwk);
        break;
    case 4:
        udblk6_mov1(actionwk);
        break;
    case 6:
        udblk6_chk2(actionwk);
        break;
    case 8:
        udblk6_mov2(actionwk);
        break;
    }
    actionsub(actionwk);
    frameout_s(actionwk);
}

void udblk6_ridechk(sprite_status *actionwk) {
    ride_on_chk(actionwk, &actwk[0]);
}

void udblk6_init(sprite_status *actionwk) {
    udblk6_work *work = udblk6_get_work(actionwk);

    actionwk->r_no0 += 2;
    actionwk->actflg |= 4;
    actionwk->sprpri = 3;
    actionwk->sproffset = 17640;
    actionwk->patbase = udblk6pat;
    actionwk->sprvsize = 80;
    actionwk->sprhsize = 48;
    work->origin_y = actionwk->yposi.w.h;
}

void udblk6_chk1(sprite_status *actionwk) {
    Sint16 d0;

    d0 = actionwk->yposi.w.h - actwk[0].yposi.w.h;
    if (d0 < 0) {
        d0 = -d0;
    }

    if (d0 < 192) {
        d0 = actionwk->xposi.w.h - actwk[0].xposi.w.h;
        if (d0 >= 0) {
            if (d0 < 136) {
                udblk6_get_work(actionwk)->direction = 0;
                udblk6_get_work(actionwk)->reset_word = 0;
                actionwk->r_no0 += 2;
            }
        }
    }

    udblk6_ridechk(actionwk);
}

void udblk6_mov1(sprite_status *actionwk) {
    udblk6_work *work = udblk6_get_work(actionwk);

    work->direction = 0;
    movecnt(actionwk);
    if (work->phase_index == 2)
        actionwk->r_no0 += 2;

    udblk6_ridechk(actionwk);
}

void udblk6_chk2(sprite_status *actionwk) {
    Sint16 d0;

    d0 = actionwk->yposi.w.h - actwk[0].yposi.w.h;
    if (d0 < 0) {
        d0 = -d0;
    }

    if (d0 < 192) {
        d0 = actwk[0].xposi.w.h - actionwk->xposi.w.h;
        if (d0 >= 0) {
            if (d0 >= 136) {
                udblk6_get_work(actionwk)->reset_word = 0;
                actionwk->r_no0 += 2;
            }
        }
    }

    udblk6_ridechk(actionwk);
}

void udblk6_mov2(sprite_status *actionwk) {
    udblk6_work *work = udblk6_get_work(actionwk);

    work->direction = 1;
    movecnt(actionwk);
    if (work->phase_index == 2)
        actionwk->r_no0 = 2;

    udblk6_ridechk(actionwk);
}

void movecnt(sprite_status *actionwk) {
    udblk6_work *work = udblk6_get_work(actionwk);

    Sint16 mvtbl1[6] = {64, 8, 0, 64, -8, 512};

    Sint16 mvtbl2[6] = {64, -8, 0, 64, 8, -512};
    Sint16 *a2;
    Sint16 d0;

    a2 = mvtbl1;
    if (work->direction != 0)
        a2 = mvtbl2;

    if (work->phase_timer == 0) {
        d0 = work->phase_index * 3;
        work->phase_timer = (char)a2[d0];
        work->acceleration = a2[d0 + 1];
        work->speed_reset = a2[d0 + 2];
    } else {

        actionwk->yposi.l += actionwk->yspeed.w << 8;
        actionwk->yspeed.w += work->acceleration;

        if (--work->phase_timer == 0)
            ++work->phase_index;
    }
}
