#include "../equ.h"
#include "friend7.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../etc.h"
#include "../playsub.h"
#include "../suicide.h"

#if defined(R73C) || defined(R73D)
#define SPRITE_FRIEND7_BASE 441
#else
#define SPRITE_FRIEND7_BASE 462
#endif

static void t_roll(Sint32 cos_shift, Sint32 sin_shift, sprite_status *pActwk);
static void rev_h(sprite_status *pActwk);
static void set_sproffset(sprite_status *pActwk);
static void t_init(sprite_status *pActwk);
static void t_move(sprite_status *pActwk);
static void t_movie(sprite_status *pActwk);
static void p_init(sprite_status *pActwk);
static void p_move(sprite_status *pActwk);
static void p_rev(sprite_status *pActwk);
static void p_movie(sprite_status *pActwk);

Uint8 pchg00[4] = {3, 0, 1, 255};
Uint8 pchg01[30] = {0, 0, 0, 2, 0, 0, 2, 1, 1, 2, 2, 1, 1, 2, 2,
                    0, 0, 2, 0, 0, 2, 1, 1, 2, 2, 1, 1, 2, 2, 255};
Uint8 pchg10[58] = {0, 3, 3, 2, 3, 3, 2, 3, 3, 2, 3, 3, 2, 3, 3, 2, 2, 3,  3, 2,
                    2, 3, 3, 2, 2, 3, 3, 2, 2, 4, 4, 2, 4, 4, 2, 4, 4, 2,  4, 4,
                    2, 4, 4, 2, 2, 4, 4, 2, 2, 4, 4, 2, 2, 4, 4, 2, 2, 255};
Uint8 *pchg0[2] = {pchg00, pchg01};
Uint8 *pchg1[1] = {pchg10};
sprite_pattern spr_flicky1 = {1, {{-8, -8, 0, SPRITE_FRIEND7_BASE}}};
sprite_pattern spr_flicky2 = {1, {{-8, -8, 0, SPRITE_FRIEND7_BASE + 1}}};
sprite_pattern spr_ricky1 = {1, {{-8, -12, 0, SPRITE_FRIEND7_BASE + 2}}};
sprite_pattern spr_ricky2 = {1, {{-8, -12, 0, SPRITE_FRIEND7_BASE + 3}}};
sprite_pattern spr_ricky3 = {1, {{-8, -12, 0, SPRITE_FRIEND7_BASE + 4}}};
sprite_pattern spr_ricky4 = {1, {{-8, -4, 0, SPRITE_FRIEND7_BASE + 5}}};
sprite_pattern spr_friend0 = {0, {{-8, -4, 0, SPRITE_FRIEND7_BASE + 5}}};
sprite_pattern *pat_friend0[3] = {&spr_flicky1, &spr_flicky2, &spr_friend0};
sprite_pattern *pat_friend1[5] = {&spr_ricky1, &spr_ricky2, &spr_friend0,
                                  &spr_ricky3, &spr_ricky4};
Uint16 tbl0sproffset[11] = {918, 918, 918, 0, 918, 918, 918, 0, 0, 0, 918};

static void t_roll(Sint32 cos_shift, Sint32 sin_shift, sprite_status *pActwk) {
    Uint16 sin, cos;

    sinset(pActwk->actfree[4], (Sint16 *)&sin, (Sint16 *)&cos);
    cos = (Sint16)cos >> cos_shift;
    sin = (Sint16)sin >> sin_shift;
    cos += ((Uint16 *)pActwk)[23];
    sin += ((Uint16 *)pActwk)[24];
    pActwk->xposi.w.h = cos;
    pActwk->yposi.w.h = sin;
}

static void rev_h(sprite_status *pActwk) {
    pActwk->actflg ^= 1;
    pActwk->cddat ^= 1;
}

static void set_sproffset(sprite_status *pActwk) {
    Sint32 temp;

    temp = stageno.b.l;
    temp *= 4;
    temp += time_flag;
    temp &= 255;
    pActwk->sproffset = tbl0sproffset[temp];
}

void friend(sprite_status *pActwk) {
    void (*tbl_f[3])(sprite_status *) = {&t_init, &t_move, &t_movie};
    void (*tbl_r[5])(sprite_status *) = {&p_init, &p_move, &p_move, &p_rev,
                                         &p_movie};

    if (friend_suicide(pActwk))
        return;
    if (!(pActwk->userflag.b.h & 127))
        tbl_f[pActwk->r_no0 / 2](pActwk);
    else
        tbl_r[pActwk->r_no0 / 2](pActwk);
}

static void t_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg = 4;
    pActwk->sprvsize = 8;
    pActwk->sprhs = 8;
    pActwk->sprpri = 4;
    pActwk->sprhsize = 8;
    pActwk->patbase = pat_friend0;
    ((Sint16 *)pActwk)[23] = pActwk->xposi.w.h;
    ((Sint16 *)pActwk)[24] = pActwk->yposi.w.h;
    rev_h(pActwk);
    set_sproffset(pActwk);

    if (pActwk->userflag.b.h >= 0) {

        pActwk->actfree[4] = 1;
        pActwk->actfree[5] = 1;
    } else {
        pActwk->r_no0 += 2;
        pActwk->mstno.b.h = 1;
        pActwk->sprpri = 3;
    }
}

static void t_move(sprite_status *pActwk) {
    Uint8 temp;

    t_roll(1, 1, pActwk);
    temp = pActwk->actfree[4] + pActwk->actfree[5];
    if ((Uint8)(temp - 1) < 127)
        goto label1;
    temp = pActwk->actfree[4];
    ((char *)pActwk)[51] *= -1;
    rev_h(pActwk);
label1:
    pActwk->actfree[4] = temp;
    patchg(pActwk, pchg0);
    actionsub(pActwk);
    frameout_s00(pActwk, ((Sint16 *)pActwk)[23]);
}

static void t_movie(sprite_status *pActwk) {
    sprite_status *tempact;

    tempact = &actwk[((Sint16 *)pActwk)[33]];
    if (tempact->actno != 39) {
        frameout(pActwk);
        return;
    }
    if (((char *)tempact)[67]) {
        frameout(pActwk);
        return;
    }

    t_roll(3, 4, pActwk);

    pActwk->actfree[4] += 4;
    if (!(pActwk->actfree[4] & 127))
        rev_h(pActwk);
    else {
        patchg(pActwk, pchg0);
        actionsub(pActwk);
    }
}

static void p_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg = 4;
    pActwk->sprvsize = 12;
    pActwk->sprhs = 8;
    pActwk->sprpri = 4;
    pActwk->sprhsize = 8;
    pActwk->patbase = pat_friend1;

    set_sproffset(pActwk);

    if (pActwk->userflag.b.h >= 0) {
        ((Sint32 *)pActwk)[12] = 65536;
        ((Sint32 *)pActwk)[13] = -0x40000;
    } else {
        pActwk->r_no0 = 8;
        rev_h(pActwk);
    }
}

static void p_move(sprite_status *pActwk) {
    Sint16 temp;

    pActwk->xposi.l += ((Sint32 *)pActwk)[12];
    pActwk->yposi.l += ((Sint32 *)pActwk)[13];

    if ((((Sint32 *)pActwk)[13] += 8192) < 0)
        pActwk->patno = 0;
    else
        pActwk->patno = 1;

    if ((temp = emycol_d(pActwk)) <= 0) {
        pActwk->r_no0 += 2;
        pActwk->yposi.w.h += temp;
        ((Sint32 *)pActwk)[13] = -0x40000;
    }
    actionsub(pActwk);
    frameout_s(pActwk);
}

static void p_rev(sprite_status *pActwk) {
    pActwk->r_no0 = 2;
    ((Sint32 *)pActwk)[12] *= -1;
    rev_h(pActwk);

    actionsub(pActwk);
    frameout_s(pActwk);
}

static void p_movie(sprite_status *pActwk) {
    sprite_status *tempact;

    tempact = &actwk[((Sint16 *)pActwk)[33]];
    if (tempact->actno != 39) {
        frameout(pActwk);
        return;
    }
    if (((char *)tempact)[67]) {
        frameout(pActwk);
        return;
    }
    patchg(pActwk, pchg1);
    actionsub(pActwk);
}
