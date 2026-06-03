#include <stddef.h>

#include "../equ.h"
#include "seesaw6.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../playsub.h"
#include "../ridechk.h"

#pragma pack(push, 1)
typedef struct {
    union {
        Uint16 left_slave_index;
        Uint16 parent_index;
    };
    Uint16 right_slave_index;
    Sint16 timer;
    Uint8 unused6[15];
    Uint8 pressed;
} seesaw6_work;
#pragma pack(pop)

_Static_assert(offsetof(seesaw6_work, left_slave_index) == 0,
               "seesaw6_work.left_slave_index offset");
_Static_assert(offsetof(seesaw6_work, parent_index) == 0,
               "seesaw6_work.parent_index offset");
_Static_assert(offsetof(seesaw6_work, right_slave_index) == 2,
               "seesaw6_work.right_slave_index offset");
_Static_assert(offsetof(seesaw6_work, timer) == 4,
               "seesaw6_work.timer offset");
_Static_assert(offsetof(seesaw6_work, pressed) == 21,
               "seesaw6_work.pressed offset");
_Static_assert(sizeof(seesaw6_work) <= sizeof(((sprite_status *)0)->actfree),
               "seesaw6_work fits in actfree");

static seesaw6_work *seesaw6_get_work(sprite_status *pActwk) {
    return (seesaw6_work *)pActwk->actfree;
}

#if defined(R61B) || defined(R62B)
#define SPRITE_SEESAW6_BASE 481
#elif defined(R63C) || defined(R63D)
#define SPRITE_SEESAW6_BASE 450
#else
#define SPRITE_SEESAW6_BASE 487
#endif

void (*seesaw6_tbl[3])(sprite_status *) = {&m_init, &m_move, &m_up};
Uint8 seesaw_pat_00[4] = {2, 0, 1, 255};
Uint8 seesaw_pat_01[5] = {2, 1, 2, 3, 255};
Uint8 seesaw_pat_02[4] = {2, 2, 3, 255};
Uint8 seesaw_pat_03[4] = {2, 4, 5, 255};
Uint8 seesaw_pat_04[6] = {2, 4, 5, 6, 7, 255};
Uint8 seesaw_pat_05[4] = {2, 6, 7, 255};
Uint8 *seesaw_pchg[6] = {seesaw_pat_00, seesaw_pat_01, seesaw_pat_02,
                         seesaw_pat_03, seesaw_pat_04, seesaw_pat_05};
sprite_pattern seesawPat00 = {1, {{-24, -24, 0, SPRITE_SEESAW6_BASE}}};
sprite_pattern seesawPat01 = {1, {{-24, -24, 0, SPRITE_SEESAW6_BASE + 1}}};
sprite_pattern seesawPat02 = {1, {{-24, -24, 0, SPRITE_SEESAW6_BASE + 2}}};
sprite_pattern seesawPat03 = {1, {{-24, -24, 0, SPRITE_SEESAW6_BASE + 3}}};
sprite_pattern seesawPat04 = {1, {{-24, -24, 0, SPRITE_SEESAW6_BASE + 4}}};
sprite_pattern seesawPat05 = {1, {{-24, -24, 0, SPRITE_SEESAW6_BASE + 5}}};
sprite_pattern seesawPat06 = {1, {{-24, -24, 0, SPRITE_SEESAW6_BASE + 6}}};
sprite_pattern seesawPat07 = {1, {{-24, -24, 0, SPRITE_SEESAW6_BASE + 7}}};
sprite_pattern seesawPat08 = {1, {{-24, -8, 0, SPRITE_SEESAW6_BASE + 8}}};
sprite_pattern seesawPat09 = {1, {{-16, -7, 0, SPRITE_SEESAW6_BASE + 9}}};
sprite_pattern *pat_seesaw6[10] = {
    &seesawPat00, &seesawPat01, &seesawPat02, &seesawPat03, &seesawPat04,
    &seesawPat05, &seesawPat06, &seesawPat07, &seesawPat08, &seesawPat09};

void seesaw6(sprite_status *pActwk) {
    if (pActwk->userflag.b.h != 0) {
        slave(pActwk);
    } else {

        seesaw6_tbl[pActwk->r_no0 / 2](pActwk);
        actionsub(pActwk);
        frameout_s(pActwk);
    }
}

void m_init(sprite_status *pActwk) {
    seesaw6_work *work = seesaw6_get_work(pActwk);
    sprite_status *pActfree;

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->sprhs = 24;
    pActwk->sprhsize = 24;
    pActwk->sprvsize = 24;
    pActwk->sproffset = 952;
    pActwk->patbase = pat_seesaw6;

    if (actwkchk(&pActfree) != 0) {
        frameout(pActwk);
        return;
    }
    m_ini_s(pActwk, pActfree);

    work->left_slave_index = pActfree - actwk;
    pActfree->xposi.w.h -= 40;
    pActfree->yposi.w.h -= 24;
    if (actwkchk(&pActfree) != 0) {
        frameout(pActwk);
        return;
    }
    m_ini_s(pActwk, pActfree);

    work->right_slave_index = pActfree - actwk;
    pActfree->xposi.w.h += 40;
    pActfree->yposi.w.h += 24;
    pActfree->actflg |= 1;
    pActfree->cddat |= 1;
}

void m_ini_s(sprite_status *pActwk, sprite_status *pActfree) {
    pActfree->actno = pActwk->actno;
    pActfree->actflg = pActwk->actflg;
    pActfree->sprpri = pActwk->sprpri;
    pActfree->sproffset = pActwk->sproffset;
    pActfree->patbase = pActwk->patbase;
    pActfree->xposi.w.h = pActwk->xposi.w.h;
    pActfree->yposi.w.h = pActwk->yposi.w.h;
    pActfree->userflag.b.h = -1;
    pActfree->sprhs = 16;
    pActfree->sprhsize = 16;
    pActfree->sprvsize = 8;
    pActfree->patno = 9;

    seesaw6_get_work(pActfree)->parent_index = pActwk - actwk;
    seesaw6_get_work(pActwk)->timer = 120;
}

void m_move(sprite_status *pActwk) {
    seesaw6_work *work = seesaw6_get_work(pActwk);
    Uint16 wActwkNo;

    if (pActwk->yspeed.w == 0)
        m_stay(pActwk);
    else
        m_fall(pActwk);

    wActwkNo = work->right_slave_index;
    ridechk(&actwk[wActwkNo], &actwk[0]);
    actionsub(&actwk[wActwkNo]);

    wActwkNo = work->left_slave_index;
    if (ridechk(&actwk[wActwkNo], &actwk[0]) != 0)
        seesaw6_get_work(&actwk[wActwkNo])->pressed = 255;
    else
        seesaw6_get_work(&actwk[wActwkNo])->pressed = 0;
    actionsub(&actwk[wActwkNo]);

    if (seesaw6_get_work(&actwk[wActwkNo])->pressed == 0) {
        patchg(pActwk, seesaw_pchg);
    } else {
        pActwk->r_no0 = 4;
        work->timer = 3;
        pActwk->patno = 8;
    }
}

void m_stay(sprite_status *pActwk) {
    seesaw6_work *work = seesaw6_get_work(pActwk);
    Sint16 iwk;

    iwk = work->timer;
    if (work->timer >= 0) {

        --work->timer;
        if (work->timer < 0) {
            pActwk->yspeed.w = 256;
        } else {
            if (work->timer != 60)
                return;
        }

        ++pActwk->mstno.b.h;
    }
}

void m_fall(sprite_status *pActwk) {
    seesaw6_work *work = seesaw6_get_work(pActwk);
    sprite_status *pActSu;
    sprite_status *pActSd;
    Uint16 wYspd;
    Sint16 iD0, iD1;

    pActSu = &actwk[work->left_slave_index];
    pActSd = &actwk[work->right_slave_index];

    wYspd = pActwk->yspeed.b.h;
    pActwk->yposi.w.h += wYspd;
    pActSu->yposi.w.h += wYspd;
    pActSd->yposi.w.h += wYspd;

    iD0 = pActSd->sprhsize;
    if (pActSd->xposi.w.h >= pActwk->xposi.w.h)
        iD0 = -iD0;

    iD0 += pActwk->xposi.w.h;
    iD1 = emycol_d2(pActSd, iD0);
    if (iD1 > 0)
        return;

    pActwk->yposi.w.h += iD1;
    pActSu->yposi.w.h += iD1;
    pActSd->yposi.w.h += iD1;
    pActwk->yspeed.w = 0;
}

void m_up(sprite_status *pActwk) {
    seesaw6_work *work = seesaw6_get_work(pActwk);
    sprite_status *pActSd, *pActSu;
    Uint16 wwk_u, wwk_d;
    pActSd = &actwk[work->right_slave_index];
    pActSd->yposi.w.h -= 24;
    pActwk->yposi.w.h -= 12;
    --work->timer;
    if (work->timer < 0) {

        pActwk->r_no0 = 2;
        pActwk->yspeed.w = 0;
        work->timer = 120;
        wwk_d = work->right_slave_index;
        wwk_u = work->left_slave_index;
        work->right_slave_index = wwk_u;
        work->left_slave_index = wwk_d;
        if (pActwk->mstno.b.h > 2)
            pActwk->mstno.b.h = 0;
        else
            pActwk->mstno.b.h = 3;
        pActwk->mstno.b.l = -1;
    }

    actionsub(pActSd);
    pActSu = &actwk[work->left_slave_index];
    actionsub(pActSu);
}

void slave(sprite_status *pActwk) {
    sprite_status *pActwk2;

    pActwk2 = &actwk[seesaw6_get_work(pActwk)->parent_index];
    if (pActwk2->actno != 44)
        frameout(pActwk);
}
