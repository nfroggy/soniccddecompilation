#include "../equ.h"
#include "kowasi4.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../ridechk.h"

static void m_init(sprite_status *pActwk);
static void m_wait(sprite_status *pActwk);
static void m_down(sprite_status *pActwk);
static void m_make(sprite_status *pActwk);

static sprite_pattern pat00 = {1, {{-16, -16, 0, 484}}};
static sprite_pattern pat01 = {1, {{-8, -8, 0, 485}}};
sprite_pattern *pat_kowasi4[2] = {&pat00, &pat01};
static void (*kowasi4_act_tbl[3])(sprite_status *) = {&m_init, &m_wait,
                                                      &m_down};

void kowasi4(sprite_status *pActwk) {
    kowasi4_act_tbl[pActwk->r_no0 / 2](pActwk);
}

static void m_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 1;
    pActwk->sprhs = pActwk->sprhsize = pActwk->sprvsize = 16;
    pActwk->colino = 243;
    pActwk->sproffset = 17252;
    pActwk->patbase = pat_kowasi4;

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
    if (actwk[0].yposi.w.h - pActwk->yposi.w.h < -223) {

        frameout(pActwk);
    } else {
        actionsub(pActwk);
    }
}

static void m_make(sprite_status *pActwk) {
    Sint16 tbl0dposi[8] = {-8, -8, 16, 0, 0, 16, 16, 16};
    Sint32 tbl0speed[24] = {
        -84144,  -393216, 84144,   -393216, -189326, -262144, 189326,  -262144,
        -252434, -84144,  -189326, -189326, -252434, 84144,   -189326, 189326,
        252434,  -84144,  189326,  -189326, 252434,  84144,   189326,  189326};
    Sint16 *pPosi;
    Sint16 spdwk1, spdwk2;
    Sint16 yspdwk;
    Sint32 *pSpeed;
    Sint32 i;
    sprite_status *pNewact;

    soundset(176);

    pPosi = tbl0dposi;
    pSpeed = tbl0speed;
    spdwk1 = spdwk2 = ((Sint16 *)pActwk)[23];
    if (spdwk1 < 0) {
        spdwk1 *= -1;
    }

    if ((yspdwk = ((Sint16 *)pActwk)[25]) < 0) {
        yspdwk *= -1;
    }

    if (yspdwk <= spdwk1) {
        pSpeed += 8;
        if (spdwk2 < 0) {
            pSpeed += 8;
        }
    }

    actwk[0].xspeed.w = actwk[0].yspeed.w = actwk[0].mspeed.w = 0;
    pNewact = pActwk;
    for (i = 0; i < 4; ++i) {
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

        pNewact->patno = 1;
        pNewact->sprhs = pNewact->sprhsize = pNewact->sprvsize = 8;
        pNewact->xposi.w.h += *pPosi++;
        pNewact->yposi.w.h += *pPosi++;
        *(Sint32 *)&pNewact->actfree[0] = *pSpeed++;
        *(Sint32 *)&pNewact->actfree[4] = *pSpeed++;
    }

    m_down(pActwk);
}
