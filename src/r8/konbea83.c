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

typedef struct {
    Sint16 parent_index;
    Sint16 segment_timer;
    Sint16 origin_x;
    Sint16 origin_y;
    Uint8 ride_pressed;
} konbea83_work;

static konbea83_work *konbea83_get_work(sprite_status *pActwk) {
    return (konbea83_work *)pActwk->actfree;
}

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
    konbea83_work *work = konbea83_get_work(pActwk);
    sprite_status *pMainwk;

    if (pActwk->userflag.b.l) {
        pMainwk = &actwk[work->parent_index];
        konbea83_work *main_work = konbea83_get_work(pMainwk);

        if (pMainwk->actno != 42) {
            frameout(pActwk);
            return;
        }

        if (work->origin_x != main_work->origin_x) {

            frameout(pActwk);
            return;
        }

        if (work->origin_y != main_work->origin_y) {

            frameout(pActwk);
            return;
        }
    }

    a_act_tbl[pActwk->r_no0 / 2](pActwk);
    actionsub(pActwk);

    if (!(pActwk->userflag.b.l & 128)) {
        frameout_s00(pActwk, work->origin_x);
    }
}

static void a_init(sprite_status *pActwk) {
    Sint16 *pTbl, i;
    sprite_status *pNewact;

    pTbl = a_tbl0[pActwk->userflag.b.l];

    konbea83_get_work(pActwk)->segment_timer = *pTbl++;
    pNewact = pActwk;
    a_init_sub(pActwk, pNewact);

    for (i = *pTbl++ - 1; i >= 0; --i) {

        if (actwkchk(&pNewact) != 0) {
            frameout(pActwk);
            break;
        }

        pNewact->actno = pActwk->actno;
        konbea83_get_work(pNewact)->parent_index = pActwk - actwk;
        pNewact->userflag.b.h = pActwk->userflag.b.h;
        pNewact->userflag.b.l = -1;
        konbea83_get_work(pNewact)->segment_timer = *pTbl++;
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
    konbea83_get_work(pNewact)->origin_x = pActwk->xposi.w.h;
    konbea83_get_work(pNewact)->origin_y = pActwk->yposi.w.h;
}

static void a_stop(sprite_status *pActwk) {
    pActwk->sprvsize = 5;
    konbea83_get_work(pActwk)->segment_timer = 120;
    pActwk->r_no0 += 2;
    a_stop1(pActwk);
}

static void a_stop1(sprite_status *pActwk) {
    konbea83_work *work = konbea83_get_work(pActwk);

    if (ridechk(pActwk, &actwk[0])) {
        work->ride_pressed = 255;
    } else {
        work->ride_pressed = 0;
    }

    if (!(--work->segment_timer)) {
        pActwk->sprvsize = 16;
        pActwk->mstno.w = 255;
        pActwk->r_no0 += 2;

        if (work->ride_pressed) {
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
