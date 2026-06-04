#include "../equ.h"
#include "osumizu.h"
#include "../action.h"
#include "../actset.h"
#include "playsub4.h"

typedef struct {
    Sint16 timer;
    Sint32 x_velocity;
    Sint16 spawn_x;
} osumizu_work;

static osumizu_work *osumizu_work_get(sprite_status *pActwk) {
    return (osumizu_work *)pActwk->actfree;
}

sprite_pattern osumizu_pat0 = {1, {{-8, -16, 0, 471}}};
sprite_pattern osumizu_pat1 = {1, {{-16, -8, 0, 472}}};
sprite_pattern osumizu_pat2 = {1, {{-16, -8, 0, 473}}};
sprite_pattern osumizu_pat3 = {1, {{-16, -8, 0, 474}}};
sprite_pattern osumizu_pat4 = {1, {{-16, -8, 0, 475}}};
sprite_pattern *pat_osumizu[1] = {&osumizu_pat0};
sprite_pattern *pat_osumizu1[4] = {&osumizu_pat1, &osumizu_pat2, &osumizu_pat3,
                                   &osumizu_pat4};
Uint8 osumizu_chg0[6] = {9, 0, 1, 2, 3, 255};
Uint8 *osumizu_chg[1] = {osumizu_chg0};

void osumizu(sprite_status *pActwk) {
    osumizu_work *work = osumizu_work_get(pActwk);
    sprite_status *pNewactwk;
    osumizu_work *new_work;

    if (!(pActwk->userflag.b.h & 128)) {

        if (!pActwk->r_no0) {

            pActwk->r_no0 += 2;

            pActwk->actflg |= 4;
            pActwk->sprpri = 3;
            pActwk->sprhsize = 8;
            pActwk->sprvsize = 16;
            pActwk->sproffset = 17176;
            pActwk->patbase = pat_osumizu;

            if (pActwk->userflag.b.h) {
                pActwk->actflg |= 1;
                pActwk->cddat |= 1;

                work->x_velocity = 196608;
                work->spawn_x = -32;
            } else {
                work->x_velocity = -196608;
                work->spawn_x = 32;
            }
            work->spawn_x += pActwk->xposi.w.h;
        }

        if (--work->timer < 0) {
            work->timer = 30;

            if (actwkchk(&pNewactwk) == 0) {
                new_work = osumizu_work_get(pNewactwk);

                pNewactwk->actno = pActwk->actno;
                pNewactwk->userflag.b.h = -1;
                pNewactwk->xposi.w.h = work->spawn_x;
                pNewactwk->yposi.w.h = pActwk->yposi.w.h + 4;
                new_work->x_velocity = work->x_velocity;
                pNewactwk->actflg = pActwk->actflg;
                pNewactwk->sprpri = pActwk->sprpri + 1;
                pNewactwk->sprhsize = 32;
                pNewactwk->sprvsize = 6;
                pNewactwk->sproffset = 792;
                pNewactwk->patbase = pat_osumizu1;
                pNewactwk->cddat = pActwk->cddat;
            }
        }
        actionsub(pActwk);
        frameout_s(pActwk);
    } else {

        if ((Uint16)(actwk[0].yposi.w.h - pActwk->yposi.w.h + 12) < 24) {
            if ((Uint16)(actwk[0].xposi.w.h - pActwk->xposi.w.h + 20) < 40) {
                actwk[0].mspeed.w = work->x_velocity >> 8;
            }
        }
        pActwk->xposi.l += work->x_velocity;

        patchg(pActwk, osumizu_chg);
        actionsub(pActwk);
        frameout_s(pActwk);
    }
}
