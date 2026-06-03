#include <stddef.h>

#include "../equ.h"
#include "hashi5.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"
#include "../player_work.h"
#include "../ridechk.h"

static void hashi5_init(sprite_status *pActwk);
static void hashi5_move(sprite_status *pActwk);
static void hashi5_move2(sprite_status *pActwk);
static void hashi5_posiget1p(sprite_status *pActwk);
static void hashi5_rideplayset1p(sprite_status *pActwk);
static void hashi5_posiset(sprite_status *pActwk);
static Sint32 hashi5_ridechk1p(sprite_status *pActwk);

#pragma pack(push, 1)
typedef struct {
    Uint8 child_indices[8];
    Sint16 origin_y;
    Sint16 parent_index;
    Sint16 origin_x;
    Uint8 unused14[2];
    Uint8 bend_angle;
    Uint8 unused17;
    Uint8 segment_order;
    Uint8 ride_segment;
    Uint8 unused20;
    Uint8 remove_flag;
} hashi5_work;
#pragma pack(pop)

_Static_assert(offsetof(hashi5_work, child_indices) == 0,
               "hashi5_work.child_indices offset");
_Static_assert(offsetof(hashi5_work, origin_y) == 8,
               "hashi5_work.origin_y offset");
_Static_assert(offsetof(hashi5_work, parent_index) == 10,
               "hashi5_work.parent_index offset");
_Static_assert(offsetof(hashi5_work, origin_x) == 12,
               "hashi5_work.origin_x offset");
_Static_assert(offsetof(hashi5_work, bend_angle) == 16,
               "hashi5_work.bend_angle offset");
_Static_assert(offsetof(hashi5_work, segment_order) == 18,
               "hashi5_work.segment_order offset");
_Static_assert(offsetof(hashi5_work, ride_segment) == 19,
               "hashi5_work.ride_segment offset");
_Static_assert(offsetof(hashi5_work, remove_flag) == 21,
               "hashi5_work.remove_flag offset");
_Static_assert(sizeof(hashi5_work) <= sizeof(((sprite_status *)0)->actfree),
               "hashi5_work fits in actfree");

static hashi5_work *hashi5_get_work(sprite_status *pActwk) {
    return (hashi5_work *)pActwk->actfree;
}

static sprite_pattern hashi5_pat0 = {1, {{-8, -8, 0, 429}}};
static sprite_pattern hashi5_pat1;
static sprite_pattern hashi5_pat2 = {1, {{-72, -8, 0, 430}}};
sprite_pattern *hashi5pat[3] = {&hashi5_pat0, &hashi5_pat1, &hashi5_pat2};
static Uint8 hashitbl[272] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  2,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  4,  2,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  2,  4,  4,  2,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  2,  4,  6,  4,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  2,  4,  6,  6,  4,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  4,
    6,  8,  6,  4,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  4,  6,  8,  8,
    6,  4,  2,  0,  0,  0,  0,  0,  0,  0,  0,  2,  4,  6,  8,  10, 8,  6,  4,
    2,  0,  0,  0,  0,  0,  0,  0,  2,  4,  6,  8,  10, 10, 8,  6,  4,  2,  0,
    0,  0,  0,  0,  0,  2,  4,  6,  8,  10, 12, 10, 8,  6,  4,  2,  0,  0,  0,
    0,  0,  2,  4,  6,  8,  10, 12, 12, 10, 8,  6,  4,  2,  0,  0,  0,  0,  2,
    4,  6,  8,  10, 12, 14, 12, 10, 8,  6,  4,  2,  0,  0,  0,  2,  4,  6,  8,
    10, 12, 14, 14, 12, 10, 8,  6,  4,  2,  0,  0,  2,  4,  6,  8,  10, 12, 14,
    16, 14, 12, 10, 8,  6,  4,  2,  0,  2,  4,  6,  8,  10, 12, 14, 16, 16, 14,
    12, 10, 8,  6,  4,  2};
static Uint8 hashitbl2[256] = {
    255, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   181, 255, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   126, 219, 255, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   97,  181, 236, 255, 0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   74,  147, 205, 243, 255, 0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   62,  126, 176, 219, 246, 255, 0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   56,  109, 157, 197, 228, 248, 255, 0,   0,
    0,   0,   0,   0,   0,   0,   0,   49,  97,  142, 181, 212, 236, 251, 255,
    0,   0,   0,   0,   0,   0,   0,   0,   43,  86,  126, 162, 193, 219, 238,
    251, 255, 0,   0,   0,   0,   0,   0,   0,   37,  74,  115, 147, 176, 205,
    225, 243, 252, 255, 0,   0,   0,   0,   0,   0,   31,  68,  103, 136, 167,
    189, 212, 231, 244, 253, 255, 0,   0,   0,   0,   0,   31,  62,  92,  126,
    152, 176, 201, 219, 234, 246, 253, 255, 0,   0,   0,   0,   25,  56,  86,
    115, 142, 167, 189, 209, 225, 238, 248, 254, 255, 0,   0,   0,   25,  56,
    80,  109, 131, 157, 176, 197, 216, 228, 241, 248, 254, 255, 0,   0,   25,
    49,  74,  103, 126, 147, 167, 189, 205, 219, 231, 243, 249, 254, 255, 0,
    25,  49,  74,  97,  120, 142, 162, 181, 197, 212, 225, 236, 244, 251, 254,
    255};

void hashi5(sprite_status *pActwk) {
    void (*hashi5_tbl[3])(sprite_status *) = {&hashi5_init, &hashi5_move,
                                              &hashi5_move2};

    hashi5_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
}

static void hashi5_init(sprite_status *pActwk) {
    sprite_status *pNewactwk;
    Sint32 i;
    hashi5_work *work = hashi5_get_work(pActwk);

    Sint16 xofsettbl[8] = {48, 32, 16, 0, -16, -32, -48, -64};

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->sproffset = 17152;
    pActwk->patbase = hashi5pat;
    work->origin_x = pActwk->xposi.w.h;
    work->origin_y = pActwk->yposi.w.h;
    pActwk->sprhsize = 64;
    pActwk->sprvsize = 8;
    pActwk->patno = 1;

    for (i = 0; i < 8; ++i) {

        if (actwkchk(&pNewactwk) == 0) {
            work->child_indices[i] = (Uint8)(pNewactwk - actwk);
            pNewactwk->actno = 47;
            pNewactwk->r_no0 = 4;
            hashi5_get_work(pNewactwk)->origin_x = pActwk->xposi.w.h;
            hashi5_get_work(pNewactwk)->origin_y = pActwk->yposi.w.h;
            pNewactwk->yposi.w.h = pActwk->yposi.w.h;
            pNewactwk->actflg |= 4;
            pNewactwk->sprpri = 3;
            pNewactwk->sproffset = 17152;
            pNewactwk->patbase = hashi5pat;
            pNewactwk->sprvsize = 8;
            pNewactwk->sprhsize = 8;
            hashi5_get_work(pNewactwk)->parent_index = pActwk - actwk;
            hashi5_get_work(pNewactwk)->remove_flag = 255;

            hashi5_get_work(pNewactwk)->segment_order = 7 - i;
            pNewactwk->xposi.w.h = xofsettbl[7 - i] + pActwk->xposi.w.h;
        }
    }
}

static void hashi5_move(sprite_status *pActwk) {
    hashi5_work *work = hashi5_get_work(pActwk);

    work->ride_segment = 0;
    if (hashi5_ridechk1p(pActwk) != 0) {
        hashi5_posiget1p(pActwk);
    }
    if (work->ride_segment) {
        if (work->bend_angle != 64) {
            work->bend_angle += 4;
        }
        hashi5_posiset(pActwk);

        if (work->ride_segment) {
            hashi5_rideplayset1p(pActwk);
        }
    } else {

        if (work->bend_angle) {
            work->bend_angle -= 4;
            hashi5_posiset(pActwk);
        }
    }

    frameout_s(pActwk);
}

static void hashi5_move2(sprite_status *pActwk) {
    hashi5_work *work = hashi5_get_work(pActwk);

    if (actwk[work->parent_index].actno != 47)
        frameout(pActwk);
    if (hashi5_get_work(&actwk[work->parent_index])->remove_flag)
        frameout(pActwk);
}

static void hashi5_posiget1p(sprite_status *pActwk) {
    Uint16 wD0;

    if (!(actwk[0].cddat & 2)) {
        wD0 = actwk[0].xposi.w.h - pActwk->xposi.w.h + 72;
        if (!(wD0 & 32768)) {
            if (wD0 < 128) {

                hashi5_get_work(pActwk)->ride_segment = wD0 >> 4;
                return;
            }
        }
    }

    hashi5_get_work(pActwk)->ride_segment = 0;
}

static void hashi5_rideplayset1p(sprite_status *pActwk) {
    Uint8 bD0;
    hashi5_work *work = hashi5_get_work(pActwk);

    bD0 = work->ride_segment;
    actwk[0].yposi.w.h = actwk[work->child_indices[bD0]].yposi.w.h - 8 -
                         (Sint16)(Uint16)actwk[0].sprvsize;
}

static void hashi5_posiset(sprite_status *pActwk) {
    sprite_status *pSubactwk;
    Uint16 Sin, Cos;
    int_union lD0;
    Sint16 wD2;
    Uint8 bD5;
    Uint8 *pA2;
    Uint8 *pA3;
    hashi5_work *work = hashi5_get_work(pActwk);

    wD2 = (Uint16)work->ride_segment;

    sinset(work->bend_angle, (Sint16 *)&Sin, (Sint16 *)&Cos);
    bD5 = hashitbl[wD2 + 128];
    pA3 = &hashitbl2[(wD2 + 128 & 15) * 16];
    pA2 = work->child_indices;

    for (; wD2 >= 0; --wD2) {
        pSubactwk = &actwk[*pA2++];
        lD0.l = 0;
        lD0.b.b4 = *pA3++;
        lD0.l = (lD0.w.l + 1) * bD5 * Sin;
        pSubactwk->yposi.w.h = hashi5_get_work(pSubactwk)->origin_y + lD0.w.h;
    }

    lD0.l = 0;
    lD0.b.b4 = work->ride_segment;
    lD0.b.b4 = -(lD0.b.b4 - 7);
    if (lD0.b.b4 >= 0) {
        wD2 = lD0.w.l;
        lD0.w.l *= 16;
        pA3 = &hashitbl2[lD0.w.l + wD2];
        if (--wD2 < 0)
            return;

        for (; wD2 >= 0; --wD2) {
            pSubactwk = &actwk[*pA2++];
            lD0.l = 0;
            lD0.w.l = (Uint16)(*--pA3) + 1;
            lD0.l = (lD0.w.l + 1) * bD5 * Sin;
            pSubactwk->yposi.w.h =
                hashi5_get_work(pSubactwk)->origin_y + lD0.w.h;
        }
    }
}

static Sint32 hashi5_ridechk1p(sprite_status *pActwk) {
    Sint16 wD0, wD1, wD2;

    if (actwk[0].r_no0 != 6) {
        if (!editmode.b.h) {
            wD1 = 72;
            wD2 = 128;

            if (actwk[0].yspeed.w >= 0) {
                wD0 = wD1 + (actwk[0].xposi.w.h - pActwk->xposi.w.h);
                if (wD0 >= 0) {
                    if (wD0 < wD2) {

                        wD0 = pActwk->yposi.w.h - 8;

                        wD2 = actwk[0].yposi.w.h;
                        wD1 = wD2 + (Sint16)actwk[0].sprvsize + 4;
                        if ((Uint16)wD0 <= (Uint16)wD1) {
                            wD0 -= wD1;
                            if ((Uint16)wD0 >= 65520) {
                                if (actwk[0].r_no0 < 6) {
                                    actwk[0].yposi.w.h = wD2 + wD0 + 3;

                                    ride_on_set(pActwk, &actwk[0]);
                                    return 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (player_work_get(&actwk[0])->ride_actor_index == pActwk - actwk) {
        player_work_get(&actwk[0])->jump_lock = 0;
        actwk[0].cddat &= 247;
    }

    return 0;
}
