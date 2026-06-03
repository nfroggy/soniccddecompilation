#include "../equ.h"
#include "harid4.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../ridechk.h"
#include <stddef.h>

#if defined(R41A)
#define SPRITE_HARID4_BASE 444
#elif defined(R41C)
#define SPRITE_HARID4_BASE 432
#else
#define SPRITE_HARID4_BASE 467
#endif

static void act_init(sprite_status *pActwk);
static void act_wait(sprite_status *pActwk);
static void act_slide(sprite_status *pActwk);
static void act_slide1(sprite_status *pActwk);
static void act_down(sprite_status *pActwk);
static void act_down1(sprite_status *pActwk);
static void act_stop(sprite_status *pActwk);

static sprite_pattern pat00 = {
    2,
    {{-16, -64, 0, SPRITE_HARID4_BASE + 1}, {-16, 32, 0, SPRITE_HARID4_BASE}}};
static sprite_pattern pat01;
sprite_pattern *pat_harid4[2] = {&pat00, &pat01};
static void (*hari4_act_tbl[7])(sprite_status *) = {
    &act_init, &act_wait,  &act_slide, &act_slide1,
    &act_down, &act_down1, &act_stop};

#pragma pack(push, 1)
typedef struct {
    Sint32 y_velocity;
    Sint32 stop_distance;
    Sint16 timer;
    Sint32 acceleration;
    Uint8 unused14[4];
    Sint16 child_actor;
    Sint16 parent_actor;
} harid4_work;
#pragma pack(pop)

_Static_assert(offsetof(harid4_work, y_velocity) == 0,
               "harid4_work.y_velocity offset");
_Static_assert(offsetof(harid4_work, stop_distance) == 4,
               "harid4_work.stop_distance offset");
_Static_assert(offsetof(harid4_work, timer) == 8,
               "harid4_work.timer offset");
_Static_assert(offsetof(harid4_work, acceleration) == 10,
               "harid4_work.acceleration offset");
_Static_assert(offsetof(harid4_work, child_actor) == 18,
               "harid4_work.child_actor offset");
_Static_assert(offsetof(harid4_work, parent_actor) == 20,
               "harid4_work.parent_actor offset");
_Static_assert(sizeof(harid4_work) <= sizeof(((sprite_status *)0)->actfree),
               "harid4_work fits in actfree");

static harid4_work *harid4_work_get(sprite_status *pActwk) {
    return (harid4_work *)pActwk->actfree;
}

void harid4(sprite_status *pActwk) {
    harid4_work *work = harid4_work_get(pActwk);

    if (pActwk->userflag.b.h & 128) {

        if (actwk[work->parent_actor].actno != 50) {
            frameout(pActwk);
        }

        actionsub(pActwk);
    } else {
        if (pActwk->r_no0 == 0 || pActwk->actflg & 128) {

            hari4_act_tbl[pActwk->r_no0 / 2](pActwk);
            hitchk(pActwk, &actwk[0]);
        }

        actionsub(pActwk);
        frameout_s(pActwk);
    }
}

static void act_init(sprite_status *pActwk) {
    harid4_work *work = harid4_work_get(pActwk);
    Sint32 xacwk;
    sprite_status *pNewact;

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->sprhs = 16;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 64;
    pActwk->sproffset = 928;
    pActwk->patbase = pat_harid4;

    if (waterposi_m >= pActwk->yposi.w.h) {
        xacwk = 12288;
    } else {
        xacwk = 4096;
    }
    work->acceleration = xacwk;

    if (actwkchk2(pActwk, &pNewact) == 0) {
        harid4_work *new_work = harid4_work_get(pNewact);

        pNewact->actno = pActwk->actno;
        pNewact->userflag.b.h = -1;
        pNewact->xposi.w.h = pActwk->xposi.w.h;
        pNewact->yposi.w.h = pActwk->yposi.w.h + 48;
        new_work->parent_actor = pActwk - actwk;
        work->child_actor = pNewact - actwk;
        pNewact->colino = 176;
        pNewact->actflg = pActwk->actflg;
        pNewact->sprpri = pActwk->sprpri;
        pNewact->sprhs = 16;
        pNewact->sprhsize = 16;
        pNewact->sprvsize = 16;
        pNewact->sproffset = 928;
        pNewact->patbase = pat_harid4;
        pNewact->patno = 1;
    }
}

static void act_wait(sprite_status *pActwk) {
    Sint16 wk;

    wk = actwk[0].yposi.w.h - pActwk->yposi.w.h;
    wk -= 64;
    if ((Uint16)wk >= 128)
        return;

    wk = actwk[0].xposi.w.h - pActwk->xposi.w.h;
    wk += 16;
    if ((Uint16)wk >= 32)
        return;

    pActwk->r_no0 += 2;
    pActwk->yspeed.w = 256;
    harid4_work_get(pActwk)->timer = 20;
}

static void act_slide(sprite_status *pActwk) {
    harid4_work *work = harid4_work_get(pActwk);
    Sint32 spdwk;

    spdwk = work->y_velocity;
    pActwk->yposi.l += spdwk;
    actwk[work->child_actor].yposi.l += spdwk;
    work->y_velocity += 8192;

    if (--work->timer < 0) {
        pActwk->r_no0 += 2;
        work->y_velocity = 0;
        work->timer = 30;
    }
}

static void act_slide1(sprite_status *pActwk) {
    harid4_work *work = harid4_work_get(pActwk);

    if (--work->timer < 0) {
        pActwk->r_no0 += 2;
    }
}

static void act_down(sprite_status *pActwk) {
    harid4_work *work = harid4_work_get(pActwk);
    Sint32 spdwk;
    Sint16 ret;

    spdwk = work->y_velocity;
    pActwk->yposi.l += spdwk;
    actwk[work->child_actor].yposi.l += spdwk;
    work->y_velocity += work->acceleration;

    if ((ret = emycol_d(pActwk)) < 0) {
        pActwk->r_no0 += 2;
        pActwk->yposi.w.h += ret;
        pActwk->yspeed.w = 0;
        work->stop_distance = 0x200000;

        frameout(&actwk[work->child_actor]);
    }
}

static void act_down1(sprite_status *pActwk) {
    harid4_work *work = harid4_work_get(pActwk);
    Sint32 spdwk;

    spdwk = work->y_velocity;
    pActwk->yposi.l += spdwk;

    work->stop_distance -= spdwk;
    if (work->stop_distance < 0) {
        pActwk->r_no0 += 2;
    }
}

static void act_stop(sprite_status *pActwk) { pActwk = pActwk; }
