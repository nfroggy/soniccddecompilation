#include <stddef.h>

#include "../equ.h"
#include "brunko4.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"
#include "../ridechk.h"

#pragma pack(push, 1)
typedef struct {
    Sint8 unused0[8];
    Sint16 origin_y;
    Sint16 helper_parent_index;
    Sint16 origin_x;
    Sint8 helper_y_offset;
    Sint8 helper_x_offset;
    Uint8 angle_high;
    Uint8 angle_low;
    Uint8 link_id;
} brunko4_work;
#pragma pack(pop)

_Static_assert(offsetof(brunko4_work, origin_y) == 8,
               "brunko4_work.origin_y offset");
_Static_assert(offsetof(brunko4_work, helper_parent_index) == 10,
               "brunko4_work.helper_parent_index offset");
_Static_assert(offsetof(brunko4_work, origin_x) == 12,
               "brunko4_work.origin_x offset");
_Static_assert(offsetof(brunko4_work, helper_y_offset) == 14,
               "brunko4_work.helper_y_offset offset");
_Static_assert(offsetof(brunko4_work, helper_x_offset) == 15,
               "brunko4_work.helper_x_offset offset");
_Static_assert(offsetof(brunko4_work, angle_high) == 16,
               "brunko4_work.angle_high offset");
_Static_assert(offsetof(brunko4_work, angle_low) == 17,
               "brunko4_work.angle_low offset");
_Static_assert(offsetof(brunko4_work, link_id) == 18,
               "brunko4_work.link_id offset");
_Static_assert(sizeof(brunko4_work) <= sizeof(((sprite_status *)0)->actfree),
               "brunko4_work fits in actfree");

static brunko4_work *brunko4_get_work(sprite_status *pActwk) {
    return (brunko4_work *)pActwk->actfree;
}

#if defined(R42A)
#define SPRITE_BRUNKO4_BASE 476
#else
#define SPRITE_BRUNKO4_BASE 470
#endif

static void brunko4_ridechk(sprite_status *pActwk);
static void brunko_init(sprite_status *pActwk);
static void brunko_move(sprite_status *pActwk);
static void brunko4_posiset(sprite_status *pActwk);

static sprite_pattern pat0 = {1, {{-8, -8, 0, SPRITE_BRUNKO4_BASE}}};
static sprite_pattern pat1 = {1, {{-32, -8, 0, SPRITE_BRUNKO4_BASE + 1}}};
static sprite_pattern pat2 = {1, {{-8, -8, 0, SPRITE_BRUNKO4_BASE + 2}}};
sprite_pattern *brunko4pat[3] = {&pat0, &pat1, &pat2};

void brunko4(sprite_status *pActwk) {
    Sint16 wD0, wD1;
    static void (*tbl[2])(sprite_status *) = {&brunko_init, &brunko_move};

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);

    wD0 = brunko4_get_work(pActwk)->origin_x & -128;
    wD1 = (scra_h_posit.w.h - 128) & -128;
    wD0 -= wD1;
    if ((Uint16)wD0 <= 640)
        return;
    frameout(pActwk);
}

static void brunko4_ridechk(sprite_status *pActwk) {
    if (ridechk(pActwk, &actwk[0])) {

        actwk[0].yposi.w.h = (pActwk->yposi.w.h - (Sint16)pActwk->sprvsize) -
                             (Sint16)actwk[0].sprvsize + 2;
    }
}

static void brunko_init(sprite_status *pActwk) {
    sprite_status *subActwk;
    Uint8 id;
    Sint32 i;

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 4;
    pActwk->patbase = brunko4pat;
    pActwk->sproffset = 832;
    if (pActwk->patno == 1)
        pActwk->sproffset += 16384;

    pActwk->sprhsize = 32;
    pActwk->sprvsize = 10;
    brunko4_get_work(pActwk)->angle_high = 192;

    if (brunko4_get_work(pActwk)->link_id == 6) {
        if (actwkchk2(pActwk, &subActwk) == 0) {
            subActwk->actno = 39;
            brunko4_get_work(subActwk)->helper_parent_index =
                (Uint16)(Uint8)(pActwk - actwk);
            brunko4_get_work(subActwk)->helper_x_offset = -16;
            brunko4_get_work(subActwk)->helper_y_offset = 16;
            subActwk->userflag.b.h = (Uint8)(pActwk->userflag.b.h & 15);
        }
    }

    if (!brunko4_get_work(pActwk)->link_id) {
        brunko4_get_work(pActwk)->origin_x = pActwk->xposi.w.h;
        brunko4_get_work(pActwk)->origin_y = pActwk->yposi.w.h;

        id = 6;
        for (i = 0; i < 6; ++i) {
            if (actwkchk(&subActwk) == 0) {
                subActwk->actno = 41;
                brunko4_get_work(subActwk)->origin_x =
                    brunko4_get_work(pActwk)->origin_x;
                brunko4_get_work(subActwk)->origin_y =
                    brunko4_get_work(pActwk)->origin_y;
                subActwk->userflag.b.h = pActwk->userflag.b.h;
                brunko4_get_work(subActwk)->link_id = id;
                subActwk->patno = 2;
                if (id == 6)
                    subActwk->patno = 1;
            }

            --id;
        }
    }
    brunko_move(pActwk);
}

static void brunko_move(sprite_status *pActwk) {
    int_union xposi_bak, yposi_bak;
    Sint32 xposi_f, yposi_f;
    Sint32 i;

    xposi_bak.l = pActwk->xposi.l;
    yposi_bak.l = pActwk->yposi.l;
    brunko4_posiset(pActwk);
    if (brunko4_get_work(pActwk)->link_id == 6) {
        xposi_bak.l = pActwk->xposi.l - xposi_bak.l;
        yposi_bak.l = pActwk->yposi.l - yposi_bak.l;
        xposi_f = xposi_bak.l & 0x80000000;
        yposi_f = yposi_bak.l & 0x80000000;
        for (i = 0; i < 8; ++i)
            xposi_bak.l = xposi_bak.l >> 1 | xposi_f;
        for (i = 0; i < 8; ++i)
            yposi_bak.l = yposi_bak.l >> 1 | yposi_f;
        pActwk->xspeed.w = xposi_bak.w.l;
        pActwk->yspeed.w = yposi_bak.w.l;
        brunko4_ridechk(pActwk);
    }
}

static void brunko4_posiset(sprite_status *pActwk) {
    short_union temp;
    Uint16 sin, cos, sinf, cosf, wD3, wD4;
    Uint8 bD0;
    Sint32 i;
    brunko4_work *pWork = brunko4_get_work(pActwk);

    temp.b.h = pWork->angle_high;
    temp.b.l = pWork->angle_low;
    if (!(pActwk->userflag.b.h & 16))
        temp.w -= 128;
    else
        temp.w += 256;
    pWork->angle_high = temp.b.h;
    pWork->angle_low = temp.b.l;

    sinset(pWork->angle_high, (Sint16 *)&sin, (Sint16 *)&cos);
    wD3 = wD4 = 0;
    bD0 = pWork->link_id;
    if (bD0) {
        do {
            wD3 += sin;
            wD4 += cos;
        } while (--bD0 != 0);
    }

    sinf = wD3 & 32768;
    cosf = wD4 & 32768;
    for (i = 4; i > 0; --i)
        wD3 = wD3 >> 1 | sinf;
    for (i = 4; i > 0; --i)
        wD4 = wD4 >> 1 | cosf;
    wD3 += (Uint16)pWork->origin_y;
    wD4 += (Uint16)pWork->origin_x;
    pActwk->yposi.w.h = wD3;
    pActwk->xposi.w.h = wD4;
}
