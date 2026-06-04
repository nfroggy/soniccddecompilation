#include "../equ.h"
#include "tobira4.h"
#include "../action.h"
#include "../actset.h"
#include "../ridechk.h"

typedef struct {
    Uint8 switch_index;
    Uint8 door_type;
    Sint16 origin_y;
    Sint16 origin_x;
    Sint16 player_x;
    Uint8 open_amount;
    Uint8 open_limit;
    Uint8 closing;
    Sint16 player_y;
} tobira4_work;

static tobira4_work *tobira4_get_work(sprite_status *pActwk) {
    return (tobira4_work *)pActwk->actfree;
}

#if defined(R41A)
#define SPRITE_TOBIRA4_BASE 512
#elif defined(R42A)
#define SPRITE_TOBIRA4_BASE 513
#else
#define SPRITE_TOBIRA4_BASE 506
#endif

static sprite_pattern pat0 = {1, {{-8, -64, 0, SPRITE_TOBIRA4_BASE}}};
static sprite_pattern pat1 = {1, {{-8, -32, 0, SPRITE_TOBIRA4_BASE + 1}}};
static sprite_pattern pat2 = {1, {{-64, -8, 0, SPRITE_TOBIRA4_BASE + 2}}};
sprite_pattern *tobira4pat[3] = {&pat0, &pat1, &pat2};

void tobira4(sprite_status *pActwk) {
    void (*tbl[4])(sprite_status *) = {&tobira4_init, &tobira4_move,
                                       &tobira4_chek, &tobira4_clse};

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s(pActwk);
}

void tobira4_init(sprite_status *pActwk) {
    tobira4_work *work = tobira4_get_work(pActwk);
    Uint8 d0, d1;
    Uint8 tbl[20] = {64, 8, 128, 2,  64,  8, 128, 2,  8,   32,
                     64, 1, 8,   64, 128, 0, 8,   64, 128, 0};

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 17472;
    pActwk->sprpri = 2;
    pActwk->patbase = tobira4pat;
    work->origin_x = pActwk->xposi.w.h;
    work->origin_y = pActwk->yposi.w.h;

    d1 = d0 = pActwk->userflag.b.h;
    d1 &= 15;
    work->switch_index = d1;
    d0 >>= 4;
    d0 &= 15;
    work->door_type = d0;
    d0 <<= 2;
    pActwk->sprhsize = tbl[d0];
    pActwk->sprvsize = tbl[d0 + 1];
    work->open_limit = tbl[d0 + 2];
    pActwk->patno = tbl[d0 + 3];
}

void tobira4_move(sprite_status *pActwk) {
    tobira4_work *work = tobira4_get_work(pActwk);
    Uint8 d0;

    d0 = work->switch_index;
    if (!(switchflag[d0] & 128)) {
        work->closing = 255;
    } else {
        work->closing = 0;
    }

    if (work->door_type != 4) {
        if (work->open_amount) {
            work->closing = 0;
        }
    }
    work->player_x = actwk[0].xposi.w.h;
    work->player_y = actwk[0].yposi.w.h;
    tobira4_cnt(pActwk);
    hitchk(pActwk, &actwk[0]);

    if (work->door_type != 4) {
        if (work->open_amount == work->open_limit) {
            pActwk->r_no0 += 2;
        }
    }
}

void cl_type1(sprite_status *pActwk) {
    sprite_status *pPlayerwk;
    Sint16 d0;

    pPlayerwk = &actwk[0];
    d0 = pActwk->yposi.w.h;
    if (d0 < tobira4_get_work(pActwk)->player_y) {
        d0 = pPlayerwk->sprvsize;
        d0 += pPlayerwk->yposi.w.h;
        if (d0 >= pActwk->yposi.w.h)
            return;
        d0 -= pActwk->yposi.w.h;
        d0 *= -1;
        if (d0 < 16)
            return;
    } else {
        d0 = -pPlayerwk->sprvsize;
        d0 += pPlayerwk->yposi.w.h;
        if (d0 < pPlayerwk->yposi.w.h)
            return;
        d0 -= pPlayerwk->yposi.w.h;
        if (d0 < 8)
            return;
    }
    pActwk->r_no0 += 2;
}

void cl_type2(sprite_status *pActwk) {
    sprite_status *pPlayerwk;
    Sint16 d0;

    pPlayerwk = &actwk[0];
    d0 = pActwk->xposi.w.h;
    if (d0 < tobira4_get_work(pActwk)->player_x) {
        d0 = pPlayerwk->sprhs;
        d0 += pPlayerwk->xposi.w.h;
        if (d0 >= pActwk->xposi.w.h)
            return;
        d0 -= pActwk->xposi.w.h;
        d0 *= -1;
        if (d0 < 8)
            return;
    } else {
        d0 = -pPlayerwk->sprhs;
        d0 += pPlayerwk->xposi.w.h;
        if (d0 < pActwk->xposi.w.h)
            return;
        d0 -= pActwk->xposi.w.h;
        if (d0 < 8)
            return;
    }
    pActwk->r_no0 += 2;
}

void tobira4_chek(sprite_status *pActwk) {
    void (*tbl[5])(sprite_status *) = {&cl_type1, &cl_type1, &cl_type2,
                                       &cl_type2, &cl_type2};

    tbl[tobira4_get_work(pActwk)->door_type](pActwk);
    hitchk(pActwk, &actwk[0]);
}

void tobira4_clse(sprite_status *pActwk) {
    tobira4_work *work = tobira4_get_work(pActwk);

    work->closing = 255;
    tobira4_cnt(pActwk);
    if (work->open_amount == 0) {
        pActwk->r_no0 = 2;
    }
    hitchk(pActwk, &actwk[0]);
}

void type1(sprite_status *pActwk) {
    Sint16 d0;

    tobira4_sub(pActwk);
    d0 = (Uint16)tobira4_get_work(pActwk)->open_amount;
    d0 *= -1;
    d0 += tobira4_get_work(pActwk)->origin_x;
    pActwk->xposi.w.h = d0;
}

void type2(sprite_status *pActwk) {
    Sint16 d0;

    tobira4_sub(pActwk);
    d0 = (Uint16)tobira4_get_work(pActwk)->open_amount;
    d0 += tobira4_get_work(pActwk)->origin_x;
    pActwk->xposi.w.h = d0;
}

void type3(sprite_status *pActwk) {
    Sint16 d0;

    tobira4_sub(pActwk);
    d0 = (Uint16)tobira4_get_work(pActwk)->open_amount;
    d0 *= -1;
    d0 += tobira4_get_work(pActwk)->origin_y;
    pActwk->yposi.w.h = d0;
}

void type4(sprite_status *pActwk) {
    Sint16 d0;

    tobira4_sub(pActwk);
    d0 = (Uint16)tobira4_get_work(pActwk)->open_amount;
    d0 += tobira4_get_work(pActwk)->origin_y;
    pActwk->yposi.w.h = d0;
}

void tobira4_cnt(sprite_status *pActwk) {
    void (*tbl[5])(sprite_status *) = {&type1, &type2, &type3, &type4, &type3};

    tbl[tobira4_get_work(pActwk)->door_type](pActwk);
}

void tobira4_sub(sprite_status *pActwk) {
    tobira4_work *work = tobira4_get_work(pActwk);

    if (work->closing) {
        if (work->open_amount >= 8) {
            work->open_amount -= 8;
            return;
        }
        work->open_amount = 0;
    } else {
        work->open_amount += 8;
        if (work->open_amount < work->open_limit)
            return;

        work->open_amount = work->open_limit;
    }
}
