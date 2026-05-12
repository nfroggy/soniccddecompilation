#include "../equ.h"
#include "branko7.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"
#include "../ridechk.h"

#if defined(R73C) || defined(R73D)
#define SPRITE_BRANKO7_BASE 403
#else
#define SPRITE_BRANKO7_BASE 404
#endif

static void branko7_ridechk(sprite_status *pActwk);
static void branko7_init(sprite_status *pActwk);
static void branko7_move(sprite_status *pActwk);
static void branko7_posiset(sprite_status *pActwk);
static void branko7_posiset2(sprite_status *pActwk, Uint8 dirwk);

static Sint16 branko7_initbl[32] = {
    -16384, 2,  256,  -256, -16384, 2,  256,  -256, -16384, -2, -256, 256,
    -24576, 3,  384,  -384, -32768, -2, -256, 256,  -32768, 2,  256,  -256,
    -32768, -1, -256, 0,    -32768, 1,  256,  0};
static sprite_pattern pat0 = {1, {{-8, -8, 0, SPRITE_BRANKO7_BASE}}};
static sprite_pattern pat1 = {1, {{-32, -8, 0, SPRITE_BRANKO7_BASE + 1}}};
static sprite_pattern pat2 = {1, {{-8, -8, 0, SPRITE_BRANKO7_BASE + 2}}};
sprite_pattern *branko7pat[3] = {&pat0, &pat1, &pat2};
static void (*branko7_act_tbl[2])(sprite_status *) = {&branko7_init,
                                                      &branko7_move};

void branko7(sprite_status *pActwk) {
    branko7_act_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s00(pActwk, ((Sint16 *)pActwk)[29]);
}

void branko7_ridechk(sprite_status *pActwk) {
    pActwk->sprvsize = 8;

    if (!hitchk(pActwk, &actwk[0])) {
        pActwk->sprvsize = 12;

        if (!hitchk(pActwk, &actwk[0])) {
            pActwk->sprvsize = 8;
            return;
        }
    }

    pActwk->sprvsize = 8;

    if (pActwk->yposi.w.h >= actwk[0].yposi.w.h) {
        actwk[0].yposi.w.h = pActwk->yposi.w.h -
                             (Sint16)(char)pActwk->sprvsize -
                             (Sint16)(char)actwk[0].sprvsize + 2;
    }
}

void branko7_init(sprite_status *pActwk) {
    Sint32 flagwk, flagcnt;
    sprite_status *pNewact;
    Sint16 *pIniTbl;

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->patbase = branko7pat;
    pActwk->sproffset = 870;
    pActwk->sprhsize = 32;
    pActwk->sprvsize = 8;

    if (!pActwk->actfree[18]) {
        ((Sint16 *)pActwk)[29] = pActwk->xposi.w.h;
        ((Sint16 *)pActwk)[27] = pActwk->yposi.w.h;

        flagwk = flagcnt = 0;
        flagwk = pActwk->userflag.b.h & 15;
        pActwk->actfree[5] = flagwk;

        for (flagcnt = flagwk - 1; flagcnt >= 0; --flagcnt, --flagwk) {

            if (actwkchk(&pNewact) == 0) {
                pNewact->actno = 32;
                ((Sint16 *)pNewact)[29] = ((Sint16 *)pActwk)[29];

                ((Sint16 *)pNewact)[27] = ((Sint16 *)pActwk)[27];

                pNewact->userflag.b.h = pActwk->userflag.b.h;
                pNewact->actfree[5] = pActwk->actfree[5];
                pNewact->actfree[18] = flagwk;
                pNewact->patno = 2;

                if (pActwk->actfree[5] == flagwk) {
                    pNewact->patno = 1;
                    pNewact->colino = 169;
                }
            }
        }
    }

    flagwk = pActwk->userflag.b.h & 240;
    flagwk >>= 1;
    pIniTbl = &branko7_initbl[flagwk / 2];
    ((Sint16 *)pActwk)[31] = *pIniTbl++;
    ((Sint16 *)pActwk)[33] = *pIniTbl++;
    ((Sint16 *)pActwk)[28] = *pIniTbl++;
    ((Sint16 *)pActwk)[30] = *pIniTbl++;

    branko7_move(pActwk);
}

void branko7_move(sprite_status *pActwk) {
    Sint32 lXpos, lYpos;

    lXpos = pActwk->xposi.l;
    lYpos = pActwk->yposi.l;

    branko7_posiset(pActwk);

    if (pActwk->actfree[5] == pActwk->actfree[18]) {
        pActwk->xspeed.w = pActwk->xposi.l - lXpos >> 8;
        pActwk->yspeed.w = pActwk->yposi.l - lYpos >> 8;
        branko7_ridechk(pActwk);
    }
}

void branko7_posiset(sprite_status *pActwk) {
    Sint16 spdwk;

    if (!pActwk->actfree[19]) {
        spdwk = ((Sint16 *)pActwk)[26] + ((Sint16 *)pActwk)[33];

        ((Sint16 *)pActwk)[26] = spdwk;
        ((Sint16 *)pActwk)[31] += spdwk;
        if (spdwk == ((Sint16 *)pActwk)[28]) {
            pActwk->actfree[19] = 1;
        }
    } else {
        spdwk = ((Sint16 *)pActwk)[26] - ((Sint16 *)pActwk)[33];

        ((Sint16 *)pActwk)[26] = spdwk;
        ((Sint16 *)pActwk)[31] += spdwk;
        if (spdwk == ((Sint16 *)pActwk)[30]) {
            pActwk->actfree[19] = 0;
        }
    }

    branko7_posiset2(pActwk, ((char *)pActwk)[63]);
}

void branko7_posiset2(sprite_status *pActwk, Uint8 dirwk) {
    Sint16 usrwk;
    int_union sinwk, coswk;

    sinwk.l = coswk.l = 0;
    sinset(dirwk, &sinwk.w.l, &coswk.w.l);
    usrwk = ((char *)pActwk)[64];
    usrwk <<= 4;
    sinwk.l *= usrwk;
    coswk.l *= usrwk;

    sinwk.l >>= 4;
    sinwk.w.l >>= 4;
    coswk.l >>= 4;
    coswk.w.l >>= 4;
    pActwk->yposi.w.h = coswk.w.l + ((Sint16 *)pActwk)[27];
    pActwk->xposi.w.h = sinwk.w.l + ((Sint16 *)pActwk)[29];
}
