#include "../equ.h"
#include "sw6.h"
#include "../action.h"
#include "../actset.h"
#include "../fcol.h"
#include "../loader2.h"
#include "../ridechk.h"

typedef struct {
    Uint8 switch_index;
    union {
        Uint16 transition;
        struct {
            Uint8 current_state;
            Uint8 previous_state;
        };
    };
} sw6_work;

static sw6_work *sw6_get_work(sprite_status *pActwk) {
    return (sw6_work *)pActwk->actfree;
}

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
    sw6_work *work = sw6_get_work(pActwk);

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 1;
    pActwk->sprhs = 16;
    pActwk->sprhsize = 16;
    pActwk->sproffset = 922;
    pActwk->patbase = pat_sw6;

    pActwk->sprvsize = 8;
    work->switch_index = pActwk->userflag.b.h;

    work->transition = 0;
    act_move(pActwk);
}

void act_move(sprite_status *pActwk) {
    sw6_work *work = sw6_get_work(pActwk);
    Uint8 addr_off;

    addr_off = work->switch_index;
    work->previous_state = work->current_state;

    if (hitchk(pActwk, &actwk[0]) != 0) {

        work->current_state = 255;
        switchflag[addr_off] |= 128;
        switchflag[addr_off] |= 64;
    } else {

        work->current_state = 0;
        switchflag[addr_off] &= 127;
    }

    if (work->transition == 255) {
        if ((char)pActwk->actflg < 0)
            soundset(191);

        switchflag[addr_off] = bchg(5, switchflag[addr_off]);
        actwk[0].yposi.w.h += 8;
        pActwk->yposi.w.h += 4;
        ++pActwk->patno;
        pActwk->sprvsize -= 4;
    }

    if (work->transition == 65280) {
        actwk[0].yposi.w.h -= 8;
        pActwk->yposi.w.h -= 4;
        --pActwk->patno;
        pActwk->sprvsize += 4;
    }
    actionsub(pActwk);
    frameout_s(pActwk);
}
