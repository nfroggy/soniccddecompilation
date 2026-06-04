#include "../equ.h"
#include "okusieso.h"
#include "../action.h"
#include "../actset.h"
#include "../ridechk.h"

typedef struct {
    Sint8 anim_phase;
    Sint8 anim_timer;
    Sint16 parent_index;
    Sint16 base_y;
    Uint8 flags;
} okusieso_work;

static okusieso_work *okusieso_get_work(sprite_status *pActwk) {
    return (okusieso_work *)pActwk->actfree;
}

#if defined(R82)
#define SPRITE_OKUSIESO_BASE 411
#else
#define SPRITE_OKUSIESO_BASE 414
#endif

static void okusieso_init(sprite_status *pActwk);
static void okusieso_move(sprite_status *pActwk);
static void ball_move(sprite_status *pActwk);
static void dai_move(sprite_status *pActwk);

static sprite_pattern spr_sieso_00 = {
    2,
    {{-40, -56, 0, SPRITE_OKUSIESO_BASE},
     {-32, -24, 0, SPRITE_OKUSIESO_BASE + 9}}};
static sprite_pattern spr_sieso_01 = {
    2,
    {{-24, -24, 0, SPRITE_OKUSIESO_BASE + 10},
     {-40, -40, 0, SPRITE_OKUSIESO_BASE + 1}}};
static sprite_pattern spr_sieso_02 = {
    1, {{-24, -24, 0, SPRITE_OKUSIESO_BASE + 2}}};
static sprite_pattern spr_sieso_03 = {
    1, {{-40, -32, 0, SPRITE_OKUSIESO_BASE + 3}}};
static sprite_pattern spr_sieso_04 = {
    1, {{-40, -40, 0, SPRITE_OKUSIESO_BASE + 4}}};
static sprite_pattern spr_ball_00 = {1,
                                     {{-16, -16, 0, SPRITE_OKUSIESO_BASE + 5}}};
static sprite_pattern spr_ball_01 = {1,
                                     {{-24, -24, 0, SPRITE_OKUSIESO_BASE + 6}}};
static sprite_pattern spr_ball_02 = {1,
                                     {{-32, -32, 0, SPRITE_OKUSIESO_BASE + 7}}};
static sprite_pattern spr_dai_00 = {1,
                                    {{-40, -12, 0, SPRITE_OKUSIESO_BASE + 8}}};
sprite_pattern *okusiesopat[9] = {&spr_sieso_00, &spr_sieso_01, &spr_sieso_02,
                                  &spr_sieso_03, &spr_sieso_04, &spr_ball_00,
                                  &spr_ball_01,  &spr_ball_02,  &spr_dai_00};

void okusieso(sprite_status *pActwk) {
    Sint16 wD0, wD1;
    static void (*okusieso_tbl[2])(sprite_status *) = {&okusieso_init,
                                                       &okusieso_move};

    okusieso_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    wD0 = pActwk->xposi.w.h & -128;
    wD1 = scra_h_posit.w.h - 128 & -128;
    if ((Uint16)(wD0 - wD1) > 640)
        frameout(pActwk);
}

static void okusieso_init(sprite_status *pActwk) {
    sprite_status *subActwk;

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->patbase = okusiesopat;
    pActwk->sproffset = 928;
    pActwk->sprpri = 5;
    pActwk->patno = 0;
    pActwk->sprhsize = 40;
    pActwk->sprvsize = 56;
    okusieso_get_work(pActwk)->anim_phase = 1;
    okusieso_get_work(pActwk)->flags = 0;

    if (actwkchk2(pActwk, &subActwk) == 0) {
        subActwk->r_no0 = 2;
        subActwk->actflg |= 4;
        subActwk->patbase = pActwk->patbase;
        subActwk->sproffset = 17312;
        subActwk->sprpri = 6;
        subActwk->patno = 5;
        subActwk->sprhsize = 16;
        subActwk->sprvsize = 16;
        subActwk->yspeed.w = 0;
        okusieso_get_work(subActwk)->flags = 0;
        subActwk->actno = pActwk->actno;
        okusieso_get_work(subActwk)->flags |= 1;
        subActwk->xposi.l = pActwk->xposi.l;
        subActwk->yposi.l = pActwk->yposi.l;
        okusieso_get_work(subActwk)->base_y = pActwk->yposi.w.h;
        okusieso_get_work(subActwk)->parent_index =
            (Sint16)(pActwk - actwk);
    } else
        return;

    if (actwkchk2(pActwk, &subActwk) == 0) {
        subActwk->r_no0 = 2;
        subActwk->actflg |= 4;
        subActwk->patbase = okusiesopat;
        subActwk->sproffset = 928;
        subActwk->sprpri = 3;
        subActwk->patno = 8;
        subActwk->sprhsize = 40;
        subActwk->sprvsize = 12;
        subActwk->yspeed.w = 0;
        okusieso_get_work(subActwk)->flags = 0;
        subActwk->actno = pActwk->actno;
        okusieso_get_work(subActwk)->flags |= 2;
        subActwk->xposi.l = pActwk->xposi.l;
        subActwk->yposi.l = pActwk->yposi.l;
        okusieso_get_work(subActwk)->base_y = pActwk->yposi.w.h;
        subActwk->yposi.w.h = (Uint16)subActwk->yposi.w.h + 65492;
        okusieso_get_work(subActwk)->parent_index =
            (Sint16)(pActwk - actwk);
    }
}

static void okusieso_move(sprite_status *pActwk) {
    okusieso_work *work = okusieso_get_work(pActwk);
    Sint16 wD1;
    Uint8 patno_tbl[4] = {3, 2, 1, 2};

    if (work->flags & 8)
        return;

    if (work->flags & 1) {
        ball_move(pActwk);
        return;
    }
    if (work->flags & 2) {
        dai_move(pActwk);
        return;
    }
    if (!(work->flags & 16))
        return;

    if (time_stop)
        return;

    pActwk->yspeed.w = 0;
    if (!(work->flags & 64)) {
        --work->anim_phase;
        if (work->anim_phase < 0) {
            work->anim_timer = 5;
            work->flags |= 64;
            ++pActwk->patno;
        }
        return;
    }

    if (!(work->flags & 32)) {
        --work->anim_timer;
        if (work->anim_timer >= 0)
            return;

        ++pActwk->patno;
        work->anim_timer = 5;
        if (pActwk->patno < 4)
            return;

        work->flags |= 32;
        work->anim_phase = 0;
        work->anim_timer = 10;

        return;
    }

    if (work->flags & 4) {

        if (pActwk->patno != 4) {
            ++pActwk->patno;
            work->anim_timer = 10;
        }
        return;
    }
    --work->anim_timer;
    if (work->anim_timer < 0) {
        ++work->anim_phase;
        wD1 = work->anim_phase;
        if (wD1 == 4) {
            work->anim_phase = 0;
            wD1 = 0;
        }

        pActwk->patno = patno_tbl[wD1];
        work->anim_timer = 10;
    }
}

static void ball_move(sprite_status *pActwk) {
    okusieso_work *work = okusieso_get_work(pActwk);
    Sint16 subact;
    Sint16 wD1;
    Sint32 lD0;

    if (time_stop)
        goto label1;

    subact = work->parent_index;
    if (!(okusieso_get_work(&actwk[subact])->flags & 16))
        goto label1;

    if (!(work->flags & 64)) {
        pActwk->yspeed.w = -12288;
        work->flags |= 64;
        goto label1;
    }

    pActwk->yspeed.w += 256;
    if (pActwk->yspeed.w >= 0) {
        wD1 = pActwk->yposi.w.h - work->base_y;
        if ((Uint16)wD1 <= 8) {
            pActwk->yposi.w.h = work->base_y + 4;
            subact = work->parent_index;
            okusieso_get_work(&actwk[subact])->flags |= 8;
            work->flags |= 8;
            actwk[subact].patno = 4;
            pActwk->yspeed.w = 0;
            goto label1;
        }
    }

    if (pActwk->yspeed.w == 0) {
        ++pActwk->patno;
        pActwk->colino = 164;
        pActwk->sprpri = 4;
        goto label1;
    }

    if (pActwk->yspeed.w == 57344)
        ++pActwk->patno;

label1:
    lD0 = pActwk->yspeed.w;
    lD0 <<= 6;
    pActwk->yposi.l += lD0;
}

static void dai_move(sprite_status *pActwk) {
    okusieso_work *work = okusieso_get_work(pActwk);
    Sint16 subact;
    Sint16 wD1;
    Uint8 speed_tbl[5] = {0, 8, 16, 8, 0};
    Sint16 ofs_tbl[5] = {-44, -28, -12, 12, 28};

    if (time_stop)
        return;

    subact = work->parent_index;
    if (!(okusieso_get_work(&actwk[subact])->flags & 16)) {
        if (ride_on_chk(pActwk, &actwk[0])) {

            subact = work->parent_index;
            okusieso_get_work(&actwk[subact])->flags |= 16;
        }
    } else {

        subact = work->parent_index;
        okusieso_get_work(&actwk[subact])->flags |= 16;
    }

    subact = work->parent_index;
    wD1 = actwk[subact].patno;
    pActwk->yposi.w.h = work->base_y + ofs_tbl[wD1];

    subact = work->parent_index;
    wD1 = actwk[subact].patno;
    wD1 = speed_tbl[wD1];
    wD1 = (Uint16)wD1 << 8;
    pActwk->yspeed.w = wD1;

    okusieso_get_work(&actwk[subact])->flags &= 251;
    if (ride_on_chk(pActwk, &actwk[0]))
        okusieso_get_work(&actwk[subact])->flags |= 4;
}
