#include "../equ.h"
#include "wall42.h"
#include "../action.h"
#include "../ridechk.h"

static void act_init(sprite_status *pActwk);
static void act_wait(sprite_status *pActwk);
static void act_move(sprite_status *pActwk);
static void act_stop(sprite_status *pActwk);

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
        a1 = ((Uint8 **)pActwk)[12];
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
    ((Uint8 **)pActwk)[12] = a1;

    if (!(*a1 & 1)) {
        *a1 |= 1;
        ((Sint16 *)pActwk)[23] = 60;
        pActwk->r_no0 += 2;
    } else {
        pActwk->xposi.w.h += 192;
        pActwk->r_no0 += 6;
    }
}

static void act_wait(sprite_status *pActwk) {
    if (((Sint16 *)pActwk)[23] == -1) {
        ((Sint16 *)pActwk)[23] = 384;
        pActwk->r_no0 += 2;
    }
    --((Sint16 *)pActwk)[23];
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
    if (((Sint16 *)pActwk)[23] == -1) {
        pActwk->r_no0 += 2;
    }
    --((Sint16 *)pActwk)[23];
}

static void act_stop(sprite_status *pActwk) { hitchk(pActwk, &actwk[0]); }
