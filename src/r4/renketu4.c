#include "../equ.h"
#include "renketu4.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"
#include "../ridechk.h"

#if defined(R41C)
#define SPRITE_RENKETU4_BASE 445
#elif defined(R41D)
#define SPRITE_RENKETU4_BASE 431
#elif defined(R42C) || defined(R42D)
#define SPRITE_RENKETU4_BASE 512
#else
#define SPRITE_RENKETU4_BASE 496
#endif

sprite_pattern renketu4_pat0 = {1, {{-32, -8, 0, SPRITE_RENKETU4_BASE}}};
sprite_pattern renketu4_pat1 = {1, {{-8, -8, 0, SPRITE_RENKETU4_BASE + 1}}};
sprite_pattern *renketu4pat[2] = {&renketu4_pat0, &renketu4_pat1};

#pragma pack(push, 1)
typedef struct {
    Uint8 unused0[6];
    Sint16 angular_speed;
    Sint16 origin_y;
    Uint8 unused10[2];
    Sint16 origin_x;
    Uint8 unused14[4];
    Uint8 segment_index;
    Uint8 switch_latch;
    short_union angle;
} renketu4_work;
#pragma pack(pop)

static renketu4_work *get_work(sprite_status *pActwk) {
    return (renketu4_work *)pActwk->actfree;
}

void renketu4(sprite_status *pActwk) {
    renketu4_work *work = get_work(pActwk);
    Uint16 xwk1;
    Uint16 xwk2;
    void (*renketu4_acttbl[2])(sprite_status *) = {&renketu4_init,
                                                   &renketu4_move};

    renketu4_acttbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);

    xwk1 = (Uint16)work->origin_x & 65408;
    xwk2 = (Uint16)scra_h_posit.w.h - 128 & 65408;
    if ((Uint16)(xwk1 - xwk2) > 640) {
        frameout(pActwk);
    }
}

void renketu4_ridechk(sprite_status *pActwk) {
    if (ridechk(pActwk, &actwk[0])) {

        actwk[0].yposi.w.h = pActwk->yposi.w.h - (Sint16)pActwk->sprvsize -
                             (Sint16)actwk[0].sprvsize + 2;
    }
}

void renketu4_init(sprite_status *pActwk) {
    char renketu4_tbl[18] = {32, 0, 0, 0,  -32, 0,   24, 1,   16,
                             1,  8, 1, -8, 1,   -16, 1,  -24, 1};
    renketu4_work *work = get_work(pActwk);
    sprite_status *pNewactwk;
    Uint8 i;

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;

    if (stageno.b.l == 2)
        pActwk->sproffset = 17408;
    else
        pActwk->sproffset = 17216;

    pActwk->patbase = renketu4pat;
    pActwk->sprhsize = 32;
    pActwk->sprvsize = 8;

    if (!work->segment_index) {
        work->origin_x = pActwk->xposi.w.h;
        work->origin_y = pActwk->yposi.w.h;

        for (i = 1; i <= 8; ++i) {
            if (actwkchk(&pNewactwk) == 0) {
                renketu4_work *new_work = get_work(pNewactwk);

                pNewactwk->actno = 62;
                pNewactwk->userflag.b.h = pActwk->userflag.b.h;
                new_work->segment_index = i;
                new_work->origin_x = work->origin_x;
                new_work->origin_y = work->origin_y;
            }
        }
    }

    work->angle.b.h = renketu4_tbl[work->segment_index * 2];
    pActwk->patno = renketu4_tbl[work->segment_index * 2 + 1];

    pActwk->sprpri = 1;
    if (pActwk->patno) {
        pActwk->sprpri = 3;
        pActwk->sproffset -= 16384;
    }

    if (pActwk->userflag.b.h & 16)
        work->angular_speed = -256;
    else
        work->angular_speed = 256;

    renketu4_move(pActwk);
}

void renketu4_move(sprite_status *pActwk) {
    renketu4_work *work = get_work(pActwk);
    Uint16 wD0, wD1;
    int_union lD0, lD1, lD4, lD5;
    Sint16 wk;

    swchk(pActwk);

    work->angle.w += work->angular_speed;

    sinset((Uint8)work->angle.b.h, (Sint16 *)&wD0, (Sint16 *)&wD1);

    lD0.l = (Sint16)wD0 * 6 >> 4;
    lD1.l = (Sint16)wD1 * 6 >> 4;
    wk = lD0.w.h;
    lD0.w.h = lD0.w.l;
    lD0.w.l = wk;
    wk = lD1.w.h;
    lD1.w.h = lD1.w.l;
    lD1.w.l = wk;

    lD4.w.l = lD5.w.l = 0;
    lD4.w.h = work->origin_y;
    lD5.w.h = work->origin_x;
    lD0.l += lD4.l;
    lD1.l += lD5.l;

    pActwk->yspeed.w = (lD0.l - pActwk->yposi.l) << 8 >> 16;
    pActwk->xspeed.w = (lD1.l - pActwk->xposi.l) << 8 >> 16;
    pActwk->yposi.l = lD0.l;
    pActwk->xposi.l = lD1.l;

    if (!pActwk->patno)
        renketu4_ridechk(pActwk);
}

void swchk(sprite_status *pActwk) {
    renketu4_work *work = get_work(pActwk);

    if (!(pActwk->userflag.b.h & 128)) {
        if (switchflag[pActwk->userflag.b.h & 15] & 128) {
            if (work->switch_latch)
                return;
            work->angular_speed *= -1;
            work->switch_latch = 1;
            return;
        }
        work->switch_latch = 0;
    }
}
