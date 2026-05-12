#include "../equ.h"
#include "harir4.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../ridechk.h"
#include "coli4.h"

#if defined(R41A) || defined(R42A)
#define SPRITE_HARIR4_BASE 443
#elif defined(R41B) || defined(R41C)
#define SPRITE_HARIR4_BASE 431
#elif defined(R41D)
#define SPRITE_HARIR4_BASE 466
#else
#define SPRITE_HARIR4_BASE 429
#endif

sprite_pattern harir4_pat0 = {1, {{-16, -16, 0, SPRITE_HARIR4_BASE}}};
sprite_pattern harir4_pat1 = {1, {{-16, -16, 0, SPRITE_HARIR4_BASE + 1}}};
sprite_pattern *harir4pat[2] = {&harir4_pat0, &harir4_pat1};

void harir4(sprite_status *pActwk) {
    sprite_status *pRideactwk;

    void (*harir4_acttbl[2])(sprite_status *) = {&harir4_init, &harir4_move};

    harir4_acttbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);

    if (((Sint16 *)pActwk)[28]) {
        pRideactwk = &actwk[((Sint16 *)pActwk)[28]];
        if (!pRideactwk->actno) {
            frameout(pActwk);
            return;
        }

        pActwk->xposi.w.h = pRideactwk->xposi.w.h + (Sint16)pActwk->actfree[14];
        pActwk->yposi.w.h = pRideactwk->yposi.w.h + (Sint16)pActwk->actfree[15];
    }

    frameout_s00(pActwk, ((Sint16 *)pActwk)[29]);
}

void harir4_ridechk(sprite_status *pActwk) {
    if (pActwk->patno) {
        hitchk(pActwk, &actwk[0]);
        return;
    }

    if (hitchk(pActwk, &actwk[0])) {
        if (pActwk->cddat & 8) {
            if (!plpower_a) {
                if (!plpower_m) {
                    if (actwk[0].r_no0 < 4) {
                        if (!((Sint16 *)&actwk[0])[26]) {
                            actwk[0].yposi.l -= actwk[0].yspeed.w << 8;
                            playdamageset(&actwk[0], pActwk);
                        }
                    }
                }
            }
        }
    }
}

void harir4_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 928;
    pActwk->sprpri = 4;
    pActwk->patbase = harir4pat;
    if (!((Sint16 *)pActwk)[29]) {
        ((Sint16 *)pActwk)[29] = pActwk->xposi.w.h;
    }
    ((Sint16 *)pActwk)[27] = pActwk->yposi.w.h;
    pActwk->sprhsize = 18;
    pActwk->sprvsize = 12;
    if (pActwk->userflag.b.h) {
        pActwk->patno = 1;
        pActwk->colino = 131;
    }

    harir4_move(pActwk);
}

void harir4_move(sprite_status *pActwk) {
    if (pActwk->userflag.b.h != 1) {
        harir4_sub(pActwk);
        pActwk->yposi.w.h = pActwk->actfree[17] + ((Sint16 *)pActwk)[27];
    }

    harir4_ridechk(pActwk);
}

void harir4_sub(sprite_status *pActwk) {
    if (pActwk->actfree[16]) {
        if (--pActwk->actfree[16])
            return;
        if (pActwk->actflg & 128)
            soundset(183);
    }

    if (pActwk->actfree[18]) {

        if (pActwk->actfree[17] < 8) {
            pActwk->actfree[17] = 0;
            pActwk->actfree[18] = 0;
            pActwk->actfree[16] = 60;
        } else
            pActwk->actfree[17] -= 8;
    } else {
        if ((char)(pActwk->actfree[17] += 8) >= 32) {
            pActwk->actfree[17] = 32;
            pActwk->actfree[18] = 1;
            pActwk->actfree[16] = 60;
        }
    }
}
