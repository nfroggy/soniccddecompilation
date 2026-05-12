#include "../equ.h"
#include "yago.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../suicide.h"
#include "playsub4.h"

#if defined(R41A)
#define SPRITE_YAGO_BASE 490
#elif defined(R41B)
#define SPRITE_YAGO_BASE 451
#elif defined(R41C)
#define SPRITE_YAGO_BASE 468
#elif defined(R41D)
#define SPRITE_YAGO_BASE 458
#elif defined(R42A)
#define SPRITE_YAGO_BASE 496
#elif defined(R42B)
#define SPRITE_YAGO_BASE 449
#else
#define SPRITE_YAGO_BASE 453
#endif

static void act_disp(sprite_status *pActwk);
static void yago_e(sprite_status *pActwk);
static void act_init(sprite_status *pActwk);
static void act_fall(sprite_status *pActwk);
static void act_lr(sprite_status *pActwk);
static void act_rev(sprite_status *pActwk);
static void act_rev1(sprite_status *pActwk);
static void act_jet(sprite_status *pActwk);
static void act_jet1(sprite_status *pActwk);
static void act_jet2(sprite_status *pActwk);
static void act_fall1(sprite_status *pActwk);
static void yago_b(sprite_status *pActwk);
static void act_init_(sprite_status *pActwk);
static void act_fall_(sprite_status *pActwk);
static void act_jet1_(sprite_status *pActwk);
static void act_jet2_(sprite_status *pActwk);

static Uint8 pat_chg0[4] = {9, 0, 1, 255};
static Uint8 *pat_chg[1] = {pat_chg0};
static sprite_pattern spr_yago_e_00 = {1,
                                       {{-24, -12, 0, SPRITE_YAGO_BASE + 2}}};
static sprite_pattern spr_yago_e_01 = {1,
                                       {{-24, -12, 0, SPRITE_YAGO_BASE + 3}}};
static sprite_pattern spr_yago_b_00 = {1, {{-24, -12, 0, SPRITE_YAGO_BASE}}};
static sprite_pattern spr_yago_b_01 = {1,
                                       {{-24, -12, 0, SPRITE_YAGO_BASE + 1}}};
sprite_pattern *pat_yago_e[2] = {&spr_yago_e_00, &spr_yago_e_01};
sprite_pattern *pat_yago_b[2] = {&spr_yago_b_00, &spr_yago_b_01};

void yago(sprite_status *pActwk) {
    if (!pActwk->userflag.b.h)
        yago_e(pActwk);
    else
        yago_b(pActwk);
}

static void act_disp(sprite_status *pActwk) {
    patchg(pActwk, pat_chg);
    actionsub(pActwk);
    frameout_s00(pActwk, ((Sint16 *)pActwk)[33]);
}

static void yago_e(sprite_status *pActwk) {
    static void (*act_tbl[9])(sprite_status *) = {
        &act_init, &act_fall, &act_lr,   &act_rev,  &act_rev1,
        &act_jet,  &act_jet1, &act_jet2, &act_fall1};

    if (!enemy_suicide(pActwk))
        act_tbl[pActwk->r_no0 / 2](pActwk);
}

static void act_init(sprite_status *pActwk) {
    ((Sint16 *)pActwk)[33] = pActwk->xposi.w.h;
    pActwk->r_no0 += 2;
    pActwk->actflg = 4;
    pActwk->colino = 47;
    pActwk->sprpri = 1;
    pActwk->sprhsize = 24;
    pActwk->sprvsize = 12;

    pActwk->sproffset = 9200;
    pActwk->patbase = pat_yago_e;
    act_fall(pActwk);
}

static void act_fall(sprite_status *pActwk) {
    pActwk->yposi.l += 65536;
    if (emycol_d(pActwk) - 5 >= 0) {
        act_disp(pActwk);
    } else {

        pActwk->r_no0 += 2;
        *(Sint32 *)&pActwk->actfree[0] = 5242880;
        *(Sint32 *)&pActwk->actfree[4] = -16384;
        act_disp(pActwk);
    }
}

static void act_lr(sprite_status *pActwk) {
    Sint16 temp;

    *(Sint32 *)&pActwk->actfree[0] += -16384;
    if (*(Sint32 *)&pActwk->actfree[0] >= 0) {
        pActwk->xposi.l += *(Sint32 *)&pActwk->actfree[4];
        pActwk->yposi.w.h += emycol_d(pActwk) - 5;
        if (pActwk->actflg & 1)
            temp = emycol_r(pActwk, pActwk->sprhs);
        else
            temp = emycol_l(pActwk, -(char)pActwk->sprhs);
        if (temp - 5 >= 0) {
            temp = pActwk->xposi.w.h - actwk[0].xposi.w.h;
            if (pActwk->actflg & 1)
                temp *= -1;

            if ((Uint16)temp > 80) {
                temp -= 16;
                temp -= 64;
                act_disp(pActwk);
                return;
            }

            temp -= 16;
            temp -= 64;

            pActwk->r_no0 += 6;
            ((Sint16 *)pActwk)[27] = 7;
            act_disp(pActwk);
            return;
        }
    }

    pActwk->r_no0 += 2;
    ((Sint16 *)pActwk)[27] = 31;
    act_disp(pActwk);
}

static void act_rev(sprite_status *pActwk) {
    --((Sint16 *)pActwk)[27];
    if (((Sint16 *)pActwk)[27]) {
        act_disp(pActwk);
        return;
    }

    pActwk->r_no0 += 2;
    ((Sint16 *)pActwk)[27] = 31;
    *(Sint32 *)&pActwk->actfree[0] = 5242880;
    *(Sint32 *)&pActwk->actfree[4] *= -1;
    pActwk->actflg ^= 1;
    pActwk->cddat ^= 1;
    act_disp(pActwk);
}

static void act_rev1(sprite_status *pActwk) {
    --((Sint16 *)pActwk)[27];
    if (((Sint16 *)pActwk)[27]) {
        act_disp(pActwk);
        return;
    }

    pActwk->r_no0 -= 4;
    act_disp(pActwk);
}

static void act_jet(sprite_status *pActwk) {
    --((Sint16 *)pActwk)[27];
    if (((Sint16 *)pActwk)[27]) {
        act_disp(pActwk);
        return;
    }

    pActwk->r_no0 += 2;
    ((Sint16 *)pActwk)[27] = 31;
    act_disp(pActwk);
}

static void act_jet1(sprite_status *pActwk) {
    pActwk->yposi.l += 8192;
    --((Sint16 *)pActwk)[27];
    if (((Sint16 *)pActwk)[27]) {
        act_disp(pActwk);
        return;
    }

    pActwk->r_no0 += 2;
    ((Sint32 *)pActwk)[15] = -294912;
    if (pActwk->actflg & 1)
        ((Sint32 *)pActwk)[15] *= -1;

    ((Sint32 *)pActwk)[14] = -98304;
    act_disp(pActwk);
}

static void act_jet2(sprite_status *pActwk) {
    Sint16 temp;

    pActwk->xposi.l += ((Sint32 *)pActwk)[15];
    if (pActwk->actflg & 1)
        temp = emycol_r(pActwk, pActwk->sprhs);
    else
        temp = emycol_l(pActwk, -(char)pActwk->sprhs);
    if (temp - 5 >= 0) {
        pActwk->yposi.l += ((Sint32 *)pActwk)[14];
        if (((Sint32 *)pActwk)[14] < 0)
            ((Sint32 *)pActwk)[14] += 8192;

        frameout_s(pActwk);
        act_disp(pActwk);
        return;
    }

    pActwk->r_no0 = 6;

    ((Sint16 *)pActwk)[27] = 31;
    act_disp(pActwk);
}

static void act_fall1(sprite_status *pActwk) {
    Sint16 temp;

    pActwk->yposi.l += 65536;
    if ((temp = emycol_d(pActwk) - 5) >= 0) {
        act_disp(pActwk);
        return;
    }

    pActwk->r_no0 = 6;
    pActwk->yposi.w.h += temp;
    ((Sint16 *)pActwk)[27] = 31;
    act_disp(pActwk);
}

static void yago_b(sprite_status *pActwk) {
    static void (*act_tbl[9])(sprite_status *) = {
        &act_init_, &act_fall_, &act_lr,    &act_rev,  &act_rev1,
        &act_jet,   &act_jet1_, &act_jet2_, &act_fall1};

    if (!enemy_suicide(pActwk))
        act_tbl[pActwk->r_no0 / 2](pActwk);
}

static void act_init_(sprite_status *pActwk) {
    ((Sint16 *)pActwk)[33] = pActwk->xposi.w.h;
    pActwk->r_no0 += 2;
    pActwk->actflg = 4;
    pActwk->colino = 47;
    pActwk->sprpri = 1;
    pActwk->sprhsize = 24;
    pActwk->sprvsize = 12;

    pActwk->sproffset = 9200;
    pActwk->patbase = pat_yago_b;
    act_fall_(pActwk);
}

static void act_fall_(sprite_status *pActwk) {
    Sint16 temp;

    pActwk->yposi.l += 65536;
    if ((temp = emycol_d(pActwk)) - 5 >= 0) {
        act_disp(pActwk);
    } else {

        pActwk->r_no0 += 2;
        pActwk->yposi.w.h += temp - 5;
        *(Sint32 *)&pActwk->actfree[0] = 5242880;
        *(Sint32 *)&pActwk->actfree[4] = -16384;
        act_disp(pActwk);
    }
}

static void act_jet1_(sprite_status *pActwk) {
    pActwk->yposi.l += 10240;
    --((Sint16 *)pActwk)[27];
    if (((Sint16 *)pActwk)[27]) {
        act_disp(pActwk);
        return;
    }

    pActwk->r_no0 += 2;
    ((Sint32 *)pActwk)[15] = -65536;
    if (pActwk->actflg & 1)
        ((Sint32 *)pActwk)[15] *= -1;

    ((Sint32 *)pActwk)[14] = -98304;
    act_disp(pActwk);
}

static void act_jet2_(sprite_status *pActwk) {
    Sint16 temp;

    pActwk->xposi.l += ((Sint32 *)pActwk)[15];
    if (pActwk->actflg & 1)
        temp = emycol_r(pActwk, pActwk->sprhs);
    else
        temp = emycol_l(pActwk, -(char)pActwk->sprhs);
    if (temp >= 0) {
        pActwk->yposi.l += ((Sint32 *)pActwk)[14];
        ((Sint32 *)pActwk)[14] += 4096;
        if (((Sint32 *)pActwk)[14] < 0) {
            act_disp(pActwk);
            return;
        }
        if (emycol_d(pActwk) - 5 < 0) {

            pActwk->r_no0 = 4;
            act_disp(pActwk);
            return;
        }
        act_disp(pActwk);
        return;
    }

    pActwk->r_no0 += 2;

    ((Sint16 *)pActwk)[27] = 31;
    act_disp(pActwk);
}
