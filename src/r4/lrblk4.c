#include <stddef.h>

#include "../equ.h"
#include "lrblk4.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"
#include "../ridechk.h"

#if defined(R41B)
#define SPRITE_LRBLK4_BASE 497
#elif defined(R41C)
#define SPRITE_LRBLK4_BASE 485
#elif defined(R42A)
#define SPRITE_LRBLK4_BASE 504
#elif defined(R42B)
#define SPRITE_LRBLK4_BASE 519
#else
#define SPRITE_LRBLK4_BASE 491
#endif

typedef struct {
    Uint8 cnt;
    Uint8 dummy;
    Sint16 speed;
} move_data;

#pragma pack(push, 1)
typedef struct {
    Uint8 unused0[6];
    Sint16 move_speed;
    Sint16 wave_origin_x;
    Uint8 unused10[2];
    Sint16 base_x;
    Uint8 unused14[2];
    Uint8 phase;
    Uint8 segment_index;
    Uint8 child_index;
    Uint8 variant;
} lrblk4_work;
#pragma pack(pop)

_Static_assert(offsetof(lrblk4_work, move_speed) == 6,
               "lrblk4_work.move_speed offset");
_Static_assert(offsetof(lrblk4_work, wave_origin_x) == 8,
               "lrblk4_work.wave_origin_x offset");
_Static_assert(offsetof(lrblk4_work, base_x) == 12,
               "lrblk4_work.base_x offset");
_Static_assert(offsetof(lrblk4_work, phase) == 16,
               "lrblk4_work.phase offset");
_Static_assert(offsetof(lrblk4_work, segment_index) == 17,
               "lrblk4_work.segment_index offset");
_Static_assert(offsetof(lrblk4_work, child_index) == 18,
               "lrblk4_work.child_index offset");
_Static_assert(offsetof(lrblk4_work, variant) == 19,
               "lrblk4_work.variant offset");
_Static_assert(sizeof(lrblk4_work) <= sizeof(((sprite_status *)0)->actfree),
               "lrblk4_work fits in actfree");

static lrblk4_work *lrblk4_get_work(sprite_status *pActwk) {
    return (lrblk4_work *)pActwk->actfree;
}

static void lrblk4_foutchk(sprite_status *pActwk);
static void lrblk4_type1(sprite_status *pActwk);
static void type1_init(sprite_status *pActwk);
static void type1_move(sprite_status *pActwk);
static void lrblk4_type2(sprite_status *pActwk);
static void type2_init(sprite_status *pActwk);
static void type2_move(sprite_status *pActwk);
static void lrblk4_type3(sprite_status *pActwk);
static void type3_init(sprite_status *pActwk);
static void type3_move(sprite_status *pActwk);
static void lrblk4_type4(sprite_status *pActwk);
static void type4_init(sprite_status *pActwk);
static void type4_move(sprite_status *pActwk);
static void lrblk4_type5(sprite_status *pActwk);
static void type5_init(sprite_status *pActwk);
static void type5_move(sprite_status *pActwk);
static void type5_end(sprite_status *pActwk);
static void lrblk4_type6(sprite_status *pActwk);
static void type6_init(sprite_status *pActwk);
static void type6_move(sprite_status *pActwk);
static void lrblk4_type8(sprite_status *pActwk);
static void type8_init(sprite_status *pActwk);
static void type8_move(sprite_status *pActwk);

static sprite_pattern pat10 = {1, {{-64, -16, 0, SPRITE_LRBLK4_BASE}}};
static sprite_pattern pat11 = {1, {{-48, -16, 0, SPRITE_LRBLK4_BASE + 1}}};
static sprite_pattern pat20 = {1, {{-32, -16, 0, SPRITE_LRBLK4_BASE + 2}}};
static sprite_pattern pat30 = {1, {{-32, -32, 0, SPRITE_LRBLK4_BASE + 3}}};
static sprite_pattern pat50 = {1, {{-32, -32, 0, SPRITE_LRBLK4_BASE + 4}}};
static sprite_pattern pat60 = {1, {{-16, -16, 0, SPRITE_LRBLK4_BASE + 5}}};
static sprite_pattern pat61 = {1, {{-32, -16, 0, SPRITE_LRBLK4_BASE + 6}}};
static sprite_pattern pat62 = {1, {{-48, -16, 0, SPRITE_LRBLK4_BASE + 7}}};
static sprite_pattern pat63 = {1, {{-64, -16, 0, SPRITE_LRBLK4_BASE + 8}}};
sprite_pattern *lrblk4pat1[2] = {&pat10, &pat11};
sprite_pattern *lrblk4pat2[1] = {&pat20};
sprite_pattern *lrblk4pat3[1] = {&pat30};
sprite_pattern *lrblk4pat5[1] = {&pat50};
sprite_pattern *lrblk4pat6[4] = {&pat60, &pat61, &pat62, &pat63};
sprite_pattern *lrblk4pat8[2] = {&pat10, &pat11};

void lrblk4(sprite_status *pActwk) {
    void (*tbl[9])(sprite_status *) = {
        &lrblk4_type1, &lrblk4_type1, &lrblk4_type2,
        &lrblk4_type3, &lrblk4_type4, &lrblk4_type5,
        &lrblk4_type6, &lrblk4_type1, &lrblk4_type8};

    tbl[pActwk->userflag.b.h](pActwk);
}

static void lrblk4_foutchk(sprite_status *pActwk) {
    Sint16 d0, d1;
    lrblk4_work *pWork = lrblk4_get_work(pActwk);

    d0 = pWork->base_x;
    d0 &= -128;
    d1 = scra_h_posit.w.h;
    d1 -= 128;
    d1 &= -128;
    d0 -= d1;
    if ((Uint16)d0 > 640) {
        d0 = (Uint16)pActwk->cdsts;
        if (d0) {
            d0 *= 3;
            d0 += (Uint16)time_flag;
            flagwork[d0] &= 127;
        }
        frameout(pActwk);
    }
}

static void lrblk4_type1(sprite_status *pActwk) {
    void (*tbl[2])(sprite_status *) = {&type1_init, &type1_move};

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    lrblk4_foutchk(pActwk);
}

static void type1_init(sprite_status *pActwk) {
    sprite_status *pNewActwk;
    Sint16 i;
    Uint16 d0;
    Uint8 d1;

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 17514;
    pActwk->sprpri = 3;
    pActwk->patbase = lrblk4pat1;
    lrblk4_get_work(pActwk)->wave_origin_x = pActwk->xposi.w.h;
    pActwk->sprhsize = 48;
    pActwk->sprvsize = 16;
    pActwk->patno = 1;

    if (lrblk4_get_work(pActwk)->child_index) {
        type1_move(pActwk);
        return;
    }

    lrblk4_get_work(pActwk)->base_x = pActwk->xposi.w.h;
    if (lrblk4_get_work(pActwk)->variant == 0) {
        d1 = 1;
        for (i = 0; i <= 4; ++i) {
            if (actwkchk(&pNewActwk) == 0) {
                pNewActwk->actno = 36;
                pNewActwk->userflag.b.h = 1;
                lrblk4_get_work(pNewActwk)->variant = d1;
                pNewActwk->xposi.w.h = pActwk->xposi.w.h;

                lrblk4_get_work(pNewActwk)->phase = (char)(d1 * 32) + 16;
                d0 = d1;
                d0 *= 96;
                d0 += pActwk->yposi.w.h;
                pNewActwk->yposi.w.h = d0;
            }

            ++d1;
        }
    }
    if (actwkchk(&pNewActwk) == 0) {
        pNewActwk->actno = 36;
        pNewActwk->userflag.b.h = 1;
        lrblk4_get_work(pNewActwk)->child_index = 1;

        lrblk4_get_work(pNewActwk)->base_x = lrblk4_get_work(pActwk)->base_x;
        pNewActwk->yposi.w.h = pActwk->yposi.w.h;
        pNewActwk->xposi.w.h = pActwk->xposi.w.h + 224;
        lrblk4_get_work(pNewActwk)->phase = lrblk4_get_work(pActwk)->phase;
    }
    type1_move(pActwk);
}

static void type1_move(sprite_status *pActwk) {
    Sint16 d0, d1, stk;
    lrblk4_work *pWork = lrblk4_get_work(pActwk);

    stk = pActwk->xposi.w.h;
    sinset(pWork->phase, &d0, &d1);
    d0 >>= 3;
    ++pWork->phase;
    d0 += pWork->wave_origin_x;
    pActwk->xposi.w.h = d0;
    d1 = stk;
    d0 -= d1;
    d0 <<= 8;
    pActwk->xspeed.w = d0;
    hitchk(pActwk, &actwk[0]);
}

static void lrblk4_type2(sprite_status *pActwk) {
    void (*tbl[2])(sprite_status *) = {&type2_init, &type2_move};

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    lrblk4_foutchk(pActwk);
}

static void type2_init(sprite_status *pActwk) {
    sprite_status *pNewActwk;

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 17514;
    pActwk->sprpri = 3;
    pActwk->patbase = lrblk4pat2;
    pActwk->sprhsize = 32;
    pActwk->sprvsize = 16;

    if (lrblk4_get_work(pActwk)->child_index) {
        type2_move(pActwk);
        return;
    }

    lrblk4_get_work(pActwk)->base_x = pActwk->xposi.w.h;

    lrblk4_get_work(pActwk)->wave_origin_x = pActwk->xposi.w.h - 32;

    if (actwkchk(&pNewActwk) == 0) {
        pNewActwk->actno = 36;
        pNewActwk->userflag.b.h = 2;
        lrblk4_get_work(pNewActwk)->child_index = 1;

        lrblk4_get_work(pNewActwk)->base_x = lrblk4_get_work(pActwk)->base_x;
        pNewActwk->yposi.w.h = pActwk->yposi.w.h;
        pNewActwk->xposi.w.h = pActwk->xposi.w.h + 64;
        lrblk4_get_work(pNewActwk)->wave_origin_x = pNewActwk->xposi.w.h + 32;
        lrblk4_get_work(pNewActwk)->phase = lrblk4_get_work(pActwk)->phase - 128;
    }
    type2_move(pActwk);
}

static void type2_move(sprite_status *pActwk) {
    Sint16 d0, d1, stk;
    lrblk4_work *pWork = lrblk4_get_work(pActwk);

    stk = pActwk->xposi.w.h;
    sinset(pWork->phase, &d0, &d1);
    d0 >>= 3;
    ++pWork->phase;
    d0 += pWork->wave_origin_x;
    pActwk->xposi.w.h = d0;
    d1 = stk;
    d0 -= d1;
    d0 <<= 8;
    pActwk->xspeed.w = d0;
    hitchk(pActwk, &actwk[0]);
}

static void lrblk4_type3(sprite_status *pActwk) {
    void (*tbl[2])(sprite_status *) = {&type3_init, &type3_move};

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    lrblk4_foutchk(pActwk);
}

static void type3_init(sprite_status *pActwk) {
    sprite_status *pNewActwk;
    Sint16 *a2;
    Sint16 i;
    Uint8 d0, d1;
    Sint16 tbl[18] = {0, 0,  -128, -144, 0,  -128, 144, 0,  -128,
                      0, 64, 128,  -144, 64, 128,  144, 64, 128};

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 17514;
    pActwk->sprpri = 3;
    pActwk->patbase = lrblk4pat3;
    pActwk->sprhsize = 32;
    pActwk->sprvsize = 32;

    if (lrblk4_get_work(pActwk)->child_index == 0) {
        lrblk4_get_work(pActwk)->base_x = pActwk->xposi.w.h;
        d1 = 1;
        for (i = 0; i <= 4; ++i) {
            if (actwkchk(&pNewActwk) == 0) {
                pNewActwk->actno = 36;
                pNewActwk->userflag.b.h = 3;
                lrblk4_get_work(pNewActwk)->child_index = d1;
                lrblk4_get_work(pNewActwk)->base_x = lrblk4_get_work(pActwk)->base_x;
                pNewActwk->xposi.w.h = pActwk->xposi.w.h;
                pNewActwk->yposi.w.h = pActwk->yposi.w.h;
            }
            ++d1;
        }
    }
    d0 = lrblk4_get_work(pActwk)->child_index * 6;
    a2 = &tbl[d0 / 2];
    pActwk->xposi.w.h += *a2++;
    pActwk->yposi.w.h += *a2++;
    pActwk->xspeed.w = *a2++;
}

static void type3_move(sprite_status *pActwk) {
    sprite_status *pPlayerwk;
    Sint32 ld0;
    Sint16 d0;
    lrblk4_work *pWork = lrblk4_get_work(pActwk);

    ld0 = pActwk->xspeed.w;
    ld0 <<= 8;
    pActwk->xposi.l += ld0;

    if (pActwk->xspeed.w < 0) {
        d0 = pWork->base_x;
        if ((Uint16)d0 >= (Uint16)pActwk->xposi.w.h) {
            d0 -= pActwk->xposi.w.h;
            if ((Uint16)d0 >= 224) {
                d0 = pWork->base_x;
                d0 += 224;
                pActwk->xposi.w.h = d0;
            }
        }
    } else {
        d0 = pActwk->xposi.w.h;
        if ((Uint16)d0 >= (Uint16)pWork->base_x) {
            d0 -= pWork->base_x;
            if ((Uint16)d0 >= 224) {
                d0 = pWork->base_x;
                d0 -= 224;
                pActwk->xposi.w.h = d0;
            }
        }
    }

    if (pActwk->actflg & 128) {
        pPlayerwk = &actwk[0];
        d0 = pPlayerwk->yposi.w.h;
        if ((Uint16)d0 < (Uint16)pActwk->yposi.w.h) {
            d0 -= pActwk->yposi.w.h;
            d0 *= -1;
        } else {
            d0 -= pActwk->yposi.w.h;
        }
        if ((Uint16)d0 < 64) {
            hitchk(pActwk, pPlayerwk);
        }
    }
}

static void lrblk4_type4(sprite_status *pActwk) {
    void (*tbl[2])(sprite_status *) = {&type4_init, &type4_move};

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    lrblk4_foutchk(pActwk);
}

static void type4_init(sprite_status *pActwk) {
    sprite_status *pNewActwk;

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 17514;
    pActwk->sprpri = 3;

    pActwk->sprhsize = 64;
    pActwk->sprvsize = 64;
    if (lrblk4_get_work(pActwk)->variant) {
        pActwk->patbase = lrblk4pat8;
        pActwk->sprvsize = 16;
    }

    if (lrblk4_get_work(pActwk)->child_index == 0) {
        lrblk4_get_work(pActwk)->base_x = pActwk->xposi.w.h;
        if (actwkchk(&pNewActwk) == 0) {
            pNewActwk->actno = 36;
            pNewActwk->userflag.b.h = 4;
            lrblk4_get_work(pNewActwk)->child_index = 1;
            lrblk4_get_work(pNewActwk)->variant = lrblk4_get_work(pActwk)->variant;
            lrblk4_get_work(pNewActwk)->base_x = lrblk4_get_work(pActwk)->base_x;

            pNewActwk->xposi.w.h = lrblk4_get_work(pActwk)->base_x + 128;
            pNewActwk->yposi.w.h = pActwk->yposi.w.h;

            pActwk->xposi.w.h = lrblk4_get_work(pActwk)->base_x - 128;
        }
    }
    pActwk->xspeed.w = -512;
}

static void type4_move(sprite_status *pActwk) {
    Sint16 d0;
    lrblk4_work *pWork = lrblk4_get_work(pActwk);

    pActwk->xposi.l += pActwk->xspeed.w << 8;
    hitchk(pActwk, &actwk[0]);
    d0 = pWork->base_x;
    if (d0 >= pActwk->xposi.w.h) {
        d0 -= pActwk->xposi.w.h;
        if (d0 == 256) {
            pActwk->xposi.w.h = pWork->base_x + 256;
        }
    }
}

static void lrblk4_type5(sprite_status *pActwk) {
    void (*tbl[3])(sprite_status *) = {&type5_init, &type5_move, &type5_end};

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    lrblk4_foutchk(pActwk);
}

static void type5_init(sprite_status *pActwk) {
    sprite_status *pNewActwk;
    Uint8 *a1;
    Sint16 i;
    Uint16 d0;
    Uint8 d1;
    Uint8 tbl[14] = {0, 0, 64, 1, 128, 2, 192, 3, 128, 4, 64, 5, 0, 0};

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 17514;
    pActwk->sprpri = 3;
    pActwk->patbase = lrblk4pat5;
    lrblk4_get_work(pActwk)->base_x = pActwk->xposi.w.h;
    pActwk->sprhsize = 32;
    pActwk->sprvsize = 32;

    if (lrblk4_get_work(pActwk)->child_index == 0) {
        d1 = 1;
        for (i = 0; i <= 3; ++i) {
            if (actwkchk(&pNewActwk) == 0) {
                pNewActwk->actno = 36;
                pNewActwk->userflag.b.h = 5;
                lrblk4_get_work(pNewActwk)->child_index = d1;
                pNewActwk->xposi.w.h = pActwk->xposi.w.h;
                d0 = d1;
                d0 *= 64;
                pNewActwk->yposi.w.h = pActwk->yposi.w.h + (Sint16)d0;
            }
            ++d1;
        }
    }
    a1 = &tbl[lrblk4_get_work(pActwk)->child_index * 2];
    pActwk->xposi.w.h += (Uint16)*a1++;
    lrblk4_get_work(pActwk)->segment_index = *a1++;
}

static void type5_move(sprite_status *pActwk) {
    move_data *a1;
    Sint16 d0;
    lrblk4_work *pWork = lrblk4_get_work(pActwk);

    move_data type5_mvtbl[6] = {{128, 0, 128},  {128, 0, 128},  {128, 0, 128},
                                {128, 0, -128}, {128, 0, -128}, {128, 0, -128}};

label1:
    if (pWork->phase == 0) {
        a1 = &type5_mvtbl[pWork->segment_index];
        pWork->phase = a1->cnt;
        pWork->move_speed = a1->speed;
    } else {
        d0 = pWork->move_speed;
        pActwk->xspeed.w = d0;
        pActwk->xposi.l += d0 << 8;
        --pWork->phase;
        if (pWork->phase == 0) {
            ++pWork->segment_index;
            if (pWork->segment_index != 6)
                goto label1;

            pWork->segment_index = 0;
            goto label1;
        }
    }
    type5_end(pActwk);
}

static void type5_end(sprite_status *pActwk) { hitchk(pActwk, &actwk[0]); }

static void lrblk4_type6(sprite_status *pActwk) {
    void (*tbl[2])(sprite_status *) = {&type6_init, &type6_move};

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    lrblk4_foutchk(pActwk);
}

static void type6_init(sprite_status *pActwk) {
    sprite_status *pNewActwk;
    uint_union dw;
    Sint16 i;
    Sint16 d0;
    Uint8 bd0;
    Uint8 d1;

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 17514;
    pActwk->sprpri = 3;
    pActwk->patbase = lrblk4pat6;
    pActwk->sprvsize = 16;

    if (lrblk4_get_work(pActwk)->child_index == 0) {
        lrblk4_get_work(pActwk)->base_x = pActwk->xposi.w.h;
        d1 = 1;
        for (i = 0; i <= 2; ++i) {
            if (actwkchk(&pNewActwk) == 0) {
                pNewActwk->actno = 36;
                pNewActwk->userflag.b.h = 6;
                lrblk4_get_work(pNewActwk)->child_index = d1;
                lrblk4_get_work(pNewActwk)->base_x = lrblk4_get_work(pActwk)->base_x;
                d0 = (char)d1 * 16;
                d0 *= -1;
                d0 += pActwk->xposi.w.h;
                pNewActwk->xposi.w.h = d0;
                dw.l = d1;
                dw.l <<= 5;
                d0 = dw.w.l;
                d0 += pActwk->yposi.w.h;
                pNewActwk->yposi.w.h = d0;
            }
            ++d1;
        }
    }
    bd0 = lrblk4_get_work(pActwk)->child_index;
    pActwk->patno = bd0;
    ++bd0;
    lrblk4_get_work(pActwk)->segment_index = bd0;
    bd0 = lrblk4_get_work(pActwk)->child_index;
    d1 = 0;
    for (i = 0; i <= (Sint16)(Uint16)bd0; ++i) {
        d1 += 16;
    }
    pActwk->sprhsize = d1;
    d0 = pActwk->xposi.w.h;
    d0 += (Uint16)d1;
    lrblk4_get_work(pActwk)->wave_origin_x = d0;
    lrblk4_get_work(pActwk)->phase = 128;

    type6_move(pActwk);
}

static void type6_move(sprite_status *pActwk) {
    Sint16 d0, d1, d3, stk;
    Uint8 d2;
    lrblk4_work *pWork = lrblk4_get_work(pActwk);

    stk = pActwk->xposi.w.h;
    sinset(pWork->phase, &d0, &d1);

    d3 = 0;
    d2 = pWork->segment_index;
    if (d2) {
        do {
            d3 += d0;
            --d2;
        } while (d2 != 0);
    }

    d3 >>= 4;
    d0 = d3;
    d0 += pWork->wave_origin_x;
    pActwk->xposi.w.h = d0;

    d1 = stk;
    d0 -= d1;
    d0 <<= 8;
    pActwk->xspeed.w = d0;
    ++pWork->phase;
    hitchk(pActwk, &actwk[0]);
}

static void lrblk4_type8(sprite_status *pActwk) {
    void (*tbl[2])(sprite_status *) = {&type8_init, &type8_move};

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    lrblk4_foutchk(pActwk);
}

static void type8_init(sprite_status *pActwk) {
    sprite_status *pNewActwk;
    Sint16 d0;

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 17514;
    pActwk->sprpri = 3;
    pActwk->patbase = lrblk4pat8;
    pActwk->sprhsize = 64;
    pActwk->sprvsize = 16;

    if (lrblk4_get_work(pActwk)->child_index == 0) {
        lrblk4_get_work(pActwk)->base_x = pActwk->xposi.w.h;
        if (actwkchk(&pNewActwk) == 0) {
            pNewActwk->actno = 36;
            pNewActwk->userflag.b.h = 4;
            lrblk4_get_work(pNewActwk)->variant = 8;
            pNewActwk->xposi.w.h = pActwk->xposi.w.h;
            pNewActwk->yposi.w.h = pActwk->yposi.w.h - 96;
        }
        if (actwkchk(&pNewActwk) == 0) {
            pNewActwk->actno = 36;
            pNewActwk->userflag.b.h = 8;
            lrblk4_get_work(pNewActwk)->child_index = 1;
            lrblk4_get_work(pNewActwk)->base_x = lrblk4_get_work(pActwk)->base_x;
            d0 = lrblk4_get_work(pActwk)->base_x;
            d0 -= 128;
            pNewActwk->xposi.w.h = d0;
            pNewActwk->yposi.w.h = pActwk->yposi.w.h;
            d0 = lrblk4_get_work(pActwk)->base_x;
            d0 += 128;
            pActwk->xposi.w.h = d0;
        }
    }
    pActwk->xspeed.w = 512;
}

static void type8_move(sprite_status *pActwk) {
    Sint16 d0;
    lrblk4_work *pWork = lrblk4_get_work(pActwk);

    pActwk->xposi.l += pActwk->xspeed.w << 8;
    hitchk(pActwk, &actwk[0]);
    d0 = pActwk->xposi.w.h;
    if (d0 >= pWork->base_x) {
        d0 -= pWork->base_x;
        if (d0 == 256) {
            pActwk->xposi.w.h = pWork->base_x - 256;
        }
    }
}
