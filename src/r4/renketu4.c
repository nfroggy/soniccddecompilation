#include "../equ.h"
#include "renketu4.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"
#include "../ridechk.h"

#if defined(R41C)
#define SPRITE_RENKETU4_BASE 445
#elif defined(R41D)
#define SPRITE_RENKETU4_BASE 431
#elif defined(R42C) || defined(R42D)
#define SPRITE_RENKETU4_BASE 512
#else
#define SPRITE_RENKETU4_BASE 496
#endif

sprite_pattern renketu4_pat0 = {1, {{-32, -8, 0, SPRITE_RENKETU4_BASE}}};
sprite_pattern renketu4_pat1 = {1, {{-8, -8, 0, SPRITE_RENKETU4_BASE + 1}}};
sprite_pattern *renketu4pat[2] = {&renketu4_pat0, &renketu4_pat1};

void renketu4(sprite_status *pActwk) {
    Uint16 xwk1;
    Uint16 xwk2;
    void (*renketu4_acttbl[2])(sprite_status *) = {&renketu4_init,
                                                   &renketu4_move};

    renketu4_acttbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);

    xwk1 = ((Uint16 *)pActwk)[29] & 65408;
    xwk2 = (Uint16)scra_h_posit.w.h - 128 & 65408;
    if ((Uint16)(xwk1 - xwk2) > 640) {
        frameout(pActwk);
    }
}

void renketu4_ridechk(sprite_status *pActwk) {
    if (ridechk(pActwk, &actwk[0])) {

        actwk[0].yposi.w.h = pActwk->yposi.w.h - (Sint16)pActwk->sprvsize -
                             (Sint16)actwk[0].sprvsize + 2;
    }
}

void renketu4_init(sprite_status *pActwk) {
    char renketu4_tbl[18] = {32, 0, 0, 0,  -32, 0,   24, 1,   16,
                             1,  8, 1, -8, 1,   -16, 1,  -24, 1};
    sprite_status *pNewactwk;
    Uint8 i;

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;

    if (stageno.b.l == 2)
        pActwk->sproffset = 17408;
    else
        pActwk->sproffset = 17216;

    pActwk->patbase = renketu4pat;
    pActwk->sprhsize = 32;
    pActwk->sprvsize = 8;

    if (!pActwk->actfree[18]) {
        ((Sint16 *)pActwk)[29] = pActwk->xposi.w.h;
        ((Sint16 *)pActwk)[27] = pActwk->yposi.w.h;

        for (i = 1; i <= 8; ++i) {
            if (actwkchk(&pNewactwk) == 0) {
                pNewactwk->actno = 62;
                pNewactwk->userflag.b.h = pActwk->userflag.b.h;
                pNewactwk->actfree[18] = i;
                ((Sint16 *)pNewactwk)[29] = ((Sint16 *)pActwk)[29];
                ((Sint16 *)pNewactwk)[27] = ((Sint16 *)pActwk)[27];
            }
        }
    }

    pActwk->actfree[21] = renketu4_tbl[pActwk->actfree[18] * 2];
    pActwk->patno = renketu4_tbl[pActwk->actfree[18] * 2 + 1];

    pActwk->sprpri = 1;
    if (pActwk->patno) {
        pActwk->sprpri = 3;
        pActwk->sproffset -= 16384;
    }

    if (pActwk->userflag.b.h & 16)
        ((Sint16 *)pActwk)[26] = -256;
    else
        ((Sint16 *)pActwk)[26] = 256;

    renketu4_move(pActwk);
}

void renketu4_move(sprite_status *pActwk) {
    Uint16 wD0, wD1;
    int_union lD0, lD1, lD4, lD5;
    Sint16 wk;

    swchk(pActwk);

    ((Sint16 *)pActwk)[33] += ((Sint16 *)pActwk)[26];

    sinset(pActwk->actfree[21], (Sint16 *)&wD0, (Sint16 *)&wD1);

    lD0.l = (Sint16)wD0 * 6 >> 4;
    lD1.l = (Sint16)wD1 * 6 >> 4;
    wk = lD0.w.h;
    lD0.w.h = lD0.w.l;
    lD0.w.l = wk;
    wk = lD1.w.h;
    lD1.w.h = lD1.w.l;
    lD1.w.l = wk;

    lD4.w.l = lD5.w.l = 0;
    lD4.w.h = ((Sint16 *)pActwk)[27];
    lD5.w.h = ((Sint16 *)pActwk)[29];
    lD0.l += lD4.l;
    lD1.l += lD5.l;

    pActwk->yspeed.w = (lD0.l - pActwk->yposi.l) << 8 >> 16;
    pActwk->xspeed.w = (lD1.l - pActwk->xposi.l) << 8 >> 16;
    pActwk->yposi.l = lD0.l;
    pActwk->xposi.l = lD1.l;

    if (!pActwk->patno)
        renketu4_ridechk(pActwk);
}

void swchk(sprite_status *pActwk) {
    if (!(pActwk->userflag.b.h & 128)) {
        if (switchflag[pActwk->userflag.b.h & 15] & 128) {
            if (pActwk->actfree[19])
                return;
            ((Sint16 *)pActwk)[26] *= -1;
            pActwk->actfree[19] = 1;
            return;
        }
        pActwk->actfree[19] = 0;
    }
}
