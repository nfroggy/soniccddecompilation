#include "../equ.h"
#include "slight7.h"
#include "../action.h"
#include "../actset.h"

static void slight7_init(sprite_status *pActwk);
static void slight7_move(sprite_status *pActwk);
static void type0(sprite_status *pActwk);
static void slight7_pchgsub(sprite_status *pActwk, char *pA2);
static void type1(sprite_status *pActwk);
static void type2(sprite_status *pActwk);
static void type3(sprite_status *pActwk);
static void type4(sprite_status *pActwk);
static void type7(sprite_status *pActwk);

typedef struct {
    Uint8 display_phase;
    Sint16 origin_x;
    Uint8 frame_timer;
    Uint8 anim_index;
    Uint8 anim_count;
} slight7_work;

static slight7_work *slight7_work_get(sprite_status *pActwk) {
    return (slight7_work *)pActwk->actfree;
}

static sprite_pattern slight_pat0 = {1, {{-12, -112, 0, 434}}};
static sprite_pattern slight_pat1 = {1, {{-40, -112, 0, 435}}};
static sprite_pattern slight_pat2 = {2, {{-8, -112, 0, 436}, {-64, 0, 0, 437}}};
static sprite_pattern slight_pat3;
static sprite_pattern slight_pat4 = {1, {{-12, -112, 0, 438}}};
static sprite_pattern slight_pat5 = {1, {{-40, -112, 0, 439}}};
static sprite_pattern slight_pat6 = {2, {{-8, -112, 0, 440}, {-64, 0, 0, 441}}};
sprite_pattern *slight7pat[4] = {&slight_pat0, &slight_pat1, &slight_pat2,
                                 &slight_pat3};
sprite_pattern *slight7patb[4] = {&slight_pat4, &slight_pat5, &slight_pat6,
                                  &slight_pat3};

void slight7(sprite_status *pActwk) {
    slight7_work *work = slight7_work_get(pActwk);
    void (*slight_acttbl[2])(sprite_status *) = {&slight7_init, &slight7_move};

    slight_acttbl[pActwk->r_no0 / 2](pActwk);
    frameout_s00(pActwk, work->origin_x);
}

void slight7_init(sprite_status *pActwk) {
    slight7_work *work = slight7_work_get(pActwk);

    pActwk->r_no0 += 2;
    pActwk->actflg |= 4;
    pActwk->patbase = slight7pat;
    pActwk->sprhsize = 127;
    pActwk->sprvsize = 127;
    work->origin_x = pActwk->xposi.w.h;

    work->display_phase = pActwk->userflag.b.l & 6;

    pActwk->sprpri = 1;
    pActwk->sproffset = 42159;
    if (pActwk->userflag.b.l & 8) {
        pActwk->sprpri = 3;
        pActwk->sproffset = 9391;

        if (time_flag != 1) {
            pActwk->sproffset += 8192;
            pActwk->patbase = slight7patb;
        }
    }
    slight7_move(pActwk);
}

void slight7_move(sprite_status *pActwk) {
    slight7_work *work = slight7_work_get(pActwk);
    Uint8 bD0;

    void (*slight_move_tbl[9])(sprite_status *) = {
        &type0, &type1, &type2, &type3, &type4, &type4, &type4, &type7, &type7};

    pActwk->yposi.w.h = scra_v_posit.w.h + 112;

    slight_move_tbl[pActwk->userflag.b.h](pActwk);

    bD0 = gametimer.w & 6;
    if (pActwk->userflag.b.l & 4) {

        if (work->display_phase == bD0) {
            actionsub(pActwk);
        }
    } else {

        if (work->display_phase == (bD0 & 2)) {
            actionsub(pActwk);
        }
    }
}

void type0(sprite_status *pActwk) {
    slight7_work *work = slight7_work_get(pActwk);
    char type0_pchgtbl[44] = {0,  10, 0,  0,  3,  20, 0,  0,  2,  20, 0,
                              68, 1,  20, 0,  12, 3,  10, 0,  0,  2,  30,
                              1,  -4, 1,  20, 0,  12, 3,  20, 0,  0,  1,
                              10, 1,  64, 3,  20, 0,  0,  2,  20, 1,  -16};

    work->anim_count = 11;
    slight7_pchgsub(pActwk, type0_pchgtbl);
}

void slight7_pchgsub(sprite_status *pActwk, char *pA2) {
    slight7_work *work = slight7_work_get(pActwk);
    Uint8 bD1;

    if (!work->frame_timer) {
        pA2 += work->anim_index << 2;
        pActwk->patno = *pA2++;
        work->frame_timer = *pA2++;
        bD1 = *pA2++;
        pActwk->xposi.w.h = work->origin_x + (Sint16)*pA2++;

        pActwk->actflg &= 254;
        pActwk->cddat &= 254;
        if (bD1) {
            pActwk->actflg |= 1;
            pActwk->cddat |= 1;
        }
    }

    if (--work->frame_timer)
        return;
    if (++work->anim_index >= work->anim_count) {
        work->anim_index = 0;
    }
}

void type1(sprite_status *pActwk) {
    slight7_work *work = slight7_work_get(pActwk);
    char type1_pchgtbl[32] = {0,  30,  0,  0,  1,  30,  0,  48, 2, 30, 0,
                              92, 1,   30, 0,  48, 0,   30, 0,  0, 1,  30,
                              1,  -48, 2,  30, 1,  -92, 1,  30, 1, -48};

    work->anim_count = 8;
    slight7_pchgsub(pActwk, type1_pchgtbl);
}

void type2(sprite_status *pActwk) {
    slight7_work *work = slight7_work_get(pActwk);
    char type2_pchgtbl[16] = {0, 30, 0, 0,  1, 30, 0, 48,
                              2, 30, 0, 92, 1, 30, 0, 48};

    work->anim_count = 4;
    slight7_pchgsub(pActwk, type2_pchgtbl);
}

void type3(sprite_status *pActwk) {
    slight7_work *work = slight7_work_get(pActwk);
    char type3_pchgtbl[16] = {0, 30, 0, 0,   1, 30, 1, -48,
                              2, 30, 1, -92, 1, 30, 1, -48};

    work->anim_count = 4;
    slight7_pchgsub(pActwk, type3_pchgtbl);
}

void type4(sprite_status *pActwk) {
    if (pActwk->userflag.b.h < 4)
        pActwk->patno = 0;
    else
        pActwk->patno = pActwk->userflag.b.h - 4;
}

void type7(sprite_status *pActwk) {
    if (pActwk->userflag.b.h < 6)
        pActwk->patno = 0;
    else
        pActwk->patno = pActwk->userflag.b.h - 6;
    pActwk->actflg |= 1;
    pActwk->cddat |= 1;
}
