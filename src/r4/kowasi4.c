#include "../equ.h"
#include "kowasi4.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../ridechk.h"
#include <stddef.h>

static void m_init(sprite_status *pActwk);
static void m_wait(sprite_status *pActwk);
static void m_down(sprite_status *pActwk);
static void m_make(sprite_status *pActwk);

#pragma pack(push, 1)
typedef struct {
    union {
        Sint32 x_velocity;
        Sint16 saved_player_xspeed;
    };
    union {
        Sint32 y_velocity;
        Sint16 saved_player_yspeed;
    };
} kowasi4_work;
#pragma pack(pop)

_Static_assert(offsetof(kowasi4_work, x_velocity) == 0,
               "kowasi4_work.x_velocity offset");
_Static_assert(offsetof(kowasi4_work, saved_player_xspeed) == 0,
               "kowasi4_work.saved_player_xspeed offset");
_Static_assert(offsetof(kowasi4_work, y_velocity) == 4,
               "kowasi4_work.y_velocity offset");
_Static_assert(offsetof(kowasi4_work, saved_player_yspeed) == 4,
               "kowasi4_work.saved_player_yspeed offset");
_Static_assert(sizeof(kowasi4_work) <= sizeof(((sprite_status *)0)->actfree),
               "kowasi4_work fits in actfree");

static kowasi4_work *kowasi4_work_get(sprite_status *pActwk) {
    return (kowasi4_work *)pActwk->actfree;
}

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
        kowasi4_work *work = kowasi4_work_get(pActwk);

        pActwk->colino = pActwk->colicnt = 0;
        pActwk->r_no0 += 2;
        work->saved_player_xspeed = actwk[0].xspeed.w;
        work->saved_player_yspeed = actwk[0].yspeed.w;
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
    kowasi4_work *work = kowasi4_work_get(pActwk);

    work->y_velocity += 16384;
    pActwk->xposi.l += work->x_velocity;
    pActwk->yposi.l += work->y_velocity;
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
    kowasi4_work *work = kowasi4_work_get(pActwk);

    soundset(176);

    pPosi = tbl0dposi;
    pSpeed = tbl0speed;
    spdwk1 = spdwk2 = work->saved_player_xspeed;
    if (spdwk1 < 0) {
        spdwk1 *= -1;
    }

    if ((yspdwk = work->saved_player_yspeed) < 0) {
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
        work = kowasi4_work_get(pNewact);
        work->x_velocity = *pSpeed++;
        work->y_velocity = *pSpeed++;
    }

    m_down(pActwk);
}
