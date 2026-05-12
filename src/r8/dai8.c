#include "../equ.h"
#include "dai8.h"
#include "../action.h"
#include "../actset.h"
#include "../playsub.h"
#include "../ridechk.h"

#if defined(R81)
#define SPRITE_DAI8_BASE 475
#elif defined(R82)
#define SPRITE_DAI8_BASE 437
#else
#define SPRITE_DAI8_BASE 433
#endif

static void act_init(sprite_status *actionwk);

static char p00[47] = {0, 5, 5, 5, 5, 5, 4, 4, 4, 5, 5, 5, 5, 5, 4, 4,
                       4, 3, 3, 3, 3, 3, 2, 2, 2, 3, 3, 3, 3, 3, 2, 2,
                       2, 1, 1, 1, 1, 1, 7, 7, 7, 1, 1, 1, 1, 1, 0};
static char p01[152] = {
    0, 1, 1, 1, 1, 1, 7, 7, 7, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 3,
    2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 5, 5, 5, 5, 5, 4, 4, 4, 5, 5, 5,
    5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5,
    5, 4, 4, 4, 5, 5, 5, 5, 5, 4, 4, 4, 3, 3, 3, 3, 3, 2, 2, 2, 3, 3,
    3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 7, 7, 7, 1, 1, 1, 1, 1, 0};
static char *pchg[2] = {p00, p01};
static sprite_pattern pat00 = {1, {{-16, -16, 0, SPRITE_DAI8_BASE}}};
static sprite_pattern pat01 = {1, {{-16, -16, 0, SPRITE_DAI8_BASE + 1}}};
static sprite_pattern pat02 = {1, {{-8, -16, 0, SPRITE_DAI8_BASE + 2}}};
static sprite_pattern pat03 = {1, {{-8, -16, 0, SPRITE_DAI8_BASE + 3}}};
static sprite_pattern pat04 = {1, {{-4, -16, 0, SPRITE_DAI8_BASE + 4}}};
static sprite_pattern pat05 = {1, {{-4, -16, 0, SPRITE_DAI8_BASE + 5}}};
static sprite_pattern patnull = {1, {{0, 0, 0, 0}}};
sprite_pattern *pat_dai8[8] = {&pat00, &pat01, &pat02,   &pat03,
                               &pat04, &pat05, &patnull, &pat00};

void dai8(sprite_status *actionwk) {
    Sint16 ano;

    if (actionwk->userflag.b.h < 0) {
        ano = ((Sint16 *)actionwk)[24];
        if (actwk[ano].actno != 44) {
            frameout(actionwk);
            return;
        }
        if (((Sint16 *)actionwk)[25] != ((Sint16 *)&actwk[ano])[25]) {

            frameout(actionwk);
            return;
        }
        if (((Sint16 *)actionwk)[26] != ((Sint16 *)&actwk[ano])[26]) {

            frameout(actionwk);
            return;
        }
    }

    switch (actionwk->r_no0) {
    case 0:
        act_init(actionwk);
        break;
    case 2:
        act_wait(actionwk);
        break;
    case 4:
        act_appear(actionwk);
        break;
    case 6:
        act_on(actionwk);
        break;
    case 8:
        act_off(actionwk);
        break;
    }
    actionsub(actionwk);
    if (actionwk->userflag.b.h >= 0) {
        frameout_s00(actionwk, ((Sint16 *)actionwk)[25]);
    }
}

static void act_init(sprite_status *actionwk) {
    Sint16 tbl00[2] = {60, 0};
    Sint16 tbl01[5] = {60, 1, 60, 32, 0};
    Sint16 tbl02[8] = {90, 2, 180, 32, 16, 270, 64, 32};
    sprite_status *a1;
    Sint16 *a5tbl, i, d6;

    switch (actionwk->userflag.b.h) {
    case 0:
        a5tbl = tbl00;
        break;
    case 1:
        a5tbl = tbl01;
        break;
    case 2:
        a5tbl = tbl02;
        break;
    }
    ((Sint16 *)actionwk)[23] = *a5tbl++;
    act_init_sub(actionwk, actionwk);

    d6 = *a5tbl++;
    for (i = 0; i < d6; ++i) {
        if (actwkchk(&a1) != 0) {
            frameout(actionwk);
            break;
        }
        a1->actno = actionwk->actno;
        ((Sint16 *)a1)[24] = (Uint16)(actionwk - actwk);
        a1->userflag.b.h = -1;
        ((Sint16 *)a1)[23] = *a5tbl++;
        a1->xposi.w.h = actionwk->xposi.w.h + *a5tbl++;
        a1->yposi.w.h = actionwk->yposi.w.h + *a5tbl++;
        act_init_sub(actionwk, a1);
    }
}

void act_init_sub(sprite_status *actionwk, sprite_status *a6) {
    a6->actflg |= 4;
    a6->sprpri = 3;
    a6->sprhs = 16;
    a6->sprhsize = 16;
    a6->sprvsize = 16;
    a6->sproffset = 920;
    a6->patbase = pat_dai8;
    a6->patno = 6;
    a6->r_no0 = 2;
    ((Sint16 *)a6)[25] = actionwk->xposi.w.h;
    ((Sint16 *)a6)[26] = actionwk->yposi.w.h;
}

void act_wait(sprite_status *actionwk) {
    if (--((Sint16 *)actionwk)[23] != 0)
        return;
    actionwk->r_no0 += 2;
}

void act_appear(sprite_status *actionwk) {
    patchg(actionwk, (Uint8 **)pchg);
    if (actionwk->patno == 0) {
        ((Sint16 *)actionwk)[23] = 120;
        actionwk->r_no0 += 2;
    }
}

void act_on(sprite_status *actionwk) {
    if (ridechk(actionwk, &actwk[0]) != 0)
        actionwk->actfree[21] = 255;
    else
        actionwk->actfree[21] = 0;

    if (--((Sint16 *)actionwk)[23] != 0)
        return;
    actionwk->mstno.w = 511;
    actionwk->r_no0 += 2;
    if (actionwk->actfree[21] != 0) {
        ride_on_clr(actionwk, &actwk[0]);
    }
}

void act_off(sprite_status *actionwk) {
    patchg(actionwk, (Uint8 **)pchg);

    if (actionwk->patno == 0) {
        ((Sint16 *)actionwk)[23] = 120;
        actionwk->r_no0 -= 2;
    }
}
