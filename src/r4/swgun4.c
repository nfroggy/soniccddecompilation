#include "../equ.h"
#include "swgun4.h"
#include "../action.h"
#include "../actset.h"
#include "../dircol.h"
#include "../loader2.h"
#include "../ridechk.h"
#include "../ring.h"
#include "../score.h"
#include "playsub4.h"

static void swgun4_init(sprite_status *pActwk);
static void swgun4_move(sprite_status *pActwk);
static void swgun4_move1(sprite_status *pActwk);
static void switch_move(sprite_status *pActwk);
static void hariset(sprite_status *pActwk);
static void awaset(sprite_status *pActwk);
static void ring4set(sprite_status *pActwk);
static void bp1000(sprite_status *pActwk);
static void ring4_init(sprite_status *pActwk);
static void ring4_move(sprite_status *pActwk);
static void ring4_get(sprite_status *pActwk);
static void ring4_die(sprite_status *pActwk);
static void ring4_erase(sprite_status *pActwk);
static void tensu4_init(sprite_status *pActwk);
static void tensu4_move(sprite_status *pActwk);

static sprite_pattern pat0 = {1, {{-16, -8, 0, 481}}};
static sprite_pattern pat1 = {1, {{-16, 0, 0, 482}}};
static sprite_pattern pat2 = {1, {{-16, 0, 0, 483}}};
sprite_pattern *swgun4pat[3] = {&pat0, &pat1, &pat2};

void swgun4(sprite_status *pActwk) {
    void (*tbl[3])(sprite_status *) = {&swgun4_init, &swgun4_move,
                                       &swgun4_move1};

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s00(pActwk, ((Sint16 *)pActwk)[29]);
}

static void swgun4_init(sprite_status *pActwk) {
    sprite_status *pNewActwk;
    Sint16 *a2;
    Sint16 i;
    Uint8 d1;
    Sint16 tbl[12] = {0, 0, -64, 64, -128, 128, 64, -64, 96, -128, 128, -128};

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 1290;
    pActwk->sprpri = 3;
    pActwk->patbase = swgun4pat;
    pActwk->sprhsize = 14;
    pActwk->sprvsize = 8;

    if (pActwk->actfree[10] == 0) {
        ((Sint16 *)pActwk)[29] = pActwk->xposi.w.h;
        ((Sint16 *)pActwk)[27] = pActwk->yposi.w.h;
        ((Sint16 *)pActwk)[31] = (Uint16)(pActwk - actwk);

        d1 = 1;
        for (i = 0; i <= 4; ++i) {
            if (actwkchk(&pNewActwk) == 0) {
                pNewActwk->actno = 63;

                ((Sint16 *)pNewActwk)[29] = ((Sint16 *)pActwk)[29];

                ((Sint16 *)pNewActwk)[31] = (Uint16)(pActwk - actwk);
                pNewActwk->actfree[10] = d1;
                a2 = &tbl[d1 * 2];
                pNewActwk->xposi.w.h = pActwk->xposi.w.h + *a2++;
                pNewActwk->yposi.w.h = pActwk->yposi.w.h + *a2++;
                ((Sint16 *)pNewActwk)[27] = pNewActwk->yposi.w.h;
            }
            ++d1;
        }
    }
}

static void swgun4_move(sprite_status *pActwk) {
    sprite_status *a1;
    Sint16 d0;

    switch_move(pActwk);

    a1 = &actwk[((Sint16 *)pActwk)[31]];
    d0 = ((Sint16 *)a1)[32];
    if (d0 == 0)
        return;

    if (pActwk->actfree[4] == 0) {
        soundset(191);
        pActwk->actfree[4] = 255;
    }

    if ((Uint16)((Uint16)pActwk->actfree[5] + 16) < 256) {
        pActwk->actfree[5] += 16;
        return;
    }
    pActwk->actfree[5] += 16;

    pActwk->r_no0 += 2;
    ride_on_clr(pActwk, &actwk[0]);
    pActwk->actfree[15] = pActwk->actfree[14] = 0;
    pActwk->patno = 2;
}

static void swgun4_move1(sprite_status *pActwk) {
    sprite_status *a1;
    void (*tbl[4])(sprite_status *) = {&hariset, &bp1000, &awaset, &ring4set};

    a1 = &actwk[((Sint16 *)pActwk)[31]];
    tbl[a1->actfree[11]](pActwk);
}

static void switch_move(sprite_status *pActwk) {
    sprite_status *pPlayerwk;
    sprite_status *pActwk_m;
    Uint16 d0, d1;
    Sint16 r;
    Uint8 tbl[42] = {2, 1, 0, 3, 3, 2, 1, 0, 3, 3, 2, 2, 0, 3,
                     3, 2, 2, 1, 2, 2, 1, 0, 3, 3, 3, 2, 2, 1,
                     0, 3, 3, 3, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0};

    pActwk->actfree[6] = pActwk->actfree[7];

    pPlayerwk = &actwk[0];
    r = hitchk_u(pActwk, pPlayerwk);
    if (r) {
        pActwk->actfree[7] = 255;
    } else {
        pActwk->actfree[7] = 0;
        goto label1;
    }
    if (pPlayerwk->yspeed.w < 0)
        goto label1;
    if (pActwk->actfree[4])
        goto label1;
    pActwk_m = &actwk[((Sint16 *)pActwk)[31]];
    ((Sint16 *)pActwk_m)[32] = (Uint16)(pActwk - actwk);

    d0 = pActwk->actfree[10];
    d0 *= 6;
    d1 = pltime.b.b3;
    d1 &= 255;
    d1 /= 10;
    d1 &= 7;
    d0 += d1;
    pActwk->actfree[11] = tbl[d0];

    pActwk_m->actfree[11] = pActwk->actfree[11];

label1:
    if (pActwk->actfree[6] == 0 && pActwk->actfree[7] == 255) {
        pPlayerwk->yposi.w.h += 8;
        pActwk->yposi.w.h += 4;
        pActwk->patno = 1;
        pActwk->sprvsize -= 4;
    }

    if (pActwk->actfree[6] == 255 && pActwk->actfree[7] == 0) {
        pPlayerwk->yposi.w.h -= 8;
        pActwk->yposi.w.h -= 4;
        pActwk->patno = 0;
        pActwk->sprvsize += 4;
    }
}

static void hariset(sprite_status *pActwk) {
    sprite_status *pMaster;
    sprite_status *pSlave;
    sprite_status *pNewActwk;

    if (pActwk->actfree[14]) {
        frameout(pActwk);
        return;
    }
    pMaster = &actwk[((Sint16 *)pActwk)[31]];
    pSlave = &actwk[((Sint16 *)pMaster)[32]];
    if (pActwk != pSlave) {
        if (pActwk->actfree[10] != 4) {
            if (actwkchk(&pNewActwk) == 0) {
                pNewActwk->actno = 34;
                pNewActwk->xposi.w.h = pActwk->xposi.w.h;

                ((Sint16 *)pNewActwk)[29] = ((Sint16 *)pActwk)[29];
                pNewActwk->yposi.w.h = ((Sint16 *)pActwk)[27];
            }
        }
    }
    ++pActwk->actfree[14];
}

static void awaset(sprite_status *pActwk) {
    sprite_status *pMaster;
    sprite_status *pSlave;
    sprite_status *pNewActwk;

    pMaster = &actwk[((Sint16 *)pActwk)[31]];
    pSlave = &actwk[((Sint16 *)pMaster)[32]];
    if (pActwk == pSlave) {
        if ((Uint16)((Uint16)pActwk->actfree[14] + 1) > 255) {
            frameout(pActwk);
        }
        ++pActwk->actfree[14];
        return;
    }
    if ((Uint16)((Uint16)pActwk->actfree[15] + 4) <= 255) {
        pActwk->actfree[15] += 4;
        return;
    }
    pActwk->actfree[15] += 4;
    if (actwkchk(&pNewActwk) == 0) {
        pNewActwk->actno = 32;
        pNewActwk->xposi.w.h = pActwk->xposi.w.h;
        pNewActwk->yposi.w.h = ((Sint16 *)pActwk)[27];
        pNewActwk->userflag.b.h = 2;
    }
    ++pActwk->actfree[14];
    if (pActwk->actfree[14] >= 2) {
        frameout(pActwk);
    }
}

static void ring4set(sprite_status *pActwk) {
    sprite_status *pMaster;
    sprite_status *pNewActwk;
    Sint16 d0;
    char tbl[22] = {-32, 0, 32, 64, 96, 127, 96, 64, 32, 0, -32,
                    -32, 0, 32, 64, 96, 127, 96, 64, 32, 0, -32};

    pMaster = &actwk[((Sint16 *)pActwk)[31]];
    if (pActwk != pMaster) {
        frameout(pActwk);
        return;
    }
    if ((Uint16)pActwk->actfree[14] + 16 < 256) {
        pActwk->actfree[14] += 16;
        return;
    }
    pActwk->actfree[14] += 16;
    if (pActwk->actfree[15] >= 21) {
        frameout(pActwk);
        return;
    }

    if (actwkchk(&pNewActwk) == 0) {
        pNewActwk->actno = 64;
        d0 = pActwk->yposi.w.h;
        d0 -= 384;
        pNewActwk->yposi.w.h = d0;
        d0 = tbl[pActwk->actfree[15]];
        d0 += pActwk->xposi.w.h;
        pNewActwk->xposi.w.h = d0;
    }
    ++pActwk->actfree[15];
}

static void bp1000(sprite_status *pActwk) {
    sprite_status *pMaster;
    sprite_status *pSlave;
    sprite_status *pNewActwk;

    if (pActwk->actfree[14]) {
        frameout(pActwk);
        return;
    }
    pMaster = &actwk[((Sint16 *)pActwk)[31]];
    pSlave = &actwk[((Sint16 *)pMaster)[32]];
    if (pMaster != pSlave) {
        scoreup(100);
        if (actwkchk(&pNewActwk) == 0) {
            pNewActwk->actno = 65;
            pNewActwk->xposi.w.h = pActwk->xposi.w.h;
            pNewActwk->yposi.w.h = pActwk->yposi.w.h;
        }
    }
    ++pActwk->actfree[14];
}

static sprite_pattern ringsp0 = {1, {{-8, -8, 0, 362}}};
static sprite_pattern ringsp1 = {1, {{-8, -8, 0, 363}}};
static sprite_pattern ringsp2 = {1, {{-4, -8, 0, 364}}};
static sprite_pattern ringsp3 = {1, {{-8, -8, 0, 365}}};
static sprite_pattern ringsp4 = {1, {{-8, -8, 0, 366}}};
static sprite_pattern ringsp5 = {1, {{-8, -8, 0, 367}}};
static sprite_pattern ringsp6 = {1, {{-8, -8, 0, 368}}};
static sprite_pattern ringsp7 = {1, {{-8, -8, 0, 369}}};
static sprite_pattern ringsp8;
static sprite_pattern *ringpat[9] = {&ringsp0, &ringsp1, &ringsp2,
                                     &ringsp3, &ringsp4, &ringsp5,
                                     &ringsp6, &ringsp7, &ringsp8};
static Uint8 pchg0[6] = {5, 4, 5, 6, 7, 252};
static Uint8 *ringchg[1] = {pchg0};

void ring4(sprite_status *pActwk) {
    void (*tbl[5])(sprite_status *) = {&ring4_init, &ring4_move, &ring4_get,
                                       &ring4_die, &ring4_erase};

    tbl[pActwk->r_no0 / 2](pActwk);
}

static void ring4_init(sprite_status *pActwk) {

    pActwk->r_no0 += 2;
    pActwk->sprvsize = 8;
    pActwk->sprhs = 8;
    pActwk->patbase = ringpat;
    pActwk->sproffset = 42926;
    pActwk->actflg |= 4;
    pActwk->sprpri = 3;
    pActwk->colino = 71;
    pActwk->sprhsize = 8;
    ((Sint16 *)pActwk)[30] = 2048;
    pActwk->yspeed.w = 256;
    pActwk->actfree[16] = 255;
    pActwk->actfree[17] = 48;

    ring4_move(pActwk);
}

static void ring4_move(sprite_status *pActwk) {
    Sint16 d0, d1;

    speedset2(pActwk);
    pActwk->yspeed.w += 24;

    if (pActwk->actfree[17]) {
        --pActwk->actfree[17];
        if (pActwk->actfree[17])
            goto label1;
    }

    d1 = emycol_d(pActwk);
    if (d1 < 0) {
        pActwk->yposi.w.h += d1;
        d0 = pActwk->yspeed.w;
        d0 >>= 2;
        pActwk->yspeed.w -= d0;
        pActwk->yspeed.w *= -1;
    }
label1:
    d0 = ((Sint16 *)pActwk)[29];
    d0 += ((Sint16 *)pActwk)[30];
    ((Sint16 *)pActwk)[29] = d0;
    d0 >>= 12;
    d0 &= 3;
    pActwk->patno = d0;
    ((Sint16 *)pActwk)[30] -= 8;

    --pActwk->actfree[16];
    if (pActwk->actfree[16] == 0) {
        ring4_erase(pActwk);
        return;
    }
    d0 = scralim_down;
    d0 += 224;
    if (d0 < pActwk->yposi.w.h) {
        ring4_erase(pActwk);
        return;
    }
    actionsub(pActwk);
}

static void ring4_get(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->colino = 0;
    pActwk->sprpri = 1;
    ringgetsub();

    ring4_die(pActwk);
}

static void ring4_die(sprite_status *pActwk) {
    patchg(pActwk, ringchg);
    actionsub(pActwk);
}

static void ring4_erase(sprite_status *pActwk) { frameout(pActwk); }

static sprite_pattern pat00 = {1, {{-8, -4, 0, 484}}};
sprite_pattern *tensu4pat[1] = {&pat00};

void tensu4(sprite_status *pActwk) {
    void (*tbl[2])(sprite_status *) = {&tensu4_init, &tensu4_move};

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
}

static void tensu4_init(sprite_status *pActwk) {
    pActwk->r_no0 += 2;
    pActwk->patbase = tensu4pat;
    pActwk->sproffset = 878;
    pActwk->actflg = 4;
    pActwk->sprpri = 1;
    pActwk->sprhsize = 8;
    pActwk->yspeed.w = -768;

    tensu4_move(pActwk);
}

static void tensu4_move(sprite_status *pActwk) {
    if (pActwk->yspeed.w >= 0) {
        frameout(pActwk);
    } else {
        speedset2(pActwk);
        pActwk->yspeed.w += 24;
    }
}
