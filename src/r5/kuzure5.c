#include "../equ.h"
#include "kuzure5.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"
#include "../ridechk.h"
#include <stddef.h>

#if defined(R53C) || defined(R53D)
#define SPRITE_KUZURE5_BASE 403
#else
#define SPRITE_KUZURE5_BASE 477
#endif

static void main_ini(sprite_status *pActwk);
static void main_check(sprite_status *pActwk);
static void main_wait(sprite_status *pActwk);
static void main_break(sprite_status *pActwk);
static void parts_ini(sprite_status *pActwk);
static void parts_wait(sprite_status *pActwk);
static void parts_fall(sprite_status *pActwk);

#pragma pack(push, 1)
typedef struct {
    Sint16 wait_timer;
    union {
        Sint16 break_step;
        Sint32 y_velocity;
    };
} kuzure5_work;
#pragma pack(pop)

_Static_assert(offsetof(kuzure5_work, wait_timer) == 0,
               "kuzure5_work.wait_timer offset");
_Static_assert(offsetof(kuzure5_work, break_step) == 2,
               "kuzure5_work.break_step offset");
_Static_assert(offsetof(kuzure5_work, y_velocity) == 2,
               "kuzure5_work.y_velocity offset");
_Static_assert(sizeof(kuzure5_work) <= sizeof(((sprite_status *)0)->actfree),
               "kuzure5_work fits in actfree");

static kuzure5_work *kuzure5_work_get(sprite_status *pActwk) {
    return (kuzure5_work *)pActwk->actfree;
}

static sprite_pattern pat_00 = {1, {{-8, -16, 0, SPRITE_KUZURE5_BASE}}};
static sprite_pattern pat_01 = {1, {{-16, -16, 0, SPRITE_KUZURE5_BASE + 1}}};
static sprite_pattern pat_02 = {1, {{-24, -16, 0, SPRITE_KUZURE5_BASE + 2}}};
static sprite_pattern pat_03 = {1, {{-32, -16, 0, SPRITE_KUZURE5_BASE + 3}}};
static sprite_pattern pat_04 = {1, {{-40, -16, 0, SPRITE_KUZURE5_BASE + 4}}};
static sprite_pattern pat_05 = {1, {{-48, -16, 0, SPRITE_KUZURE5_BASE + 5}}};
static sprite_pattern pat_06 = {1, {{-56, -16, 0, SPRITE_KUZURE5_BASE + 6}}};
static sprite_pattern pat_07 = {1, {{-64, -16, 0, SPRITE_KUZURE5_BASE + 7}}};
static sprite_pattern pat_08 = {1, {{-8, -8, 0, SPRITE_KUZURE5_BASE + 8}}};
static sprite_pattern pat_09 = {1, {{-8, -8, 0, SPRITE_KUZURE5_BASE + 9}}};
sprite_pattern *pat_kuzure_a[10] = {&pat_00, &pat_01, &pat_02, &pat_03,
                                    &pat_04, &pat_05, &pat_06, &pat_07,
                                    &pat_08, &pat_09};
static void (*kuzure5_act_tbl[4])(sprite_status *) = {&main_ini, &main_check,
                                                      &main_wait, &main_break};
static void (*parts_act_tbl[3])(sprite_status *) = {&parts_ini, &parts_wait,
                                                    &parts_fall};

void kuzure5(sprite_status *pActwk) {
    if (pActwk->userflag.b.l) {
        parts_act_tbl[pActwk->r_no0 / 2](pActwk);
    } else {
        kuzure5_act_tbl[pActwk->r_no0 / 2](pActwk);
    }
}

static void main_ini(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg = 4;
    pActwk->patbase = pat_kuzure_a;
    pActwk->sprpri = 3;
    pActwk->sprvsize = 17;

    if (stageno.b.l != 2) {
        pActwk->sproffset = 17232;
    } else {
        pActwk->sproffset = 17312;
    }

    pActwk->patno = pActwk->userflag.b.h - 1;
    pActwk->sprhs = pActwk->sprhsize = pActwk->userflag.b.h * 8;
    main_check(pActwk);
}

static void main_check(sprite_status *pActwk) {
    if (ridechk(pActwk, &actwk[0])) {
        kuzure5_work *work = kuzure5_work_get(pActwk);

        pActwk->r_no0 += 2;
        pActwk->cdsts = 0;
        work->wait_timer = 8;

        if (actwk[0].xspeed.w >= 0) {
            work->break_step = 8;
        } else
            work->break_step = -8;

        soundset(163);
    }

    actionsub(pActwk);
    frameout_s(pActwk);
}

static void main_wait(sprite_status *pActwk) {
    ridechk(pActwk, &actwk[0]);

    if (--kuzure5_work_get(pActwk)->wait_timer < 0) {
        pActwk->r_no0 += 2;
    }

    actionsub(pActwk);
}

static void main_break(sprite_status *pActwk) {
    Uint8 flagwk;
    Sint16 xwk;
    Sint16 ywk;
    sprite_status *pNewact;

    flagwk = 255;
    ywk = 8;
    xwk = ((Sint16)pActwk->userflag.b.h - 1) * 8;

    if (kuzure5_work_get(pActwk)->break_step >= 0) {
        xwk *= -1;
    }

    if (actwkchk(&pNewact) == 0) {

        pNewact->actno = pActwk->actno;
        pNewact->actflg = pActwk->actflg;
        pNewact->sproffset = pActwk->sproffset;
        pNewact->patbase = pActwk->patbase;
        pNewact->xposi.l = pActwk->xposi.l;
        pNewact->yposi.l = pActwk->yposi.l;

        pNewact->userflag.b.l = flagwk;
        pNewact->yposi.w.h -= ywk;
        pNewact->xposi.w.h += xwk;
        kuzure5_work_get(pNewact)->wait_timer = 4;
        pNewact->patno = 8;

        if (actwkchk(&pNewact) == 0) {

            pNewact->actno = pActwk->actno;
            pNewact->actflg = pActwk->actflg;
            pNewact->sproffset = pActwk->sproffset;
            pNewact->patbase = pActwk->patbase;
            pNewact->xposi.l = pActwk->xposi.l;
            pNewact->yposi.l = pActwk->yposi.l;

            pNewact->userflag.b.l = flagwk;
            pNewact->yposi.w.h += ywk;
            pNewact->xposi.w.h += xwk;
            pNewact->patno = 9;
        }
    }

    if (ridechk(pActwk, &actwk[0])) {
        ride_on_clr(pActwk, &actwk[0]);
    }

    if (--pActwk->userflag.b.h <= 0) {
        frameout(pActwk);
    } else {
        pActwk->r_no0 -= 2;
        --pActwk->patno;
        pActwk->sprhs -= 8;
        pActwk->sprhsize -= 8;
        kuzure5_work_get(pActwk)->wait_timer = 7;
        pActwk->xposi.w.h += kuzure5_work_get(pActwk)->break_step;
        ridechk(pActwk, &actwk[0]);
        actionsub(pActwk);
    }
}

static void parts_ini(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->sprpri = 3;
    pActwk->sprhs = pActwk->sprhsize = 8;
    pActwk->sprvsize = 8;
    actionsub(pActwk);
}

static void parts_wait(sprite_status *pActwk) {
    if (--kuzure5_work_get(pActwk)->wait_timer < 0) {
        pActwk->r_no0 += 2;
    }

    actionsub(pActwk);
}

static void parts_fall(sprite_status *pActwk) {
    Sint32 spdwk;

    if (!(pActwk->actflg & 128)) {
        frameout(pActwk);
    } else {
        spdwk = kuzure5_work_get(pActwk)->y_velocity + 16384;
        if (spdwk > 1441792) {
            spdwk = 1441792;
        }
        kuzure5_work_get(pActwk)->y_velocity = spdwk;
        pActwk->yposi.l += spdwk;
        actionsub(pActwk);
    }
}
