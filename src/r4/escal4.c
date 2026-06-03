#include <stddef.h>

#include "../equ.h"
#include "escal4.h"
#include "../action.h"
#include "../ridechk.h"

sprite_pattern escal4_pat0 = {1, {{-16, -16, 0, 429}}};
sprite_pattern escal4_pat1 = {1, {{-16, -16, 0, 430}}};
sprite_pattern *escal4pat[2] = {&escal4_pat0, &escal4_pat1};

#pragma pack(push, 1)
typedef struct {
    Uint8 reserved0[20];
    Sint16 move_timer;
} escal4_work;
#pragma pack(pop)

_Static_assert(offsetof(escal4_work, move_timer) == 20,
               "escal4_work.move_timer must map to offset 20");
_Static_assert(sizeof(escal4_work) <= sizeof(((sprite_status *)0)->actfree),
               "escal4_work must fit in sprite_status.actfree");

static escal4_work *escal4_work_get(sprite_status *pActwk) {
    return (escal4_work *)pActwk->actfree;
}

void escal4(sprite_status *pActwk) {
    void (*escal4_acttbl[3])(sprite_status *) = {&escal4_init, &escal4_wait,
                                                 &escal4_moveup};

    escal4_acttbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);

    if ((pActwk->xposi.w.h & 65408) - ((scra_h_posit.w.h - 128) & 65408) <= 640)
        return;

    if (pActwk->cdsts)
        flagwork[time_flag + pActwk->cdsts * 3] &= 127;

    frameout(pActwk);
}

Sint16 escal4_ridechk(sprite_status *pActwk) { hitchk(pActwk, &actwk[0]); }

void escal4_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 17244;
    pActwk->sprpri = 3;
    pActwk->patbase = escal4pat;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 16;
}

void escal4_wait(sprite_status *pActwk) {
    escal4_work *work = escal4_work_get(pActwk);
    Sint16 escal4_tbl[9] = {256, -256, 160, 256, -256, 160, 512, -512, 80};

    if (pActwk->actflg & 128) {
        if (escal4_ridechk(pActwk)) {
            pActwk->r_no0 += 2;
            pActwk->xspeed.w = escal4_tbl[(time_flag & 3) * 6 / 2];
            pActwk->yspeed.w = escal4_tbl[(time_flag & 3) * 6 / 2 + 1];
            work->move_timer = escal4_tbl[(time_flag & 3) * 6 / 2 + 2];
        }
    }
}

void escal4_moveup(sprite_status *pActwk) {
    escal4_work *work = escal4_work_get(pActwk);

    escal4_ridechk(pActwk);

    if (work->move_timer) {
        pActwk->xposi.l += pActwk->xspeed.w << 8;
        pActwk->yposi.l += pActwk->yspeed.w << 8;

        if (!(--work->move_timer)) {
            pActwk->xspeed.w = 0;
            pActwk->yspeed.w = 0;
        }
    }
}
