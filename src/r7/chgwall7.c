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

void chgwall7(sprite_status *pActwk) {
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
            pActwk->actfree[0] = 1;
        }
    } else {
        if (pActwk->actfree[0]) {
            pActwk->actfree[0] = 0;
            ride_on_clr(pActwk, &actwk[0]);
        }
    }

    actionsub(pActwk);
    frameout_s(pActwk);
}
