#include "../equ.h"
#include "wall42.h"
#include "../action.h"
#include "../ridechk.h"
#include <stddef.h>

static void act_init(sprite_status *pActwk);
static void act_wait(sprite_status *pActwk);
static void act_move(sprite_status *pActwk);
static void act_stop(sprite_status *pActwk);

#pragma pack(push, 1)
typedef struct {
    Sint16 timer;
    Uint8 *flagwork_entry;
} wall42_work;
#pragma pack(pop)

_Static_assert(offsetof(wall42_work, timer) == 0,
               "wall42_work.timer offset");
_Static_assert(offsetof(wall42_work, flagwork_entry) == 2,
               "wall42_work.flagwork_entry offset");
_Static_assert(sizeof(wall42_work) <= sizeof(((sprite_status *)0)->actfree),
               "wall42_work fits in actfree");

static wall42_work *wall42_work_get(sprite_status *pActwk) {
    return (wall42_work *)pActwk->actfree;
}

static sprite_pattern pat00 = {1, {{-32, -64, 0, 531}}};
sprite_pattern *pat_wall42[1] = {&pat00};

void wall42(sprite_status *pActwk) {
    Sint16 d0, d1;
    Uint8 *a1;
    void (*tbl[4])(sprite_status *) = {&act_init, &act_wait, &act_move,
                                       &act_stop};

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);

    d0 = pActwk->xposi.w.h & -128;
    d1 = scra_h_posit.w.h - 128 & -128;
    d0 -= d1;
    if (d0 > 640) {
        a1 = wall42_work_get(pActwk)->flagwork_entry;
        *a1 &= 127;
        frameout(pActwk);
    }
}

static void act_init(sprite_status *pActwk) {
    Sint16 index;
    Uint8 *a1;

    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->sprhsize = 32;
    pActwk->sprvsize = 64;
    pActwk->sproffset = 17514;
    pActwk->patbase = pat_wall42;

    index = (Uint16)pActwk->cdsts;
    index *= 3;
    index += (Uint16)time_flag;
    a1 = &flagwork[index];
    wall42_work_get(pActwk)->flagwork_entry = a1;

    if (!(*a1 & 1)) {
        *a1 |= 1;
        wall42_work_get(pActwk)->timer = 60;
        pActwk->r_no0 += 2;
    } else {
        pActwk->xposi.w.h += 192;
        pActwk->r_no0 += 6;
    }
}

static void act_wait(sprite_status *pActwk) {
    wall42_work *work = wall42_work_get(pActwk);

    if (work->timer == -1) {
        work->timer = 384;
        pActwk->r_no0 += 2;
    }
    --work->timer;
    act_stop(pActwk);
}

static void act_move(sprite_status *pActwk) {
    Sint32 d0;

    if (hitchk(pActwk, &actwk[0])) {
        d0 = 32768;
        d0 <<= 8;
        actwk[0].mspeed.w = d0 & 65535;
    }
    pActwk->xposi.l += 32768;
    if (wall42_work_get(pActwk)->timer == -1) {
        pActwk->r_no0 += 2;
    }
    --wall42_work_get(pActwk)->timer;
}

static void act_stop(sprite_status *pActwk) { hitchk(pActwk, &actwk[0]); }
