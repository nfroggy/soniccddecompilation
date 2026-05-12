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

    d0 = ((Sint16 *)pActwk)[29];
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
    ((Sint16 *)pActwk)[27] = pActwk->xposi.w.h;
    pActwk->sprhsize = 48;
    pActwk->sprvsize = 16;
    pActwk->patno = 1;

    if (pActwk->actfree[18]) {
        type1_move(pActwk);
        return;
    }

    ((Sint16 *)pActwk)[29] = pActwk->xposi.w.h;
    if (pActwk->actfree[19] == 0) {
        d1 = 1;
        for (i = 0; i <= 4; ++i) {
            if (actwkchk(&pNewActwk) == 0) {
                pNewActwk->actno = 36;
                pNewActwk->userflag.b.h = 1;
                pNewActwk->actfree[19] = d1;
                pNewActwk->xposi.w.h = pActwk->xposi.w.h;

                pNewActwk->actfree[16] = (char)(d1 * 32) + 16;
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
        pNewActwk->actfree[18] = 1;

        ((Sint16 *)pNewActwk)[29] = ((Sint16 *)pActwk)[29];
        pNewActwk->yposi.w.h = pActwk->yposi.w.h;
        pNewActwk->xposi.w.h = pActwk->xposi.w.h + 224;
        pNewActwk->actfree[16] = pActwk->actfree[16];
    }
    type1_move(pActwk);
}

static void type1_move(sprite_status *pActwk) {
    Sint16 d0, d1, stk;

    stk = pActwk->xposi.w.h;
    sinset(pActwk->actfree[16], &d0, &d1);
    d0 >>= 3;
    ++pActwk->actfree[16];
    d0 += ((Sint16 *)pActwk)[27];
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

    if (pActwk->actfree[18]) {
        type2_move(pActwk);
        return;
    }

    ((Sint16 *)pActwk)[29] = pActwk->xposi.w.h;

    ((Sint16 *)pActwk)[27] = pActwk->xposi.w.h - 32;

    if (actwkchk(&pNewActwk) == 0) {
        pNewActwk->actno = 36;
        pNewActwk->userflag.b.h = 2;
        pNewActwk->actfree[18] = 1;

        ((Sint16 *)pNewActwk)[29] = ((Sint16 *)pActwk)[29];
        pNewActwk->yposi.w.h = pActwk->yposi.w.h;
        pNewActwk->xposi.w.h = pActwk->xposi.w.h + 64;
        ((Sint16 *)pNewActwk)[27] = pNewActwk->xposi.w.h + 32;
        pNewActwk->actfree[16] = pActwk->actfree[16] - 128;
    }
    type2_move(pActwk);
}

static void type2_move(sprite_status *pActwk) {
    Sint16 d0, d1, stk;

    stk = pActwk->xposi.w.h;
    sinset(pActwk->actfree[16], &d0, &d1);
    d0 >>= 3;
    ++pActwk->actfree[16];
    d0 += ((Sint16 *)pActwk)[27];
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

    if (pActwk->actfree[18] == 0) {
        ((Sint16 *)pActwk)[29] = pActwk->xposi.w.h;
        d1 = 1;
        for (i = 0; i <= 4; ++i) {
            if (actwkchk(&pNewActwk) == 0) {
                pNewActwk->actno = 36;
                pNewActwk->userflag.b.h = 3;
                pNewActwk->actfree[18] = d1;
                ((Sint16 *)pNewActwk)[29] = ((Sint16 *)pActwk)[29];
                pNewActwk->xposi.w.h = pActwk->xposi.w.h;
                pNewActwk->yposi.w.h = pActwk->yposi.w.h;
            }
            ++d1;
        }
    }
    d0 = pActwk->actfree[18] * 6;
    a2 = &tbl[d0 / 2];
    pActwk->xposi.w.h += *a2++;
    pActwk->yposi.w.h += *a2++;
    pActwk->xspeed.w = *a2++;
}

static void type3_move(sprite_status *pActwk) {
    sprite_status *pPlayerwk;
    Sint32 ld0;
    Sint16 d0;

    ld0 = pActwk->xspeed.w;
    ld0 <<= 8;
    pActwk->xposi.l += ld0;

    if (pActwk->xspeed.w < 0) {
        d0 = ((Sint16 *)pActwk)[29];
        if ((Uint16)d0 >= (Uint16)pActwk->xposi.w.h) {
            d0 -= pActwk->xposi.w.h;
            if ((Uint16)d0 >= 224) {
                d0 = ((Sint16 *)pActwk)[29];
                d0 += 224;
                pActwk->xposi.w.h = d0;
            }
        }
    } else {
        d0 = pActwk->xposi.w.h;
        if ((Uint16)d0 >= (Uint16)((Sint16 *)pActwk)[29]) {
            d0 -= ((Sint16 *)pActwk)[29];
            if ((Uint16)d0 >= 224) {
                d0 = ((Sint16 *)pActwk)[29];
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
    if (pActwk->actfree[19]) {
        pActwk->patbase = lrblk4pat8;
        pActwk->sprvsize = 16;
    }

    if (pActwk->actfree[18] == 0) {
        ((Sint16 *)pActwk)[29] = pActwk->xposi.w.h;
        if (actwkchk(&pNewActwk) == 0) {
            pNewActwk->actno = 36;
            pNewActwk->userflag.b.h = 4;
            pNewActwk->actfree[18] = 1;
            pNewActwk->actfree[19] = pActwk->actfree[19];
            ((Sint16 *)pNewActwk)[29] = ((Sint16 *)pActwk)[29];

            pNewActwk->xposi.w.h = ((Sint16 *)pActwk)[29] + 128;
            pNewActwk->yposi.w.h = pActwk->yposi.w.h;

            pActwk->xposi.w.h = ((Sint16 *)pActwk)[29] - 128;
        }
    }
    pActwk->xspeed.w = -512;
}

static void type4_move(sprite_status *pActwk) {
    Sint16 d0;

    pActwk->xposi.l += pActwk->xspeed.w << 8;
    hitchk(pActwk, &actwk[0]);
    d0 = ((Sint16 *)pActwk)[29];
    if (d0 >= pActwk->xposi.w.h) {
        d0 -= pActwk->xposi.w.h;
        if (d0 == 256) {
            pActwk->xposi.w.h = ((Sint16 *)pActwk)[29] + 256;
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
    ((Sint16 *)pActwk)[29] = pActwk->xposi.w.h;
    pActwk->sprhsize = 32;
    pActwk->sprvsize = 32;

    if (pActwk->actfree[18] == 0) {
        d1 = 1;
        for (i = 0; i <= 3; ++i) {
            if (actwkchk(&pNewActwk) == 0) {
                pNewActwk->actno = 36;
                pNewActwk->userflag.b.h = 5;
                pNewActwk->actfree[18] = d1;
                pNewActwk->xposi.w.h = pActwk->xposi.w.h;
                d0 = d1;
                d0 *= 64;
                pNewActwk->yposi.w.h = pActwk->yposi.w.h + (Sint16)d0;
            }
            ++d1;
        }
    }
    a1 = &tbl[pActwk->actfree[18] * 2];
    pActwk->xposi.w.h += (Uint16)*a1++;
    pActwk->actfree[17] = *a1++;
}

static void type5_move(sprite_status *pActwk) {
    move_data *a1;
    Sint16 d0;

    move_data type5_mvtbl[6] = {{128, 0, 128},  {128, 0, 128},  {128, 0, 128},
                                {128, 0, -128}, {128, 0, -128}, {128, 0, -128}};

label1:
    if (pActwk->actfree[16] == 0) {
        a1 = &type5_mvtbl[pActwk->actfree[17]];
        pActwk->actfree[16] = a1->cnt;
        ((Sint16 *)pActwk)[26] = a1->speed;
    } else {
        d0 = ((Sint16 *)pActwk)[26];
        pActwk->xspeed.w = d0;
        pActwk->xposi.l += d0 << 8;
        --pActwk->actfree[16];
        if (pActwk->actfree[16] == 0) {
            ++pActwk->actfree[17];
            if (pActwk->actfree[17] != 6)
                goto label1;

            pActwk->actfree[17] = 0;
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

    if (pActwk->actfree[18] == 0) {
        ((Sint16 *)pActwk)[29] = pActwk->xposi.w.h;
        d1 = 1;
        for (i = 0; i <= 2; ++i) {
            if (actwkchk(&pNewActwk) == 0) {
                pNewActwk->actno = 36;
                pNewActwk->userflag.b.h = 6;
                pNewActwk->actfree[18] = d1;
                ((Sint16 *)pNewActwk)[29] = ((Sint16 *)pActwk)[29];
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
    bd0 = pActwk->actfree[18];
    pActwk->patno = bd0;
    ++bd0;
    pActwk->actfree[17] = bd0;
    bd0 = pActwk->actfree[18];
    d1 = 0;
    for (i = 0; i <= (Sint16)(Uint16)bd0; ++i) {
        d1 += 16;
    }
    pActwk->sprhsize = d1;
    d0 = pActwk->xposi.w.h;
    d0 += (Uint16)d1;
    ((Sint16 *)pActwk)[27] = d0;
    pActwk->actfree[16] = 128;

    type6_move(pActwk);
}

static void type6_move(sprite_status *pActwk) {
    Sint16 d0, d1, d3, stk;
    Uint8 d2;

    stk = pActwk->xposi.w.h;
    sinset(pActwk->actfree[16], &d0, &d1);

    d3 = 0;
    d2 = pActwk->actfree[17];
    if (d2) {
        do {
            d3 += d0;
            --d2;
        } while (d2 != 0);
    }

    d3 >>= 4;
    d0 = d3;
    d0 += ((Sint16 *)pActwk)[27];
    pActwk->xposi.w.h = d0;

    d1 = stk;
    d0 -= d1;
    d0 <<= 8;
    pActwk->xspeed.w = d0;
    ++pActwk->actfree[16];
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

    if (pActwk->actfree[18] == 0) {
        ((Sint16 *)pActwk)[29] = pActwk->xposi.w.h;
        if (actwkchk(&pNewActwk) == 0) {
            pNewActwk->actno = 36;
            pNewActwk->userflag.b.h = 4;
            pNewActwk->actfree[19] = 8;
            pNewActwk->xposi.w.h = pActwk->xposi.w.h;
            pNewActwk->yposi.w.h = pActwk->yposi.w.h - 96;
        }
        if (actwkchk(&pNewActwk) == 0) {
            pNewActwk->actno = 36;
            pNewActwk->userflag.b.h = 8;
            pNewActwk->actfree[18] = 1;
            ((Sint16 *)pNewActwk)[29] = ((Sint16 *)pActwk)[29];
            d0 = ((Sint16 *)pActwk)[29];
            d0 -= 128;
            pNewActwk->xposi.w.h = d0;
            pNewActwk->yposi.w.h = pActwk->yposi.w.h;
            d0 = ((Sint16 *)pActwk)[29];
            d0 += 128;
            pActwk->xposi.w.h = d0;
        }
    }
    pActwk->xspeed.w = 512;
}

static void type8_move(sprite_status *pActwk) {
    Sint16 d0;

    pActwk->xposi.l += pActwk->xspeed.w << 8;
    hitchk(pActwk, &actwk[0]);
    d0 = pActwk->xposi.w.h;
    if (d0 >= ((Sint16 *)pActwk)[29]) {
        d0 -= ((Sint16 *)pActwk)[29];
        if (d0 == 256) {
            pActwk->xposi.w.h = ((Sint16 *)pActwk)[29] - 256;
        }
    }
}
