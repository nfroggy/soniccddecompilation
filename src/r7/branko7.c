#include <stddef.h>

#include "../equ.h"
#include "branko7.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"
#include "../ridechk.h"

#if defined(R73C) || defined(R73D)
#define SPRITE_BRANKO7_BASE 403
#else
#define SPRITE_BRANKO7_BASE 404
#endif

static void branko7_ridechk(sprite_status *pActwk);
static void branko7_init(sprite_status *pActwk);
static void branko7_move(sprite_status *pActwk);
static void branko7_posiset(sprite_status *pActwk);
static void branko7_posiset2(sprite_status *pActwk, Uint8 dirwk);

static Sint16 branko7_initbl[32] = {
    -16384, 2,  256,  -256, -16384, 2,  256,  -256, -16384, -2, -256, 256,
    -24576, 3,  384,  -384, -32768, -2, -256, 256,  -32768, 2,  256,  -256,
    -32768, -1, -256, 0,    -32768, 1,  256,  0};
static sprite_pattern pat0 = {1, {{-8, -8, 0, SPRITE_BRANKO7_BASE}}};
static sprite_pattern pat1 = {1, {{-32, -8, 0, SPRITE_BRANKO7_BASE + 1}}};
static sprite_pattern pat2 = {1, {{-8, -8, 0, SPRITE_BRANKO7_BASE + 2}}};
sprite_pattern *branko7pat[3] = {&pat0, &pat1, &pat2};
static void (*branko7_act_tbl[2])(sprite_status *) = {&branko7_init,
                                                      &branko7_move};

#pragma pack(push, 1)
typedef struct {
    Uint8 reserved0[5];
    Uint8 total_segments;
    Sint16 swing_speed;
    Sint16 origin_y;
    Sint16 area1;
    Sint16 origin_x;
    Sint16 area2;
    Sint16 angle;
    Uint8 segment_index;
    Uint8 direction;
    Sint16 acceleration;
} branko7_work;
#pragma pack(pop)

_Static_assert(offsetof(branko7_work, total_segments) == 5,
               "branko7_work.total_segments must map to offset 5");
_Static_assert(offsetof(branko7_work, swing_speed) == 6,
               "branko7_work.swing_speed must map to offset 6");
_Static_assert(offsetof(branko7_work, origin_y) == 8,
               "branko7_work.origin_y must map to offset 8");
_Static_assert(offsetof(branko7_work, area1) == 10,
               "branko7_work.area1 must map to offset 10");
_Static_assert(offsetof(branko7_work, origin_x) == 12,
               "branko7_work.origin_x must map to offset 12");
_Static_assert(offsetof(branko7_work, area2) == 14,
               "branko7_work.area2 must map to offset 14");
_Static_assert(offsetof(branko7_work, angle) == 16,
               "branko7_work.angle must map to offset 16");
_Static_assert(offsetof(branko7_work, segment_index) == 18,
               "branko7_work.segment_index must map to offset 18");
_Static_assert(offsetof(branko7_work, direction) == 19,
               "branko7_work.direction must map to offset 19");
_Static_assert(offsetof(branko7_work, acceleration) == 20,
               "branko7_work.acceleration must map to offset 20");
_Static_assert(sizeof(branko7_work) <= sizeof(((sprite_status *)0)->actfree),
               "branko7_work must fit in sprite_status.actfree");

static branko7_work *branko7_work_get(sprite_status *pActwk) {
    return (branko7_work *)pActwk->actfree;
}

void branko7(sprite_status *pActwk) {
    branko7_work *work = branko7_work_get(pActwk);

    branko7_act_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s00(pActwk, work->origin_x);
}

void branko7_ridechk(sprite_status *pActwk) {
    pActwk->sprvsize = 8;

    if (!hitchk(pActwk, &actwk[0])) {
        pActwk->sprvsize = 12;

        if (!hitchk(pActwk, &actwk[0])) {
            pActwk->sprvsize = 8;
            return;
        }
    }

    pActwk->sprvsize = 8;

    if (pActwk->yposi.w.h >= actwk[0].yposi.w.h) {
        actwk[0].yposi.w.h = pActwk->yposi.w.h -
                             (Sint16)(char)pActwk->sprvsize -
                             (Sint16)(char)actwk[0].sprvsize + 2;
    }
}

void branko7_init(sprite_status *pActwk) {
    branko7_work *work = branko7_work_get(pActwk);
    Sint32 flagwk, flagcnt;
    sprite_status *pNewact;
    branko7_work *new_work;
    Sint16 *pIniTbl;

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->patbase = branko7pat;
    pActwk->sproffset = 870;
    pActwk->sprhsize = 32;
    pActwk->sprvsize = 8;

    if (!work->segment_index) {
        work->origin_x = pActwk->xposi.w.h;
        work->origin_y = pActwk->yposi.w.h;

        flagwk = flagcnt = 0;
        flagwk = pActwk->userflag.b.h & 15;
        work->total_segments = flagwk;

        for (flagcnt = flagwk - 1; flagcnt >= 0; --flagcnt, --flagwk) {

            if (actwkchk(&pNewact) == 0) {
                new_work = branko7_work_get(pNewact);
                pNewact->actno = 32;
                new_work->origin_x = work->origin_x;

                new_work->origin_y = work->origin_y;

                pNewact->userflag.b.h = pActwk->userflag.b.h;
                new_work->total_segments = work->total_segments;
                new_work->segment_index = (Uint8)flagwk;
                pNewact->patno = 2;

                if (work->total_segments == flagwk) {
                    pNewact->patno = 1;
                    pNewact->colino = 169;
                }
            }
        }
    }

    flagwk = pActwk->userflag.b.h & 240;
    flagwk >>= 1;
    pIniTbl = &branko7_initbl[flagwk / 2];
    work->angle = *pIniTbl++;
    work->acceleration = *pIniTbl++;
    work->area1 = *pIniTbl++;
    work->area2 = *pIniTbl++;

    branko7_move(pActwk);
}

void branko7_move(sprite_status *pActwk) {
    branko7_work *work = branko7_work_get(pActwk);
    Sint32 lXpos, lYpos;

    lXpos = pActwk->xposi.l;
    lYpos = pActwk->yposi.l;

    branko7_posiset(pActwk);

    if (work->total_segments == work->segment_index) {
        pActwk->xspeed.w = pActwk->xposi.l - lXpos >> 8;
        pActwk->yspeed.w = pActwk->yposi.l - lYpos >> 8;
        branko7_ridechk(pActwk);
    }
}

void branko7_posiset(sprite_status *pActwk) {
    branko7_work *work = branko7_work_get(pActwk);
    Sint16 spdwk;

    if (!work->direction) {
        spdwk = work->swing_speed + work->acceleration;

        work->swing_speed = spdwk;
        work->angle += spdwk;
        if (spdwk == work->area1) {
            work->direction = 1;
        }
    } else {
        spdwk = work->swing_speed - work->acceleration;

        work->swing_speed = spdwk;
        work->angle += spdwk;
        if (spdwk == work->area2) {
            work->direction = 0;
        }
    }

    branko7_posiset2(pActwk, (Uint8)((Uint16)work->angle >> 8));
}

void branko7_posiset2(sprite_status *pActwk, Uint8 dirwk) {
    branko7_work *work = branko7_work_get(pActwk);
    Sint16 usrwk;
    int_union sinwk, coswk;

    sinwk.l = coswk.l = 0;
    sinset(dirwk, &sinwk.w.l, &coswk.w.l);
    usrwk = work->segment_index;
    usrwk <<= 4;
    sinwk.l *= usrwk;
    coswk.l *= usrwk;

    sinwk.l >>= 4;
    sinwk.w.l >>= 4;
    coswk.l >>= 4;
    coswk.w.l >>= 4;
    pActwk->yposi.w.h = coswk.w.l + work->origin_y;
    pActwk->xposi.w.h = sinwk.w.l + work->origin_x;
}
