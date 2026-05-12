#include "../equ.h"
#include "buranko5.h"
#include "../action.h"
#include "../actset.h"
#include "../etc.h"
#include "../ridechk.h"

static void c_init(sprite_status *pActwk);
static void c_move(sprite_status *pActwk);
static void middle(sprite_status *pActwk);
static void radius(sprite_status *pActwk);

static sprite_pattern pat_buranko5_00 = {1, {{-8, -8, 0, 403}}};
static sprite_pattern pat_buranko5_01 = {1, {{-8, -8, 0, 404}}};
static sprite_pattern pat_buranko5_02 = {1, {{-32, -8, 0, 405}}};
sprite_pattern *pat_buranko5[3] = {&pat_buranko5_00, &pat_buranko5_01,
                                   &pat_buranko5_02};

void buranko5(sprite_status *pActwk) {
    if (!pActwk->userflag.b.l) {

        if (pActwk->r_no0) {
            c_move(pActwk);
            return;
        }
        c_init(pActwk);
    } else {
        if (pActwk->userflag.b.l & 128) {

            radius(pActwk);
        } else {

            middle(pActwk);
        }
    }
}

static void c_init(sprite_status *pActwk) {
    sprite_status *pNewactwk;
    Sint16 wD6;
    Sint32 i;

    Sint16 tbl0[6] = {5, 0, 5, -32768, 7, 16384};

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->sprhs = 8;
    pActwk->sprhsize = 8;
    pActwk->sprvsize = 8;
    pActwk->sprpri = 3;
    pActwk->sproffset = 856;
    pActwk->patbase = pat_buranko5;
    ((Sint16 *)pActwk)[29] = tbl0[pActwk->userflag.b.h & 254];
    ((Sint16 *)pActwk)[25] = tbl0[(pActwk->userflag.b.h & 254) + 1];
    ((Sint16 *)pActwk)[27] = 256;
    if (pActwk->userflag.b.h & 1) {
        *(Sint32 *)&pActwk->actfree[4] *= -1;
        *(Sint32 *)&pActwk->actfree[8] *= -1;
    }

    wD6 = ((Sint16 *)pActwk)[29];
    for (i = 0; wD6 >= 0; --wD6, ++i) {

        if (actwkchk2(pActwk, &pNewactwk) != 0) {
            frameout(pActwk);
            return;
        }
        pActwk->actfree[i + 14] = pNewactwk - actwk;
        pNewactwk->userflag.b.l = 1;
        ((Sint16 *)pNewactwk)[23] = pActwk - actwk;
        pNewactwk->actno = pActwk->actno;
        pNewactwk->xposi.w.h = pActwk->xposi.w.h;
        pNewactwk->yposi.w.h = pActwk->yposi.w.h;
        pNewactwk->actflg = pActwk->actflg;
        pNewactwk->patbase = pActwk->patbase;
    }
    pNewactwk->userflag.b.l = -1;
}

static void c_move(sprite_status *pActwk) {
    sprite_status *pMainactwk;
    Sint32 lD3, lD4, lD5, lD6;
    int_union lD0, lD1;
    Sint16 wD6;
    Uint16 Sin, Cos;
    Sint32 i;

    lD6 = *(Sint32 *)&pActwk->actfree[0] + *(Sint32 *)&pActwk->actfree[8];

    if ((lD5 = *(Sint32 *)&pActwk->actfree[4]) != 0) {
        lD1.l = lD6;
        if (lD0.l = lD5, lD5 < 0) {
            lD1.l = -lD1.l;
            lD0.l = -lD0.l;
        }

        if (lD1.l < 0) {
            lD6 = 0;
            *(Sint32 *)&pActwk->actfree[8] *= -1;
        } else {
            if (lD1.l > lD0.l) {
                lD6 = lD5;
                *(Sint32 *)&pActwk->actfree[8] *= -1;
            }
        }
    }
    *(Sint32 *)&pActwk->actfree[0] = lD6;

    wD6 = ((Sint16 *)pActwk)[29] - 1;
    lD5 = pActwk->xposi.l;
    lD4 = pActwk->yposi.l;
    lD1.l = 0;
    lD0.l = 0;
    sinset(pActwk->actfree[1], (Sint16 *)&Sin, (Sint16 *)&Cos);
    lD1.w.h = Cos;
    lD0.w.h = Sin;
    lD1.l /= 16;
    lD0.l /= 16;

    for (i = 0; wD6 >= 0; --wD6) {

        pMainactwk = &actwk[pActwk->actfree[i++ + 14]];
        pMainactwk->xposi.l = lD5 += lD1.l;
        pMainactwk->yposi.l = lD4 += lD0.l;
    }
    pMainactwk = &actwk[pActwk->actfree[i + 14]];
    lD3 = lD5 + lD1.l, pMainactwk->xposi.l = lD3;
    pMainactwk->xposi.l = lD5 + lD1.l;
    pMainactwk->yposi.l = lD4 + lD0.l;
    pMainactwk->xspeed.w = (Sint16)(lD5 - lD3) >> 8;
    actionsub(pActwk);
    frameout_s(pActwk);
}

static void middle(sprite_status *pActwk) {
    if (pActwk->r_no0) {

        if (actwk[pActwk->actfree[0]].actno == 42)
            actionsub(pActwk);
        else
            frameout(pActwk);
        return;
    }

    pActwk->r_no0 += 2;
    pActwk->patno = 1;
    pActwk->sprhs = 8;
    pActwk->sprhsize = 8;
    pActwk->sprvsize = 8;
    pActwk->sprpri = 3;
    pActwk->sproffset = 856;
}

static void radius(sprite_status *pActwk) {
    Sint16 wSp, wD0;
    Uint8 bD0;

    if (!pActwk->r_no0) {

        pActwk->r_no0 += 2;
        pActwk->patno = 2;
        pActwk->sprhs = 32;
        pActwk->sprhsize = 32;
        pActwk->sprvsize = 8;
        pActwk->sprpri = 2;
        pActwk->sproffset = 17180;
    }

    wSp = pActwk->xspeed.w;
    pActwk->xspeed.w = 0;
    pActwk->sprvsize = 10;
    if (ridechk(pActwk, &actwk[0]))
        bD0 = 255;
    else
        bD0 = 0;
    pActwk->xspeed.w = wSp;
    pActwk->sprvsize = 8;
    if (bD0) {
        if (ridechk(pActwk, &actwk[0])) {

            wD0 = 2;
            if (colrevflag) {
                wD0 *= -1;
            }
            actwk[0].xposi.w.h += wD0;
        }
    }

    if (actwk[pActwk->actfree[0]].actno == 42)
        actionsub(pActwk);
    else
        frameout(pActwk);
}
