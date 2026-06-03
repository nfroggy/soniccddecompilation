#include <stddef.h>

#include "equ.h"
#include "taki.h"
#include "action.h"
#include "playsub.h"

static void taki_init(sprite_status *pActwk);
static void taki_move(sprite_status *pActwk);

extern void block_wrt(Uint16 BlockNo, Uint16 xOffs, Uint16 yOffs);

#if defined(R11A)
#define SPRITE_SHIBUKI_BASE 465
#else
#define SPRITE_SHIBUKI_BASE 448
#endif

static char shibukichg0[4] = {4, 0, 1, -1};
Uint8 *shibukichg = {(Uint8 *)shibukichg0};
static sprite_pattern shibuki0 = {1, {{-96, -16, 0, SPRITE_SHIBUKI_BASE}}};
static sprite_pattern shibuki1 = {1, {{-96, -16, 0, SPRITE_SHIBUKI_BASE + 1}}};
sprite_pattern *shibukipat[2] = {&shibuki0, &shibuki1};
static void (*taki_move_tbl[2])(sprite_status *) = {&taki_init, &taki_move};

#pragma pack(push, 1)
typedef struct {
    Sint16 end_yposi;
} taki_work;
#pragma pack(pop)

_Static_assert(offsetof(taki_work, end_yposi) == 0,
               "taki_work.end_yposi must map to offset 0");
_Static_assert(sizeof(taki_work) <= sizeof(((sprite_status *)0)->actfree),
               "taki_work must fit in sprite_status.actfree");

static taki_work *taki_work_get(sprite_status *pActwk) {
    return (taki_work *)pActwk->actfree;
}

void taki(sprite_status *pActwk) {
    taki_move_tbl[pActwk->r_no0 / 2](pActwk);
    patchg(pActwk, &shibukichg);
    actionsub(pActwk);
}

static void taki_init(sprite_status *pActwk) {
    taki_work *work = taki_work_get(pActwk);

    pActwk->r_no0 += 2;
    pActwk->patbase = shibukipat;
    pActwk->actflg = 4;
    pActwk->sprpri = 1;
    pActwk->sprhsize = 16;
    pActwk->sproffset = 954;
    pActwk->yposi.w.h &= -16;

    work->end_yposi = pActwk->yposi.w.h + 384;
}

static void taki_move(sprite_status *pActwk) {
    taki_work *work = taki_work_get(pActwk);
    Uint16 blkno;
    Sint16 i, xoffs;

    if (pActwk->yposi.w.h + 4 >= work->end_yposi) {

        frameout(pActwk);
        return;
    }

    pActwk->yposi.w.h += 4;
    blkno = 8194;
    xoffs = pActwk->xposi.w.h - 96;

    if (pActwk->yposi.w.h & 15)
        return;

    for (i = 0; i < 12; ++i, xoffs += 16) {
        block_wrt(blkno, xoffs, pActwk->yposi.w.h);
    }
}

void jisin_set(sprite_status *pActwk) { pActwk = pActwk; }

void jisin(sprite_status *pActwk) { pActwk = pActwk; }
