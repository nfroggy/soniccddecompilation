#include "../equ.h"
#include "stopper.h"
#include "../action.h"
#include "../playsub.h"
#include "../ridechk.h"

static void act_init(sprite_status *actionwk);
static void act_move(sprite_status *actionwk);

static sprite_pattern patstop1 = {1, {{-16, -8, 0, 567}}};
static sprite_pattern patstop2 = {1, {{-16, -8, 0, 568}}};
static sprite_pattern patstop3 = {1, {{-16, -8, 0, 569}}};
sprite_pattern *stopper_pat[3] = {&patstop1, &patstop2, &patstop3};
static char pchg0[5] = {9, 0, 1, 2, -4};
static char pchg1[5] = {9, 2, 1, 0, -4};
static char *pchg[2] = {pchg0, pchg1};

typedef struct {
    Uint16 parent_actor;
} stopper_work;

static stopper_work *stopper_work_get(sprite_status *actionwk) {
    return (stopper_work *)actionwk->actfree;
}

void stopper(sprite_status *actionwk) {
    stopper_work *work = stopper_work_get(actionwk);
    sprite_status *a1;

    switch (actionwk->r_no0) {
    case 0:
        act_init(actionwk);
    case 2:
        act_move(actionwk);
        break;
    case 4:
        act_open(actionwk);
        break;
    case 6:
        act_close(actionwk);
        break;
    case 8:
        act_move1(actionwk);
        break;
    case 10:
        act_move(actionwk);
        break;
    }

    a1 = &actwk[work->parent_actor];
    if (a1->actno != 54) {
        frameout(actionwk);
        return;
    }
    actionsub(actionwk);
}

static void act_init(sprite_status *actionwk) {
    actionwk->r_no0 += 2;
    actionwk->actflg |= 4;
    actionwk->sprpri = 3;
    actionwk->sproffset = 1086;
    actionwk->patbase = stopper_pat;
    actionwk->sprhsize = 16;
    actionwk->sprvsize = 8;
}

static void act_move(sprite_status *actionwk) {
    sprite_status *a1;

    if (actionwk->patno != 2)
        return;
    a1 = &actwk[0];
    if (ridechk(actionwk, a1) == 0)
        return;
    ;
    a1->cddat &= 247;
    a1->cddat |= 2;
    a1->cddat &= 239;
    a1->cddat &= 223;
    a1->xspeed.w = 0;
    a1->yspeed.w = -1024;
}

void act_open(sprite_status *actionwk) {
    actionwk->mstno.w = 255;
    actionwk->r_no0 += 4;
    act_move1(actionwk);
}

void act_close(sprite_status *actionwk) {
    actionwk->mstno.w = 511;
    actionwk->r_no0 += 2;
    act_move1(actionwk);
}

void act_move1(sprite_status *actionwk) { patchg(actionwk, (Uint8 **)pchg); }
