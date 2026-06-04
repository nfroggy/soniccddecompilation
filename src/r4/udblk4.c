#include "../equ.h"
#include "udblk4.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"
#include "../ridechk.h"

#if defined(R41B)
#define SPRITE_UDBLK4_BASE 523
#elif defined(R41C)
#define SPRITE_UDBLK4_BASE 494
#elif defined(R41D)
#define SPRITE_UDBLK4_BASE 485
#elif defined(R42B)
#define SPRITE_UDBLK4_BASE 511
#else
#define SPRITE_UDBLK4_BASE 514
#endif

typedef struct {
    Uint8 kakudo;
    char xofst;
} kakudo_xofst;

typedef struct {
    char xofs;
    char yofs;
    Sint16 yspd;
} xofs_yofs_yspd;

typedef struct {
    Sint16 base_y;
    Sint16 parent_index;
    Sint16 base_x;
    Uint8 child_y_offset;
    Uint8 phase_high;
    Uint8 phase_low;
    Uint8 child_index;
    Uint8 variant;
} udblk4_work;

static udblk4_work *udblk4_get_work(sprite_status *pActwk) {
    return (udblk4_work *)pActwk->actfree;
}

static void sin_move(sprite_status *pActwk, Sint16 d2, Sint16 d3);
static void udblk4_type1(sprite_status *pActwk);
static void type1_init(sprite_status *pActwk);
static void type1_move(sprite_status *pActwk);
static void udblk4_type2(sprite_status *pActwk);
static void udblk4_type3(sprite_status *pActwk);
static void type3_init(sprite_status *pActwk);
static void type3_move(sprite_status *pActwk);
static void udblk4_type4(sprite_status *pActwk);
static void type4_coset(sprite_status *pActwk, sprite_status *pNewActwk,
                        Uint8 d0, Sint16 d1);
static void type4_init(sprite_status *pActwk);
static void type4_move(sprite_status *pActwk);
static void udblk4_type5(sprite_status *pActwk);
static void type5_coset(sprite_status *pActwk, sprite_status *pNewActwk,
                        Uint8 d0, Sint16 d1, Uint8 d2, Sint16 d3);
static void type5_init(sprite_status *pActwk);
static void type5_move(sprite_status *pActwk);
static void udblk4_type6(sprite_status *pActwk);
static void type6_init(sprite_status *pActwk);
static void type6_move(sprite_status *pActwk);
static void udblk4_type7(sprite_status *pActwk);
static void type7_coset(sprite_status *pActwk, sprite_status *pNewActwk,
                        Uint8 d0, Sint16 d1, Uint8 d2);
static void type7_init(sprite_status *pActwk);
static void type7_move(sprite_status *pActwk);
static void udblk4_type8(sprite_status *pActwk);
static void type8_init(sprite_status *pActwk);
static void type8_move(sprite_status *pActwk);
static void udblk4_type9(sprite_status *pActwk);
static void type9_init(sprite_status *pActwk);
static void type9_move(sprite_status *pActwk);
static void udblk4_typeA(sprite_status *pActwk);
static void typeA_init(sprite_status *pActwk);
static void typeA_move(sprite_status *pActwk);
static void udblk4_typeB(sprite_status *pActwk);
static void typeB_init(sprite_status *pActwk);
static void typeB_move(sprite_status *pActwk);

static sprite_pattern pat00 = {1, {{-16, -64, 0, SPRITE_UDBLK4_BASE}}};
static sprite_pattern pat01 = {1, {{-32, -96, 0, SPRITE_UDBLK4_BASE + 1}}};
static sprite_pattern pat02 = {1, {{-16, -48, 0, SPRITE_UDBLK4_BASE + 2}}};
static sprite_pattern pat03 = {1, {{-16, -64, 0, SPRITE_UDBLK4_BASE + 3}}};
static sprite_pattern pat04 = {1, {{-16, -32, 0, SPRITE_UDBLK4_BASE + 4}}};
static sprite_pattern pat05 = {1, {{-16, -96, 0, SPRITE_UDBLK4_BASE + 5}}};
static sprite_pattern pat06 = {1, {{-32, -48, 0, SPRITE_UDBLK4_BASE + 6}}};
static sprite_pattern pat07 = {1, {{-16, -16, 0, SPRITE_UDBLK4_BASE + 7}}};
sprite_pattern *udblk4pat1[1] = {&pat00};
sprite_pattern *udblk4pat3[1] = {&pat01};
sprite_pattern *udblk4pat4[1] = {&pat02};
sprite_pattern *udblk4pat5[1] = {&pat03};
sprite_pattern *udblk4pat7[1] = {&pat03};
sprite_pattern *udblk4pat8[1] = {&pat04};
sprite_pattern *udblk4pat6[2] = {&pat05, &pat06};
sprite_pattern *udblk4pat9[2] = {&pat05, &pat06};
sprite_pattern *udblk4patA[1] = {&pat07};

void udblk4(sprite_status *pActwk) {
    void (*tbl[12])(sprite_status *) = {
        &udblk4_type1, &udblk4_type1, &udblk4_type2, &udblk4_type3,
        &udblk4_type4, &udblk4_type5, &udblk4_type6, &udblk4_type7,
        &udblk4_type8, &udblk4_type9, &udblk4_typeA, &udblk4_typeB};

    tbl[pActwk->userflag.b.h](pActwk);
}

static void sin_move(sprite_status *pActwk, Sint16 d2, Sint16 d3) {
    int_union ld0;
    Sint32 stk;
    Sint16 d0, d1;
    udblk4_work *pWork = udblk4_get_work(pActwk);

    if (pActwk->yspeed.w < 0) {
        ride_on_chk(pActwk, &actwk[0]);
    }
    stk = pActwk->yposi.l;
    sinset(pWork->phase_high, &d0, &d1);
    d1 = d0;
    d0 <<= d2;
    d1 <<= d3;
    d0 += d1;
    d0 >>= 8;

    ++pWork->phase_high;
    d0 += pWork->base_y;
    pActwk->yposi.w.h = d0;
    ld0.w.h = d0;
    ld0.w.l = 0;
    ld0.l -= stk;
    ld0.l >>= 8;
    pActwk->yspeed.w = ld0.w.l;

    if (pActwk->yspeed.w >= 0) {
        ride_on_chk(pActwk, &actwk[0]);
    }
}

static void udblk4_type1(sprite_status *pActwk) {
    void (*tbl[2])(sprite_status *) = {&type1_init, &type1_move};

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s00(pActwk, udblk4_get_work(pActwk)->base_x);
}

static void type1_init(sprite_status *pActwk) {
    sprite_status *pNewActwk;
    Sint16 i;
    Uint8 d0, d1;
    kakudo_xofst tbl[7] = {{192, 0},  {168, -32}, {144, -64}, {120, -96},
                           {216, 32}, {240, 64},  {8, 96}};

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 17514;
    pActwk->sprpri = 3;
    pActwk->patbase = udblk4pat1;
    udblk4_get_work(pActwk)->base_y = pActwk->yposi.w.h + 64;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 64;

    if (udblk4_get_work(pActwk)->child_index == 0) {
        udblk4_get_work(pActwk)->base_x = pActwk->xposi.w.h;
        d1 = 1;
        for (i = 0; i <= 5; ++i) {
            if (actwkchk(&pNewActwk) == 0) {
                pNewActwk->actno = 35;
                pNewActwk->userflag.b.h = 1;
                pNewActwk->xposi.w.h = pActwk->xposi.w.h;
                pNewActwk->yposi.w.h = pActwk->yposi.w.h;
                udblk4_get_work(pNewActwk)->base_x =
                    udblk4_get_work(pActwk)->base_x;
                udblk4_get_work(pNewActwk)->child_index = d1;
            }
            ++d1;
        }
    }
    d0 = udblk4_get_work(pActwk)->child_index;
    udblk4_get_work(pActwk)->phase_high = tbl[d0].kakudo;
    pActwk->xposi.w.h += tbl[d0].xofst;
    type1_move(pActwk);
}

static void type1_move(sprite_status *pActwk) { sin_move(pActwk, 6, 0); }

static void udblk4_type2(sprite_status *pActwk) {}

static void udblk4_type3(sprite_status *pActwk) {
    void (*tbl[2])(sprite_status *) = {&type3_init, &type3_move};

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s00(pActwk, udblk4_get_work(pActwk)->base_x);
}

static void type3_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 17514;
    pActwk->sprpri = 3;
    pActwk->patbase = udblk4pat3;
    udblk4_get_work(pActwk)->base_x = pActwk->xposi.w.h;
    udblk4_get_work(pActwk)->base_y = pActwk->yposi.w.h - 80;
    pActwk->sprhsize = 32;
    pActwk->sprvsize = 96;
    udblk4_get_work(pActwk)->phase_high = 64;

    type3_move(pActwk);
}

static void type3_move(sprite_status *pActwk) { sin_move(pActwk, 6, 4); }

static void udblk4_type4(sprite_status *pActwk) {
    void (*tbl[2])(sprite_status *) = {&type4_init, &type4_move};

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s00(pActwk, udblk4_get_work(pActwk)->base_x);
}

static void type4_coset(sprite_status *pActwk, sprite_status *pNewActwk,
                        Uint8 d0, Sint16 d1) {
    pNewActwk->actno = 35;
    pNewActwk->userflag.b.h = 4;
    udblk4_get_work(pNewActwk)->child_index = d0;
    pNewActwk->yposi.w.h = pActwk->yposi.w.h;
    pNewActwk->xposi.w.h = pActwk->xposi.w.h + d1;
    udblk4_get_work(pNewActwk)->base_x = udblk4_get_work(pActwk)->base_x;
}

static void type4_init(sprite_status *pActwk) {
    sprite_status *pNewActwk;

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 17514;
    pActwk->sprpri = 3;
    pActwk->patbase = udblk4pat4;
    udblk4_get_work(pActwk)->base_y = pActwk->yposi.w.h + 48;
    pActwk->sprhsize = 18;
    pActwk->sprvsize = 48;
    udblk4_get_work(pActwk)->phase_high = 192;

    if (udblk4_get_work(pActwk)->child_index == 0) {

        udblk4_get_work(pActwk)->base_x = pActwk->xposi.w.h;
    }
    if (actwkchk(&pNewActwk) == 0) {

        pNewActwk->actno = 34;

        pNewActwk->xposi.w.h = pActwk->xposi.w.h;
        pNewActwk->yposi.w.h = pActwk->yposi.w.h;
        udblk4_get_work(pNewActwk)->base_x = udblk4_get_work(pActwk)->base_x;

        udblk4_get_work(pNewActwk)->parent_index = (Sint16)(pActwk - actwk);
        udblk4_get_work(pNewActwk)->child_y_offset = 64;
        pNewActwk->userflag.b.h = 1;
    }
    if (udblk4_get_work(pActwk)->child_index == 0) {
        if (actwkchk(&pNewActwk) == 0) {
            type4_coset(pActwk, pNewActwk, 1, -80);
        }
        if (actwkchk(&pNewActwk) == 0) {
            type4_coset(pActwk, pNewActwk, 2, 80);
        }
    }

    type4_move(pActwk);
}

static void type4_move(sprite_status *pActwk) {
    Sint16 stk;
    Sint16 d0, d1;
    udblk4_work *pWork = udblk4_get_work(pActwk);

    stk = pActwk->yposi.w.h;
    sinset(pWork->phase_high, &d0, &d1);
    d0 *= 3;
    d0 >>= 4;
    d0 += pWork->base_y;
    pActwk->yposi.w.h = d0;
    ++pWork->phase_high;
    d1 = stk;
    d0 -= d1;
    d0 <<= 8;
    pActwk->yspeed.w = d0;
    d1 = pWork->parent_index;
    if (d1) {
        actwk[d1].yspeed.w = d0;
    }
    ride_on_chk(pActwk, &actwk[0]);
}

static void udblk4_type5(sprite_status *pActwk) {
    void (*tbl[2])(sprite_status *) = {&type5_init, &type5_move};

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s00(pActwk, udblk4_get_work(pActwk)->base_x);
}

static void type5_coset(sprite_status *pActwk, sprite_status *pNewActwk,
                        Uint8 d0, Sint16 d1, Uint8 d2, Sint16 d3) {
    pNewActwk->actno = 35;
    pNewActwk->userflag.b.h = 5;
    udblk4_get_work(pNewActwk)->child_index = d0;
    udblk4_get_work(pNewActwk)->phase_high = d2;

    pNewActwk->xposi.w.h = pActwk->xposi.w.h + d1;

    pNewActwk->yposi.w.h = pActwk->yposi.w.h + d3;
    udblk4_get_work(pNewActwk)->base_x = udblk4_get_work(pActwk)->base_x;
}

static void type5_init(sprite_status *pActwk) {
    sprite_status *pNewActwk;

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 17514;
    pActwk->sprpri = 3;
    pActwk->patbase = udblk4pat5;
    udblk4_get_work(pActwk)->base_y = pActwk->yposi.w.h + 64;
    pActwk->sprhsize = 20;
    pActwk->sprvsize = 65;

    if (udblk4_get_work(pActwk)->child_index == 0) {

        udblk4_get_work(pActwk)->base_x = pActwk->xposi.w.h;
        udblk4_get_work(pActwk)->phase_high = 0;
        if (actwkchk(&pNewActwk) == 0) {
            type5_coset(pActwk, pNewActwk, 1, 32, 240, -32);
        }
        if (actwkchk(&pNewActwk) == 0) {
            type5_coset(pActwk, pNewActwk, 1, 64, 224, -64);
        }
        if (actwkchk(&pNewActwk) == 0) {
            type5_coset(pActwk, pNewActwk, 1, 96, 208, -96);
        }
    }

    type5_move(pActwk);
}

static void type5_move(sprite_status *pActwk) { sin_move(pActwk, 6, 0); }

static void udblk4_type6(sprite_status *pActwk) {
    if (pActwk->r_no0)
        type6_move(pActwk);
    else
        type6_init(pActwk);
}

static void type6_init(sprite_status *pActwk) {
    sprite_status *pNewActwk;
    Sint16 i, d1;
    kakudo_xofst tbl[7] = {{0, 0},   {32, -32}, {64, -64}, {96, -96},
                           {32, 32}, {64, 64},  {96, 96}};

    pActwk->r_no0 += 2;
    udblk4_get_work(pActwk)->base_x = pActwk->xposi.w.h;
    d1 = 0;
    for (i = 0; i <= 6; ++i) {
        if (actwkchk(&pNewActwk) == 0) {
            pNewActwk->actno = 35;
            pNewActwk->userflag.b.h = 9;
            pNewActwk->yposi.w.h = pActwk->yposi.w.h;
            udblk4_get_work(pNewActwk)->parent_index = (Sint16)(pActwk - actwk);
            udblk4_get_work(pNewActwk)->variant = 255;
            udblk4_get_work(pNewActwk)->phase_high = tbl[d1].kakudo;
            pNewActwk->xposi.w.h = pActwk->xposi.w.h + (Sint16)tbl[d1].xofst;
        }
        ++d1;
    }
    type6_move(pActwk);
}

static void type6_move(sprite_status *pActwk) {
    frameout_s00(pActwk, udblk4_get_work(pActwk)->base_x);
}

static void udblk4_type7(sprite_status *pActwk) {
    void (*tbl[2])(sprite_status *) = {&type7_init, &type7_move};

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s00(pActwk, udblk4_get_work(pActwk)->base_x);
}

static void type7_coset(sprite_status *pActwk, sprite_status *pNewActwk,
                        Uint8 d0, Sint16 d1, Uint8 d2) {
    pNewActwk->actno = 35;
    pNewActwk->userflag.b.h = 7;
    udblk4_get_work(pNewActwk)->child_index = d0;
    udblk4_get_work(pNewActwk)->phase_high = d2;
    pNewActwk->yposi.w.h = pActwk->yposi.w.h;

    pNewActwk->xposi.w.h = pActwk->xposi.w.h + d1;
    udblk4_get_work(pNewActwk)->base_x = udblk4_get_work(pActwk)->base_x;
}

static void type7_init(sprite_status *pActwk) {
    sprite_status *pNewActwk;

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 17514;
    pActwk->sprpri = 3;
    pActwk->patbase = udblk4pat7;
    udblk4_get_work(pActwk)->base_y = pActwk->yposi.w.h + 64;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 64;

    if (udblk4_get_work(pActwk)->child_index == 0) {

        udblk4_get_work(pActwk)->base_x = pActwk->xposi.w.h + 48;
        udblk4_get_work(pActwk)->phase_high = 192;
        if (actwkchk(&pNewActwk) == 0) {
            type7_coset(pActwk, pNewActwk, 1, 32, 224);
        }
        if (actwkchk(&pNewActwk) == 0) {
            type7_coset(pActwk, pNewActwk, 1, 64, 0);
        }
        if (actwkchk(&pNewActwk) == 0) {
            type7_coset(pActwk, pNewActwk, 1, 96, 32);
        }
    }

    type7_move(pActwk);
}

static void type7_move(sprite_status *pActwk) { sin_move(pActwk, 6, 0); }

static void udblk4_type8(sprite_status *pActwk) {
    Sint16 d0;
    void (*tbl[2])(sprite_status *) = {&type8_init, &type8_move};

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);

    d0 = udblk4_get_work(pActwk)->parent_index;
    if (d0) {
        if (actwk[d0].actno != 35) {
            frameout_s0(pActwk);
        }
    } else {
        frameout_s00(pActwk, udblk4_get_work(pActwk)->base_x);
    }
}

static void type8_init(sprite_status *pActwk) {
    sprite_status *pNewActwk;
    Sint16 i;
    Sint16 d0;
    Uint8 d1;

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 17514;
    pActwk->sprpri = 3;
    pActwk->patbase = udblk4pat8;
    udblk4_get_work(pActwk)->base_y = pActwk->yposi.w.h - 32;
    pActwk->sprhsize = 32;
    pActwk->sprvsize = 48;
    if (udblk4_get_work(pActwk)->variant == 0) {
        udblk4_get_work(pActwk)->base_x = pActwk->xposi.w.h;
    }

    if (udblk4_get_work(pActwk)->child_index == 0) {
        if (actwkchk(&pNewActwk) == 0) {
            pNewActwk->actno = 35;
            udblk4_get_work(pNewActwk)->child_index = 1;
            pNewActwk->userflag.b.h = 8;
            pNewActwk->xposi.w.h = pActwk->xposi.w.h;
            udblk4_get_work(pNewActwk)->base_x = udblk4_get_work(pActwk)->base_x;
            pNewActwk->yposi.w.h = pActwk->yposi.w.h + 160;
            udblk4_get_work(pNewActwk)->phase_high =
                udblk4_get_work(pActwk)->phase_high;
            udblk4_get_work(pNewActwk)->variant = udblk4_get_work(pActwk)->variant;
            if (udblk4_get_work(pActwk)->variant == 0) {
                d1 = 1;
                for (i = 0; i <= 1; ++i) {
                    if (actwkchk(&pNewActwk) == 0) {
                        pNewActwk->actno = 35;
                        pNewActwk->userflag.b.h = 8;
                        udblk4_get_work(pNewActwk)->variant = d1;
                        udblk4_get_work(pNewActwk)->phase_high = 128;
                        d0 = -64;
                        if (d1 != 1)
                            d0 *= -1;
                        pNewActwk->xposi.w.h = pActwk->xposi.w.h + d0;
                        pNewActwk->yposi.w.h = pActwk->yposi.w.h;
                        udblk4_get_work(pNewActwk)->base_x =
                            udblk4_get_work(pActwk)->base_x;
                    }
                    ++d1;
                }
            }
        }
    }

    type8_move(pActwk);
}

static void type8_move(sprite_status *pActwk) {
    short_union tmp;
    Sint16 stk;
    Sint16 d0, d1;
    udblk4_work *pWork = udblk4_get_work(pActwk);

    stk = pActwk->yposi.w.h;
    sinset(pWork->phase_high, &d0, &d1);
    d0 = d0 * 2 >> 4;

    tmp.b.h = pWork->phase_high;
    tmp.b.l = pWork->phase_low;
    tmp.w += 128;
    pWork->phase_high = tmp.b.h;
    pWork->phase_low = tmp.b.l;

    pActwk->yposi.w.h = pWork->base_y + d0;
    d0 -= stk;
    d0 <<= 8;
    pActwk->yspeed.w = d0;
    ride_on_chk(pActwk, &actwk[0]);
}

static void udblk4_type9(sprite_status *pActwk) {
    Sint16 d0;
    void (*tbl[2])(sprite_status *) = {&type9_init, &type9_move};

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);

    d0 = udblk4_get_work(pActwk)->parent_index;
    if (d0) {
        if (actwk[d0].actno == 0) {
            frameout_s0(pActwk);
        }
    } else {
        frameout_s00(pActwk, udblk4_get_work(pActwk)->base_x);
    }
}

static void type9_init(sprite_status *pActwk) {
    sprite_status *pNewActwk;
    Sint16 d1;
    Uint8 d0;

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 17514;
    pActwk->sprpri = 3;
    pActwk->patbase = udblk4pat9;
    udblk4_get_work(pActwk)->base_x = pActwk->xposi.w.h;
    udblk4_get_work(pActwk)->base_y = pActwk->yposi.w.h;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 32;

    if (udblk4_get_work(pActwk)->child_index == 0) {
        if (actwkchk(&pNewActwk) == 0) {
            pNewActwk->actno = 35;
            udblk4_get_work(pNewActwk)->child_index = 1;
            pNewActwk->userflag.b.h = 9;
            pNewActwk->xposi.w.h = pActwk->xposi.w.h;
            d1 = 128;
            d0 = udblk4_get_work(pActwk)->phase_high;
            if (udblk4_get_work(pActwk)->variant & 128) {
                d1 = 160;
                d0 -= 128;
            }
            udblk4_get_work(pNewActwk)->phase_high = d0;
            pNewActwk->yposi.w.h = pActwk->yposi.w.h + d1;
        }
    }
}

static void type9_move(sprite_status *pActwk) { sin_move(pActwk, 5, 0); }

static void udblk4_typeA(sprite_status *pActwk) {
    Sint16 d0;
    void (*tbl[2])(sprite_status *) = {&typeA_init, &typeA_move};

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);

    d0 = udblk4_get_work(pActwk)->parent_index;
    if (d0) {
        if (actwk[d0].actno == 0) {
            frameout_s0(pActwk);
        }
    } else {
        frameout_s00(pActwk, udblk4_get_work(pActwk)->base_x);
    }
}

static void typeA_init(sprite_status *pActwk) {
    sprite_status *pNewActwk;
    Sint16 i;
    Uint8 d1;
    kakudo_xofst tbl[7] = {{0, 0},    {224, -32}, {192, -64}, {160, -96},
                           {224, 32}, {192, 64},  {160, 96}};

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 17514;
    pActwk->sprpri = 3;
    pActwk->patbase = udblk4patA;
    udblk4_get_work(pActwk)->base_y = pActwk->yposi.w.h;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 16;

    if (udblk4_get_work(pActwk)->child_index == 0) {
        udblk4_get_work(pActwk)->base_x = pActwk->xposi.w.h;
        d1 = 1;
        for (i = 0; i <= 5; ++i) {
            if (actwkchk(&pNewActwk) == 0) {
                pNewActwk->actno = 35;
                pNewActwk->userflag.b.h = 10;
                pNewActwk->yposi.w.h = pActwk->yposi.w.h;
                udblk4_get_work(pNewActwk)->base_x =
                    udblk4_get_work(pActwk)->base_x;
                udblk4_get_work(pNewActwk)->child_index = d1;
                udblk4_get_work(pNewActwk)->phase_high = tbl[d1].kakudo;

                pNewActwk->xposi.w.h =
                    pActwk->xposi.w.h + (Sint16)tbl[d1].xofst;
            }
            ++d1;
        }
    }
    typeA_move(pActwk);
}

static void typeA_move(sprite_status *pActwk) { sin_move(pActwk, 5, 4); }

static void udblk4_typeB(sprite_status *pActwk) {
    Sint16 d0;
    void (*tbl[2])(sprite_status *) = {&typeB_init, &typeB_move};

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);

    d0 = udblk4_get_work(pActwk)->parent_index;
    if (d0) {
        if (actwk[d0].actno == 0) {
            frameout_s0(pActwk);
        }
    } else {
        frameout_s00(pActwk, udblk4_get_work(pActwk)->base_x);
    }
}

static void typeB_init(sprite_status *pActwk) {
    sprite_status *pNewActwk;
    xofs_yofs_yspd *a1;
    Sint16 i;
    Uint8 d1;
    xofs_yofs_yspd tbl[6] = {{0, 0, 128},      {0, -128, 128}, {-80, 32, -256},
                             {-80, -96, -256}, {80, 32, -256}, {80, -96, -256}};

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 17514;
    pActwk->sprpri = 3;
    pActwk->patbase = udblk4pat9;
    pActwk->sprhsize = 16;
    pActwk->sprvsize = 32;
    pActwk->patno = 0;

    if (udblk4_get_work(pActwk)->child_index == 0) {
        udblk4_get_work(pActwk)->base_x = pActwk->xposi.w.h;
        udblk4_get_work(pActwk)->base_y = pActwk->yposi.w.h;
        d1 = 1;
        for (i = 0; i <= 4; ++i) {
            if (actwkchk(&pNewActwk) == 0) {
                pNewActwk->actno = 35;
                pNewActwk->userflag.b.h = 11;
                udblk4_get_work(pNewActwk)->child_index = d1;
                udblk4_get_work(pNewActwk)->base_x =
                    udblk4_get_work(pActwk)->base_x;
                udblk4_get_work(pNewActwk)->base_y =
                    udblk4_get_work(pActwk)->base_y;
            }
            ++d1;
        }
    }
    a1 = &tbl[udblk4_get_work(pActwk)->child_index];
    pActwk->xposi.w.h = udblk4_get_work(pActwk)->base_x + (Sint16)a1->xofs;
    pActwk->yposi.w.h = udblk4_get_work(pActwk)->base_y + (Sint16)a1->yofs;
    pActwk->yspeed.w += a1->yspd;
}

static void typeB_move(sprite_status *pActwk) {
    Sint16 d0;
    udblk4_work *pWork = udblk4_get_work(pActwk);

    pActwk->yposi.l = pActwk->yposi.l + (pActwk->yspeed.w << 8);

    if (pActwk->yspeed.w < 0) {
        d0 = pWork->base_y;
        if (d0 < pActwk->yposi.w.h)
            return;
        d0 -= pActwk->yposi.w.h;
        if (d0 < 128)
            return;
        pActwk->yposi.w.h = pWork->base_y + 128;
    } else {
        d0 = pActwk->xposi.w.h;
        if (d0 < pWork->base_y)
            return;
        d0 -= pWork->base_y;
        if (d0 < 128)
            return;
        pActwk->yposi.w.h = pWork->base_y - 128;
    }
}
