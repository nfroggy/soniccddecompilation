#include "../equ.h"
#include "kowasi5.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../ridechk.h"

static void m_init(sprite_status *pActwk);
static void m_wait(sprite_status *pActwk);
static void m_down(sprite_status *pActwk);
static void m_make(sprite_status *pActwk);

static sprite_pattern pat00 = {1, {{-8, -24, 0, 487}}};
static sprite_pattern pat01 = {1, {{-8, -24, 0, 488}}};
static sprite_pattern pat02 = {1, {{-8, -8, 0, 489}}};
static sprite_pattern pat03 = {1, {{-8, -8, 0, 490}}};
static sprite_pattern pat04 = {1, {{-8, -8, 0, 491}}};
static sprite_pattern pat05 = {1, {{-8, -8, 0, 492}}};
sprite_pattern *pat_kowasi[6] = {&pat00, &pat01, &pat02,
                                 &pat03, &pat04, &pat05};
static void (*kowasi_act_tbl[3])(sprite_status *) = {&m_init, &m_wait, &m_down};

void kowasi(sprite_status *pActwk) {
    kowasi_act_tbl[pActwk->r_no0 / 2](pActwk);
}

static void m_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 1;
    pActwk->sprhs = pActwk->sprhsize = 8;
    pActwk->sprvsize = 8;
    pActwk->colino = 243;
    pActwk->sproffset = 17575;
    pActwk->patbase = pat_kowasi;

    pActwk->patno = pActwk->userflag.b.h & 1;
    if (pActwk->userflag.b.h == 2) {
        pActwk->actflg |= 1;
        pActwk->cddat |= 1;
    }

    m_wait(pActwk);
}

static void m_wait(sprite_status *pActwk) {
    if (pActwk->colicnt) {
        pActwk->colino = pActwk->colicnt = 0;
        pActwk->r_no0 += 2;
        ((Sint16 *)pActwk)[23] = actwk[0].xspeed.w;
        ((Sint16 *)pActwk)[25] = actwk[0].yspeed.w;
        if (hitchk(pActwk, &actwk[0])) {
            ride_on_clr(pActwk, &actwk[0]);
        }

        m_make(pActwk);
        return;
    }

    hitchk(pActwk, &actwk[0]);
    actionsub(pActwk);
    frameout_s(pActwk);
}

static void m_down(sprite_status *pActwk) {
    *(Sint32 *)&pActwk->actfree[4] += 16384;
    pActwk->xposi.l += *(Sint32 *)&pActwk->actfree[0];
    pActwk->yposi.l += *(Sint32 *)&pActwk->actfree[4];
    if (actwk[0].yposi.w.h - pActwk->yposi.w.h <= -224) {

        frameout(pActwk);
    } else {
        actionsub(pActwk);
    }
}

static void m_make(sprite_status *pActwk) {
    Uint8 *pPat;
    Sint16 *pPosiy;
    Sint32 *pSpeed;
    Sint32 spdwk;
    Sint32 i;
    sprite_status *pNewact;
    static Sint16 tbl0dposiy[3] = {-16, 0, 16};
    static Uint8 tbl0pat[6] = {2, 3, 4, 5, 5, 5};
    static Sint32 tbl0speed[6] = {-194180, -84144, -252434, 0, -194180, 84144};

    soundset(176);

    pPosiy = tbl0dposiy;
    pPat = tbl0pat;
    pSpeed = tbl0speed;

    if (pActwk->userflag.b.h & 1) {
        pPat += 3;
    }

    actwk[0].xspeed.w >>= 1;

    pNewact = pActwk;
    for (i = 0; i < 3; ++i) {

        if (i != 0) {
            if (actwkchk(&pNewact) != 0)
                break;

            pNewact->actno = pActwk->actno;
            pNewact->r_no0 = pActwk->r_no0;
            pNewact->xposi.w.h = pActwk->xposi.w.h;
            pNewact->yposi.w.h = pActwk->yposi.w.h;
            pNewact->actflg = pActwk->actflg;
            pNewact->sprpri = pActwk->sprpri;
            pNewact->patbase = pActwk->patbase;
            pNewact->sproffset = pActwk->sproffset;
        }

        pNewact->patno = *pPat++;
        pNewact->sprhs = pNewact->sprhsize = pNewact->sprvsize = 8;
        pNewact->yposi.w.h += *pPosiy++;
        spdwk = *pSpeed++;
        *(Sint32 *)&pNewact->actfree[4] = *pSpeed++;

        if (actwk[0].xspeed.w < 0) {
            spdwk *= -1;
        }

        *(Sint32 *)&pNewact->actfree[0] = spdwk;
    }

    m_down(pActwk);
}
