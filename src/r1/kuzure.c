#include "../equ.h"
#include "kuzure.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../ridechk.h"

extern Uint8 *tblb[];
extern sprite_pattern *patbase_kuzure_b[];
extern Uint8 *tbla[];
extern sprite_pattern *patbase_kuzure_a[];
extern sprite_pattern *patc[];
extern sprite_pattern *patd[];

typedef struct {
    Sint16 wait_timer;
    Sint32 y_velocity;
    Uint8 top_piece;
    Uint8 source_actno;
} kuzure_work;

static kuzure_work *kuzure_work_get(sprite_status *pActwk) {
    return (kuzure_work *)pActwk->actfree;
}

void kuzureru_tikei(sprite_status *pActwk) {
    void (*act_tbl[4])(sprite_status *) = {&acta_init, &acta_check, &actb_wait,
                                           &actb_down};

    act_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    if (pActwk->r_no0 < 4)
        frameout_s(pActwk);
}

void acta_init(sprite_status *pActwk) {
    sprite_pattern **pPB_kuzure;
    ushort_union id_no;
    Uint8 **pTbltbl;
    Uint8 *pTbl;
    char userflag;
    char block_x;
    char block_y;

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->sproffset = 17598;

    pPB_kuzure = patbase_kuzure_a;
    pTbltbl = tbla;
    userflag = pActwk->userflag.b.h;
    if (userflag < 0) {
        pPB_kuzure = patbase_kuzure_b;
        pTbltbl = tblb;
    }
    pActwk->patbase = pPB_kuzure;
    if (userflag & 16) {
        pActwk->actflg |= 1;
        pActwk->cddat |= 1;
    }
    id_no.b.h = 0;
    id_no.b.l = (Uint8)userflag & 15;
    pActwk->patno = id_no.b.l;
    pTbl = pTbltbl[id_no.w];

    block_x = pTbl++[0];
    block_x = (block_x + 1) << 3;
    pActwk->sprhs = block_x;
    pActwk->sprhsize = block_x;

    block_y = pTbl++[0];
    if (block_y < 0)
        block_y = -block_y;
    block_y = ((block_y + 1) << 3) + 2;
    pActwk->sprvsize = block_y;
}

void acta_check(sprite_status *pActwk) {
    sprite_status *pPlayerwk;

    pPlayerwk = &actwk[0];
    if (ridechk(pActwk, pPlayerwk)) {
        ride_on_clr(pActwk, pPlayerwk);
        soundset(163);
        pActwk->r_no0 += 2;
        if (pActwk->userflag.b.h < 0)
            actb_init_b(pActwk);
        else
            actb_init_a(pActwk);
    }
}

void actb_wait(sprite_status *pActwk) {
    kuzure_work *work = kuzure_work_get(pActwk);
    sprite_status *pPlayerwk;

    --work->wait_timer;
    if (work->wait_timer == 0) {
        pActwk->r_no0 += 2;
    }
    if (work->top_piece) {
        pPlayerwk = &actwk[0];
        if (ridechk(pActwk, pPlayerwk)) {
            if (work->wait_timer == 0) {
                ride_on_clr(pActwk, pPlayerwk);
            }
        }
    }
}

void actb_down(sprite_status *pActwk) {
    kuzure_work *work = kuzure_work_get(pActwk);
    sprite_status *pPlayerwk;
    Sint16 yposi;

    pActwk->yposi.l += work->y_velocity;
    work->y_velocity += 16384;

    yposi = pActwk->yposi.w.h;
    pPlayerwk = &actwk[0];
    yposi -= pPlayerwk->yposi.w.h;
    if (yposi > 512)
        frameout(pActwk);
}

void actb_init_a(sprite_status *pActwk) {
    kuzure_work *work = kuzure_work_get(pActwk);
    sprite_status *pActwk_w;
    kuzure_work *piece_work;
    sprite_status *pPlayerwk;
    Uint8 **pTbltbl;
    Uint8 *pTbla;
    char patno;
    char userflag;
    Sint16 time_x;
    Sint16 time_y;
    Sint16 posi_x;
    Sint16 posi_y;
    Sint16 posi_x_start;
    Sint16 posi_x_step;
    Sint16 reverse_flag;
    Sint16 count0x;
    short_union count_x;
    short_union count_y;
    short_union tmp;

    userflag = pActwk->userflag.b.h;
    if (userflag & 16)
        reverse_flag = 1;
    else
        reverse_flag = 0;
    pTbltbl = tbla;
    pTbla = pTbltbl[userflag & 15];
    tmp.w = 0;
    tmp.b.l = pTbla++[0];
    count0x = tmp.w;
    tmp.w *= 8;
    time_x = -16;
    if (reverse_flag == 0) {
        tmp.w *= -1;
        time_x *= -1;
    }
    tmp.w += pActwk->xposi.w.h;
    posi_x_start = tmp.w;
    posi_x_step = time_x;
    count_y.w = 0;
    count_y.b.l = pTbla++[0];
    posi_y = count_y.w;
    posi_y *= 8;
    posi_y += pActwk->yposi.w.h;
    time_y = 9;
    work->source_actno = pActwk->actno;
    do {
        count_x.w = count0x;
        posi_x = posi_x_start;
        time_x = time_y;
        do {
            if (actwkchk(&pActwk_w) != 0) {
                pPlayerwk = &actwk[0];
                if (ridechk(pActwk, pPlayerwk))
                    ride_on_clr(pActwk, pPlayerwk);
                frameout(pActwk);
                return;
            }
            patno = pTbla++[0];
            if (patno >= 0) {
                pActwk_w->patno = patno;
                pActwk_w->actflg |= 4;
                pActwk_w->sprpri = 3;
                pActwk_w->sproffset = 17598;
                pActwk_w->patbase = patc;
                piece_work = kuzure_work_get(pActwk_w);
                piece_work->y_velocity = 0x20000;
                pActwk_w->actno = work->source_actno;
                pActwk_w->r_no0 = pActwk->r_no0;
                if (reverse_flag) {
                    pActwk_w->actflg |= 1;
                    pActwk_w->cddat |= 1;
                }
                if (count_y.w == 0) {
                    piece_work->top_piece = 255;
                    pActwk_w->sprhs = 8;
                    pActwk_w->sprhsize = 8;
                    pActwk_w->sprvsize = 9;
                }
                pActwk_w->yposi.w.h = posi_y;
                pActwk_w->xposi.w.h = posi_x;
                piece_work->wait_timer = time_x;
            }
            posi_x += posi_x_step;
            time_x += 12;
        } while (--count_x.w >= 0);

        posi_y -= 16;
        time_y += 5;
    } while (--count_y.w >= 0);

    frameout(pActwk);
}

void actb_init_b(sprite_status *pActwk) {
    kuzure_work *work = kuzure_work_get(pActwk);
    sprite_status *pActwk_w;
    kuzure_work *piece_work;
    sprite_status *pPlayerwk;
    Uint8 **pTbltbl;
    Uint8 *pTblb;
    char userflag;
    Uint8 sprvsize;
    Sint16 time_w;
    Sint16 d3;
    Sint16 d4;
    short_union d5;
    Sint16 d6;
    Sint16 xspeed;

    userflag = pActwk->userflag.b.h;
    pTbltbl = tblb;
    pTblb = tblb[userflag & 31];
    d5.b.l = pTblb++[0];
    sprvsize = pTblb++[0];
    ++sprvsize;
    sprvsize = sprvsize * 8;
    sprvsize += 2;
    d5.w &= 255;
    d4 = d5.w;
    d4 <<= 3;
    d4 *= -1;
    d3 = 16;
    d6 = 1;
    if ((userflag & 64) == 0) {
        userflag <<= 2;
        if (userflag < 0) {
            pTblb += d5.w;
            d4 *= -1;
            d3 *= -1;
            d6 *= -1;
        }
    } else {
        pPlayerwk = &actwk[0];
        xspeed = pPlayerwk->xspeed.w;
        if (userflag & 32)
            xspeed *= -1;
        if (xspeed < 0) {
            pTblb += d5.w;
            d4 *= -1;
            d3 *= -1;
            d6 *= -1;
        }
    }
    d4 += pActwk->xposi.w.h;

    time_w = 9;
    work->source_actno = pActwk->actno;
    do {
        if (actwkchk(&pActwk_w) != 0) {
            pPlayerwk = &actwk[0];
            if (ridechk(pActwk, pPlayerwk))
                ride_on_clr(pActwk, pPlayerwk);
            frameout(pActwk);
            return;
        }
        pActwk_w->sprpri = 3;
        pActwk_w->sproffset = 17598;
        pActwk_w->actflg |= 4;
        pActwk_w->patbase = patd;
        piece_work = kuzure_work_get(pActwk_w);
        piece_work->y_velocity = 0x20000;
        pActwk_w->actno = work->source_actno;
        pActwk_w->r_no0 = pActwk->r_no0;
        pActwk_w->yposi.w.h = pActwk->yposi.w.h;
        piece_work->top_piece = 255;
        pActwk_w->sprhs = 8;
        pActwk_w->sprhsize = 8;
        pActwk_w->sprvsize = sprvsize;

        pActwk_w->patno = pTblb[0];
        pTblb += d6;
        pActwk_w->xposi.w.h = d4;
        d4 += d3;
        piece_work->wait_timer = time_w;
        time_w += 12;
    } while (--d5.w >= 0);

    frameout(pActwk);
}
