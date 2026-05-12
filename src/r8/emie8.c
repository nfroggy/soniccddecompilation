#include "../equ.h"
#include "emie8.h"
#include "../action.h"
#include "../impfuncs.h"
#include "../playsub.h"

static void emie8_ini(sprite_status *pActwk);
static void emie8_wait(sprite_status *pActwk);
static void emie8_l(sprite_status *pActwk);
static void emie8_j(sprite_status *pActwk);
static void emie8_j2(sprite_status *pActwk);
static void emie8_l2(sprite_status *pActwk);
static void add_spd(sprite_status *pActwk);
static void snc_ctrl(sprite_status *pActwk);
static void emie8_snc_r(sprite_status *pActwk, sprite_status *pPlayerwk);
static void emie8_snc_l(sprite_status *pActwk, sprite_status *pPlayerwk);
static void emie8_snc_w(sprite_status *pActwk, sprite_status *pPlayerwk);
static void emie8_snc_pata(sprite_status *pActwk, sprite_status *pPlayerwk);
static void flash_out8(void);
static void emie8_jisin(sprite_status *pActwk);

static Uint8 pchg0[8] = {3, 0, 1, 2, 3, 4, 5, 255};
Uint8 *emie8_pchg[1] = {pchg0};
static sprite_pattern pat00 = {1, {{-16, -20, 0, 513}}};
static sprite_pattern pat01 = {1, {{-16, -20, 0, 514}}};
static sprite_pattern pat02 = {1, {{-16, -20, 0, 515}}};
static sprite_pattern pat03 = {1, {{-16, -20, 0, 516}}};
static sprite_pattern pat04 = {1, {{-16, -20, 0, 517}}};
static sprite_pattern pat05 = {1, {{-16, -20, 0, 518}}};
static sprite_pattern pat06 = {1, {{-16, -20, 0, 519}}};
static sprite_pattern pat07 = {1, {{-16, -20, 0, 520}}};
sprite_pattern *emie8_pat[8] = {&pat00, &pat01, &pat02, &pat03,
                                &pat04, &pat05, &pat06, &pat07};
static void (*emie8_act_tbl[8])(sprite_status *) = {
    &emie8_ini, &emie8_wait, &emie8_l,  &emie8_j,
    &emie8_j2,  &emie8_j,    &emie8_j2, &emie8_l2};
static void (*snc_act_tbl[4])(sprite_status *, sprite_status *) = {
    &emie8_snc_r, &emie8_snc_l, &emie8_snc_w, &emie8_snc_pata};

void emie8(sprite_status *pActwk) {
    snc_ctrl(pActwk);
    emie8_jisin(pActwk);
    emie8_act_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
}

static void emie8_ini(sprite_status *pActwk) {
    pActwk->actflg |= 4;
    pActwk->sprpri = 1;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 20;

    pActwk->sproffset = 8736;
    pActwk->patbase = emie8_pat;
    pActwk->r_no0 += 2;
    plautoflag |= 1;

    emie8_wait(pActwk);
}

static void emie8_wait(sprite_status *pActwk) {
    if (bossstart)
        return;

    pActwk->r_no0 += 2;
    pActwk->xspeed.w = 320;

    emie8_l(pActwk);
}

static void emie8_l(sprite_status *pActwk) {
    patchg(pActwk, emie8_pchg);
    add_spd(pActwk);
    if (pActwk->xposi.w.h < 3744)
        return;

    pActwk->r_no0 += 2;
    emie8_j(pActwk);
}

static void emie8_j(sprite_status *pActwk) {
    pActwk->patno = 6;
    pActwk->xspeed.w = 0;
    pActwk->yspeed.w = -768;
    pActwk->r_no0 += 2;
    emie8_j2(pActwk);
}

static void emie8_j2(sprite_status *pActwk) {
    add_spd(pActwk);
    if ((pActwk->yspeed.w += 64) >= 0) {
        pActwk->patno = 7;
    }

    if (pActwk->yposi.w.h < 460)
        return;

    pActwk->yposi.w.h = 460;
    pActwk->xspeed.w = 320;
    pActwk->yspeed.w = 0;
    pActwk->r_no0 += 2;
    pActwk->mstno.w = 1;
}

static void emie8_l2(sprite_status *pActwk) {
    sMemSet(clchgtim, 127, sizeof(clchgtim));

    add_spd(pActwk);

    if (pActwk->xposi.w.h >= 3808) {
        if (++pActwk->actfree[0] >= 8) {
            pActwk->actfree[0] = 0;

            if (++pActwk->actfree[1] >= 8) {

                time_flag |= 128;
                gameflag.w = 2;
                stageno.w = 1792;
                return;
            }

            flash_out8();
        }
    }

    patchg(pActwk, emie8_pchg);
}

static void add_spd(sprite_status *pActwk) {
    pActwk->xposi.l += pActwk->xspeed.w << 8;
    pActwk->yposi.l += pActwk->yspeed.w << 8;
}

static void snc_ctrl(sprite_status *pActwk) {
    snc_act_tbl[pActwk->actfree[3] / 2](pActwk, &actwk[0]);
}

static void emie8_snc_r(sprite_status *pActwk, sprite_status *pPlayerwk) {
    swdata.w = 2056;
    if (pPlayerwk->xposi.w.h < 3984)
        return;

    pActwk->actfree[3] += 2;
    emie8_snc_l(pActwk, pPlayerwk);
}

static void emie8_snc_l(sprite_status *pActwk, sprite_status *pPlayerwk) {
    swdata.w = 1028;
    if (pPlayerwk->xposi.w.h > 3952)
        return;

    pActwk->actfree[3] += 2;
    swdata.w = 0;
    emie8_snc_w(pActwk, pPlayerwk);
}

static void emie8_snc_w(sprite_status *pActwk, sprite_status *pPlayerwk) {
    if (pActwk->r_no0 >= 6) {

        pActwk->actfree[3] += 2;
        pPlayerwk->mstno.b.h = 5;
    }
}

static void emie8_snc_pata(sprite_status *pActwk, sprite_status *pPlayerwk) {

    pActwk = pActwk;
    if (pPlayerwk->patcnt < 16) {
        pPlayerwk->patcnt = 16;
    }
}

static void flash_out8(void) {
    PALETTEENTRY *lpColwk;
    Sint32 i;

    lpColwk = lpcolorwk;

    for (i = 0; i < 64; ++i) {

        if (lpColwk->peRed != 224) {
            lpColwk->peRed += 32;
        }
        if (lpColwk->peGreen != 224) {
            lpColwk->peGreen += 32;
        }
        if (lpColwk->peBlue != 224) {
            lpColwk->peBlue += 32;
        }
        ++lpColwk;
    }
}

static void emie8_jisin(sprite_status *pActwk) {
    Sint16 wk;

    --pActwk->actfree[4];
    wk = -2;

    if (pActwk->actfree[4] / 2 & 1) {
        wk = -4;
    }
    scralim_down += wk;
    scralim_n_down += wk;
}
