#include "../equ.h"
#include "gaitou73.h"
#include "../action.h"
#include "../actset.h"

static void gaitou73_ini(sprite_status *pActwk);
static void gaitou73_01(sprite_status *pActwk);
static Sint16 get_x(sprite_status *pActwk);
static void gaitou73_02(sprite_status *pActwk);

static sprite_pattern pat0 = {1, {{-24, -12, 128, 417}}};
static sprite_pattern pat1 = {1, {{-8, -112, 128, 418}}};
sprite_pattern *gaitou73_pat[2] = {&pat0, &pat1};
static void (*gaitou_act_tbl[3])(sprite_status *) = {
    &gaitou73_ini, &gaitou73_01, &gaitou73_02};

void gaitou73(sprite_status *pActwk) {
    gaitou_act_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
}

static void gaitou73_ini(sprite_status *pActwk) {
    sprite_status *pNewact;

    pActwk->actflg = 4;
    pActwk->sprpri = 0;
    pActwk->sproffset = 17692;
    pActwk->patbase = gaitou73_pat;

    if (!pActwk->userflag.b.h) {
        if (actwkchk2(pActwk, &pNewact) == 0) {
            pNewact->actno = 54;
            pNewact->userflag.b.h = 1;
            ((Sint16 *)pNewact)[25] = pActwk - actwk;
            ((Sint16 *)pActwk)[25] = pNewact - actwk;
        }

        pActwk->sprhsize = 24;
        pActwk->sprvsize = 12;
        pActwk->r_no0 = 2;
    } else {
        pActwk->patno = 1;
        pActwk->sprhsize = 8;
        pActwk->sprvsize = 112;
        pActwk->r_no0 = 4;
    }
}

static void gaitou73_01(sprite_status *pActwk) {
    ((Sint16 *)pActwk)[26] = get_x(pActwk);
    ((Sint16 *)pActwk)[27] = 352 - (scra_v_posit.w.h - 352) / 3;

    if (((Sint16 *)pActwk)[27] + 12 >= scra_v_posit.w.h) {

        pActwk->xposi.w.h = ((Sint16 *)pActwk)[26];
        pActwk->yposi.w.h = ((Sint16 *)pActwk)[27];
        pActwk->patno = 0;
        pActwk->sprhsize = 24;
        pActwk->sprvsize = 12;
    } else {
        pActwk->xposi.w.h = ((Sint16 *)pActwk)[26];
        pActwk->yposi.w.h = ((Sint16 *)pActwk)[27];
        pActwk->yposi.w.h += 348;

        pActwk->patno = 1;
        pActwk->sprhsize = 8;
        pActwk->sprvsize = 112;
    }
}

static Sint16 get_x(sprite_status *pActwk) {
    Sint16 wk1, wk2;

    wk1 = (Uint32)scra_h_posit.w.h / 320;
    wk2 = (Uint32)scra_h_posit.w.h % 320;

    if (wk1 > 48) {
        wk1 = 0;
    }

    wk1 = ((wk1 + 1) * 320) + 256;
    wk2 = wk2 * 256 / 320;

    return wk1 - wk2;
}

static void gaitou73_02(sprite_status *pActwk) {
    sprite_status *pBptr;

    pBptr = &actwk[((Sint16 *)pActwk)[25]];
    pActwk->xposi.w.h = ((Sint16 *)pBptr)[26];
    pActwk->yposi.w.h = ((Sint16 *)pBptr)[27];
    pActwk->yposi.w.h += 124;
}
