#include "../equ.h"
#include "funsui4.h"
#include "../action.h"
#include "../actset.h"
#include "../ridechk.h"
#include "playsub4.h"
#include <stddef.h>

static void funsui4_init(sprite_status *pActwk);
static void funsui4_move(sprite_status *pActwk);
static void sibuki_set(sprite_status *pActwk);
static void sibuki_init(sprite_status *pActwk);
static void sibuki_move(sprite_status *pActwk);

#pragma pack(push, 1)
typedef struct {
    Uint8 reserved0[16];
    Uint8 splash_index;
    Uint8 timer;
} funsui4_work;
#pragma pack(pop)

_Static_assert(offsetof(funsui4_work, splash_index) == 16,
               "funsui4_work.splash_index offset");
_Static_assert(offsetof(funsui4_work, timer) == 17,
               "funsui4_work.timer offset");
_Static_assert(sizeof(funsui4_work) <= sizeof(((sprite_status *)0)->actfree),
               "funsui4_work fits in actfree");

static funsui4_work *funsui4_work_get(sprite_status *pActwk) {
    return (funsui4_work *)pActwk->actfree;
}

Uint8 pchg0[7] = {0, 0, 1, 2, 3, 4, 255};
Uint8 pchg1[6] = {0, 5, 6, 7, 8, 255};
Uint8 *funsui4pchg[2] = {pchg0, pchg1};
static sprite_pattern pat0 = {2, {{-56, -80, 0, 498}, {-32, -64, 0, 503}}};
static sprite_pattern pat1 = {2, {{-56, -80, 0, 499}, {-32, -64, 0, 504}}};
static sprite_pattern pat2 = {2, {{-56, -80, 0, 500}, {-32, -64, 0, 505}}};
static sprite_pattern pat3 = {2, {{-56, -80, 0, 501}, {-32, -64, 0, 506}}};
static sprite_pattern pat4 = {2, {{-56, -80, 0, 502}, {-32, -64, 0, 507}}};
static sprite_pattern pat5 = {1, {{-16, -16, 0, 508}}};
static sprite_pattern pat6 = {1, {{-16, -16, 0, 509}}};
static sprite_pattern pat7 = {1, {{-16, -16, 0, 510}}};
static sprite_pattern pat8 = {1, {{-16, -16, 0, 511}}};
sprite_pattern *funsui4pat[9] = {&pat0, &pat1, &pat2, &pat3, &pat4,
                                 &pat5, &pat6, &pat7, &pat8};

void funsui4(sprite_status *pActwk) {
    void (*tbl[3])(sprite_status *) = {&funsui4_init, &funsui4_move,
                                       &sibuki_move};

    if (pActwk->userflag.b.h == -1) {
        sibuki(pActwk);
    } else {
        tbl[pActwk->r_no0 / 2](pActwk);
        actionsub(pActwk);
        frameout_s(pActwk);
    }
}

static void funsui4_init(sprite_status *pActwk) {
    lpcolorwk[55].peRed = 96;
    lpcolorwk[55].peGreen = 64;
    lpcolorwk[55].peBlue = 96;
    lpcolorwk[55].peFlags = 1;

    lpcolorwk[56].peRed = 160;
    lpcolorwk[56].peGreen = 128;
    lpcolorwk[56].peBlue = 160;
    lpcolorwk[56].peFlags = 1;

    lpcolorwk[57].peRed = 224;
    lpcolorwk[57].peGreen = 224;
    lpcolorwk[57].peBlue = 224;
    lpcolorwk[57].peFlags = 1;

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 25344;
    pActwk->sprpri = 1;
    pActwk->patbase = funsui4pat;
    pActwk->sprhsize = 32;
    pActwk->sprvsize = 60;

    funsui4_move(pActwk);
}

static void funsui4_move(sprite_status *pActwk) {
    sibuki_set(pActwk);
    patchg(pActwk, funsui4pchg);
    pActwk->yposi.w.h = waterposi - 48;
    ridechk(pActwk, &actwk[0]);
}

static void sibuki_set(sprite_status *pActwk) {
    funsui4_work *work = funsui4_work_get(pActwk);
    sprite_status *pNewActwk;
    funsui4_work *new_work;
    Sint16 *a2;
    Uint8 d0;

    Sint16 tbl[32] = {-40, -72, -32, -128, 8,  -72, 32,  -128,
                      40,  -72, 64,  -128, -8, -72, -64, -128,
                      -40, -72, 0,   -128, -8, -72, 0,   -128,
                      8,   -72, 0,   -128, 40, -72, 0,   -128};

    if ((Uint16)work->timer + 16 < 256) {
        work->timer += 16;
        return;
    }
    work->timer += 16;

    if (actwkchk(&pNewActwk) == 0) {
        new_work = funsui4_work_get(pNewActwk);
        pNewActwk->actno = pActwk->actno;
        pNewActwk->userflag.b.h = -1;
        pNewActwk->patno = 5;
        d0 = work->splash_index & 3;
        new_work->splash_index = d0;
        d0 <<= 3;
        a2 = &tbl[d0 / 2];
        pNewActwk->xposi.w.h = pActwk->xposi.w.h + *a2++;
        pNewActwk->yposi.w.h = pActwk->yposi.w.h + *a2++;
        pNewActwk->xspeed.w = *a2++;
        pNewActwk->yspeed.w = *a2++;

        ++work->splash_index;
    }
}

void sibuki(sprite_status *pActwk) {
    if (pActwk->r_no0)
        sibuki_move(pActwk);
    else
        sibuki_init(pActwk);
}

static void sibuki_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 25344;
    pActwk->sprpri = 3;
    pActwk->patbase = funsui4pat;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 16;
    pActwk->mstno.b.h = 1;

    sibuki_move(pActwk);
}

static void sibuki_move(sprite_status *pActwk) {
    funsui4_work *work = funsui4_work_get(pActwk);
    Sint32 d0;

    if ((Uint16)work->timer + 2 > 255) {
        work->timer += 2;
        frameout(pActwk);
        return;
    }
    work->timer += 2;

    d0 = pActwk->xspeed.w;
    d0 <<= 8;
    pActwk->xposi.l += d0;
    d0 = pActwk->yspeed.w;
    d0 <<= 8;
    pActwk->yposi.l += d0;
    pActwk->yspeed.w += 8;
    patchg(pActwk, funsui4pchg);
    actionsub(pActwk);
    frameout_s(pActwk);
}
