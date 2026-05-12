#include "../equ.h"
#include "sw6.h"
#include "../action.h"
#include "../actset.h"
#include "../fcol.h"
#include "../loader2.h"
#include "../ridechk.h"

sprite_pattern pat0_sw6 = {1, {{-16, -8, 0, 307}}};
sprite_pattern pat1_sw6 = {1, {{-16, -4, 0, 308}}};
sprite_pattern *pat_sw6[2] = {&pat0_sw6, &pat1_sw6};

void sw6(sprite_status *pActwk) {
    if (pActwk->r_no0 == 0)
        act_init(pActwk);
    else
        act_move(pActwk);
}

void act_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 1;
    pActwk->sprhs = 16;
    pActwk->sprhsize = 16;
    pActwk->sproffset = 922;
    pActwk->patbase = pat_sw6;

    pActwk->sprvsize = 8;
    pActwk->actfree[18] = pActwk->userflag.b.h;

    ((Sint16 *)pActwk)[33] = 0;
    act_move(pActwk);
}

void act_move(sprite_status *pActwk) {
    Uint8 addr_off;

    addr_off = pActwk->actfree[18];
    pActwk->actfree[21] = pActwk->actfree[20];

    if (hitchk(pActwk, &actwk[0]) != 0) {

        pActwk->actfree[20] = 255;
        switchflag[addr_off] |= 128;
        switchflag[addr_off] |= 64;
    } else {

        pActwk->actfree[20] = 0;
        switchflag[addr_off] &= 127;
    }

    if (((Uint16 *)pActwk)[33] == 255) {
        if ((char)pActwk->actflg < 0)
            soundset(191);

        switchflag[addr_off] = bchg(5, switchflag[addr_off]);
        actwk[0].yposi.w.h += 8;
        pActwk->yposi.w.h += 4;
        ++pActwk->patno;
        pActwk->sprvsize -= 4;
    }

    if (((Uint16 *)pActwk)[33] == 65280) {
        actwk[0].yposi.w.h -= 8;
        pActwk->yposi.w.h -= 4;
        --pActwk->patno;
        pActwk->sprvsize += 4;
    }
    actionsub(pActwk);
    frameout_s(pActwk);
}
