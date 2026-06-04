#include "../equ.h"
#include "tobidai6.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../loader2.h"
#include "../ridechk.h"

#if defined(R61B) || defined(R62B)
#define SPRITE_TOBIDAI6_BASE 476
#elif defined(R63C) || defined(R63D)
#define SPRITE_TOBIDAI6_BASE 449
#else
#define SPRITE_TOBIDAI6_BASE 482
#endif

typedef struct {
    Uint8 ride_height_adjust;
    Sint16 saved_player_yspeed;
    Sint16 vertical_acceleration;
} tobidai6_work;

static tobidai6_work *tobidai6_get_work(sprite_status *actionwk) {
    return (tobidai6_work *)actionwk->actfree;
}

static sprite_pattern tobidai6pat0 = {1, {{-32, -16, 0, SPRITE_TOBIDAI6_BASE}}};
sprite_pattern *tobidai6pat[1] = {&tobidai6pat0};

void tobidai6(sprite_status *actionwk) {
    switch (actionwk->r_no0) {
    case 0:
        tobidai6_init(actionwk);
    case 2:
        tobidai6_move0(actionwk);
        break;
    case 4:
        tobidai6_move1(actionwk);
        break;
    case 6:
        tobidai6_move2(actionwk);
        break;
    }
    actionsub(actionwk);
    frameout_s(actionwk);
}

void tobidai6_ridechk(sprite_status *actionwk) { yuka_ridechk(actionwk); }

void tobidai6_init(sprite_status *actionwk) {
    actionwk->r_no0 += 2;
    actionwk->actflg |= 4;
    actionwk->sprpri = 3;
    actionwk->sproffset = 17384;
    actionwk->patbase = tobidai6pat;
    actionwk->sprvsize = 16;
    actionwk->sprhsize = 32;
}

void tobidai6_move0(sprite_status *actionwk) {
    tobidai6_work *work = tobidai6_get_work(actionwk);

    if (yuka_chk() == 0) {
        actionwk->yspeed.w = -1536;
        work->vertical_acceleration = 16;
        actionwk->r_no0 += 2;
    }

    tobidai6_ridechk(actionwk);
}

void tobidai6_move1(sprite_status *actionwk) {
    tobidai6_work *work = tobidai6_get_work(actionwk);
    Sint16 d1;

    tobidai6_speedset(actionwk);
    d1 = emycol_u(actionwk);
    if (d1 < 0)
        actionwk->yspeed.w = 0;

    d1 = emycol_d(actionwk);
    if (d1 < 0) {
        if (yuka_chk() == 0) {
            actionwk->yspeed.w = -1536;
            work->vertical_acceleration = 16;
            if (actionwk->actflg & 128) {
                soundset(180);
                goto label1;
            }
        }

        actionwk->yspeed.w = -384;
        work->vertical_acceleration = 16;
        actionwk->r_no0 += 2;
    }
label1:
    tobidai6_ridechk(actionwk);
}

void tobidai6_move2(sprite_status *actionwk) {
    tobidai6_work *work = tobidai6_get_work(actionwk);
    Sint16 d1;

    tobidai6_speedset(actionwk);
    if (actionwk->yspeed.w >= 0) {
        d1 = emycol_d(actionwk);
        if (d1 < 0) {
            actionwk->yspeed.w = 0;
            work->vertical_acceleration = 0;
            actionwk->r_no0 -= 4;
        }
    }

    tobidai6_ridechk(actionwk);
}

void tobidai6_speedset(sprite_status *actionwk) {
    tobidai6_work *work = tobidai6_get_work(actionwk);
    Sint16 d0;

    d0 = actionwk->yspeed.w + work->vertical_acceleration;
    if (d0 >= 0) {
        if (d0 >= 1536)
            d0 = 1536;
    }

    actionwk->yspeed.w = d0;
    actionwk->yposi.l += d0 << 8;
}

Sint32 yuka_chk(void) {
    Uint8 d0;

    if (time_flag < 2) {
        d0 = 60;
        if (time_flag != 0)
            d0 += 30;
        if (clchgcnt[3] == d0)
            return -1;
    }

    return 0;
}

void yuka_ridechk(sprite_status *actionwk) {
    tobidai6_work *work = tobidai6_get_work(actionwk);
    Sint16 d0;

    d0 = actwk[0].yspeed.w;
    if (d0 >= 0)
        goto label1;
    d0 = -d0;
    if (d0 > 1536)
        return;

label1:
    if (actionwk->yspeed.w == 0)
        work->ride_height_adjust = 0;
    else
        work->ride_height_adjust = 4;

    actionwk->sprvsize += work->ride_height_adjust;
    chk(actionwk, &actwk[0]);
    actionwk->sprvsize -= work->ride_height_adjust;
}

void chk(sprite_status *actionwk, sprite_status *pw) {
    tobidai6_work *work = tobidai6_get_work(actionwk);

    work->saved_player_yspeed = pw->yspeed.w;
    if (pw->cddat & 8) {
        if (!(pw->cddat & 2))
            pw->yspeed.w = 0;
    }

    if (ride_on_chk(actionwk, pw)) {
        yuka_ride_on(actionwk, pw);
        return;
    }
    pw->yspeed.w = work->saved_player_yspeed;
}

void yuka_ride_on(sprite_status *actionwk, sprite_status *pw) {
    if (pw->r_no0 >= 6) {
        pw->yspeed.w = 0;
    } else {
        pw->yposi.l = actionwk->yposi.l;
        pw->yposi.w.h -= (Sint16)pw->sprvsize + 16;
        if (actionwk->yspeed.w >= 0) {
            pw->yspeed.w = 0;
            return;
        }

        pw->yspeed.w = actionwk->yspeed.w;
    }
}
