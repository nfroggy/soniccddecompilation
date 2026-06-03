#include <stddef.h>

#include "../equ.h"
#include "amenbo.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../suicide.h"
#include "playsub4.h"

#pragma pack(push, 1)
typedef struct {
    Sint16 stop_timer;
    Sint32 velocity_x;
    Sint32 acceleration_x;
    Sint32 deceleration_x;
    Sint32 palette_style;
    Sint16 shot_timer;
    Sint16 origin_x;
} amenbo_work;

typedef struct {
    Uint8 unused0[88];
    Sint16 projectile_owner_index;
} amenbo_projectile_legacy_slot;
#pragma pack(pop)

_Static_assert(offsetof(amenbo_work, stop_timer) == 0,
               "amenbo_work.stop_timer offset");
_Static_assert(offsetof(amenbo_work, velocity_x) == 2,
               "amenbo_work.velocity_x offset");
_Static_assert(offsetof(amenbo_work, acceleration_x) == 6,
               "amenbo_work.acceleration_x offset");
_Static_assert(offsetof(amenbo_work, deceleration_x) == 10,
               "amenbo_work.deceleration_x offset");
_Static_assert(offsetof(amenbo_work, palette_style) == 14,
               "amenbo_work.palette_style offset");
_Static_assert(offsetof(amenbo_work, shot_timer) == 18,
               "amenbo_work.shot_timer offset");
_Static_assert(offsetof(amenbo_work, origin_x) == 20,
               "amenbo_work.origin_x offset");
_Static_assert(sizeof(amenbo_work) <= sizeof(((sprite_status *)0)->actfree),
               "amenbo_work fits in actfree");
_Static_assert(offsetof(amenbo_projectile_legacy_slot, projectile_owner_index) ==
                   88,
               "amenbo projectile owner legacy offset");

static amenbo_work *amenbo_get_work(sprite_status *pActwk) {
    return (amenbo_work *)pActwk->actfree;
}

static amenbo_projectile_legacy_slot *
amenbo_get_projectile_legacy_slot(sprite_status *pActwk) {
    return (amenbo_projectile_legacy_slot *)pActwk;
}

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

        pActwk->xposi.l += amenbo_get_work(pActwk)->velocity_x;
        if (amenbo_get_work(pActwk)->palette_style == 0)
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
    amenbo_get_work(pActwk)->shot_timer = 120;
    amenbo_get_work(pActwk)->origin_x = pActwk->xposi.w.h;
    if (!pActwk->userflag.b.h) {
        pActwk->patbase = pat_amenbo_e;
        amenbo_get_work(pActwk)->palette_style = 1;
        amenbo_get_work(pActwk)->acceleration_x = -1536;
        amenbo_get_work(pActwk)->deceleration_x = 256;
    } else {
        pActwk->patbase = pat_amenbo_b;
        amenbo_get_work(pActwk)->palette_style = 0;
        amenbo_get_work(pActwk)->acceleration_x = -512;
        amenbo_get_work(pActwk)->deceleration_x = 192;
    }
    act_accel(pActwk);
}

static void act_accel(sprite_status *pActwk) {
    Sint32 lD5, lD6;
    amenbo_work *pWork = amenbo_get_work(pActwk);

    lD6 = pWork->velocity_x + pWork->acceleration_x;
    if (!pActwk->userflag.b.h)
        lD5 = -98304;
    else
        lD5 = -49152;

    if (lD6 <= lD5) {
        lD6 = lD5;

        pActwk->r_no0 += 2;
        pWork->acceleration_x *= -1;
    } else {

        lD5 = -lD5;
        if (lD6 < lD5)
            goto label1;

        lD6 = lD5;

        pActwk->r_no0 += 2;
        pWork->acceleration_x *= -1;
    }

label1:
    pWork->velocity_x = lD6;
    if (!pActwk->userflag.b.h)
        act_tama(pActwk);

    if (pWork->palette_style == 0)
        patchg(pActwk, pchg_b);
    else
        patchg(pActwk, pchg_e);
    act_move0(pActwk);
}

static void act_decel(sprite_status *pActwk) {
    Sint32 lD6;
    Uint8 temp0, temp1;
    amenbo_work *pWork = amenbo_get_work(pActwk);

    lD6 = pWork->velocity_x + pWork->deceleration_x;
    temp0 = temp1 = 0;
    if (pWork->velocity_x < 0)
        temp0 = 255;
    if (lD6 < 0)
        temp1 = 255;
    if (temp0 ^ temp1) {
        pActwk->r_no0 += 2;
        pWork->deceleration_x *= -1;
        lD6 = 0;
        if (!pActwk->userflag.b.h)
            pWork->stop_timer = 60;
        else
            pWork->stop_timer = 60;
    }

    pWork->velocity_x = lD6;
    if (!pActwk->userflag.b.h)
        act_tama(pActwk);

    act_move0(pActwk);
}

static void act_stop(sprite_status *pActwk) {
    amenbo_work *pWork = amenbo_get_work(pActwk);

    --pWork->stop_timer;
    if (pWork->stop_timer <= 0) {
        pActwk->r_no0 += 2;
        pActwk->actflg ^= 1;
        pActwk->cddat ^= 1;
        if (!pActwk->userflag.b.h)
            pWork->stop_timer = 60;
        else
            pWork->stop_timer = 60;
    }

    if (!pActwk->userflag.b.h)
        act_tama(pActwk);

    act_move0(pActwk);
}

static void act_stop1(sprite_status *pActwk) {
    --amenbo_get_work(pActwk)->stop_timer;
    if (amenbo_get_work(pActwk)->stop_timer <= 0)
        pActwk->r_no0 = 2;

    act_move0(pActwk);
}

static void act_move0(sprite_status *pActwk) {
    pActwk->xposi.l += amenbo_get_work(pActwk)->velocity_x;
    pActwk->yposi.w.h = waterposi - 8;
}

static void act_tama(sprite_status *pActwk) {
    sprite_status *subActwk;

    --amenbo_get_work(pActwk)->shot_timer;
    if (amenbo_get_work(pActwk)->shot_timer <= 0) {
        amenbo_get_work(pActwk)->shot_timer = 120;
        if (act_check(pActwk) != 0) {

            if (actwkchk(&subActwk) == 0) {
                subActwk->actno = pActwk->actno;
                subActwk->userflag.b.h = -1;
                amenbo_get_projectile_legacy_slot(subActwk)
                    ->projectile_owner_index = (Uint8)(pActwk - actwk);
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
                amenbo_get_work(subActwk)->palette_style =
                    amenbo_get_work(pActwk)->palette_style;
                subActwk->mstno.b.h = 1;
                if (!(subActwk->actflg & 1))
                    amenbo_get_work(subActwk)->velocity_x = -0x20000;
                else
                    amenbo_get_work(subActwk)->velocity_x = 0x20000;
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
