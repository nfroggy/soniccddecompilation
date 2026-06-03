#include "../equ.h"
#include "gaitou73.h"
#include "../action.h"
#include "../actset.h"
#include <stddef.h>

static void gaitou73_ini(sprite_status *pActwk);
static void gaitou73_01(sprite_status *pActwk);
static Sint16 get_x(sprite_status *pActwk);
static void gaitou73_02(sprite_status *pActwk);

#pragma pack(push, 1)
typedef struct {
    Uint8 reserved0[4];
    Sint16 partner_actor;
    Sint16 cached_x;
    Sint16 cached_y;
} gaitou73_work;
#pragma pack(pop)

_Static_assert(offsetof(gaitou73_work, partner_actor) == 4,
               "gaitou73_work.partner_actor offset");
_Static_assert(offsetof(gaitou73_work, cached_x) == 6,
               "gaitou73_work.cached_x offset");
_Static_assert(offsetof(gaitou73_work, cached_y) == 8,
               "gaitou73_work.cached_y offset");
_Static_assert(sizeof(gaitou73_work) <= sizeof(((sprite_status *)0)->actfree),
               "gaitou73_work fits in actfree");

static gaitou73_work *gaitou73_work_get(sprite_status *pActwk) {
    return (gaitou73_work *)pActwk->actfree;
}

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
            gaitou73_work_get(pNewact)->partner_actor =
                (Sint16)(pActwk - actwk);
            gaitou73_work_get(pActwk)->partner_actor =
                (Sint16)(pNewact - actwk);
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
    gaitou73_work *work = gaitou73_work_get(pActwk);

    work->cached_x = get_x(pActwk);
    work->cached_y = 352 - (scra_v_posit.w.h - 352) / 3;

    if (work->cached_y + 12 >= scra_v_posit.w.h) {

        pActwk->xposi.w.h = work->cached_x;
        pActwk->yposi.w.h = work->cached_y;
        pActwk->patno = 0;
        pActwk->sprhsize = 24;
        pActwk->sprvsize = 12;
    } else {
        pActwk->xposi.w.h = work->cached_x;
        pActwk->yposi.w.h = work->cached_y;
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
    gaitou73_work *base_work;

    pBptr = &actwk[gaitou73_work_get(pActwk)->partner_actor];
    base_work = gaitou73_work_get(pBptr);
    pActwk->xposi.w.h = base_work->cached_x;
    pActwk->yposi.w.h = base_work->cached_y;
    pActwk->yposi.w.h += 124;
}
