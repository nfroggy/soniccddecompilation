#include "../equ.h"
#include "hari5f.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"

static void f_init(sprite_status *pActwk);
static void f_wait(sprite_status *pActwk);
static void f_fall(sprite_status *pActwk);

static sprite_pattern hari5f_pat0 = {1, {{-8, -24, 0, 421}}};
sprite_pattern *pat_hari5f[1] = {&hari5f_pat0};

void hari5f(sprite_status *pActwk) {
    void (*hari5f_move_tbl[3])(sprite_status *) = {&f_init, &f_wait, &f_fall};

    hari5f_move_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s(pActwk);
}

static void f_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->sprhs = 8;
    pActwk->sprhsize = 8;
    pActwk->sprvsize = 24;
    pActwk->patbase = pat_hari5f;
    pActwk->sproffset = 834;

    if (time_flag == 0)
        pActwk->sproffset = 17218;
    else if (time_flag == 1)
        pActwk->sproffset = 834;
    else if (generate_flag)
        pActwk->sproffset = 834;
    else
        pActwk->sproffset = 17218;

    f_wait(pActwk);
}

static void f_wait(sprite_status *pActwk) {
    if ((Uint16)(actwk[0].yposi.w.h - pActwk->yposi.w.h - 24) < 104) {
        if ((Uint16)(actwk[0].xposi.w.h - pActwk->xposi.w.h + 8) < 16) {
            pActwk->r_no0 += 2;
            pActwk->colino = 185;
        }
    }
}

static void f_fall(sprite_status *pActwk) {
    Uint8 bD0;
    Sint16 wD1;

    if (!(pActwk->actflg & 128)) {
        if (pActwk->yposi.w.h >= actwk[0].yposi.w.h) {
            frameout(pActwk);
            return;
        }
    }

    wD1 = emycol_d3(pActwk, pActwk->xposi.w.h,
                    pActwk->yposi.w.h - (Uint16)pActwk->sprvsize);

    if (wD1 >= 0) {
        bD0 = 0;
        if (pActwk->actfree[21] & 128) {
            frameout(pActwk);
            return;
        }

    } else
        bD0 = 255;
    pActwk->actfree[21] = bD0;

    pActwk->yposi.l += *(Sint32 *)((char *)&pActwk->actfree[0]);
    *(Sint32 *)&pActwk->actfree[0] += 16384;
}
