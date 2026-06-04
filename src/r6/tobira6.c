#include "../equ.h"
#include "tobira6.h"
#include "../action.h"
#include "../actset.h"
#include "../ridechk.h"

typedef struct {
    Uint8 switch_index;
    Sint16 base_y;
    Sint16 previous_player_x;
    Uint8 open_amount;
    Uint8 close_flag;
    Sint16 previous_player_y;
} tobira6_work;

static tobira6_work *tobira6_get_work(sprite_status *pActwk) {
    return (tobira6_work *)pActwk->actfree;
}

void (*tobira_tbl[4])(sprite_status *) = {&tobira6_init, &tobira6_move,
                                          &tobira6_chek, &tobira6_clse};
sprite_pattern tpat0 = {1, {{-8, -32, 0, 309}}};
sprite_pattern tpat1 = {1, {{-32, -32, 0, 310}}};
sprite_pattern *tobira6pat[2] = {&tpat0, &tpat1};

void tobira6(sprite_status *pActwk) {
    tobira_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s(pActwk);
}

void tobira6_ridechk(sprite_status *pActwk) { ride_on_chk(pActwk, &actwk[0]); }

void tobira6_init(sprite_status *pActwk) {
    tobira6_work *work = tobira6_get_work(pActwk);

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->patbase = tobira6pat;
    work->base_y = pActwk->yposi.w.h;

    pActwk->sproffset = 928;
    pActwk->sprvsize = 32;
    pActwk->sprhsize = 8;
    if (stageno.b.l == 2) {
        pActwk->sproffset = 816;
        pActwk->sprhsize = 32;
        pActwk->patno = 1;
    }

    work->switch_index = pActwk->userflag.b.h & 15;
    work->close_flag = 255;
    tobira6_move(pActwk);
}

void tobira6_move(sprite_status *pActwk) {
    tobira6_work *work = tobira6_get_work(pActwk);
    Uint8 byD0;

    byD0 = work->switch_index;
    if (switchflag[byD0] & 128)
        work->close_flag = 0;

    work->previous_player_x = actwk[0].xposi.w.h;
    work->previous_player_y = actwk[0].yposi.w.h;

    tobira6_cnt(pActwk);
    tobira6_ridechk(pActwk);

    if (work->open_amount == 64)
        pActwk->r_no0 += 2;
}

void tobira6_chek(sprite_status *pActwk) {
    tobira6_work *work = tobira6_get_work(pActwk);
    Sint16 iD0;

    iD0 = pActwk->xposi.w.h - work->previous_player_x;
    if (iD0 < 0) {

        iD0 = actwk[0].sprhs;
        iD0 += actwk[0].xposi.w.h;
        iD0 -= pActwk->xposi.w.h;
        if (iD0 >= 0)
            return;

        iD0 = -iD0;
        if (iD0 < (Sint16)pActwk->sprhsize)
            return;

    } else {

        iD0 = actwk[0].sprhs;
        iD0 = -iD0;
        iD0 += actwk[0].xposi.w.h;
        iD0 -= pActwk->xposi.w.h;
        if (iD0 < 0)
            return;

        if (iD0 < (Sint16)pActwk->sprhsize)
            return;
    }

    pActwk->r_no0 += 2;
}

void tobira6_clse(sprite_status *pActwk) {
    tobira6_work *work = tobira6_get_work(pActwk);

    work->close_flag = 255;
    tobira6_cnt(pActwk);
    if (work->open_amount == 0)
        pActwk->r_no0 = 2;
    tobira6_ridechk(pActwk);
}

void tobira6_cnt(sprite_status *pActwk) {
    tobira6_work *work = tobira6_get_work(pActwk);
    ushort_union wD0;
    Sint16 iD0;

    wD0.w = 0;
    tobira6_sub(pActwk);
    wD0.b.l = work->open_amount;
    iD0 = -wD0.w;
    iD0 += work->base_y;
    pActwk->yposi.w.h = iD0;
}

void tobira6_sub(sprite_status *pActwk) {
    tobira6_work *work = tobira6_get_work(pActwk);

    if (work->close_flag != 0) {
        work->open_amount -= 4;
        if ((Sint8)work->open_amount >= 0)
            return;

        work->open_amount = 0;
        return;
    }

    work->open_amount += 4;
    if (work->open_amount >= 64) {
        work->open_amount = 64;
    }
}
