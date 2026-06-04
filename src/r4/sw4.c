#include "../equ.h"
#include "sw4.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../ridechk.h"

#if defined(R41A) || defined(R42A)
#define SPRITE_SW4_BASE 438
#elif defined(R41B)
#define SPRITE_SW4_BASE 492
#elif defined(R41C) || defined(R41D)
#define SPRITE_SW4_BASE 476
#elif defined(R42B)
#define SPRITE_SW4_BASE 489
#elif defined(R42C) || defined(R42D)
#define SPRITE_SW4_BASE 486
#else
#define SPRITE_SW4_BASE 472
#endif

typedef struct {
    Sint16 parent_index;
    Sint8 linked_x_offset;
    Sint8 linked_y_offset;
    Sint16 switch_index;
    Uint8 previous_state;
    Uint8 current_state;
} sw4_work;

typedef struct {
    Sint16 origin_x;
} sw4_parent_work;

static sw4_work *sw4_get_work(sprite_status *pActwk) {
    return (sw4_work *)pActwk->actfree;
}

static sw4_parent_work *sw4_get_parent_work(sprite_status *pActwk) {
    return (sw4_parent_work *)pActwk->actfree;
}

static void act_init(sprite_status *pActwk);
static void act_move(sprite_status *pActwk);
static Sint16 hosei(sprite_status *pActwk, sprite_status *pActwk2);

static sprite_pattern pat00 = {1, {{-16, -8, 0, SPRITE_SW4_BASE}}};
static sprite_pattern pat01 = {1, {{-16, -4, 0, SPRITE_SW4_BASE + 1}}};
sprite_pattern *pat_sw[2] = {&pat00, &pat01};

void sw4(sprite_status *pActwk) {
    if (pActwk->r_no0)
        act_move(pActwk);
    else
        act_init(pActwk);
}

static void act_init(sprite_status *pActwk) {
    sw4_work *work = sw4_get_work(pActwk);

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->sprhsize = 16;
    pActwk->sproffset = 1290;
    pActwk->patbase = pat_sw;
    pActwk->sprvsize = 10;

    work->switch_index = (Uint16)pActwk->userflag.b.h;

    work->previous_state = 0;
    work->current_state = 0;

    act_move(pActwk);
}

static void act_move(sprite_status *pActwk) {
    sw4_work *work = sw4_get_work(pActwk);
    sprite_status *pActwk_a1;
    sprite_status *pPlayerwk;
    Uint8 *a4;
    Sint16 d0, d1, d3, d4;
    Sint16 z;

    d0 = work->parent_index;
    if (d0) {
        pActwk_a1 = &actwk[d0];
        d1 = pActwk->xposi.w.h;
        d0 = work->linked_x_offset;
        d0 += pActwk_a1->xposi.w.h;
        pActwk->xposi.w.h = d0;
        d0 -= d1;
        d0 <<= 8;
        pActwk->xspeed.w = d0;
        d0 = work->linked_y_offset;
        d0 += pActwk_a1->yposi.w.h;
        pActwk->yposi.w.h = d0;
    }
    work->previous_state = work->current_state;
    d3 = pActwk->xposi.w.h;
    d4 = pActwk->yposi.w.h;
    pPlayerwk = &actwk[0];
    if (hitchk(pActwk, pPlayerwk)) {
        z = hosei(pActwk, pPlayerwk);
    } else {
        z = 1;
    }
    a4 = &switchflag[work->switch_index];
    if (z == 0)
        work->current_state = 255;
    else
        work->current_state = 0;
    if (z) {
        *a4 &= 127;
    } else {
        hosei(pActwk, pPlayerwk);
        *a4 |= 128;
        *a4 |= 64;
    }

    if (work->previous_state == 0 && work->current_state == 255) {
        if (pActwk->actflg & 128) {
            soundset(191);
        }
        *a4 ^= 32;
        pPlayerwk->yposi.w.h += 8;
        if (work->parent_index) {
            work->linked_y_offset += 4;
        } else {
            pActwk->yposi.w.h += 4;
        }
        pActwk->patno = 1;
        pActwk->sprvsize -= 4;
    }

    if (work->previous_state == 255 && work->current_state == 0) {
        pPlayerwk->yposi.w.h -= 8;
        if (work->parent_index) {
            work->linked_y_offset -= 4;
        } else {
            pActwk->yposi.w.h -= 4;
        }
        pActwk->patno = 0;
        pActwk->sprvsize += 4;
    }
    actionsub(pActwk);
    d0 = work->parent_index;
    if (d0 == 0) {
        frameout_s(pActwk);
        return;
    }
    pActwk_a1 = &actwk[d0];
    d0 = sw4_get_parent_work(pActwk_a1)->origin_x;
    d0 &= -128;
    d1 = scra_h_posit.w.h;
    d1 -= 128;
    d1 &= -128;
    d0 -= d1;
    if ((Uint16)d0 > 640)
        frameout_s0(pActwk);
}

static Sint16 hosei(sprite_status *pActwk, sprite_status *pActwk2) {
    Sint16 d0, d1;
    Sint16 z;

    d0 = pActwk->yposi.w.h;
    d1 = pActwk->sprvsize;
    d0 -= d1;
    d1 = pActwk2->sprvsize;
    d0 -= d1;
    ++d0;
    pActwk2->yposi.w.h = d0;
    if (d0)
        z = 0;
    else
        z = 1;
    return z;
}
