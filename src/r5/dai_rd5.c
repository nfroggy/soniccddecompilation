#include "../equ.h"
#include "dai_rd5.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../etc.h"
#include "../ridechk.h"

#if defined(R53C) || defined(R53D)
#define SPRITE_DAIRD5_BASE 413
#else
#define SPRITE_DAIRD5_BASE 468
#endif

static void dair5_init(sprite_status *pActwk);
static void dair5_move(sprite_status *pActwk);
static void type01(sprite_status *pActwk);
static void type02(sprite_status *pActwk);
static void type03(sprite_status *pActwk);
static void type04(sprite_status *pActwk);
static void type05(sprite_status *pActwk);
static void type06(sprite_status *pActwk);
static void type06_00(sprite_status *pActwk);
static void type06_01(sprite_status *pActwk);
static void type06_02(sprite_status *pActwk);
static void dodai_sub(sprite_status *pActwk, Uint8 d4);
static void dai5sub(sprite_status *pActwk);
static void banesetsub(sprite_status *pActwk, char d2);
static void belttask(sprite_status *pActwk);

static sprite_pattern pat00 = {1, {{-16, -16, 0, SPRITE_DAIRD5_BASE}}};
static sprite_pattern pat01 = {2,
                               {{-16, -16, 0, SPRITE_DAIRD5_BASE + 1},
                                {-16, -16, 0, SPRITE_DAIRD5_BASE + 2}}};
static sprite_pattern pat02 = {1, {{-32, -16, 0, SPRITE_DAIRD5_BASE + 3}}};
static sprite_pattern pat03 = {2,
                               {{-32, -16, 0, SPRITE_DAIRD5_BASE + 4},
                                {-32, -16, 0, SPRITE_DAIRD5_BASE + 5}}};
static sprite_pattern pat04 = {1, {{-48, -16, 0, SPRITE_DAIRD5_BASE + 6}}};
static sprite_pattern pat05 = {2,
                               {{-48, -16, 0, SPRITE_DAIRD5_BASE + 7},
                                {-48, -16, 0, SPRITE_DAIRD5_BASE + 8}}};
sprite_pattern *dair5pat[6] = {&pat00, &pat01, &pat02, &pat03, &pat04, &pat05};

void dair5(sprite_status *pActwk) {
    void (*tbl[2])(sprite_status *) = {&dair5_init, &dair5_move};

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s00(pActwk, ((Sint16 *)pActwk)[29]);
}

static void dair5_init(sprite_status *pActwk) {
    Uint8 *a2;
    Sint16 d0;
    Uint8 bd0;
    Uint8 bd1;
    Uint8 ini_tbl[50] = {0,  16, 1,  16, 0,  16, 0,  16, 1,  16, 0,  16, 0,
                         16, 2,  32, 2,  32, 3,  32, 2,  32, 3,  32, 2,  32,
                         3,  32, 2,  32, 2,  32, 2,  32, 4,  48, 5,  48, 4,
                         48, 4,  48, 5,  48, 4,  48, 4,  48, 4,  48};

    Uint8 belt_flag[50] = {
        0,  1,   0,   0,   1,   0,   0, 0, 0, 1,   0,   1,   0,   1,   0, 0, 0,
        0,  1,   0,   0,   1,   0,   0, 0, 0, 192, 192, 192, 192, 0,   0, 0, 64,
        64, 192, 192, 192, 192, 192, 0, 0, 0, 0,   192, 192, 192, 192, 0, 0};

    Uint8 bane_flag[25] = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                           0, 2, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0};

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;

    d0 = 17156;
    if (stageno.b.l == 2) {
        d0 = 17272;
    }
    pActwk->sproffset = d0;

    pActwk->patbase = dair5pat;
    ((Sint16 *)pActwk)[29] = pActwk->xposi.w.h;
    ((Sint16 *)pActwk)[27] = pActwk->yposi.w.h;
    pActwk->sprvsize = 16;
    bd0 = 0;
    bd1 = 0;

    bd0 = pActwk->userflag.b.h;
    bd0 &= 31;
    bd1 = bd0;
    bd1 = bd1 + bd1;
    a2 = &ini_tbl[bd1];
    pActwk->patno = *a2++;
    pActwk->sprhsize = *a2++;

    a2 = &belt_flag[bd0];
    pActwk->actfree[21] = *a2;

    a2 = &bane_flag[bd0];
    bd1 = *a2;
    switch (bd1) {

    case 0:
        break;

    case 1:
        banesetsub(pActwk, 0);
        break;

    case 2:
        banesetsub(pActwk, -16);
        banesetsub(pActwk, 16);
        break;

    case 3:
        banesetsub(pActwk, -32);
        banesetsub(pActwk, 0);
        banesetsub(pActwk, 32);
        break;
    }

    dair5_move(pActwk);
}

static void dair5_move(sprite_status *pActwk) {
    void (*tbl[25])(sprite_status *) = {
        &type01, &type02, &type02, &type03, &type03, &type06, &type05,
        &type01, &type04, &type04, &type03, &type03, &type02, &type02,
        &type02, &type06, &type05, &type01, &type01, &type03, &type02,
        &type02, &type02, &type06, &type05,
    };

    tbl[pActwk->userflag.b.h & 31](pActwk);
    dai5sub(pActwk);
    belttask(pActwk);
    ridechk(pActwk, &actwk[0]);
}

static void type01(sprite_status *pActwk) { pActwk->actfree[21] |= 128; }

static void type02(sprite_status *pActwk) {
    Sint16 d0;

    pActwk->actfree[21] |= 128;
    ((Sint16 *)pActwk)[26] = 4;
    dodai_sub(pActwk, 48);

    d0 = ((Sint16 *)pActwk)[25];
    if (pActwk->userflag.b.h & 128) {
        d0 *= -1;
    }
    pActwk->xspeed.w = d0;
    pActwk->xposi.l += d0 << 8;
}

static void type03(sprite_status *pActwk) {
    Sint32 ld0;
    Sint16 d0;

    ((Sint16 *)pActwk)[26] = 4;
    dodai_sub(pActwk, 48);
    d0 = ((Sint16 *)pActwk)[25];
    if (pActwk->userflag.b.h & 128) {
        d0 *= -1;
    }
    ld0 = d0;
    ld0 <<= 8;
    pActwk->yposi.l += ld0;
}

static void type04(sprite_status *pActwk) {
    Sint16 d0;

    pActwk->actfree[21] |= 128;
    ((Sint16 *)pActwk)[26] = 4;
    dodai_sub(pActwk, 32);

    d0 = ((Sint16 *)pActwk)[25];
    if (pActwk->userflag.b.h & 128) {
        d0 *= -1;
    }
    pActwk->xspeed.w = d0;
    pActwk->xposi.l += d0 << 8;
}

static void type05(sprite_status *pActwk) {
    Sint32 ld0;
    Sint16 d0;

    if (pActwk->actfree[19] == 0) {
        pActwk->actfree[21] |= 128;
        if (pActwk->cddat & 8) {
            pActwk->actfree[17] = 30;
            pActwk->actfree[19] = 1;
        }
    } else {
        if (pActwk->actfree[17]) {
            --pActwk->actfree[17];
            if (pActwk->actfree[17])
                return;
            pActwk->actfree[21] &= 127;
        }
        pActwk->sprvsize = 16;
        if (pActwk->yspeed.w >= 512) {
            pActwk->sprvsize = 18;
            if (pActwk->yspeed.w >= 768) {
                pActwk->sprvsize = 20;
            }
        }
        d0 = pActwk->yspeed.w;
        if (d0 < 1024) {
            pActwk->yspeed.w += 32;
        }
        ld0 = pActwk->yspeed.w;
        ld0 <<= 8;
        pActwk->yposi.l += ld0;
        d0 = scra_v_posit.w.h;
        d0 += 224;
        if (d0 < pActwk->yposi.w.h) {
            ride_on_clr(pActwk, &actwk[0]);
            frameout(pActwk);
        }
    }
}

static void type06(sprite_status *pActwk) {
    void (*tbl[3])(sprite_status *) = {&type06_00, &type06_01, &type06_02};

    tbl[pActwk->actfree[19] / 2](pActwk);
}

static void type06_00(sprite_status *pActwk) {
    pActwk->actfree[21] |= 128;
    if (pActwk->cddat & 8) {
        pActwk->actfree[19] += 2;
        pActwk->actfree[17] = 30;
        type06_01(pActwk);
    }
}

static void type06_01(sprite_status *pActwk) {
    Sint32 stk;
    Sint16 d0, d1;

    if (pActwk->actfree[17]) {
        --pActwk->actfree[17];
        if (pActwk->actfree[17])
            return;
        pActwk->actfree[21] &= 127;
        pActwk->sprvsize = 14;
    }
    d0 = pActwk->yspeed.w;
    d0 -= 8;
    if (d0 < -768) {
        d0 = -768;
    }
    pActwk->yspeed.w = d0;
    speedset2(pActwk);
    stk = pActwk->yposi.l;
    pActwk->yposi.w.h -= 48;
    d1 = emycol_u(pActwk);
    pActwk->yposi.l = stk;
    if (d1 < 0) {
        pActwk->sprvsize = 16;
        pActwk->yspeed.w = 0;
        ((Sint16 *)pActwk)[27] = pActwk->yposi.w.h;
        pActwk->actfree[21] |= 128;
        pActwk->actfree[19] += 2;
    }
}

static void type06_02(sprite_status *pActwk) {}

static void dodai_sub(sprite_status *pActwk, Uint8 d4) {
    Sint16 d0, d2;

    d2 = ((Sint16 *)pActwk)[26];
    if (pActwk->actfree[18] == 0) {
        d0 = ((Sint16 *)pActwk)[25];
        d0 += d2;
        ((Sint16 *)pActwk)[25] = d0;
        ((Sint16 *)pActwk)[24] += d0;
        if (d4 <= pActwk->actfree[3]) {
            pActwk->actfree[18] = 255;
        }
    } else {
        d0 = ((Sint16 *)pActwk)[25];
        d0 -= d2;
        ((Sint16 *)pActwk)[25] = d0;
        ((Sint16 *)pActwk)[24] += d0;
        if (d4 > pActwk->actfree[3]) {
            pActwk->actfree[18] = 0;
        }
    }
}

static void dai5sub(sprite_status *pActwk) {
    int_union ld0;
    Sint16 d0, d1;

    if (pActwk->actfree[21] & 128) {
        if (!(pActwk->cddat & 8)) {
            if (pActwk->actfree[20] == 0)
                return;

            ((char *)pActwk)[66] -= 8;
        } else {
            if (pActwk->actfree[20] == 64)
                return;

            ((char *)pActwk)[66] += 8;
        }
        sinset(pActwk->actfree[20], &d0, &d1);
        d1 = 1024;
        ld0.l = d0 * d1;
        d0 = ld0.w.h;
        d0 += ((Sint16 *)pActwk)[27];
        pActwk->yposi.w.h = d0;
    }
}

static void banesetsub(sprite_status *pActwk, char d2) {
    sprite_status *pNewActwk;

    if (actwkchk2(pActwk, &pNewActwk) == 0) {
        pNewActwk->actno = 10;
        pNewActwk->yposi.w.h = pActwk->yposi.w.h;
        pNewActwk->xposi.w.h = pActwk->xposi.w.h;
        ((char *)pNewActwk)[60] = d2;
        ((char *)pNewActwk)[61] = -24;
        ((Sint16 *)pNewActwk)[28] = (Uint16)(pActwk - actwk);
        pNewActwk->userflag.b.h = 2;
    }
}

static void belttask(sprite_status *pActwk) {
    sprite_status *pPlayerwk;
    Sint32 ld0;
    Sint16 d0;
    Uint8 bd1;

    if (!(pActwk->actfree[21] & 1))
        return;
    if (!(pActwk->cddat & 8))
        return;

    pPlayerwk = &actwk[0];

    if (pPlayerwk->actno == 0)
        return;

    if (!(pPlayerwk->cddat & 8))
        return;
    d0 = (Uint16)pPlayerwk->actfree[19];

    if (d0 != (Sint16)(Uint16)(pActwk - actwk))
        return;

    bd1 = time_flag;
    ld0 = 32768;
    if (bd1) {
        ld0 = 65536;
        --bd1;
        if (bd1) {
            ld0 = 0x20000;
        }
    }
    if (colrevflag) {
        ld0 = -ld0;
    }
    pPlayerwk->xposi.l += ld0;
}
