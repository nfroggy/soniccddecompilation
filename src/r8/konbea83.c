#include "../equ.h"
#include "konbea83.h"
#include "../action.h"
#include "../actset.h"
#include "../ridechk.h"
#include "../playsub.h"

static void a_init(sprite_status *pActwk);
static void a_init_sub(sprite_status *pActwk, sprite_status *pNewact);
static void a_stop(sprite_status *pActwk);
static void a_stop1(sprite_status *pActwk);
static void a_move(sprite_status *pActwk);

static Sint16 a_tbl_00[2] = {60, 0};
static Sint16 a_tbl_01[5] = {60, 1, 60, 32, 0};
static Sint16 *a_tbl0[2] = {a_tbl_00, a_tbl_01};
static Uint8 pchg_00[14] = {1, 1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 0, 255};
static Uint8 *pchg[1] = {pchg_00};
static sprite_pattern pat_00 = {1, {{-16, -8, 0, 507}}};
static sprite_pattern pat_01 = {1, {{-16, -12, 0, 508}}};
static sprite_pattern pat_02 = {1, {{-12, -16, 0, 509}}};
static sprite_pattern pat_03 = {1, {{-8, -16, 0, 510}}};
static sprite_pattern pat_04 = {1, {{-12, -16, 0, 511}}};
static sprite_pattern pat_05 = {1, {{-16, -12, 0, 512}}};
sprite_pattern *pat_konbea[7] = {&pat_00, &pat_01, &pat_02, &pat_03,
                                 &pat_04, &pat_05, &pat_00};
static void (*a_act_tbl[4])(sprite_status *) = {&a_init, &a_stop, &a_stop1,
                                                &a_move};

void konbea(sprite_status *pActwk) {
    sprite_status *pMainwk;

    if (pActwk->userflag.b.l) {
        pMainwk = &actwk[((Sint16 *)pActwk)[23]];

        if (pMainwk->actno != 42) {
            frameout(pActwk);
            return;
        }

        if (((Sint16 *)pActwk)[25] != ((Sint16 *)pMainwk)[25]) {

            frameout(pActwk);
            return;
        }

        if (((Sint16 *)pActwk)[26] != ((Sint16 *)pMainwk)[26]) {

            frameout(pActwk);
            return;
        }
    }

    a_act_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);

    if (!(pActwk->userflag.b.l & 128)) {
        frameout_s00(pActwk, ((Sint16 *)pActwk)[25]);
    }
}

static void a_init(sprite_status *pActwk) {
    Sint16 *pTbl, i;
    sprite_status *pNewact;

    pTbl = a_tbl0[pActwk->userflag.b.l];

    ((Sint16 *)pActwk)[24] = *pTbl++;
    pNewact = pActwk;
    a_init_sub(pActwk, pNewact);

    for (i = *pTbl++ - 1; i >= 0; --i) {

        if (actwkchk(&pNewact) != 0) {
            frameout(pActwk);
            break;
        }

        pNewact->actno = pActwk->actno;
        ((Sint16 *)pNewact)[23] = pActwk - actwk;
        pNewact->userflag.b.h = pActwk->userflag.b.h;
        pNewact->userflag.b.l = -1;
        ((Sint16 *)pNewact)[24] = *pTbl++;
        pNewact->xposi.w.h = pActwk->xposi.w.h + *pTbl++;
        pNewact->yposi.w.h = pActwk->yposi.w.h + *pTbl++;
        a_init_sub(pActwk, pNewact);
    }
}

static void a_init_sub(sprite_status *pActwk, sprite_status *pNewact) {
    pNewact->r_no0 = 2;
    pNewact->actflg |= 4;
    pNewact->sprpri = 3;
    pNewact->sprhs = pNewact->sprhsize = 16;
    pNewact->sprvsize = 5;
    pNewact->sproffset = 844;
    pNewact->patbase = pat_konbea;
    ((Sint16 *)pNewact)[25] = pActwk->xposi.w.h;
    ((Sint16 *)pNewact)[26] = pActwk->yposi.w.h;
}

static void a_stop(sprite_status *pActwk) {
    pActwk->sprvsize = 5;
    ((Sint16 *)pActwk)[24] = 120;
    pActwk->r_no0 += 2;
    a_stop1(pActwk);
}

static void a_stop1(sprite_status *pActwk) {
    if (ridechk(pActwk, &actwk[0])) {
        pActwk->actfree[20] = 255;
    } else {
        pActwk->actfree[20] = 0;
    }

    if (!(--((Sint16 *)pActwk)[24])) {
        pActwk->sprvsize = 16;
        pActwk->mstno.w = 255;
        pActwk->r_no0 += 2;

        if (pActwk->actfree[20]) {
            ride_on_clr(pActwk, &actwk[0]);
        }
    }
}

static void a_move(sprite_status *pActwk) {
    patchg(pActwk, pchg);

    if (!pActwk->patno) {
        pActwk->r_no0 = 2;
    }
}
