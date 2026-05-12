#include "../equ.h"
#include "friend4.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"
#include "../suicide.h"
#include "playsub4.h"

#if defined(R41A)
#define SPRITE_FRIEND4_BASE 494
#elif defined(R41B)
#define SPRITE_FRIEND4_BASE 455
#elif defined(R41C)
#define SPRITE_FRIEND4_BASE 472
#elif defined(R41D)
#define SPRITE_FRIEND4_BASE 462
#elif defined(R42A)
#define SPRITE_FRIEND4_BASE 500
#elif defined(R42B)
#define SPRITE_FRIEND4_BASE 453
#elif defined(R42C) || defined(R42D)
#define SPRITE_FRIEND4_BASE 457
#else
#define SPRITE_FRIEND4_BASE 435
#endif

static void m_init(sprite_status *pActwk);
static void m0move(sprite_status *pActwk);
static void m1move(sprite_status *pActwk);

static Uint8 pchg0[30] = {0, 0, 0, 2, 0, 0, 2, 1, 1, 2, 1, 1, 2, 0, 0,
                          2, 2, 0, 0, 2, 2, 1, 1, 2, 2, 1, 1, 2, 2, 255};
static Uint8 pchg1[4] = {19, 0, 1, 255};
static Uint8 *pchg[2] = {pchg0, pchg1};
static sprite_pattern spr_friend4_00 = {1, {{-8, -12, 0, SPRITE_FRIEND4_BASE}}};
static sprite_pattern spr_friend4_01 = {
    1, {{-8, -12, 0, SPRITE_FRIEND4_BASE + 1}}};
static sprite_pattern spr_friend4_10 = {
    1, {{-8, -12, 0, SPRITE_FRIEND4_BASE + 2}}};
static sprite_pattern spr_friend4_11 = {
    1, {{-8, -12, 0, SPRITE_FRIEND4_BASE + 3}}};
static sprite_pattern spr_friend4_99 = {
    0, {{-8, -12, 0, SPRITE_FRIEND4_BASE + 3}}};
sprite_pattern *pat_friend0[3] = {&spr_friend4_00, &spr_friend4_01,
                                  &spr_friend4_99};
sprite_pattern *pat_friend1[3] = {&spr_friend4_10, &spr_friend4_11,
                                  &spr_friend4_99};
static Uint16 tbl0sproffset[11] = {1104, 1104, 1104, 0, 1104, 1104,
                                   1104, 0,    0,    0, 1104};

void friend4(sprite_status *pActwk) {
    if (!friend_suicide(pActwk)) {
        if ((char)pActwk->r_no0 < 0) {
            m0move(pActwk);
            return;
        }

        if (pActwk->r_no0 != 0) {
            m1move(pActwk);
            return;
        }
        m_init(pActwk);
    }
}

static void m_init(sprite_status *pActwk) {
    short_union temp;

    pActwk->actflg |= 4;
    pActwk->sprpri = 5;
    pActwk->sprhs = 8;
    pActwk->sprhsize = 8;
    pActwk->sprvsize = 12;

    temp.w = 0;
    temp.b.l = stageno.b.l;
    temp.w <<= 2;
    temp.b.l = temp.b.l + (char)time_flag;
    temp.w <<= 1;
    pActwk->sproffset = tbl0sproffset[temp.w];
    if (pActwk->userflag.b.h & 1) {
        pActwk->sproffset += 8192;
    }

    pActwk->patbase = pat_friend0;
    if (pActwk->userflag.b.h & 1)
        pActwk->patbase = pat_friend1;
    ((Sint16 *)pActwk)[25] = pActwk->yposi.w.h;
    if (pActwk->userflag.b.h < 0) {

        pActwk->r_no0 -= 2;
        pActwk->actflg ^= 1;
        pActwk->cddat ^= 1;
        ((Sint16 *)pActwk)[30] = pActwk->xposi.w.h;
        ((Sint32 *)pActwk)[14] = 512;
        if (pActwk->userflag.b.h & 1) {

            pActwk->actflg ^= 1;
            pActwk->cddat ^= 1;
            ((Sint32 *)pActwk)[14] *= -1;
            ((Sint32 *)pActwk)[13] = 12288;
        }
    } else {

        pActwk->r_no0 += 2;
        pActwk->mstno.b.h = 1;
        *(Sint32 *)&pActwk->actfree[0] = 65536;
        ((Sint32 *)pActwk)[14] = 256;
    }
}

static void m0move(sprite_status *pActwk) {
    sprite_status *tempact;
    Uint16 sin, cos;
    Sint32 sinl, cosl;

    tempact = &actwk[((Sint16 *)pActwk)[33]];
    if (tempact->actno != 82) {

        frameout(pActwk);
        return;
    }
    if (((char *)tempact)[67]) {

        frameout(pActwk);
        return;
    }

    sinset(((char *)pActwk)[53], (Sint16 *)&sin, (Sint16 *)&cos);
    cosl = 0, sinl = 0;
    cosl = cos;
    sinl = sin;
    cosl = (Uint32)cosl >> 16 & 65535 | cosl << 16 & -65536;
    sinl = (Uint32)sinl >> 16 & 65535 | sinl << 16 & -65536;
    cosl >>= 3;
    sinl >>= 4;
    cosl = cosl + 32768;
    sinl = sinl + 32768;
    cosl = (Uint32)cosl >> 16 & 65535 | cosl << 16 & -65536;
    sinl = (Uint32)sinl >> 16 & 65535 | sinl << 16 & -65536;
    pActwk->xposi.w.h = ((Sint16 *)pActwk)[30] + (Sint16)(cosl & 65535);
    pActwk->yposi.w.h = ((Sint16 *)pActwk)[25] + (Sint16)(sinl & 65535);

    ((Sint32 *)pActwk)[13] += ((Sint32 *)pActwk)[14];
    if (!(((Sint32 *)pActwk)[13] & 32767)) {

        pActwk->actflg ^= 1;
        pActwk->cddat ^= 1;
        if (pActwk->sprpri != 5) {

            pActwk->sprpri = 5;
        } else {

            pActwk->sprpri = 3;
        }
    }

    patchg(pActwk, pchg);
    actionsub(pActwk);
}

static void m1move(sprite_status *pActwk) {
    Uint16 sin, cos;
    Sint32 sinl;

    pActwk->xposi.l += *(Sint32 *)&pActwk->actfree[0];

    sinset(((char *)pActwk)[53], (Sint16 *)&sin, (Sint16 *)&cos);
    sinl = 0;
    sinl = sin;
    sinl = (Uint32)sinl >> 16 & 65535 | sinl << 16 & -65536;
    sinl >>= 3;
    sinl = sinl + 32768;
    sinl = (Uint32)sinl >> 16 & 65535 | sinl << 16 & -65536;
    pActwk->yposi.w.h = ((Sint16 *)pActwk)[25] + (Sint16)(sinl & 65535);

    ((Sint32 *)pActwk)[13] += ((Sint32 *)pActwk)[14];
    if (((Sint32 *)pActwk)[13] < 0) {

        ((Sint32 *)pActwk)[14] *= -1;
        *(Sint32 *)&pActwk->actfree[0] *= -1;
        pActwk->actflg ^= 1;
        pActwk->cddat ^= 1;
    } else {

        if (((Sint32 *)pActwk)[13] >= 65536) {

            ((Sint32 *)pActwk)[14] *= -1;
            *(Sint32 *)&pActwk->actfree[0] *= -1;
            pActwk->actflg ^= 1;
            pActwk->cddat ^= 1;
        }
    }

    patchg(pActwk, pchg);
    actionsub(pActwk);
    frameout_s(pActwk);
}
