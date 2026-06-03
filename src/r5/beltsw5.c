#include <stddef.h>

#include "../equ.h"
#include "beltsw5.h"
#include "../action.h"
#include "../actset.h"
#include "../loader2.h"

static void beltsw5_init(sprite_status *pActwk);
static void beltsw5_move1(sprite_status *pActwk);
static void beltsw5_move2(sprite_status *pActwk);
static void beltsw5_move3(sprite_status *pActwk);

static sprite_pattern beltsw5_pat0 = {1, {{-4, -16, 0, 406}}};
static sprite_pattern beltsw5_pat1 = {1, {{-8, -36, 0, 407}}};
static sprite_pattern beltsw5_pat2 = {1, {{-28, -16, 0, 408}}};
sprite_pattern *beltsw5pat[3] = {&beltsw5_pat0, &beltsw5_pat1, &beltsw5_pat2};

#pragma pack(push, 1)
typedef struct {
    Uint8 reserved0[16];
    Uint8 animation_timer;
    Uint8 reserved17;
    Uint8 last_direction;
    Uint8 external_change_pending;
} beltsw5_work;
#pragma pack(pop)

_Static_assert(offsetof(beltsw5_work, animation_timer) == 16,
               "beltsw5_work.animation_timer must map to offset 16");
_Static_assert(offsetof(beltsw5_work, last_direction) == 18,
               "beltsw5_work.last_direction must map to offset 18");
_Static_assert(offsetof(beltsw5_work, external_change_pending) == 19,
               "beltsw5_work.external_change_pending must map to offset 19");
_Static_assert(sizeof(beltsw5_work) <= sizeof(((sprite_status *)0)->actfree),
               "beltsw5_work must fit in sprite_status.actfree");

static beltsw5_work *beltsw5_work_get(sprite_status *pActwk) {
    return (beltsw5_work *)pActwk->actfree;
}

void beltsw5(sprite_status *pActwk) {
    void (*beltsw5_move_tbl[4])(sprite_status *) = {
        &beltsw5_init, &beltsw5_move1, &beltsw5_move2, &beltsw5_move3};

    beltsw5_move_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s(pActwk);
}

static void beltsw5_init(sprite_status *pActwk) {
    beltsw5_work *work = beltsw5_work_get(pActwk);

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->sprhsize = 24;
    pActwk->sprvsize = 24;
    pActwk->sproffset = 18327;
    pActwk->patbase = beltsw5pat;

    if (colrevflag & 1)
        pActwk->patno = 2;
    work->last_direction = colrevflag;
    beltsw5_move1(pActwk);
}

static void beltsw5_move1(sprite_status *pActwk) {
    beltsw5_work *work = beltsw5_work_get(pActwk);
    Sint16 wD0;

    if (!editmode.b.h) {
        wD0 = actwk[0].yposi.w.h - pActwk->yposi.w.h;
        if ((Uint16)actwk[0].yposi.w.h < (Uint16)pActwk->yposi.w.h)
            wD0 *= -1;
        if ((Uint16)wD0 < 16) {
            wD0 = actwk[0].xposi.w.h - pActwk->xposi.w.h;
            if ((Uint16)actwk[0].xposi.w.h < (Uint16)pActwk->xposi.w.h)
                wD0 *= -1;
            if ((Uint16)wD0 < 8) {
                colrevflag ^= 1;
                pActwk->r_no0 += 2;

                if (pActwk->actflg & 128)
                    soundset(191);
                return;
            }
        }
    }

    if (colrevflag != work->last_direction) {
        work->external_change_pending = 255;
        pActwk->r_no0 += 2;
    }

    work->last_direction = colrevflag;
}

static void beltsw5_move2(sprite_status *pActwk) {
    beltsw5_work *work = beltsw5_work_get(pActwk);

    ++work->animation_timer;
    if (work->animation_timer == 7) {
        pActwk->patno = 1;
    } else {

        if (work->animation_timer == 14) {
            work->animation_timer = 0;
            pActwk->patno = 0;
            if (colrevflag & 1)
                pActwk->patno = 2;

            pActwk->r_no0 += 2;
            work->external_change_pending = 0;
        }
    }
}

static void beltsw5_move3(sprite_status *pActwk) {
    beltsw5_work *work = beltsw5_work_get(pActwk);
    Sint16 wD0;

    wD0 = actwk[0].yposi.w.h - pActwk->yposi.w.h;
    if ((Uint16)actwk[0].yposi.w.h < (Uint16)pActwk->yposi.w.h)
        wD0 *= -1;
    if ((Uint16)wD0 < 32) {
        wD0 = actwk[0].xposi.w.h - pActwk->xposi.w.h;
        if ((Uint16)actwk[0].xposi.w.h < (Uint16)pActwk->xposi.w.h)
            wD0 *= -1;
        if ((Uint16)wD0 < 16)
            return;
    }

    pActwk->r_no0 = 2;
    work->last_direction = colrevflag;
}
