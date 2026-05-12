#include "equ.h"
#include "branko1.h"
#include "action.h"
#include "actset.h"
#include "dummy.h"
#include "etc.h"
#include "ridechk.h"

static void branko1_init(sprite_status *pActwk);
static void branko1_move(sprite_status *pActwk);
static void branko1_posiset(sprite_status *pActwk);
static void branko1_ridechk(sprite_status *pActwk);

#if defined(R12A) || defined(R12B)
#define SPRITE_BRANKO1_BASE 524
#define SPRITE_BRANKO1_MIDDLE_XOFF -24
#define BRANKO1_INITBL_AREA1_2 -256
#elif defined(R12C) || defined(R12D)
#define SPRITE_BRANKO1_BASE 515
#define SPRITE_BRANKO1_MIDDLE_XOFF -24
#define BRANKO1_INITBL_AREA1_2 -256
#else
#define SPRITE_BRANKO1_BASE 515
#define SPRITE_BRANKO1_MIDDLE_XOFF -8
#define BRANKO1_INITBL_AREA1_2 -512
#endif

static sprite_pattern pat0 = {1, {{-8, -8, 0, SPRITE_BRANKO1_BASE}}};
static sprite_pattern pat1 = {
    1, {{SPRITE_BRANKO1_MIDDLE_XOFF, -8, 0, SPRITE_BRANKO1_BASE + 1}}};
static sprite_pattern pat2 = {1, {{-8, -8, 0, SPRITE_BRANKO1_BASE + 2}}};
sprite_pattern *branko1pat[3] = {&pat0, &pat1, &pat2};
static void (*branko1_move_tbl[2])(sprite_status *) = {&branko1_init,
                                                       &branko1_move};
static brankodata branko1_initbl[8] = {
    {-16384, 8, 512, -512},
    {-16384, 2, 256, -256},
    {-16384, -2, BRANKO1_INITBL_AREA1_2, 256},
    {-24576, 3, 384, -384},
    {-32768, -2, -256, 256},
    {-32768, 2, 256, -256},
    {-32768, -1, -256, 0},
    {-32768, 1, 256, 0}};

void branko1(sprite_status *pActwk) {
    branko1_move_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s00(pActwk, ((Sint16 *)pActwk)[29]);
}

static void branko1_init(sprite_status *pActwk) {
    Sint32 i;
    Uint8 knum;
    sprite_status *pNewact;

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->patbase = branko1pat;

    pActwk->sprhsize = 24;
    pActwk->sprvsize = 8;

    if (pActwk->actfree[18] == 0) {
        ((Uint16 *)pActwk)[29] = pActwk->xposi.w.h;
        ((Uint16 *)pActwk)[27] = pActwk->yposi.w.h;
        pActwk->actfree[5] = knum = (Uint8)(pActwk->userflag.b.h & 15);

        for (; knum > 0; --knum) {
            if (actwkchk(&pNewact) == 0) {

                pNewact->actno = 41;
                ((Uint16 *)pNewact)[29] = ((Uint16 *)pActwk)[29];

                ((Uint16 *)pNewact)[27] = ((Uint16 *)pActwk)[27];

                pNewact->userflag.b.h = pActwk->userflag.b.h;
                pNewact->actfree[5] = pActwk->actfree[5];
                pNewact->actfree[18] = knum;

                if (pActwk->actfree[5] == knum) {

                    pNewact->patno = 1;
                } else {
                    pNewact->patno = 2;
                }
            }
        }
    }

    i = (Uint8)(pActwk->userflag.b.h & 112) >> 4;
    ((Sint16 *)pActwk)[31] = branko1_initbl[i].angle;
    ((Sint16 *)pActwk)[33] = branko1_initbl[i].accel;
    ((Sint16 *)pActwk)[28] = branko1_initbl[i].area1;
    ((Sint16 *)pActwk)[30] = branko1_initbl[i].area2;

    branko1_move(pActwk);
}

static void branko1_move(sprite_status *pActwk) {
    Sint32 xwk, ywk;

    xwk = pActwk->xposi.l;
    ywk = pActwk->yposi.l;
    branko1_posiset(pActwk);
    if (pActwk->actfree[5] == pActwk->actfree[18]) {
        pActwk->xspeed.w = (pActwk->xposi.l - xwk) >> 8;
        pActwk->yspeed.w = (pActwk->yposi.l - ywk) >> 8;
        branko1_ridechk(pActwk);
    }
}

static void branko1_posiset(sprite_status *pActwk) {
    Sint16 spdwk, sinwk, coswk;
    Uint32 lSinwk, lCoswk;
    ushort_union direc;

    if (pActwk->actfree[19] == 0) {
        spdwk = ((Sint16 *)pActwk)[26] + ((Sint16 *)pActwk)[33];

        ((Sint16 *)pActwk)[26] = spdwk;
        ((Sint16 *)pActwk)[31] += spdwk;
        if (((Sint16 *)pActwk)[28] == spdwk) {
            pActwk->actfree[19] = 255;
        }
    } else {
        spdwk = ((Sint16 *)pActwk)[26] - ((Sint16 *)pActwk)[33];

        ((Sint16 *)pActwk)[26] = spdwk;
        ((Sint16 *)pActwk)[31] += spdwk;
        if (((Sint16 *)pActwk)[30] == spdwk) {
            pActwk->actfree[19] = 0;
        }
    }

    direc.w = ((Uint16 *)pActwk)[31];
    sinset(direc.b.h, &sinwk, &coswk);
    spdwk = (Sint16)((char *)pActwk)[64] << 4;
    lSinwk = sinwk * spdwk;
    lCoswk = coswk * spdwk;
    sinwk = (Sint16)((Sint32)lSinwk >> 4) >> 4;
    coswk = (Sint16)((Sint32)lCoswk >> 4) >> 4;
    pActwk->yposi.w.h = coswk + ((Sint16 *)pActwk)[27];
    pActwk->xposi.w.h = sinwk + ((Sint16 *)pActwk)[29];
}

static void branko1_ridechk(sprite_status *pActwk) {
    sprite_status *pPlayerwk;
    Sint32 i;

    for (i = 0, pPlayerwk = &actwk[0]; i < 2; ++i) {
        ;
        pActwk->sprvsize = 8;

        if (ridechk(pActwk, pPlayerwk) == 0) {
            pActwk->sprvsize = 12;

            if (ridechk(pActwk, pPlayerwk) == 0)
                goto label1;
        }

        pActwk->sprvsize = 8;
        if (pActwk->yposi.w.h >= pPlayerwk->yposi.w.h) {

            pPlayerwk->yposi.w.h = pActwk->yposi.w.h -
                                   (Sint16)((char *)pActwk)[23] -
                                   (Sint16)((char *)pPlayerwk)[23] + 2;
        }

    label1:
        pActwk->sprvsize = 8;
        pPlayerwk = &actwk[1];
    }
}
