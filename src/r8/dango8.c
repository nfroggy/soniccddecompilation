#include "../equ.h"
#include "dango8.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../etc.h"
#include "../player_work.h"
#include "../playsub.h"
#include "../suicide.h"

typedef struct {
    Sint32 speed;
    Sint32 form;
} dango_work;

static dango_work *dango_get_work(sprite_status *pActwk) {
    return (dango_work *)pActwk->actfree;
}

static void a_init(sprite_status *pActwk);
static void a_fall(sprite_status *pActwk);
static void a_walk(sprite_status *pActwk);
static void a_walk1(sprite_status *pActwk);
static void a_patchg(sprite_status *pActwk);
static void a_walk_stop(sprite_status *pActwk);
static void a_to_roll(sprite_status *pActwk);
static void a_roll_stop(sprite_status *pActwk);
static void a_roll(sprite_status *pActwk);
static void a_roll1(sprite_status *pActwk);
static void a_to_walk(sprite_status *pActwk);
static void a_to_walk1(sprite_status *pActwk);
static void a_speedset(Sint32 new_speed, Sint32 old_speed,
                       sprite_status *pActwk);
static Sint16 a_move(sprite_status *pActwk);

static Uint8 pat_chg_e00[4] = {9, 0, 1, 255};
static Uint8 pat_chg_e01[3] = {0, 0, 252};
static Uint8 pat_chg_e02[5] = {3, 4, 4, 5, 252};
static Uint8 pat_chg_e03[8] = {4, 2, 3, 2, 3, 2, 3, 252};
static Uint8 pat_chg_e04[4] = {4, 2, 3, 255};
static Uint8 pat_chg_e05[5] = {4, 5, 4, 4, 252};
Uint8 *pchg_e[6] = {pat_chg_e00, pat_chg_e01, pat_chg_e02,
                    pat_chg_e03, pat_chg_e04, pat_chg_e05};
static Uint8 pat_chg_b00[4] = {29, 0, 1, 255};
static Uint8 pat_chg_b01[3] = {59, 0, 252};
static Uint8 pat_chg_b02[5] = {9, 4, 4, 5, 252};
static Uint8 pat_chg_b03[14] = {9, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 252};
static Uint8 pat_chg_b04[4] = {9, 2, 3, 255};
static Uint8 pat_chg_b05[5] = {9, 5, 4, 4, 252};
Uint8 *pchg_b[6] = {pat_chg_b00, pat_chg_b01, pat_chg_b02,
                    pat_chg_b03, pat_chg_b04, pat_chg_b05};
static sprite_pattern spr_dango_e_00 = {1, {{-16, -12, 0, 466}}};
static sprite_pattern spr_dango_e_01 = {1, {{-16, -12, 0, 467}}};
static sprite_pattern spr_dango_e_02 = {
    2, {{-5, -12, 0, 471}, {-16, -16, 0, 474}}};
static sprite_pattern spr_dango_e_03 = {
    2, {{-4, -12, 0, 471}, {-16, -16, 0, 474}}};
static sprite_pattern spr_dango_e_04 = {1, {{-16, -16, 0, 472}}};
static sprite_pattern spr_dango_e_05 = {1, {{-16, -15, 0, 473}}};
sprite_pattern *pat_dango_e[6] = {&spr_dango_e_00, &spr_dango_e_01,
                                  &spr_dango_e_02, &spr_dango_e_03,
                                  &spr_dango_e_04, &spr_dango_e_05};
static sprite_pattern spr_dango_b_00 = {1, {{-16, -12, 0, 468}}};
static sprite_pattern spr_dango_b_01 = {1, {{-16, -12, 0, 469}}};
static sprite_pattern spr_dango_b_02 = {1, {{-16, -17, 0, 470}}};
static sprite_pattern spr_dango_b_03 = {
    2, {{-4, -12, 0, 471}, {-16, -16, 0, 474}}};
static sprite_pattern spr_dango_b_04 = {1, {{-16, -16, 0, 472}}};
static sprite_pattern spr_dango_b_05 = {1, {{-16, -15, 0, 473}}};
sprite_pattern *pat_dango_b[6] = {&spr_dango_b_00, &spr_dango_b_01,
                                  &spr_dango_b_02, &spr_dango_b_03,
                                  &spr_dango_b_04, &spr_dango_b_05};

void dango(sprite_status *pActwk) {
    void (*tbl[15])(sprite_status *) = {
        &a_init,   &a_fall,    &a_walk,    &a_walk1,     &a_walk_stop,
        &a_patchg, &a_to_roll, &a_patchg,  &a_roll_stop, &a_roll1,
        &a_roll,   &a_roll1,   &a_to_walk, &a_patchg,    &a_to_walk1};

    if (enemy_suicide(pActwk))
        return;
    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s(pActwk);
}

static void a_init(sprite_status *pActwk) {
    dango_work *work = dango_get_work(pActwk);

    pActwk->r_no0 += 2;
    pActwk->actflg = 4;
    pActwk->sprpri = 3;
    pActwk->sprhs = 14;
    pActwk->sprhsize = 14;
    pActwk->sprvsize = 9;
    pActwk->colino = 44;
    pActwk->sproffset = 9263;
    if (!pActwk->userflag.b.h) {
        pActwk->patbase = pat_dango_e;
        work->form = 0;
    } else {
        pActwk->patbase = pat_dango_b;
        work->form = 1;
    }
}

static void a_fall(sprite_status *pActwk) {
    Sint16 wD1;

    pActwk->yposi.l += 65536;
    wD1 = emycol_d(pActwk);
    if (wD1 < 0) {
        pActwk->yposi.w.h += wD1;
        pActwk->r_no0 += 2;
    }
}

static void a_walk(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    a_speedset(24576, 4096, pActwk);
}

static void a_walk1(sprite_status *pActwk) {
    Sint16 wD0, wD1;
    dango_work *work = dango_get_work(pActwk);

    if (!a_move(pActwk)) {

        work->speed *= -1;

        pActwk->actflg ^= 1;
        pActwk->cddat ^= 1;
        return;
    }
    wD0 = actwk[0].xposi.w.h - pActwk->xposi.w.h;
    if (wD0 >= 0)
        wD1 = 255;
    else
        wD1 = 0;
    if ((Uint16)(wD0 + 128) < 256) {
        wD0 = actwk[0].yposi.w.h - pActwk->yposi.w.h + 128;
        if ((Uint16)wD0 < 256) {
            if (pActwk->actflg & 1)
                wD0 = 255;
            else
                wD0 = 0;
            if (!(wD0 ^ wD1))
                pActwk->r_no0 += 2;
        }
    }
    a_patchg(pActwk);
}

static void a_patchg(sprite_status *pActwk) {
    dango_work *work = dango_get_work(pActwk);

    if (work->form == 0)
        patchg(pActwk, pchg_e);
    else
        patchg(pActwk, pchg_b);
}

static void a_walk_stop(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    ++pActwk->mstno.b.h;
}

static void a_to_roll(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    ++pActwk->mstno.b.h;
    pActwk->yposi.w.h -= 6;
    pActwk->sprvsize = 16;
}

static void a_roll_stop(sprite_status *pActwk) {
    dango_work *work = dango_get_work(pActwk);

    pActwk->r_no0 += 2;
    ++pActwk->mstno.b.h;
    pActwk->yposi.w.h += 3;
    pActwk->sprvsize = 13;
    pActwk->colino = 237;
    work->speed = 0;
}

static void a_roll(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    ++pActwk->mstno.b.h;
    a_speedset(0x40000, 65536, pActwk);
}

static void a_roll1(sprite_status *pActwk) {
    Sint16 wD0, wD1, wD2, sin, cos;
    Sint32 lD0, lD1;
    dango_work *work = dango_get_work(pActwk);
    player_work *player = player_work_get(&actwk[0]);

    if (!pActwk->colicnt) {
        if (a_move(pActwk)) {
            if (work->form == 0)
                patchg(pActwk, pchg_e);
            else
                patchg(pActwk, pchg_b);
        } else {

            pActwk->r_no0 += 2;
        }
    } else {

        wD1 = pActwk->xposi.w.h - actwk[0].xposi.w.h;
        wD2 = pActwk->yposi.w.h - actwk[0].yposi.w.h;
        wD0 = atan_sonic(wD1, wD2);
        sinset(wD0 & 255, &sin, &cos);
        lD1 = cos * -1792;
        lD1 >>= 8;
        actwk[0].xspeed.w = (Uint16)lD1;
        lD0 = sin * -1792;
        lD0 >>= 8;
        actwk[0].yspeed.w = (Uint16)lD0;
        actwk[0].cddat |= 2;
        actwk[0].cddat &= 239;
        actwk[0].cddat &= 223;
        player->jump_started = 0;

        pActwk->colicnt = 0;
        lD1 &= 65535;
        lD1 = -lD1;
        lD1 <<= 8;
        work->speed = lD1;
        pActwk->mstno.b.h = 4;
        pActwk->r_no0 = 22;
    }
}

static void a_to_walk(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    ++pActwk->mstno.b.h;
    pActwk->yposi.w.h -= 3;
    pActwk->sprvsize = 16;
    pActwk->colino = 44;
}

static void a_to_walk1(sprite_status *pActwk) {
    pActwk->r_no0 = 4;
    pActwk->mstno.b.h = 0;
    pActwk->yposi.w.h += 6;
    pActwk->sprvsize = 9;
}

static void a_speedset(Sint32 new_speed, Sint32 old_speed,
                       sprite_status *pActwk) {
    dango_work *work = dango_get_work(pActwk);

    if (pActwk->userflag.b.h)
        new_speed = old_speed;

    if (!(pActwk->actflg & 1))
        new_speed = -new_speed;

    work->speed = new_speed;
}

static Sint16 a_move(sprite_status *pActwk) {
    Sint16 temp1, temp2;
    Sint16 ret;
    dango_work *work = dango_get_work(pActwk);

    pActwk->xposi.l += work->speed;
    if (work->speed >= 0)
        temp1 = emycol_r(pActwk, pActwk->sprhs);
    else {
        temp1 = emycol_l(pActwk, -(char)pActwk->sprhs);
    }
    if (temp1 >= 0) {
        temp1 = emycol_d(pActwk);
        temp2 = temp1 + 7;
        if ((Uint16)temp2 < 14) {
            pActwk->yposi.w.h += temp1;
            ret = -1;
        } else {

            pActwk->xposi.l -= work->speed;
            ret = 0;
        }
    } else {

        pActwk->xposi.l -= work->speed;
        ret = 0;
    }
    return ret;
}
