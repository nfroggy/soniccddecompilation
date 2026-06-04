#include "../equ.h"
#include "chgwall7.h"
#include "../action.h"
#include "../actset.h"
#include "../ridechk.h"

#if defined(R73C) || defined(R73D)
#define SPRITE_CHGWALL7_BASE 452
#else
#define SPRITE_CHGWALL7_BASE 403
#endif

sprite_pattern pat00;
sprite_pattern pat01 = {1, {{-32, -16, 0, SPRITE_CHGWALL7_BASE}}};
sprite_pattern *pat_chgwall7[2] = {&pat00, &pat01};

typedef struct {
    Uint8 player_contact_latch;
} chgwall7_work;

static chgwall7_work *chgwall7_work_get(sprite_status *pActwk) {
    return (chgwall7_work *)pActwk->actfree;
}

void chgwall7(sprite_status *pActwk) {
    chgwall7_work *work = chgwall7_work_get(pActwk);

    if (!pActwk->r_no0) {

        pActwk->r_no0 += 2;
        pActwk->actflg |= 4;
        pActwk->sprhs = pActwk->sprhsize = 32;
        pActwk->sprvsize = 17;
        pActwk->patbase = pat_chgwall7;
    }

    pActwk->patno = 0;
    if (prio_flag) {

        if (hitchk(pActwk, &actwk[0])) {
            work->player_contact_latch = 1;
        }
    } else {
        if (work->player_contact_latch) {
            work->player_contact_latch = 0;
            ride_on_clr(pActwk, &actwk[0]);
        }
    }

    actionsub(pActwk);
    frameout_s(pActwk);
}
