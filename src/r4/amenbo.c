#include "../equ.h"
#include "amenbo.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../suicide.h"
#include "playsub4.h"

#if defined(R41A)
#define SPRITE_AMENBO_BASE 479
#elif defined(R41B)
#define SPRITE_AMENBO_BASE 440
#elif defined(R41C)
#define SPRITE_AMENBO_BASE 457
#elif defined(R41D)
#define SPRITE_AMENBO_BASE 447
#elif defined(R42A)
#define SPRITE_AMENBO_BASE 485
#elif defined(R42B)
#define SPRITE_AMENBO_BASE 438
#else
#define SPRITE_AMENBO_BASE 442
#endif

static void act_init(sprite_status *pActwk);
static void act_accel(sprite_status *pActwk);
static void act_decel(sprite_status *pActwk);
static void act_stop(sprite_status *pActwk);
static void act_stop1(sprite_status *pActwk);
static void act_move0(sprite_status *pActwk);
static void act_tama(sprite_status *pActwk);
static Sint32 act_check(sprite_status *pActwk);

static Uint8 pchg_b0[4] = {20, 0, 1, 255};
static Uint8 pchg_e0[4] = {10, 0, 1, 255};
static Uint8 pchg_e1[4] = {5, 2, 3, 255};
static Uint8 *pchg_b[1] = {pchg_b0};
static Uint8 *pchg_e[2] = {pchg_e0, pchg_e1};
static sprite_pattern spr_amenbo_b_00 = {1,
                                         {{-20, -12, 0, SPRITE_AMENBO_BASE}}};
static sprite_pattern spr_amenbo_b_01 = {
    1, {{-20, -12, 0, SPRITE_AMENBO_BASE + 1}}};
static sprite_pattern spr_amenbo_e_00 = {
    1, {{-20, -12, 0, SPRITE_AMENBO_BASE + 2}}};
static sprite_pattern spr_amenbo_e_01 = {
    1, {{-20, -12, 0, SPRITE_AMENBO_BASE + 3}}};
static sprite_pattern spr_amenbo_e_02 = {1,
                                         {{-4, -4, 0, SPRITE_AMENBO_BASE + 4}}};
static sprite_pattern spr_amenbo_e_03 = {1,
                                         {{-4, -4, 0, SPRITE_AMENBO_BASE + 5}}};
sprite_pattern *pat_amenbo_b[2] = {&spr_amenbo_b_00, &spr_amenbo_b_01};
sprite_pattern *pat_amenbo_e[4] = {&spr_amenbo_e_00, &spr_amenbo_e_01,
                                   &spr_amenbo_e_02, &spr_amenbo_e_03};

void amenbo(sprite_status *pActwk) {
    static void (*act_tbl[5])(sprite_status *) = {
        &act_init, &act_accel, &act_decel, &act_stop, &act_stop1};

    if (pActwk->userflag.b.h < 0) {

        pActwk->xposi.l += ((Sint32 *)pActwk)[12];
        if (((Sint32 *)pActwk)[15] == 0)
            patchg(pActwk, pchg_b);
        else
            patchg(pActwk, pchg_e);
        actionsub(pActwk);
        frameout_s(pActwk);
        return;
    }
    if (enemy_suicide(pActwk))
        return;
    act_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s(pActwk);
}

static void act_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->sproffset = 9136;
    pActwk->sprhs = 20;
    pActwk->sprhsize = 20;
    pActwk->sprvsize = 12;
    pActwk->colino = 49;
    ((Sint16 *)pActwk)[32] = 120;
    ((Sint16 *)pActwk)[33] = pActwk->xposi.w.h;
    if (!pActwk->userflag.b.h) {
        pActwk->patbase = pat_amenbo_e;
        ((Sint32 *)pActwk)[15] = 1;
        ((Sint32 *)pActwk)[13] = -1536;
        ((Sint32 *)pActwk)[14] = 256;
    } else {
        pActwk->patbase = pat_amenbo_b;
        ((Sint32 *)pActwk)[15] = 0;
        ((Sint32 *)pActwk)[13] = -512;
        ((Sint32 *)pActwk)[14] = 192;
    }
    act_accel(pActwk);
}

static void act_accel(sprite_status *pActwk) {
    Sint32 lD5, lD6;

    lD6 = ((Sint32 *)pActwk)[12] + ((Sint32 *)pActwk)[13];
    if (!pActwk->userflag.b.h)
        lD5 = -98304;
    else
        lD5 = -49152;

    if (lD6 <= lD5) {
        lD6 = lD5;

        pActwk->r_no0 += 2;
        ((Sint32 *)pActwk)[13] *= -1;
    } else {

        lD5 = -lD5;
        if (lD6 < lD5)
            goto label1;

        lD6 = lD5;

        pActwk->r_no0 += 2;
        ((Sint32 *)pActwk)[13] *= -1;
    }

label1:
    ((Sint32 *)pActwk)[12] = lD6;
    if (!pActwk->userflag.b.h)
        act_tama(pActwk);

    if (((Sint32 *)pActwk)[15] == 0)
        patchg(pActwk, pchg_b);
    else
        patchg(pActwk, pchg_e);
    act_move0(pActwk);
}

static void act_decel(sprite_status *pActwk) {
    Sint32 lD6;
    Uint8 temp0, temp1;

    lD6 = ((Sint32 *)pActwk)[12] + ((Sint32 *)pActwk)[14];
    temp0 = temp1 = 0;
    if (((Sint32 *)pActwk)[12] < 0)
        temp0 = 255;
    if (lD6 < 0)
        temp1 = 255;
    if (temp0 ^ temp1) {
        pActwk->r_no0 += 2;
        ((Sint32 *)pActwk)[14] *= -1;
        lD6 = 0;
        if (!pActwk->userflag.b.h)
            ((Sint16 *)pActwk)[23] = 60;
        else
            ((Sint16 *)pActwk)[23] = 60;
    }

    ((Sint32 *)pActwk)[12] = lD6;
    if (!pActwk->userflag.b.h)
        act_tama(pActwk);

    act_move0(pActwk);
}

static void act_stop(sprite_status *pActwk) {
    --((Sint16 *)pActwk)[23];
    if (((Sint16 *)pActwk)[23] <= 0) {
        pActwk->r_no0 += 2;
        pActwk->actflg ^= 1;
        pActwk->cddat ^= 1;
        if (!pActwk->userflag.b.h)
            ((Sint16 *)pActwk)[23] = 60;
        else
            ((Sint16 *)pActwk)[23] = 60;
    }

    if (!pActwk->userflag.b.h)
        act_tama(pActwk);

    act_move0(pActwk);
}

static void act_stop1(sprite_status *pActwk) {
    --((Sint16 *)pActwk)[23];
    if (((Sint16 *)pActwk)[23] <= 0)
        pActwk->r_no0 = 2;

    act_move0(pActwk);
}

static void act_move0(sprite_status *pActwk) {
    pActwk->xposi.l += ((Sint32 *)pActwk)[12];
    pActwk->yposi.w.h = waterposi - 8;
}

static void act_tama(sprite_status *pActwk) {
    sprite_status *subActwk;

    --((Sint16 *)pActwk)[32];
    if (((Sint16 *)pActwk)[32] <= 0) {
        ((Sint16 *)pActwk)[32] = 120;
        if (act_check(pActwk) != 0) {

            if (actwkchk(&subActwk) == 0) {
                subActwk->actno = pActwk->actno;
                subActwk->userflag.b.h = -1;
                ((Sint16 *)subActwk)[44] = (Uint8)(pActwk - actwk);
                subActwk->xposi.w.h = pActwk->xposi.w.h;
                subActwk->yposi.w.h = pActwk->yposi.w.h;
                subActwk->actflg = pActwk->actflg;
                subActwk->sprpri = pActwk->sprpri + 1;
                subActwk->sproffset = pActwk->sproffset;
                subActwk->sprhs = 4;
                subActwk->sprhsize = 4;
                subActwk->sprvsize = 4;
                subActwk->colino = 178;
                subActwk->patbase = pActwk->patbase;
                ((Sint32 *)subActwk)[15] = ((Sint32 *)pActwk)[15];
                subActwk->mstno.b.h = 1;
                if (!(subActwk->actflg & 1))
                    ((Sint32 *)subActwk)[12] = -0x20000;
                else
                    ((Sint32 *)subActwk)[12] = 0x20000;
                if ((char)pActwk->actflg < 0)
                    soundset(160);
            }
        }
    }
}

static Sint32 act_check(sprite_status *pActwk) {
    Sint16 tmp;
    Uint8 bD0;
    Uint8 bD1;
    Sint32 ret;

    ret = 0;

    tmp = actwk[0].yposi.w.h - pActwk->yposi.w.h + 96;
    if ((Uint16)tmp < 192) {
        tmp -= 192;
        bD0 = bD1 = 0;
        tmp = actwk[0].xposi.w.h - pActwk->xposi.w.h;
        if (tmp < 0)
            bD1 = 255;
        tmp += 96;
        if ((Uint16)tmp < 192) {
            if (!(pActwk->actflg & 1))
                bD0 = 255;
            if (!(bD1 ^ bD0))
                ret = -1;
        }
    }
    return ret;
}
