#include "../equ.h"
#include "sikake.h"
#include "../action.h"
#include "../actset.h"
#include "../player_work.h"
#include "../playsub.h"
#include "../ridechk.h"
#include "coli1.h"

#if defined(R11A)
#define SPRITE_SIKAKE_BASE 488
#else
#define SPRITE_SIKAKE_BASE 471
#endif

typedef struct {
    Uint8 wobble_timer;
} tobita1_work;

static tobita1_work *tobita1_get_work(sprite_status *pActwk) {
    return (tobita1_work *)pActwk->actfree;
}

static Uint8 tobita1chg0[16] = {2, 0, 1, 0, 2, 0, 1, 0,
                                3, 4, 3, 5, 3, 4, 3, 255};
static Uint8 tobita1chg1[9] = {2, 0, 1, 0, 2, 0, 1, 0, 255};
static Uint8 tobita1chg2[9] = {2, 3, 4, 3, 5, 3, 4, 3, 255};
static Uint8 tobitastop1[3] = {0, 0, 255};
static Uint8 tobitastop2[3] = {0, 3, 255};
Uint8 *tobita1chg[5] = {tobita1chg0, tobita1chg1, tobita1chg2, tobitastop1,
                        tobitastop2};
static sprite_pattern tobita_a = {1, {{-8, -4, 0, SPRITE_SIKAKE_BASE}}};
static sprite_pattern tobita_b = {1, {{-8, -4, 0, SPRITE_SIKAKE_BASE + 1}}};
static sprite_pattern tobita_c = {1, {{-8, -16, 0, SPRITE_SIKAKE_BASE + 2}}};
static sprite_pattern tobita_r_a = {1, {{-24, -4, 0, SPRITE_SIKAKE_BASE + 3}}};
static sprite_pattern tobita_r_b = {1, {{-24, -4, 0, SPRITE_SIKAKE_BASE + 4}}};
static sprite_pattern tobita_r_c = {1, {{-24, -16, 0, SPRITE_SIKAKE_BASE + 5}}};
sprite_pattern *tobita1_pat[6] = {&tobita_a,   &tobita_b,   &tobita_c,
                                  &tobita_r_a, &tobita_r_b, &tobita_r_c};
static sprite_pattern pat0 = {1, {{-16, -16, 0, SPRITE_SIKAKE_BASE + 6}}};
sprite_pattern *hari12pat[1] = {&pat0};

Sint16 colli_pl_sikake_d(sprite_status *pActwk, sprite_status *pPlayerwk,
                         Sint16 collinum) {
    if (editmode.b.h == 0) {
        if (pPlayerwk->r_no0 < 6) {
            if (pPlayerwk->yspeed.w >= 0) {
                return colli_pl_sikake(pActwk, pPlayerwk, collinum);
            }
        }
    }
    pActwk->cddat &= 247;
    return 0;
}

Sint16 colli_pl_sikake(sprite_status *pActwk, sprite_status *pPlayerwk,
                       Sint16 collnum) {
    Sint16 xe;
    Sint16 xe_w;
    Sint16 xp;
    Sint16 xp_w;
    Sint16 hp;
    Sint16 ye;
    Sint16 ye_w;
    Sint16 yp;
    Sint16 yp_w;
    Sint16 vp;
    Sint16 right, left;
    Sint16 up, down;
    char *pPs_colli;
    char ps_colli_tbl[32] = {16, -16, 16,  -16, 16, -16, 4,   -4,  9,  -9, 56,
                             16, 0,   -24, 4,   -4, 0,   -24, 12,  0,  24, 0,
                             4,  -4,  24,  0,   12, 0,   32,  -32, 32, -32};

    collnum &= 7;
    collnum *= 4;
    pPs_colli = &ps_colli_tbl[collnum];

    xe = pActwk->xposi.w.h;
    xp = pPlayerwk->xposi.w.h;
    hp = (char)pPlayerwk->sprhs;

    right = pPs_colli[0];
    xe_w = xe;
    xp_w = xp;
    xe_w += right;
    xp_w -= hp;
    if (xp_w - xe_w >= 0)
        goto label1;

    left = pPs_colli[1];
    left *= -1;
    xe_w = xe;
    xp_w = xp;
    xe_w -= left;
    xp_w += hp;
    if (xe_w - xp_w >= 0)
        goto label1;

    ye = pActwk->yposi.w.h;
    yp = pPlayerwk->yposi.w.h;
    vp = (char)pPlayerwk->sprvsize;

    down = pPs_colli[2];
    ye_w = ye;
    yp_w = yp;
    ye_w += down;
    yp_w -= vp;
    if (yp_w - ye_w >= 0)
        goto label1;

    up = pPs_colli[3];
    up *= -1;
    ye_w = ye;
    yp_w = yp;
    ye_w -= up;
    yp_w += vp;
    if (ye_w - yp_w >= 0)
        goto label1;

    pActwk->cddat |= 8;
    return -1;

label1:
    pActwk->cddat &= 247;
    return 0;
}

void tobita1(sprite_status *pActwk) {
    void (*tobita_tbl[7])(sprite_status *) = {
        &tobita_init,   &tobita_stop_r, &tobita_stop_l, &tobita_ride_r,
        &tobita_ride_l, &tobita_yure_r, &tobita_yure_l};

    tobita_tbl[pActwk->r_no0 / 2](pActwk);
    frameout_s(pActwk);
}

void tobita_init(sprite_status *pActwk) {
    Uint8 d0, d1;

    pActwk->patbase = tobita1_pat;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->sprhsize = 16;
    pActwk->sprhs = 24;
    pActwk->sprvsize = 4;

    d0 = 3;
    d1 = 2;

    if (pActwk->userflag.b.h)
        goto label1;
    if (!(pActwk->actflg & 1))
        goto label2;
label1:
    d0 = 4;
    d1 = 4;
    pActwk->actflg &= 254;
    pActwk->cddat &= 254;
label2:
    pActwk->mstno.b.h = d0;
    pActwk->r_no0 = d1;
}

void tobita_stop_r(sprite_status *pActwk) {
    sprite_status *pPlayerwk;
    Sint32 d0;
    int_union d1;
    Sint16 tmp;

    pPlayerwk = &actwk[0];

    if (colli_pl_sikake_d(pActwk, pPlayerwk, 5)) {
        d0 = pActwk->yposi.l;
        d1.l = 0;
        d1.b.b4 = pPlayerwk->sprvsize;
        tmp = d1.w.l;
        d1.w.l = d1.w.h;
        d1.w.h = tmp;
        d0 -= d1.l;
        pPlayerwk->yposi.l = d0;
        pActwk->r_no0 = 10;
        pActwk->mstno.b.h = 3;
    }

    patchg(pActwk, tobita1chg);
    actionsub(pActwk);
}

void tobita_stop_l(sprite_status *pActwk) {
    sprite_status *pPlayerwk;
    Sint32 d0;
    int_union d1;
    Sint16 tmp;

    pPlayerwk = &actwk[0];

    if (colli_pl_sikake_d(pActwk, pPlayerwk, 3)) {
        d0 = pActwk->yposi.l;
        d1.l = 0;
        d1.b.b4 = pPlayerwk->sprvsize;
        tmp = d1.w.l;
        d1.w.l = d1.w.h;
        d1.w.h = tmp;
        d0 -= d1.l;
        pPlayerwk->yposi.l = d0;
        pActwk->r_no0 = 12;
        pActwk->mstno.b.h = 4;
    }

    patchg(pActwk, tobita1chg);
    actionsub(pActwk);
}

void tobita_ride_r(sprite_status *pActwk) {
    sprite_status *pPlayerwk;

    pPlayerwk = &actwk[0];

    if (colli_pl_sikake_d(pActwk, pPlayerwk, 5) == 0) {
        pActwk->r_no0 = 2;
        if (pPlayerwk->cddat & 2)
            pActwk->r_no0 = 10;
        if (pActwk->r_no0 == 10)
            tobita1_get_work(pActwk)->wobble_timer = 64;
    }

    patchg(pActwk, tobita1chg);
    actionsub(pActwk);
}

void tobita_ride_l(sprite_status *pActwk) {
    sprite_status *pPlayerwk;

    pPlayerwk = &actwk[0];

    if (colli_pl_sikake_d(pActwk, pPlayerwk, 3) == 0) {
        pActwk->r_no0 = 4;
        if (pPlayerwk->cddat & 2)
            pActwk->r_no0 = 12;
        if (pActwk->r_no0 == 12)
            tobita1_get_work(pActwk)->wobble_timer = 64;
    }

    patchg(pActwk, tobita1chg);
    actionsub(pActwk);
}

void tobita_yure_r(sprite_status *pActwk) {
    sprite_status *pPlayerwk;
    Sint32 zflag;
    Sint16 d0;
    char cnt;

    pActwk->mstno.b.h = 1;

    pPlayerwk = &actwk[0];

    if (colli_pl_sikake_d(pActwk, pPlayerwk, 6)) {
        d0 = pPlayerwk->yspeed.w;
        d0 += 256;
        if (d0 > 2560)
            d0 = 2560;
        d0 *= -1;
        pPlayerwk->yspeed.w = d0;
        tobita1_get_work(pActwk)->wobble_timer = 64;
        zflag = (pPlayerwk->cddat & 2) ? 0 : 1;
        pPlayerwk->cddat |= 2;
        if (zflag == 0)
            player_work_get(pPlayerwk)->jump_started = 0;
        pPlayerwk->cddat &= 223;
        player_work_get(pPlayerwk)->jump_lock = 0;

        pPlayerwk->sprvsize = 19;
        pPlayerwk->sprhs = 9;
        if ((pPlayerwk->cddat & 4) == 0) {
            pPlayerwk->sprvsize = 14;
            pPlayerwk->sprhs = 7;
            pPlayerwk->yposi.w.h += 5;
            pPlayerwk->cddat |= 4;
            pPlayerwk->mstno.b.h = 2;
        } else {
            pPlayerwk->cddat |= 16;
        }
    }

    cnt = tobita1_get_work(pActwk)->wobble_timer;
    --cnt;
    tobita1_get_work(pActwk)->wobble_timer = cnt;
    if (cnt == 0) {
        pActwk->r_no0 = 2;
        pActwk->mstno.b.h = 3;
        tobita1_get_work(pActwk)->wobble_timer = 64;
    }

    patchg(pActwk, tobita1chg);
    actionsub(pActwk);
}

void tobita_yure_l(sprite_status *pActwk) {
    sprite_status *pPlayerwk;
    Sint32 zflag;
    Sint16 d0;
    char cnt;

    pActwk->mstno.b.h = 2;

    pPlayerwk = &actwk[0];

    if (colli_pl_sikake_d(pActwk, pPlayerwk, 4)) {
        d0 = pPlayerwk->yspeed.w;
        d0 += 256;
        if (d0 > 2560)
            d0 = 2560;
        d0 *= -1;
        pPlayerwk->yspeed.w = d0;
        tobita1_get_work(pActwk)->wobble_timer = 64;
        zflag = (pPlayerwk->cddat & 2) ? 0 : 1;
        pPlayerwk->cddat |= 2;
        if (zflag == 0)
            player_work_get(pPlayerwk)->jump_started = 0;
        pPlayerwk->cddat &= 223;
        player_work_get(pPlayerwk)->jump_lock = 0;

        pPlayerwk->sprvsize = 19;
        pPlayerwk->sprhs = 9;
        if ((pPlayerwk->cddat & 4) == 0) {
            pPlayerwk->sprvsize = 14;
            pPlayerwk->sprhs = 7;
            pPlayerwk->yposi.w.h += 5;
            pPlayerwk->cddat |= 4;
            pPlayerwk->mstno.b.h = 2;
        } else {
            pPlayerwk->cddat |= 16;
        }
    }

    cnt = tobita1_get_work(pActwk)->wobble_timer;
    --cnt;
    tobita1_get_work(pActwk)->wobble_timer = cnt;
    if (cnt == 0) {
        tobita1_get_work(pActwk)->wobble_timer = 64;
        pActwk->r_no0 = 4;
        pActwk->mstno.b.h = 4;
    }

    patchg(pActwk, tobita1chg);
    actionsub(pActwk);
}

void hariyama(sprite_status *pActwk) {
    void (*hari_tbl[2])(sprite_status *) = {&hari_init, &hari_normal};

    hari_tbl[pActwk->r_no0 / 2](pActwk);
}

void hari_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->patbase = hari12pat;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 8;

    if (pActwk->actflg & 2) {
        pActwk->sprhsize = 18;
        pActwk->colino = 131;
    }
    hari_normal(pActwk);
}

void hari_normal(sprite_status *pActwk) {
    sprite_status *pPlayerwk;
    Sint16 py;

    pPlayerwk = &actwk[0];
    py = pPlayerwk->yposi.w.h;
    py -= pActwk->yposi.w.h;
    if (py < 0)
        py *= -1;
    if (py >= 32)
        goto label1;
    if (pActwk->actflg & 2) {
        hitchk(pActwk, pPlayerwk);
        goto label1;
    }
    if (hitchk(pActwk, pPlayerwk) == 0)
        goto label1;
    if (!(pActwk->cddat & 8))
        goto label1;

    if (plpower_a)
        goto label1;
    if (plpower_m)
        goto label1;

    if (pPlayerwk->r_no0 < 4) {
        if ((Uint8)player_work_get(pPlayerwk)->damage_invulnerability_timer ==
            0) {
            pPlayerwk->yposi.w.h -= pPlayerwk->yspeed.w;
            playdamageset(pPlayerwk, pActwk);
        }
    }
label1:
    actionsub(pActwk);
    frameout_s(pActwk);
}
