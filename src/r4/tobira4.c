#include "../equ.h"
#include "tobira4.h"
#include "../action.h"
#include "../actset.h"
#include "../ridechk.h"

#if defined(R41A)
#define SPRITE_TOBIRA4_BASE 512
#elif defined(R42A)
#define SPRITE_TOBIRA4_BASE 513
#else
#define SPRITE_TOBIRA4_BASE 506
#endif

static sprite_pattern pat0 = {1, {{-8, -64, 0, SPRITE_TOBIRA4_BASE}}};
static sprite_pattern pat1 = {1, {{-8, -32, 0, SPRITE_TOBIRA4_BASE + 1}}};
static sprite_pattern pat2 = {1, {{-64, -8, 0, SPRITE_TOBIRA4_BASE + 2}}};
sprite_pattern *tobira4pat[3] = {&pat0, &pat1, &pat2};

void tobira4(sprite_status *pActwk) {
    void (*tbl[4])(sprite_status *) = {&tobira4_init, &tobira4_move,
                                       &tobira4_chek, &tobira4_clse};

    tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);
    frameout_s(pActwk);
}

void tobira4_init(sprite_status *pActwk) {
    Uint8 d0, d1;
    Uint8 tbl[20] = {64, 8, 128, 2,  64,  8, 128, 2,  8,   32,
                     64, 1, 8,   64, 128, 0, 8,   64, 128, 0};

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sproffset = 17472;
    pActwk->sprpri = 2;
    pActwk->patbase = tobira4pat;
    ((Sint16 *)pActwk)[29] = pActwk->xposi.w.h;
    ((Sint16 *)pActwk)[27] = pActwk->yposi.w.h;

    d1 = d0 = pActwk->userflag.b.h;
    d1 &= 15;
    pActwk->actfree[6] = d1;
    d0 >>= 4;
    d0 &= 15;
    pActwk->actfree[7] = d0;
    d0 <<= 2;
    pActwk->sprhsize = tbl[d0];
    pActwk->sprvsize = tbl[d0 + 1];
    pActwk->actfree[17] = tbl[d0 + 2];
    pActwk->patno = tbl[d0 + 3];
}

void tobira4_move(sprite_status *pActwk) {
    Uint8 d0;

    d0 = pActwk->actfree[6];
    if (!(switchflag[d0] & 128)) {
        pActwk->actfree[18] = 255;
    } else {
        pActwk->actfree[18] = 0;
    }

    if (pActwk->actfree[7] != 4) {
        if (pActwk->actfree[16]) {
            pActwk->actfree[18] = 0;
        }
    }
    ((Sint16 *)pActwk)[30] = actwk[0].xposi.w.h;
    ((Sint16 *)pActwk)[33] = actwk[0].yposi.w.h;
    tobira4_cnt(pActwk);
    hitchk(pActwk, &actwk[0]);

    if (pActwk->actfree[7] != 4) {
        if (pActwk->actfree[16] == pActwk->actfree[17]) {
            pActwk->r_no0 += 2;
        }
    }
}

void cl_type1(sprite_status *pActwk) {
    sprite_status *pPlayerwk;
    Sint16 d0;

    pPlayerwk = &actwk[0];
    d0 = pActwk->yposi.w.h;
    if (d0 < ((Sint16 *)pActwk)[33]) {
        d0 = pPlayerwk->sprvsize;
        d0 += pPlayerwk->yposi.w.h;
        if (d0 >= pActwk->yposi.w.h)
            return;
        d0 -= pActwk->yposi.w.h;
        d0 *= -1;
        if (d0 < 16)
            return;
    } else {
        d0 = -pPlayerwk->sprvsize;
        d0 += pPlayerwk->yposi.w.h;
        if (d0 < pPlayerwk->yposi.w.h)
            return;
        d0 -= pPlayerwk->yposi.w.h;
        if (d0 < 8)
            return;
    }
    pActwk->r_no0 += 2;
}

void cl_type2(sprite_status *pActwk) {
    sprite_status *pPlayerwk;
    Sint16 d0;

    pPlayerwk = &actwk[0];
    d0 = pActwk->xposi.w.h;
    if (d0 < ((Sint16 *)pActwk)[30]) {
        d0 = pPlayerwk->sprhs;
        d0 += pPlayerwk->xposi.w.h;
        if (d0 >= pActwk->xposi.w.h)
            return;
        d0 -= pActwk->xposi.w.h;
        d0 *= -1;
        if (d0 < 8)
            return;
    } else {
        d0 = -pPlayerwk->sprhs;
        d0 += pPlayerwk->xposi.w.h;
        if (d0 < pActwk->xposi.w.h)
            return;
        d0 -= pActwk->xposi.w.h;
        if (d0 < 8)
            return;
    }
    pActwk->r_no0 += 2;
}

void tobira4_chek(sprite_status *pActwk) {
    void (*tbl[5])(sprite_status *) = {&cl_type1, &cl_type1, &cl_type2,
                                       &cl_type2, &cl_type2};

    tbl[pActwk->actfree[7]](pActwk);
    hitchk(pActwk, &actwk[0]);
}

void tobira4_clse(sprite_status *pActwk) {
    pActwk->actfree[18] = 255;
    tobira4_cnt(pActwk);
    if (pActwk->actfree[16] == 0) {
        pActwk->r_no0 = 2;
    }
    hitchk(pActwk, &actwk[0]);
}

void type1(sprite_status *pActwk) {
    Sint16 d0;

    tobira4_sub(pActwk);
    d0 = (Uint16)pActwk->actfree[16];
    d0 *= -1;
    d0 += ((Sint16 *)pActwk)[29];
    pActwk->xposi.w.h = d0;
}

void type2(sprite_status *pActwk) {
    Sint16 d0;

    tobira4_sub(pActwk);
    d0 = (Uint16)pActwk->actfree[16];
    d0 += ((Sint16 *)pActwk)[29];
    pActwk->xposi.w.h = d0;
}

void type3(sprite_status *pActwk) {
    Sint16 d0;

    tobira4_sub(pActwk);
    d0 = (Uint16)pActwk->actfree[16];
    d0 *= -1;
    d0 += ((Sint16 *)pActwk)[27];
    pActwk->yposi.w.h = d0;
}

void type4(sprite_status *pActwk) {
    Sint16 d0;

    tobira4_sub(pActwk);
    d0 = (Uint16)pActwk->actfree[16];
    d0 += ((Sint16 *)pActwk)[27];
    pActwk->yposi.w.h = d0;
}

void tobira4_cnt(sprite_status *pActwk) {
    void (*tbl[5])(sprite_status *) = {&type1, &type2, &type3, &type4, &type3};

    tbl[pActwk->actfree[7]](pActwk);
}

void tobira4_sub(sprite_status *pActwk) {
    if (pActwk->actfree[18]) {
        if (pActwk->actfree[16] >= 8) {
            pActwk->actfree[16] -= 8;
            return;
        }
        pActwk->actfree[16] = 0;
    } else {
        pActwk->actfree[16] += 8;
        if (pActwk->actfree[16] < pActwk->actfree[17])
            return;

        pActwk->actfree[16] = pActwk->actfree[17];
    }
}
